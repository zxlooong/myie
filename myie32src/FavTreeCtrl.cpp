// FavTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE.h"
#include "FavTreeCtrl.h"
#include "MainFrm.h"
#include "setscdlg.h"
#include "inputalias.h"
#include "FavTree.h"
#include "menuorder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#pragma optimize( "s", on )

//MyIE url extension
//[myie]
//MostFav = 1 if it is the most favorite url

/////////////////////////////////////////////////////////////////////////////
// CFavTreeCtrl

CFavTreeCtrl::CFavTreeCtrl()
{
	m_hStartFolder = NULL;
	pImageList = NULL;
	m_idTimer = 0;
	m_nTimerID =0;
	m_bDragging = FALSE;
	m_hItem = NULL;
	m_timerticks = 0;
	m_hOldDropItem = NULL;
}

CFavTreeCtrl::~CFavTreeCtrl()
{
	try{
	m_astrFav.RemoveAll();
	if(pImageList)
		delete pImageList;
	}catch(...)
	{
	}

}


BEGIN_MESSAGE_MAP(CFavTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CFavTreeCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_COMMAND(ID_RFAV_COPY, OnRfavCopy)
	ON_COMMAND(ID_RFAV_DEL, OnRfavDel)
	ON_COMMAND(ID_RFAV_OPENALL, OnRfavOpenall)
	ON_COMMAND(ID_RFAV_START_FOLDER, OnRfavStartFolder)
	ON_COMMAND(ID_RFAV_START, OnRfavStart)
	ON_COMMAND(ID_RFAV_OPEN_NEW, OnRfavOpenNew)
	ON_COMMAND(ID_RFAV_MOST_FAV, OnRfavMostFav)
	ON_COMMAND(ID_RFAVFOLDER_OPENMOSTFAVORITES, OnRfavfolderOpenmostfavorites)
	ON_COMMAND(ID_RFAV_ADDSHORTCUT, OnRfavAddshortcut)
	ON_COMMAND(ID_RFAV_ALIAS, OnRfavAlias)
	ON_COMMAND(ID_RFAV_MOST_FOLDER, OnRfavMostFolder)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_COMMAND(ID_RFAVFOLDER_FIND, OnRfavfolderFind)
	ON_COMMAND(ID_RFAV_PROPERTY, OnRfavProperty)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND(ID_RFAVFOLDER_RESET, OnRfavfolderReset)
	ON_COMMAND(ID_RFAV_RENAME, OnRfavRename)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(0x6101, 0x61ff, OnAddtoGroup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFavTreeCtrl message handlers


void CFavTreeCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	HTREEITEM hItem;

	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	hItem = HitTest(pt);

	if ((hItem != NULL) && !ItemHasChildren(hItem))
	{
		int img, selimg;
		GetItemImage(hItem, img, selimg);
		if(img == 0 || img == 5)
		{
			ExpandNode(hItem);
			Expand(hItem,TVE_TOGGLE);
		}
		else
		{
			CMyIEView* pvw = GetCurrentView(1, bFavInNewWindow);
			int d = GetItemData(hItem);
			if(pvw!=NULL && d>=0)
			{
				CString url;
				POSITION pos = m_astrFav.FindIndex(d);
				if(pos!=NULL)
					GetURL(m_astrFav.GetAt(pos), url);
				pvw->ToNavigate(url,0,NULL);
			}
		}
	}
	else
		Expand(hItem,TVE_TOGGLE);

	*pResult = 0;
	
}

void CFavTreeCtrl::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	HTREEITEM hItem;

	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	hItem = HitTest(pt);
	m_hItem = hItem;

	if ((hItem != NULL) && GetItemData(hItem)!=-1)
	{
		CMenu menu;
		CMenu* pPopup, *pGroups = NULL;

		// the font popup is stored in a resource
		menu.LoadMenu(IDR_RFAV_URL);
		pPopup = menu.GetSubMenu(0);
		int nImage, nSImage;
		GetItemImage(hItem, nImage, nSImage);
		if(nImage == 4)
		{
			CString menustr;
			menustr.LoadString(IDS_UNSET_MOST_FAV);
			pPopup->ModifyMenu(ID_RFAV_MOST_FAV,MF_BYCOMMAND,ID_RFAV_MOST_FAV, menustr);
		}
		if(m_hStartFolder == NULL)
			pPopup->EnableMenuItem(ID_RFAV_START, MF_GRAYED|MF_DISABLED);

		//load group list
		pGroups = pPopup->GetSubMenu(0);
		((CMainFrame*)pMainFrame)->MenuLoadGroupList(pGroups);

		ClientToScreen(&pt);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, pt.x, pt.y + 1, this);

	}
	else if ((hItem != NULL))
	{
		CMenu menu;
		CMenu* pPopup;

		// the font popup is stored in a resource
		menu.LoadMenu(IDR_RFAV_FOLDER);
		pPopup = menu.GetSubMenu(0);
		if(hItem == m_hStartFolder)
		{
			CString menustr;
			menustr.LoadString(IDS_UNSET_START_FOLDER);
			pPopup->ModifyMenu(ID_RFAV_START_FOLDER,MF_BYCOMMAND,ID_RFAV_START_FOLDER, menustr);
		}
		int nImage, nSImage;
		GetItemImage(hItem, nImage, nSImage);
		if(nImage == 5)
		{
			CString menustr;
			menustr.LoadString(IDS_UNSET_MOST_FOLDER);
			pPopup->ModifyMenu(ID_RFAV_MOST_FOLDER,MF_BYCOMMAND,ID_RFAV_MOST_FOLDER, menustr);
		}
		ClientToScreen(&pt);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, pt.x, pt.y + 1, this);

	}
	
	*pResult = 0;
}


void CFavTreeCtrl::OnRfavCopy() 
{
	// TODO: Add your command handler code here
	HTREEITEM hItem;

	hItem = m_hItem;

	if (hItem != NULL )
	{
		int npos = GetItemData(hItem);
		POSITION pos = m_astrFav.FindIndex(npos);
		CString url;
		if(pos!=NULL)
			url = m_astrFav.GetAt(pos);
		int l = url.GetLength();

		if ( !OpenClipboard() )
		  {
			AfxMessageBox( "Cannot open the Clipboard" );
			return;
		  }
		  // Remove the current Clipboard contents
		  if( !EmptyClipboard() )
		  {
			AfxMessageBox( "Cannot empty the Clipboard" );
			return;
		  }

		HGLOBAL hData = GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE, l+1);
		CopyMemory(GlobalLock(hData), (LPCSTR)url, l+1);
		GlobalUnlock(hData);

		  // For the appropriate data formats...
		  if ( ::SetClipboardData( CF_TEXT, hData ) == NULL )
		  {
			AfxMessageBox( "Unable to set Clipboard data" );
			CloseClipboard();
			GlobalFree(hData);
			return;
		  }
		  // ...
		  CloseClipboard();
		  GlobalFree(hData);
	}
}

void CFavTreeCtrl::OnRfavDel() 
{
	// TODO: Add your command handler code here
	CString strName;
	strName = GetItemText(m_hItem);
	CString path,szPath;
	if(!((CMainFrame*)pMainFrame)->GetFavoriteFolder(szPath))
		return ;

	GetItemPath(m_hItem, path);
/*	HTREEITEM hItem = GetParentItem(m_hItem);
	HTREEITEM hParent = GetParentItem(hItem);
	HTREEITEM hRoot = GetRootItem();
	while(hParent !=NULL)
	{
		path = GetItemText(hItem)+"\\"+path;
		hItem = hParent;
		hParent = GetParentItem(hParent);
	}*/
	path = "\\"+path;
	path = szPath+ path;

	if(DeleteFile(path+strName+".url"))
	{
		DeleteItem(m_hItem);
	}
	else if(DeleteFile(path+strName+".lnk"))
	{
		DeleteItem(m_hItem);
	}
	
	//update menu
	((CMainFrame*)pMainFrame)->m_bFavDirty = TRUE;

}

void CFavTreeCtrl::OnRfavOpenall() 
{
	// TODO: Add your command handler code here
	ExpandNode(m_hItem);
	HTREEITEM hit;
	hit = GetChildItem(m_hItem);
	int nWins = 0;

	while(hit!=NULL && (nWins<nMaxWins || nMaxWins==0))
	{
		if(GetItemData(hit)!=-1)
		{
			if(nWins!=0)
			{
				CString url;
				int d = GetItemData(hit);
				POSITION pos = m_astrFav.FindIndex(d);
				if(pos!=NULL)
					GetURL(m_astrFav.GetAt(pos), url);
				((CMainFrame*)pMainFrame)->m_astrOpenAll.AddTail(url);
				nWins++;
			}
			else
			{
				CMyIEView* pvw = CreateNewView();
				int d = GetItemData(hit);
				if(pvw!=NULL)
				{
					CString url;
					POSITION pos = m_astrFav.FindIndex(d);
					if(pos!=NULL)
						GetURL(m_astrFav.GetAt(pos), url);
					pvw->ToNavigate(url,0,NULL);
					nWins ++;
				}
			}
		}
		hit = GetNextSiblingItem(hit);
	}
	if(nWins >1 && ((CMainFrame*)pMainFrame)->m_nInterval)
		((CMainFrame*)pMainFrame)->m_nTimerID = ::SetTimer(((CMainFrame*)pMainFrame)->GetSafeHwnd(), 1000, ((CMainFrame*)pMainFrame)->m_nInterval*1000,NULL);
	else if(nWins>1)
		((CMainFrame*)pMainFrame)->m_nTimerID = ::SetTimer(((CMainFrame*)pMainFrame)->GetSafeHwnd(), 1000, 20,NULL);
	//enable toolbar button
	((CMainFrame*)pMainFrame)->m_wndToolBar.GetToolBarCtrl().EnableButton(ID_FILE_STOP_OPENALL);
}


void CFavTreeCtrl::OnRfavStartFolder() 
{
	// TODO: Add your command handler code here
	try{

	HTREEITEM hItem, hParent, hRoot;
	hItem = m_hItem;

	if (hItem != NULL )
	{
		//get the path
		CString           szPath;

		// find out from the registry where the favorites are located.
		if(!((CMainFrame*)pMainFrame)->GetFavoriteFolder(szPath))
			return ;


		CString path;

		hParent = GetParentItem(hItem);
		hRoot = GetRootItem();
		while(hParent !=NULL)
		{
			path = GetItemText(hItem)+"\\"+path;
			hItem = hParent;
			hParent = GetParentItem(hParent);
		}
		if(m_hItem != GetRootItem())
		{
			path = "\\"+path;
			path = szPath+ path;
		}
		else
			path = szPath;

		CString oldpath = ((CMainFrame*)pMainFrame)->m_strStartFavFolder;
		if(oldpath == path)
		{
			//clean the startup folder
			((CMainFrame*)pMainFrame)->m_strStartFavFolder = "";
			SetItemState(m_hStartFolder,0 , TVIS_BOLD);
			m_hStartFolder = NULL;
			return;
		}

		//update icon
		SetItemState(m_hItem,TVIS_BOLD,TVIS_BOLD);
		SetItemState(m_hStartFolder,0 , TVIS_BOLD);

		//save the path
		((CMainFrame*)pMainFrame)->m_strStartFavFolder = path;
		m_hStartFolder = m_hItem;
	}
	}catch(...)
	{
	}
}

void CFavTreeCtrl::OnRfavStart() 
{
	// TODO: Add your command handler code here
	try{

	if(GetParentItem(m_hItem)==m_hStartFolder || m_hStartFolder == NULL)
		return;

	CString strName;
	strName = GetItemText(m_hItem);
	int id = GetItemData(m_hItem);
	CString path,szPath;
	if(!((CMainFrame*)pMainFrame)->GetFavoriteFolder(szPath))
		return ;

	GetItemPath(m_hItem, path);
	HTREEITEM hItem;
	/*
	HTREEITEM hItem = GetParentItem(m_hItem);
	HTREEITEM hParent = GetParentItem(hItem);
	HTREEITEM hRoot = GetRootItem();
	while(hParent !=NULL)
	{
		path = GetItemText(hItem)+"\\"+path;
		hItem = hParent;
		hParent = GetParentItem(hParent);
	}*/
	path = "\\"+path;
	path = szPath+ path;

	if(!CopyFile(path+strName+".url", ((CMainFrame*)pMainFrame)->m_strStartFavFolder+strName+".url",TRUE))
	{
		if(CopyFile(path+strName+".lnk", ((CMainFrame*)pMainFrame)->m_strStartFavFolder+strName+".lnk",TRUE))
		{
			if(ItemHasChildren(m_hStartFolder))
			{
				hItem = InsertItem(strName, 1,1,m_hStartFolder, TVI_SORT);
				SetItemData(hItem, id);
			}
		}
	}
	else
	{
		if(ItemHasChildren(m_hStartFolder))
		{
			hItem = InsertItem(strName, 1,1,m_hStartFolder, TVI_SORT);
			SetItemData(hItem, id);
		}
	}

	//update menu
	((CMainFrame*)pMainFrame)->m_bFavDirty = TRUE;
	}catch(...)
	{
	}
}

void CFavTreeCtrl::OnRfavOpenNew() 
{
	// TODO: Add your command handler code here
	CMyIEView* pvw = CreateNewView();
	int d = GetItemData(m_hItem);
	if(pvw!=NULL && d>=0)
	{
		CString url;
		POSITION pos = m_astrFav.FindIndex(d);
		if(pos!=NULL)
			GetURL(m_astrFav.GetAt(pos), url);
		pvw->ToNavigate(url,0,NULL);
	}
	
}

void CFavTreeCtrl::OnRfavMostFav() 
{
	// TODO: Add your command handler code here
	CString strName;
	strName = GetItemText(m_hItem);
	CString path,szPath;
	if(!((CMainFrame*)pMainFrame)->GetFavoriteFolder(szPath))
		return ;

	GetItemPath(m_hItem, path);
	/*
	HTREEITEM hItem = GetParentItem(m_hItem);
	HTREEITEM hParent = GetParentItem(hItem);
	HTREEITEM hRoot = GetRootItem();
	while(hParent !=NULL)
	{
		path = GetItemText(hItem)+"\\"+path;
		hItem = hParent;
		hParent = GetParentItem(hParent);
	}*/
	path = "\\"+path;
	path = szPath+ path;

	OFSTRUCT ofs;
	if(OpenFile(path+strName+".url", &ofs, OF_EXIST)!=HFILE_ERROR)
	{
		int nImage, nSImage;
		GetItemImage(m_hItem, nImage, nSImage);
		if(nImage == 1)
		{
			SetItemImage(m_hItem, 4,4);
			WritePrivateProfileString("MyIE", "MostFav", "1", path+strName+".url");
		}
		else if (nImage == 4)
		{
			SetItemImage(m_hItem, 1,1);
			WritePrivateProfileString("MyIE", "MostFav", "0", path+strName+".url");
		}
		//update menu
		((CMainFrame*)pMainFrame)->m_bFavDirty = TRUE;
	}
	else 
	{
		AfxMessageBox(IDS_NOT_URL);
	}
	
	
}

void CFavTreeCtrl::OnRfavfolderOpenmostfavorites() 
{
	// TODO: Add your command handler code here
	ExpandNode(m_hItem);
	HTREEITEM hit;
	hit = GetChildItem(m_hItem);
	int nImage, nSImage;
	int nWins =0;

	while(hit!=NULL && (nWins<nMaxWins || nMaxWins==0))
	{
		if(GetItemData(hit)!=-1)
		{
			GetItemImage(hit, nImage, nSImage);
			if(nImage == 4)
			{
				if(nWins!=0)
				{
					CString url;
					int d = GetItemData(hit);
					POSITION pos = m_astrFav.FindIndex(d);
					if(pos!=NULL)
						GetURL(m_astrFav.GetAt(pos), url);
					((CMainFrame*)pMainFrame)->m_astrOpenAll.AddTail(url);
					nWins ++;
				}
				else
				{
					CMyIEView* pvw = CreateNewView();
					int d = GetItemData(hit);
					if(pvw!=NULL)
					{
						CString url;
						POSITION pos = m_astrFav.FindIndex(d);
						if(pos!=NULL)
							GetURL(m_astrFav.GetAt(pos), url);
						pvw->ToNavigate(url,0,NULL);
						nWins ++;
					}
				}
			}
		}
		hit = GetNextSiblingItem(hit);
	}
	if(nWins >1 && ((CMainFrame*)pMainFrame)->m_nInterval)
		((CMainFrame*)pMainFrame)->m_nTimerID = ::SetTimer(((CMainFrame*)pMainFrame)->GetSafeHwnd(), 1000, ((CMainFrame*)pMainFrame)->m_nInterval*1000,NULL);
	else if(nWins>1)
		((CMainFrame*)pMainFrame)->m_nTimerID = ::SetTimer(((CMainFrame*)pMainFrame)->GetSafeHwnd(), 1000, 20,NULL);
	//enable toolbar button
	((CMainFrame*)pMainFrame)->m_wndToolBar.GetToolBarCtrl().EnableButton(ID_FILE_STOP_OPENALL);
}

BOOL CFavTreeCtrl::GetURL(CString &path, CString &url)
{
	url = path;
	if(url.Right(4) == _T(".lnk"))
	{
		TCHAR buf[2048];
		if(((CMainFrame*)pMainFrame)->ResolveLink(url, buf))
		{
			url = buf;
			path = url;
		}
		else
			url = "about:blank";
	}
	return TRUE;
}

void CFavTreeCtrl::OnRfavAddshortcut() 
{
	// TODO: Add your command handler code here
	int d = GetItemData(m_hItem);
	if(d>=0)
	{
		CString url;
		POSITION pos = m_astrFav.FindIndex(d);
		if(pos!=NULL)
			GetURL(m_astrFav.GetAt(pos), url);
		CSetSCDlg dlg;
		dlg.m_strEdit = url;
		dlg.DoModal();
	}	
}

void CFavTreeCtrl::OnRfavAlias() 
{
	// TODO: Add your command handler code here
	int d = GetItemData(m_hItem);
	if(d>=0)
	{
		CString url, alias;
		POSITION pos = m_astrFav.FindIndex(d);
		if(pos!=NULL)
			GetURL(m_astrFav.GetAt(pos), url);
		CInputAlias dlg;
		dlg.m_url = url;
		dlg.m_alias = GetItemText(m_hItem);
		if(dlg.DoModal()==IDOK)
		{
			alias = dlg.m_alias; url = dlg.m_url;
			alias.TrimLeft();
			url.TrimLeft();
			if(alias !="" && !alias.IsEmpty() && url!="" && !url.IsEmpty())
				((CMainFrame*)pMainFrame)->m_mapAlias.SetAt(alias, url);
		}
	}	
}

int CFavTreeCtrl::ExpandNode(HTREEITEM hItem)
{
	if ((hItem != NULL) && !ItemHasChildren(hItem))
	{
		int img, selimg;
		GetItemImage(hItem, img, selimg);
		if(img == 0 || img == 5)
		{
			int nEndPos = m_astrFav.GetCount();
			
			CString strName;
			strName = GetItemText(hItem);
			CString path,szPath;
			if(!((CMainFrame*)pMainFrame)->GetFavoriteFolder(szPath))
				return 0;

			GetItemPath(hItem, path);
			/*
			HTREEITEM hItemp = GetParentItem(hItem);
			HTREEITEM hParent = GetParentItem(hItemp);
			HTREEITEM hRoot = GetRootItem();
			while(hParent !=NULL)
			{
				path = GetItemText(hItemp)+"\\"+path;
				hItemp = hParent;
				hParent = GetParentItem(hParent);
			}*/
			path = "\\"+path;
			path = szPath+ path + strName;

			return ((CFavTree*)m_pFavTree)->BuildFavorites(path, nEndPos, hItem);
		}
	}
	return 0;
}

void CFavTreeCtrl::OnRfavMostFolder() 
{
	// TODO: Add your command handler code here
	CString strName;
	strName = "myiefolder.mic";
	CString path,szPath;
	if(!((CMainFrame*)pMainFrame)->GetFavoriteFolder(szPath))
		return ;

	GetItemPath(m_hItem, path);
	/*
	HTREEITEM hItem = GetParentItem(m_hItem);
	HTREEITEM hParent = GetParentItem(hItem);
	HTREEITEM hRoot = GetRootItem();
	while(hParent !=NULL)
	{
		path = GetItemText(hItem)+"\\"+path;
		hItem = hParent;
		hParent = GetParentItem(hParent);
	}*/
	if(m_hItem != GetRootItem())
	{
		path = "\\"+path;
		path = szPath+ path + GetItemText(m_hItem) + "\\";
	}
	else
		path = szPath + "\\";

//	if(CreateFile(path+strName, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL)!=NULL)
	{
		int nImage, nSImage;
		GetItemImage(m_hItem, nImage, nSImage);
		if(nImage == 0)
		{
			SetItemImage(m_hItem, 5,6);
			WritePrivateProfileString("MyIE", "MostFav", "1", path+strName);
		}
		else if (nImage == 5)
		{
			SetItemImage(m_hItem, 0,2);
			WritePrivateProfileString("MyIE", "MostFav", "0", path+strName);
		}
		SetFileAttributes(path+strName, FILE_ATTRIBUTE_HIDDEN);

		//update menu
		((CMainFrame*)pMainFrame)->m_bFavDirty = TRUE;
	}
}

void CFavTreeCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
/*	HTREEITEM hItem;

	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	hItem = HitTest(pt);
	m_hItem = hItem;

	if (hItem != NULL )
	{
		int img, selimg;
		GetItemImage(hItem, img, selimg);
		if(img == 0 || img == 5)
		{
			if(!ItemHasChildren(hItem))
				ExpandNode(hItem);
			OnRfavOpenall();
		}

	}
*/
	*pResult = 0;
}

void CFavTreeCtrl::OnRfavfolderFind() 
{
	// TODO: Add your command handler code here
	CString path,szPath;
	if(!((CMainFrame*)pMainFrame)->GetFavoriteFolder(szPath))
		return ;

	GetItemPath(m_hItem, path);
	/*
	HTREEITEM hItem = GetParentItem(m_hItem);
	HTREEITEM hParent = GetParentItem(hItem);
	HTREEITEM hRoot = GetRootItem();
	while(hParent !=NULL)
	{
		path = GetItemText(hItem)+"\\"+path;
		hItem = hParent;
		hParent = GetParentItem(hParent);
	}*/
	if(m_hItem != GetRootItem())
	{
		path = "\\"+path;
		path = szPath+ path + GetItemText(m_hItem) + "\\";
	}
	else
		path = szPath + "\\";

	ShellExecute(NULL, "find", path, NULL, NULL, 0);

}

void CFavTreeCtrl::OnRfavProperty() 
{
	// TODO: Add your command handler code here
	CString strName;
	strName = GetItemText(m_hItem);
	CString path,szPath;
	if(!((CMainFrame*)pMainFrame)->GetFavoriteFolder(szPath))
		return ;

	GetItemPath(m_hItem, path);
	/*
	HTREEITEM hItem = GetParentItem(m_hItem);
	HTREEITEM hParent = GetParentItem(hItem);
	HTREEITEM hRoot = GetRootItem();
	while(hParent !=NULL)
	{
		path = GetItemText(hItem)+"\\"+path;
		hItem = hParent;
		hParent = GetParentItem(hParent);
	}*/
	path = "\\"+path;
	path = szPath+ path;

	try{

	SHELLEXECUTEINFO sei;
	sei.cbSize = sizeof(sei);
	sei.fMask = SEE_MASK_INVOKEIDLIST|SEE_MASK_FLAG_NO_UI ;
	sei.lpIDList = NULL;
	sei.hwnd = NULL;
	sei.lpFile = path+strName+".url";
	sei.lpVerb = "properties";
	sei.nShow = SW_SHOWDEFAULT;
	sei.lpDirectory = path;
	if(!ShellExecuteEx(&sei))
		ShellExecute(NULL, "properties", path+strName+".lnk", NULL, NULL, SW_SHOWDEFAULT );
	else
	{
		int d = GetItemData(m_hItem);
		if(d>=0)
		{
			char buf[2048];
			::GetPrivateProfileString(_T("InternetShortcut"), _T("URL"),
								  _T(""), buf, 2048,
								  path+strName+".url");
			CString url;
			POSITION pos = m_astrFav.FindIndex(d);
			if(pos!=NULL)
				url = m_astrFav.GetAt(pos);

			if(url==buf)
				return;
			else
			{
				if(pos!=NULL)
					m_astrFav.SetAt(pos, buf);
				//update menu
				((CMainFrame*)pMainFrame)->m_bFavDirty = TRUE;

			}
		}
	}

	}catch(...)
	{
	}
}

void CFavTreeCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if(pImageList)
	{
		delete pImageList;
		pImageList = NULL;
	}
	m_hDragItem = pNMTreeView->itemNew.hItem;
	if(m_hDragItem == NULL)
		return;

/* 	int img, selimg;
	GetItemImage(m_hDragItem, img, selimg);
	if(img == 0 || img == 5)
	{
		//not drag folders
		*pResult = 0;
		return;
	}
*/
	pImageList = CreateDragImage(m_hDragItem);

	if(pImageList!=NULL)
	{
		VERIFY (pImageList->BeginDrag (0, CPoint (8, 8)));
		VERIFY (pImageList->DragEnter (GetDesktopWindow (), pNMTreeView->ptDrag));
		m_bDragging = TRUE;
		m_hDropItem = NULL;
		m_hOldDropItem = NULL;
		m_bMoveIntoFolder = FALSE;
		m_HalfItemHeight = GetItemHeight()/2;
		SetCapture(); 
	//	SetCursor(LoadCursor(NULL, IDC_ARROW));

		m_nTimerID = SetTimer(1, 75, NULL); //auto scroll time
    }

	*pResult = 0;
}

void CFavTreeCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	try{

	if(m_bDragging)
	{
		if(pImageList==NULL)
		{
			m_bDragging = FALSE;
			return;
		}

		CPoint pt (point);
		ClientToScreen (&pt);

		// move the drag image
		VERIFY (pImageList->DragMove (pt));

		// unlock window updates
		VERIFY (pImageList->DragShowNolock (FALSE));

		ScreenToClient (&pt);
		UINT uFlags;
		// get the item that is below cursor
		m_hDropItem = HitTest (pt, &uFlags);

		if( m_hOldDropItem == NULL )
				m_hOldDropItem = GetDropHilightItem();

		// highlight it
		if(m_hDropItem != GetRootItem())
			pt.y-=m_HalfItemHeight;
		if(HitTest(pt, &uFlags) == m_hDropItem)
		{
			SelectDropTarget (m_hDropItem);
			m_bMoveIntoFolder = TRUE;
		}
		else
		{ 
			m_bMoveIntoFolder = FALSE;
			HTREEITEM hdi = m_hDropItem;
			m_hDropItem = GetPrevSiblingItem(m_hDropItem);
			if(m_hDropItem == NULL)
			{
				m_hDropItem = GetParentItem(hdi);
				m_bMoveIntoFolder = TRUE;
			}
			SelectDropTarget(NULL);
		}

		if( m_idTimer && m_hDropItem == m_hOldDropItem )
			{
				KillTimer( m_idTimer );
				m_idTimer = 0;
			}
			
		if( !m_idTimer )
			m_idTimer = SetTimer( 1000, 2000, NULL );


		VERIFY (pImageList->DragShowNolock (TRUE));

	}
	else
	{
		//display url in status bar
		UINT uFlags;
		// get the item that is below cursor
		HTREEITEM hItem = HitTest (point, &uFlags);

		if(hItem != m_hOldDropItem)
		{
			CString url; 
			int img=-1, selimg=-1;
			GetItemImage(hItem, img, selimg);
			if(img != 0 && img != 5)
			{
				int d = GetItemData(hItem);
				if(d>=0)
				{
					POSITION pos = m_astrFav.FindIndex(d);
					if(pos!=NULL)
						url = m_astrFav.GetAt(pos);
				}
				else
					url = "";
			}
			pMainFrame->SetMessageText(url);
			m_hOldDropItem = hItem;
		}


	}

	}catch(...)
	{
	}

	CTreeCtrl::OnMouseMove(nFlags, point);
}

void CFavTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bDragging)
	{
		KillTimer( m_nTimerID );//auto scroll
		KillTimer( m_idTimer); //auto expand
		m_nTimerID =0;
		m_idTimer = 0;

		VERIFY (pImageList->DragLeave (GetDesktopWindow ()));	
		pImageList->EndDrag ();
		// stop intercepting all mouse messages
		VERIFY (ReleaseCapture ());
 
		delete pImageList;
		pImageList = NULL;
		m_bDragging = FALSE;

		if(m_hDropItem==NULL)
			return;

		int img, selimg;
		GetItemImage(m_hDragItem, img, selimg);
		BOOL bMoveFolder=FALSE;
		if(img==0|| img == 5)
			bMoveFolder=TRUE;

		GetItemImage(m_hDropItem, img, selimg);
		BOOL bTargetFolder=FALSE;
		if((img==0|| img == 5) && m_bMoveIntoFolder)
			bTargetFolder=TRUE;


		//get source and taarget
		CString strName;
		strName = GetItemText(m_hDragItem);
		CString path,szPath;
		if(!((CMainFrame*)pMainFrame)->GetFavoriteFolder(szPath))
			return ;

		//update menu
		((CMainFrame*)pMainFrame)->m_bFavDirty = TRUE;

		GetItemPath(m_hDragItem, path);
		path = "\\"+path;
		path = szPath+ path;
		CString source;
		if(!bMoveFolder)
		{
			source = path+strName+".url";
			OFSTRUCT ofs;
			if(OpenFile(source, &ofs, OF_EXIST)==HFILE_ERROR)
			{
				source = path+strName+".lnk";
				strName += ".lnk";
			}
			else
				strName += ".url";
		}
		else
			source = path + strName;

		//get target
		CString strDir = GetItemText(m_hDropItem);

		path ="";
		GetItemPath(m_hDropItem, path);
		HTREEITEM hRoot = GetRootItem();

		path = "\\"+path;
		path = szPath+ path;
		CString target;
		if(m_hDropItem == hRoot)
			target = szPath+"\\"+strName;
		else if(!bTargetFolder)
			target = path+strName;
		else if(!m_bMoveIntoFolder)
			target = path + strName;
		else
			target = path+strDir+"\\"+strName;

		HTREEITEM hnode, hParent;
		if(!bMoveFolder)
		{
			hParent = GetParentItem(m_hDropItem);
			if(source!=target)
			{
				//copy or move here
				//get source path
				if(CopyFile(source, target, TRUE))
				{
					if(ItemHasChildren(m_hDropItem) && m_bMoveIntoFolder)//is folder
					{
						GetItemImage(m_hDragItem, img, selimg);
						hnode = InsertItem(strName.Left(strName.GetLength()-4), img, selimg, m_hDropItem, TVI_LAST);
						SetItemData(hnode, GetItemData(m_hDragItem));
					}
					else if(!bTargetFolder || !m_bMoveIntoFolder)
					{
						GetItemImage(m_hDragItem, img, selimg);
						hnode = InsertItem(strName.Left(strName.GetLength()-4), img, selimg, hParent, m_hDropItem);
						SetItemData(hnode, GetItemData(m_hDragItem));
					}


					short ks=GetKeyState(VK_CONTROL);
					if(ks>=0)
					{
						//delete original
						if(DeleteFile(source))
							DeleteItem(m_hDragItem);
					}
					//update menu order
					source = source.Left(source.GetLength()-strName.GetLength());
					hParent = GetParentItem(m_hDragItem);
					BuildMenuOrder(source, hParent);
					target = target.Left(target.GetLength()-strName.GetLength());
					if(bTargetFolder)
						hParent = m_hDropItem;
					else
						hParent = GetParentItem(m_hDropItem);
					BuildMenuOrder(target, hParent);
				
				}
				else
					//not update menu
					((CMainFrame*)pMainFrame)->m_bFavDirty = FALSE;


			}
			else
			{
				//only sort the order
				GetItemImage(m_hDragItem, img, selimg);
				if(!bTargetFolder)
				{
					hnode = InsertItem(strName.Left(strName.GetLength()-4), img, selimg, hParent, m_hDropItem);
				}
				else
					hnode = InsertItem(strName.Left(strName.GetLength()-4), img, selimg, m_hDropItem, TVI_FIRST);
				SetItemData(hnode, GetItemData(m_hDragItem));
				DeleteItem(m_hDragItem);

				//update menu order
				source = source.Left(source.GetLength()-strName.GetLength());
				if(bTargetFolder)
					hParent = m_hDropItem;
				else
					hParent = GetParentItem(m_hDropItem);
				BuildMenuOrder(source, hParent);

			}
		}
		else
		{
			hParent = GetParentItem(m_hDropItem);
			if(source!=target)
			{
				//copy or move here
				//get source path
				if(MoveFile(source, target))
				{
					if(ItemHasChildren(m_hDropItem))//is folder
					{
						GetItemImage(m_hDragItem, img, selimg);
						hnode = InsertItem(strName, img, selimg, m_hDropItem, TVI_LAST);
						SetItemData(hnode, GetItemData(m_hDragItem));
					}
					else if(!bTargetFolder)
					{
						GetItemImage(m_hDragItem, img, selimg);
						hnode = InsertItem(strName, img, selimg, hParent, m_hDropItem);
						SetItemData(hnode, GetItemData(m_hDragItem));
					}
					DeleteItem(m_hDragItem);

					if(source+"\\" == ((CMainFrame*)pMainFrame)->m_strStartFavFolder)
					{
						m_hStartFolder = hnode;
						SetItemState(hnode,TVIS_BOLD,TVIS_BOLD);
					}

					//update menu order
					source = source.Left(source.GetLength()-strName.GetLength());
					hParent = GetParentItem(m_hDragItem);
					BuildMenuOrder(source, hParent);
					target = target.Left(target.GetLength()-strName.GetLength());
					if(bTargetFolder && m_bMoveIntoFolder)
						hParent = m_hDropItem;
					else
						hParent = GetParentItem(m_hDropItem);
					BuildMenuOrder(target, hParent);

					//move sub order
					CString str;
					if(((CMainFrame*)pMainFrame)->GetFavoriteFolder(str))
					{
						source = source.Mid(str.GetLength()+1);
						target = target.Mid(str.GetLength()+1);
	
						MoveSubMenuOrder(source+strName, target+strName);
					}
				}
				else
					//not update menu
					((CMainFrame*)pMainFrame)->m_bFavDirty = FALSE;

			}
			else
			{
				//only sort the order
				GetItemImage(m_hDragItem, img, selimg);
				if(!bTargetFolder)
				{
					hnode = InsertItem(strName, img, selimg, hParent, m_hDropItem);
				}
				else
					hnode = InsertItem(strName, img, selimg, m_hDropItem, TVI_FIRST);
				SetItemData(hnode, GetItemData(m_hDragItem));
				DeleteItem(m_hDragItem);
			
				if(source+"\\" == ((CMainFrame*)pMainFrame)->m_strStartFavFolder)
				{
					m_hStartFolder = hnode;
					SetItemState(hnode,TVIS_BOLD,TVIS_BOLD);
				}
			
				//update menu order
				source = source.Left(source.GetLength()-strName.GetLength());
				if(bTargetFolder && m_bMoveIntoFolder)
					hParent = m_hDropItem;
				else
					hParent = GetParentItem(m_hDropItem);
				BuildMenuOrder(source, hParent);
			}
		}

	}
	CTreeCtrl::OnLButtonUp(nFlags, point);
}


#define SCROLL_BORDER  10
#define SCROLL_SPEED_ZONE_WIDTH 20

void CFavTreeCtrl::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	if( nIDEvent != m_nTimerID )
	{
		if( nIDEvent == m_idTimer )
        {
                HTREEITEM htiFloat = GetDropHilightItem();
                if( htiFloat && htiFloat == m_hDropItem )
                {
                        int img, selimg;
						GetItemImage(htiFloat, img, selimg);
						if(img == 0 || img == 5)
						{
							ExpandNode(htiFloat);
							Expand( htiFloat, TVE_EXPAND );
						}
				}
		}


		CTreeCtrl::OnTimer(nIDEvent);
		return;
	}

	HTREEITEM hitem;

	// Doesn't matter that we didn't initialize m_timerticks
	m_timerticks++;

	POINT pt;
	GetCursorPos( &pt );
	CRect rect;
	GetClientRect( &rect );
	ClientToScreen( &rect );

	// NOTE: Screen coordinate is being used because the call
	// to DragEnter had used the Desktop window.
	CImageList::DragMove(pt);

  	int iMaxV = GetScrollLimit(SB_VERT);
 	int iPosV = GetScrollPos(SB_VERT);
	// The cursor must not only be SOMEWHERE above/beneath the tree control
	// BUT RIGHT above or beneath it 
	// i.e. the x-coordinates must be those of the control (+/- SCROLL_BORDER)
	if ( pt.x < rect.left - SCROLL_BORDER ) 
	  ; // Too much to the left
	else if ( pt.x > rect.right + SCROLL_BORDER ) 
	  ; // Too much to the right
	else if( (pt.y < rect.top + SCROLL_BORDER) && iPosV )
	{
		hitem = GetFirstVisibleItem();
		// We need to scroll up
		// Scroll slowly if cursor near the treeview control
		int slowscroll = 6 - (rect.top + SCROLL_BORDER - pt.y) / SCROLL_SPEED_ZONE_WIDTH;
		if( 0 == ( m_timerticks % (slowscroll > 0? slowscroll : 1) ) )
		{
			CImageList::DragShowNolock(FALSE);
			SendMessage( WM_VSCROLL, SB_LINEUP);
			SelectDropTarget(hitem);
			m_hDropItem = hitem;
			CImageList::DragShowNolock(TRUE);
		}
	}
	else if( (pt.y > rect.bottom - SCROLL_BORDER) && (iPosV!=iMaxV) )
	{
		// We need to scroll down
		// Scroll slowly if cursor near the treeview control
		int slowscroll = 6 - (pt.y - rect.bottom + SCROLL_BORDER ) / SCROLL_SPEED_ZONE_WIDTH;
		if( 0 == ( m_timerticks % (slowscroll > 0? slowscroll : 1) ) )
		{
			CImageList::DragShowNolock(FALSE);
			SendMessage( WM_VSCROLL, SB_LINEDOWN);
			int nCount = GetVisibleCount();
			hitem = GetFirstVisibleItem();
			for ( int i=0; i<nCount-1; ++i )
				hitem = GetNextVisibleItem(hitem);
			if( hitem )
				SelectDropTarget(hitem);
			m_hDropItem = hitem;
			CImageList::DragShowNolock(TRUE);
		}
	}

	// The cursor must be in a small zone IN the treecontrol at the left/right
	int iPosH = GetScrollPos(SB_HORZ);
	int iMaxH = GetScrollLimit(SB_HORZ);

	if ( !rect.PtInRect(pt) ) return; // not in TreeCtrl
	else if ( (pt.x < rect.left + SCROLL_BORDER) && (iPosH != 0) )
	{
		// We need to scroll to the left
		CImageList::DragShowNolock(FALSE);
		SendMessage(WM_HSCROLL, SB_LINELEFT);
		CImageList::DragShowNolock(TRUE);
	}
	else if ( (pt.x > rect.right - SCROLL_BORDER) && (iPosH != iMaxH) )
	{
		// We need to scroll to the right
		CImageList::DragShowNolock(FALSE);
		SendMessage(WM_HSCROLL, SB_LINERIGHT);
		CImageList::DragShowNolock(TRUE);
	}
}

void CFavTreeCtrl::OnDestroy() 
{
	try{
	CTreeCtrl::OnDestroy();
	
	// TODO: Add your message handler code here
	CImageList* pImage = GetImageList( TVSIL_NORMAL );
	delete pImage;

	if( m_idTimer )
	{
		KillTimer( m_idTimer );
		m_idTimer = 0;
	}
	}catch(...)
	{
	}
	
}

void CFavTreeCtrl::GetItemPath(HTREEITEM hti, CString &path)
{
	HTREEITEM hItem = GetParentItem(hti);
	HTREEITEM hParent = GetParentItem(hItem);
	HTREEITEM hRoot = GetRootItem();
	while(hParent !=NULL)
	{
		path = GetItemText(hItem)+"\\"+path;
		hItem = hParent;
		hParent = GetParentItem(hParent);
	}
}

void CFavTreeCtrl::BuildMenuOrder(CString &path, HTREEITEM hParent)
{
	//read objects in a folder
	CString         strPath;
	CString         str,str2;
	WIN32_FIND_DATA wfd;
	HANDLE          h;
	CStringArray    astrFavorites;
	CStringArray    astrDirs;
	CStringArray	astrTree;
	CArray<int, int> anType;
	int num = 0;
	int			slen;


	try{

	//load order in the tree 
	if(ItemHasChildren(hParent))
	{
		int img, selimg;
		HTREEITEM hchild = GetChildItem(hParent);
		while(hchild!=NULL)
		{
			astrTree.Add(GetItemText(hchild));
			GetItemImage(hchild, img, selimg);
			if(img==0|| img == 5)
				anType.Add(1); //folder;
			else
				anType.Add(2);

			hchild = GetNextSiblingItem(hchild);
		}
	}
	else
		return;

	//construct cmenuorder
	BOOL found;
	num = astrTree.GetSize();
	CMenuOrder mo;
	try
	{
	mo.mois = new CMenuOrderItem[num];
	}
	catch(...)
	{
		mo.mois = NULL;
//		if(e!=NULL)e->Delete();
	}

	if(mo.mois==NULL)
		return;
	mo.len = num;

	// make sure there's a trailing backslash
	strPath = path;
	if(strPath[strPath.GetLength() - 1] != _T('\\'))
		strPath += _T('\\');
	strPath += "*.*";

//	CString strcha = "ÆµµÀ";
//	CString strcha2; strcha2.LoadString(IDS_CHANNEL_E);
	// now scan the directory, first for .URL files and then for subdirectories
	// that may also contain .URL files
	int i=0;
	h = FindFirstFile(strPath, &wfd);
	if(h != INVALID_HANDLE_VALUE)
	{
		do
		{
			if((wfd.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_SYSTEM))==0)
			{
				str = wfd.cFileName;
				str.MakeLower();
				if(str.Right(4) == _T(".url") || str.Right(4) == _T(".lnk"))
				{
					str=str.Left(str.GetLength()-4);
					found = FALSE;
					for(int j=0; j<astrTree.GetSize() && !found; j++)
					{
						if(anType[j]==2 && str.CompareNoCase(astrTree[j])==0)
						{
							found = TRUE;
							if(i<num)
								mo.mois[i].order = j;
						}
					}	
					if(found && i<num)
					{
						mo.mois[i].itemtype = 0x32;
						FileTimeToDosDateTime(&wfd.ftLastWriteTime, &mo.mois[i].filedate, &mo.mois[i].filetime);
						mo.mois[i].filesize = wfd.nFileSizeLow;
						mo.mois[i].filetype = 0x20;
						slen = strlen(wfd.cFileName);
						try
						{
						mo.mois[i].longname=new char[slen+2];
						}
						catch(...)
						{
							mo.mois[i].longname=NULL;
//							if(e!=NULL)e->Delete();
						}

						if(mo.mois[i].longname)
							strcpy(mo.mois[i].longname, wfd.cFileName);
						try
						{
							mo.mois[i].shortname=new char[14];
						}
						catch(...)
						{
							mo.mois[i].shortname = NULL;
//							if(e!=NULL)e->Delete();
						}

						if(mo.mois[i].shortname)
							strcpy(mo.mois[i].shortname, wfd.cAlternateFileName);
						i++;
					}
				}
			}
			else if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// ignore the current and parent directory entries
				if(lstrcmp(wfd.cFileName, _T(".")) == 0 || lstrcmp(wfd.cFileName, _T("..")) == 0)
					continue;
	
				str = wfd.cFileName;
				found = FALSE;
				for(int j=0; j<astrTree.GetSize() && !found; j++)
				{
					if(anType[j]==1 && str.CompareNoCase(astrTree[j])==0)
					{
						found = TRUE;
						if(i<num)
							mo.mois[i].order = j;
					}
				}	
				if(found && i<num)
				{
					mo.mois[i].itemtype = 0x31;
					FileTimeToDosDateTime(&wfd.ftLastWriteTime, &mo.mois[i].filedate, &mo.mois[i].filetime);
					mo.mois[i].filesize = wfd.nFileSizeLow;
					mo.mois[i].filetype = 0x10;
					try
					{
					mo.mois[i].longname=new char[strlen(wfd.cFileName)+2];
					}
					catch(...)
					{
						mo.mois[i].longname=NULL;
//						if(e!=NULL)e->Delete();
					}
					if(mo.mois[i].longname)
						strcpy(mo.mois[i].longname, wfd.cFileName);
					try
					{
					mo.mois[i].shortname=new char[14];
					}
					catch(...)
					{
						mo.mois[i].shortname=NULL;
//						if(e!=NULL)e->Delete();
					}

					if(mo.mois[i].shortname)
						strcpy(mo.mois[i].shortname, wfd.cAlternateFileName);
					i++;
				}
			}
		} while(FindNextFile(h, &wfd));
		FindClose(h);
	}
	else
		return;

	//save it
	mo.len = i;
	if(((CMainFrame*)pMainFrame)->GetFavoriteFolder(str))
	{
		strPath = path;
		strPath = strPath.Mid(str.GetLength()+1);
		mo.WriteMenuOrder(strPath);
	}

	}catch(...)
	{
	}
}

void CFavTreeCtrl::MoveSubMenuOrder(LPCSTR source, LPCSTR target)
{
	HKEY hSrcKey;
	HKEY hTarKey;
	CString lpSubKey = MENU_ORDER_KEY, lpSubKey2;
	lpSubKey += source;

	HRESULT hr = RegOpenKey(HKEY_CURRENT_USER, lpSubKey, &hSrcKey);
	if(hr == ERROR_SUCCESS)
	{
		BYTE* lpBuf = NULL;
		DWORD size = 0;
		DWORD type = 0;
		hr = RegQueryValueEx(hSrcKey, "Order", NULL, &type, NULL, &size);
		if(size>0)
		{
			try
			{
			lpBuf = new BYTE[size];
			}
			catch(...)
			{
				RegCloseKey(hSrcKey);
//				if(e!=NULL)e->Delete();
				return;
			}

			hr = RegQueryValueEx(hSrcKey, "Order", NULL, &type, lpBuf, &size);

			//copy it
			lpSubKey = MENU_ORDER_KEY;
			lpSubKey += target;
			hr = RegCreateKeyEx(HKEY_CURRENT_USER, lpSubKey, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hTarKey, NULL);
			
			if(hr == ERROR_SUCCESS)
			{
				RegSetValueEx(hTarKey, "Order", 0, REG_BINARY, lpBuf, size);
			}
			RegCloseKey(hTarKey);
			//release
			delete[] lpBuf;
		}

		//sub keys
		int i =0;
		char Name[MAX_PATH+1];
		hr = RegEnumKey(hSrcKey, i, Name, MAX_PATH);
		while(hr == ERROR_SUCCESS)
		{
			lpSubKey = source; lpSubKey += "\\"; lpSubKey += Name;
			lpSubKey2 = target; lpSubKey2 += "\\"; lpSubKey2+= Name;
			MoveSubMenuOrder(lpSubKey, lpSubKey2);
			i++;
			hr = RegEnumKey(hSrcKey, i, Name, MAX_PATH);
		}
		RegCloseKey(hSrcKey);

		//delete key
		lpSubKey = MENU_ORDER_KEY;
		lpSubKey += source;
		RegDeleteKey(HKEY_CURRENT_USER, lpSubKey);
	}

}


void CFavTreeCtrl::OnAddtoGroup(UINT nID)
{
	int d = GetItemData(m_hItem);
	if(d>=0)
	{
		CString url, name;
		POSITION pos = m_astrFav.FindIndex(d);
		if(pos!=NULL)
			GetURL(m_astrFav.GetAt(pos), url);
		name = GetItemText(m_hItem);
		((CMainFrame*)pMainFrame)->AddtoGroup(name, url, nID); 
	}	

}

void CFavTreeCtrl::OnRfavfolderReset() 
{
	// TODO: Add your command handler code here
	CString strName;
	strName = GetItemText(m_hItem);
	CString lpSubKey;
	GetItemPath(m_hItem, lpSubKey);
	lpSubKey = MENU_ORDER_KEY + lpSubKey;
	if(m_hItem != GetRootItem())
		lpSubKey += strName;

	HKEY hKey;
	HRESULT hr = RegOpenKey(HKEY_CURRENT_USER, lpSubKey, &hKey);
	if(hr == ERROR_SUCCESS)
	{
		RegDeleteValue(hKey, "Order");
		RegCloseKey(hKey);
	}

	//update tree
	if(m_hItem == GetRootItem())
	{
		((CFavTree*)m_pFavTree)->Update(1);
	}
	else if(ItemHasChildren(m_hItem))
	{
		Expand(m_hItem, TVE_COLLAPSE);
		HTREEITEM hItem, hItem2;
		hItem = GetChildItem(m_hItem);
		while(hItem)
		{
			hItem2 = hItem;
			hItem = GetNextSiblingItem(hItem);
			DeleteItem(hItem2);
		}
	}

	//update menu
	((CMainFrame*)pMainFrame)->m_bFavDirty = TRUE;

}

void CFavTreeCtrl::OnRfavRename() 
{
	// TODO: Add your command handler code here
	EditLabel(m_hItem);
}

void CFavTreeCtrl::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString strName;
	strName = GetItemText(pTVDispInfo->item.hItem);
	CString path,szPath;
	if(!((CMainFrame*)pMainFrame)->GetFavoriteFolder(szPath))
		return ;

	GetItemPath(m_hItem, path);

	path = "\\"+path;
	path = szPath+ path;
	
	CString str;
	GetEditControl()->GetWindowText(str);
	str.MakeLower();
	str.TrimLeft();
	str.TrimRight();
	SetItemText(pTVDispInfo->item.hItem, str);
	


	if(!MoveFile(path+strName+".url", path+str+".url"))
		MoveFile(path+strName+".lnk", path+str+".lnk");

	//update menu
	((CMainFrame*)pMainFrame)->m_bFavDirty = TRUE;

	*pResult = 0;
}


//#pragma optimize( "s", off)
