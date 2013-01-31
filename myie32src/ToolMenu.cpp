#include "stdafx.h"
#include "ToolMenu.h"
#include "afxpriv.h"
#include "MenuData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL bCompactFavBar;

/////////////////////////////////////////////////////////////////////////////
// CToolMenu
/*
	Toooooo lazy to calculate or to obtain from the system
*/
#define		MENU_SPACETEXT			10
#define		MENU_TOPFACTORS			2
#define		MENU_LEFTFACTORS		4
#define		MENU_RIGHTFACTORS		4
#define		MENU_BOTTOMFACTORS		2
#define		MENU_MAXWIDTH			100



CToolMenu::CToolMenu()
{
}

CToolMenu::~CToolMenu()
{
	try{
	DestroyMenuData(m_hMenu);
	}catch(...)
	{
	}

}

/////////////////////////////////////////////////////////////////////////////
/*
	This is the drawing routine for the owner drawn menu
	The toolbar pointer have to be valid ! 
*/

void CToolMenu::DrawItem( LPDRAWITEMSTRUCT lpds ) 
{
	if(lpds == NULL)
		return;

	CDC	dc;

	try
	{
	// Get the size of the image
	int cx=0, cy=0, iImage;
	UINT iID;
	CImageList* pImg = NULL;
	CMenuData* pmd = (CMenuData*)(lpds->itemData);
	if (pmd == NULL)
		return;

	pImg = pmd->m_pImgList;
	if(pImg != NULL)
		::ImageList_GetIconSize(*pImg, &cx, &cy);

	// Have to find which image the button is using, image list wants index
	iID = lpds->itemID;

	dc.Attach ( lpds->hDC );

	// Get the drawing rectangle
	CRect	rectDraw = lpds->rcItem;
	
	// Refer DrawIndirect( ) API for the drawstyle, raster op, etc...
	// Only selection/checked/grayed state is handled here
	// Add or change here for all the drawing operations...
	int		iRasterOP = SRCCOPY;
	int		iDrawStyle= ILD_NORMAL;

	COLORREF	clrTextBack;
	COLORREF	clrTextFore;
	COLORREF	clrMaskFore;

	clrTextBack = ::GetSysColor (COLOR_MENU);
	clrTextFore = ::GetSysColor (COLOR_MENUTEXT);
	
	clrMaskFore = CLR_DEFAULT;

	// Get appropriate colors from the system for painting our menu
	if ((lpds->itemState & ODS_SELECTED) &&
		(lpds->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		clrTextBack = ::GetSysColor (COLOR_HIGHLIGHT);
		clrTextFore = ::GetSysColor (COLOR_HIGHLIGHTTEXT);
	}
	dc.FillSolidRect ( rectDraw, clrTextBack );

	if (lpds->itemState & ODS_GRAYED || lpds->itemState & ODS_DISABLED )
	{
/*		clrTextFore = ::GetSysColor (COLOR_GRAYTEXT);*/
		iDrawStyle= ILD_BLEND25;
		clrMaskFore = CLR_NONE;
	}


	CPoint lefttop;
	lefttop.x = rectDraw.left + MENU_LEFTFACTORS;
	lefttop.y = rectDraw.top + MENU_TOPFACTORS ;

	// A  special 3d rect for the checked state
//	if (((CMenuData*)(lpds->itemData))->nFlags & MF_CHECKED )
	if(lpds->itemState & ODS_CHECKED )
	{
		CRect	rect3D;
		rect3D.SetRect ( lefttop.x, lefttop.y, lefttop.x+cx, lefttop.y+cy );
		rect3D.InflateRect (2,2);
		dc.Draw3dRect( &rect3D, ::GetSysColor (COLOR_3DSHADOW), ::GetSysColor (COLOR_3DHILIGHT) );
	}
	// Draw the toolbar button bitmap
	iImage = pmd->m_nBitmap;
	if(pImg!=NULL && iImage>=0)
		pImg->DrawIndirect( &dc, iImage, lefttop, CSize(cx, cy), CPoint(0, 0), 
							iDrawStyle, iRasterOP, clrTextBack, clrMaskFore);

	// Now time to draw the text
	CRect	rectText = rectDraw;
	rectText.DeflateRect ( MENU_SPACETEXT + MENU_LEFTFACTORS + cx, MENU_TOPFACTORS, MENU_RIGHTFACTORS, MENU_BOTTOMFACTORS );

	COLORREF	clrTextForeold = dc.SetTextColor( clrTextFore );
	dc.SetBkMode ( TRANSPARENT );


	// Try to load the string from resource
	// The resource of the toolbar buttons usually have the format
	// Status bar text\ntooltip
	// this extracts the tooltip text and displays
	// Users who dont like this, please change whatever you wanted to display
	CString	strText;
	strText = pmd->m_szMenuText;
	strText.Replace("&", "&&");
	dc.DrawText ( strText, -1, &rectText, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS );

	if (lpds->itemState & ODS_GRAYED || lpds->itemState & ODS_DISABLED )
	{
        RECT offset = rectText;
        offset.left+=1;
        offset.right+=1;
        offset.top+=1;
        offset.bottom+=1;
		UINT nFormat  = DT_LEFT|DT_SINGLELINE|DT_VCENTER;
		UINT nFormatr = DT_RIGHT|DT_SINGLELINE|DT_VCENTER;

 		if (!((lpds->itemState & ODS_SELECTED) &&
		(lpds->itemAction & (ODA_SELECT | ODA_DRAWENTIRE))))
		{
			dc.SetTextColor(GetSysColor(COLOR_BTNHILIGHT));
			dc.DrawText(strText,&offset, nFormat);
		}
       // if(tablocr!=-1) dc.DrawText (rightStr,&offset,nFormatr);
		dc.SetTextColor(GetSysColor(COLOR_GRAYTEXT));
		dc.DrawText(strText,&rectText, nFormat);
	}
	dc.SetTextColor( clrTextForeold );
	}
	catch(...)
	{
	}

	// Detach the dc
	dc.Detach ();
}


#define GAP 1

void CToolMenu::MeasureItem( LPMEASUREITEMSTRUCT lpMIS ) 
{
	if(lpMIS == NULL)
		return;

	try
	{
	// get image size
	int cx=16, cy=0;
	CMenuData* pmd = (CMenuData*)(lpMIS->itemData);
	if(pmd==NULL)
		return;

	if(pmd->m_pImgList != NULL)
		::ImageList_GetIconSize(*(pmd->m_pImgList), &cx, &cy);
	

	// Calculate width and height of the item.
	// To lazy to get the info from system or to calculate.
/*	lpms->itemHeight = cy + MENU_BOTTOMFACTORS + MENU_TOPFACTORS;
	lpms->itemWidth = cx +	MENU_LEFTFACTORS + MENU_RIGHTFACTORS + 
							MENU_MAXWIDTH + MENU_SPACETEXT;
*/
//  UINT state = pmd->nFlags;
/*  if(state & MF_SEPARATOR){
    lpMIS->itemWidth = 0;
    lpMIS->itemHeight = GetSystemMetrics(SM_CYMENU)>>1;
  }
  else{*/
    CFont m_fontMenu;
    LOGFONT m_lf;
    ZeroMemory ((PVOID) &m_lf,sizeof (LOGFONT));
    NONCLIENTMETRICS nm;
    nm.cbSize = sizeof (NONCLIENTMETRICS);
    VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
           nm.cbSize,&nm,0)); 
    m_lf =  nm.lfMenuFont;
    m_fontMenu.CreateFontIndirect (&m_lf);

    // Obtain the width of the text:
    CWnd *pWnd = AfxGetMainWnd();            // Get main window
    CDC *pDC = pWnd->GetDC();              // Get device context
    CFont* pFont=NULL;    // Select menu font in...
    
 //   if (IsNewShell())
    pFont = pDC->SelectObject (&m_fontMenu);// Select menu font in...
        
    //Get pointer to text SK
    const char *lpstrText = pmd->m_szMenuText;//SK: we use const to prevent misuse
    
        
    SIZE size;
	if(lpstrText!=NULL)
		::GetTextExtentPoint32(pDC->m_hDC,lpstrText,
             strlen(lpstrText),&size);
 /*  //it's Win32suckx
      RECT rect;
      rect.left=rect.top=0;
      size.cy=DrawText(pDC->m_hDC,(LPCTSTR)lpstrText,
                       strlen(lpstrText),&rect,
                       DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_CALCRECT);
      //+3 makes at least three pixels space to the menu border
      size.cx=rect.right-rect.left+3;
      size.cx += 3*(size.cx/strlen(lpstrText));
   
*/
    CSize t = CSize(size);
//    if(IsNewShell())
    pDC->SelectObject (pFont);  // Select old font in
    AfxGetMainWnd()->ReleaseDC(pDC);  // Release the DC

    // Set width and height:

    lpMIS->itemWidth = cx + t.cx + cx + GAP;
    //int temp = GetSystemMetrics(SM_CYMENU);
	int temp = t.cy+4;
    lpMIS->itemHeight = temp>cy+4 ? temp : cy+4;
    m_fontMenu.DeleteObject();
	
	}
	catch(...)
	{
	}
}

BOOL DestroyMenuData(HMENU hMenu)
{
	if(hMenu==NULL)
		return FALSE;
	if(!IsMenu(hMenu))
		return FALSE;

	try
	{
	int iCount;
	MENUITEMINFO mii; 
	for(iCount=0; iCount<::GetMenuItemCount(hMenu);iCount++)
	{

		mii.fMask = MIIM_DATA|MIIM_TYPE;    // get the state of the menu item
	    mii.dwTypeData = NULL;
		mii.cch = 0;
		mii.dwItemData = NULL;
		mii.cbSize = sizeof(mii);
		::GetMenuItemInfo(hMenu, iCount, TRUE, &mii); 
		CMenuData* pmd = (CMenuData*)(mii.dwItemData);
		if(mii.fType & MFT_OWNERDRAW && pmd!=NULL)
		{
			if(pmd->m_szMenuText != NULL)
			{
				delete[] pmd->m_szMenuText;
				pmd->m_szMenuText = NULL;
			}
			if(pmd->m_szURL != NULL)
			{
				delete[] pmd->m_szURL;
				pmd->m_szURL = NULL;
			}
			delete pmd;
			mii.dwItemData = NULL;
			mii.fMask = MIIM_DATA;
			mii.cbSize = sizeof(mii);
			::SetMenuItemInfo(hMenu, iCount, TRUE, &mii); 
		}
		HMENU hSubMenu = ::GetSubMenu(hMenu, iCount);
		if(IsMenu(hSubMenu))
		{
			DestroyMenuData(hSubMenu);
		}
	}
	}
	catch(...)
	{
	}

	return true;
}

BOOL CToolMenu::DestroyMenu()
{
	DestroyMenuData(m_hMenu);
	return CMenu::DestroyMenu();
}


