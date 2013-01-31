// QuickSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "myie.h"
#include "QuickSearchDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// CQuickSearchDlg dialog


CQuickSearchDlg::CQuickSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQuickSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQuickSearchDlg)
	//}}AFX_DATA_INIT
}


void CQuickSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQuickSearchDlg)
	DDX_Control(pDX, IDC_SEARCHENGINE, m_cSearchEngine);
	DDX_Control(pDX, IDC_NAME, m_cName);
	DDX_Control(pDX, IDC_SEACHLIST, m_EngineList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CQuickSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CQuickSearchDlg)
	ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
	ON_BN_CLICKED(IDC_INSERT, OnInsert)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SEACHLIST, OnItemchangedSeachlist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQuickSearchDlg message handlers

BOOL CQuickSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_EngineList.SetExtendedStyle(m_EngineList.GetExtendedStyle()| LVS_EX_FULLROWSELECT);

	//setup list ctrl
	CRect rect;
	int i=0;
	m_bUpdating = FALSE;

	m_EngineList.InsertColumn(0,"Name");
	m_EngineList.GetWindowRect(rect);
	m_EngineList.SetColumnWidth(0, 60);
	m_EngineList.InsertColumn(1,"Search Engine");
	m_EngineList.SetColumnWidth(1, rect.Width()-73);

	POSITION pos = ((CMainFrame*)pMainFrame)->m_QuickSearchMap.GetStartPosition();
	CString name, url;
	while(pos!=NULL)
	{
		((CMainFrame*)pMainFrame)->m_QuickSearchMap.GetNextAssoc(pos, name, url);
		m_EngineList.InsertItem(i, name);
		m_EngineList.SetItemText(i, 1, url);
		i++;
	}

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CQuickSearchDlg::OnOK() 
{
	// TODO: Add extra validation here
	((CMainFrame*)pMainFrame)->m_QuickSearchMap.RemoveAll();
	int n = m_EngineList.GetItemCount();
	CString alias;
	CString filename = ((CMyIEApp*)AfxGetApp())->m_strQuickSearchPath;
	char key[9];
	WritePrivateProfileSection("QuickSearch", NULL, filename);
	//save quick search
	for( int i=0; i<n; i++)
	{
		alias = m_EngineList.GetItemText(i,0);
		alias.TrimLeft(); alias.TrimRight();
		((CMainFrame*)pMainFrame)->m_QuickSearchMap.SetAt(alias,m_EngineList.GetItemText(i,1));
		key[0]='N';
		itoa(i, key+1, 10);
		WritePrivateProfileString("QuickSearch", key, alias, filename);
		key[0]='S';
		itoa(i, key+1, 10);
		WritePrivateProfileString("QuickSearch", key, m_EngineList.GetItemText(i,1), filename);
	}
	
	CDialog::OnOK();
}

void CQuickSearchDlg::OnUpdate() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_EngineList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int i = m_EngineList.GetNextSelectedItem(pos);
		CString alias;
		m_cName.GetWindowText(alias);
		CString url;
		m_cSearchEngine.GetWindowText(url);
		alias.TrimLeft();
		if(alias.Find(' ')>0)
		{
			AfxMessageBox(IDS_INVALIDSEARCHNAME);
			return;	
		}
		if(url.Find("%s")<0)
		{
			AfxMessageBox(IDS_NEEDKEYSTRING);
			return;
		}


		alias.TrimLeft(); alias.TrimRight();
		url.TrimLeft();
		if(alias !="" && !alias.IsEmpty() && url!="" && !url.IsEmpty())
		{
			m_bUpdating = TRUE;
			m_EngineList.SetItemText(i, 0, alias);
			m_EngineList.SetItemText(i, 1, url);
		}
	}	
}

void CQuickSearchDlg::OnInsert() 
{
	// TODO: Add your control notification handler code here
	CString alias;
	m_cName.GetWindowText(alias);
	CString url;
	m_cSearchEngine.GetWindowText(url);
	
	alias.TrimLeft();
	if(alias.Find(' ')>0)
	{
		AfxMessageBox(IDS_INVALIDSEARCHNAME);
		return;	
	}
	if(url.Find("%s")<0)
	{
		AfxMessageBox(IDS_NEEDKEYSTRING);
		return;
	}


	alias.TrimLeft(); alias.TrimRight();
	url.TrimLeft();
	int i = m_EngineList.GetItemCount();
	if(alias !="" && !alias.IsEmpty() && url!="" && !url.IsEmpty())
	{
		m_EngineList.InsertItem(i, alias);
		m_EngineList.SetItemText(i, 1, url);
		m_cName.SetWindowText("");
		m_cSearchEngine.SetWindowText("");

	}
}

void CQuickSearchDlg::OnDelete() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_EngineList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int i = m_EngineList.GetNextSelectedItem(pos);
		m_EngineList.DeleteItem(i);
	}	
	
}



void CQuickSearchDlg::OnItemchangedSeachlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if(m_bUpdating)
	{
		m_bUpdating = FALSE;
		return;
	}

	int i, nIndex = pNMListView->iItem;
	POSITION pos = m_EngineList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		i = m_EngineList.GetNextSelectedItem(pos);
	}
	if(nIndex==i)
	{
		m_cName.SetWindowText(m_EngineList.GetItemText(nIndex,0));
		m_cSearchEngine.SetWindowText(m_EngineList.GetItemText(nIndex,1));
	}
	else
	{
		m_cName.SetWindowText("");
		m_cSearchEngine.SetWindowText("");
	}

	*pResult = 0;
}

//#pragma optimize( "s", off)
