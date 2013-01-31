// InputAlias.cpp : implementation file
//

#include "stdafx.h"
#include "myie.h"
#include "InputAlias.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// CInputAlias dialog
typedef HRESULT (CALLBACK* LPFNDLLFUNC1)(HWND ,DWORD);


CInputAlias::CInputAlias(CWnd* pParent /*=NULL*/)
	: CDialog(CInputAlias::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputAlias)
	m_alias = _T("");
	m_url = _T("");
	//}}AFX_DATA_INIT
}


void CInputAlias::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputAlias)
	DDX_Control(pDX, IDC_URL, m_edURL);
	DDX_Text(pDX, IDC_ALIAS, m_alias);
	DDX_Text(pDX, IDC_URL, m_url);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputAlias, CDialog)
	//{{AFX_MSG_MAP(CInputAlias)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputAlias message handlers

BOOL CInputAlias::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	try{
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

