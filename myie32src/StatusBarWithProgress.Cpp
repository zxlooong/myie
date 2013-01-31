//---------------------------------------------------------------------------
// Copyright (C) 1997, Interscope Ltd. All rights reserved.
// This software is protected by copyright law. Unauthorised reproduction
// or distribution of this program, or any portion of it, may result in
// severe civil or criminal penalties. For more information, contact:
//
// Interscope Ltd., 5 Culturii St., 5th floor, 4800 Baia Mare, Romania
//    Phone/Fax: +40(62)415023
//    E-mail: office@interscope.ro
//
//   $Author: Levente Farkas $
//     $Date: 11/07/97 11:53p $
//  $Modtime: 11/07/97 11:53p $
// $Revision: 2 $
//  $Archive: /Interscope/Thebe/SetupWizard/StatusBarWithProgress.Cpp $
// $Workfile: StatusBarWithProgress.Cpp $
//---------------------------------------------------------------------------

#include "StdAfx.H"
#include "Resource.h"
#include "StatusBarWithProgress.H"


//--- Debugee ---------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#pragma optimize( "s", on )

//--- Miscellaneous ---------------------------------------------------------

#define ID_STATUS_PROGRESS  17234


//--- Message map 4 classs CStatusBarWithProgress ---------------------------

IMPLEMENT_DYNCREATE(CStatusBarWithProgress,CStatusBar)

BEGIN_MESSAGE_MAP(CStatusBarWithProgress,CStatusBar)
	//{{AFX_MSG_MAP(CStatusBarWithProgress)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------------------------------
// Pre     : 
// Post    : 
// Globals : 
// I/O     : 
// Task    : Construct a status bar with a progress control
//---------------------------------------------------------------------------
CStatusBarWithProgress::CStatusBarWithProgress(int nProgressBarWidth):
                        m_nProgressWidth(nProgressBarWidth),
                        m_bProgressVisible(FALSE)
{
}

//---------------------------------------------------------------------------
// Pre     : 
// Post    : 
// Globals : 
// I/O     : 
// Task    : When creating the status bar, also create the progress control,
//           but do not show it yet
//---------------------------------------------------------------------------
BOOL CStatusBarWithProgress::Create(CWnd *pParentWnd, DWORD dwStyle, UINT nID)
{
    // Default creation
    BOOL bCreatedOK =CStatusBar::Create(pParentWnd,dwStyle,nID);
    if(bCreatedOK)
    {
		GetStatusBarCtrl().SetMinHeight(20);

        // Also create the progress bar
        m_Progress.Create(WS_CHILD | WS_EX_STATICEDGE | PBS_SMOOTH,CRect(0,0,m_nProgressWidth,10),this,ID_STATUS_PROGRESS);

		//========create a mini toolbar in status bar
		CImageList img;
		//CBitmap    bitmap;

		if (!m_Toolbar.CreateEx(this))
		{
			TRACE0("Failed to create toolbar\n");
			return -1;      // fail to create
		}

		img.Create(10, 10, ILC_COLORDDB|ILC_MASK, 5, 1);
		HBITMAP hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP13));
		ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
		m_Toolbar.GetToolBarCtrl().SetImageList(&img);
		img.Detach();

		//bitmap.LoadBitmap(IDB_BITMAP13);
/*		img.Create(10, 10, ILC_COLORDDB|ILC_MASK, 3, 1);
		hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP13));
		ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
		m_Toolbar.SendMessage(TB_SETHOTIMAGELIST, 0, (LPARAM)img.m_hImageList);
		img.Detach();*/
		//bitmap.Detach();

		m_Toolbar.SetButtons(NULL, 4);
		m_Toolbar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT |TBSTYLE_TOOLTIPS );
		m_Toolbar.ModifyStyleEx(0, WS_EX_TOOLWINDOW);
		m_Toolbar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);

		m_Toolbar.SetButtonInfo(0, ID_FILE_WORKOFFLINE, TBSTYLE_BUTTON, 0);
		m_Toolbar.SetButtonInfo(1, ID_TOOLS_AUTOFILTER_CUR, TBSTYLE_BUTTON, 1);
		m_Toolbar.SetButtonInfo(2, ID_OPTIONS_ACTIVENEWWINDOW, TBSTYLE_BUTTON, 2);
		m_Toolbar.SetButtonInfo(3, ID_OPTIONS_OPENLINKINNEWWINDOW, TBSTYLE_BUTTON, 3);
		m_Toolbar.SetButtonInfo(4, ID_TOOLS_COLLECT, TBSTYLE_BUTTON, 4);

		m_Toolbar.SetBarStyle(m_Toolbar.GetBarStyle() |
			 CBRS_FLYBY | CBRS_SIZE_FIXED);

		RECT rect;
		m_Toolbar.GetToolBarCtrl().GetItemRect(m_Toolbar.GetToolBarCtrl().GetButtonCount()-1,&rect);
		m_nToolbarWidth = rect.right;

		//register drag&drop
		if (!ms_dropTarget.Register(&m_Toolbar)) {
			TRACE("Fail in registing drop target\n");
		}

    }

    return bCreatedOK;
}

//---------------------------------------------------------------------------
// Pre     : 
// Post    : Return old visible status
// Globals : 
// I/O     : 
// Task    : Show/hide the progress bar
//---------------------------------------------------------------------------
BOOL CStatusBarWithProgress::ShowProgressBar(BOOL bShow)
{
    // Save old visible status
    BOOL bOldVisible =m_bProgressVisible;

	try{

    if((bOldVisible != bShow) && ::IsWindow(m_Progress.m_hWnd))
    {
        // Show/hide
        m_Progress.ShowWindow(bShow ? SW_SHOWNA : SW_HIDE);
        m_bProgressVisible =bShow;

        // If just shown, make sure it's in the right position
        if(bShow)
        {
			SetPaneInfo(1, ID_INDICATOR_PROCESS , SBPS_NORMAL, m_nProgressWidth-6);
            AdjustProgressBarPosition();
            RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_UPDATENOW);
        }
		else
		{
			SetPaneInfo(1, ID_INDICATOR_PROCESS , SBPS_NORMAL, 4);
            RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_UPDATENOW);
		}
    }
	}catch(...)
	{
	}

    return bOldVisible;
}

//---------------------------------------------------------------------------
// Pre     : 
// Post    : 
// Globals : 
// I/O     : 
// Task    : Adjust the progress bar's position, so that it comes after the text 
//           in the first pane
//---------------------------------------------------------------------------
void CStatusBarWithProgress::AdjustProgressBarPosition()
{
	try{

    // Make sure the progress bar is created
    if(!::IsWindow(m_Progress.m_hWnd))
        return;

    // Find out the size of the text in first pane
   // CString str_pane_text;
    CRect   progress_rect;
	//Cover the left column
    GetItemRect(1,progress_rect);
    //GetPaneText(0,str_pane_text);


    // Measure the size of the text in the first pane
    //CClientDC temp(this);
    //CFont *pOldFont =temp.SelectObject(GetFont());

    //progress_rect.left  =temp.GetTextExtent(str_pane_text).cx + 10;
    //progress_rect.right =progress_rect.left + m_nProgressWidth;
    progress_rect.left = progress_rect.right - m_nProgressWidth;
	progress_rect.InflateRect(0,0);

    //temp.SelectObject(pOldFont);

    // Now adjust the size of the progrss control
    m_Progress.SetWindowPos(NULL,progress_rect.left,
                                 progress_rect.top,
                                 progress_rect.Width(),
                                 progress_rect.Height(),SWP_NOZORDER | SWP_NOACTIVATE);
	}
	catch(...)
	{
	}
}

//---------------------------------------------------------------------------
// Pre     : 
// Post    : 
// Globals : 
// I/O     : 
// Task    : After each sizing, if the progress bar is visible, adjust 
//           its position
//---------------------------------------------------------------------------
void CStatusBarWithProgress::OnSize(UINT nType, int cx, int cy) 
{
    // Default sizing
	CStatusBar::OnSize(nType, cx, cy);
	
    if(m_bProgressVisible)
        AdjustProgressBarPosition();

	AdjustToolbarPosition();
}


void CStatusBarWithProgress::AdjustToolbarPosition()
{
	try{

    // Make sure the progress bar is created
    if(!::IsWindow(m_Toolbar.m_hWnd))
        return;

    // Find out the size of the text in first pane
   // CString str_pane_text;
    CRect   tool_rect;
	//Cover the left column
    GetItemRect(2,tool_rect);
    //GetPaneText(0,str_pane_text);


    // Measure the size of the text in the first pane
    //CClientDC temp(this);
    //CFont *pOldFont =temp.SelectObject(GetFont());

    //progress_rect.left  =temp.GetTextExtent(str_pane_text).cx + 10;
    //progress_rect.right =progress_rect.left + m_nProgressWidth;
//    tool_rect.left = tool_rect.right - m_nToolbarWidth;
	tool_rect.InflateRect(0,0);

    //temp.SelectObject(pOldFont);

    // Now adjust the size of the progrss control
    m_Toolbar.SetWindowPos(NULL,tool_rect.left+1,
                                 tool_rect.top+1,
                                 m_nToolbarWidth,
                                 tool_rect.Height()-2,SWP_NOZORDER | SWP_NOACTIVATE);
	}
	catch(...)
	{
	}
}


void CStatusBarWithProgress::SetWindowText(LPCSTR Str)
{
	m_Progress.SetWindowText(Str);
}


//#pragma optimize( "s", off )

void CStatusBarWithProgress::IdleUpdateUI()
{
	m_Toolbar.PostMessage(WM_IDLEUPDATECMDUI, TRUE, 0);

}
