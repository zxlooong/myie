// FixedHtmlView.cpp : implementation file
//

//2.5 387 	strURL = V_BSTR(URL); Use a global CString for onbeforenavigate, ondownloadcomplete, onnavigatecomplete ...
//2.5 387 remove CByteArray in onbeforenavigate


#include "stdafx.h"
#include "MyIE.h"
#include "FixedHtmlView.h"
#include "MainFrm.h"
#include <Mshtmhst.h>

#ifdef AFX_HTML_SEG
#pragma code_seg(AFX_HTML_SEG)
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_BROWSER_CONTROL 100

/////////////////////////////////////////////////////////////////////////////
// CFixedHtmlView

IMPLEMENT_DYNCREATE(CFixedHtmlView, CView)

CFixedHtmlView::CFixedHtmlView()
{//12/27
//	m_pBrowserApp = NULL;
	
	glpDisp = NULL;

	m_bForceResize = FALSE;

	m_tLastClick.LowPart = 0;
	m_tLastRClick.LowPart = 0;
	m_bRClick = FALSE;
	m_bIniDL = TRUE;
	m_pITravelLog = NULL;
	m_bIniTravelLog = FALSE;
}

CFixedHtmlView::~CFixedHtmlView()
{//11/7
/*	try{
	if (m_pBrowserApp != NULL)
		m_pBrowserApp->Release();
	}catch(...)
	{
	}*/
//	if(m_wndBrowser.m_hWnd!=NULL)
//		m_wndBrowser.DestroyWindow();

	if(m_pITravelLog!=NULL)
		m_pITravelLog->Release();
	m_pITravelLog = NULL;
}


BEGIN_MESSAGE_MAP(CFixedHtmlView, CView)
	//{{AFX_MSG_MAP(CFixedHtmlView)
	ON_WM_PARENTNOTIFY()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CFixedHtmlView, CView)
	ON_EVENT(CFixedHtmlView, IDC_BROWSER_CONTROL, 102 /* StatusTextChange */, OnStatusTextChange, VTS_BSTR)
	ON_EVENT(CFixedHtmlView, IDC_BROWSER_CONTROL, 108 /* ProgressChange */, OnProgressChange, VTS_I4 VTS_I4)
	ON_EVENT(CFixedHtmlView, IDC_BROWSER_CONTROL, 105 /* CommandStateChange */, OnCommandStateChange, VTS_I4 VTS_BOOL)
//	ON_EVENT(CFixedHtmlView, IDC_BROWSER_CONTROL, 106 /* DownloadBegin */, OnDownloadBegin, VTS_NONE)
//	ON_EVENT(CFixedHtmlView, IDC_BROWSER_CONTROL, 104 /* DownloadComplete */, OnDownloadComplete, VTS_NONE)
	ON_EVENT(CFixedHtmlView, IDC_BROWSER_CONTROL, 113 /* TitleChange */, OnTitleChange, VTS_BSTR)
	ON_EVENT(CFixedHtmlView, IDC_BROWSER_CONTROL, 252 /* NavigateComplete2 */, NavigateComplete2, VTS_DISPATCH VTS_PVARIANT)
	ON_EVENT(CFixedHtmlView, IDC_BROWSER_CONTROL, 250 /* BeforeNavigate2 */, BeforeNavigate2, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
//	ON_EVENT(CFixedHtmlView, IDC_BROWSER_CONTROL, 112 /* PropertyChange */, OnPropertyChange, VTS_BSTR)
	ON_EVENT(CFixedHtmlView, IDC_BROWSER_CONTROL, 251 /* NewWindow2 */, OnNewWindow2, VTS_PDISPATCH VTS_PBOOL)
	ON_EVENT(CFixedHtmlView, IDC_BROWSER_CONTROL, 259 /* DocumentComplete */, DocumentComplete, VTS_DISPATCH VTS_PVARIANT)
//	ON_EVENT(CFixedHtmlView, IDC_BROWSER_CONTROL, 253 /* OnQuit */, OnQuit, VTS_NONE)
//	ON_EVENT(CFixedHtmlView, IDC_BROWSER_CONTROL, 254 /* OnVisible */, OnVisible, VTS_BOOL)
//	ON_EVENT(CFixedHtmlView, IDC_BROWSER_CONTROL, 255 /* OnToolBar */, OnToolBar, VTS_BOOL)
//	ON_EVENT(CFixedHtmlView, IDC_BROWSER_CONTROL, 256 /* OnMenuBar */, OnMenuBar, VTS_BOOL)
//	ON_EVENT(CFixedHtmlView, IDC_BROWSER_CONTROL, 257 /* OnStatusBar */, OnStatusBar, VTS_BOOL)
//	ON_EVENT(CFixedHtmlView, IDC_BROWSER_CONTROL, 258 /* OnFullScreen */, OnFullScreen, VTS_BOOL)
//	ON_EVENT(CFixedHtmlView, IDC_BROWSER_CONTROL, 260 /* OnTheaterMode */, OnTheaterMode, VTS_BOOL)
//328
//	ON_EVENT(CFixedHtmlView, IDC_BROWSER_CONTROL, EVENT_STOCK_ERROREVENT, OnError, VTS_I2 VTS_PBSTR VTS_SCODE VTS_BSTR VTS_BSTR VTS_I4 VTS_PBOOL)
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFixedHtmlView drawing

void CFixedHtmlView::OnDraw(CDC* pDC)
{
//	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

//12/27
void CFixedHtmlView::OnDestroy()
{
/*	try{
	if (m_pBrowserApp != NULL)
		m_pBrowserApp->Release();
	}catch(...)
	{
	}
	m_pBrowserApp = NULL;*/
}

/////////////////////////////////////////////////////////////////////////////
// CFixedHtmlView diagnostics

#ifdef _DEBUG
void CFixedHtmlView::AssertValid() const
{
	CView::AssertValid();
}

void CFixedHtmlView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFixedHtmlView message handlers


/*int CFixedHtmlView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	// Create the embedded browser control:
	// No TRY block needed.
	return 0;
}*/

/*
CString CFixedHtmlView::GetType() 
{
	CString str = m_wndBrowser.GetType();
	return str;
}*/

long CFixedHtmlView::GetLeft() 
{
	long result;
	try{
	result = m_wndBrowser.GetLeft();
	}catch(...)
	{
	}
	return result;
}


long CFixedHtmlView::GetTop() 
{
	long result;
	try{
	result = m_wndBrowser.GetTop();
	}catch(...)
	{
	}
	return result;
}

long CFixedHtmlView::GetHeight() 
{
	long result;
	try{
	result = m_wndBrowser.GetHeight();
	}catch(...)
	{
	}
	return result;
}


long CFixedHtmlView::GetWidth() 
{
	long result;
	try{
	result = m_wndBrowser.GetWidth();
	}catch(...)
	{
	}
	return result;
}

void CFixedHtmlView::SetTop(long nNewValue)
{ 
	try{
	m_wndBrowser.SetTop(nNewValue);
	}catch(...)
	{
	}
}

void CFixedHtmlView::SetLeft(long nNewValue)
{ 
	try{
	m_wndBrowser.SetLeft(nNewValue); 
	}catch(...)
	{
	}
}

void CFixedHtmlView::SetHeight(long nNewValue)
{ 
	try{
	m_wndBrowser.SetHeight(nNewValue);
	}catch(...)
	{
	}
}

void CFixedHtmlView::SetWidth(long nNewValue)
{ 
	try{
	m_wndBrowser.SetWidth(nNewValue); 
	}catch(...)
	{
	}
}

/*
int CFixedHtmlView::GetToolBar() 
{
	return m_wndBrowser.GetToolBar();
}


BOOL CFixedHtmlView::GetVisible() 
{
	return m_wndBrowser.GetVisible();
}
*/
void CFixedHtmlView::GetLocationName(CString& strLocName) 
{
	try
	{
//2.2 315		strLocName = "";
		strLocName= m_wndBrowser.GetLocationName();
	}
/*	catch(CMemoryException* e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}
}

void CFixedHtmlView::GetLocationURL(CString& strLocURL) 
{
	try
	{
	strLocURL = "";
	strLocURL= m_wndBrowser.GetLocationURL();
	}
/*	catch(CMemoryException* e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}

}
/*
BOOL CFixedHtmlView::GetBusy() 
{
	return m_wndBrowser.GetBusy();
}*/

READYSTATE CFixedHtmlView::GetReadyState() 
{
	return (READYSTATE)m_wndBrowser.GetReadyState();
}
/*
BOOL CFixedHtmlView::GetOffline() 
{
	return m_wndBrowser.GetOffline();
}

BOOL CFixedHtmlView::GetSilent() 
{
	return m_wndBrowser.GetSilent();
}
*/
LPDISPATCH CFixedHtmlView::GetApplication() 
{
	try
	{
		return m_wndBrowser.GetApplication();
	}
/*	catch(CMemoryException* e)
	{
		if(e!=NULL)e->Delete();
		return NULL;
	}*/
	catch(...)
	{
		return NULL;
	}
}


/*LPDISPATCH CFixedHtmlView::GetParentBrowser() 
{
	return m_wndBrowser.GetParent();
}

LPDISPATCH CFixedHtmlView::GetContainer() 
{
	return m_wndBrowser.GetContainer();
}*/

LPDISPATCH CFixedHtmlView::GetHtmlDocument() 
{
	try
	{//b5
		return m_wndBrowser.GetDocument();
	}
	catch(...)
	{
		return NULL;
	}
}
/*
BOOL CFixedHtmlView::GetTopLevelContainer() 
{
	return m_wndBrowser.GetTopLevelContainer();
}

BOOL CFixedHtmlView::GetMenuBar() 
{
	return m_wndBrowser.GetMenuBar();
}

BOOL CFixedHtmlView::GetFullScreen() 
{
	return m_wndBrowser.GetFullScreen();
}

BOOL CFixedHtmlView::GetStatusBar() 
{
	return m_wndBrowser.GetStatusBar();
}
*/
OLECMDF CFixedHtmlView::QueryStatusWB(OLECMDID cmdID) 
{
	return (OLECMDF)m_wndBrowser.QueryStatusWB(cmdID);

}

void CFixedHtmlView::ExecWB(OLECMDID cmdID, OLECMDEXECOPT cmdexecopt,
	VARIANT* pvaIn, VARIANT* pvaOut)
{
	LPDISPATCH lpDisp = NULL;
	IWebBrowser2* lpWB = NULL;

	try{
	lpDisp = m_wndBrowser.GetApplication();
	if(lpDisp != NULL)
	{
		//2.2
		if(SUCCEEDED(lpDisp->QueryInterface(IID_IWebBrowser2, (void**)&lpWB)))
		{
			lpWB->ExecWB(cmdID, cmdexecopt, pvaIn, pvaOut);
		}
	}

	}
	catch(...)
	{
	}

//2.2
	try{
		if(lpWB!=NULL)
			lpWB->Release();
		if(lpDisp!=NULL)
			lpDisp->Release();
	}
	catch(...)
	{
	}
}

/*
BOOL CFixedHtmlView::GetRegisterAsBrowser() 
{
	return m_wndBrowser.GetRegisterAsBrowser();
}

BOOL CFixedHtmlView::GetRegisterAsDropTarget() 
{
	return m_wndBrowser.GetRegisterAsDropTarget();
}

BOOL CFixedHtmlView::GetTheaterMode() 
{
	return m_wndBrowser.GetTheaterMode();
}

BOOL CFixedHtmlView::GetAddressBar() 
{
	return m_wndBrowser.GetAddressBar();
}*/

/////////////////////////////////////////////////////////////////////////////
// CFixedHtmlView operations

BOOL CFixedHtmlView::LoadFromResource(LPCTSTR lpszResource)
{
	HINSTANCE hInstance = AfxGetResourceHandle();
	ASSERT(hInstance != NULL);

	CString strResourceURL;
	BOOL bRetVal = TRUE;
//	LPTSTR lpszModule = new TCHAR[_MAX_PATH];
	TCHAR lpszModule[_MAX_PATH];

	if (GetModuleFileName(hInstance, lpszModule, _MAX_PATH))
	{
		strResourceURL.Format(_T("res://%s/%s"), lpszModule, lpszResource);
		Navigate(strResourceURL, 0, 0, 0);
	}
	else
		bRetVal = FALSE;

//	delete [] lpszModule;
	return bRetVal;
}

BOOL CFixedHtmlView::LoadFromResource(UINT nRes)
{
	HINSTANCE hInstance = AfxGetResourceHandle();
	ASSERT(hInstance != NULL);

	CString strResourceURL;
	BOOL bRetVal = TRUE;
//	LPTSTR lpszModule = new TCHAR[_MAX_PATH];
	TCHAR lpszModule[_MAX_PATH];

	if (GetModuleFileName(hInstance, lpszModule, _MAX_PATH))
	{
		strResourceURL.Format(_T("res://%s/%d"), lpszModule, nRes);
		Navigate(strResourceURL, 0, 0, 0);
	}
	else
		bRetVal = FALSE;

//	delete [] lpszModule;
	return bRetVal;
}

void CFixedHtmlView::Navigate(LPCTSTR lpszURL, DWORD dwFlags /* = 0 */,
	LPCTSTR lpszTargetFrameName /* = NULL */ ,
	LPCTSTR lpszHeaders /* = NULL */, LPVOID lpvPostData /* = NULL */,
	DWORD dwPostDataLen /* = 0 */)
{
//	if(m_pBrowserApp == NULL)
//		return;

	LPDISPATCH lpDisp = NULL;
	IWebBrowser2* lpWB = NULL;
	BSTR bstrURL = NULL;
	try
	{
/*	COleSafeArray vPostData;
	if (lpvPostData != NULL)
	{
		if (dwPostDataLen == 0)
			dwPostDataLen = lstrlen((LPCTSTR) lpvPostData);

		vPostData.CreateOneDim(VT_UI1, dwPostDataLen, lpvPostData);
	}*/

	strURL2 = lpszURL;
	bstrURL = strURL2.AllocSysString();
//	m_pBrowserApp->Navigate(bstrURL,
	lpDisp = m_wndBrowser.GetApplication();
	if(lpDisp != NULL)
	{
		//2.2
		if(SUCCEEDED(lpDisp->QueryInterface(IID_IWebBrowser2, (void**)&lpWB)))
		{
			lpWB->Navigate(bstrURL,
				COleVariant((long) dwFlags, VT_I4),
				COleVariant(lpszTargetFrameName, VT_BSTR),
				vPostData,
				COleVariant(lpszHeaders, VT_BSTR));
		}
//2.1
//		lpDisp->Release();
//		lpDisp = NULL;
	}

	}
/*	catch(CMemoryException* e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}

	try{
		//2.2
		if(bstrURL!=NULL)
			SysFreeString(bstrURL);
		if(lpWB!=NULL)
			lpWB->Release();
		if(lpDisp!=NULL)
			lpDisp->Release();
	}
	catch(...)
	{
	}
}


void CFixedHtmlView::Navigate2(LPCTSTR lpszURL, DWORD dwFlags /* = 0 */,
	LPCTSTR lpszTargetFrameName /* = NULL */,
	LPCTSTR lpszHeaders /* = NULL */,
	LPVOID lpvPostData /* = NULL */, DWORD dwPostDataLen /* = 0 */)
{
//	if(m_pBrowserApp == NULL)
//		return;
	LPDISPATCH lpDisp = NULL;
	IWebBrowser2* lpWB = NULL;
	try
	{
//	COleSafeArray vPostData;
/*	if (lpvPostData != NULL)
	{
		if (dwPostDataLen == 0)
			dwPostDataLen = lstrlen((LPCTSTR) lpvPostData);

		vPostData.CreateOneDim(VT_UI1, dwPostDataLen, lpvPostData);
	}*/

	COleVariant vURL(lpszURL, VT_BSTR);
	COleVariant vHeaders(lpszHeaders, VT_BSTR);
	COleVariant vTargetFrameName(lpszTargetFrameName, VT_BSTR);
	COleVariant vFlags((long) dwFlags, VT_I4);

	lpDisp = m_wndBrowser.GetApplication();
	if(lpDisp!=NULL)
	{
		//2.2
		if(SUCCEEDED(lpDisp->QueryInterface(IID_IWebBrowser2, (void**)&lpWB)))
			lpWB->Navigate2(vURL,
				vFlags, vTargetFrameName, vPostData, vHeaders);

//		lpDisp->Release();
//		lpDisp = NULL;
	}
	}
/*	catch(CMemoryException* e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}

	try{
		if(lpWB!=NULL)
			lpWB->Release();
		if(lpDisp!=NULL)
			lpDisp->Release();
	}
	catch(...)
	{
	}

}


/*
void CFixedHtmlView::PutProperty(LPCTSTR lpszProperty, const VARIANT& vtValue)
{
	m_wndBrowser.PutProperty(lpszProperty, vtValue);
}

BOOL CFixedHtmlView::GetProperty(LPCTSTR lpszProperty, CString& strValue)
{

	BOOL bResult = FALSE;
	VARIANT vReturn;
	vReturn.vt = VT_BSTR;
	vReturn.bstrVal = NULL;
	vReturn = m_wndBrowser.GetProperty_(lpszProperty);

	strValue = CString(vReturn.bstrVal);

	bResult = TRUE;

	return bResult;
}

COleVariant CFixedHtmlView::GetProperty(LPCTSTR lpszProperty)
{
	COleVariant result;

	VARIANT vReturn;
	vReturn.vt = VT_BSTR;
	vReturn.bstrVal = NULL;
	vReturn = m_wndBrowser.GetProperty_(lpszProperty);

	result = vReturn;

	return result;
}

CString CFixedHtmlView::GetFullName() 
{
	return m_wndBrowser.GetFullName();
}
*/
void CFixedHtmlView::GoHome()
{
	LPDISPATCH lpDisp = NULL;
	IWebBrowser2* lpWB = NULL;
	try{
	lpDisp = m_wndBrowser.GetApplication();
	if(lpDisp != NULL)
	{
		//2.2
		if(SUCCEEDED(lpDisp->QueryInterface(IID_IWebBrowser2, (void**)&lpWB)))
			lpWB->GoHome();
			//lpDisp->Release();
	}
	}catch(...)
	{
	}

	try{
		if(lpWB!=NULL)
			lpWB->Release();
		if(lpDisp!=NULL)
			lpDisp->Release();
	}catch(...)
	{
	}
}

void CFixedHtmlView::Stop()
{
	LPDISPATCH lpDisp = NULL;
	IWebBrowser2* lpWB = NULL;
	try{
	lpDisp = m_wndBrowser.GetApplication();
	if(lpDisp != NULL)
	{
		//2.2
		if(SUCCEEDED(lpDisp->QueryInterface(IID_IWebBrowser2, (void**)&lpWB)))
			lpWB->Stop();
		//lpDisp->Release();
	}
	}catch(...)
	{
	}

	try{
		if(lpWB!=NULL)
			lpWB->Release();
		if(lpDisp!=NULL)
			lpDisp->Release();
	}catch(...)
	{
	}
}

void CFixedHtmlView::GoBack(int dist)
{
	LPDISPATCH lpDisp = NULL;
	IWebBrowser2* lpWB = NULL;
	lpDisp = m_wndBrowser.GetApplication();
	if(lpDisp != NULL)
	{
		//2.2
		if(SUCCEEDED(lpDisp->QueryInterface(IID_IWebBrowser2, (void**)&lpWB)))
		{
			for(int i=0; i<dist; i++)
				lpWB->GoBack();
			m_nBackSteps = dist;
		}
		//lpDisp->Release();
	}

	try{
		if(lpWB!=NULL)
			lpWB->Release();
		if(lpDisp!=NULL)
			lpDisp->Release();
	}catch(...)
	{
	}

}

void CFixedHtmlView::GoForward(int dist)
{
	LPDISPATCH lpDisp = NULL;
	IWebBrowser2* lpWB = NULL;
	lpDisp = m_wndBrowser.GetApplication();
	if(lpDisp != NULL)
	{
		//2.2
		if(SUCCEEDED(lpDisp->QueryInterface(IID_IWebBrowser2, (void**)&lpWB)))
		{
			for(int i=0; i<dist; i++)
				lpWB->GoForward();
			m_nBackSteps = -dist;
		}
		//lpDisp->Release();
	}

	try{
		if(lpWB!=NULL)
			lpWB->Release();
		if(lpDisp!=NULL)
			lpDisp->Release();
	}catch(...)
	{
	}
}

/*void CFixedHtmlView::GoSearch()
{
	m_wndBrowser.GoSearch();
}*/

void CFixedHtmlView::Refresh()
{
	LPDISPATCH lpDisp = NULL;
	IWebBrowser2* lpWB = NULL;
	try{
	lpDisp = m_wndBrowser.GetApplication();
	if(lpDisp != NULL)
	{
		//2.2
		if(SUCCEEDED(lpDisp->QueryInterface(IID_IWebBrowser2, (void**)&lpWB)))
			lpWB->Refresh();
		//lpDisp->Release();

		//3.0
		m_nProgress = 0;
	}
	}catch(...)
	{
	}

	try{
		if(lpWB!=NULL)
			lpWB->Release();
		if(lpDisp!=NULL)
			lpDisp->Release();
	}catch(...)
	{
	}
}

void CFixedHtmlView::Refresh2(int nLevel)
{
	LPDISPATCH lpDisp = NULL;
	IWebBrowser2* lpWB = NULL;
	try{
	lpDisp = m_wndBrowser.GetApplication();
	if(lpDisp != NULL)
	{
		//2.2
		if(SUCCEEDED(lpDisp->QueryInterface(IID_IWebBrowser2, (void**)&lpWB)))
			lpWB->Refresh2(COleVariant((long) nLevel, VT_I4));
		//lpDisp->Release();

		//3.0
		m_nProgress = 0;
	}
	}catch(...)
	{
	}

	try{
		if(lpWB!=NULL)
			lpWB->Release();
		if(lpDisp!=NULL)
			lpDisp->Release();
	}catch(...)
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFixedHtmlView event reflectors
void CFixedHtmlView::NavigateComplete2(LPDISPATCH  pDisp , VARIANT* URL)
{
	ASSERT(V_VT(URL) == VT_BSTR);

	try
	{
		//2.5 add for sina ads
		m_bLClick = FALSE;
		m_bRClick = FALSE;

		USES_CONVERSION;

		strURL = OLE2T(V_BSTR(URL));

		//successfully set history,
		//this can get the real url of this navigation
		//but not fit for history list, because the back and forward doesn't 
		//get the same url as navigation
//		if(!IsIFrame(pDisp ,str))
		if(m_pITravelLog==NULL)
		{
			SetHistory(strURL);
			m_strLastURL = strURL;
		}

		if (!glpDisp)
		{
			glpDisp = pDisp;

		}

		//316
		if(m_bForceResize)
		{
			CRect rect;
			GetWindowRect(&rect);
			PostMessage(WM_SIZE,  0, ((rect.Height())<<16)+rect.Width());
			m_bForceResize = FALSE;
		}

		OnNavigateComplete2(strURL);

	}
	catch(...)
	{
	}
}

void CFixedHtmlView::BeforeNavigate2(LPDISPATCH  pDisp , VARIANT* URL,
		VARIANT* Flags, VARIANT* TargetFrameName,
		VARIANT* PostData, VARIANT* Headers, BOOL* Cancel)
{
	ASSERT(V_VT(URL) == VT_BSTR);
	ASSERT(V_VT(TargetFrameName) == VT_BSTR);
	ASSERT(V_VT(PostData) == (VT_VARIANT | VT_BYREF));
	ASSERT(V_VT(Headers) == VT_BSTR);
	ASSERT(Cancel != NULL);

	try
	{
	USES_CONVERSION;
	strURL = V_BSTR(URL);

	//2.5 for download tools, share one instance between windows. let's test, if it
	//can work properly, we can create separate instance for each window. But this may
	//increase the cost of memory.
	if(m_bIniDL)
	{
		if(pMainFrame->pDownload!=NULL && (strURL.Find('?')<0 || strURL.Find(".jsp?")>0))
		{
			try{

			if(IsDownloadable(strURL))
			{
				IObjectWithSite* pSite = NULL;
				pMainFrame->pDownload->QueryInterface(IID_IObjectWithSite, (void**)&pSite);
				if(pSite!=NULL)
				{
					pSite->SetSite(GetApplication());
					pSite->Release();
					m_bIniDL = FALSE;
				}


	/*		//	VARIANT VarResult;
			//	EXCEPINFO ExcepInfo;
			//	unsigned int uArgErr=0;
				DISPPARAMS dispparams; 
				dispparams.rgvarg = new VARIANT[7];

			  // The parameters for this DISPID are as follows:
			  // [0]: Cancel flag  - VT_BYREF|VT_BOOL
			  // [1]: HTTP headers - VT_BYREF|VT_VARIANT
			  // [2]: Address of HTTP POST data  - VT_BYREF|VT_VARIANT 
			  // [3]: Target frame name - VT_BYREF|VT_VARIANT 
			  // [4]: Option flags - VT_BYREF|VT_VARIANT
			  // [5]: URL to navigate to - VT_BYREF|VT_VARIANT
			  // [6]: An object that evaluates to the top-level or frame
			  //      WebBrowser object corresponding to the event 
			  //
      
				VARIANT_BOOL cancel=VARIANT_FALSE;
				dispparams.rgvarg[0].vt = VT_BYREF|VT_BOOL;
				dispparams.rgvarg[0].pboolVal = &cancel;

				dispparams.rgvarg[1].vt = VT_BYREF|VT_VARIANT;
				dispparams.rgvarg[1].pvarVal = Headers;
				dispparams.rgvarg[2].vt = VT_BYREF|VT_VARIANT;
				dispparams.rgvarg[2].pvarVal = PostData;
				dispparams.rgvarg[3].vt = VT_BYREF|VT_VARIANT;
				dispparams.rgvarg[3].pvarVal = TargetFrameName;
				dispparams.rgvarg[4].vt = VT_BYREF|VT_VARIANT;
				dispparams.rgvarg[4].pvarVal = Flags;
				dispparams.rgvarg[5].vt = VT_BYREF|VT_VARIANT;
				dispparams.rgvarg[5].pvarVal = URL;
				dispparams.rgvarg[6].vt = VT_DISPATCH;
				dispparams.rgvarg[6].pdispVal = pDisp;
				dispparams.cArgs = 7;
				dispparams.cNamedArgs = 0;

				HRESULT hr = ((CMainFrame*)pMainFrame)->pDownload->Invoke(250, IID_NULL,
					LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, NULL, NULL, NULL);

				delete[] dispparams.rgvarg;*/

			/*	CStdioFile f;
				f.Open("d:\\tmp.txt", CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);
				f.SeekToEnd();
				f.WriteString(strURL);
				f.WriteString("\r\n");
				f.Close();	*/

	/*			if(cancel==VARIANT_TRUE)
				{
					*Cancel=TRUE;
					return;
				}
				else //2.5 377 a test
					return;*/
			}

			}catch(...)
			{
			}
		}
	}
	//end 2.5


	VARIANT* vtPostedData = V_VARIANTREF(PostData);
	//CByteArray array;
	BOOL array = FALSE;
	if (V_VT(vtPostedData) & VT_ARRAY)
	{
		// must be a vector of bytes
		ASSERT(vtPostedData->parray->cDims == 1 && vtPostedData->parray->cbElements == 1);

		vtPostedData->vt |= VT_UI1;
		COleSafeArray safe(vtPostedData);

		DWORD dwSize = safe.GetOneDimSize();
		//LPVOID pVoid;
		//safe.AccessData(&pVoid);

		//array.SetSize(dwSize>0?1:0);
		array = dwSize>0?TRUE:FALSE;  //2.5

		//LPBYTE lpByte = array.GetData();

		//memcpy(lpByte, pVoid, dwSize);
		//safe.UnaccessData();
	}
	// make real parameters out of the notification

	//2.0
//	CString strTargetFrameName(V_BSTR(TargetFrameName));
//	CString strHeaders = V_BSTR(Headers);
	DWORD nFlags = V_I4(Flags);

	// notify the user's class
//	OnBeforeNavigate2(strURL, nFlags, strTargetFrameName,
//		array, strHeaders, Cancel);
	OnBeforeNavigate2(strURL, nFlags, "",
		array, "", Cancel);
	}
	catch(...)
	{
	}
}

void CFixedHtmlView::DocumentComplete(LPDISPATCH pDisp, VARIANT* URL)
{
	try{
//	UNUSED_ALWAYS(pDisp);
	ASSERT(V_VT(URL) == VT_BSTR);

	//is download complete?
	if (glpDisp && glpDisp == pDisp)
	   {
		  // if the LPDISPATCH are same, that means
		  // it is the final DocumentComplete. Reset glpDisp
		  TRACE("Document is done downloading");
		  glpDisp = NULL;

	   }


//	CString str(V_BSTR(URL));
	strURL = V_BSTR(URL);

	OnDocumentComplete(strURL);
	}catch(...)
	{
	}
}

//event




/////////////////////////////////////////////////////////////////////////////
// CFixedHtmlView Events

void CFixedHtmlView::OnProgressChange(long lProgress, long lProgressMax)
{
	// user will override to handle this notification
	UNUSED_ALWAYS(lProgress);
	UNUSED_ALWAYS(lProgressMax);
}

void CFixedHtmlView::OnCommandStateChange(long lCommand, BOOL bEnable)
{
	// user will override to handle this notification
	UNUSED_ALWAYS(lCommand);
	UNUSED_ALWAYS(bEnable);
}

/*void CFixedHtmlView::OnDownloadBegin()
{
	// user will override to handle this notification
}

void CFixedHtmlView::OnDownloadComplete()
{
	// user will override to handle this notification
}
*/
void CFixedHtmlView::OnTitleChange(LPCTSTR lpszText)
{
	// user will override to handle this notification
	UNUSED_ALWAYS(lpszText);
}
/*
void CFixedHtmlView::OnPropertyChange(LPCTSTR lpszProperty)
{
	// user will override to handle this notification
	UNUSED_ALWAYS(lpszProperty);
}*/

void CFixedHtmlView::OnNewWindow2(LPDISPATCH* ppDisp, BOOL* bCancel)
{
	// default to continuing
	*bCancel = FALSE;

	// user will override to handle this notification
	UNUSED_ALWAYS(ppDisp);
}

void CFixedHtmlView::OnDocumentComplete(LPCTSTR lpszURL)
{
	// user will override to handle this notification
	UNUSED_ALWAYS(lpszURL);
}
/*
void CFixedHtmlView::OnQuit()
{
	// user will override to handle this notification
}

void CFixedHtmlView::OnVisible(BOOL bVisible)
{
	// user will override to handle this notification
	UNUSED_ALWAYS(bVisible);
}

void CFixedHtmlView::OnToolBar(BOOL bToolBar)
{
	// user will override to handle this notification
	UNUSED_ALWAYS(bToolBar);
}

void CFixedHtmlView::OnMenuBar(BOOL bMenuBar)
{
	// user will override to handle this notification
	UNUSED_ALWAYS(bMenuBar);
}

void CFixedHtmlView::OnStatusBar(BOOL bStatusBar)
{
	// user will override to handle this notification
	UNUSED_ALWAYS(bStatusBar);
}

void CFixedHtmlView::OnFullScreen(BOOL bFullScreen)
{
	// user will override to handle this notification
	UNUSED_ALWAYS(bFullScreen);
}

void CFixedHtmlView::OnTheaterMode(BOOL bTheaterMode)
{
	// user will override to handle this notification
	UNUSED_ALWAYS(bTheaterMode);
}
*/
void CFixedHtmlView::OnNavigateComplete2(LPCTSTR lpszURL)
{
	// user will override to handle this notification
	UNUSED_ALWAYS(lpszURL);
}

void CFixedHtmlView::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags,
	LPCTSTR lpszTargetFrameName, BOOL baPostData,
	LPCTSTR lpszHeaders, BOOL* bCancel)
{
	// default to continuing
	*bCancel = FALSE;

	// user will override to handle this notification
	UNUSED_ALWAYS(lpszURL);
	UNUSED_ALWAYS(nFlags);
	UNUSED_ALWAYS(lpszTargetFrameName);
	UNUSED_ALWAYS(baPostData);
	UNUSED_ALWAYS(lpszHeaders);
}

void CFixedHtmlView::OnStatusTextChange(LPCTSTR pszText)
{
	// try to set the status bar text via the frame

//	CFrameWnd* pFrame = GetParentFrame();
//	if (pFrame != NULL)
//		pFrame->SetMessageText(pszText);
}

BOOL CFixedHtmlView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
		// create the view window itself
	//AfxEnableControlContainer();

	try{
//	m_pCreateContext = pContext;
	if (!CView::Create(lpszClassName, lpszWindowName,
				dwStyle, rect, pParentWnd,  nID, pContext))
	{
		return FALSE;
	}

//327
	RECT rectClient;
	if(m_bMax==1)
	{
		::GetClientRect(pMainFrame->m_hWndMDIClient, &rectClient);
		rectClient.top -= 2;
		rectClient.left -= 2;
		rectClient.bottom += 2;
		rectClient.right += 2;
	}
	else
	{
		rectClient.top = rect.top - 2;
		rectClient.left = rect.left - 2;
		rectClient.bottom = rect.bottom + 2;
		rectClient.right = rect.right + 2;
	}

//end 327
	
	if(!m_wndBrowser.Create ("Browser", WS_VISIBLE|WS_CHILD|WS_TABSTOP, rectClient, this, IDC_BROWSER_CONTROL))
	{//11/7
		DestroyWindow();
		return FALSE;
	}

	// create the control window
	// AFX_IDW_PANE_FIRST is a safe but arbitrary ID
/*	if (!m_wndBrowser.CreateControl(CLSID_WebBrowser, lpszWindowName,
				WS_VISIBLE | WS_CHILD, rectClient, this, AFX_IDW_PANE_FIRST))
	{
		DestroyWindow();
		return FALSE;
	}
*/
	//11/7
/*	try{
	LPUNKNOWN lpUnk = m_wndBrowser.GetControlUnknown();
	HRESULT hr;
	if(lpUnk!=NULL)
		hr = lpUnk->QueryInterface(IID_IWebBrowser2, (void**) &m_pBrowserApp);
	if (!SUCCEEDED(hr))
	{
		m_pBrowserApp = NULL;
		m_wndBrowser.DestroyWindow();
		DestroyWindow();
		return FALSE;
	}
	}catch(...)
	{
	}
*/
	//12/27
	//m_pBrowserApp = (IWebBrowser2*)m_wndBrowser.GetApplication();


	}
/*	catch(CMemoryException* e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}

	return TRUE;
	//return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

BOOL CFixedHtmlView::PreTranslateMessage(MSG* pMsg)
{
	ASSERT(pMsg != NULL);
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);


	try
	{
	if(pMsg == NULL || m_hWnd == NULL)
		return TRUE;

	// allow tooltip messages to be filtered
	if (CView::PreTranslateMessage(pMsg))
		return TRUE;
	// filter both messages to dialog and from children

 	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		QueryPerformanceCounter(&m_tLastClick);
		m_bRClick = FALSE;
		m_bLClick = TRUE;
	}

	//2.4
	short ks;
    ks =GetKeyState(VK_CONTROL);
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam != VK_ESCAPE && (pMsg->wParam<VK_F1 || pMsg->wParam>VK_F12) && (ks>=0 || pMsg->wParam==VK_CONTROL || pMsg->wParam=='C' || pMsg->wParam == 'X' || pMsg->wParam == 'V' || pMsg->wParam == 'Z' || pMsg->wParam == VK_INSERT)) // && ((pMsg->wParam == VK_TAB && ks>=0) || pMsg->wParam == VK_BACK || pMsg->wParam == VK_DELETE || pMsg->wParam == 'C' || pMsg->wParam == 'X' || pMsg->wParam == 'V' || pMsg->wParam == 'Z' || pMsg->wParam == VK_INSERT))
		if(pMsg->wParam == VK_RETURN)
		{
//			PreTranslateInput(pMsg);
			return FALSE;
		}
		else
			return PreTranslateInput(pMsg);

	// don't translate dialog messages when in Shift+F1 help mode
//	CFrameWnd* pFrameWnd = GetTopLevelFrame();
//	if (pFrameWnd != NULL && pFrameWnd->m_bHelpMode)
//		return FALSE;

	// since 'IsDialogMessage' will eat frame window accelerators,
	//   we call all frame windows' PreTranslateMessage first
/*	pFrameWnd = GetParentFrame();   // start with first parent frame
	while (pFrameWnd != NULL)
	{
		// allow owner & frames to translate before IsDialogMessage does
		if (pFrameWnd->PreTranslateMessage(pMsg))
			return TRUE;

		// try parent frames until there are no parent frames
		pFrameWnd = pFrameWnd->GetParentFrame();
	}*/

/*	if (pMainFrame != NULL && pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_BACK ))
	{
		// allow owner & frames to translate before IsDialogMessage does
		if (pMainFrame->PreTranslateMessage(pMsg) )
			return TRUE;
	}*/

/*	// don't call IsDialogMessage if form is empty
	if (::GetWindow(m_hWnd, GW_CHILD) == NULL)
		return FALSE;
*/



	}
/*	catch(CMemoryException* e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}

	return FALSE;
}


BOOL CFixedHtmlView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

    cs.lpszClass = AfxRegisterWndClass(
       CS_DBLCLKS,                       // if you need double-clicks
       NULL,                             // no cursor (use default)
       NULL,                             // no background brush
       NULL); // app icon
    ASSERT(cs.lpszClass);
 

//	cs.style |= WS_CLIPCHILDREN;

	if( !CView::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



void CFixedHtmlView::OnParentNotify(UINT message, LPARAM lParam) 
{
	CView::OnParentNotify(message, lParam);
	
	// TODO: Add your message handler code here
	try{
	if(message == WM_DESTROY)
	{
		m_bToClose = TRUE;
		GetParentFrame()->PostMessage(WM_CLOSE);
	}
	}catch(...)
	{
	}
}


int CFixedHtmlView::SetHistory(LPCSTR strCurUrl)
{
	if(!m_bIniTravelLog)
	{
		//not initialize at startup
		if(strcmp(strCurUrl, "about:blank")==0)
			return 0;

		//load ITravelLogStg for IE 5.5
		m_pITravelLog = NULL;
		IServiceProvider* pISP = NULL;
		LPDISPATCH pDisp = GetApplication();
		if(pDisp!=NULL)
		{
			HRESULT hr = pDisp->QueryInterface(IID_IServiceProvider, (void**)&pISP);
			if(SUCCEEDED(hr) && pISP!=NULL) 
			{
				hr = pISP->QueryService(SID_STravelLogCursor , IID_ITravelLogStg, (void**)&m_pITravelLog);
				if(FAILED(hr))
					m_pITravelLog=NULL;
				
				pISP->Release();
			}
			pDisp->Release();
		}

		m_bIniTravelLog = TRUE;

		if(m_pITravelLog!=NULL)
			return 0;
	}

	short p = m_nHistoryLength;
	int seth = 0;
	LPDISPATCH pDisp = NULL;
	IHTMLDocument2 *pHTMLDoc = NULL;
	IHTMLWindow2 *pHTMLWnd = NULL;
	IOmHistory  *pHistory = NULL;
//	IPersistMoniker *pPM = NULL;
//	IMoniker * pMok = NULL;


	try{
		//get history length
		pDisp = GetHtmlDocument();
		if( pDisp )
		{
			if(SUCCEEDED(pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc)))
			{
				if(SUCCEEDED(pHTMLDoc->get_parentWindow( &pHTMLWnd )))
				{
					if(SUCCEEDED(pHTMLWnd->get_history( &pHistory )))
					{
						if(pHistory->get_length(&p)==S_OK)
						{
						//
	//					DWORD d;
	//					pHTMLDoc->QueryInterface(IID_IPersistMoniker, (void**)&pPM);
	//					pPM->GetCurMoniker(&pMok);
	//					pMok->Hash(&d);
	/*					FILE * file;
						file = fopen("c:\\moniker.txt", "a");
						fprintf(file, "%d\r\n", p);
						fclose(file);*/
						//
							if(p>0 && p!=m_nHistoryLength)
							{
							/*	CString m_strLastURL;
								//get last url from url list
								int ul = m_astrLastURLs.GetSize();
								if(ul==1)
									m_strLastURL = m_astrLastURLs.GetAt(0);
								else
								{
									CString pre1, pre2, tmp; 
									if(p-1-1>=0)
										pre1 = m_astrHistoryURL.GetAt(p-2);
									if(p-1-2>=0)
										pre2 = m_astrHistoryURL.GetAt(p-3);
										
									int i;
									for(i=ul-1; i>0; i--)
									{
										tmp = m_astrLastURLs.GetAt(i);
										if(tmp != pre1 && tmp != pre2)
											break;
									}
									m_strLastURL = m_astrLastURLs.GetAt(i);
								}
								m_astrLastURLs.RemoveAll();*/

								if(m_strLastTitle.IsEmpty())
									m_strLastTitle = m_strLastURL;
								if(p>m_nHistoryLength)
								{
									m_astrHistory.SetAtGrow(p-1, m_strLastTitle);
									m_astrHistoryURL.SetAtGrow(p-1, m_strLastURL);
								}
								else
								{
									m_astrHistory.SetAt(p-1, m_strLastTitle);
									m_astrHistory.SetSize(p);
									if(m_nHistoryLength - p >10)
										m_astrHistory.FreeExtra();

									m_astrHistoryURL.SetAt(p-1, m_strLastURL);
									m_astrHistoryURL.SetSize(p);
									if(m_nHistoryLength - p >10)
										m_astrHistoryURL.FreeExtra();
								}

								m_nCurHistory = p;
								seth = 1;
							//	m_strLastURL = strCurUrl;

							}
							else if(p==0)
								m_nCurHistory = 0;


							//adjust cur history
							m_nCurHistory -= m_nBackSteps;
							if(!m_back)
								m_nCurHistory = 0;
							else if(!m_forward && seth && m_nBackSteps>=0)
								m_nCurHistory = p;
							else if(m_nCurHistory>=0 && m_nCurHistory<p && strCurUrl == m_astrHistoryURL.GetAt(m_nCurHistory))
								m_nCurHistory = m_nCurHistory;
							else if(m_nCurHistory-1>=0 && strCurUrl == m_astrHistoryURL.GetAt(m_nCurHistory-1))
								m_nCurHistory --;
							else if(m_nCurHistory-2>=0 && strCurUrl  == m_astrHistoryURL.GetAt(m_nCurHistory-2))
								m_nCurHistory -=2;
							else if(m_nCurHistory+1<p && strCurUrl == m_astrHistoryURL.GetAt(m_nCurHistory+1))
								m_nCurHistory ++;
							else if(m_nCurHistory+2<p && strCurUrl  == m_astrHistoryURL.GetAt(m_nCurHistory+2))
								m_nCurHistory +=2;

							if(m_forward && m_nCurHistory>=p-1)
								m_nCurHistory = p-2;
							if(m_back && m_nCurHistory==0)
								m_nCurHistory =1;
								
							m_nHistoryLength = p;
						}
					}
				}
			}
		}
	}
	catch(...)
	{
	}



	try{
		if(pHistory != NULL)
			pHistory->Release();
		if(pHTMLWnd != NULL)
			pHTMLWnd->Release();
		if(pHTMLDoc != NULL)
			pHTMLDoc->Release();
		if(pDisp != NULL)
			pDisp->Release();
	}
	catch(...)
	{
	}
	
	m_nBackSteps = 0;

	return seth;
}

/*BOOL CFixedHtmlView::IsIFrame(LPDISPATCH  pDisp , LPCSTR url)
{
	//find url in last iframe list
	POSITION pos = m_lstrIFrames.Find(url);
	if(pos != NULL)
		return TRUE;

	//get the new iframe list
	m_lstrIFrames.RemoveAll();

	IWebBrowser2 * pApp = NULL;
	IHTMLDocument2 * pDoc = NULL;
	IHTMLElementCollection  * pAllElem = NULL;
	LPDISPATCH pDp = NULL;
	IHTMLElementCollection *pFrames = NULL;
	VARIANT tag, index;
	long p;

	tag.vt = VT_BSTR;
	CString tagname("Frame");
	tag.bstrVal = tagname.AllocSysString();

	pDisp->QueryInterface(IID_IWebBrowser2, (void**)&pApp);
	if(pApp!=NULL)
	{
		pApp->get_Document((LPDISPATCH*)&pDoc);
		if(pDoc!=NULL)
		{
			pDoc->get_all(&pAllElem);
			if(pAllElem!=NULL)
			{
				pAllElem->tags(tag, &pDp);
				if(pDp!=NULL)
				{
					pDp->QueryInterface(IID_IHTMLElementCollection, (void**)&pFrames);
					pFrames->get_length(&p);
					if(p>0)
					{
					}
				}
			}
		}
	}

	SysFreeString(tag.bstrVal);

	return FALSE;
}*/

BOOL CFixedHtmlView::IsDownloadable(CString &URL)
{
	int i = URL.ReverseFind('.');
	if(i>0)
	{
		ext = URL.Mid(i);
		ext.MakeUpper();
		if(ext.Find('/')>=0 || ext==".HTM" || ext==".HTML" || ext==".ASP")
			return FALSE;
		int j=0, l=ext.GetLength();
		char ch;
		i=0;
		int dllen = pMainFrame->m_strDownloadFileExt.GetLength();
		while(i<dllen)
		{
			ch = pMainFrame->m_strDownloadFileExt.GetAt(i);
			if(ext.GetAt(j)!=ch && ch!='?')
			{
				i+=j+1;
				j=0;
			}
			else
			{
				j++;
				i++;
			}
			if(j>=l)
				return TRUE;
		}
		return FALSE;
	}else
		return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Pre-startup code

/*
#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif
*/



