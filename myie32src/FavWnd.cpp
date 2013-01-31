// FavWnd.cpp : implementation file
//

#include "stdafx.h"
#include "myie.h"
#include "FavWnd.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#pragma optimize( "s", on )


/////////////////////////////////////////////////////////////////////////////
// CFavWnd

CFavWnd::CFavWnd()
{
	m_bBest = FALSE;
}

CFavWnd::~CFavWnd()
{
}


BEGIN_MESSAGE_MAP(CFavWnd, CWnd)
	//{{AFX_MSG_MAP(CFavWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_FAVORITES_ORGNIZEFAVORITES, OnFavoritesOrgnizefavorites)
	ON_COMMAND(ID_FAVORITES_ADDTOFAVORITES, OnFavoritesAddtofavorites)
	ON_COMMAND(ID_FAVORITES_BEST, OnFavoritesBest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFavWnd message handlers

int CFavWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_FavTreeCtrl.Create(WS_CHILD|WS_VISIBLE|TVS_SHOWSELALWAYS|TVS_LINESATROOT|
		TVS_FULLROWSELECT |TVS_TRACKSELECT | TVS_EDITLABELS,
		CRect(0,0,0,0), this, 100);
	m_FavTreeCtrl.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	// bring the tooltips to front
	CWnd* pTT = m_FavTreeCtrl.FromHandle((HWND) m_FavTreeCtrl.SendMessage(TVM_GETTOOLTIPS));
    if (pTT != NULL)
		pTT->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0,
			SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);

	CImageList img;
/*	img.Create(IDB_BITMAP1, 16, 0, RGB(255, 0, 255));
	m_FavTreeCtrl.SetImageList(&img, TVSIL_NORMAL);
	img.Detach();
*/

//	CBitmap    bitmap;
	// Create and set the normal toolbar image list.
//	bitmap.LoadBitmap(IDB_BITMAP1);
	img.Create(16, 16, ILC_COLORDDB|ILC_MASK, 7, 1);
	HBITMAP hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP1));
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));

//	img.Add(&bitmap, RGB(255,0,255));
	m_FavTreeCtrl.SetImageList(&img, TVSIL_NORMAL);
	img.Detach();
//	bitmap.Detach();

	if (!m_ToolBar.CreateEx(this))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	// set up toolbar properties
	m_ToolBar.GetToolBarCtrl().SetButtonWidth(16, 100);
/*
//	img.Create(IDB_BITMAP3, 16, 0, RGB(255, 0, 255));
	img.Create(16, 16, ILC_COLORDDB|ILC_MASK, 3, 1);
	hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP3));
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
//	m_ToolBar.GetToolBarCtrl().SetHotImageList(&img);
	m_ToolBar.SendMessage(TB_SETHOTIMAGELIST, 0, (LPARAM)img.m_hImageList);
	img.Detach();*/

//	img.Create(IDB_BITMAP4, 16, 0, RGB(255, 0, 255));
	img.Create(16, 16, ILC_COLORDDB|ILC_MASK, 3, 1);
	hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP3));
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
//	m_ToolBar.GetToolBarCtrl().SetImageList(&img);
	m_ToolBar.SendMessage(TB_SETIMAGELIST, 0, (LPARAM)img.m_hImageList);
	img.Detach();

	m_ToolBar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | TBSTYLE_LIST);
	m_ToolBar.SetButtons(NULL, 3);

	// set up each toolbar button
	CString str;
	str.LoadString(IDS_FAV_ADD);
	m_ToolBar.SetButtonInfo(0, ID_FAVORITES_ADDTOFAVORITES, TBSTYLE_BUTTON|TBSTYLE_AUTOSIZE, 0);
	m_ToolBar.SetButtonText(0, str);
	
	str.LoadString(IDS_FAV_ORG);
	m_ToolBar.SetButtonInfo(1, ID_FAVORITES_ORGNIZEFAVORITES, TBSTYLE_BUTTON|TBSTYLE_AUTOSIZE, 1);
	m_ToolBar.SetButtonText(1, str);

	str.LoadString(IDS_FAV_MOST);
	m_ToolBar.SetButtonInfo(2, ID_FAVORITES_BEST, TBSTYLE_BUTTON|TBSTYLE_AUTOSIZE, 2);
	m_ToolBar.SetButtonText(2, str);

	m_ToolBar.SetBarStyle(m_ToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED);

	CRect rectToolBar;
	// set up toolbar button sizes
	m_ToolBar.GetItemRect(1, &rectToolBar);
	m_ToolBar.SetSizes(rectToolBar.Size(), CSize(16,14));

	if (!m_Rebar.Create(this,RBS_BANDBORDERS|RBS_DBLCLKTOGGLE | RBS_REGISTERDROP |RBS_VARHEIGHT ,CCS_NODIVIDER |CCS_NOPARENTALIGN |WS_BORDER |WS_CHILD |WS_CLIPCHILDREN |WS_CLIPSIBLINGS |WS_VISIBLE | CBRS_TOP))
	{
		return -1;      // fail to create
	}

	m_Rebar.AddBar(&m_ToolBar);
	REBARBANDINFO rbbi;

	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE|RBBIM_STYLE;
	rbbi.fStyle = RBBS_NOGRIPPER;
	rbbi.cxMinChild = rectToolBar.Width();
	rbbi.cyMinChild = rectToolBar.Height();
	rbbi.cx = rbbi.cxIdeal = rectToolBar.Width()*2;
	m_Rebar.GetReBarCtrl().SetBandInfo(0, &rbbi);

	return 0;
}

void CFavWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_Rebar.MoveWindow(1,1, cx-1,26);
	m_FavTreeCtrl.MoveWindow(0,27,cx,cy-27);
}

void CFavWnd::OnFavoritesOrgnizefavorites() 
{
	// TODO: Add your command handler code here
	((CMainFrame*)pMainFrame)->SendMessage(WM_COMMAND, ID_FAVORITES_ORGNIZEFAVORITES, 0);
}

void CFavWnd::OnFavoritesAddtofavorites() 
{
	// TODO: Add your command handler code here
	((CMainFrame*)pMainFrame)->SendMessage(WM_COMMAND, ID_FAVORITES_ADDTOFAVORITES, 0);
}

void CFavWnd::OnFavoritesBest() 
{
	// TODO: Add your command handler code here
	m_bBest = !m_bBest;

	((CMainFrame*)pMainFrame)->m_bMostFavChecked = m_bBest;

	((CMainFrame*)pMainFrame)->m_pwndExpObj->Update(1);

	m_ToolBar.GetToolBarCtrl().CheckButton(ID_FAVORITES_BEST, m_bBest);
}

//#pragma optimize( "s", off)
