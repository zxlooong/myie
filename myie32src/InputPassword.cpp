// InputPassword.cpp : implementation file
//

#include "stdafx.h"
#include "myie.h"
#include "InputPassword.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// CInputPassword dialog


CInputPassword::CInputPassword(CWnd* pParent /*=NULL*/)
	: CDialog(CInputPassword::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputPassword)
	m_strPassword = _T("");
	m_bRemPsw = FALSE;
	//}}AFX_DATA_INIT
}


void CInputPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputPassword)
	DDX_Text(pDX, IDC_PASSWORD, m_strPassword);
	DDX_Check(pDX, IDC_REM_PSW, m_bRemPsw);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputPassword, CDialog)
	//{{AFX_MSG_MAP(CInputPassword)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputPassword message handlers

BOOL CInputPassword::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//#pragma optimize( "s", off)

