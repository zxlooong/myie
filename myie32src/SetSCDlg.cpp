// SetSCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "myie.h"
#include "SetSCDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// CSetSCDlg dialog
typedef HRESULT (CALLBACK* LPFNDLLFUNC1)(HWND ,DWORD);


CSetSCDlg::CSetSCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetSCDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetSCDlg)
	m_strEdit = _T("");
	//}}AFX_DATA_INIT
}


void CSetSCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetSCDlg)
	DDX_Control(pDX, IDC_EDIT1, m_url);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_KEYLIST, m_keylist);
	DDX_Text(pDX, IDC_EDIT1, m_strEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetSCDlg, CDialog)
	//{{AFX_MSG_MAP(CSetSCDlg)
	ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
	ON_CBN_SELCHANGE(IDC_KEYLIST, OnSelchangeKeylist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetSCDlg message handlers

BOOL CSetSCDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rect;
	m_list.InsertColumn(0,"Key");
	m_list.GetWindowRect(rect);
	m_list.SetColumnWidth(0, 40);
	m_list.InsertColumn(1,"URL");
	m_list.SetColumnWidth(1, rect.Width()-58);

	char name[5] = "F";
	for(int i=0;i<11; i++)
	{
		itoa(i+2, name+1,10);
		m_list.InsertItem(i, name);
		m_list.SetItemText(i, 1, ((CMainFrame*)pMainFrame)->m_strShortCuts[i]);
	}
	
	//auto complete
	HINSTANCE hIns = LoadLibrary("shlwapi.dll");
//		AfxMessageBox(IDS_SHLWAPI_FAIL);
//	else
	if(hIns != NULL)
	{
		LPFNDLLFUNC1 lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hIns, "SHAutoComplete");
		if(lpfnDllFunc1!=NULL)
			lpfnDllFunc1(m_url.m_hWnd, 0xe);
		FreeLibrary(hIns);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetSCDlg::OnOK() 
{
	// TODO: Add extra validation here
	for( int i=0; i<11; i++)
	{
		((CMainFrame*)pMainFrame)->m_strShortCuts[i]=m_list.GetItemText(i,1);
	}
	
	CDialog::OnOK();
}

void CSetSCDlg::OnUpdate() 
{
	// TODO: Add your control notification handler code here
	int i;
	i = m_keylist.GetCurSel();
	if(i>=0)
	{
		CString url;
		m_url.GetWindowText(url);
		m_list.SetItemText(i, 1, url);
		m_url.SetWindowText("");
	}
}

void CSetSCDlg::OnSelchangeKeylist() 
{
	// TODO: Add your control notification handler code here
	CString url;
	m_url.GetWindowText(url);
	if(url.IsEmpty())
	{
		int i;
		i = m_keylist.GetCurSel();
		if(i>=0)
		{
			url = m_list.GetItemText(i, 1);
			m_url.SetWindowText(url);
		}
	}
}

//#pragma optimize( "s", off )

