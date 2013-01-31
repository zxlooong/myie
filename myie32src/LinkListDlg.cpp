// LinkListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "myie.h"
#include "LinkListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLinkListDlg dialog
//#pragma optimize( "s", on)


CLinkListDlg::CLinkListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLinkListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLinkListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLinkListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLinkListDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLinkListDlg, CDialog)
	//{{AFX_MSG_MAP(CLinkListDlg)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLinkListDlg message handlers

int CLinkListDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	SetIcon(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MyIEYPE)),FALSE);

	m_wndLinks.Create(NULL, "", WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN, CRect(0,0,0,0), this, IDS_TITLE_RESOURCE,NULL);
	return 0;
}


void CLinkListDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_wndLinks.SetWindowPos(&CWnd::wndTop, 0,0, cx,cy, 0);

}


void CLinkListDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(!IsIconic())
	{
		CRect wrect;
		CWinApp* app=AfxGetApp();
		GetWindowRect(&wrect);
		app->WriteProfileInt("Settings", "LinkListtop", wrect.top);
		app->WriteProfileInt("Settings", "LinkListleft", wrect.left);
		app->WriteProfileInt("Settings", "LinkListwidth", wrect.Width());
		app->WriteProfileInt("Settings", "LinkListheight", wrect.Height());
	}

	CFile::Remove(((CMyIEApp*)AfxGetApp())->m_strLinkListPath+"tmp");
}


//#pragma optimize( "s", off)
