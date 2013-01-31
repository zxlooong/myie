// ShellWnd.cpp : implementation file
//

#include "stdafx.h"
#include "myie.h"
#include "ShellWnd.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// CShellWnd

CShellWnd::CShellWnd()
{
}

CShellWnd::~CShellWnd()
{
}


BEGIN_MESSAGE_MAP(CShellWnd, CWnd)
	//{{AFX_MSG_MAP(CShellWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(IDOK, OnRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CShellWnd message handlers

int CShellWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_ShellTreeCtrl.Create(WS_CHILD|WS_VISIBLE|TVS_SHOWSELALWAYS|TVS_HASLINES|TVS_HASBUTTONS|TVS_LINESATROOT|TVS_TRACKSELECT,
		CRect(0,0,0,0), this, 100);
	m_ShellTreeCtrl.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	// bring the tooltips to front
	CWnd* pTT = m_ShellTreeCtrl.FromHandle((HWND) m_ShellTreeCtrl.SendMessage(TVM_GETTOOLTIPS));
    if (pTT != NULL)
		pTT->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0,
			SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);


	if (!m_fType.Create(ES_AUTOHSCROLL|ES_LOWERCASE|WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, 101))
	{
		return -1;      // fail to create
	}
	m_fType.ModifyStyleEx(0, WS_EX_CLIENTEDGE);

	CFont* font = m_ShellTreeCtrl.GetFont();
	m_fType.SetFont(font);

	CImageList img;
	m_wndRef.CreateEx(this);
	img.Create(14, 14, ILC_COLORDDB|ILC_MASK, 1, 1);
	HBITMAP hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP9));
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
	m_wndRef.GetToolBarCtrl().SetImageList(&img);
	img.Detach();

	m_wndRef.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT|TBSTYLE_TOOLTIPS  );

	m_wndRef.SetButtonInfo(0, IDOK, TBSTYLE_BUTTON, 0);

	m_wndRef.GetToolBarCtrl().GetToolTips()->UpdateTipText(IDS_REFRESH, this, IDOK);

	return 0;
}

void CShellWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	int nWidth = 20;
	m_ShellTreeCtrl.MoveWindow(0,0,cx,cy-nWidth);
	m_fType.MoveWindow(1,cy - nWidth, cx-nWidth, nWidth);
	m_wndRef.MoveWindow(cx-nWidth-2, cy -nWidth, nWidth+2, nWidth);

//	m_ShellTreeCtrl.MoveWindow(0,0,cx,cy);

}

void CShellWnd::OnRefresh()
{
	CString str;
	if(m_ShellTreeCtrl.GetCurFilePath(str))
	{
		LockWindowUpdate();
		m_fType.GetWindowText(m_ShellTreeCtrl.m_strFilter);
		if(m_ShellTreeCtrl.m_strFilter.Right(1) != ";")
			m_ShellTreeCtrl.m_strFilter += ";";
		m_ShellTreeCtrl.DeleteAllItems();
		m_ShellTreeCtrl.PopulateTree();
		m_ShellTreeCtrl.TunnelTree(str);
		UnlockWindowUpdate();
	}
}

//#pragma optimize( "s", off )

