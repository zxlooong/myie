// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "MyIE.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include <..\src\afximpl.h>
#include "InputPassword.h"
#include "mshtml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CChildFrame * pPreWin = NULL;
/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_CLOSE()
//	ON_WM_DESTROY()
	ON_WM_MDIACTIVATE()
	ON_WM_SYSCOMMAND()
	ON_WM_SIZE()
	ON_WM_NCACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	bLock = FALSE;
	bProtect = FALSE;
	m_pView = NULL;
//	m_bFirst = TRUE;
	m_pBlankChild = NULL;
	m_bSkipSel = FALSE;
	m_bForceActive = FALSE;
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

    cs.lpszClass = AfxRegisterWndClass(
       CS_DBLCLKS,                       // if you need double-clicks
       NULL,                             // no cursor (use default)
       NULL,                             // no background brush
       LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MyIEYPE))); // app icon
    ASSERT(cs.lpszClass);
 

	cs.style = WS_CHILD  | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
		| FWS_ADDTOTITLE | WS_THICKFRAME  | WS_MAXIMIZEBOX| WS_MINIMIZEBOX ;


    cs.cx = 520;
    cs.cy = 320; 

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	BOOL bMax;
	if( pMainFrame != NULL)
	{
		if(pMainFrame->MDIGetActive(&bMax)==NULL)
		{
			bMax = TRUE;
			cs.style |= WS_VISIBLE;
		}
	}
	else
		bMax = TRUE;
	if(bMax)
		cs.style |= WS_MAXIMIZE;

	if(bMax)
		m_bMax = 1;
	else
		m_bMax = 0;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers


/*int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
//	((CMainFrame*)GetParentFrame())->AddNewTab(this);

	return 0;
}*/

void CChildFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//clear the blank window
 try
 {
	if(pMainFrame!=NULL)
	{
		int cn = pMainFrame->m_wndTab.GetItemCount();
		int nTabID;
		if(!IsWindowVisible() && pMainFrame->IsWindowVisible())
			nTabID = -1;
		else
		{
			nTabID = pMainFrame->FindTab(this, TRUE);
			pMainFrame->nWin= cn-1;
		}
		if(nTabID>=0 && nTabID<cn)
		{
			pMainFrame->m_wndTab.DeleteItem(nTabID);

			if(m_pView!=NULL)
			{
				if(m_pView->m_nDupID>nTabID)
					m_pView->m_nDupID--;

				if(m_pView->m_lpszUrl!="about:blank")
				{
					pMainFrame->m_strUndoURL[pMainFrame->undoEnd] = m_pView->m_lpszUrl;
					pMainFrame->m_strUndoName[pMainFrame->undoEnd] = m_Title;
					
					pMainFrame->undoEnd = (pMainFrame->undoEnd+1)%16;
					if(pMainFrame->undoStart == pMainFrame->undoEnd)
						pMainFrame->undoStart = (pMainFrame->undoStart + 1)%16;

					pMainFrame->UpdateUndoMenu();
				}
			}

			//2.3 move here
			//3.2
			if(pMainFrame->m_bAutoTab)
				pMainFrame->PostMessage(WM_UPDATE_TAB, TRUE);
			else
				pMainFrame->PostMessage(WM_UPDATE_TAB, FALSE);

			pMainFrame->PostMessage(WM_UPDATE_TAB_TIP);

			//((CMainFrame*)pMainFrame)->SaveLastVisit();
			bVerbose = TRUE;
		}

		if(bProtect)
			pMainFrame->m_nProtectNum--;
		

		//update address bar
		if(cn==1)
		{
			pMainFrame->m_wndAddress->SetWindowText("");
			pMainFrame->m_SysMenuBar.PostMessage(WM_IDLEUPDATECMDUI, TRUE, 0);
		}

		/*	if(m_pView!=NULL)
		{
			m_pView->DestroyWindow();
			delete m_pView;
			m_pView = NULL;
		}*/
		nNextTab = -2;
		if(m_pView!=NULL && (m_pView->IsActive || m_pView->m_nDupID>=0))
		{
			if(m_pView->m_nDupID>=0 && pMainFrame->m_bActiveNewWindow)
				nNextTab = m_pView->m_nDupID;
			else if(pMainFrame->m_nSeq==1)
			{
				nNextTab = nTabID;
			}
			else if(pMainFrame->m_nSeq==2)
			{
				nNextTab = nTabID-1;
			}
		}

		if(nNextTab>=0)
		{
			TCITEM TabCtrlItem;
			TabCtrlItem.mask = TCIF_PARAM;

			pMainFrame->m_wndTab.GetItem(nNextTab, &TabCtrlItem);
			CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
			nTabID = nNextTab;
			nNextTab = -2;
			if(tcf!=NULL)
			{
				pMainFrame->MDIActivate(tcf);
				pMainFrame->m_wndTab.SetCurSel(nTabID);
			}
		}
	}

 }
 catch(...)
 {
//	CMDIChildWnd::OnClose();
//2.2	 return;
 }
//2.2
	CMDIChildWnd::OnClose();
}



/*void CChildFrame::OnDestroy() 
{
	CMDIChildWnd::OnDestroy();

	// TODO: Add your message handler code here


	if(m_pView!=NULL)
		m_pView->SendMessage(WM_CLOSE, 0, 0);

	if(pMainFrame!=NULL)
	{
		int nTabID = ((CMainFrame*)pMainFrame)->FindTab(this);
		((CMainFrame*)pMainFrame)->m_wndTab.DeleteItem(nTabID);

		pMainFrame->PostMessage(WM_UPDATE_TAB, TRUE);
		pMainFrame->PostMessage(WM_UPDATE_TAB_TIP);
		//update address bar
		if(((CMainFrame*)pMainFrame)->m_wndTab.GetItemCount()==0)
			((CMainFrame*)pMainFrame)->m_wndAddress->SetWindowText("");

		CChildFrame* tcf;
		tcf=(CChildFrame*)((CMainFrame*)pMainFrame)->MDIGetActive();
		if(tcf && ((CMainFrame*)pMainFrame)->m_nSeq==0)
		{
			nTabID = ((CMainFrame*)pMainFrame)->FindTab(tcf);
			((CMainFrame*)pMainFrame)->m_wndTab.SetCurSel(nTabID);
		}
	}
}*/

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class

	CMyIEView* pvw = NULL;

	try
	{
		try{
			pvw = new CMyIEView;
		}catch(...)
		{
			
		}

	if(pvw==NULL)
		return FALSE;

//	int TotalTabs = 0;
//	if(pMainFrame!=NULL)
//		TotalTabs = ((CMainFrame*)pMainFrame)->m_wndTab.GetItemCount();

	if(pMainFrame!=NULL && pMainFrame->nWin >0)
		if(pMainFrame->m_bActiveNewWindow || m_bForceActive)
			pvw->IsActive=TRUE;
		else
		{
			pvw->IsActive=FALSE;
		}
	else
		pvw->IsActive = TRUE;

	CRect rect;
	try{
	GetClientRect(&rect);
	pvw->Create(NULL, "", WS_CHILD|WS_VISIBLE, rect, this, AFX_IDW_PANE_FIRST,NULL);
//
	m_pView = pvw;
//316	m_pView->m_wndBrowser.SetWindowPos(this, -BORDER,-BORDER,200,200, SWP_NOREDRAW|SWP_NOSIZE|SWP_NOZORDER);
	}catch(...)
	{
	}

	}
/*	catch(CMemoryException* e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}

	return TRUE; //CMDIChildWnd::OnCreateClient(lpcs, pContext);

}

BOOL CChildFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class

	try{

//2.2	CPushRoutingFrame push(this);

	// pump through current view FIRST
	CWnd* pw = GetWindow(GW_CHILD);
	if (pw != NULL && pw->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// then pump through frame
//2.2	if (CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
//		return TRUE;

	// last but not least, pump through app
//2.2	CWinApp* pApp = AfxGetApp();
//	if (pApp != NULL && pApp->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
//		return TRUE;

	}catch(...)
	{
	}

	return FALSE;
}



void CChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	if(m_pView == NULL)
		return;

	try{

		//2.5
		if(m_pView->m_nProgress==-10)
			m_pView->m_nProgress=-1;

	if(bProtect && bActivate && !m_pView->IsActive && pPreWin!=this)
	{
		m_pView->ShowWindow(SW_HIDE);
		CInputPassword dlg;
		BOOL validate = FALSE;
		if(dlg.DoModal()==IDOK)
		{
			if(dlg.m_strPassword == m_strPsw)
				validate = TRUE;
			else
				AfxMessageBox(IDS_PASSWORD_ERROR);
		}

		if(!validate)
		{
			pPreWin = this;
			//go back 
			if(pMainFrame->m_wndTab.GetItemCount()==pMainFrame->m_nProtectNum)
			{
				BOOL olda = pMainFrame->m_bActiveNewWindow;
				pMainFrame->m_bActiveNewWindow = TRUE;
				CChildFrame* ncf = pMainFrame->NewChildWindow(2,0);
				pMainFrame->m_bActiveNewWindow = olda;
				pMainFrame->m_wndAddress->SetFocus();
			}
			else
				pMainFrame->MDINext();
			m_pView->ShowWindow(SW_SHOW);
			return;
		}
		else
			m_pView->ShowWindow(SW_SHOW);
	}

	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);

	//3.01
	if(bActivate)
	{
		try{
			//active webbrowser
			LPDISPATCH pDisp = NULL;
			IHTMLDocument2 *pHTMLDoc = NULL;
			IHTMLWindow2 *pHTMLWnd = NULL;

			pDisp = m_pView->GetHtmlDocument();
			if( pDisp )
			{
				if (SUCCEEDED(pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc)))
				{
					if(SUCCEEDED(pHTMLDoc->get_parentWindow( &pHTMLWnd )))	
					{
						pHTMLWnd->focus();
						pHTMLWnd->Release();
					}
					pHTMLDoc->Release();
				}
				pDisp->Release();
			}
		}catch(...)
		{
		}
	}

	if(!bProtect && pPreWin == this && bActivate)
	{
		m_pView->IsActive = bActivate;
		return;
	}

	if(bActivate)
		pPreWin = this;

	// TODO: Add your message handler code here


	BOOL lastActive = m_pView->IsActive;
	int lastper = percent;
//	if(!m_bFirst)
		m_pView->IsActive = bActivate;
//	m_bFirst = FALSE;

	percent = -1;
	if(pMainFrame!=NULL && pMainFrame->GetForegroundWindow() != pMainFrame)
	{
		m_pView->IsActive = lastActive;
		percent = lastper;
	}
	
	SetWindowText(m_Title);

	//set address bar
	if(m_pView->IsActive)
	{
		//7/17
	//	CString lpszUrl = m_pView->GetLocationURL();
	//	if(!lpszUrl.IsEmpty() && lpszUrl != "")
//			((CMainFrame*)GetParentFrame())->SetAddress(lpszUrl);
//		else
			pMainFrame->SetAddress(m_pView->m_lpszUrl);

		pMainFrame->m_bIsSecure = m_pView->m_bIsSecure;

		//2.3 get the current tab
		int nTabID=0;
		if(m_bSkipSel)
			nTabID = nCurTabID;
		else
		{
			nTabID = pMainFrame->FindTab(this);
		}

		if(nNextTab>=0 && nNextTab!=nTabID)
		{
			m_bSkipSel = FALSE;
			return;
		}


		//select current tab
		if(!m_bSkipSel)
			pMainFrame->m_wndTab.SetCurSel( nTabID );
		int oi = m_pView->oldIcon;
		if(oi==5 || oi==7 || m_pView->m_nProgress<0)
		{
			if(bLock)
				m_pView->oldIcon = pMainFrame->SetTabIcon( 4, nTabID );
			else if(bProtect)
				m_pView->oldIcon = pMainFrame->SetTabIcon( 6, nTabID );
			else
			{
				m_pView->oldIcon = pMainFrame->SetTabIcon( -1, nTabID );
			}

			//update tab bar
			if(oi*m_pView->oldIcon <=0 && pMainFrame->m_bFBTab)
				pMainFrame->PostMessage(WM_UPDATE_TAB, FALSE);
		}

	}
	


	if(m_pView->IsActive && !lastActive )
	{
		//set status bar
		m_pView->OnStatusTextChange("");
		percent = m_pView->m_nProgress;

	}

	m_bSkipSel = FALSE;

	}catch(...)
	{
	}
}

/*BOOL CChildFrame::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CMDIChildWnd::OnEraseBkgnd(pDC);
}*/




void CChildFrame::OnSysCommand(UINT nID, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default
	if(nID ==SC_MAXIMIZE)
	{
		m_bMax = 1;
	}
	else if(nID ==SC_RESTORE || nID == SC_MINIMIZE)
	{
		m_bMax = 3;
	}

	CMDIChildWnd::OnSysCommand(nID, lParam);
}



void CChildFrame::OnSize(UINT nType, int cx, int cy) 
{

	CMDIChildWnd::OnSize(nType, cx, cy);
	

	// TODO: Add your message handler code here
	if(m_pView!=NULL && m_bMax!=1)
	{
	//	try{
		CRect rect;
		GetClientRect(&rect);
		if(m_pView->GetWidth()!= rect.Width() || m_pView->GetHeight()!=rect.Height())
		{
			m_pView->PostMessage(WM_SIZE, nType, (rect.Height()<<16)+rect.Width());
		}
	//	}catch(...)
	//	{
	//	}
	}

}

BOOL CChildFrame::OnNcActivate(BOOL bActive) 
{
	// TODO: Add your message handler code here and/or call default
	if(!bActive)
	{
		//stop the timer;
		if(m_pView->m_nScrollTimer)
			m_pView->KillTimer(m_pView->m_nScrollTimer);
		m_pView->m_nScrollTimer = 0;
	}
	else
	{
		//start to scroll
		if(!m_pView->m_nScrollTimer && (m_pView->m_bUseAutoScroll || pMainFrame->m_bAllScroll) && m_pView->m_nSrSpeed!=0)
		{
			if(m_pView->m_nProgress==100 || m_pView->m_nProgress<0 || (!m_pView->m_bScrollAfterDL && m_pView->m_nProgress>=0))
			{
//				m_pView->m_nScrollTimer = m_pView->SetTimer(2132, (UINT)(1000*gSeed/m_pView->m_nSrSpeed), NULL);
				if(!pMainFrame->m_bMouseCtrlScrSpeed)
					m_pView->m_nLastScrSpd = (26 + gSeed * 100)/m_pView->m_nSrSpeed;
				else
					m_pView->m_nLastScrSpd = STOP_TIMER;
				m_pView->m_nScrollTimer = m_pView->SetTimer(2132, m_pView->m_nLastScrSpd, NULL);
			}
		}
	}
	
	return CMDIChildWnd::OnNcActivate(bActive);
}


