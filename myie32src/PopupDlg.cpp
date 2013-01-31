// PopupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE.h"
#include "PopupDlg.h"
#include "MainFrm.h"
//#include "SmartEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// CPopupDlg dialog
IMPLEMENT_DYNCREATE(CPopupDlg, CPropertyPage)

CPopupDlg::CPopupDlg()
	: CPropertyPage(CPopupDlg::IDD)
{
	//{{AFX_DATA_INIT(CPopupDlg)
	m_EnableFilter = FALSE;
	m_EnableDupFilter = FALSE;
	m_EnableAutoFilter = FALSE;
	//}}AFX_DATA_INIT
}


void CPopupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPopupDlg)
	DDX_Control(pDX, IDD_POPUPLIST, m_PopupList);
	DDX_Check(pDX, IDC_ENABLE_POPUP, m_EnableFilter);
	DDX_Check(pDX, IDC_FILTER_DUP, m_EnableDupFilter);
	DDX_Check(pDX, IDC_AUTO_FILTER, m_EnableAutoFilter);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPopupDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CPopupDlg)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_INSERT, OnInsert)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDD_POPUPLIST, OnEndlabeleditPopuplist)
	ON_NOTIFY(NM_DBLCLK, IDD_POPUPLIST, OnDblclkPopuplist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPopupDlg message handlers

BOOL CPopupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	try{

	POSITION pos = ((CMainFrame*)pMainFrame)->m_astrPopup.GetHeadPosition();
	int i=0;
	CRect rect;
	m_PopupList.InsertColumn(0,"");
	m_PopupList.GetWindowRect(rect);
	m_PopupList.SetColumnWidth(0, rect.Width());
	while(pos!=NULL)
	{
		m_PopupList.InsertItem(i, ((CMainFrame*)pMainFrame)->m_astrPopup.GetAt(pos));
		((CMainFrame*)pMainFrame)->m_astrPopup.GetNext(pos);
		i++;
	}

	}catch(...)
	{
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPopupDlg::OnOK() 
{
	// TODO: Add extra validation here
	((CMainFrame*)pMainFrame)->m_astrPopup.RemoveAll();
	int n = m_PopupList.GetItemCount();
	for( int i=0; i<n; i++)
	{
		((CMainFrame*)pMainFrame)->m_astrPopup.AddTail(m_PopupList.GetItemText(i,0));
	}
	
	CDialog::OnOK();
}

void CPopupDlg::OnDelete() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_PopupList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int nItem = m_PopupList.GetNextSelectedItem(pos);
		m_PopupList.DeleteItem(nItem);
		m_PopupList.SetItemState(nItem, LVIS_SELECTED,LVIS_SELECTED);
	}
}

void CPopupDlg::OnEdit() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_PopupList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int nItem = m_PopupList.GetNextSelectedItem(pos);
		m_PopupList.SetFocus();
		m_PopupList.EditLabel(nItem);
	}

/*	const INT nIndex = m_PopupList.GetCurSel();
	if(nIndex == LB_ERR) return;
	CString string;
	m_PopupList.GetText(nIndex, string);
	RECT rect;
	INT result = m_PopupList.GetItemRect(nIndex, &rect);
	if(result==LB_ERR) return;
	CSmartEdit* pEdit = new CSmartEdit(&m_PopupList);
	rect.bottom += 4;
	pEdit->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		rect, &m_PopupList, (UINT)-1);
	pEdit->SetWindowText(string);
	pEdit->SetFont(m_PopupList.GetFont());
	pEdit->SetFocus();
	pEdit->LimitText(MAX_PATH); */// user defined maximum length of string
}

void CPopupDlg::OnInsert() 
{
	// TODO: Add your control notification handler code here
	int i = m_PopupList.GetItemCount();
	m_PopupList.InsertItem(i, "New Item");
	m_PopupList.SetFocus();
	m_PopupList.EnsureVisible(i,FALSE);
	m_PopupList.EditLabel(i);

}

void CPopupDlg::OnEndlabeleditPopuplist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString str;
	m_PopupList.GetEditControl()->GetWindowText(str);

	m_PopupList.SetItemText(pDispInfo->item.iItem,0,str);
	*pResult = 0;
}

void CPopupDlg::OnDblclkPopuplist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnEdit();
	
	*pResult = 0;
}

//#pragma optimize( "s", off)

