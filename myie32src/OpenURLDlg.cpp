// OpenURLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "myie.h"
#include "OpenURLDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpenURLDlg dialog


COpenURLDlg::COpenURLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COpenURLDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COpenURLDlg)
	m_strURL = _T("");
	m_bInNewWin = TRUE;
	//}}AFX_DATA_INIT
}


void COpenURLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpenURLDlg)
	DDX_CBString(pDX, IDC_ADDRESS, m_strURL);
	DDX_Check(pDX, IDC_IN_NEW_WIN, m_bInNewWin);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COpenURLDlg, CDialog)
	//{{AFX_MSG_MAP(COpenURLDlg)
	ON_BN_CLICKED(IDC_BROWSER, OnBrowser)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenURLDlg message handlers

void COpenURLDlg::OnBrowser() 
{
	// TODO: Add your control notification handler code here
	
}
