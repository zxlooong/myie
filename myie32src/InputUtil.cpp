// InputUtil.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE.h"
#include "InputUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// CInputUtil dialog


CInputUtil::CInputUtil(CWnd* pParent /*=NULL*/)
	: CDialog(CInputUtil::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputUtil)
	m_Command = _T("");
	m_Name = _T("");
	m_bStartup = FALSE;
	m_bClose = FALSE;
	//}}AFX_DATA_INIT
}


void CInputUtil::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputUtil)
	DDX_Control(pDX, IDC_STARTUP, m_btnStart);
	DDX_Control(pDX, IDC_CLOSEUTIL, m_btnClose);
	DDX_Text(pDX, IDC_COMMAND, m_Command);
	DDX_Text(pDX, IDC_NAME, m_Name);
	DDX_Check(pDX, IDC_STARTUP, m_bStartup);
	DDX_Check(pDX, IDC_CLOSEUTIL, m_bClose);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputUtil, CDialog)
	//{{AFX_MSG_MAP(CInputUtil)
	ON_BN_CLICKED(IDC_BROWSER_FILE, OnBrowserFile)
	ON_BN_CLICKED(IDC_STARTUP, OnStartup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputUtil message handlers

void CInputUtil::OnBrowserFile() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"Exe Files (*.exe)|*.exe|All Files (*.*)|*.*||",
		this);
	if(dlg.DoModal()==IDOK)
	{
		CString str = dlg.GetPathName();
		str = "\""+str;
		str += "\"";
		GetDlgItem(IDC_COMMAND)->SetWindowText(str);
	}
}

BOOL CInputUtil::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(!m_bStartup)
		m_btnClose.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInputUtil::OnStartup() 
{
	// TODO: Add your control notification handler code here
		m_btnClose.EnableWindow(m_btnStart.GetCheck());
}

#pragma optimize( "s", off)

