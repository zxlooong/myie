// OrgGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "myie.h"
#include "MainFrm.h"
#include "OrgGroupDlg.h"
#include "GroupUrlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// COrgGroupDlg dialog


COrgGroupDlg::COrgGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COrgGroupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COrgGroupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDragImage = NULL;

}


void COrgGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COrgGroupDlg)
	DDX_Control(pDX, IDC_INSERT, m_btnInsert);
	DDX_Control(pDX, IDC_URL_LIST, m_UrlList);
	DDX_Control(pDX, IDC_GROUP_LIST, m_GroupList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COrgGroupDlg, CDialog)
	//{{AFX_MSG_MAP(COrgGroupDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_GROUP_LIST, OnSelChanging)
	ON_BN_CLICKED(IDC_NEW_GROUP, OnNewGroup)
	ON_BN_CLICKED(IDC_DEL_GROUP, OnDelGroup)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_GROUP_LIST, OnEndlabeleditGroupList)
	ON_BN_CLICKED(IDC_INSERT, OnInsert)
	ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, OnDeltaposSpin2)
	ON_NOTIFY(NM_CLICK, IDC_GROUP_LIST, OnClickGroupList)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_URL_LIST, OnBegindragUrlList)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY(NM_DBLCLK, IDC_URL_LIST, OnDblclkUrlList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrgGroupDlg message handlers

BOOL COrgGroupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_bInit = TRUE;
	
	// TODO: Add extra initialization here
	m_GroupList.SetExtendedStyle(m_GroupList.GetExtendedStyle()|LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);

	//setup list ctrl
	CRect rect;
	int i=0;
	m_GroupList.InsertColumn(0,"");
	m_GroupList.GetWindowRect(rect);
	m_GroupList.SetColumnWidth(0, rect.Width()-10);

	m_UrlList.InsertColumn(0,"Name");
	m_UrlList.GetWindowRect(rect);
	m_UrlList.SetColumnWidth(0, 100);
	m_UrlList.InsertColumn(1,"URL");
	m_UrlList.SetColumnWidth(1, rect.Width()-110);

	//load group list
	m_strStartGroup = ((CMainFrame*)pMainFrame)->m_strStartGroup;
	if(((CMyIEApp*)AfxGetApp())->m_strGroupPath.Right(1) != "\\")
		((CMyIEApp*)AfxGetApp())->m_strGroupPath += "\\";
	CString psz = ((CMyIEApp*)AfxGetApp())->m_strGroupPath;

	WIN32_FIND_DATA wfd;
	HANDLE          h;
	psz += "*.cgp";
	
	CString strMenu;
	h = FindFirstFile(psz, &wfd);
	if(h != INVALID_HANDLE_VALUE)
	{
		do{
			strMenu = wfd.cFileName;
			strMenu = strMenu.Left(strMenu.GetLength()-4);
			m_GroupList.InsertItem(i, strMenu);
			if(strMenu == m_strStartGroup)
			{
				m_GroupList.SetCheck(i);
			}
			i++;
		}
		while(FindNextFile(h, &wfd));
	} 

	FindClose(h);

	m_GroupList.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	
	m_nLastSelItemID = -1;
	FillUrlList(0);
	if(i==0)
			m_btnInsert.EnableWindow(FALSE);

	m_bInit = FALSE;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COrgGroupDlg::FillUrlList(int index)
{
	if(m_nLastSelItemID == index)
		return;

	if(m_nLastSelItemID>=0)
		SaveUrlList(m_nLastSelItemID);

	m_nLastSelItemID = index;

	//remove all first
	m_UrlList.DeleteAllItems();
	m_UrlList.SetItemCount(0);

	if(index<0)
		return;

	CString filename;
	filename = m_GroupList.GetItemText(index, 0);

	filename = ((CMyIEApp*)AfxGetApp())->m_strGroupPath + filename+".cgp";

	char * valuepair = NULL;
	int nsize = 0;
	HANDLE hfile = INVALID_HANDLE_VALUE;
	try{
		hfile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hfile == INVALID_HANDLE_VALUE)
			return;
		
		nsize = GetFileSize(hfile, NULL);
		CloseHandle(hfile);
		hfile = INVALID_HANDLE_VALUE;

		if(nsize>0)
		{
			nsize*=2;
			valuepair = new char[nsize+1];
			GetPrivateProfileSection("CaptorGroup", valuepair, nsize, filename);

			char * value, * tmp;
			int i =0;
			value = valuepair;
			while(value!=NULL && strlen(value)>0)
			{
				tmp = strchr(value, '=');
				*tmp = '\0';
				if(value!=NULL)
				{
					//value is name
					m_UrlList.InsertItem(i, value);
					value = tmp;
					value ++;
					m_UrlList.SetItemText(i, 1, value);
					i++;
				}
				value = strchr(value, '\0');
				value ++;
			}

			delete[] valuepair;
			valuepair = NULL;
		}
	}catch(...)
	{//?
		if(valuepair != NULL)
			delete[] valuepair;
		if(hfile!=INVALID_HANDLE_VALUE)
			CloseHandle(hfile);
	}
}

void COrgGroupDlg::SaveUrlList(int index)
{
	if(index<0)
		return;

	CString filename;
	filename = m_GroupList.GetItemText(index, 0);
	if(filename.IsEmpty())
		return;

	filename = ((CMyIEApp*)AfxGetApp())->m_strGroupPath + filename+".cgp";

	int n = m_UrlList.GetItemCount();
	WritePrivateProfileSection("CaptorGroup", NULL, filename);
	for( int i=0; i<n; i++)
	{
		WritePrivateProfileString("CaptorGroup", m_UrlList.GetItemText(i,0), m_UrlList.GetItemText(i,1), filename);
	}
	
}

void COrgGroupDlg::OnSelChanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_bInit)
		return;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int i = m_GroupList.GetSelectedCount();
	if(i>0)
	{
		POSITION pos = m_GroupList.GetFirstSelectedItemPosition();
		int item = m_GroupList.GetNextSelectedItem(pos);
		FillUrlList(item);
		m_btnInsert.EnableWindow(1);
	}
	else
	{
		FillUrlList(-1);
		m_btnInsert.EnableWindow(0);
	}

	*pResult = 0;
}



void COrgGroupDlg::OnNewGroup() 
{
	// TODO: Add your control notification handler code here
	CString def = "New Group", str(def);
	
	char si[5]; int i=0;
	LVFINDINFO info;

	info.flags = LVFI_STRING;
	info.psz = str;
	
	while (m_GroupList.FindItem(&info) != -1)
	{
		i++;
		itoa(i, si, 10);
		str = def + " (";
		str += si;
		str += ")";
		info.psz = str;
	}
	
	i = m_GroupList.GetItemCount();
	m_GroupList.InsertItem(i, str);
	m_GroupList.SetFocus();
	m_GroupList.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);

	CString filename;
	filename = ((CMyIEApp*)AfxGetApp())->m_strGroupPath + str+".cgp";

	HANDLE hfile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	CloseHandle(hfile);

	m_GroupList.EnsureVisible(i,FALSE);
	m_GroupList.EditLabel(i);

}

void COrgGroupDlg::OnDelGroup() 
{
	// TODO: Add your control notification handler code here
	int i = m_GroupList.GetSelectedCount();
	if(i>0)
	{
		POSITION pos = m_GroupList.GetFirstSelectedItemPosition();
		int item = m_GroupList.GetNextSelectedItem(pos);
		if(AfxMessageBox(IDS_CONFIRM_DEL, MB_YESNO|MB_ICONSTOP) == IDYES)
		{
			BOOL rem = FALSE;
			CString filename;
			filename = m_GroupList.GetItemText(item, 0);
			if(filename == m_strStartGroup)
				rem = TRUE;

			filename = ((CMyIEApp*)AfxGetApp())->m_strGroupPath + filename+".cgp";

			if(DeleteFile(filename))
			{
				m_nLastSelItemID = -1;
				m_GroupList.DeleteItem(item);
				m_UrlList.DeleteAllItems();
				if(rem)
					m_strStartGroup = "";
			}
		}
	}
}

void COrgGroupDlg::OnEndlabeleditGroupList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString str;
	m_GroupList.GetEditControl()->GetWindowText(str);
	str.TrimLeft();
	str.TrimRight();

	//rename file
	CString filename;
	BOOL ren = FALSE;
	filename = m_GroupList.GetItemText(pDispInfo->item.iItem, 0);
	if(filename == m_strStartGroup)
		ren = TRUE;

	filename = ((CMyIEApp*)AfxGetApp())->m_strGroupPath + filename+".cgp";

	if(MoveFile(filename, ((CMyIEApp*)AfxGetApp())->m_strGroupPath + str + ".cgp"))
	{
		m_GroupList.SetItemText(pDispInfo->item.iItem,0,str);
		if(ren)
			m_strStartGroup = str;
	}
	
	*pResult = 0;
}

void COrgGroupDlg::OnOK() 
{
	// TODO: Add extra validation here
	int i = m_GroupList.GetSelectedCount();
	if(i>0)
	{
		POSITION pos = m_GroupList.GetFirstSelectedItemPosition();
		int item = m_GroupList.GetNextSelectedItem(pos);
		SaveUrlList(item);
	}
	((CMainFrame*)pMainFrame)->m_strStartGroup = m_strStartGroup;
	
	CDialog::OnOK();
}

void COrgGroupDlg::OnDelete() 
{
	// TODO: Add your control notification handler code here
	int i = m_UrlList.GetSelectedCount();
	if(i>0)
	{
		POSITION pos = m_UrlList.GetFirstSelectedItemPosition();
		int item = m_UrlList.GetNextSelectedItem(pos);
		m_UrlList.DeleteItem(item);
	}
}

void COrgGroupDlg::OnInsert() 
{
	// TODO: Add your control notification handler code here
	if(m_nLastSelItemID<0)
		return;

	CGroupUrlDlg dlg;
	dlg.m_bNotEmpty = FALSE;
	if(dlg.DoModal() == IDOK)
	{
		int item;
		int i = m_UrlList.GetSelectedCount();
		if(i>0)
		{
			POSITION pos = m_UrlList.GetFirstSelectedItemPosition();
			item = m_UrlList.GetNextSelectedItem(pos);
		}
		else
			item = m_UrlList.GetItemCount();

		CString str = dlg.m_strName;
		str.TrimLeft();
		str.TrimRight();
		if(str.IsEmpty())
			GetUnqBlankName(str);
		m_UrlList.InsertItem(item, str);
		dlg.m_strUrl.TrimLeft();
		m_UrlList.SetItemText(item, 1, dlg.m_strUrl);

	}
}

void COrgGroupDlg::OnUpdate() 
{
	// TODO: Add your control notification handler code here
	int item;
	int i = m_UrlList.GetSelectedCount();
	if(i>0)
	{
		POSITION pos = m_UrlList.GetFirstSelectedItemPosition();
		item = m_UrlList.GetNextSelectedItem(pos);
	}
	else
		return;
	

	CGroupUrlDlg dlg;
	dlg.m_strName = m_UrlList.GetItemText(item, 0);
	dlg.m_strUrl = m_UrlList.GetItemText(item,1);
	if(dlg.DoModal() == IDOK)
	{
		CString str = dlg.m_strName;
		str.TrimLeft();
		str.TrimRight();
		if(str.IsEmpty())
			GetUnqBlankName(str);
		m_UrlList.SetItemText(item, 0, str);
		dlg.m_strUrl.TrimLeft();
		m_UrlList.SetItemText(item, 1, dlg.m_strUrl);

	}	
}

void COrgGroupDlg::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	POSITION pos = m_UrlList.GetFirstSelectedItemPosition();
	int nItem = -1, newItem;
	if(pos!=NULL)
	{
		nItem = m_UrlList.GetNextSelectedItem(pos);
		if(pNMUpDown->iDelta<0)
			newItem = nItem -1;
		else
			newItem = nItem + 1;
		int n = m_UrlList.GetItemCount();
		if(newItem>=0 && newItem<n)
		{
			CString str = m_UrlList.GetItemText(nItem,0);
			CString str2 = m_UrlList.GetItemText(nItem, 1);
			m_UrlList.DeleteItem(nItem);

			m_UrlList.InsertItem(newItem, str);
			m_UrlList.SetItemText(newItem, 1, str2);
			m_UrlList.SetItemState(newItem, LVIS_SELECTED, LVIS_SELECTED);
			m_UrlList.EnsureVisible(newItem, FALSE);

		}
	}
	
	*pResult = 0;
}

void COrgGroupDlg::GetUnqBlankName(CString &newblnk)
{
	CString def = "blank";
	newblnk = def;
	
	char si[5]; int i=0;
	LVFINDINFO info;

	info.flags = LVFI_STRING;
	
	do{
		i++;
		itoa(i, si, 10);
		newblnk = def;
		newblnk += si;
		info.psz = newblnk;
	}while(m_UrlList.FindItem(&info) != -1);

}


void COrgGroupDlg::OnClickGroupList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint pt;
	UINT flag;
	GetCursorPos(&pt);
	m_GroupList.ScreenToClient(&pt);
	int iItem = m_GroupList.HitTest(pt, &flag);
	if(flag != LVHT_ONITEMSTATEICON  || iItem<0)
		return;

	BOOL bcheck = m_GroupList.GetCheck(iItem);

	LVFINDINFO info;
	info.flags = LVFI_STRING;
	info.psz = m_strStartGroup;
	int item = m_GroupList.FindItem(&info);

	if(!bcheck)
	{
		m_GroupList.SetCheck(item, 0);
		//m_GroupList.SetCheck(iItem);
		m_strStartGroup = m_GroupList.GetItemText(iItem, 0);
	}
	else if(iItem == item)
	{
		m_strStartGroup = "";
	}
	
	*pResult = 0;
}

void COrgGroupDlg::OnBegindragUrlList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_nDragIndex = pNMListView->iItem;
	POINT pt;
	pt.x = 8;
	pt.y = 8;
	// create a drag image

	// Added 7/29/96
	if(m_pDragImage)
		delete m_pDragImage;
	
	m_pDragImage = m_UrlList.CreateDragImage (m_nDragIndex, &pt);
	ASSERT (m_pDragImage);
	// changes the cursor to the drag image (DragMove() is still required in 
	// OnMouseMove())
	VERIFY (m_pDragImage->BeginDrag (0, CPoint (8, 8)));
	VERIFY (m_pDragImage->DragEnter (GetDesktopWindow (), pNMListView->ptAction));
	// set dragging flag
	m_bDragging = TRUE;
	m_nDropIndex = -1;
	m_pDropWnd = &m_GroupList;
	// capture all mouse messages
	SetCapture ();
	
	*pResult = 0;
}

void COrgGroupDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bDragging)
	{
		// release mouse capture
		VERIFY (::ReleaseCapture ());
		m_bDragging = FALSE;
		// end dragging
		VERIFY (m_pDragImage->DragLeave (GetDesktopWindow ()));
		m_pDragImage->EndDrag ();

		// turn off hilight for previous drop target
		m_GroupList.SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED);
		// redraw previous item
		m_GroupList.RedrawItems (m_nDropIndex, m_nDropIndex);

		CPoint pt (point);
		ClientToScreen (&pt);
		// get the CWnd pointer of the window that is under the mouse cursor
		// if window is CTreeCtrl
		CWnd* pDropWnd = WindowFromPoint (pt);
		if (pDropWnd == m_pDropWnd && m_nDropIndex>=0 && m_nDropIndex!=m_nLastSelItemID)
		{
			CString name = m_UrlList.GetItemText(m_nDragIndex,0);
			CString url = m_UrlList.GetItemText(m_nDragIndex,1);

			//add to new list
			CString filename;
			filename = m_GroupList.GetItemText(m_nDropIndex, 0);

			filename = ((CMyIEApp*)AfxGetApp())->m_strGroupPath + filename+".cgp";

			WritePrivateProfileString("CaptorGroup", name, url, filename);

			short ks =GetKeyState(VK_CONTROL);
			if(ks>=0)
				OnDelete();
		}
	}
	
	CDialog::OnLButtonUp(nFlags, point);
}

void COrgGroupDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bDragging)
	{
		CPoint pt (point);
		ClientToScreen (&pt);
		// move the drag image
		VERIFY (m_pDragImage->DragMove (pt));
		// unlock window updates
		VERIFY (m_pDragImage->DragShowNolock (FALSE));

		// get the CWnd pointer of the window that is under the mouse cursor
		CWnd* pDropWnd = WindowFromPoint (pt);
		ASSERT (pDropWnd);

		// convert from screen coordinates to drop target client coordinates
		pDropWnd->ScreenToClient (&pt);

		// if window is CTreeCtrl
		if (pDropWnd == m_pDropWnd)
		{			
			UINT uFlags;
			CListCtrl* pList = (CListCtrl*)pDropWnd;

			// turn off hilight for previous drop target
			pList->SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED);
			// redraw previous item
			pList->RedrawItems (m_nDropIndex, m_nDropIndex);
			
			// get the item that is below cursor
			m_nDropIndex = ((CListCtrl*)pDropWnd)->HitTest (pt, &uFlags);
			// highlight it
			if(m_nDropIndex>=0)
			{
				pList->SetItemState (m_nDropIndex, LVIS_DROPHILITED, LVIS_DROPHILITED);
				// redraw item
				pList->RedrawItems (m_nDropIndex, m_nDropIndex);
				pList->UpdateWindow ();
			}
		}
		// lock window updates
		VERIFY (m_pDragImage->DragShowNolock (TRUE));
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

void COrgGroupDlg::OnDblclkUrlList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int item;
	int i = m_UrlList.GetSelectedCount();
	if(i>0)
	{
		POSITION pos = m_UrlList.GetFirstSelectedItemPosition();
		item = m_UrlList.GetNextSelectedItem(pos);
	}
	else
		return;

	((CMainFrame*)pMainFrame)->NewChildWindow(1,2, m_UrlList.GetItemText(item,1));

	*pResult = 0;
}

//#pragma optimize( "s", off)

