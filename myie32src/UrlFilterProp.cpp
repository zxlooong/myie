// UrlFilterProp.cpp : implementation file
//

#include "stdafx.h"
#include "myie.h"
#include "UrlFilterProp.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUrlFilterProp property page

IMPLEMENT_DYNCREATE(CUrlFilterProp, CPropertyPage)

CUrlFilterProp::CUrlFilterProp() : CPropertyPage(CUrlFilterProp::IDD)
{
	//{{AFX_DATA_INIT(CUrlFilterProp)
	m_bUseUrlFilter = FALSE;
	//}}AFX_DATA_INIT
}

CUrlFilterProp::~CUrlFilterProp()
{
}

void CUrlFilterProp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUrlFilterProp)
	DDX_Control(pDX, IDC_URL_LIST, m_cURLList);
	DDX_Check(pDX, IDC_URL_FILTER, m_bUseUrlFilter);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUrlFilterProp, CPropertyPage)
	//{{AFX_MSG_MAP(CUrlFilterProp)
	ON_BN_CLICKED(IDC_INSERT, OnInsert)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_URL_LIST, OnDblclkUrlList)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_URL_LIST, OnEndlabeleditUrlList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUrlFilterProp message handlers

void CUrlFilterProp::OnInsert() 
{
	// TODO: Add your control notification handler code here
	int i = m_cURLList.GetItemCount();
	m_cURLList.InsertItem(i, "New Item");
	m_cURLList.SetFocus();
	m_cURLList.EnsureVisible(i,FALSE);
	m_cURLList.EditLabel(i);
}

void CUrlFilterProp::OnEdit() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_cURLList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int nItem = m_cURLList.GetNextSelectedItem(pos);
		m_cURLList.SetFocus();
		m_cURLList.EditLabel(nItem);
	}	
}

void CUrlFilterProp::OnDelete() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_cURLList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int nItem = m_cURLList.GetNextSelectedItem(pos);
		m_cURLList.DeleteItem(nItem);
		m_cURLList.SetItemState(nItem, LVIS_SELECTED,LVIS_SELECTED);
	}
}

BOOL CUrlFilterProp::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	int i=0;
	CRect rect;
	m_cURLList.InsertColumn(0,"");
	m_cURLList.GetWindowRect(rect);
	m_cURLList.SetColumnWidth(0, rect.Width());
	for(i=0; i<=pMainFrame->m_astrUrlFilter.GetUpperBound( ) ; i++)
	{
		m_cURLList.InsertItem(i, ((CMainFrame*)pMainFrame)->m_astrUrlFilter.GetAt(i));
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUrlFilterProp::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	((CMainFrame*)pMainFrame)->m_astrUrlFilter.RemoveAll();
	int n = m_cURLList.GetItemCount();
	for( int i=0; i<n; i++)
	{
		((CMainFrame*)pMainFrame)->m_astrUrlFilter.Add(m_cURLList.GetItemText(i,0));
	}
	((CMainFrame*)pMainFrame)->m_bUrlFilterDirty = TRUE;
	((CMainFrame*)pMainFrame)->InitialUrlFilter();

	CPropertyPage::OnOK();
}

void CUrlFilterProp::OnDblclkUrlList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnEdit();
	
	*pResult = 0;
}

void CUrlFilterProp::OnEndlabeleditUrlList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString str;
	m_cURLList.GetEditControl()->GetWindowText(str);

	m_cURLList.SetItemText(pDispInfo->item.iItem,0,str);
	
	*pResult = 0;
}
