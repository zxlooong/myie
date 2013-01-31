// MyIENSHandle.cpp : Implementation of CMyIEApp and DLL registration.

#include "stdafx.h"
#include "MyIE.h"
#include "MyIENSHandle.h"
#include "afxmt.h"
#include "MainFrm.h"

/////////////////////////////////////////////////////////////////////////////
//
BYTE bytePlaceHold[]=	{0x47, 0x49, 0x46, 0x38, 0x39, 0x61, 0x01, 0x00,
						0x01, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
						0xFF, 0xFF, 0xFF, 0x21, 0xF9, 0x04, 0x01, 0x00,
						0x00, 0x00, 0x00, 0x2C, 0x00, 0x00, 0x00, 0x00,
						0x01, 0x00, 0x01, 0x00, 0x00, 0x02, 0x01, 0x44,
						0x00, 0x3B};


CMapStringToPtr mapUrlFilter;
CStringArray aUrlPattern;

CMyIENSHandle::CMyIENSHandle()
{
}


STDMETHODIMP CMyIENSHandle::Start(
        LPCWSTR szUrl,
        IInternetProtocolSink *pIProtSink,
        IInternetBindInfo *pIBindInfo,
        DWORD grfSTI,
        DWORD dwReserved)
{
	// Should we do the following or always fail? How can we continue if parsing failed?
	try{

	if(pMainFrame->m_bUseUrlFilter && !pMainFrame->m_bUrlFilterDirty)
	{
		CString strUrl(szUrl); void* tmp;
		strUrl.MakeLower();
		BOOL bFilter = FALSE;
		if(mapUrlFilter.Lookup(strUrl, tmp))
			bFilter = TRUE;
		else if(PatternFilter(strUrl))
			bFilter = TRUE;
		if(bFilter)
		{
			if(pIProtSink!=NULL)
			{
				m_pProtSink = pIProtSink;
				m_pProtSink->AddRef();
				m_pProtSink->ReportData(BSCF_FIRSTDATANOTIFICATION || BSCF_LASTDATANOTIFICATION || BSCF_DATAFULLYAVAILABLE, 42, 42);
				m_pProtSink->ReportResult(S_OK, 0, 0);
				m_pProtSink->Release();
				m_pProtSink = NULL;
			}
			return S_OK;
		}
	}

	}catch(...)
	{
	}

	return INET_E_USE_DEFAULT_PROTOCOLHANDLER;
}

STDMETHODIMP CMyIENSHandle::Continue(PROTOCOLDATA *pStateInfo)
{
	return S_OK;
}

STDMETHODIMP CMyIENSHandle::Abort(HRESULT hrReason,DWORD dwOptions)
{
	return E_NOTIMPL;
}

STDMETHODIMP CMyIENSHandle::Terminate(DWORD dwOptions)
{
	return  S_OK;
}

STDMETHODIMP CMyIENSHandle::Suspend()
{
ATLTRACENOTIMPL("Suspend\n");

	return E_NOTIMPL;
}

STDMETHODIMP CMyIENSHandle::Resume()
{
ATLTRACENOTIMPL("Resume\n");
	
	return E_NOTIMPL;
}

// IInternetProtocol::Read
// -----------------------

// This is how URLMON asks us for data. As we report data on the protocol sink,
// URLMON will periodically ask us for data via this method.

// How URLMON asks for data will not always correlate directly to what data is
// reported.

// Do not expect to see 1 and only 1 Read method for each time data is reported
// to the protocol sink (give URLMON what it asks for if you can.)

// Expect that extra read calls may come in after all the data in the buffer has
// been read (continue to return S_FALSE.)

// The amount requested may sometimes be much more than what was reported. URLMON
// typically looks for data in 4k buffers, but do not depend on this.

STDMETHODIMP CMyIENSHandle::Read(void *pv, ULONG cb, ULONG *pcbRead)
{
	try{

	if(cb>=42 && pv!=NULL)
	{
		memcpy(pv, bytePlaceHold, 42);
		*pcbRead =	42;
	}
	else
		*pcbRead = 0;
	}
	catch(...)
	{
	}

	return S_FALSE;
}

STDMETHODIMP CMyIENSHandle::Seek(
        LARGE_INTEGER dlibMove,
        DWORD dwOrigin,
        ULARGE_INTEGER *plibNewPosition)
{
ATLTRACENOTIMPL("Seek\n");
	
	return E_NOTIMPL;
}

STDMETHODIMP CMyIENSHandle::LockRequest(DWORD dwOptions)
{
ATLTRACE(_T("LockRequest\n"));


	return S_OK;
}

STDMETHODIMP CMyIENSHandle::UnlockRequest()
{
ATLTRACE(_T("UnlockRequest\n"));

	return S_OK;
}


STDMETHODIMP CMyIENSHandle::CombineUrl(LPCWSTR pwzBaseUrl, LPCWSTR pwzRelativeUrl, DWORD dwCombineFlags,
	LPWSTR pwzResult, DWORD cchResult, DWORD *pcchResult, DWORD dwReserved)
{
ATLTRACE(_T("CombineURL\n"));

	return INET_E_DEFAULT_ACTION;
}

STDMETHODIMP CMyIENSHandle::CompareUrl(LPCWSTR pwzUrl1, LPCWSTR pwzUrl2, DWORD dwCompareFlags)
{
ATLTRACE(_T("CompareUrl\n"));

	// Ignoring dwCompareFlags
	if (0 == wcscmp(pwzUrl1, pwzUrl2))
		return S_OK;
	else
		return S_FALSE;
}

// IInternetProtocolInfo::ParseUrl
// -------------------------------
//
// Unlike the other methods of IIPI, ParseUrl is worthy of note:
//
// 1. ParseUrl is used in many HTML-based security checks. If you seem to be having a security
//    problem with your own protocol, it is probably due to your ParseUrl implementation.
//
// 2. ParseUrl is called many many times, even when the protocol is not doing a bind. Be aware
//    that this is used as a "general utility" function by IE for parsing and checking on URLs.
//    For example, mousing over a hyperlink to a X-DB2XML URL will cause MSHTML to call
//    CoInternetParseUrl 2 or more times. Every call will cocreate a new instance of this protocol
//    handler object, call IIPI::ParseUrl, and destroy the protocol handler instance.

// Our protocol handler "cheats". We pretend that the URL we were passed as the UDL file path is
// actually our URL. This allows us to easily map ourselves to the HTTP(S) protocol so that HTML
// pages served by HTTP(S) don't continually run into security problems with our protocol. Normally
// it is Against The Rules for web pages to access data across domains or between different protocols.

STDMETHODIMP CMyIENSHandle::ParseUrl(LPCWSTR pwzUrl, PARSEACTION ParseAction, DWORD dwParseFlags,
	LPWSTR pwzResult, DWORD cchResult, DWORD *pcchResult, DWORD dwReserved)
{
ATLTRACE(_T("ParseUrl\n"));

//	HRESULT hr = CoInternetParseUrl(m_url, ParseAction, dwParseFlags, pwzResult, dwUrlLen, pcchResult, dwReserved);

	return INET_E_DEFAULT_ACTION;
}

// TODO: Implement queryinfo against xml result set... not UDL file
STDMETHODIMP CMyIENSHandle::QueryInfo( LPCWSTR pwzUrl, QUERYOPTION QueryOption, DWORD dwQueryFlags,
	LPVOID pBuffer, DWORD cbBuffer, DWORD *pcbBuf, DWORD dwReserved)
{
ATLTRACE(_T("QueryInfo\n"));

	return INET_E_DEFAULT_ACTION;
}



inline BOOL CMyIENSHandle::PatternFilter(CString & url)
{
	CString str, tmp, tmp2;

	try{

	int i = 0, i2=0, start = 0, start2 = 0;//start - str, start2 - tmp
	BOOL cmode = TRUE; //compare mode;
	BOOL isame = FALSE;
	BOOL final = FALSE;
	int p = 0;
	while(p<=aUrlPattern.GetUpperBound() && !isame)
	{
		str = aUrlPattern.GetAt(p);
		//star filter
		i = 0; i2 = 0; start = 0; start2 = 0;
		isame = TRUE;cmode = TRUE;final = FALSE;
		while(start<str.GetLength() && isame)
		{
			//get the string before *
			i=str.Find('*', start);
			if(i<0)
			{
				tmp2 = str.Mid(start);
				i=str.GetLength();
				final = TRUE;
			}
			else
				tmp2 = str.Mid(start,i-start);
			if(tmp2!="")
			{
				if(cmode)
				{
					tmp = url.Mid(start2, i-start);
					if(tmp != tmp2)
						isame = FALSE;
				}
				else if(final)
				{
					tmp = url.Right(tmp2.GetLength());
					if(tmp != tmp2)
						isame = FALSE;
				}
				else
				{
					i2=url.Find(tmp2, start2);
					if( i2<0)
						isame = FALSE;
				}
			}
			cmode = FALSE;
			start = i+1;
			start2 = i2 + tmp2.GetLength();
		}
		p++;
	}

	return isame;
	}
/*	catch(CMemoryException * e)
	{
		if(e!=NULL)e->Delete();
//		AfxMessageBox("statfilter error");
		return FALSE;
	}*/
	catch(...)
	{
		return FALSE;
	}
}
