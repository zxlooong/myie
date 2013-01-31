// ExternalUtils.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE.h"
#include "ExternalUtils.h"
#include "MainFrm.h"
#include "InputUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#pragma optimize( "s", on)

/////////////////////////////////////////////////////////////////////////////
// CExternalUtils dialog


CExternalUtils::CExternalUtils(CWnd* pParent /*=NULL*/)
	: CDialog(CExternalUtils::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExternalUtils)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CExternalUtils::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExternalUtils)
	DDX_Control(pDX, IDC_STARTUP, m_cStartup);
	DDX_Control(pDX, IDC_CLOSEUTIL, m_cClose);
	DDX_Control(pDX, IDC_NAME, m_cName);
	DDX_Control(pDX, IDC_COMMAND, m_cCommand);
	DDX_Control(pDX, IDC_EXTERN_UTIL, m_UtilList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExternalUtils, CDialog)
	//{{AFX_MSG_MAP(CExternalUtils)
	ON_BN_CLICKED(IDC_ADD_UTIL, OnAddUtil)
	ON_BN_CLICKED(IDC_MOD_UTIL, OnModUtil)
	ON_BN_CLICKED(IDC_DEL_UTIL, OnDelUtil)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_EXTERN_UTIL, OnEndlabeleditExternUtil)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_EXTERN_UTIL, OnItemchangedExternUtil)
	ON_BN_CLICKED(IDC_BROWSER_FILE, OnBrowserFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExternalUtils message handlers

BOOL CExternalUtils::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_UtilList.SetExtendedStyle(m_UtilList.GetExtendedStyle()|LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);

	int i=0;
	CRect rect;
	m_UtilList.InsertColumn(0,"Name");
	m_UtilList.GetWindowRect(rect);
	m_UtilList.SetColumnWidth(0, 70);
	m_UtilList.InsertColumn(1,"Command");
	m_UtilList.SetColumnWidth(1, rect.Width()-75);

	m_ExternalList.Clone(&((CMainFrame*)pMainFrame)->m_ExternalUtilList);

	m_bSkip = TRUE;
	for (i=0;i <= m_ExternalList.m_UtilList.GetUpperBound();i++)
	{
		m_UtilList.InsertItem(i, m_ExternalList.m_UtilList.GetAt(i)->m_strUtilName);
		m_UtilList.SetItemText(i, 1, m_ExternalList.m_UtilList.GetAt(i)->m_strUtilCmd);
		if(m_ExternalList.m_UtilList.GetAt(i)->m_bUtilStart)
			m_UtilList.SetCheck(i);
	}

	m_bSkip = FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CExternalUtils::OnOK() 
{
	// TODO: Add extra validation here
	((CMainFrame*)pMainFrame)->m_ExternalUtilList.Clone(&m_ExternalList);
	
	CDialog::OnOK();
}

void CExternalUtils::OnAddUtil() 
{
	// TODO: Add your control notification handler code here
	CString Name;
	m_cName.GetWindowText(Name);
	CString Command;
	m_cCommand.GetWindowText(Command);
		if(!Command.IsEmpty())
		{
			if(Name.IsEmpty())
				Name = Command;
	
			m_bSkip = TRUE;
			int i = m_UtilList.GetItemCount();
			i = m_UtilList.InsertItem(i, Name);
			m_UtilList.SetItemText(i, 1, Command);
			m_UtilList.SetCheck(i,m_cStartup.GetCheck());

			CExternalUtilItem* eui = new CExternalUtilItem;
			eui->m_strUtilName = Name;
			eui->m_strUtilCmd = Command;
			eui->m_bUtilStart = m_cStartup.GetCheck();
			eui->m_bUtilClose = m_cClose.GetCheck();
			eui->m_iProcess = 0;
			m_ExternalList.m_UtilList.Add(eui);
			m_bSkip = FALSE;

			m_cName.SetWindowText("");
			m_cCommand.SetWindowText("");
			m_cStartup.SetCheck(FALSE);
			m_cClose.SetCheck(FALSE);
		}
}

void CExternalUtils::OnModUtil() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_UtilList.GetFirstSelectedItemPosition();
	int nItem;
	if(pos!=NULL)
	{
		nItem = m_UtilList.GetNextSelectedItem(pos);
		m_bSkip = TRUE;
		CString Name;
		m_cName.GetWindowText(Name);
		CString Command;
		m_cCommand.GetWindowText(Command);
		if(!Command.IsEmpty())
		{
			if(Name.IsEmpty())
				Name = Command;
	
			m_UtilList.SetItemText(nItem, 0, Name);
	
			m_UtilList.SetItemText(nItem, 1, Command);

			m_UtilList.SetCheck(nItem,m_cStartup.GetCheck());

			CExternalUtilItem* eui = m_ExternalList.m_UtilList.GetAt(nItem);
			eui->m_strUtilName = Name;
			eui->m_strUtilCmd = Command;
			eui->m_bUtilStart = m_cStartup.GetCheck();
			eui->m_bUtilClose = m_cClose.GetCheck();
		}
		m_bSkip = FALSE;
	}
	else
		return;
	
}

void CExternalUtils::OnDelUtil() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_UtilList.GetFirstSelectedItemPosition();
	int nItem;
	if(pos!=NULL)
	{
		nItem = m_UtilList.GetNextSelectedItem(pos);
		m_UtilList.DeleteItem(nItem);
		delete m_ExternalList.m_UtilList.GetAt(nItem);
		m_ExternalList.m_UtilList.RemoveAt(nItem);
	}
	else
		return;

}

void CExternalUtils::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	POSITION pos = m_UtilList.GetFirstSelectedItemPosition();
	int nItem = -1, newItem;
	if(pos!=NULL)
	{
		nItem = m_UtilList.GetNextSelectedItem(pos);
		if(pNMUpDown->iDelta<0)
			newItem = nItem -1;
		else
			newItem = nItem + 1;
		int n = m_UtilList.GetItemCount();
		if(newItem>=0 && newItem<n)
		{
			CString str = m_UtilList.GetItemText(nItem,0);
			CString str1 = m_UtilList.GetItemText(nItem,1);
			BOOL bs = m_UtilList.GetCheck(nItem);
			m_UtilList.DeleteItem(nItem);

			m_UtilList.InsertItem(newItem, str);
			m_UtilList.SetItemText(newItem, 1, str1);
			m_UtilList.SetCheck(newItem, bs);
			m_UtilList.SetItemState(newItem, LVIS_SELECTED, LVIS_SELECTED);
			m_UtilList.EnsureVisible(newItem, FALSE);

			CExternalUtilItem* eui = m_ExternalList.m_UtilList.GetAt(nItem);
			m_ExternalList.m_UtilList.SetAt(nItem, m_ExternalList.m_UtilList.GetAt(newItem));
			m_ExternalList.m_UtilList.SetAt(newItem, eui);
		}
	}	
	*pResult = 0;
}

void CExternalUtils::OnEndlabeleditExternUtil(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString str;
	m_UtilList.GetEditControl()->GetWindowText(str);
	str.TrimLeft();
	str.TrimRight();
	m_UtilList.SetItemText(pDispInfo->item.iItem,0,str);
	
	m_ExternalList.m_UtilList.GetAt(pDispInfo->item.iItem)->m_strUtilName = str;

	*pResult = 0;
}

void CExternalUtils::OnItemchangedExternUtil(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	if(!m_bSkip)
	{
		m_bSkip = FALSE;

		m_ExternalList.m_UtilList.GetAt(pNMListView->iItem)->m_bUtilStart = 
			m_UtilList.GetCheck(pNMListView->iItem);
		
		int i, nIndex = pNMListView->iItem;
		POSITION pos = m_UtilList.GetFirstSelectedItemPosition();
		if(pos!=NULL)
		{
			i = m_UtilList.GetNextSelectedItem(pos);
		}
		if(nIndex==i)
		{
			m_cName.SetWindowText(m_UtilList.GetItemText(nIndex,0));
			m_cCommand.SetWindowText(m_UtilList.GetItemText(nIndex,1));
			m_cStartup.SetCheck(m_UtilList.GetCheck(pNMListView->iItem));
			m_cClose.SetCheck(m_ExternalList.m_UtilList.GetAt(pNMListView->iItem)->m_bUtilClose);
		}
		else
		{
			m_cName.SetWindowText("");
			m_cCommand.SetWindowText("");
			m_cStartup.SetCheck(FALSE);
			m_cClose.SetCheck(FALSE);
		}
	}

	*pResult = 0;
}



void CExternalUtils::OnBrowserFile() 
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


//#pragma optimize( "s", off)
