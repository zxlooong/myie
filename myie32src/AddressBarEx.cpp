// AddressBarEx.cpp : implementation file
//

#include "stdafx.h"
#include "myie.h"
#include "AddressBarEx.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#pragma optimize( "s", on)

/////////////////////////////////////////////////////////////////////////////
// CAddressBarEx

CAddressBarEx::CAddressBarEx()
{
	HKEY            hKey;
	TCHAR           sz[MAX_PATH];
	DWORD           dwSize = MAX_PATH;

	ShowGoButton = TRUE;
	if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\Main"), &hKey) != ERROR_SUCCESS)
	{
		TRACE0("Main setting not found\n");
	}
	else
	{
		dwSize = sizeof(sz);
		RegQueryValueEx(hKey, _T("ShowGoButton"), NULL, NULL, (LPBYTE)sz, &dwSize);
		if(strcmp(sz, "no")==0)
			ShowGoButton = FALSE;

		RegCloseKey(hKey);
	}
}

CAddressBarEx::~CAddressBarEx()
{
}


BEGIN_MESSAGE_MAP(CAddressBarEx, CToolBar)
	//{{AFX_MSG_MAP(CAddressBarEx)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddressBarEx message handlers

void CAddressBarEx::OnSize(UINT nType, int cx, int cy) 
{
	CToolBar::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
//===

	CRect rect2;
	GetItemRect(1, rect2);
	int w;
	if(ShowGoButton)
		w= cx-rect2.Width()-2;
	else
		w = cx;
		
	SetButtonInfo(0, 0, TBBS_SEPARATOR, w);
	m_wndAddress.SetWindowPos(NULL,  0, 0, w, cy, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOCOPYBITS);
	if(!m_wndAddress.IsWindowVisible())
		m_wndAddress.ShowWindow(SW_SHOW);
//====


}


CComboBoxEx* CAddressBarEx::GetAddressBox()
{
	return &m_wndAddress;
}

void CAddressBarEx::Init() 
{
	// TODO: Add your specialized creation code here
	// create a combo box for the address bar
	if (!m_wndAddress.Create(CBS_DROPDOWN | CBS_AUTOHSCROLL | WS_CHILD, CRect(0, 0, 200, 280), this, AFX_IDW_TOOLBAR + 1))
	{
		TRACE0("Failed to create combobox\n");
		return ;      // fail to create
	}
	m_wndAddress.SetExtendedStyle(0, m_wndAddress.GetExtendedStyle()|CBES_EX_NOSIZELIMIT);

//	CBitmap bitmap;
	CImageList img;
//	bitmap.LoadBitmap(IDB_BITMAP1);
	HBITMAP hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP1));
	img.Create(16, 16, ILC_COLORDDB|ILC_MASK, 7, 1);
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
	m_wndAddress.SetImageList(&img);
	img.Detach();
//	bitmap.Detach();



	if (!ms_dropTarget.Register(&m_wndAddress)) {
		TRACE("Fail in registing drop target\n");
	}

	return;
}




/*************************************************************
CAddDropTarget
**************************************************************/
/*BOOL CAddDropTarget::Register(CComboBoxEx* pComboBox)
{
    ms_pCtl=pComboBox;
    return COleDropTarget::Register(pComboBox);
}*/

DROPEFFECT CAddDropTarget::OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point)
{

 //      if (dwKeyState & MK_CONTROL)
            return DROPEFFECT_SCROLL | DROPEFFECT_COPY | DROPEFFECT_LINK;
 //       else
 //           return DROPEFFECT_SCROLL | DROPEFFECT_MOVE;
}

DROPEFFECT CAddDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, 
                                      DWORD dwKeyState, CPoint point)
{
//    if (!pDataObject->IsDataAvailable(CF_TEXT)) 
 //       return DROPEFFECT_NONE;

    DROPEFFECT dwEffect;
	dwEffect=DROPEFFECT_COPY;
    
//    if (dwKeyState & MK_CONTROL)
 //       dwEffect=DROPEFFECT_COPY;
 //   else
 //       dwEffect=DROPEFFECT_MOVE;

    //we set focus to current window such that the caret will be shown
 //   ms_pCtl->SetFocus();

    return dwEffect;
}

void CAddDropTarget::OnDragLeave(CWnd* pWnd)
{
}

DROPEFFECT CAddDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, 
                                     DWORD dwKeyState, CPoint point)
{

    DROPEFFECT dwEffect;
    
	dwEffect=DROPEFFECT_COPY;

//    if (dwKeyState & MK_CONTROL)
  //      dwEffect=DROPEFFECT_COPY;
 //   else
 //       dwEffect=DROPEFFECT_MOVE;

 //   ms_pCtl->DrawCaretByCursor();
    return dwEffect;
}

BOOL CAddDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
                           DROPEFFECT dropEffect, CPoint point)
{

	try{

    HGLOBAL hData=pDataObject->GetGlobalData(CF_TEXT);
	LPCSTR lpcszData;
	CString str2, url;
    if (hData)
	{
		lpcszData=(LPCSTR)GlobalLock(hData);
	//	ms_pCtl->SetWindowText(lpcszData);
	//	SendMessage(pMainFrame->GetSafeHwnd(), WM_COMMAND, IDOK, 0);
		str2 = lpcszData;
    }
	else
	{
		hData=pDataObject->GetGlobalData(CF_HDROP);
		if (!hData) {
			TRACE("Fail in getting data\n");
			return FALSE;
		}

		HDROP hdrop =(HDROP)GlobalLock(hData);

		DragQueryFile(hdrop, 0, lpfiles, 1024);
		str2 = lpfiles;
	}


    GlobalUnlock(hData);

	//if the protocol is javascript then ignore
	if(str2.Left(11)=="javascript:")
		return FALSE;

	//check alias and 3721
	BOOL use3721 = FALSE;
	if(((CMainFrame*)pMainFrame)->m_bUseAlias)
		if(((CMainFrame*)pMainFrame)->m_mapAlias.Lookup(str2, url))
			str2 = url;
		else
			use3721=TRUE;
	else
		use3721=TRUE;


	if(((CMainFrame*)pMainFrame)->m_bUse3721 && use3721)
	{
		//use 3721
		if(str2.Find(':')<0 && str2.Find('.')<0)
		{
			//not a url
			url = str2;
			str2 = "http://cns.3721.com/cns.dll?platform=pc&fl=1&fd=1&name="+str2; // www.3721.com/***
		}
	}

	CMyIEView* pvw = CreateNewView();
	if(pvw!=NULL)
	{
		pvw->ToNavigate(str2,0,NULL);
	}

	}catch(...)
	{
	}

    return TRUE;
}

//#pragma optimize( "s", off)

void CAddressBarEx::GetSelText(int index, CString & str)
{
	if(index<0)
		return;

	try{

	static TCHAR           sz[MAX_PATH];
	COMBOBOXEXITEM item;
	item.mask = CBEIF_TEXT;
	item.pszText = (LPTSTR)sz;
	item.cchTextMax = MAX_PATH;

	item.iItem = index;
	m_wndAddress.GetItem(&item);
	str = sz;
	}catch(...)
	{
	}
}

int CAddressBarEx::FindStringExact(int start, CString & str)
{
	//try to add static
	static TCHAR           sz[MAX_PATH];

	try{
	COMBOBOXEXITEM item;
	item.mask = CBEIF_TEXT;
	item.pszText = (LPTSTR)sz;
	item.cchTextMax = MAX_PATH;

	int nc = m_wndAddress.SendMessage(CB_GETCOUNT, 0, 0);
	for(register int i=start+1;i<nc;i++)
	{
		item.iItem = i;
		m_wndAddress.GetItem(&item);
		if(str.CompareNoCase(sz)==0)
			return i;
	}
	}catch(...)
	{
	}

	return CB_ERR;
}
