// GroupUrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "myie.h"
#include "GroupUrlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#pragma optimize( "s", on )


/////////////////////////////////////////////////////////////////////////////
// CGroupUrlDlg dialog
typedef HRESULT (CALLBACK* LPFNDLLFUNC1)(HWND ,DWORD);


CGroupUrlDlg::CGroupUrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGroupUrlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGroupUrlDlg)
	m_strName = _T("");
	m_strUrl = _T("");
	//}}AFX_DATA_INIT
}


void CGroupUrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGroupUrlDlg)
	DDX_Control(pDX, IDC_NAME, m_edName);
	DDX_Control(pDX, IDC_URL, m_edURL);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_URL, m_strUrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGroupUrlDlg, CDialog)
	//{{AFX_MSG_MAP(CGroupUrlDlg)
	ON_EN_CHANGE(IDC_NAME, OnChangeName)
	ON_EN_CHANGE(IDC_URL, OnChangeUrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroupUrlDlg message handlers

void CGroupUrlDlg::OnChangeName() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(!m_bNotEmpty)
		return;

	CString name;
	m_edName.GetWindowText(name);
	name.TrimLeft();
	if(name.IsEmpty())
		m_btnOK.EnableWindow(0);
	else
	{
		if(!m_btnOK.IsWindowEnabled())
			m_btnOK.EnableWindow();
	}
}

void CGroupUrlDlg::OnChangeUrl() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString url;
	m_edURL.GetWindowText(url);
	url.TrimLeft();
	if(url.IsEmpty())
		m_btnOK.EnableWindow(0);
	else
	{
		if(!m_btnOK.IsWindowEnabled())
			m_btnOK.EnableWindow();
	}	
}

BOOL CGroupUrlDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	try{
	if(m_strUrl.IsEmpty())
		m_btnOK.EnableWindow(0);

	//auto complete
	HINSTANCE hIns = LoadLibrary("shlwapi.dll");
//		AfxMessageBox(IDS_SHLWAPI_FAIL);
//	else
	if(hIns != NULL)
	{
		LPFNDLLFUNC1 lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hIns, "SHAutoComplete");
		if(lpfnDllFunc1!=NULL)
			lpfnDllFunc1(m_edURL.m_hWnd, 0xe);
		FreeLibrary(hIns);
	}
	}catch(...)
	{
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//#pragma optimize( "s", off)

