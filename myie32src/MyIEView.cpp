// MyIEView.cpp : implementation of the CMyIEView class
//

#include "stdafx.h"
#include "MyIE.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include <wininet.h>

#include <Mshtmdid.h>
//#include "MyIEDoc.h"
#include "MyIEView.h"
#include "mshtml.h"
#include "SetRefIntervaldlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CMyIEView

IMPLEMENT_DYNCREATE(CMyIEView, CFixedHtmlView)

BEGIN_MESSAGE_MAP(CMyIEView, CFixedHtmlView)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_OPTIONS_LOADIMAGE, OnOptionsLoadimage)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_LOADIMAGE, OnUpdateOptions)
	ON_COMMAND(ID_OPTIONS_ALLOWSCRIPTS, OnOptionsAllowscripts)
	ON_COMMAND(ID_OPTIONS_LOADSOUNDS, OnOptionsLoadsounds)
	ON_COMMAND(ID_OPTIONS_LOADVIDEOS, OnOptionsLoadvideos)
	ON_COMMAND(ID_OPTIONS_USE_WEBP, OnOptionsUseWebp)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_USE_WEBP, OnUpdateOptionsUseWebp)
	ON_COMMAND(ID_OPTIONS_ALLOWACTIVEX, OnOptionsAllowactivex)
	ON_COMMAND(ID_OPTIONS_ALLOWJAVAAPPLET, OnOptionsAllowjavaapplet)
	ON_UPDATE_COMMAND_UI(ID_GO_BACK, OnUpdateGoBack)
	ON_UPDATE_COMMAND_UI(ID_GO_FORWARD, OnUpdateGoForward)
//	ON_WM_ERASEBKGND()
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_ALLOWSCRIPTS, OnUpdateOptions)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_LOADSOUNDS, OnUpdateOptions)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_LOADVIDEOS, OnUpdateOptions)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_ALLOWACTIVEX, OnUpdateOptions)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_ALLOWJAVAAPPLET, OnUpdateOptions)
	ON_WM_SIZE()
	ON_UPDATE_COMMAND_UI(ID_VIEW_FONTS_LARGEST, OnUpdateViewFonts)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FONTS_LARGE, OnUpdateViewFonts)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FONTS_MEDIUM, OnUpdateViewFonts)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FONTS_SMALL, OnUpdateViewFonts)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FONTS_SMALLEST, OnUpdateViewFonts)
	//{{AFX_MSG_MAP(CMyIEView)
	ON_WM_PARENTNOTIFY()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_COMMAND(ID_TOOLS_AUTOREF, OnToolsAutoref)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_AUTOREF, OnUpdateToolsAutoref)
	ON_COMMAND(ID_TOOLS_REFFRE, OnToolsReffre)
	ON_COMMAND(ID_TOOLS_AUTOFILTER_CUR, OnToolsAutofilterCur)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_AUTOFILTER_CUR, OnUpdateToolsAutofilterCur)
	ON_COMMAND(ID_FILE_SAVEAS, OnFileSaveas)
	ON_COMMAND(ID_FILE_SAVEHTML, OnFileSavehtml)
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_FILE_PRINT_SETUP, OnFilePrintSetup)
	ON_COMMAND(ID_FILE_PROPERTY, OnFileProperty)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_FILE_AUTOSAVE, OnFileAutosave)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_GO_BACK, OnGoBack)
	ON_COMMAND(ID_GO_FORWARD, OnGoForward)
//	ON_COMMAND(ID_GO_SEARCH_THE_WEB, OnGoSearchTheWeb)
	ON_COMMAND(ID_GO_START_PAGE, OnGoStartPage)
	/*ON_COMMAND(ID_HELP_WEB_TUTORIAL, OnHelpWebTutorial)
	ON_COMMAND(ID_HELP_ONLINE_SUPPORT, OnHelpOnlineSupport)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_FREE_STUFF, OnHelpMicrosoftOnTheWebFreeStuff)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_FREQUENTLY_ASKED_QUESTIONS, OnHelpMicrosoftOnTheWebFrequentlyAskedQuestions)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_GET_FASTER_INTERNET_ACCESS, OnHelpMicrosoftOnTheWebGetFasterInternetAccess)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_MICROSOFT_HOME_PAGE, OnHelpMicrosoftOnTheWebMicrosoftHomePage)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_SEARCH_THE_WEB, OnHelpMicrosoftOnTheWebSearchTheWeb)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_SEND_FEEDBACK, OnHelpMicrosoftOnTheWebSendFeedback)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_INTERNET_START_PAGE, OnHelpMicrosoftOnTheWebInternetStartPage)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_BEST_OF_THE_WEB, OnHelpMicrosoftOnTheWebBestOfTheWeb) */
//	ON_COMMAND(ID_VIEW_FONTS_LARGEST, OnViewFontsLargest)
//	ON_COMMAND(ID_VIEW_FONTS_LARGE, OnViewFontsLarge)
//	ON_COMMAND(ID_VIEW_FONTS_MEDIUM, OnViewFontsMedium)
//	ON_COMMAND(ID_VIEW_FONTS_SMALL, OnViewFontsSmall)
//	ON_COMMAND(ID_VIEW_FONTS_SMALLEST, OnViewFontsSmallest)
//	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_TOOLS_AUTOSCROLL_AUTOSCROLLINGAFTERDOWNLOADING, OnToolsAutoscrollAutoscrollingafterdownloading)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_AUTOSCROLL_AUTOSCROLLINGAFTERDOWNLOADING, OnUpdateToolsAutoscrollAutoscrollingafterdownloading)
//	ON_COMMAND(ID_TOOLS_AUTOSCROLL_FAST, OnToolsAutoscrollFast)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_AUTOSCROLL_FAST, OnUpdateToolsAutoscrollSpeed)
//	ON_COMMAND(ID_TOOLS_AUTOSCROLL_MEDIUM, OnToolsAutoscrollMedium)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_AUTOSCROLL_MEDIUM, OnUpdateToolsAutoscrollSpeed)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_AUTOSCROLL_SLOW, OnUpdateToolsAutoscrollSpeed)
	ON_COMMAND(ID_TOOLS_AUTOSCROLL_USEAUTOSCROLLING, OnToolsAutoscrollUseautoscrolling)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_AUTOSCROLL_USEAUTOSCROLLING, OnUpdateToolsAutoscrollUseautoscrolling)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyIEView construction/destruction

CMyIEView::CMyIEView()
{
	// TODO: add construction code here
	IsPopUp = FALSE;
	dwProperty = 0;
	m_nFilterDup = 0;
	m_bEnWebProxy = FALSE;

	m_bIsCapture = FALSE;

	IsActive = FALSE;
	m_bReUsed = FALSE;

	m_back= FALSE;
	m_forward = FALSE;

	m_nProgress = -1;

	m_IsNewPopup = FALSE;

	bDelayAdd = TRUE;

	m_nDupID = -1;

	m_bToClose = FALSE;

	m_bIsSecure = FALSE;

	m_nFontSize = nDefFontSize; //4, smallest, 3, small, 2, medium, 1, large, 0, largest

	//load internet setting
	HKEY            hKey;
	TCHAR           sz[MAX_PATH];
	DWORD           dwSize = MAX_PATH;

	if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\Main"), &hKey) != ERROR_SUCCESS)
	{
		TRACE0("Main setting not found\n");
		return ;
	}
	dwSize = sizeof(sz);
	RegQueryValueEx(hKey, _T("Display Inline Images"), NULL, NULL, (LPBYTE)sz, &dwSize);
	if(strcmp(sz, "yes")==0)
		dwProperty |=DLCTL_DLIMAGES;
	
	dwSize = MAX_PATH;
	RegQueryValueEx(hKey, _T("Display Inline Videos"), NULL, NULL, (LPBYTE)sz, &dwSize);
	if(strcmp(sz, "yes")==0)
		dwProperty |=DLCTL_VIDEOS;

	dwSize = MAX_PATH;
	RegQueryValueEx(hKey, _T("Play_Background_Sounds"), NULL, NULL, (LPBYTE)sz, &dwSize);
	if(strcmp(sz, "yes")==0)
		dwProperty |=DLCTL_BGSOUNDS;

	RegCloseKey(hKey);

	if(AfxGetApp()->GetProfileInt("Settings", "NoJava", 0))
		dwProperty |= DLCTL_NO_JAVA;

	if(AfxGetApp()->GetProfileInt("Settings", "NoActivex", 0))
		dwProperty |= DLCTL_NO_RUNACTIVEXCTLS;

	if(AfxGetApp()->GetProfileInt("Settings", "NoScript", 0))
		dwProperty |= DLCTL_NO_SCRIPTS;

	//keep silent???
//	if(bSilent)
//		dwProperty |= DLCTL_SILENT;

	m_bForceRefresh = FALSE;

	m_bFirst = TRUE;

	m_nSrSpeed = 1; //1-slow, 2-medium, 3-fast
	m_bScrollAfterDL = TRUE;
	m_bUseAutoScroll = FALSE;

	m_nScrollTimer = 0;

	oldIcon = -1;

	m_nCurHistory = -1;
	m_nHistoryLength = 0;

	m_bAutoFilter = EnableAutoFilter;

	m_LClickPT.x=0;
	m_LClickPT.y = 0;

	m_StartPoint.x=0;
	m_StartPoint.y=0;
	m_iMouseGS = 0;
	m_iMouseGE = 0;
	m_iMGLen = 4;

	m_nAutoRefreshTimer = 0;
	m_bUseAutoRef = FALSE;
	if(pMainFrame!=NULL)
		m_nRefreshInterval = ((CMainFrame*)pMainFrame)->m_nDefRefreshInterval;
}

CMyIEView::~CMyIEView()
{
	try{
	if(m_nScrollTimer)
		KillTimer(m_nScrollTimer);

	if(m_nAutoRefreshTimer)
		KillTimer(m_nAutoRefreshTimer);
	}catch(...)
	{
	}
}

BOOL CMyIEView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	//cs.style |= WS_CLIPCHILDREN;
	try{
    cs.lpszClass = AfxRegisterWndClass(
       CS_DBLCLKS,                       // if you need double-clicks
       NULL,                             // no cursor (use default)
       NULL,                             // no background brush
       NULL); // app icon
    ASSERT(cs.lpszClass);
	}
	catch(...)
	{
	}
	return CFixedHtmlView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMyIEView drawing


void CMyIEView::OnInitialUpdate(int nOpenType, LPCSTR strUrl)
{
	try{
	m_bFirst = TRUE;
	CFixedHtmlView::OnInitialUpdate();
	if (nOpenType == 0)
	{
		m_lpszUrl = "about:blank";
		Navigate2(m_lpszUrl, 0, NULL);
	}
	else if (nOpenType == 2)
		if(strUrl!=NULL && strUrl!="")
		{
			Navigate2(strUrl, 0, NULL);
			m_lpszUrl = strUrl;
		}
		else
		{
			m_lpszUrl = "about:blank";
			Navigate2(m_lpszUrl, 0, NULL);
		}
	else
		GoHome();

	//nOpenType = 1;

	m_strWebProxy = ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_strDefaultWebProxy;

	}/*catch(CMemoryException * e)
	{
//		AfxMessageBox("initial error");
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}
	// TODO: This code navigates to a popular spot on the web.
	//  change the code to go where you'd like.
	//Navigate2(_T("http://www.microsoft.com/visualc/"),NULL,NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CMyIEView printing


/////////////////////////////////////////////////////////////////////////////
// CMyIEView diagnostics

#ifdef _DEBUG
void CMyIEView::AssertValid() const
{
	CFixedHtmlView::AssertValid();
}

void CMyIEView::Dump(CDumpContext& dc) const
{
	CFixedHtmlView::Dump(dc);
}

/*CMyIEDoc* CMyIEView::GetDocument() // non-debug version is inline
{
//	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyIEDoc)));
//	return (CMyIEDoc*)m_pDocument;
	return NULL;
}*/
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyIEView message handlers
void CMyIEView::OnTitleChange(LPCTSTR lpszText)
{
	// this will change the main frame's title bar
//	if (m_pDocument != NULL )
//		m_pDocument->SetTitle(lpszText);

	try{

	CChildFrame* tcf = ((CChildFrame*)GetParentFrame());
	if(tcf!=NULL)
	{
		if(tcf->m_strStickName != "")
		{
			tcf->SetWindowText(tcf->m_strStickName);
		}
		else
		{
			tcf->SetWindowText(lpszText);
		}
		tcf->m_Title = lpszText;
	}

	CString url("");


	//need pay attention!
	try{
	GetLocationURL(url);
	}catch(...)
	{
	}

/*2.3
	if(!IsPopUp && m_IsNewPopup && EnablePopupFilter && m_IsNewPopup && !url.IsEmpty())
	{
		if(((CMainFrame*)pMainFrame)->m_astrPopup.Find(url)!=NULL || PopFilterStar(url))
		{
			IsPopUp = TRUE;
			m_strPossibleURL = url;
		}
	}
	if(IsPopUp && m_IsNewPopup)
	{
		//is the popup is in a frame or top window
		url2 = url.Right(m_strPossibleURL.GetLength()+13);
		if(url2 == "navcancl.htm#" + m_strPossibleURL /*for IE 5* /|| url == "about:NavigationCanceled" /*for IE4* / || url==m_strPossibleURL)
		{
		//	CChildFrame* tcf = (CChildFrame*)GetParentFrame();
		//	tcf->m_pView->IsActive = FALSE;
			if(tcf!=NULL) 
				tcf->PostMessage(WM_CLOSE);

			return;
		}
		else
			tcf->ShowWindow(SW_SHOW);
	//	IsPopUp = FALSE;
	}
	else if(!tcf->IsWindowVisible())
		tcf->ShowWindow(SW_SHOW);
*/


	BOOL bSetAddress = TRUE;
	if(!url.IsEmpty())
	{
		if(m_lpszUrl == url)
			bSetAddress = FALSE;
		else
			m_lpszUrl = url;
//2.3		IsPopUp=FALSE;

		//for history
		if(m_pITravelLog == NULL)
			SetHistory(url);
	}
	else
		bSetAddress = FALSE;
	if(m_pITravelLog == NULL && strlen(lpszText)>0)
		m_strLastTitle = lpszText;

	if(pMainFrame!=NULL)
	{
		//set the new title of tab
		if(tcf->m_strStickName=="")
		{
			int nTabID = pMainFrame->FindTab(tcf);
			if(strcmp(lpszText, "about:blank")==0)
			{
				CString tab; tab.LoadString(IDS_UNTITLED);
				pMainFrame->SetTabTitle( tab, nTabID );
			}
			else
				pMainFrame->SetTabTitle( (char*)lpszText, nTabID );
		}
		if(bSetAddress && IsActive)
		{
			pMainFrame->SetAddress(url);
		
		}

		//update tab bar
		if(!((CMainFrame*)pMainFrame)->m_bAutoTab)
			pMainFrame->PostMessage(WM_UPDATE_TAB);
	}

//	m_strLastURL = url;

	}
	catch(...)
	{
	}
}

void WBShowScrollBar(CMyIEView* pvw)
{
	try{
		LPDISPATCH pDisp = NULL;

		IHTMLDocument2 *pHTMLDoc = NULL;
		IHTMLElement * pHTMLElem = NULL;
		IHTMLBodyElement *pHTMLBody = NULL;

		try{
		pDisp = pvw->GetHtmlDocument();
		if( pDisp )
		{
			if (SUCCEEDED(pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc)))
			{
				if(SUCCEEDED(pHTMLDoc->get_body( &pHTMLElem )))
				{
					if (SUCCEEDED(pHTMLElem->QueryInterface(IID_IHTMLBodyElement, (void**)&pHTMLBody)))
					{
						pHTMLBody->put_scroll(bstryes);

						//pHTMLBody->Release();
						//pHTMLBody = NULL;
					}
					//pHTMLElem->Release();
					//pHTMLElem = NULL;
				}
				//pHTMLDoc->Release();
				//pHTMLDoc = NULL;
			}
			//pDisp->Release();
			//pDisp = NULL;
		}
		}
		catch(...)
		{//?
 		//	AfxMessageBox("Show scrollbar error");
		}
		try{
			if(pHTMLBody!=NULL)
				pHTMLBody->Release();
			if(pHTMLElem!=NULL)
				pHTMLElem->Release();
			if(pHTMLDoc!=NULL)
				pHTMLDoc->Release();
			if(pDisp!=NULL)
				pDisp->Release();
		}catch(...)
		{
		}
	}catch(...)
	{
	}
}

void CMyIEView::OnDocumentComplete(LPCTSTR lpszUrl)
{
	// make sure the main frame has the new URL.  This call also stops the animation
	try{
//		CString strUrl;
		GetLocationURL(strUrl);
		if(strUrl.IsEmpty() && lpszUrl!=NULL)
			strUrl = lpszUrl;
		m_lpszUrl = strUrl;

		//filter popup
		if(!m_bToClose && !IsPopUp && pMainFrame!=NULL && EnablePopupFilter && !strUrl.IsEmpty()) 
		{
	//2.2 m_IsNewPopup && 
			if((pMainFrame->m_astrPopup.Find(strUrl)!=NULL || PopFilterStar(strUrl)))
			{
				IsPopUp = TRUE;
				m_strPossibleURL = strUrl;
				m_nProgress = -10;
				CChildFrame* tcf = ((CChildFrame*)GetParentFrame());
				if(tcf!=NULL)
				{
					m_bToClose = TRUE;
					tcf->PostMessage(WM_CLOSE);
					return;
				}
			}
		}

	if(glpDisp == NULL) //complete
	{
		m_nProgress = -10;

		if(IsActive && pMainFrame!=NULL && !strUrl.IsEmpty())
		{
			//((CMainFrame*)((CChildFrame*)GetParentFrame()->GetParentFrame()))->SetAddress(strUrl);
			pMainFrame->SetAddress(strUrl);
			percent= -1;	
		}

		//set icon
		if(pMainFrame!=NULL)
		{
			
			if((m_nProgress>95 || m_nProgress<0))//3.2 && !strUrl.IsEmpty()
			{
				CChildFrame* tcf = (CChildFrame*)GetParentFrame();
				int nTabID = pMainFrame->FindTab(tcf);
				int oi = oldIcon;
				if(IsActive)
				{
					if(tcf->bLock)
						oldIcon = pMainFrame->SetTabIcon( 4, nTabID );
					else if(tcf->bProtect)
						oldIcon = pMainFrame->SetTabIcon( 6, nTabID );
					else
						oldIcon = pMainFrame->SetTabIcon( -1, nTabID );

					//start to scroll
					if(!m_nScrollTimer && (m_bUseAutoScroll || pMainFrame->m_bAllScroll) && m_nSrSpeed!=0)
					{
						if(m_nProgress==100 || m_nProgress<0 || (!m_bScrollAfterDL && m_nProgress>=0))
						{
	//						m_nScrollTimer = SetTimer(2132, (UINT)(1000*gSeed/m_nSrSpeed), NULL);
							if(pMainFrame->m_bMouseCtrlScrSpeed)
								m_nLastScrSpd = STOP_TIMER;
							else
								m_nLastScrSpd = (26 + gSeed * 100)/m_nSrSpeed;
							m_nScrollTimer = SetTimer(2132, m_nLastScrSpd, NULL);
						}
					}

				}
				else
					oldIcon = pMainFrame->SetTabIcon( 5, nTabID );
				//update tab bar tab0
				if(pMainFrame->m_bFBTab)
					pMainFrame->PostMessage(WM_UPDATE_TAB);
			}

		}

		//restore homepage
		if(bLockHomepage)
		{
			pMainFrame->GetHomePage(pMainFrame->m_strCurHome);
			if(strcmp(pMainFrame->m_strHome, pMainFrame->m_strCurHome)!=0)
				pMainFrame->SetHomePage(pMainFrame->m_strHome);
		}
	}

	//scroll bar and background
	try{
		if(m_IsNewPopup &&(m_nProgress==100 || m_nProgress<0))//3.2!strUrl.IsEmpty() && 
		{
			WBShowScrollBar(this);
		}

		if(bAutoSetBG)
		{
			VARIANT color;
			color.vt = VT_BSTR;
			color.bstrVal = pMainFrame->m_strBGColor.AllocSysString();

			pMainFrame->SetBackground(this, color);
			SysFreeString(color.bstrVal);
		}
	}
	catch(...)
	{
	}

	}
	catch(...)
	{
	}
}


void CMyIEView::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD /*nFlags*/,
	LPCTSTR lpszTargetFrameName, BOOL baPostedData,
	LPCTSTR /*lpszHeaders*/, BOOL* pbCancel)
{
	//open in new window
	//get time
	LARGE_INTEGER liFinish;
	LARGE_INTEGER liFreq;
	BOOL bCanNew = FALSE;
	TCITEM TabCtrlItem;
	try
	{
	CChildFrame* tcf = (CChildFrame*)GetParentFrame();
	CMainFrame* mf = (CMainFrame*)(tcf->GetParentFrame());

	BOOL bLock = FALSE;
	if(tcf!=NULL)
		bLock = tcf->bLock;

	if(bLinkInNewWindow || bLock)
	{
		if(m_tLastClick.LowPart != 0 && !baPostedData)//2.5.GetSize()==0
		{
			QueryPerformanceCounter(&liFinish);
			QueryPerformanceFrequency(&liFreq);
			if(liFreq.LowPart!=0)
			{
				double secs = (double)(liFinish.LowPart - m_tLastClick.LowPart)/(double)liFreq.LowPart;
				if(secs<0.6 && secs>0)
				{
					int dup = ((CMainFrame*)pMainFrame)->FindDupURL(lpszURL, tcf);
					if(dup<0)
						bCanNew = TRUE;
					else
					{
						*pbCancel = TRUE;
						if(mf!=NULL && mf->m_bActiveNewWindow)
						{
							TabCtrlItem.mask = TCIF_PARAM;

							mf->m_wndTab.GetItem(dup, &TabCtrlItem);
							CChildFrame* tcf2 = (CChildFrame*)TabCtrlItem.lParam;
							if(tcf2!=NULL && tcf2!=tcf)
							{
								mf->MDIActivate(tcf2);
								//((CMainFrame*)pMainFrame)->m_wndTab.SetCurSel(m_nDupID);
							}
						}
					}
				}
				else
					m_tLastClick.LowPart = 0;
			}
		}


		if(!m_bFirst && pMainFrame!=NULL && bCanNew)
		{
			char* pdest = strchr(lpszURL, '#');
			if(pdest!=NULL)
			{
				if(strncmp(lpszURL, m_lpszUrl, pdest - lpszURL)!=0)
					pdest = NULL;
			}
			if(strstr(lpszURL, "mailto:")==NULL && strstr(lpszURL, "javascript:")==NULL && pdest == NULL )
			{
				CWinApp* pApp = AfxGetApp();
				CChildFrame *pWnd = NULL;
				if(pApp==NULL)
					return;
				pWnd = ((CMainFrame*)pApp->m_pMainWnd)->NewChildWindow(0,1, NULL, this);
				if(pWnd != NULL)
				{
					pWnd->m_pView->dwProperty = dwProperty;

					pWnd->m_pView->m_IsNewPopup = FALSE; //TRUE;
					
					pWnd->m_pView->m_bFirst = TRUE;
					pWnd->m_pView->m_nProgress = 0;

					pWnd->m_pView->ToNavigate(lpszURL);

					if(pbCancel!=NULL)
						*pbCancel = TRUE;

					return;
				}
			}

		}

	}//end lock and open new

	m_bFirst = FALSE;

	m_bForceRefresh =  FALSE;



	//filter popup
	if(pMainFrame!=NULL && EnablePopupFilter && lpszURL!=NULL && strcmp(lpszURL, "about:blank")!=0)
	{
		//mutex
//		WaitForSingleObject(m_hMutex, INFINITE);
//2.2 m_IsNewPopup && 
		if((pMainFrame->m_astrPopup.Find(lpszURL)!=NULL || PopFilterStar(lpszURL)))
		{
			if(pbCancel!=NULL)
				*pbCancel = TRUE;
			IsPopUp = TRUE;
			m_strPossibleURL = lpszURL;
			m_nProgress = -1;
		}
		else
		{
			m_nProgress = 0;
			if(IsActive)
			{	
				percent = m_nProgress;
			}
		}

	}
	
	//2.3 329 filter duplicate url
	if(EnableDupFilter && m_nFilterDup && !IsPopUp && m_IsNewPopup && pMainFrame!=NULL && lpszURL!=NULL && strcmp(lpszURL, "about:blank")!=0)
	{
		//mutex
//		WaitForSingleObject(m_hMutex, INFINITE);
		//only filter once the window popup
		m_nFilterDup --;
		int dup = ((CMainFrame*)pMainFrame)->FindDupURL(lpszURL, tcf);
		if(dup>=0)
		{
			if(pbCancel!=NULL)
				*pbCancel = TRUE;
			IsPopUp = TRUE;
			m_strPossibleURL = lpszURL;
			m_nDupID = dup;
			m_nProgress = -1;
			m_nFilterDup = 0;

			//active that dup window
			if(m_bReUsed && tcf!=NULL && mf!=NULL && (mf->m_bActiveNewWindow || tcf->m_bForceActive))
			{
			//	TCITEM TabCtrlItem;
				TabCtrlItem.mask = TCIF_PARAM;

				((CMainFrame*)pMainFrame)->m_wndTab.GetItem(m_nDupID, &TabCtrlItem);
				CChildFrame* tcf2 = (CChildFrame*)TabCtrlItem.lParam;
				if(tcf2!=NULL && tcf2!=tcf)
				{
					((CMainFrame*)pMainFrame)->MDIActivate(tcf2);
					//((CMainFrame*)pMainFrame)->m_wndTab.SetCurSel(m_nDupID);
				}

				m_nDupID = -1;
			}

		}
		else
		{
			m_nProgress = 0;
			if(IsActive)
			{	
				percent = m_nProgress;
			}
		}

	}

	//secure info
	if(strncmp(lpszURL, "https", 5)!=0)
		m_bIsSecure = FALSE;

	if(IsActive && pMainFrame!=NULL)
		pMainFrame->m_bIsSecure = m_bIsSecure;


/*	if(bDelayAdd && m_IsNewPopup && !IsPopUp && tcf!=NULL)
	{
	//resize
/*		CRect rect, oldrect;
		m_wndBrowser.GetWindowRect(&oldrect);
		GetParentFrame()->GetClientRect(&rect);
		rect.left-=2;
		rect.top-=2;
		rect.right +=2;
		rect.bottom +=2;
		if(oldrect.Width()!=rect.Width() || oldrect.Height()!=rect.Height())
		{
			m_wndBrowser.MoveWindow(rect);
		}* /
		try{
		CRect rect;
		tcf->GetClientRect(&rect);
		rect.left-=2;
		rect.top-=2;
		rect.right +=2;
		rect.bottom +=2;
		//m_wndBrowser.MoveWindow(rect);
		SendMessage(WM_SIZE,  0, ((rect.Height())<<16)+rect.Width());
		}catch(...)
		{
		}
	}*/

	//reused window
	if(!IsPopUp && m_bReUsed && tcf!=NULL && mf!=NULL && (mf->m_bActiveNewWindow || tcf->m_bForceActive))
	{
		int nTabID = mf->FindTab(tcf);
		//active window
		mf->MDIActivate(tcf);
		//select current tab
		mf->m_wndTab.SetCurSel( nTabID );
			
		tcf->m_bForceActive = FALSE;

	}
	m_bReUsed = FALSE;

	//Add a tab to tabbar
	if(!IsPopUp && bDelayAdd && tcf!=NULL && mf!=NULL)
	{
		tcf->ShowWindow(SW_SHOW);

		bDelayAdd = FALSE;


		//add tab
		mf->AddNewTab(tcf);

		int nTabID = mf->FindTab(tcf);
		//active window
		if(mf->m_bActiveNewWindow || tcf->m_bForceActive)
		{
			mf->MDIActivate(tcf);
			//select current tab
			mf->m_wndTab.SetCurSel( nTabID );
			
			tcf->m_bForceActive = FALSE;
		}

		//set icon
		oldIcon = mf->SetTabIcon( 0, nTabID );

		//update tab bar
		if(mf->m_bFBTab)
			mf->PostMessage(WM_UPDATE_TAB);
//		bUpdateTab = TRUE;
		mf->PostMessage(WM_UPDATE_TAB_TIP);
	}

	}
/*	catch(CMemoryException* e)
	{
//		AfxMessageBox("before nav error");
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}
}

// these are all simple one-liners to do simple controlling of the browser
void CMyIEView::OnGoBack()
{
	if(m_back)
		GoBack();
}

void CMyIEView::OnGoForward()
{
	if(m_forward)
		GoForward();
}

void CMyIEView::OnGoStartPage()
{
	GoHome();
}



// these functions control the font size.  There is no explicit command in the
// CFixedHtmlView class to do this, but we can do it by using the ExecWB() function.
/*void CMyIEView::OnViewFontsLargest()
{
	nDefFontSize = m_nFontSize = 0;
	COleVariant vaZoomFactor(4l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);

}

void CMyIEView::OnViewFontsLarge()
{
	nDefFontSize = m_nFontSize = 1;

	COleVariant vaZoomFactor(3l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);

}

void CMyIEView::OnViewFontsMedium()
{
	nDefFontSize = m_nFontSize = 2;
	COleVariant vaZoomFactor(2l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);

}

void CMyIEView::OnViewFontsSmall()
{
	nDefFontSize = m_nFontSize = 3;

	COleVariant vaZoomFactor(1l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);

}

void CMyIEView::OnViewFontsSmallest()
{
	nDefFontSize = m_nFontSize = 4;
	COleVariant vaZoomFactor(0l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);

}
*/

void CMyIEView::OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel) 
{
	// TODO: Add your specialized code here and/or call the base class

	CChildFrame *pWnd = NULL;

	if(ppDisp == NULL || Cancel == NULL)
		return;

	//mutex
//	WaitForSingleObject(m_hMutex, INFINITE);

	try{
	*Cancel = TRUE;
	*ppDisp = NULL;

	//auto filter popup window not opened by user
	if(m_bAutoFilter)
	{
		//this window is to be closed, so don't open any other windows from it.
		if(m_bToClose)
			return;
		
		//3.2
		AfxGetApp()->WriteProfileInt("Settings", "InAutoFilter", 1);

		LARGE_INTEGER liFinish;
		LARGE_INTEGER liFreq;
		BSTR bstr=NULL;

		QueryPerformanceCounter(&liFinish);
		QueryPerformanceFrequency(&liFreq);
		if(liFreq.LowPart!=0)
		{
			double secs = (double)(liFinish.LowPart - m_tLastClick.LowPart)/(double)liFreq.LowPart;
			double Rsecs = (double)(liFinish.LowPart - m_tLastRClick.LowPart)/(double)liFreq.LowPart;

	//		tag = "";
			if((secs>0.7 && secs<4.0 && m_bLClick) || /*2.5*/ m_nProgress==-1)
			{
				if(m_nProgress>=0)/*2.5*/
				{
					//get click point
					IHTMLElement* pElem = NULL;
					IHTMLDocument2* pHTMLDoc=NULL;
					try{
					LPDISPATCH pDisp = GetHtmlDocument();

					if(pDisp!=NULL)
					{
						if(SUCCEEDED(pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc)))
						{
							if(pHTMLDoc!=NULL)
							{
								if(SUCCEEDED(pHTMLDoc->elementFromPoint(m_LClickPT.x, m_LClickPT.y, &pElem)))
								{
									if(pElem!=NULL)
									{
										pElem->get_tagName(&bstr);
										pElem->Release();
									}
								}
								pHTMLDoc->Release();
							}
						}
						pDisp->Release();
					}
					}catch(...)
					{
					}


				}
				else
				{	
					//2.5 whether browser is busy when m_mPorgress <0
					try{
						if(m_wndBrowser.GetBusy())
							m_nProgress=0;
					}catch(...)
					{
					}
				}
			}
			try{

			if(!(m_bLClick && ((secs<=0.7 && secs>=0) || (bstr!=NULL && wcscmp(bstr, L"A")==0) || /*2.5*/m_nProgress==-1)) && !((Rsecs>=0 && Rsecs<=5 && m_bRClick) ||/*2.5*/ m_nProgress==-1))
			{
				*ppDisp = NULL;
				m_bLClick = FALSE;
				//3.01 tag == "";
				if(bstr!=NULL)
					SysFreeString(bstr);
				//3.2
				AfxGetApp()->WriteProfileInt("Settings", "InAutoFilter", 0);
				return;
			}
			if(bstr!=NULL)
			{
					SysFreeString(bstr);
				
			}
			
			}catch(...)
			{}
		}
		//3.2
		AfxGetApp()->WriteProfileInt("Settings", "InAutoFilter", 0);
	}

	m_bLClick = FALSE;
//	tag == "";
	if(pMainFrame!=NULL)
		pWnd = pMainFrame->NewChildWindow(0,1, NULL, this);

	//mutex
//	ReleaseMutex(m_hMutex);

	if(pWnd!=NULL)
	{
		//this maybe a blank window.
		if(((CChildFrame*)GetParentFrame())->m_pBlankChild != pWnd)
		{
			((CChildFrame*)GetParentFrame())->m_pBlankChild = pWnd;
			QueryPerformanceCounter(&((CChildFrame*)GetParentFrame())->m_tBlankChild);
		}
		else
			((CChildFrame*)GetParentFrame())->m_pBlankChild = NULL;

		pWnd->m_pView->dwProperty = dwProperty;

		pWnd->m_pView->m_IsNewPopup = TRUE;
		pWnd->m_pView->IsPopUp = FALSE;
		pWnd->m_pView->m_bToClose = FALSE;

		pWnd->m_pView->m_nFilterDup = 1;

		pWnd->m_pView->m_bForceResize = TRUE;

		pWnd->m_pView->m_nProgress = 0;
		pWnd->m_pView->m_bAutoFilter = m_bAutoFilter;

		*ppDisp = pWnd->m_pView->GetApplication();
	}

	if(*ppDisp != NULL)
		*Cancel = FALSE;

	}
/*	catch(CMemoryException* e)
	{
//		AfxMessageBox("new window error");
		*Cancel=TRUE;
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}


 	//CFixedHtmlView::OnNewWindow2(ppDisp, Cancel);
	
}





void CMyIEView::OnProgressChange(long nProgress, long nProgressMax) 
{
	// TODO: Add your specialized code here and/or call the base class
	try{

	int oldp = m_nProgress;
	if(nProgressMax>0 && nProgress>=0 && nProgressMax/100>0)
	{
		m_nProgress  = nProgress/(nProgressMax/100);
		if(m_nProgress>100)
			m_nProgress=100;
	}
	else
	{
		m_nProgress = -10;
//b5
//		CString strUrl2;
//		GetLocationURL(strUrl2);
//		if((strUrl2 != "" && !strUrl2.IsEmpty()))
//			m_lpszUrl = strUrl2;
	}

	if(IsActive)
		percent = m_nProgress;

	if(IsActive && (m_bUseAutoScroll || pMainFrame->m_bAllScroll) )
	{	

		//start to scroll
		if(m_nScrollTimer==0 && (m_bUseAutoScroll || pMainFrame->m_bAllScroll) && m_nSrSpeed!=0)
		{
			if(m_nProgress==100 || m_nProgress<0 || (!m_bScrollAfterDL && m_nProgress>=0))
			{
//				m_nScrollTimer = SetTimer(2132, (UINT)(1000*gSeed/m_nSrSpeed), NULL);
				if(pMainFrame->m_bMouseCtrlScrSpeed)
					m_nLastScrSpd = STOP_TIMER;
				else
					m_nLastScrSpd = (26 + gSeed * 100)/m_nSrSpeed;
				m_nScrollTimer = SetTimer(2132, m_nLastScrSpd, NULL);
			}
		}
		else if(m_nScrollTimer && m_nProgress<20 && m_nProgress>0)
		{
			//stop the timer;
			if(m_nScrollTimer)
				KillTimer(m_nScrollTimer);
			m_nScrollTimer = 0;

		}

	}


	//set icon
	if(pMainFrame!=NULL)
	{
		int nIcon = m_nProgress/25;
		int oi = oldIcon;
		if(m_nProgress<0)
			nIcon=-1;
		if(nIcon==4)
			nIcon=3;

		if((m_nProgress>=0 && nIcon != oi) || m_nProgress<0)
		{
			int nTabID;
			nTabID = pMainFrame->FindTab((CChildFrame*)GetParentFrame());
			if(m_nProgress>=0 && nIcon != oi)
				oldIcon = pMainFrame->SetTabIcon( nIcon, nTabID );
			else if(m_nProgress<0)
			{
				if(!IsActive)
					oldIcon = pMainFrame->SetTabIcon( 5, nTabID );
				else if(((CChildFrame*)GetParentFrame())->bLock)
					oldIcon = pMainFrame->SetTabIcon( 4, nTabID );
				else if(((CChildFrame*)GetParentFrame())->bProtect)
					oldIcon = pMainFrame->SetTabIcon( 6, nTabID );
				else
					oldIcon = pMainFrame->SetTabIcon( -1, nTabID );
			}

			//update tab bar
			if(m_nProgress*oldp<=0 && oi*oldIcon<=0 && oldIcon != oi && pMainFrame->m_bFBTab)
				pMainFrame->PostMessage(WM_UPDATE_TAB);
		}
	}

	}/*catch(CException * e)
	{
		if(e!=NULL)e->Delete();
	//	AfxMessageBox("progress error");
	}*/
	catch(...)
	{
	}

//	CFixedHtmlView::OnProgressChange(nProgress, nProgressMax);
}


void CMyIEView::OnEditCopy() 
{
	// TODO: Add your command handler code here
	ExecWB(OLECMDID_COPY, OLECMDEXECOPT_DONTPROMPTUSER,
	   NULL, NULL);

}

void CMyIEView::OnEditCut() 
{
	// TODO: Add your command handler code here
	ExecWB(OLECMDID_CUT, OLECMDEXECOPT_DONTPROMPTUSER,
	   NULL, NULL);

}

void CMyIEView::OnEditFind() 
{
	// TODO: Add your command handler code here
	((CMainFrame*)pMainFrame)->ExecCmdTarget(HTMLID_FIND, this);
}

void CMyIEView::OnEditPaste() 
{
	// TODO: Add your command handler code here
	ExecWB(OLECMDID_PASTE, OLECMDEXECOPT_DONTPROMPTUSER,
	   NULL, NULL);
	
}

void CMyIEView::OnEditSelectAll() 
{
	// TODO: Add your command handler code here
	ExecWB(OLECMDID_SELECTALL, OLECMDEXECOPT_DONTPROMPTUSER,
	   NULL, NULL);
	
}

void CMyIEView::OnStatusTextChange(LPCTSTR lpszText) 
{
	// TODO: Add your specialized code here and/or call the base class
	try
	{
	if(IsActive && pMainFrame!=NULL)
	{
		if(bURLOnly && lpszText!=NULL && strlen(lpszText)!=0)
		{
			char* pdes = strstr(lpszText, "about:");
			if(pdes!=lpszText)
			{
				pdes = strstr(lpszText, "://");
				if(pdes == NULL)
				{
					pdes = strstr(lpszText, "mailto:");
					if(pdes == NULL)
					{
						pdes = strstr(lpszText, "javascript:");
					}
				}
			}
			int pos =0;
			if(pdes != NULL)
			{
				pos = pdes - lpszText;
			}
			if(strncmp(lpszText,"Shortcut", 8)!=0 && strstr(lpszText, "的快捷方式")==NULL && strstr(lpszText, "地址已")==NULL && strcmp(lpszText, "Done")!=0&& (pdes==NULL || pos>10 )
				&& strncmp(lpszText, "正在", 4)!=0 && strncmp(lpszText, "完成", 4)!=0 && strncmp(lpszText, "完毕", 4)!=0 && strstr(lpszText, "项剩余")==NULL 
				&& strncmp(lpszText, "Wait", 4)!=0 && strncmp(lpszText, "Start", 5)!=0 && strncmp(lpszText, "Open", 4)!=0 && strncmp(lpszText, "Connect", 7)!=0 && strncmp(lpszText,"Find",4)!=0 && strncmp(lpszText, "download", 8)!=0 && strncmp(lpszText, "Web", 3)!=0 && strstr(lpszText, "remain")==NULL)
				return;
		}
		if( !( pMainFrame->m_bStatusBlank && lpszText!=NULL && strlen(lpszText)==0))
			pMainFrame->SetMessageText(lpszText);
		if(strlen(lpszText)==0)
			pMainFrame->m_bStatusBlank = TRUE;
		else
			pMainFrame->m_bStatusBlank = FALSE;

	}
	}/*catch(CException * e)
	{
		if(e!=NULL)e->Delete();
//		AfxMessageBox("status text error");
	}*/
	catch(...)
	{
	}
}

BOOL CMyIEView::OnAmbientProperty(COleControlSite* pSite, DISPID dispid, VARIANT* pvar) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (dispid == DISPID_AMBIENT_DLCONTROL)
	{
//	  dwProperty |= DLCTL_SILENT;
//	  if(!bSilent)
//		  dwProperty -= DLCTL_SILENT;

	  pvar->vt = VT_I4;
	  pvar->lVal = dwProperty; 
	  //DLCTL_DLIMAGES  | DLCTL_NO_SCRIPTS | DLCTL_NO_JAVA
	//				 | DLCTL_NO_RUNACTIVEXCTLS | DLCTL_NO_DLACTIVEXCTLS;
	  return TRUE;

	}


	return CFixedHtmlView::OnAmbientProperty(pSite, dispid, pvar);
	

}

void CMyIEView::OnOptionsLoadimage() 
{
	// TODO: Add your command handler code here
	dwProperty ^= DLCTL_DLIMAGES;
//	m_bForceRefresh = TRUE;
	ChangeProperty();
	Refresh();
}

void CMyIEView::OnUpdateOptions(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	try
	{

	switch (pCmdUI->m_nID)
	{
	case ID_OPTIONS_LOADIMAGE:
	/*	if(dwProperty&DLCTL_DLIMAGES)
			pCmdUI->SetCheck(1);
		else
			pCmdUI->SetCheck(0);*/
		((CMainFrame*)pMainFrame)->ToolBarSetCheck(pCmdUI, (dwProperty&DLCTL_DLIMAGES)!=0);
		/*
		if(dwProperty&DLCTL_DLIMAGES)
		{
			if(((CMainFrame*)pMainFrame)->m_strBGPic == "" || pCmdUI->m_pMenu !=NULL)
				pCmdUI->SetCheck();
			else
			{
				pCmdUI->SetCheck(0);
				((CMainFrame*)pMainFrame)->m_wndToolBar.GetToolBarCtrl().PressButton(pCmdUI->m_nID, TRUE);
			}
		}
		else
		{
			((CMainFrame*)pMainFrame)->m_wndToolBar.GetToolBarCtrl().PressButton(pCmdUI->m_nID, FALSE);
			pCmdUI->SetCheck(0);
		}*/
		break;
	case ID_OPTIONS_LOADVIDEOS:
		if(dwProperty&DLCTL_VIDEOS)
			pCmdUI->SetCheck(1);
		else
			pCmdUI->SetCheck(0);
		break;
	case ID_OPTIONS_LOADSOUNDS:
		if(dwProperty&DLCTL_BGSOUNDS)
			pCmdUI->SetCheck(1);
		else
			pCmdUI->SetCheck(0);
		break;
	case ID_OPTIONS_ALLOWSCRIPTS:
		if(dwProperty&DLCTL_NO_SCRIPTS)
			pCmdUI->SetCheck(0);
		else
			pCmdUI->SetCheck(1);
		break;
	case ID_OPTIONS_ALLOWJAVAAPPLET:
		if(dwProperty&DLCTL_NO_JAVA)
			pCmdUI->SetCheck(0);
		else
			pCmdUI->SetCheck(1);
		break;
	case ID_OPTIONS_ALLOWACTIVEX:
		if(dwProperty&DLCTL_NO_RUNACTIVEXCTLS)
			pCmdUI->SetCheck(0);
		else
			pCmdUI->SetCheck(1);
		break;
	}

	}
	catch(...)
	{
	//	AfxMessageBox("update option error");
	}

}

void CMyIEView::OnOptionsAllowscripts() 
{
	// TODO: Add your command handler code here
	dwProperty ^= DLCTL_NO_SCRIPTS;
	ChangeProperty();
	Refresh();

}

void CMyIEView::OnOptionsLoadsounds() 
{
	// TODO: Add your command handler code here
	dwProperty ^= DLCTL_BGSOUNDS;
	ChangeProperty();
	Refresh();
	
}

void CMyIEView::OnOptionsLoadvideos() 
{
	// TODO: Add your command handler code here
	dwProperty ^= DLCTL_VIDEOS;
	ChangeProperty();
	
}

void CMyIEView::OnOptionsUseWebp() 
{
	// TODO: Add your command handler code here
	if(m_bEnWebProxy)
		m_bEnWebProxy = FALSE;
	else
	{
		m_bEnWebProxy = TRUE;
		//auto refresh
		if(!m_lpszUrl.IsEmpty() && m_lpszUrl!="about:blank")
		{
			ToNavigate(m_lpszUrl, 0, NULL, NULL, NULL, 0, TRUE);
		}
	}
}

void CMyIEView::OnUpdateOptionsUseWebp(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	try
	{
		((CMainFrame*)pMainFrame)->ToolBarSetCheck(pCmdUI, m_bEnWebProxy);
	}
	catch(...)
	{
	}
	/*
	if(m_bEnWebProxy)
	{
		if(((CMainFrame*)pMainFrame)->m_strBGPic == "" || pCmdUI->m_pMenu !=NULL)
			pCmdUI->SetCheck();
		else
		{
			pCmdUI->SetCheck(0);
			((CMainFrame*)pMainFrame)->m_wndToolBar.GetToolBarCtrl().PressButton(pCmdUI->m_nID, TRUE);
		}
	}
	else
	{
		((CMainFrame*)pMainFrame)->m_wndToolBar.GetToolBarCtrl().PressButton(pCmdUI->m_nID, FALSE);
		pCmdUI->SetCheck(0);
	}*/

}

void CMyIEView::OnOptionsAllowactivex() 
{
	// TODO: Add your command handler code here
	dwProperty ^= DLCTL_NO_RUNACTIVEXCTLS;
	ChangeProperty();

}

void CMyIEView::OnOptionsAllowjavaapplet() 
{
	// TODO: Add your command handler code here
	dwProperty ^= DLCTL_NO_JAVA;
	ChangeProperty();

}


void CMyIEView::OnCommandStateChange(long nCommand, BOOL bEnable) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(nCommand == 2)
		m_back = bEnable;
	else if(nCommand == 1)
		m_forward = bEnable;

	//	CFixedHtmlView::OnCommandStateChange(nCommand, bEnable);
}

void CMyIEView::OnUpdateGoBack(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	try{
	pCmdUI->Enable(m_back);
	}catch(...)
	{
	}
}

void CMyIEView::OnUpdateGoForward(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	try{
	pCmdUI->Enable(m_forward);
	}catch(...)
	{
	}

}




/*BOOL CMyIEView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if (CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	return FALSE;
}*/

void ResizeSibling(CMyIEView* pvw)
{
	try{
	m_bMax = 0;
	CChildFrame* tcf;
	CRect oldrect,rect;
	if(pMainFrame!=NULL)
	{
		int n = ((CMainFrame*)pMainFrame)->m_wndTab.GetItemCount();
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;

		for(int i=n-1;i>=0;i--)
		{
			((CMainFrame*)pMainFrame)->m_wndTab.GetItem(i, &TabCtrlItem);
			tcf = ((CChildFrame*)TabCtrlItem.lParam);
			if(tcf != NULL && tcf->m_pView!=NULL)
			{
				if(tcf->m_pView != pvw)
				{
					tcf->GetClientRect(&rect);
					tcf->m_pView->m_wndBrowser.GetWindowRect(&oldrect);
					tcf->m_pView->ScreenToClient(&oldrect);
					if(oldrect.left != -BORDER && oldrect.top != -BORDER)
					{
						try{
						tcf->m_pView->m_wndBrowser.MoveWindow(-2,-2, rect.Width()+BORDER+BORDER, rect.Height()+BORDER+BORDER);
						}catch(...)
						{
						}
					}
					else
					{
						try{
						if(oldrect.Width()!=rect.Width()+BORDER+BORDER && rect.Width()!=0)
							tcf->m_pView->SetWidth (rect.Width()+BORDER+BORDER);
						if(oldrect.Height()!=rect.Height()+BORDER+BORDER && rect.Height()!=0)
							tcf->m_pView->SetHeight (rect.Height()+BORDER+BORDER);
						}catch(...)
						{
						}
					}
				}
			}
		}
	}

	}/*catch(CMemoryException * e)
	{
		if(e!=NULL)e->Delete();
//		AfxMessageBox("resize sibling error");
	}*/
	catch(...)
	{
	}

}


void CMyIEView::OnSize(UINT nType, int cx, int cy) 
{
//	if (nType != SIZE_MINIMIZED )
	try{
		if(m_bMax==1)
		{
			if(GetParentFrame()->IsZoomed())
			{
				CRect oldrect;
				m_wndBrowser.GetWindowRect(&oldrect);
				ScreenToClient(&oldrect);
/*				if(oldrect.Width()!=cx+4 && oldrect.Height()!=cy+4)
					m_wndBrowser.MoveWindow(-2,-2, cx+4, cy+4);
				else if(oldrect.Width()!=cx+4 )
					m_wndBrowser.SetWidth (cx+4);
				else if(oldrect.Height()!=cy+4)
					m_wndBrowser.SetHeight (cy+4);*/
				if(oldrect.left != -BORDER && oldrect.top != -BORDER)
				{
					try{
					m_wndBrowser.MoveWindow(-2,-2, cx+BORDER+BORDER, cy+BORDER+BORDER);
					}catch(...)
					{
					}
				}
				else
				{
					try{
					if(oldrect.Width()!=cx+BORDER+BORDER )
						SetWidth (cx+BORDER+BORDER);
					if(oldrect.Height()!=cy+BORDER+BORDER)
						SetHeight (cy+BORDER+BORDER);
					}catch(...)
					{
					}
				}
			}
		}
		else
		{
				CRect oldrect;
				m_wndBrowser.GetWindowRect(&oldrect);
				ScreenToClient(&oldrect);
//				if(oldrect.Width()!=cx+4 && oldrect.Height()!=cy+4)
				if(oldrect.left != -BORDER && oldrect.top != -BORDER)
				{
					try{
					m_wndBrowser.MoveWindow(-2,-2, cx+BORDER+BORDER, cy+BORDER+BORDER);
					}catch(...)
					{
					}
				}
				else
				{
					try{
					if(oldrect.Width()!=cx+BORDER+BORDER && cx!=0)
						SetWidth (cx+BORDER+BORDER);
					if(oldrect.Height()!=cy+BORDER+BORDER && cy!=0)
						SetHeight (cy+BORDER+BORDER);
					}catch(...)
					{
					}
				}
				if(GetParentFrame()->GetStyle() & WS_MAXIMIZE)
					m_bMax = 1;
		}

		//size other views
		if(m_bMax == 3)
			ResizeSibling(this);

	}/*catch(CMemoryException * e)
	{
//		AfxMessageBox("resize error");
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}
	
}

void CMyIEView::OnUpdateViewFonts(CCmdUI* pCmdUI) 
{ 
	// TODO: Add your command update UI handler code here
	if(pCmdUI->m_nID==ID_VIEW_FONTS_LARGEST+m_nFontSize)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

BOOL CMyIEView::PopFilterStar(LPCSTR lpszURL)
{
	CString str, url(lpszURL),tmp, tmp2;

	try{

	POSITION pos = ((CMainFrame*)pMainFrame)->m_astrPopup.GetHeadPosition();
//	url = lpszURL;
	int i = 0, i2=0, start = 0, start2 = 0;//start - str, start2 - tmp
	BOOL cmode = TRUE; //compare mode;
	BOOL isame = FALSE;
	while(pos!=NULL && !isame)
	{
		str = ((CMainFrame*)pMainFrame)->m_astrPopup.GetAt(pos);
		if (str.Find('*')>=0 && str!="*")
		{
			//star filter
			i = 0; i2 = 0; start = 0; start2 = 0;
			isame = TRUE;cmode = TRUE;
			while(start<str.GetLength() && isame)
			{
				//get the string before *
				i=str.Find('*', start);
				if(i<0)
				{
					tmp2 = str.Mid(start);
					i=str.GetLength();
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
		}

		((CMainFrame*)pMainFrame)->m_astrPopup.GetNext(pos);
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

void CMyIEView::ToNavigate(LPCTSTR URL, DWORD dwFlags, LPCTSTR lpszTargetFrameName, LPCTSTR lpszHeaders, LPVOID lpvPostData,  DWORD dwPostDataLen, BOOL bOnlyRefresh)
{
	try{

//	CString str;
	str = URL;
	str2 = URL;
	m_lpszUrl=URL;

	m_bFirst = TRUE;

//2.2	if(dwFlags==0)
//		dwFlags = 4;

	if(m_bEnWebProxy)
	{
		if(m_strWebProxy.IsEmpty() && pMainFrame!=NULL)
			m_strWebProxy = ((CMainFrame*)pMainFrame)->m_strDefaultWebProxy;

		if((str.Left(4)=="http" || str.Left(4)=="www.") && str.Find(m_strWebProxy)<0)
		{
			if(str.Left(4)=="www.")
				str2 = "http://" + str;
			if(m_strWebProxy.Find("%s")>0)
			{
				str2.Replace(":", "%3A");
				str2.Replace("/", "%2F");
				str = m_strWebProxy;
				str.Replace("%s", str2);
			}
			else if(m_strWebProxy.Find("%S")>0)
			{
				str = m_strWebProxy;
				str.Replace("%S", str2);
			}
			else
				str = m_strWebProxy+str2;
			Navigate2(str, dwFlags, lpszTargetFrameName, lpszHeaders,lpvPostData, dwPostDataLen);
		}
		else if(!bOnlyRefresh)
			Navigate2(str, dwFlags, lpszTargetFrameName, lpszHeaders,lpvPostData, dwPostDataLen);
	}
	else if(!bOnlyRefresh)
		Navigate2(str, dwFlags, lpszTargetFrameName, lpszHeaders,lpvPostData, dwPostDataLen);
	}/*catch(CMemoryException * e)
	{
//		AfxMessageBox("to nav error");
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}

	m_nProgress = -1;
}



void CMyIEView::OnParentNotify(UINT message, LPARAM lParam) 
{
	CFixedHtmlView::OnParentNotify(message, lParam);
	
	// TODO: Add your message handler code here
	try{
	//336
	CChildFrame* tcf = (CChildFrame*)GetParentFrame();

	if((message == WM_LBUTTONDOWN || message == WM_RBUTTONDOWN) && (bLinkInNewWindow || tcf->bLock || m_bAutoFilter) )
	{
/*		BSTR bstr=NULL;
		IHTMLElement* pElem = NULL;
		IHTMLDocument2* pHTMLDoc=NULL;
		try{
		LPDISPATCH pDisp = GetHtmlDocument();

		if(pDisp!=NULL)
		{
			if(SUCCEEDED(pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc)))
			{
				if(SUCCEEDED(pHTMLDoc->elementFromPoint(LOWORD((DWORD)lParam), HIWORD((DWORD)lParam), &pElem)))
				{
					pElem->get_tagName(&bstr);
					pElem->Release();
				}
				pHTMLDoc->Release();
			}
			pDisp->Release();
		}
		}catch(...)
		{
		}

		CString tag("");

		try{
		if(bstr!=NULL)
		{
			CString t(bstr);
			tag = t;
			SysFreeString(bstr);
		}
		}catch(...)
		{}
*/
		if(message == WM_LBUTTONDOWN )
		{	//get time
			QueryPerformanceCounter(&m_tLastClick);
			m_bRClick = FALSE;
			m_bLClick = TRUE;
			m_LClickPT.x = LOWORD((DWORD)lParam);
			m_LClickPT.y = HIWORD((DWORD)lParam);
		}
		else if(message == WM_RBUTTONDOWN ) //&& tag!="BODY" && tag!="TD"
		{
			QueryPerformanceCounter(&m_tLastRClick);
			m_bRClick = TRUE;
			m_bLClick = FALSE;
		}
		else
			m_bRClick = FALSE;

	}

	
	if(message==WM_RBUTTONDOWN && ((CMainFrame*)pMainFrame)->m_bAllowMouseGestures)
	{
		SetCapture();
		m_bIsCapture = TRUE;
//		m_bRClick = FALSE;
		m_StartPoint.x = LOWORD((DWORD)lParam);
		m_StartPoint.y = HIWORD((DWORD)lParam);
		m_iMouseGS = 0;
		m_iMouseGE = 0;
	}

	}catch(...)
	{
	}
}


void CMyIEView::OnToolsAutoscrollAutoscrollingafterdownloading() 
{
	// TODO: Add your command handler code here
	m_bScrollAfterDL = !m_bScrollAfterDL;

	if(pMainFrame!=NULL)
		((CMainFrame*)pMainFrame)->m_bDefScrollAfterDL = m_bScrollAfterDL;

}

void CMyIEView::OnUpdateToolsAutoscrollAutoscrollingafterdownloading(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bScrollAfterDL);
}



void CMyIEView::OnUpdateToolsAutoscrollSpeed(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(pMainFrame->m_bMouseCtrlScrSpeed)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable();

	if(pCmdUI->m_nID == ID_TOOLS_AUTOSCROLL_FAST)
		pCmdUI->SetCheck(m_nSrSpeed==3);
	else if(pCmdUI->m_nID == ID_TOOLS_AUTOSCROLL_MEDIUM)
		pCmdUI->SetCheck(m_nSrSpeed == 2);
	else
		pCmdUI->SetCheck(m_nSrSpeed == 1);
}

/*void CMyIEView::OnToolsAutoscrollMedium() 
{
	// TODO: Add your command handler code here
	m_nSrSpeed = 2;
	
	if(pMainFrame!=NULL)
		((CMainFrame*)pMainFrame)->m_nDefSrSpeed = m_nSrSpeed;

	ChangeSpeed(2);
}


void CMyIEView::OnToolsAutoscrollSlow() 
{
	// TODO: Add your command handler code here
	m_nSrSpeed = 1;
	if(pMainFrame!=NULL)
		((CMainFrame*)pMainFrame)->m_nDefSrSpeed = m_nSrSpeed;
	ChangeSpeed(1);
}
*/

void CMyIEView::OnToolsAutoscrollUseautoscrolling() 
{
	// TODO: Add your command handler code here
	m_bUseAutoScroll = !m_bUseAutoScroll;
	if(pMainFrame!=NULL)
		((CMainFrame*)pMainFrame)->m_bDefUseAutoScroll = m_bUseAutoScroll;

	//start to scroll
	if(!m_nScrollTimer && (m_bUseAutoScroll || pMainFrame->m_bAllScroll) && m_nSrSpeed!=0)
	{
		if(m_nProgress==100 || m_nProgress<0 || (!m_bScrollAfterDL && m_nProgress>=0))
		{
			//m_nScrollTimer = SetTimer(2132, (UINT)(1000*gSeed/m_nSrSpeed), NULL);
			if(pMainFrame->m_bMouseCtrlScrSpeed)
				m_nLastScrSpd = STOP_TIMER;
			else
				m_nLastScrSpd = (26 + gSeed * 100)/m_nSrSpeed;
			m_nScrollTimer = SetTimer(2132, m_nLastScrSpd, NULL);
		}
	}
	else if(!(m_bUseAutoScroll || pMainFrame->m_bAllScroll) && m_nScrollTimer)
	{		//stop the timer;
		if(m_nScrollTimer)
			KillTimer(m_nScrollTimer);
		m_nScrollTimer = 0;
	}
}

void CMyIEView::OnUpdateToolsAutoscrollUseautoscrolling(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	try
	{
		((CMainFrame*)pMainFrame)->ToolBarSetCheck(pCmdUI, m_bUseAutoScroll);
	}
	catch(...)
	{
	}
	/*
	if(m_bUseAutoScroll)
	{
		if(((CMainFrame*)pMainFrame)->m_strBGPic == "" || pCmdUI->m_pMenu !=NULL)
			pCmdUI->SetCheck();
		else
		{
			pCmdUI->SetCheck(0);
			((CMainFrame*)pMainFrame)->m_wndToolBar.GetToolBarCtrl().PressButton(pCmdUI->m_nID, TRUE);
		}
	}
	else
	{
		((CMainFrame*)pMainFrame)->m_wndToolBar.GetToolBarCtrl().PressButton(pCmdUI->m_nID, FALSE);
		pCmdUI->SetCheck(0);
	}*/
}

void CMyIEView::ChangeSpeed(int nSpeed)
{
	if(m_nScrollTimer)
	{		//stop the timer;
		KillTimer(m_nScrollTimer);
		m_nScrollTimer = 0;
	}

	//start to scroll
	if(!m_nScrollTimer && (m_bUseAutoScroll || pMainFrame->m_bAllScroll) && nSpeed!=0)
	{
		if(m_nProgress==100 || m_nProgress<0 || (!m_bScrollAfterDL && m_nProgress>=0))
		{
//			m_nScrollTimer = SetTimer(2132, (UINT)(1000*gSeed/nSpeed), NULL);
			if(pMainFrame->m_bMouseCtrlScrSpeed)
				m_nLastScrSpd = nSpeed;
			else
				m_nLastScrSpd = (26 + gSeed * 100)/m_nSrSpeed;

			m_nScrollTimer = SetTimer(2132, m_nLastScrSpd, NULL);
		}
	}
}


void WBScroll(CMyIEView* pvw)
{
	if(pvw->m_bToClose)
		return;

		LPDISPATCH pDisp = NULL;

		IHTMLDocument2 *pHTMLDoc = NULL;
		IHTMLWindow2 *pHTMLWnd = NULL;

		try{
		pDisp = pvw->GetHtmlDocument();
		if( pDisp )
		{
			if (SUCCEEDED(pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc)))
			{
				if(SUCCEEDED(pHTMLDoc->get_parentWindow( &pHTMLWnd )))
				{
					pHTMLWnd->scrollBy(0, 1);
					//2.1
					//pHTMLWnd->Release();
					//pHTMLWnd = NULL;
				}
				//pHTMLDoc->Release();
				//pHTMLDoc = NULL;
			}
			//pDisp->Release();
			//pDisp = NULL;
		}
		}
	/*	catch(CException * e)
		{//?
//		AfxMessageBox("wbs error");
			if(e!=NULL)e->Delete();
		}*/
		catch(...)
		{
//		AfxMessageBox("wbs error");
		}

		try{
			if(pHTMLWnd!=NULL)
				pHTMLWnd->Release();
			if(pHTMLDoc!=NULL)
				pHTMLDoc->Release();
			if(pDisp!=NULL)
				pDisp->Release();
		}
		catch(...)
		{
		}
}


void CMyIEView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	try{
	if(nIDEvent == m_nScrollTimer && (m_bUseAutoScroll || pMainFrame->m_bAllScroll))
	{
		if(m_nLastScrSpd<STOP_TIMER)
			WBScroll(this);
		//update scroll speed
		if(pMainFrame->m_bMouseCtrlScrSpeed)
		{
			CPoint mousept;
			GetCursorPos(&mousept);
			ScreenToClient(&mousept);
			CRect rect;
			GetWindowRect(&rect);
			if(mousept.x<=rect.Width() && mousept.x>=rect.Width()-pMainFrame->m_nScrollWidth)
			{
				if(mousept.y<=rect.Height()-pMainFrame->m_nScrollArrowH && mousept.y>0)
				{
					int sp = 800 - mousept.y*800/(rect.Height()-pMainFrame->m_nScrollArrowH)+1;
					if(abs(sp-m_nLastScrSpd)>5)
						ChangeSpeed(sp);
				}
				else
					//stop;
					ChangeSpeed(STOP_TIMER);
			}
			else
				ChangeSpeed(STOP_TIMER);
		}

	}
	else if(nIDEvent == m_nAutoRefreshTimer && m_bUseAutoRef && !IsActive)
	{
		//Refresh(); 
		try{
		Refresh2(REFRESH_COMPLETELY);
		}catch(...)
		{
		}
	}

	CFixedHtmlView::OnTimer(nIDEvent);
	}catch(...)
	{
	}
}


void CMyIEView::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	try{

	CFixedHtmlView::OnClose();
	}
	catch(...)
	{
		//SendMessage(WM_CLOSE);
		//return;
	}
}

void CMyIEView::ChangeProperty()
{
	try{
	IOleControl* poc;
	LPDISPATCH pDisp = GetHtmlDocument();
	if(pDisp != NULL)
	{
		if(SUCCEEDED(pDisp->QueryInterface(IID_IOleControl, (void**)&poc)))
		{
			poc->OnAmbientPropertyChange(DISPID_AMBIENT_DLCONTROL);
			poc->Release();
		}
		pDisp->Release();
	}
	}catch(...)
	{
	}
}

void CMyIEView::OnToolsAutoref() 
{
	// TODO: Add your command handler code here
	m_bUseAutoRef = !m_bUseAutoRef;

	//start to scroll
	if(!m_nAutoRefreshTimer && m_bUseAutoRef && m_nRefreshInterval!=0)
	{
		//m_nScrollTimer = SetTimer(2132, (UINT)(1000*gSeed/m_nSrSpeed), NULL);
		m_nAutoRefreshTimer = SetTimer(3132+rand()/500, (UINT)(m_nRefreshInterval*1000), NULL);
	}
	else if(!m_bUseAutoRef && m_nAutoRefreshTimer)
	{		//stop the timer;
		if(m_nAutoRefreshTimer)
			KillTimer(m_nAutoRefreshTimer);
		m_nAutoRefreshTimer = 0;
	}
	
}

void CMyIEView::OnUpdateToolsAutoref(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bUseAutoRef);

	static char sec[10];
	if(pCmdUI->m_pMenu!=NULL)
	{
		itoa(m_nRefreshInterval, sec, 10);
		CString menu;
		pCmdUI->m_pMenu->GetMenuString(pCmdUI->m_nID, menu, MF_BYCOMMAND);
		int i = menu.Find('(');
		if(i>0)
			menu = menu.Left(i);
		menu += "(";
		menu += sec;
		menu += ")";
		pCmdUI->SetText(menu);
	}
}

void CMyIEView::OnToolsReffre() 
{
	// TODO: Add your command handler code here
	CSetRefIntervalDlg dlg;
	dlg.m_nRefInt = m_nRefreshInterval;
	if(dlg.DoModal() == IDOK)
	{
		m_nRefreshInterval = dlg.m_nRefInt;
		((CMainFrame*)pMainFrame)->m_nDefRefreshInterval = m_nRefreshInterval;
		//change timer
		if(m_bUseAutoRef)
			ChangeRefresh();
	}
}

void CMyIEView::ChangeRefresh()
{
	if(m_nAutoRefreshTimer)
	{		//stop the timer;
		KillTimer(m_nAutoRefreshTimer);
		m_nAutoRefreshTimer = 0;
	}

	//start to scroll
	if(!m_nAutoRefreshTimer && m_bUseAutoRef && m_nRefreshInterval!=0)
	{
//			m_nScrollTimer = SetTimer(2132, (UINT)(1000*gSeed/nSpeed), NULL);
		m_nAutoRefreshTimer = SetTimer(3132, (UINT)(m_nRefreshInterval*1000), NULL);
	}

}

void CMyIEView::OnNavigateComplete2(LPCTSTR lpszURL)
{
	//2.3
	try{
	CString strUrl;
	CChildFrame* tcf = ((CChildFrame*)GetParentFrame());
	GetLocationURL(strUrl);

	if(!m_bToClose && ((IsPopUp && m_IsNewPopup) || (tcf!=NULL && !tcf->IsWindowVisible())) && !strUrl.IsEmpty())
	{
		CString url2 = strUrl.Right(m_strPossibleURL.GetLength()+13);
		if((pMainFrame->IsWindowVisible() && tcf!=NULL && !tcf->IsWindowVisible()) || url2 == "navcancl.htm#" + m_strPossibleURL /*for IE 5*/|| strUrl == "about:NavigationCanceled" /*for IE4*/ || strUrl==m_strPossibleURL)
		{
			//	CChildFrame* tcf = (CChildFrame*)GetParentFrame();
				//IsActive = FALSE;
			if(tcf!=NULL) 
			{
				tcf->PostMessage(WM_CLOSE);
				m_bToClose = TRUE;
			}

			IsPopUp = FALSE;
		}

	}


		try{

		//2.4 security flag
		if(!m_bToClose)
		{
			bVerbose = TRUE;


			if(strUrl.Left(5)=="https")
			{
				LPDISPATCH pDisp = NULL;

				IHTMLDocument2 *pHTMLDoc = NULL;
				BSTR p;

				try{
					pDisp = GetHtmlDocument();
					if( pDisp )
					{
						pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc);
					}
					else
						m_bIsSecure = FALSE;

					if(pHTMLDoc)
					{
						if(SUCCEEDED(pHTMLDoc->get_protocol( &p )))
						{
							CString s(p);
							if(s=="HyperText Transfer Protocol with Privacy" || s.Find("私人验证")>0 || s.Find("玂盞")>0)
								m_bIsSecure = TRUE;
							else
								m_bIsSecure = FALSE;
							SysFreeString(p);

						}
					}

					if(IsActive && pMainFrame!=NULL)
						pMainFrame->m_bIsSecure = m_bIsSecure;
				}
				catch(...)
				{
				}

				try{
			//			if(pSecuMang!=NULL)
			//				pSecuMang->Release();
						if(pHTMLDoc!=NULL)
							pHTMLDoc->Release();
						if(pDisp!=NULL)
							pDisp->Release();
				}
				catch(...)
				{
				}
			}

			//zone
	/*		IInternetSecurityManager* pSecuMang = NULL;
		    HRESULT hr = CoCreateInstance(CLSID_InternetSecurityManager, NULL, 
										 CLSCTX_INPROC_SERVER, IID_IInternetSecurityManager, 
										 (LPVOID*)&pSecuMang);
			if (SUCCEEDED(hr))
			{
				DWORD zone;
				p = strUrl.AllocSysString();
				pSecuMang->MapUrlToZone(p, &zone, 0);
				SysFreeString(p);
				char tmp[5];
				itoa(zone, tmp, 10);
				AfxMessageBox(tmp);
			}
*/


		}

		}catch(...)
		{
		}

	}catch(...)
	{
	}

//2.3 end

}

void CMyIEView::OnToolsAutofilterCur() 
{
	// TODO: Add your command handler code here
	m_bAutoFilter = !m_bAutoFilter;
}

void CMyIEView::OnUpdateToolsAutofilterCur(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	//((CMainFrame*)pMainFrame)->ToolBarSetCheck(pCmdUI, m_bAutoFilter);
	pCmdUI->SetCheck(m_bAutoFilter);

	//2.5 if m_nProgess=-1 disable autofiler
	if(m_nProgress==-10)
		m_nProgress=-1;

}




void CMyIEView::OnFileSaveas() 
{
	// TODO: Add your command handler code here
	ExecWB(OLECMDID_SAVEAS, OLECMDEXECOPT_DODEFAULT,
		   NULL, NULL);
}

void CMyIEView::OnFileSavehtml() 
{
	// TODO: Add your command handler code here
	ExecWB(OLECMDID_SAVEAS, OLECMDEXECOPT_DONTPROMPTUSER,
		   NULL, NULL);
}


void CMyIEView::OnFilePrintSetup() 
{
	// TODO: Add your command handler code here
	ExecCommand(OLECMDID_PAGESETUP);
}

void CMyIEView::OnFileProperty() 
{
	// TODO: Add your command handler code here
	ExecCommand(OLECMDID_PROPERTIES);
}

void CMyIEView::ExecCommand(DWORD nCmdID)
{
	LPOLECOMMANDTARGET lpTarget = NULL;
	LPDISPATCH lpDisp = GetHtmlDocument();

	if (lpDisp != NULL)
	{
		// the control will handle all printing UI

		if (SUCCEEDED(lpDisp->QueryInterface(IID_IOleCommandTarget,
				(LPVOID*) &lpTarget)))
		{
			lpTarget->Exec(NULL, nCmdID, 0, NULL, NULL);
			lpTarget->Release();
		}
		lpDisp->Release();
	}	
}

#define RBUT_LEFT 'L'
#define RBUT_RIGHT 'R'
#define RBUT_UP 'U'
#define RBUT_DOWN 'D'

void CMyIEView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	try{

	if(m_bIsCapture)
	{
		m_bIsCapture = FALSE;
		ReleaseCapture();
		int i =0; m_SeqMG[0]= '\0';
		while(m_iMouseGE != m_iMouseGS)
		{
			m_SeqMG[i] = m_MouseGestures[m_iMouseGS];
			i++;
			m_SeqMG[i] = '\0';

			m_iMouseGS = (m_iMouseGS +1)%m_iMGLen;		
		}

		if(i>0)
		{
			if(strcmp(m_SeqMG, "D")==0)
			{
				// TODO: Add your command handler code here
				switch(pMainFrame->m_nNewWindow2)
				{
				case 0: 
					pMainFrame->PostMessage(WM_COMMAND, ID_FILE_NEW_BLANK, 0);
					break;
				case 1: 
					pMainFrame->PostMessage(WM_COMMAND, ID_FILE_NEW_S, 0);
					break;
				case 2: 
					pMainFrame->PostMessage(WM_COMMAND, ID_FILE_NEW_CURRENTPAGE, 0);
				}
			}
			else if(strcmp(m_SeqMG, "L")==0)
				GoBack();
			else if(strcmp(m_SeqMG, "R")==0)
				GoForward();
			else if(strcmp(m_SeqMG, "UD")==0)
				Refresh2(REFRESH_COMPLETELY);
			else if(strcmp(m_SeqMG, "RLR")==0 || strcmp(m_SeqMG, "DR")==0)
				pMainFrame->PostMessage(WM_COMMAND, ID_FILE_CLOSE_1, 0);
			else if(strcmp(m_SeqMG, "UL")==0)
				pMainFrame->PostMessage(WM_COMMAND, ID_TAB_PRE, 0);
			else if(strcmp(m_SeqMG, "UR")==0)
				pMainFrame->PostMessage(WM_COMMAND, ID_TAB_NEXT, 0);
			else 
				i = 0;
		}
		
		
		if(i==0)
		{
			//popup right menu;
			POINT p2 = point;
			ClientToScreen(&p2);
			m_pCaptured = WindowFromPoint(p2);
			if(m_pCaptured)
			{
				m_pCaptured->ScreenToClient(&p2);
				m_pCaptured->PostMessage(WM_RBUTTONUP, nFlags, MAKELONG(p2.x, p2.y)); 
//				m_bRClick = TRUE;
//				QueryPerformanceCounter(&m_tLastRClick);
			}
		}
	}
	
	}catch(...)
	{
	}

	CFixedHtmlView::OnRButtonUp(nFlags, point);
}

void CMyIEView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	try{
	if(m_bIsCapture && nFlags == MK_RBUTTON)
	{
		char dir;
		if(MoveDirection(point, &dir))
		{
			PushMouseGesture(dir);
			m_StartPoint = point;
		}
	}

	}catch(...)
	{
	}

	CFixedHtmlView::OnMouseMove(nFlags, point);
}

void CMyIEView::PushMouseGesture(char gesture)
{
	if(m_iMouseGE!=0 || m_iMouseGS !=0)
	{
		int pre = (m_iMouseGE -1 + m_iMGLen)%m_iMGLen;		
		if(m_MouseGestures[pre] == gesture)
			return;
	}

	m_MouseGestures[m_iMouseGE] = gesture;
	m_iMouseGE = (m_iMouseGE+1)%m_iMGLen;
	if(m_iMouseGS == m_iMouseGE)
		m_iMouseGS = (m_iMouseGS + 1)%m_iMGLen;

}

BOOL CMyIEView::MoveDirection(CPoint &point, char *Direction)
{
	int x = point.x - m_StartPoint.x;
	int y = point.y - m_StartPoint.y;
	int dist = x*x+y*y;
	if(dist>64)
	{
		if(x>abs(y) && x>0)
			*Direction = RBUT_RIGHT;
		else if(abs(x)>abs(y) && x<0)
			*Direction = RBUT_LEFT;
		else if(y>abs(x) && y>0)
			*Direction = RBUT_DOWN;
		else if(abs(y)>abs(x) && y<0)
			*Direction = RBUT_UP;
		else
			return FALSE;

		return TRUE;
	}
	else
		return FALSE;
}



void CMyIEView::OnFileAutosave() 
{
	// TODO: Add your command handler code here
	try{

		IHTMLDocument2 * pDoc = NULL;
		IPersistFile *pPersistFile = NULL;
		CString filename, ext, htmlname;
		int l;
		BSTR bfilename;
		char tmp[4]; 
		GetLocationURL(filename);

		pMainFrame->m_strAutoSavePath = ((CMyIEApp*)AfxGetApp())->m_strDefaultDir;
		if(pMainFrame->m_bCateWithSite)
		{
			CString site;
			GetLocationURL(site);
			site.MakeLower();
			site.Replace('\\', '/');
			l = site.Find(':');
			l++;
			if(site.GetAt(l)=='/')
				while(site.GetAt(l)=='/')
					l++;
			site = site.Mid(l);
			l =	site.Find('/');
			if(l>0)
				site = site.Left(l);
			if(site.Right(1) == ":")
				site = site.Left(site.GetLength()-1);
			static TCHAR alph[] = "abcdefghijkmnopqistuvwxyz";
			l = site.FindOneOf(alph);
			if(l>=0)
			{
				l = site.Find('.');
				if(l>0)
					site = site.Mid(l+1);
				l = site.Find('.');
				if(l>0)
					site = site.Left(l);
			}

			pMainFrame->m_strAutoSavePath += site;
			pMainFrame->m_strAutoSavePath += "\\";
			CreateDirectory(pMainFrame->m_strAutoSavePath, NULL);
		}

		if(pMainFrame->m_nAutoSave == 0 || pMainFrame->m_nAutoSave == 1 || pMainFrame->m_nAutoSave == 4 || (pMainFrame->m_nAutoSave == 3 && filename.Left(5)=="file:"))
		{
			keybd_event(VK_TAB, MapVirtualKey(VK_TAB, 0), 0, 0);
			if(pMainFrame->m_nAutoSave==1 || pMainFrame->m_nAutoSave==4)
				keybd_event('W', MapVirtualKey('W', 0), 0, 0);
			if(pMainFrame->m_nAutoSave==4)
			{
				keybd_event('A', MapVirtualKey('A', 0), 0, 0);
				keybd_event('W', MapVirtualKey('W', 0), 0, 0);
			}
			keybd_event(VK_RETURN, MapVirtualKey(VK_RETURN, 0), 0, 0);
			//set save directory
			HKEY hKey;
			if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\Main"), &hKey) == ERROR_SUCCESS)
			{
				RegSetValueEx(hKey, _T("Save Directory"), 0, REG_SZ, (LPBYTE)(pMainFrame->m_strAutoSavePath.GetBuffer(10)), pMainFrame->m_strAutoSavePath.GetLength());
				pMainFrame->m_strAutoSavePath.ReleaseBuffer();
			}
			SendMessage(WM_COMMAND, ID_FILE_SAVEAS, 0);
		}
		else if(pMainFrame->m_nAutoSave == 2)
		{
			filename.Replace('/', '\\');
			if(filename.Right(1) == "\\")
				filename = filename.Left(filename.GetLength()-1);

			l = filename.ReverseFind('\\');
			filename = filename.Mid(l+1);
			filename.Replace(':', '_');
			l = filename.Find('?');
			if(l>0)
				filename = filename.Left(l);
			l = filename.ReverseFind('.');
			if(l>0)
			{
				ext = filename.Right(filename.GetLength()-l);
				ext.MakeLower();
				filename = filename.Left(l);
			}
			if(ext != ".htm" && ext != ".html" && ext !=".jpg" && ext != ".gif" && ext != ".shtml")
			{
				ext = ".htm";
			}
			filename = pMainFrame->m_strAutoSavePath + filename;
			l = 0;
			HFILE hf; OFSTRUCT of;
			htmlname = filename + ext;
			hf = OpenFile(htmlname, &of, OF_EXIST);
			while(hf!=HFILE_ERROR)
			{
				l++;
				htmlname = filename;
				htmlname += '[';
				itoa(l, tmp, 10);
				htmlname += tmp;
				htmlname += ']';
				htmlname += ext;
				hf = OpenFile(htmlname, &of, OF_EXIST);
			}

			if(ext != ".jpg" && ext != ".gif")
			{
				pDoc = (IHTMLDocument2*)GetHtmlDocument();
				bfilename = htmlname.AllocSysString();
				if (SUCCEEDED(pDoc->QueryInterface(IID_IPersistFile, (void**)&pPersistFile)))
					pPersistFile->Save(bfilename, FALSE);

				try{
					SysFreeString(bfilename);
					if(pPersistFile!=NULL)
						pPersistFile->Release();
					if(pDoc != NULL)
						pDoc->Release();
				}
				catch(...)
				{
				}
			}
			else
			{
				//save image
				GetLocationURL(filename);
				DWORD dwEntrySize=0;
				LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry;
				if (!GetUrlCacheEntryInfo(filename,NULL,&dwEntrySize))
				{
					
				   if (GetLastError()==ERROR_INSUFFICIENT_BUFFER)
				   {
                        
					   lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) 
													new char[dwEntrySize];

						if (GetUrlCacheEntryInfo(filename,lpCacheEntry,&dwEntrySize))
						{
							CopyFile(lpCacheEntry->lpszLocalFileName, htmlname, FALSE);
						}

						delete lpCacheEntry;
						lpCacheEntry = NULL;
						dwEntrySize = 0;
				   }
				   else
				   {
					   //local image files
					    pMainFrame->DecodeEscap(filename);
						pMainFrame->DecodeEscap(htmlname);
						if(filename.Left(8)=="file:///")
							filename = filename.Mid(8);
						CopyFile(filename, htmlname, FALSE);
				   }
				}
			}

		}
		else if(pMainFrame->m_nAutoSave == 3)
		{
			pMainFrame->m_bIsAutoSave = TRUE;
			pMainFrame->m_pCurView = this;
			pMainFrame->SendMessage(WM_COMMAND, ID_FILE_QUICKSAVE,0);
			pMainFrame->m_bIsAutoSave = FALSE;
		}

		l = pMainFrame->FindTab((CChildFrame*)GetParentFrame());
		oldIcon = pMainFrame->SetTabIcon(7, l);
		if(pMainFrame->m_bFBTab)
			pMainFrame->PostMessage(WM_UPDATE_TAB);
		CString savedmsg;
		savedmsg.LoadString(IDS_PAGE_SAVED);
		savedmsg+= pMainFrame->m_strAutoSavePath;
		pMainFrame->SetMessageText(savedmsg);

	}catch(...)
	{
	}
	
}
