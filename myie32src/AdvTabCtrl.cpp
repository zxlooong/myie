// AdvTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "myie.h"
#include "AdvTabCtrl.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdvTabCtrl

CAdvTabCtrl::CAdvTabCtrl()
{
	m_hDragCursor = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_POINTER_DRAG));
	TabCtrlItem.pszText = pszText;
	TabCtrlItem.cchTextMax = 255;
	m_crSelColour     = GetSysColor(COLOR_BTNTEXT); //RGB(0,0,255);
	m_crUnselColour   = GetSysColor(COLOR_BTNTEXT); //RGB(50,50,50);
	m_crSelBKColour     = GetSysColor(COLOR_BTNFACE); //RGB(0,0,255);
	m_crUnselBKColour   = RGB(180,180,180);
	pImageList = NULL;
}

CAdvTabCtrl::~CAdvTabCtrl()
{
	m_SelFont.DeleteObject();
	m_UnselFont.DeleteObject();
}


BEGIN_MESSAGE_MAP(CAdvTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CAdvTabCtrl)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdvTabCtrl message handlers

void CAdvTabCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if(m_bDragging)
	{
		int id = GetTabIDFromPoint(point);
		if(id<0 || id==m_nDragTabID)
			SetCursor(m_hOldCursor);
		else
			SetCursor(m_hDragCursor);
	}
	
	CTabCtrl::OnMouseMove(nFlags, point);
}

void CAdvTabCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	try{

/*	if(id == m_id && GetCurSel()!=id)
	{
		//change select
		TCITEM TabCtrlItem;

		if(id>=0)
		{
			TabCtrlItem.mask = TCIF_PARAM;
			GetItem(id, &TabCtrlItem);
			if(TabCtrlItem.lParam != NULL)
			{	
				BOOL bMax;
				CChildFrame * tcf;
				tcf = (CChildFrame*)pMainFrame->MDIGetActive(&bMax);
				((CChildFrame*)TabCtrlItem.lParam)->m_bSkipSel = TRUE;
				((CChildFrame*)TabCtrlItem.lParam)->nCurTabID = id;

				if(((CChildFrame*)TabCtrlItem.lParam)->bProtect)
					pMainFrame->PostMessage(WM_ACTIVATE_WINDOW, 0, TabCtrlItem.lParam);
				else
				{
					pMainFrame->MDIActivate((CChildFrame*)TabCtrlItem.lParam);
					SetCurSel(id);
				}
				if(tcf!=NULL && bMax)
				{
					m_bMax = 1;
				}

			}
			
		}
	}
	m_id = -1;*/
	
	if(m_bDragging)
	{
		int id = GetTabIDFromPoint(point);
		m_bDragging = FALSE;
		if(id>=0 && id!=m_nDragTabID)
		{
		
			if(id<m_nDragTabID)
				id--;

			//insert new tab
			TabCtrlItem.mask = TCIF_TEXT|TCIF_PARAM|TCIF_IMAGE|TCIF_STATE;
			InsertItem( id+1, &TabCtrlItem );

			//delete tab
			if(id+1<m_nDragTabID)
				m_nDragTabID ++;
			DeleteItem(m_nDragTabID);

			//select this tab
			if(m_nDragTabID<id+1)
				id--;
			if(m_bSel)
				SetCurSel(id+1);

			AfxGetMainWnd()->PostMessage(WM_UPDATE_TAB_TIP);
		}
	}

	}catch(...)
	{
	}
	
	CTabCtrl::OnLButtonUp(nFlags, point);
}

int CAdvTabCtrl::GetTabIDFromPoint(CPoint &point)
{
	TCHITTESTINFO info;
	info.pt = point;
	info.flags = TCHT_NOWHERE;
	int id = HitTest(&info);
	return id;
}

void CAdvTabCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(pMainFrame->m_nAllowDBC>0)
	{
		ProcessClick(nFlags, point, 0);
	}

	CTabCtrl::OnLButtonDblClk(nFlags, point);
}


void CAdvTabCtrl::OnMButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(pMainFrame->m_nAllowMC>0)
	{
		ProcessClick(nFlags, point, 1);
	}

	CTabCtrl::OnMButtonDown(nFlags, point);
}

void CAdvTabCtrl::ProcessClick(UINT nFlags, CPoint point, int nClickButton)
{
	try{
		TCHITTESTINFO hti;
		hti.pt = point;
		hti.flags = TCHT_ONITEM | TCHT_ONITEMICON | TCHT_ONITEMLABEL ;
		int nTabID = HitTest(&hti);
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;

		GetItem(nTabID, &TabCtrlItem);
		if(TabCtrlItem.lParam != NULL)
		{
			CChildFrame* tcf = ((CChildFrame*)TabCtrlItem.lParam);
			if((nClickButton==0 && pMainFrame->m_nAllowDBC==1) || (nClickButton==1 && pMainFrame->m_nAllowMC==1) || (nClickButton==2 && pMainFrame->m_nAllowRC==1))
			{
				//2.2 313 post instead send
				tcf->PostMessage(WM_CLOSE);
				tcf->m_pView->m_bToClose = TRUE;
				//tcf->MDIDestroy();
			}
			else if((nClickButton==0 && pMainFrame->m_nAllowDBC==2) || (nClickButton==1 && pMainFrame->m_nAllowMC==2) || (nClickButton==2 && pMainFrame->m_nAllowRC==2))
			{
				//save as
				tcf->m_pView->PostMessage(WM_COMMAND, ID_FILE_SAVEAS, 0);
			}
			else if((nClickButton==0 && pMainFrame->m_nAllowDBC==3) || (nClickButton==1 && pMainFrame->m_nAllowMC==3) || (nClickButton==2 && pMainFrame->m_nAllowRC==3))
			{
				//save html
				tcf->m_pView->PostMessage(WM_COMMAND, ID_FILE_SAVEHTML, 0);
			}
			else if((nClickButton==0 && pMainFrame->m_nAllowDBC==4) || (nClickButton==1 && pMainFrame->m_nAllowMC==4) || (nClickButton==2 && pMainFrame->m_nAllowRC==4))
			{
				//save html
				tcf->m_pView->PostMessage(WM_COMMAND, ID_FILE_AUTOSAVE, 0);
			}
			else if((nClickButton==0 && pMainFrame->m_nAllowDBC==5) || (nClickButton==1 && pMainFrame->m_nAllowMC==5) || (nClickButton==2 && pMainFrame->m_nAllowRC==5))
			{
				tcf->bLock =!tcf->bLock;

				if(tcf->m_pView->m_nProgress<0)
				{
					int nID = pMainFrame->FindTab(tcf);
					if(tcf->bLock)
						tcf->m_pView->oldIcon = ((CMainFrame*)pMainFrame)->SetTabIcon( 4, nID );
					else
						tcf->m_pView->oldIcon = ((CMainFrame*)pMainFrame)->SetTabIcon( -1, nID );

					//update tab bar tab0
					if(pMainFrame->m_bFBTab)
						pMainFrame->PostMessage(WM_UPDATE_TAB, FALSE);
				}
			}

		}

	}catch(...)
	{
	}
}


void CAdvTabCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	m_id = GetTabIDFromPoint(point);

	if(!m_bDragging && (nFlags & MK_LBUTTON))
	{
		m_bDragging = TRUE;
		m_hOldCursor = GetCursor();
		
		m_nDragTabID = GetTabIDFromPoint(point);

		TabCtrlItem.mask = TCIF_PARAM|TCIF_IMAGE|TCIF_TEXT|TCIF_STATE;
		GetItem(m_nDragTabID, &TabCtrlItem);
		if(GetCurSel() == m_nDragTabID)
			m_bSel = TRUE;
		else
			m_bSel = FALSE;
	}
	
	CTabCtrl::OnLButtonDown(nFlags, point);
}

void CAdvTabCtrl::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bDragging = FALSE;

	if(pMainFrame->m_nAllowRC>0 && (nFlags & MK_CONTROL)!=0 )
	{
		ProcessClick(nFlags, point, 2);
	}

	CTabCtrl::OnRButtonUp(nFlags, point);
}




void CAdvTabCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
//  if (GetItemCount() == 0) return; // do nothing
	if(pMainFrame->nWin == 0) return;

  // windows should draw the control as usual
  _AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
  pThreadState->m_lastSentMsg.wParam = WPARAM(HDC(dc));
  Default();

/*  if(!pMainFrame->m_bFBTab)
  {
	  if (m_bTop)
	  {
		  // remove board
		  CRect rect;
		  int bottom, right;
		  GetWindowRect(&rect);
		  right = rect.Width();
		  bottom = rect.Height();
  
/*		  HPEN pen = ::CreatePen(PS_SOLID, 0, ::GetSysColor(COLOR_3DFACE));
		  HGDIOBJ old = ::SelectObject(dc, pen);
			dc.MoveTo(0, bottom-1);
			dc.LineTo(right, bottom-1);
			dc.MoveTo(0, bottom-2);
			dc.LineTo(right, bottom-2);
		  ::SelectObject(dc, old);
		  ::DeleteObject(pen);* /
		  	dc.FillSolidRect(CRect(0, bottom, right, bottom-2), m_crSelBKColour);

 	  }
	/*	  else
		  {
			dc.MoveTo(0, 0);
			dc.LineTo(right, 0);
		  }* /
 }*/

  // Do not call CTabCtrl::OnPaint() for painting messages
}

//custimized for myie
void CAdvTabCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{	
	CRect rect = lpDrawItemStruct->rcItem;
	int nTabIndex = lpDrawItemStruct->itemID;
	if (nTabIndex < 0) return;
	BOOL bSelected = (lpDrawItemStruct->itemState&ODS_SELECTED);// (nTabIndex == GetCurSel());

	static char label[256];
	TC_ITEM tci;
	tci.mask = TCIF_TEXT|TCIF_IMAGE;
	tci.pszText = label;     
	tci.cchTextMax = 255;    	
	if (!GetItem(nTabIndex, &tci )) return;

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	if (!pDC) return;
//	int nSavedDC = pDC->SaveDC();

	// For some bizarre reason the rcItem you get extends above the actual
	// drawing area. We have to workaround this "feature".
//	rect.top += ::GetSystemMetrics(SM_CYEDGE);

	//background
	pDC->FillSolidRect(rect, m_crSelBKColour);
/*	pDC->SetBkMode(TRANSPARENT);
	if (bSelected) 
		pDC->FillSolidRect(rect, m_crSelBKColour);
	else
	{
		pDC->FillSolidRect(rect, m_crUnselBKColour);
		if(!m_bTop)
			pDC->FillSolidRect(CRect(0,0,rect.right, 2), m_crUnselBKColour);
	}*/

	rect.left += 6; //pDC->GetTextExtent(_T(" ")).cx;		// Margin

	// Draw image
	if (pImageList && tci.iImage >= 0) {


		// Get height of image so we 
/*		IMAGEINFO info;
		pImageList->GetImageInfo(tci.iImage, &info);
		CRect ImageRect(info.rcImage);*/
		int nYpos = rect.top+4;
		if(bSelected)
			if(!m_bTop)
				nYpos += 3;
			else
				nYpos += 2;

		pImageList->Draw(pDC, tci.iImage, CPoint(rect.left, nYpos), ILD_NORMAL);
		rect.left += 14; //ImageRect.Width();
	}

	//Draw Text
	if (bSelected) {
//		pDC->SetTextColor(m_crSelColour);
		pDC->SelectObject(&m_SelFont);
		//rect.top -= ::GetSystemMetrics(SM_CYEDGE);
		pDC->DrawText(label, rect, DT_SINGLELINE|DT_VCENTER);
	} else {
//		pDC->SetTextColor(m_crUnselColour);
		pDC->SelectObject(&m_UnselFont);
		pDC->DrawText(label, rect, DT_SINGLELINE|DT_VCENTER);
	}

//	pDC->RestoreDC(nSavedDC);
}

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlEx operations

void CAdvTabCtrl::SetColours(COLORREF bSelColour, COLORREF bUnselColour)
{
	m_crSelColour = bSelColour;
	m_crUnselColour = bUnselColour;
	Invalidate();
}

void CAdvTabCtrl::SetBKColours(COLORREF bSelBKColour, COLORREF bUnselBKColour)
{
	m_crSelBKColour = bSelBKColour;
	m_crUnselBKColour = bUnselBKColour;
	Invalidate();
}

void CAdvTabCtrl::SetFonts(CFont* pSelFont, CFont* pUnselFont)
{
	ASSERT(pSelFont && pUnselFont);

	LOGFONT lFont;
	int nSelHeight, nUnselHeight;

	m_SelFont.DeleteObject();
	m_UnselFont.DeleteObject();

	pSelFont->GetLogFont(&lFont);
	m_SelFont.CreateFontIndirect(&lFont);
	nSelHeight = lFont.lfHeight;

	pUnselFont->GetLogFont(&lFont);
	m_UnselFont.CreateFontIndirect(&lFont);
	nUnselHeight = lFont.lfHeight;

	SetFont( (nSelHeight > nUnselHeight)? &m_SelFont : &m_UnselFont);
}


void CAdvTabCtrl::SetFonts(int nSelWeight,   BOOL bSelItalic,   BOOL bSelUnderline,
						  int nUnselWeight, BOOL bUnselItalic, BOOL bUnselUnderline)
{
	// Free any memory currently used by the fonts.
	m_SelFont.DeleteObject();
	m_UnselFont.DeleteObject();

	// Get the current font
	LOGFONT lFont;
	CFont *pFont = GetFont();
	if (pFont)
		pFont->GetLogFont(&lFont);
	else {
		NONCLIENTMETRICS ncm;
		ncm.cbSize = sizeof(NONCLIENTMETRICS);
		VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0));
		lFont = ncm.lfMessageFont; 
	}

	// Create the "Selected" font
	lFont.lfWeight = nSelWeight;
	lFont.lfItalic = bSelItalic;
	lFont.lfUnderline = bSelUnderline;
	m_SelFont.CreateFontIndirect(&lFont);

	// Create the "Unselected" font
	lFont.lfWeight = nUnselWeight;
	lFont.lfItalic = bUnselItalic;
	lFont.lfUnderline = bUnselUnderline;
	m_UnselFont.CreateFontIndirect(&lFont);

	SetFont( (nSelWeight > nUnselWeight)? &m_SelFont : &m_UnselFont);
}