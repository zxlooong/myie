// InputQuickSearch.cpp : implementation file
//

#include "stdafx.h"
#include "myie.h"
#include "InputQuickSearch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// CInputQuickSearch dialog


CInputQuickSearch::CInputQuickSearch(CWnd* pParent /*=NULL*/)
	: CDialog(CInputQuickSearch::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputQuickSearch)
	m_strName = _T("");
	m_strEngine = _T("");
	//}}AFX_DATA_INIT
}


void CInputQuickSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputQuickSearch)
	DDX_Control(pDX, IDC_SEARCHENGINE, m_cEngine);
	DDX_Control(pDX, IDC_NAME, m_cName);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_SEARCHENGINE, m_strEngine);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputQuickSearch, CDialog)
	//{{AFX_MSG_MAP(CInputQuickSearch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputQuickSearch message handlers

void CInputQuickSearch::OnOK() 
{
	// TODO: Add extra validation here
	CString t;
	m_cName.GetWindowText(t);
	t.TrimLeft();
	if(t.Find(' ')>0)
	{
		AfxMessageBox(IDS_INVALIDSEARCHNAME);
		return;	
	}
	m_cEngine.GetWindowText(t);
	if(t.Find("%s")<0)
	{
		AfxMessageBox(IDS_NEEDKEYSTRING);
		return;
	}
	CDialog::OnOK();
}

//#pragma optimize( "s", off)

