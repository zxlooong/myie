// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// =  oooo   oooo  
// = oooooo oooooo  
// = oo  oo oo  oo  
// = oo  oo oo  oo  
// = oooooo oooooo  
// =  oooo   oooo    Copyright ( c ) The Old Ones 1998
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// Start =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// General information section.
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// $Author::                                                               $ User who last changed the file
// $Date::                                                                 $ Date and time of last check in
// $Revision::                                                             $ Visual SourceSafe version number
// $Workfile::                                                             $ Filename
// End =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// Start =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// History section.
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// $History::                                                              $
// End =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- 

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @doc
// @module OOExToolBar.cpp | 
//         This module include an improved tool bar.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Start =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Include file section.
// ------------------------------------------------------------------------
// Precompile header file.
#include "StdAfx.h"

// Base class definition.
#include "OOExToolBar.hpp"

// Math include file.
#include "math.h"
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Start =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Macro and other definition.
// ------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC( COOExToolBar, CToolBar )
BEGIN_MESSAGE_MAP( COOExToolBar, CToolBar )
	//{{AFX_MSG_MAP( COOExToolBar )
	ON_WM_NCCALCSIZE()
	ON_WM_CREATE()
	ON_WM_NCPAINT()
	ON_WM_SYSCOLORCHANGE()
    ON_WM_WINDOWPOSCHANGING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Start =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Initialization and destruction method.
// ------------------------------------------------------------------------
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (IMPLEMENTATION)
//            <c COOExToolBar> 
//            This is the main constructor.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
COOExToolBar::COOExToolBar( void )
{
    // Set the default flag.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    m_nShowIconMode = 1;        // Large icon.
    m_bShowIconText = false;     // Text.

    m_SmallIconSize = CSize( 16, 16 );
    m_LargeIconSize = CSize( 22, 20 );

    m_bOldFloating=false; 
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (IMPLEMENTATION)
//            <c COOExToolBar> 
//            This is the main destructor.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
COOExToolBar::~COOExToolBar( void )
{
    // Free the image list associated memory.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    for ( int i=0; i<NB_POSSIBLE_MODE; i++ )
    {
        if ( m_ImageList[ i ].GetSafeHandle() )
            m_ImageList[ i ].DeleteImageList();
    }
}
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Start =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Mode related method.
// ------------------------------------------------------------------------
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (FUNCTIONAL)
//            <c COOExToolBar> 
//            To view large or small icon.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void COOExToolBar::SetIconMode( 
     UINT _nShowIconMode )  // @Parm 0 Small Icon, 1 Large Icon, 2 Extra Large Icon.
{
    // Store the new value.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    m_nShowIconMode = _nShowIconMode;

    // Load the image list.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    AssignImageList();

    // Resize the toolbar.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    ResizeToolBar();
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (FUNCTIONAL)
//            <c COOExToolBar> 
//            To get the large or small icon mode.
// @RDesc     0 Small Icon, 1 Large Icon, 2 Extra Large Icon.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
UINT COOExToolBar::GetIconMode( void ) const
{
    return m_nShowIconMode;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (FUNCTIONAL)
//            <c COOExToolBar> 
//            To set the text on or off.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void COOExToolBar::SetTextMode( 
     bool _bShowIconText )  // @Parm True to view the text, false to hide it.
{
    // Store the new value.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    m_bShowIconText = _bShowIconText;

    // Resize the toolbar.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    ResizeToolBar();
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (FUNCTIONAL)
//            <c COOExToolBar> 
//            To get the text on or off.
// @RDesc     True if the text is on, False otherwise.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
bool COOExToolBar::GetTextMode( void ) const
{
    return m_bShowIconText;
}
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Start =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Loading information.
// ------------------------------------------------------------------------
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (FUNCTIONAL)
//            <c COOExToolBar> 
//            To load the toolbar information.
// @RDesc     True if the tool bar is loaded.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
BOOL COOExToolBar::LoadToolBar( 
     UINT _ResourceId )     // @Parm The toolbar resource id.
{
    // Convert the resource id into a resource name and load the toolbar
    // using the base class method.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 /*   CString lpszResourceName;
    lpszResourceName.Format( "#%d", _ResourceId );
    BOOL bReturn = CToolBar::LoadToolBar( lpszResourceName );

    // Check if we loaded the toolbar.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    if ( bReturn == FALSE )
    {
        return bReturn;
    }*/

    // Retrieve the height of the toolbar before putting text.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    CToolBarCtrl& bar = GetToolBarCtrl();
    int nIndex = 0;
    CRect NoTextRc( 0, 0, 0, 0 );
    bar.GetItemRect( 0, NoTextRc );
  
	// Set the text for each button.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    TBBUTTON tb;
    for ( nIndex = bar.GetButtonCount(); nIndex >= 0; nIndex-- )
    {
        ZeroMemory(&tb, sizeof(TBBUTTON));
        bar.GetButton(nIndex, &tb);

        // Do we have a separator?
        if ( ( tb.fsStyle & TBSTYLE_SEP ) ==  TBSTYLE_SEP ) {
            continue;
        }

        // Have we got a valid command id?
        if ( tb.idCommand == 0 ) {
            continue;
        }

		// Get the resource string if there is one.
        CString strText((LPCSTR)tb.idCommand);
        LPCTSTR lpszButtonText = NULL;
        CString	strButtonText(_T(&""));
        _TCHAR	seps[] = _T(&"\n");

        if ( !strText.IsEmpty() ) {
            lpszButtonText = _tcstok( ( LPTSTR ) ( LPCTSTR ) strText, seps );
            while( lpszButtonText )
            {
                strButtonText = lpszButtonText;
                lpszButtonText = _tcstok( NULL, seps );
            } 
        }

        if ( !strButtonText.IsEmpty() ) {
            SetButtonText( nIndex, strButtonText );
        }
    }

	// Calculate the effect of the text on the toolbar.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	CRect rc( 0, 0, 0, 0 );
	CSize sizeMax( 0, 0 );

	for ( nIndex = bar.GetButtonCount(); nIndex >= 0; nIndex-- )
	{
		bar.GetItemRect( nIndex, rc );

		rc.NormalizeRect();
		sizeMax.cx = __max( rc.Size().cx, sizeMax.cx );
		sizeMax.cy = __max( rc.Size().cy, sizeMax.cy );
	}

    // Resize the toolbar.
    // The text width is the maximum width of the bitmap.  All toolbar size
    // must at least be this width.
    // The text height is the height added to the button.  Even in large mode
    // we must add this text height.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    m_nTextWidth = sizeMax.cx;
    m_nTextHeight = sizeMax.cy - ( NoTextRc.Size().cy );
    ResizeToolBar();

    // Create the needed image list.
    // Build the image list.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    CClientDC dc( this );
    int nNbBits = dc.GetDeviceCaps( BITSPIXEL );
    for ( int i=0; i<NB_POSSIBLE_MODE; i++ )
    {
        UINT nColorMode = ILC_COLOR;
        if ( nNbBits > 8 )
        {
            nColorMode = ILC_COLORDDB;
        }

        CSize Size = m_LargeIconSize;
        if ( i < 3 )
        {
            Size = m_SmallIconSize;
        }
      
        m_ImageList[ i ].Create( Size.cx, Size.cy, nColorMode | ILC_MASK, bar.GetButtonCount(), 10 );
    }

    return TRUE;
}
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    
// Start =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Image list initialization method.
// ------------------------------------------------------------------------
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (FUNCTIONAL)
//            <c COOExToolBar> 
//            To set the image list.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void COOExToolBar::SetImageList( 
     ImageMode_ _Mode,       // @Parm The image mode.
     CImageList& _rList )    // @Parm The hoover image list.
{
    // Store the list handle for future use.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    m_ImageList[ _Mode ].Attach( _rList.Detach() );
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (FUNCTIONAL)
//            <c COOExToolBar> 
//            To set the current mode appropriate image list.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void COOExToolBar::AssignImageList( void )
{
    // Prepare the list associated with the current mode.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    CImageList* pTempHotList;
    CImageList* pTempNormalList;
    CImageList* pTempDisableList;
    if ( m_nShowIconMode == 0 )
    {
        pTempHotList     = &m_ImageList[ SmallHot ];
        pTempNormalList  = &m_ImageList[ SmallStandard ];
		pTempDisableList = &m_ImageList[ SmallDisable ];
    }
    if ( m_nShowIconMode == 1 )
    {
        pTempHotList     = &m_ImageList[ LargeHot ];
        pTempNormalList  = &m_ImageList[ LargeStandard ];
        pTempDisableList = &m_ImageList[ LargeDisable ];
    }

    // Set the list in the control.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    SetHotImageList( pTempHotList );
    SetStandardImageList( pTempNormalList );
//    SetDisableImageList( pTempDisableList );
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (FUNCTIONAL)
//            <c COOExToolBar> 
//            To set the image list in the control.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void COOExToolBar::InitImageList( void )
{
    // Set the image list according to the current mode.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    AssignImageList();
}
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Start =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Private Image list method.
// ------------------------------------------------------------------------
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (FUNCTIONAL)
//            <c COOExToolBar> 
//            Method to calculate the current size of the button.
// @rdesc     The button size.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
CSize COOExToolBar::CalcButtonSize( void )
{
    // Calcul the width of the drop button.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    CSize theButtonSize;
    if ( m_nShowIconMode == 0 )
    {
        theButtonSize = CSize( m_SmallIconSize.cx + 8, m_SmallIconSize.cy + 7 );
    }
    else if ( m_nShowIconMode == 1 )
    {
        theButtonSize = CSize( m_LargeIconSize.cx + 8, m_LargeIconSize.cy + 7 );
    }

    // Check the text mode and set or hide the text.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    if ( m_bShowIconText )
    {
        // The text width is the maximum width of the bitmap.  All toolbar size
        // must at least be this width.
        if ( theButtonSize.cx < m_nTextWidth ) 
        {
            theButtonSize.cx = m_nTextWidth;
        }

        // The text height is the height added to the button.  Even in large mode
        // we must add this text height.
        theButtonSize.cy += m_nTextHeight;
    }

    return theButtonSize;
}
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Start =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Image list information method.
// ------------------------------------------------------------------------
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (FUNCTIONAL)
//            <c COOExToolBar> 
//            To set the hoover image list.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void COOExToolBar::SetHotImageList( 
     CImageList* pList )    // @Parm The hoover image list.
{
    // Retrieve the tool bar control and set the list reference.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	CWnd* pWnd = &GetToolBarCtrl();
	pWnd->SendMessage( TB_SETHOTIMAGELIST, 0, ( LPARAM ) ( HIMAGELIST ) *pList );
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (FUNCTIONAL)
//            <c COOExToolBar> 
//            To set the normal image list.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void COOExToolBar::SetStandardImageList( 
     CImageList* pList )    // @Parm The normal image list.
{
    // Retrieve the tool bar control and set the list reference.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	CWnd* pWnd = &GetToolBarCtrl();
	pWnd->SendMessage( TB_SETIMAGELIST, 0, ( LPARAM ) ( HIMAGELIST ) *pList );
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (FUNCTIONAL)
//            <c COOExToolBar> 
//            To set the disable image list.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void COOExToolBar::SetDisableImageList( 
     CImageList* pList )    // @Parm The normal image list.
{
    // Retrieve the tool bar control and set the list reference.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	CWnd* pWnd = &GetToolBarCtrl();
	pWnd->SendMessage( TB_SETDISABLEDIMAGELIST, 0, ( LPARAM ) ( HIMAGELIST ) *pList );
}
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Start =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Usefull protected method.
// ------------------------------------------------------------------------
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (FUNCTIONAL)
//            <c COOExToolBar> 
//            To display the dropdown button for the given button.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
/*void COOExToolBar::SetButtonDropDown( 
     int nID )  // @Parm The id to show the drop down.
{
	// Change button style to dropdown.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	DWORD dwStyle = GetButtonStyle( CommandToIndex( nID ) );
	dwStyle |= TBSTYLE_DROPDOWN;
	SetButtonStyle( CommandToIndex( nID ), dwStyle );

    // Calculate the drop button size.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    CToolBarCtrl& bar = GetToolBarCtrl();
    CSize theStdButtonSize = CalcButtonSize();
    CRect rc( 0, 0, 0, 0 );
    bar.GetItemRect( CommandToIndex( nID ), rc );
    m_nDropButtonSize = rc.Width() - theStdButtonSize.cx;
}*/
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Start =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Usefull protected method.
// ------------------------------------------------------------------------
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (FUNCTIONAL)
//            <c COOExToolBar> 
//            To resieze the toolbar once something change.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void COOExToolBar::ResizeToolBar( void )
{
    // Set the size of the toolbar corresponding to the current mode.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    CSize theSize = ( m_nShowIconMode == 0 ) ? m_SmallIconSize : m_LargeIconSize;
    CSize theButtonSize = CalcButtonSize();

    // Resize the toolbar and the dock frame.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	SetSizes( theButtonSize, theSize );
    MoveWindow( 0, 0, 450, theButtonSize.cy );
    SendMessage( WM_SIZE, SIZE_RESTORED );
    CFrameWnd* pFrameWnd = GetDockingFrame();
	pFrameWnd->DelayRecalcLayout();
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (FUNCTIONAL)
//            <c COOExToolBar> 
//            To draw the gripper on the toolbar.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
/*void COOExToolBar::DrawGripper( 
     CDC& dc ) const        // @Parm The device context where to draw the gripper.
{
    // No gripper if floating
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	if( m_dwStyle & CBRS_FLOATING )
    {
		return;
    }
	
    // Retrieve the window coord and calculate the gripper position.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	CRect gripper;
	GetWindowRect( gripper );
	ScreenToClient( gripper );
	gripper.OffsetRect( -gripper.left, -gripper.top );

    // Draw it accordingly to the orientation of the toolbar.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    COLORREF clrBtnHilight = ::GetSysColor(COLOR_BTNHILIGHT);
    COLORREF clrBtnShadow = ::GetSysColor(COLOR_BTNSHADOW);
    if( m_dwStyle & CBRS_ORIENT_HORZ ) {
	
		// gripper at left
		gripper.DeflateRect( 3, 3 );
		gripper.right = gripper.left+3;
        dc.Draw3dRect( gripper, clrBtnHilight, clrBtnShadow );
		
		gripper.OffsetRect(5, 0);
        dc.Draw3dRect( gripper, clrBtnHilight, clrBtnShadow );
	}
	
	else {
		
		// gripper at top
		gripper.DeflateRect( 3, 3 );
		gripper.bottom = gripper.top+3;
		dc.Draw3dRect( gripper, clrBtnHilight, clrBtnShadow );
		
		gripper.OffsetRect(0, 5);
        dc.Draw3dRect( gripper, clrBtnHilight, clrBtnShadow );
	}
}*/
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Start =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// MFC Overloaded method.
// ------------------------------------------------------------------------
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (FUNCTIONAL)
//            <c COOExToolBar> 
//            Only overloaded to use our cmdui instead of their.  This is
//            the same code found in CToolBar::OnUpdateCmdUI, but we 
//            replace the CCmdUI by CCoolCmdUI.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void COOExToolBar::OnUpdateCmdUI( 
     CFrameWnd* pTarget,        // @Parm ??
     BOOL bDisableIfNoHndler )  // @Parm ??
{
	CCoolCmdUI state; // this is the only line that's different--PD
	state.m_pOther = this;
	
	state.m_nIndexMax = ( UINT ) DefWindowProc( TB_BUTTONCOUNT, 0, 0 );
	for ( state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax; state.m_nIndex++ )
	{
		// get button state
		TBBUTTON button;
		VERIFY( DefWindowProc( TB_GETBUTTON, state.m_nIndex, ( LPARAM ) &button ) );
		// TBSTATE_ENABLED == TBBS_DISABLED so invert it
		button.fsState ^= TBSTATE_ENABLED;
		
		state.m_nID = button.idCommand;
		
		// ignore separators
		if ( !( button.fsStyle & TBSTYLE_SEP ) )
		{
			// allow the toolbar itself to have update handlers
			if ( CWnd::OnCmdMsg( state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL ) )
				continue;
			
			// allow the owner to process the update
			state.DoUpdate( pTarget, bDisableIfNoHndler );
		}
	}
	
	// update the dialog controls added to the toolbar
	UpdateDialogControls( pTarget, bDisableIfNoHndler );
}
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Start =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// MFC Message method.
// ------------------------------------------------------------------------
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (FUNCTIONAL)
//            <c COOExToolBar> 
//            This will draw the gripper on the toolbar then repaints
//            client areas.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
/*void COOExToolBar::OnNcPaint( void ) 
{
	CControlBar::EraseNonClient();
	CWindowDC dc( this );
	DrawGripper( dc );
	
	CRect pRect;
	GetClientRect( &pRect );
	InvalidateRect( &pRect, TRUE );
	
	CWnd* pWnd = &GetToolBarCtrl();
	pWnd->GetClientRect( &pRect ); 
	pWnd->InvalidateRect( &pRect, TRUE ); 
}
*/
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (FUNCTIONAL)
//            <c COOExToolBar> 
//            Trap to set the flat bar style.
// @RDesc     Nonzero if the toolbar is correctly created.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int COOExToolBar::OnCreate( 
    LPCREATESTRUCT lpCreateStruct ) // @Parm Toolbar creation information.
{
    // Call the base class method.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	if ( CToolBar::OnCreate( lpCreateStruct ) == -1 )
    {
		return -1;
    }
	
    // Set the style to flat.
    // There is a but when docking vertical.  The size of the separator
    // is not calculate correctly by MFC.  Only in style flat.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	ModifyStyle( 0, TBSTYLE_FLAT );
	SendMessage( TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_DRAWDDARROWS );
    
    // The window is now created.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	return 0;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (FUNCTIONAL)
//            <c COOExToolBar> 
//            Trap to size correctly the toolbar with the carret.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
/*void COOExToolBar::OnNcCalcSize(
     BOOL bCalcValidRects,              // @Parm MFC Specific message.
     NCCALCSIZE_PARAMS FAR* lpncsp )    // @Parm MFC Specific message.
{
    // No gripper if floating
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	if( !( m_dwStyle & CBRS_FLOATING ) )
    {
        // Adjust non-client area for gripper at left or top.
	    if( m_dwStyle & CBRS_ORIENT_HORZ ) {
		    lpncsp->rgrc[0].left += 5;
		    lpncsp->rgrc[0].right += 5;
	    }
	    else {
		    lpncsp->rgrc[0].top += 5;
		    lpncsp->rgrc[0].bottom += 5;
	    }
    }
	
	CToolBar::OnNcCalcSize( bCalcValidRects, lpncsp );
}*/

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (FUNCTIONAL)
//            <c COOExToolBar> 
//            Call when ever the system color are changing. Trap to rebuild
//            the toolbar image list with the good background color.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void COOExToolBar::OnSysColorChange( void ) 
{
    // Call the base class method.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    CToolBar::OnSysColorChange();
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (FUNCTIONAL)
//            <c COOExToolBar> 
//            Call when the toolbar is moved.  There is a bug when the tool
//            bar is in flat mode, and in vertical position.  The separator
//            are not count in the height, so the last button is not completly
//            displayed.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void COOExToolBar::OnWindowPosChanging( LPWINDOWPOS _pWindowPos ) 
{
    // Default processing
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    if( m_bOldFloating != ( IsFloating()?true:false ) )
    {
        m_bOldFloating = !m_bOldFloating;
        _pWindowPos->flags |= SWP_DRAWFRAME;
    }
    CToolBar::OnWindowPosChanging( _pWindowPos );
}           
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Start =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// MFC Overloaded method.
// The following stuff is to make the command update UI mechanism
// work properly for flat tool bars. The main idea is to convert
// a "checked" button state into a "pressed" button state. Changed 
// lines marked with "PD"
// ------------------------------------------------------------------------
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (FUNCTIONAL)
//            <c CCoolCmdUI> 
//            Came from Paul Dilascia Article.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void CCoolCmdUI::Enable( BOOL bOn )
{
	m_bEnableChanged = TRUE;
	CToolBar* pToolBar = ( CToolBar* ) m_pOther;
	ASSERT( pToolBar != NULL );
	ASSERT_KINDOF( CToolBar, pToolBar );
	ASSERT( m_nIndex < m_nIndexMax );
	
	UINT nNewStyle = pToolBar->GetButtonStyle( m_nIndex ) & ~TBBS_DISABLED;
	if ( !bOn )
	{
		nNewStyle |= TBBS_DISABLED;
		// WINBUG: If a button is currently pressed and then is disabled
		// COMCTL32.DLL does not unpress the button, even after the mouse
		// button goes up!  We work around this bug by forcing TBBS_PRESSED
		// off when a button is disabled.
		nNewStyle &= ~TBBS_PRESSED;
	}
	ASSERT( !( nNewStyle & TBBS_SEPARATOR ) );
	pToolBar->SetButtonStyle( m_nIndex, nNewStyle );
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (FUNCTIONAL)
//            <c CCoolCmdUI> 
//            Came from Paul Dilascia Article.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void CCoolCmdUI::SetCheck( int nCheck )
{
	ASSERT( nCheck >= 0 && nCheck <= 2 ); // 0=>off, 1=>on, 2=>indeterminate
	CToolBar* pToolBar = ( CToolBar* ) m_pOther;
	ASSERT( pToolBar != NULL );
	ASSERT_KINDOF( CToolBar, pToolBar );
	ASSERT( m_nIndex < m_nIndexMax );
	
	UINT nOldStyle = pToolBar->GetButtonStyle( m_nIndex ); // PD
	UINT nNewStyle = nOldStyle &
		~( TBBS_PRESSED | TBBS_INDETERMINATE ); // PD
	if ( nCheck == 1 )
		nNewStyle |= TBBS_PRESSED; // PD
	else if ( nCheck == 2 )
		nNewStyle |= TBBS_INDETERMINATE;
	
	// Following is to fix display bug for TBBS_CHECKED:
	// If new state is unchecked, repaint--but only if style actually changing.
	// (Otherwise will end up with flicker)
	// 
	if ( nNewStyle != nOldStyle ) {
		ASSERT( !( nNewStyle & TBBS_SEPARATOR ) );
		pToolBar->SetButtonStyle( m_nIndex, nNewStyle );
		pToolBar->Invalidate();
	}
}
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

/*CSize COOExToolBar::CalcDynamicLayout(int nLength, DWORD dwMode)
{
	if ((nLength == -1)	&& 
	   !(dwMode & LM_MRUWIDTH) && !(dwMode & LM_COMMIT) &&
	   ((dwMode & LM_HORZDOCK) ||  (dwMode & LM_VERTDOCK)))
	{
		return CalcFixedLayout(dwMode & LM_STRETCH, dwMode & LM_HORZDOCK);
	}

	return CalcLayout(dwMode, nLength);
}
*/
//////////////////
// 1998 Microsoft Systems Journal - Paul DiLascia
// This is the all-important function that gets the true size of a button,
// instead of using m_sizeButton. And it's virtual, so you can override if
// my algorithm doesn't work, as will surely be the case in some circumstances.
//
CSize COOExToolBar::GetButtonSize(TBBUTTON* pData, int iButton)
{
	// Get the actual size of the button, not what's in m_sizeButton.
	// Make sure to do SendMessage instead of calling MFC's GetItemRect,
	// which has all sorts of bad side-effects! (Go ahead, take a look at it.)
	// 
	CRect rc;
	SendMessage(TB_GETITEMRECT, iButton, (LPARAM)&rc);
	CSize sz = rc.Size();
	
	////////////////
	// Now must do special case for various versions of comctl32.dll,
	//
	DWORD dwStyle = pData[iButton].fsStyle;
	if ((pData[iButton].fsState & TBSTATE_WRAP)) {
		if (dwStyle & TBSTYLE_SEP) {
			// this is the last separator in the row (eg vertically docked)
			// fudge the height, and ignore the width. TB_GETITEMRECT will return
			// size = (8 x 22) even for a separator in vertical toolbar
			//
			sz.cy = sz.cx;
			sz.cx = 0;			// separator takes no width if it's the last one
			
        } else if (dwStyle & TBSTYLE_DROPDOWN ) {//&&
			//!m_bShowDropdownArrowWhenVertical) {
			// ignore width of dropdown
			sz.cx = 0;
		}
	}
	return sz;
}

//////////////////
// 1998 Microsoft Systems Journal - Paul DiLascia
// I renamed this from _GetButton.
//
void COOExToolBar::GetButton(int nIndex, TBBUTTON* pButton) const
{
	CToolBar* pBar = (CToolBar*)this;
	VERIFY(pBar->SendMessage(TB_GETBUTTON, nIndex, (LPARAM)pButton));
	// TBSTATE_ENABLED == TBBS_DISABLED so invert it
	pButton->fsState ^= TBSTATE_ENABLED;
}

//////////////////
// 1998 Microsoft Systems Journal - Paul DiLascia
// I renamed this from _SetButton.
//
void COOExToolBar::SetButton(int nIndex, TBBUTTON* pButton)
{
	// get original button state
	TBBUTTON button;
	VERIFY(SendMessage(TB_GETBUTTON, nIndex, (LPARAM)&button));
	
	// prepare for old/new button comparsion
	button.bReserved[0] = 0;
	button.bReserved[1] = 0;
	// TBSTATE_ENABLED == TBBS_DISABLED so invert it
	pButton->fsState ^= TBSTATE_ENABLED;
	pButton->bReserved[0] = 0;
	pButton->bReserved[1] = 0;
	
	// nothing to do if they are the same
	if (memcmp(pButton, &button, sizeof(TBBUTTON)) != 0)
	{
		// don't redraw everything while setting the button
		DWORD dwStyle = GetStyle();
		ModifyStyle(WS_VISIBLE, 0);
		VERIFY(SendMessage(TB_DELETEBUTTON, nIndex, 0));
		VERIFY(SendMessage(TB_INSERTBUTTON, nIndex, (LPARAM)pButton));
		ModifyStyle(0, dwStyle & WS_VISIBLE);
		
		// invalidate appropriate parts
		if (((pButton->fsStyle ^ button.fsStyle) & TBSTYLE_SEP) ||
			((pButton->fsStyle & TBSTYLE_SEP) && pButton->iBitmap != button.iBitmap))
		{
			// changing a separator
			Invalidate(FALSE);
		}
		else
		{
			// invalidate just the button
			CRect rect;
			if (SendMessage(TB_GETITEMRECT, nIndex, (LPARAM)&rect))
				InvalidateRect(rect, FALSE);    // don't erase background
		}
	}
}

//////////////////
// 1998 Microsoft Systems Journal - Paul DiLascia
// Make the parent frame my owner. This is important for status bar
// prompts to work.
//

////////////////////////////////////////////////////////////////
// 1998 Microsoft Systems Journal - Paul DiLascia
// Stuff below is copied from MFC, my mods marked **PD**

#ifdef _MAC
#define CX_OVERLAP  1
#else
#define CX_OVERLAP  0
#endif

/*CSize COOExToolBar::CalcSize(TBBUTTON* pData, int nCount)
{
	ASSERT(pData != NULL && nCount > 0);
	
	CPoint cur(0,0);
	CSize sizeResult(0,0);
	int cyTallestOnRow = 0;
	
	for (int i = 0; i < nCount; i++)
	{
		if (pData[i].fsState & TBSTATE_HIDDEN)
			continue;
		
		// **PD** Load actual size of button into local var
		// that obscures CToolBar::m_sizeButton.
		CSize m_sizeButton = GetButtonSize(pData, i);
		
		// **PD** I also changed the logic below to be more correct.
		cyTallestOnRow = max(cyTallestOnRow, m_sizeButton.cy);
		sizeResult.cx = max(cur.x + m_sizeButton.cx, sizeResult.cx);
		sizeResult.cy = max(cur.y + m_sizeButton.cy, sizeResult.cy);
		
		cur.x += m_sizeButton.cx - CX_OVERLAP;
		
		if (pData[i].fsState & TBSTATE_WRAP)
		{
			cur.x = 0;
			cur.y += cyTallestOnRow;
			cyTallestOnRow = 0;
			if (pData[i].fsStyle & TBSTYLE_SEP)
				cur.y += m_sizeButton.cy;
		}
	}
	return sizeResult;
}

int COOExToolBar::WrapToolBar(TBBUTTON* pData, int nCount, int nWidth)
{
	ASSERT(pData != NULL && nCount > 0);
	
	int nResult = 0;
	int x = 0;
	for (int i = 0; i < nCount; i++)
	{
		pData[i].fsState &= ~TBSTATE_WRAP;
		
		if (pData[i].fsState & TBSTATE_HIDDEN)
			continue;
		
		int dx, dxNext;
		
		// **PD** Load actual size of button into local var
		// that obscures CToolBar::m_sizeButton.
		CSize m_sizeButton = GetButtonSize(pData, i);
		
		dx = m_sizeButton.cx;
		dxNext = dx - CX_OVERLAP;
		
		if (x + dx > nWidth)
		{
			BOOL bFound = FALSE;
			for (int j = i; j >= 0  &&  !(pData[j].fsState & TBSTATE_WRAP); j--)
			{
				// Find last separator that isn't hidden
				// a separator that has a command ID is not
				// a separator, but a custom control.
				if ((pData[j].fsStyle & TBSTYLE_SEP) &&
					(pData[j].idCommand == 0) &&
					!(pData[j].fsState & TBSTATE_HIDDEN))
				{
					bFound = TRUE; i = j; x = 0;
					pData[j].fsState |= TBSTATE_WRAP;
					nResult++;
					break;
				}
			}
			if (!bFound)
			{
				for (int j = i - 1; j >= 0 && !(pData[j].fsState & TBSTATE_WRAP); j--)
				{
					// Never wrap anything that is hidden,
					// or any custom controls
					if ((pData[j].fsState & TBSTATE_HIDDEN) ||
						((pData[j].fsStyle & TBSTYLE_SEP) &&
						(pData[j].idCommand != 0)))
						continue;
					
					bFound = TRUE; i = j; x = 0;
					pData[j].fsState |= TBSTATE_WRAP;
					nResult++;
					break;
				}
				if (!bFound)
					x += dxNext;
			}
		}
		else
			x += dxNext;
	}
	return nResult + 1;
}
*/
//////////////////////////////////////////////////////////////////////////
// 1998 Microsoft Systems Journal - Paul DiLascia
// Functions below are NOT modified. They're only here because they
// call the modified functions above, which are NOT virtual.
/*
void  COOExToolBar::SizeToolBar(TBBUTTON* pData, int nCount, int nLength, BOOL bVert)
{
	ASSERT(pData != NULL && nCount > 0);
	
	if (!bVert)
	{
		int nMin, nMax, nTarget, nCurrent, nMid;
		
		// Wrap ToolBar as specified
		nMax = nLength;
		nTarget = WrapToolBar(pData, nCount, nMax);
		
		// Wrap ToolBar vertically
		nMin = 0;
		nCurrent = WrapToolBar(pData, nCount, nMin);
		
		if (nCurrent != nTarget)
		{
			while (nMin < nMax)
			{
				nMid = (nMin + nMax) / 2;
				nCurrent = WrapToolBar(pData, nCount, nMid);
				
				if (nCurrent == nTarget)
					nMax = nMid;
				else
				{
					if (nMin == nMid)
					{
						WrapToolBar(pData, nCount, nMax);
						break;
					}
					nMin = nMid;
				}
			}
		}
		CSize size = CalcSize(pData, nCount);
		WrapToolBar(pData, nCount, size.cx);
	}
	else
	{
		CSize sizeMax, sizeMin, sizeMid;
		
		// Wrap ToolBar vertically
		WrapToolBar(pData, nCount, 0);
		sizeMin = CalcSize(pData, nCount);
		
		// Wrap ToolBar horizontally
		WrapToolBar(pData, nCount, 32767);
		sizeMax = CalcSize(pData, nCount);
		
		while (sizeMin.cx < sizeMax.cx)
		{
			sizeMid.cx = (sizeMin.cx + sizeMax.cx) / 2;
			WrapToolBar(pData, nCount, sizeMid.cx);
			sizeMid = CalcSize(pData, nCount);
			
			if (nLength < sizeMid.cy)
			{
				if (sizeMin == sizeMid)
				{
					WrapToolBar(pData, nCount, sizeMax.cx);
					return;
				}
				sizeMin = sizeMid;
			}
			else if (nLength > sizeMid.cy)
				sizeMax = sizeMid;
			else
				return;
		}
	}
}
*/
struct _AFX_CONTROLPOS
{
	int nIndex, nID;
	CRect rectOldPos;
};

/*CSize COOExToolBar::CalcLayout(DWORD dwMode, int nLength)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));
	if (dwMode & LM_HORZDOCK)
		ASSERT(dwMode & LM_HORZ);
	
	int nCount;
	TBBUTTON* pData;
	CSize sizeResult(0,0);
	
	// Load Buttons
	{
		nCount = SendMessage(TB_BUTTONCOUNT, 0, 0);
		if (nCount != 0)
		{
			int i;
			pData = new TBBUTTON[nCount];
			for (i = 0; i < nCount; i++)
				GetButton(i, &pData[i]); // **PD** renamed from _GetButton
		}
	}
	
	if (nCount > 0)
	{
		if (!(m_dwStyle & CBRS_SIZE_FIXED))
		{
			BOOL bDynamic = m_dwStyle & CBRS_SIZE_DYNAMIC;
			
			if (bDynamic && (dwMode & LM_MRUWIDTH))
				SizeToolBar(pData, nCount, m_nMRUWidth);
			else if (bDynamic && (dwMode & LM_HORZDOCK))
				SizeToolBar(pData, nCount, 32767);
			else if (bDynamic && (dwMode & LM_VERTDOCK))
				SizeToolBar(pData, nCount, 0);
			else if (bDynamic && (nLength != -1))
			{
				CRect rect; rect.SetRectEmpty();
				CalcInsideRect(rect, (dwMode & LM_HORZ));
				BOOL bVert = (dwMode & LM_LENGTHY);
				int nLen = nLength + (bVert ? rect.Height() : rect.Width());
				
				SizeToolBar(pData, nCount, nLen, bVert);
			}
			else if (bDynamic && (m_dwStyle & CBRS_FLOATING))
				SizeToolBar(pData, nCount, m_nMRUWidth);
			else
				SizeToolBar(pData, nCount, (dwMode & LM_HORZ) ? 32767 : 0);
		}
		
		sizeResult = CalcSize(pData, nCount);
		
		if (dwMode & LM_COMMIT)
		{
			_AFX_CONTROLPOS* pControl = NULL;
			int nControlCount = 0;
			BOOL bIsDelayed = m_bDelayedButtonLayout;
			m_bDelayedButtonLayout = FALSE;
			
			for(int i = 0; i < nCount; i++)
				if ((pData[i].fsStyle & TBSTYLE_SEP) && (pData[i].idCommand != 0))
					nControlCount++;
				
				if (nControlCount > 0)
				{
					pControl = new _AFX_CONTROLPOS[nControlCount];
					nControlCount = 0;
					
					for(int i = 0; i < nCount; i++)
					{
						if ((pData[i].fsStyle & TBSTYLE_SEP) && (pData[i].idCommand != 0))
						{
							pControl[nControlCount].nIndex = i;
							pControl[nControlCount].nID = pData[i].idCommand;
							
							CRect rect;
							GetItemRect(i, &rect);
							ClientToScreen(&rect);
							pControl[nControlCount].rectOldPos = rect;
							
							nControlCount++;
						}
					}
				}
				
				if ((m_dwStyle & CBRS_FLOATING) && (m_dwStyle & CBRS_SIZE_DYNAMIC))
					m_nMRUWidth = sizeResult.cx;
				for (i = 0; i < nCount; i++)
					SetButton(i, &pData[i]); // **PD** renamed from _SetButton
				
				if (nControlCount > 0)
				{
					for (int i = 0; i < nControlCount; i++)
					{
						CWnd* pWnd = GetDlgItem(pControl[i].nID);
						if (pWnd != NULL)
						{
							CRect rect;
							pWnd->GetWindowRect(&rect);
							CPoint pt = rect.TopLeft() - pControl[i].rectOldPos.TopLeft();
							GetItemRect(pControl[i].nIndex, &rect);
							pt = rect.TopLeft() + pt;
							pWnd->SetWindowPos(NULL, pt.x, pt.y, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
						}
					}
					delete[] pControl;
				}
				m_bDelayedButtonLayout = bIsDelayed;
		}
		delete[] pData;
	}
	
	//BLOCK: Adjust Margins
	{
		CRect rect; rect.SetRectEmpty();
		CalcInsideRect(rect, (dwMode & LM_HORZ));
		sizeResult.cy -= rect.Height();
		sizeResult.cx -= rect.Width();
		
		CSize size = CControlBar::CalcFixedLayout((dwMode & LM_STRETCH), (dwMode & LM_HORZ));
		sizeResult.cx = max(sizeResult.cx, size.cx);
		sizeResult.cy = max(sizeResult.cy, size.cy);
	}
	return sizeResult;
}

CSize COOExToolBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	DWORD dwMode = bStretch ? LM_STRETCH : 0;
	dwMode |= bHorz ? LM_HORZ : 0;
	
	return CalcLayout(dwMode);
}*/

// Start =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Macro and other definition.
// ------------------------------------------------------------------------
IMPLEMENT_DYNAMIC( COOBmpToolBar, COOExToolBar )
BEGIN_MESSAGE_MAP( COOBmpToolBar, COOExToolBar )
	//{{AFX_MSG_MAP( COOBmpToolBar )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Start =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Initialization and destruction method.
// ------------------------------------------------------------------------
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (IMPLEMENTATION)
//            <c COOBmpToolBar> 
//            This is the main constructor.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
COOBmpToolBar::COOBmpToolBar( void )
{
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (IMPLEMENTATION)
//            <c COOBmpToolBar> 
//            This is the main destructor.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
COOBmpToolBar::~COOBmpToolBar( void )
{
}
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Start =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Image list initialization method.
// ------------------------------------------------------------------------
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (FUNCTIONAL)
//            <c COOBmpToolBar> 
//            To set the image list in the control.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void COOBmpToolBar::InitImageList( void )
{
    // Build the image list.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    for ( int i=0; i<NB_POSSIBLE_MODE; i++ )
    {
        BuildImageList( m_ImageList[ i ], m_256Image[ i ] );
    }

    // Set the image list according to the current mode.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    AssignImageList();
}
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Start =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Private Image list method.
// ------------------------------------------------------------------------
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (FUNCTIONAL)
//            <c COOBmpToolBar> 
//            To build an image list based on the given bitmap and size.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void COOBmpToolBar::BuildImageList( 
     CImageList& _rImageList,       // @Parm The image list to build.
     const CExBitmap& _rBitmap )    // @Parm The bitmap to put in the image list.
{
    CBitmap bmpImage;
    bmpImage.LoadBitmap( _rBitmap.m_nResourceId );
    _rImageList.Add( &bmpImage, _rBitmap.m_clrTransparent );
}
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Start =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Image list information method.
// ------------------------------------------------------------------------
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @mfunc:    (FUNCTIONAL)
//            <c COOBmpToolBar> 
//            To set the small hoover bitmap.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void COOBmpToolBar::SetBitmap( 
     UINT _nBitmap,         // @Parm The bitmap id.
     ImageMode_ _Mode,      // @Parm The image mode.
     COLORREF _clrBk,       // @Parm The background color.
     bool _b256 )           // @Parm The nb of color.
{
    // Initialise the bitmap info.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    if ( _b256 )
    {
        m_256Image[ _Mode ].m_nResourceId = _nBitmap;
        m_256Image[ _Mode ].m_clrTransparent = _clrBk;
    }
    else
    {
        m_16Image[ _Mode ].m_nResourceId = _nBitmap;
        m_16Image[ _Mode ].m_clrTransparent = _clrBk;
    }
}
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Start =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Usefull protected method.
// ------------------------------------------------------------------------
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Start =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// MFC Overloaded method.
// ------------------------------------------------------------------------
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Start =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// MFC Message method.
// ------------------------------------------------------------------------
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

