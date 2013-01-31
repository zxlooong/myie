// HisTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE.h"
#include "HisTreeCtrl.h"
#include "HistoryTree.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// CHisTreeCtrl

CHisTreeCtrl::CHisTreeCtrl()
{
}

CHisTreeCtrl::~CHisTreeCtrl()
{
	m_astrHist.RemoveAll();
}


BEGIN_MESSAGE_MAP(CHisTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CHisTreeCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_WM_MOUSEMOVE()
//	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
//	ON_COMMAND(ID_HISFOLDER_DELETE, OnHisfolderDelete)
//	ON_COMMAND(ID_HISURL_DELETE, OnHisurlDelete)
//	ON_COMMAND(ID_HISURL_ADDTOFAVORITES, OnHisurlAddtofavorites)
//	ON_COMMAND(ID_HISURL_PROPERTIES, OnHisurlProperties)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHisTreeCtrl message handlers


void CHisTreeCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	HTREEITEM hItem;
	
	CPoint pt;
	try{

	GetCursorPos(&pt);
	ScreenToClient(&pt);
	hItem = HitTest(pt);

	if ((hItem != NULL) && !ItemHasChildren(hItem))
	{
		int nImg, nOImg;
		GetItemImage(hItem, nImg, nOImg);
		if(nImg==2)
		{
			CMyIEView* pvw = GetCurrentView(1, bHisInNewWindow);
			if(pvw!=NULL)
				pvw->ToNavigate(m_astrHist[GetItemData(hItem)-1],0,NULL);
		}
		else
		{
			//get folder
			LPITEMIDLIST pidlHistory = NULL;

		   HRESULT hr = SHGetSpecialFolderLocation(NULL, CSIDL_HISTORY, &pidlHistory);
		   if (NOERROR != hr)
			  return;

		   IShellFolder* psfDesktop;
		   IShellFolder* psfHistory;

		   hr = SHGetDesktopFolder(&psfDesktop);
		   if (NOERROR != hr)
			  return;

		   hr = psfDesktop->BindToObject(pidlHistory, NULL, IID_IShellFolder,
										 (LPVOID*)&psfHistory);
		   psfDesktop->Release();

		   if (NOERROR != hr)
			  return;

		   HTREEITEM hDayItem;
		   hDayItem = GetParentItem(hItem);
		   if(hDayItem == NULL)
			   hDayItem = hItem;

			CString folder = GetItemText(hDayItem);
			LPITEMIDLIST pidlSubFolder = FindFolder(folder, psfHistory);
			if(pidlSubFolder!=NULL)
			{
				IShellFolder* psfSubFolder = NULL;
				IShellFolder* psfSubFolder2 = NULL;
			    hr = psfHistory->BindToObject(pidlSubFolder, NULL, IID_IShellFolder,
					 (LPVOID*)&psfSubFolder);

				if( hDayItem != hItem)
				{
					if (pidlSubFolder)
						((CHistoryTree*)m_pHisTree)->m_pMalloc->Free(pidlSubFolder);

					folder = GetItemText(hItem);
					pidlSubFolder = FindFolder(folder, psfSubFolder);
					//psfSubFolder->Release();
					hr = psfSubFolder->BindToObject(pidlSubFolder, NULL, IID_IShellFolder,
						 (LPVOID*)&psfSubFolder2);
				}
				if(nImg ==0)
					((CHistoryTree*)m_pHisTree)->AddHistory(psfSubFolder, hItem, 1,3);
				else
					((CHistoryTree*)m_pHisTree)->AddHistory(psfSubFolder2, hItem, 2,2);

				if(psfSubFolder)
					psfSubFolder ->Release();
				if(psfSubFolder2)
					psfSubFolder2 ->Release();
			}

			((CHistoryTree*)m_pHisTree)->m_pMalloc->Free(pidlSubFolder);
		   ((CHistoryTree*)m_pHisTree)->m_pMalloc->Free(pidlHistory);

		   psfHistory->Release();

			Expand(hItem,TVE_TOGGLE);
		}
	}
	else
		Expand(hItem,TVE_TOGGLE);

	}catch(...)
	{
	}

	*pResult = 0;
	
}

LPITEMIDLIST CHisTreeCtrl::FindFolder(LPCSTR foldername, IShellFolder *pFolder)
{
	IEnumIDList* pItems = NULL;
	LPITEMIDLIST pidlNext = NULL;
	STRRET StrRetName;
	LPTSTR lpszName = NULL;

	HRESULT hr = pFolder->EnumObjects(NULL, SHCONTF_FOLDERS|SHCONTF_NONFOLDERS, &pItems);
   while (NOERROR == hr)
   {
	  hr = pItems->Next(1, &pidlNext, NULL);
	  if (NOERROR == hr)
	  {
		 pFolder->GetDisplayNameOf(pidlNext, SHGDN_NORMAL, &StrRetName);

         ((CHistoryTree*)m_pHisTree)->StrRetToStr(StrRetName, &lpszName, pidlNext);


		 if(strcmp(foldername, lpszName)==0)
		 {
	 		 if (lpszName)
				((CHistoryTree*)m_pHisTree)->m_pMalloc->Free(lpszName);

	 		 if (pItems)
				  pItems->Release();
			 return pidlNext;
		 }
		 else if (pidlNext)
			((CHistoryTree*)m_pHisTree)->m_pMalloc->Free(pidlNext);

		 if (lpszName)
			((CHistoryTree*)m_pHisTree)->m_pMalloc->Free(lpszName);

	  }
   }

   if (pItems)
	  pItems->Release();

   return NULL;
}


void CHisTreeCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	HTREEITEM hItem;

	try{

	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	hItem = HitTest(pt);

	if ((hItem != NULL) && !ItemHasChildren(hItem))
	{
		int nImg, nOImg;
		GetItemImage(hItem, nImg, nOImg);
		if(nImg==2)
		{
			pMainFrame->SetMessageText(m_astrHist[GetItemData(hItem)-1]);
		}
	}	
	
	CTreeCtrl::OnMouseMove(nFlags, point);

	}catch(...)
	{
	}
}
/*
void CHisTreeCtrl::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	HTREEITEM hItem;

	try{

	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	m_hItem = hItem = HitTest(pt);

	if (hItem != NULL) 
	{
		int nImg, nOImg;
		GetItemImage(hItem, nImg, nOImg);
		if(nImg==2)
		{
			CMenu menu;
			CMenu* pPopup;

			// the font popup is stored in a resource
			menu.LoadMenu(IDR_HIS_URL);
			pPopup = menu.GetSubMenu(0);
			ClientToScreen(&pt);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, pt.x, pt.y + 1, this);

		}
		else
		{
			CMenu menu;
			CMenu* pPopup;

			// the font popup is stored in a resource
			menu.LoadMenu(IDR_HIS_FOLDER);
			pPopup = menu.GetSubMenu(0);
			ClientToScreen(&pt);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, pt.x, pt.y + 1, this);

		}
	}	
	
	}catch(...)
	{
	}

	*pResult = 0;	
}

void CHisTreeCtrl::OnHisfolderDelete() 
{
	// TODO: Add your command handler code here
	CString strName;
	strName = GetItemText(m_hItem);
	CString path,szPath;
	if(!GetHistoryFolder(szPath))
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
	}* /
	path = "\\"+path;
	path = szPath+ path;

	if(DeleteFile(path+strName))
	{
		DeleteItem(m_hItem);
	}
}

void CHisTreeCtrl::OnHisurlDelete() 
{
	// TODO: Add your command handler code here
	
}

void CHisTreeCtrl::OnHisurlAddtofavorites() 
{
	// TODO: Add your command handler code here
	
}

void CHisTreeCtrl::OnHisurlProperties() 
{
	// TODO: Add your command handler code here
	
}

BOOL CHisTreeCtrl::GetHistoryFolder(CString &strHis)
{
	static TCHAR           szPath[MAX_PATH];

	BOOL r = TRUE;

	try
	{
	   LPITEMIDLIST pidlFavorites = NULL;
	   // Get a pointer to the IMalloc interface
	   IMalloc * m_pMalloc = NULL;
	   HRESULT hr = SHGetMalloc(&m_pMalloc);
	   if (SUCCEEDED(hr))
	   {
		   HRESULT hr = SHGetSpecialFolderLocation(NULL, CSIDL_HISTORY, &pidlFavorites);
		   if (NOERROR != hr)
			  r = FALSE;
		   else
		   {
			   r = SHGetPathFromIDList( pidlFavorites,szPath);
		   }
		   m_pMalloc->Free(pidlFavorites);
		   m_pMalloc->Release();
		   m_pMalloc = NULL;
	   }

	   strHis = szPath;

	}
	catch(...)
	{
	}

	return TRUE;

}

void CHisTreeCtrl::GetItemPath(HTREEITEM hti, CString &path)
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
*/

//#pragma optimize( "s", off)
