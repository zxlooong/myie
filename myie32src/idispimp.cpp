/*
 * idispimp.CPP
 * IDispatch for Extending Dynamic HTML Object Model
 *
 * Copyright (c)1995-1999 Microsoft Corporation, All Rights Reserved
 */

#include "stdafx.h"
#include "idispimp.h"
#include "myie.h"
#include "MainFrm.h"
#include "resource.h"       // main symbols
#include "SelProxyCatDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Hardcoded information for extending the Object Model 
// Typically this would be supplied through a TypeInfo
// In this case the name "xxyyzz" maps to DISPID_Extend 
const	WCHAR pszAddFavorite[]=L"addFavorite";
const   WCHAR pszAddProxy[]=L"addProxy";
#define DISPID_ADDFAVORITE 20
#define DISPID_ADDPROXY 21



/*
 * CImpIDispatch::CImpIDispatch
 * CImpIDispatch::~CImpIDispatch
 *
 * Parameters (Constructor):
 *  pSite           PCSite of the site we're in.
 *  pUnkOuter       LPUNKNOWN to which we delegate.
 */

CImpIDispatch::CImpIDispatch( void )
{
    m_cRef = 0;
}

CImpIDispatch::~CImpIDispatch( void )
{
	ASSERT( m_cRef == 0 );
}


/*
 * CImpIDispatch::QueryInterface
 * CImpIDispatch::AddRef
 * CImpIDispatch::Release
 *
 * Purpose:
 *  IUnknown members for CImpIDispatch object.
 */

STDMETHODIMP CImpIDispatch::QueryInterface( REFIID riid, void **ppv )
{
    *ppv = NULL;

	try{

    if ( IID_IDispatch == riid )
	{
        *ppv = this;
	}
	
	if ( NULL != *ppv )
    {
        ((LPUNKNOWN)*ppv)->AddRef();
        return NOERROR;
    }
	}catch(...)
	{
	}

	return E_NOINTERFACE;
}


STDMETHODIMP_(ULONG) CImpIDispatch::AddRef(void)
{
    return ++m_cRef;
}

STDMETHODIMP_(ULONG) CImpIDispatch::Release(void)
{
    return --m_cRef;
}


//IDispatch
STDMETHODIMP CImpIDispatch::GetTypeInfoCount(UINT* /*pctinfo*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP CImpIDispatch::GetTypeInfo(/* [in] */ UINT /*iTInfo*/,
            /* [in] */ LCID /*lcid*/,
            /* [out] */ ITypeInfo** /*ppTInfo*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP CImpIDispatch::GetIDsOfNames(
            /* [in] */ REFIID riid,
            /* [size_is][in] */ OLECHAR** rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID* rgDispId)
{
	HRESULT hr;
	UINT	i;

	// Assume some degree of success
	hr = NOERROR;

	try{
	// Hardcoded mapping for this sample
	// A more usual procedure would be to use a TypeInfo
	for ( i=0; i < cNames; i++)
	{
/*		if (  2 == CompareString( lcid, NORM_IGNOREWIDTH, (char*)pszAddFavorite, 22, (char*)rgszNames[i], 22 ) )
		{				AfxMessageBox("dd13");
			rgDispId[i] = DISPID_ADDFAVORITE;
		}
		else if (  2 == CompareString( lcid, NORM_IGNOREWIDTH, (char*)pszAddProxy, 16, (char*)rgszNames[i], 16 ) )
		{				AfxMessageBox("dd11");

			rgDispId[i] = DISPID_ADDPROXY;
		}*/
		if(wcsicmp(pszAddFavorite, rgszNames[i])==0)
		{
			rgDispId[i] = DISPID_ADDFAVORITE;
		}
		else if(wcscmp(pszAddProxy, rgszNames[i])==0)
		{
			rgDispId[i] = DISPID_ADDPROXY;
		}
		else
		{
			// One or more are unknown so set the return code accordingly
			hr = ResultFromScode(DISP_E_UNKNOWNNAME);
			rgDispId[i] = DISPID_UNKNOWN;
		}
	}

	}catch(...)
	{
	}
	return hr;
}

STDMETHODIMP CImpIDispatch::Invoke(
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID /*riid*/,
            /* [in] */ LCID /*lcid*/,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS* pDispParams,
            /* [out] */ VARIANT* pVarResult,
            /* [out] */ EXCEPINFO* /*pExcepInfo*/,
            /* [out] */ UINT* puArgErr)
{

	if ( pMainFrame!=NULL)
	{
		try
		{
			switch (dispIdMember)
			{
			case DISPID_ADDFAVORITE:
				if(pDispParams->cArgs == 2)
				   pMainFrame->AddtoFavorite(pDispParams->rgvarg[1].bstrVal, &pDispParams->rgvarg[0]);
				else if(pDispParams->cArgs == 1)
				   pMainFrame->AddtoFavorite(pDispParams->rgvarg[0].bstrVal, &pDispParams->rgvarg[0]);
				break;
			case DISPID_ADDPROXY:
				if(pDispParams->cArgs == 4)
				{
					int webproxy =pDispParams->rgvarg[0].iVal;
					int speed = pDispParams->rgvarg[1].iVal;
					CString proxy(pDispParams->rgvarg[2].bstrVal);
					CString proxyname(pDispParams->rgvarg[3].bstrVal);
					if(webproxy > 0)
					{
						bProxyDirty = TRUE;
						pMainFrame->m_bSaveConfig = TRUE;

						CProxyList * pl;
						if(webproxy == 1)
							pl = pMainFrame->m_aProxyCategories.GetWebProxyList();
						else
							pl = pMainFrame->m_aProxyCategories.GetWebProxyList(TRUE);
						if(pl!=NULL && pl->Find(proxy)==-1)
						{
							pl->Add(proxy, proxyname, speed);
							AfxMessageBox(IDS_PROXY_ADDED);
						}

						CMenu* pMenu = pMainFrame->GetMenu()->GetSubMenu(5)->GetSubMenu(2);
						pMainFrame->BuildWebProxyMenu(pMenu, 1); //translate
						pMenu = pMainFrame->GetMenu()->GetSubMenu(5)->GetSubMenu(3);
						pMainFrame->BuildWebProxyMenu(pMenu, 0); //web proxy

					}
					else
					{
						CSelProxyCatDlg dlg;
						dlg.m_bClear = FALSE;
						dlg.m_bDisableClear = TRUE;
						if(dlg.DoModal()==IDOK)
						{
							CProxyList * pl=NULL;
							int i=0;
							for(int l = 0;l <= pMainFrame->m_aProxyCategories.m_ProxyCategory.GetUpperBound();l++)
							{
								pl = pMainFrame->m_aProxyCategories.m_ProxyCategory.GetAt(l);
								if(!pl->m_bIsWebProxy)
								{
									if(i == dlg.m_SelCates)
										break;
									i++;
								}
							}
							if(pl!=NULL && pl->Find(proxy)==-1)
							{
								pl->Add(proxy, proxyname, speed);
								//pMainFrame->m_astrProxy.AddTail(proxy);
								//pMainFrame->m_aProxySpeed.AddTail(speed);
								//pMainFrame->m_astrProxyName.AddTail(proxyname);
								AfxMessageBox(IDS_PROXY_ADDED);

								bProxyDirty = TRUE;
								pMainFrame->m_bSaveConfig = TRUE;

							}
						}
					}

				}
			}
		}
		catch(...)
		{
		}
	}

	return S_OK;
}

//DropTarget
CImpIDropTarget::CImpIDropTarget( void )
{
    m_cRef = 0;
}

CImpIDropTarget::~CImpIDropTarget( void )
{
	ASSERT( m_cRef == 0 );
}


/*
 * CImpIDropTarget::QueryInterface
 * CImpIDropTarget::AddRef
 * CImpIDropTarget::Release
 *
 * Purpose:
 *  IUnknown members for CImpIDispatch object.
 */

STDMETHODIMP CImpIDropTarget::QueryInterface( REFIID riid, void **ppv )
{
    *ppv = NULL;

	try{

    if ( IID_IDropTarget == riid )
	{
        *ppv = this;
	}
	
	if ( NULL != *ppv )
    {
        ((LPUNKNOWN)*ppv)->AddRef();
        return NOERROR;
    }
	}catch(...)
	{
	}

	return E_NOINTERFACE;
}


STDMETHODIMP_(ULONG) CImpIDropTarget::AddRef(void)
{
    return ++m_cRef;
}

STDMETHODIMP_(ULONG) CImpIDropTarget::Release(void)
{
    return --m_cRef;
}


STDMETHODIMP CImpIDropTarget::DragEnter(/* [unique][in] */IDataObject __RPC_FAR *pDataObj,
            /* [in] */ DWORD grfKeyState,
            /* [in] */ POINTL pt,
            /* [out][in] */ DWORD __RPC_FAR *pdwEffect)
{
//    if (!pDataObject->IsDataAvailable(CF_TEXT)) 
 //       return DROPEFFECT_NONE;

/*	if(m_hFromWnd==NULL)
	{
		POINT p; p.x = pt.x; p.y = pt.y;
		m_hFromWnd = WindowFromPoint(p);
	}*/
	*pdwEffect = DROPEFFECT_LINK;
   
//    if (dwKeyState & MK_CONTROL)
 //       dwEffect=DROPEFFECT_COPY;
 //   else
 //       dwEffect=DROPEFFECT_MOVE;

    //we set focus to current window such that the caret will be shown
 //   ms_pCtl->SetFocus();

    return S_OK;
}

STDMETHODIMP CImpIDropTarget::DragLeave()
{
//	m_hFromWnd = NULL;
	return S_OK;
}

STDMETHODIMP CImpIDropTarget::DragOver(/* [in] */ DWORD grfKeyState,
            /* [in] */ POINTL pt,
            /* [out][in] */ DWORD __RPC_FAR *pdwEffect)
{

    
		*pdwEffect=DROPEFFECT_LINK;
	

//    if (dwKeyState & MK_CONTROL)
  //      dwEffect=DROPEFFECT_COPY;
 //   else
 //       dwEffect=DROPEFFECT_MOVE;

 //   ms_pCtl->DrawCaretByCursor();

	return S_OK;
}

STDMETHODIMP CImpIDropTarget::Drop(/* [unique][in] */ IDataObject __RPC_FAR *pDataObj,
            /* [in] */ DWORD grfKeyState,
            /* [in] */ POINTL pt,
            /* [out][in] */ DWORD __RPC_FAR *pdwEffect)
{

	try{


	HGLOBAL hData = NULL;
	CString str2;

	COleDataObject DataObj;
	DataObj.Attach(pDataObj);
	hData = DataObj.GetGlobalData(CF_TEXT);
	DataObj.Detach();

    if (hData)
	{
		LPCSTR lpcszData=(LPCSTR)GlobalLock(hData);
	//	ms_pCtl->SetWindowText(lpcszData);
	//	SendMessage(pMainFrame->GetSafeHwnd(), WM_COMMAND, IDOK, 0);
		str2 = lpcszData;
    }
	else
	{
//		hData=DataObj.GetGlobalData(CF_HDROP);
//		if (!hData) {
			TRACE("Fail in getting data\n");
			return E_INVALIDARG;
//		}

//		HDROP hdrop =(HDROP)GlobalLock(hData);

//		DragQueryFile(hdrop, 0, lpfiles, 1024);

//		str2 = lpfiles;
	}

    GlobalUnlock(hData);

	//if the protocol is javascript then ignore
	if(str2.Left(11)=="javascript:")
		return S_OK;


/*	POINT p; p.x = pt.x; p.y = pt.y;
	if(m_hFromWnd != WindowFromPoint(p))
	{

		m_hFromWnd = NULL;
		return S_OK;
	}
	m_hFromWnd = NULL;*/

		try{
			CMyIEView* pvw = CreateNewView();
			if(pvw!=NULL)
			{
				pvw->ToNavigate(str2,0,NULL);
			}
		}catch(...)
		{
		}

	}catch(...)
	{
	}

    return S_OK;
}