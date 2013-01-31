// MainFrm.cpp : implementation of the CMainFrame class
//
//2.5 animation icon may occupy many cpu time.


#include "stdafx.h"
#include "MyIE.h"

#include <INITGUID.H> 
#include <Mshtmhst.h>
#include <wininet.h>
#include <ExDisp.h>  // For ShellUIHelper
//#include <comdef.h>  // For Compiler COM Support Classes
#include <UrlHist.h>

#include "popupdlg.h"
#include "MainFrm.h"
#include "setproxy.h"
#include "externalUtils.h"

#include "HistoryTree.h"
#include "Searchbar.h"
#include "FavTree.h"
#include "ShellTree.h"
#include "ToolMenu.h"
#include "MenuData.h"
//#include <afxsock.h>
#include <Winsock.h>

#include "setscdlg.h"
#include "setaliasdlg.h"
#include "customizetoolbar.h"
#include "MyIEOptions.h"
#include "StickName.h"
#include "WebFileOpenDlg.h"
#include "InputPassword.h"
#include "Resourcebar.h"

#include "MenuOrder.h"
#include "OrgGroupDlg.h"
#include "GroupUrlDlg.h"
#include "MAPI.h"
//#include "hlink.h"
#include "SimpleSAH.h"
#include "SelProxyCatDlg.h"
#include "AnimateIcon.h"
#include "QuickSearchDlg.h"
#include "Crypt.h"
#include "CollectorDlg.h"
#include "LinkListDlg.h"
#include "AddToThisFavDlg.h"
#include "inputalias.h"
#include "urlfilterprop.h"
//ras speed
#include "rasspdm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//Define the command group GUID for the WebBrowser control 
//It is undocumented, and supose to be changed in the feature
DEFINE_GUID(CGID_IWebBrowser,0xED016940L,0xBD5B,0x11cf,0xBA,
      0x4E,0x00,0xC0,0x4F,0xD7,0x08,0x16); 
static const IID IID_ITravelLogStg	= { 0x7EBFDD80, 0xAD18, 0x11d3, {0xA4,0xC5,0x00,0xC0,0x4F,0x72,0xD6,0xB8}};

#define RBBS_USECHEVRON     0x00000200  // display drop-down button for this band if it's sized smaller than ideal width

typedef HRESULT (CALLBACK* LPFNDLLFUNC1)(HWND ,DWORD);
typedef BOOL (CALLBACK* LPFNDLLFUNC2)(HWND ,LPTSTR, int, BOOL);
typedef ULONG (CALLBACK* LPMAPISendDocuments)(ULONG, LPTSTR, LPTSTR, LPTSTR, ULONG);

#define TABPADDINGX 6
#define TABPADDINGY 4
#define FBPADDINGX 3
#define FBPADDINGY 2
 
/*typedef BOOL (CALLBACK* LPFNADDFAV)(HWND, TCHAR*, UINT, TCHAR*, UINT, 
                                    LPITEMIDLIST);*/

//hook
//HINSTANCE  hdll = NULL;
//typedef BOOL (* HOOK)(int /*ctrl,alt*/, int /*char*/); //1-ctrl, 2-alt
//typedef BOOL (* UNHOOK)();
//HOOK hook;
//UNHOOK unhook;


//global variables
//int nOpenType = 1;
//CString strURL;
int percent = -1;
CMainFrame* pMainFrame = NULL;
int nStart = 4;
BOOL EnablePopupFilter = TRUE, EnableDupFilter = TRUE, EnableAutoFilter = FALSE;
int bGoHome;
BOOL m_bTopTab;
CStringArray m_astrLinks;
CStringArray m_astrFavs;
int m_bMax;  //1. max, 0, unmax, 2, tile/cascade, 3, restore, 4 minimize
BOOL bConfirmExit = TRUE;
BOOL bSingleMenu = FALSE;
BOOL bSysTray = FALSE;
int nMaxWins = 100;
BOOL bSilent = TRUE;
int nNextTab = -1;
int nDefFontSize = 3;
BOOL bURLOnly = FALSE;
int nTrayIconID;
BOOL bOutMenu = TRUE;
HMENU hLastMenu = NULL;
UINT nLastFlag;
UINT nLastItemID;
BOOL bFilteBlank = FALSE;
BOOL m_bAutoStart = FALSE;
BOOL bHidden= FALSE;
//HANDLE m_hMutex =  CreateMutex(NULL, TRUE, "MyIESysMutex");
BOOL bLinkInNewWindow, bFileInNewWindow, bAddressInNewWindow, bFavInNewWindow,  bHisInNewWindow;
float gSeed = 1;
BOOL bCompactFavBar = FALSE;
BOOL bConfirmCloseAll = TRUE;
BOOL bOneInstance = FALSE;
//BOOL bUpdateTab = FALSE;
typedef LONG (CALLBACK* GETRES)(int);
GETRES pGetRes = NULL;
BOOL m_bRTab;
CToolMenu gmenu;
static char addr[256];
BSTR bstryes;
BOOL bVerbose;
//BOOL bHideBars = TRUE;
BOOL bAutoSetBG, bLockHomepage;
char lpfiles[1024];
BOOL bProxyDirty = FALSE;


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

const UINT MSWHELL_ROLLMSG = RegisterWindowMessage( "MSWHEEL_ROLLMSG" );
BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_COMMAND(ID_OPTIONS_MYIESTARTUP_OPENBLANKPAGE, OnOptionsMyiestartupOpenblankpage)
	ON_COMMAND(ID_OPTIONS_MYIESTARTUP_OPENFAVORITEPAGES, OnOptionsMyiestartupOpenfavoritepages)
	ON_COMMAND(ID_OPTIONS_MYIESTARTUP_RESUMELASTVISITEDPAGES, OnOptionsMyiestartupResumelastvisitedpages)
	ON_COMMAND(ID_OPTIONS_MYIESTARTUP_OPENMOSTFAVORITEPAGES, OnOptionsMyiestartupOpenmostfavpages)
	ON_COMMAND(ID_OPTIONS_MYIESTARTUP_OPENSTARTUPGROUP, OnOptionsMyiestartupGroup)
	ON_COMMAND(ID_OPTIONS_MYIEGOHOME, OnOptionsMyiegohome)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_NEW_BLANK, OnFileNewBlank)
	ON_COMMAND(ID_FILE_NEW_CURRENTPAGE, OnFileNewCurrentpage)
	ON_COMMAND(ID_VIEW_ADDRESSBAR, OnViewAddressbar)
	ON_COMMAND(ID_VIEW_TASKBAR, OnViewTaskbar)
//	ON_COMMAND(ID_FILE_SAVEAS, OnFileSaveas)
//	ON_COMMAND(ID_FILE_SAVEHTML, OnFileSavehtml)
	ON_COMMAND(ID_FILE_CLOSEALL, OnFileCloseall)
	ON_COMMAND(ID_FILE_WORKOFFLINE, OnFileWorkoffline)
	ON_WM_CLOSE()
	ON_COMMAND(ID_VIEW_SOURCE, OnViewSource)
	ON_COMMAND(ID_FILE_CLOSE_1, OnFileClose)
	ON_COMMAND(ID_FILE_CLOSE_2, OnFileClose2)
	ON_COMMAND(ID_TOOLS_INTERNETOPTIONS, OnToolsInternetoptions)
	ON_COMMAND(ID_FAVORITES_ADDTOFAVORITES, OnFavoritesAddtofavorites)
	ON_COMMAND(ID_FAVORITES_EXPORTFAVORITES, OnFavoritesExportfavorites)
	ON_COMMAND(ID_FAVORITES_IMPORTFAVORITES, OnFavoritesImportfavorites)
	ON_COMMAND(ID_FAVORITES_ORGNIZEFAVORITES, OnFavoritesOrgnizefavorites)
	ON_COMMAND(ID_RTAB_LOCK, OnRtabLock)
	ON_COMMAND(ID_TOOLS_ADDTOPOPUPFILTER, OnToolsAddtopopupfilter)
	ON_COMMAND(ID_RTAB_ADDTOPOPUPFILTER, OnRtabAddtopopupfilter)
	ON_COMMAND(ID_TOOLS_EABLEPOPUPFILTER, OnToolsEablepopupfilter)
	ON_COMMAND(ID_TOOLS_EDITPOPUPFILTERLIST, OnToolsEditpopupfilterlist)
	ON_UPDATE_COMMAND_UI(ID_FILE_WORKOFFLINE, OnUpdateFileWorkoffline)
//	ON_COMMAND(ID_OPTIONS_MYIEGOHOME, OnOptionsMyiegohome)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_INTERNETOPTIONS, OnUpdateMenu)
	ON_COMMAND(ID_TOOLS_EMAIL, OnToolsEmail)
	ON_COMMAND(ID_TOOLS_ADDR, OnToolsAddr)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_USE_PROXY, OnUpdateOptionsUseProxy)
	ON_COMMAND(ID_OPTIONS_USE_PROXY, OnOptionsUseProxy)
	ON_COMMAND(ID_TOOLS_COOKIE, OnToolsCleanCookie)
	ON_COMMAND(ID_TOOLS_CACHE, OnToolsCache)
	ON_COMMAND(ID_TOOLS_ALL, OnToolsAll)
	ON_COMMAND(ID_TOOLS_HISTORY, OnToolsHistory)
	ON_COMMAND(ID_OPTIONS_SET_PROXY, OnOptionsSetProxy)
//	ON_COMMAND(ID_OPTION_TOP_TAB, OnOptionTabTop)
//	ON_COMMAND(ID_OPTION_BOT_TAB, OnOptionTabBot)
//	ON_UPDATE_COMMAND_UI(ID_OPTION_BOT_TAB, OnUpdateOptionTabPos)
	ON_COMMAND(ID_TOOLS_EXTERNALUTILITIES_UTILITYMANAGER, OnToolsExternalUtilitymanager)
	ON_COMMAND(ID_VIEW_HIS, OnViewHis)
	ON_COMMAND(ID_VIEW_EXPLOREBAR_SEARCH, OnViewSearch)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HIS, OnUpdateViewExp)
	ON_COMMAND(ID_VIEW_FULLSCREEN, OnViewFullscreen)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FULLSCREEN, OnUpdateViewFullscreen)
	ON_COMMAND(ID_OPTIONS_SETASDEFAULTBROWSER, OnOptionsSetasdefaultbrowser)
	ON_COMMAND(ID_ADDR_DROP, OnAddrDrop)
	ON_COMMAND(ID_ADDR_FOCUS, OnAddrFocus)
	ON_COMMAND(ID_FILE_NEW_S, OnFileNew)
	ON_COMMAND(ID_HELP_HELP, OnHelpHelp)
	ON_COMMAND(ID_OPTIONS_ACTIVENEWWINDOW, OnOptionsActivenewwindow)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_ACTIVENEWWINDOW, OnUpdateOptionsActivenewwindow)
//	ON_COMMAND(ID_OPTIONS_MYIESTARTUP_OPENBLANKPAGE, OnOptionsMyiestartupOpenblankpage)
//	ON_COMMAND(ID_OPTIONS_MYIESTARTUP_OPENFAVORITEPAGES, OnOptionsMyiestartupOpenfavoritepages)
//	ON_COMMAND(ID_OPTIONS_MYIESTARTUP_RESUMELASTVISITEDPAGES, OnOptionsMyiestartupResumelastvisitedpages)
	ON_COMMAND(ID_RTAB_CLOSE_OTHER, OnCloseOther)
	ON_COMMAND(ID_RTAB_ADDFAV, OnRtabAddfav)
	ON_COMMAND(ID_VIEW_LINKS, OnViewLinks)
	ON_COMMAND(ID_VIEW_TOOLBAR, OnViewToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, OnUpdateViewToolbar)
	ON_WM_SIZE()
//	ON_COMMAND(ID_OPTION_MULTI_TAB, OnOptionMultiTab)
	ON_COMMAND(ID_HELP_FORUM, OnHelpForum)
	ON_COMMAND(ID_HELP_MAIL, OnHelpMail)
	ON_COMMAND(ID_TOOLS_OPENALLLINKS, OnToolsOpenalllinks)
	ON_COMMAND(ID_OPTION_ALWAYS_NEW, OnOptionAlwaysNew)
	ON_UPDATE_COMMAND_UI(ID_OPTION_ALWAYS_NEW, OnUpdateOptionAlwaysNew)
	ON_COMMAND(ID_FILE_CLOSE_KEEPCUR, OnFileCloseKeepcur)
//	ON_COMMAND(ID_OPTIONS_NEWTAB, OnOptionsNewtab)
//	ON_UPDATE_COMMAND_UI(ID_OPTIONS_NEWTAB, OnUpdateOptionsNewtab)
	ON_COMMAND(ID_VIEW_REFRESHALL, OnViewRefreshall)
	ON_COMMAND(ID_VIEW_STOP_ALL, OnViewStopAll)
	ON_COMMAND(ID_HELP_HOMEPAGE, OnHelpHomepage)
	ON_COMMAND(ID_FILE_NEW_CLIP, OnFileNewClip)
	ON_COMMAND(ID_WINDOW_CASCADE, OnWindowCascade)
	ON_COMMAND(ID_WINDOW_TILE_HORZ, OnWindowTileHorz)
	ON_COMMAND(ID_WINDOW_TILE_VERT, OnWindowTileVert)
	ON_WM_SETTINGCHANGE()
	ON_WM_MEASUREITEM()
	ON_COMMAND(ID_TOOLS_IP, OnToolsIp)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_IP, OnUpdateToolsIp)
	ON_COMMAND(ID_WINDOW_RESTOR, OnWindowRestore)
	ON_WM_DRAWITEM()
	ON_COMMAND(ID_WINDOW_MIN, OnWindowMin)
	ON_COMMAND(ID_VIEW_MENUBAR, OnViewMenubar)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_SETBACKGROUNDPIC_NONE, OnUpdateToolsSetbackgroundpic)
	ON_COMMAND(ID_FILE_NEWMYIE, OnFileNewmyie)
	ON_COMMAND(ID_FILE_OPENF, OnFileOpen)
	ON_COMMAND(ID_VIEW_TOOLBARTEXTLABEL, OnViewToolbartextlabel)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBARTEXTLABEL, OnUpdateViewToolbartextlabel)
	ON_COMMAND(ID_TOOLS_OPENSTART, OnToolsOpenstart)
//	ON_UPDATE_COMMAND_UI(ID_OPTION_MULTI_TAB, OnUpdateOptionMultiTab)
//	ON_COMMAND(ID_OPTION_AUTOTAB, OnOptionAutotab)
//	ON_UPDATE_COMMAND_UI(ID_OPTION_AUTOTAB, OnUpdateOptionAutotab)
	ON_COMMAND(ID_SHORTCUT_MFL, OnShortcutMfl)
	ON_COMMAND(ID_SHORTCUT_USE_SC, OnShortcutUseSc)
	ON_UPDATE_COMMAND_UI(ID_SHORTCUT_USE_SC, OnUpdateShortcutUseSc)
	ON_COMMAND(ID_SHORTCUT_SC, OnShortcutSc)
	ON_COMMAND(ID_SHORTCUT_ALIAS, OnShortcutAlias)
	ON_UPDATE_COMMAND_UI(ID_SHORTCUT_ALIAS, OnUpdateShortcutAlias)
	ON_COMMAND(ID_SHORTCUT_MANALIAS, OnShortcutManalias)
	ON_COMMAND(ID_VIEW_TOOLBARS_CUSTOMIZE, OnViewToolbarsCustomize)
	ON_COMMAND(ID_OPTIONS_MYIEOPTIONS, OnOptionsMyieoptions)
	ON_WM_INITMENUPOPUP()
//	ON_COMMAND(ID_OPTIONS_MYIESTARTUP_OPENMOSTFAVORITEPAGES, OnOptionsMyiestartupOpenmostfavpages)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVEHTML, OnUpdateMenu)
	ON_UPDATE_COMMAND_UI(ID_FILE_QUICKSAVE, OnUpdateMenu)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SOURCE, OnUpdateMenu)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVEAS, OnUpdateMenu)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSEALL, OnUpdateMenu)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE_1, OnUpdateMenu)
//	ON_UPDATE_COMMAND_UI(ID_OPTION_TOP_TAB, OnUpdateOptionTabPos)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EXPLOREBAR_SEARCH, OnUpdateViewExp)
	ON_UPDATE_COMMAND_UI(ID_FAVORITES_DROPDOWN, OnUpdateViewExp)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_RESTOR, OnUpdateMenu)
//	ON_UPDATE_COMMAND_UI(ID_WINDOW_MIN, OnUpdateMenu)
	ON_COMMAND(ID_FAVORITES_BEST, DoNothing)
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_SHORTCUTS_USE3721, OnShortcutsUse3721)
	ON_UPDATE_COMMAND_UI(ID_SHORTCUTS_USE3721, OnUpdateShortcutsUse3721)
	ON_COMMAND(ID_RTAB_STICKNAME, OnRtabStickname)
	ON_COMMAND(ID_RTAB_STOP, OnRtabStop)
	ON_COMMAND(ID_RTAB_REFRESH, OnRtabRefresh)
//	ON_COMMAND(ID_OPTION_SEQ_DEFAULT, OnOptionSeqDefault)
//	ON_UPDATE_COMMAND_UI(ID_OPTION_SEQ_DEFAULT, OnUpdateOptionSeqDefault)
//	ON_COMMAND(ID_OPTION_SEQ_NEXT, OnOptionSeqNext)
//	ON_UPDATE_COMMAND_UI(ID_OPTION_SEQ_NEXT, OnUpdateOptionSeqNext)
//	ON_COMMAND(ID_OPTION_SEQ_PRE, OnOptionSeqPre)
//	ON_UPDATE_COMMAND_UI(ID_OPTION_SEQ_PRE, OnUpdateOptionSeqPre)
	ON_COMMAND(ID_TAB_PRE, OnTabPre)
	ON_COMMAND(ID_TAB_NEXT, OnTabNext)
	ON_COMMAND(ID_OPTIONS_NEWTAB, OnNewTab)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_NEWTAB, OnUpdateNewTab)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_COMMAND(ID_OPTIONS_URLINSTATUS, OnOptionsUrlinstatus)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_URLINSTATUS, OnUpdateOptionsUrlinstatus)
	ON_COMMAND(ID_VIEW_FAVS, OnViewFavs)
	ON_COMMAND(ID_RTAB_PROTECT, OnRtabProtect)
	ON_COMMAND(ID_VIEW_EX_TOOL, OnViewExTool)
	ON_WM_DROPFILES()
	ON_WM_TIMER()
	ON_COMMAND(ID_FILE_STOP_OPENALL, OnFileStopOpenall)
	ON_UPDATE_COMMAND_UI(ID_FILE_STOP_OPENALL, OnUpdateFileStopOpenall)
	ON_COMMAND(ID_FILE_HIDE, OnFileHide)
	ON_COMMAND(ID_VIEW_FOLDER, OnViewFolder)
	ON_COMMAND(ID_OPTIONS_OPENLINKINNEWWINDOW, OnOptionsOpenlinkinnewwindow)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_OPENLINKINNEWWINDOW, OnUpdateOptionsOpenlinkinnewwindow)
	ON_COMMAND(ID_OPTIONS_BROWSEMODES_DEFAULT, OnOptionsBrowsemodesDefault)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_BROWSEMODES_DEFAULT, OnUpdateOptionsBrowsemodesDefault)
	ON_COMMAND(ID_OPTIONS_BROWSEMODES_NEWS, OnOptionsBrowsemodesNews)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_BROWSEMODES_NEWS, OnUpdateOptionsBrowsemodesNews)
	ON_COMMAND(ID_OPTIONS_BROWSEMODES_STANDARD, OnOptionsBrowsemodesStandard)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_BROWSEMODES_STANDARD, OnUpdateOptionsBrowsemodesStandard)
	ON_COMMAND(ID_VIEW_EXPLOREBAR_RESOURCE, OnViewExplorebarResource)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_EDIT_UNDOCLOSE, OnEditUndoclose)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDOCLOSE, OnUpdateEditUndoclose)
	ON_COMMAND(ID_EDIT_EDIT, OnEditEdit)
	ON_WM_ACTIVATE()
	ON_COMMAND(ID_ORGGROUP, OnOrggroup)
	ON_WM_SETTINGCHANGE()
	ON_UPDATE_COMMAND_UI(ID_WINDOW_MIN, OnUpdateWindowMin)
	ON_COMMAND(ID_NEWWINDOW, OnNewwindow)
	ON_COMMAND(ID_NEWWINDOW_FAV, OnNewwindowFav)
	ON_UPDATE_COMMAND_UI(ID_NEWWINDOW_FAV, OnUpdateNewwindowFav)
	ON_COMMAND(ID_NEWWINDOW_HISTORY, OnNewwindowHistory)
	ON_UPDATE_COMMAND_UI(ID_NEWWINDOW_HISTORY, OnUpdateNewwindowHistory)
	ON_COMMAND(ID_NEWWINDOW_FILE, OnNewwindowFile)
	ON_UPDATE_COMMAND_UI(ID_NEWWINDOW_FILE, OnUpdateNewwindowFile)
	ON_COMMAND(ID_FILE_SENDPAGEBYEMAIL, OnFileSendpagebyemail)
	ON_COMMAND(ID_FILE_QUICKSAVE, OnFileQuicksave)
	ON_COMMAND(ID_FILE_SENDLINKBYEMAIL, OnFileSendlinkbyemail)
	ON_COMMAND(ID_TOOLS_SELBG, OnToolsSelbg)
	ON_COMMAND(ID_TOOLS_SETBG, OnToolsSetbg)
	ON_COMMAND(ID_OPTIONS_EXPORTPROXYLIST, OnOptionsExportproxylist)
	ON_COMMAND(ID_OPTIONS_IMPORTPROXYLIST, OnOptionsImportproxylist)
	ON_COMMAND(ID_TOOLS_MEM, OnToolsMem)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_MEM, OnUpdateToolsMem)
	ON_COMMAND(ID_RTAB_AUTOREF, OnRtabAutoref)
	ON_UPDATE_COMMAND_UI(ID_RTAB_AUTOREF, OnUpdateRtabAutoref)
	ON_COMMAND(ID_RTAB_REFFRE, OnRtabReffre)
	ON_COMMAND(ID_VIEW_LOCK, OnViewLock)
	ON_COMMAND(ID_TOOLS_ALL_BK, OnToolsAllBk)
	ON_COMMAND(ID_TOOLS_QUICKSEARCH, OnToolsQuicksearch)
	ON_COMMAND(ID_TOOLS_SYNC, OnToolsSync)
	ON_COMMAND(ID_SC_FILLFORM, OnScFillform)
	ON_COMMAND(ID_SC_SAVEFORM, OnScSaveform)
	ON_COMMAND(ID_SC_SAVEGENFORM, OnScSavegenform)
	ON_COMMAND(ID_TOOLS_COLLECT, OnToolsCollect)
	ON_COMMAND(ID_TOOLS_CLEANADDR, OnToolsCleanaddr)
	ON_COMMAND(ID_ALL_PAGE_LINKS, OnAllPageLinks)
	ON_COMMAND(ID_TOOLS_AUTOSETBG, OnToolsAutosetbg)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_AUTOSETBG, OnUpdateToolsAutosetbg)
	ON_COMMAND(ID_VIEW_ENCODING_AUTO, OnViewEncodingAuto)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ENCODING_AUTO, OnUpdateViewEncodingAuto)
	ON_COMMAND(ID_SAVEAS_GROUP, OnSaveasGroup)
	ON_COMMAND(ID_GROUP, OnGroup)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPENF, OnUpdateFileOpenf)
	ON_COMMAND(ID_TOOL_CLEANMYIEFORM, OnToolCleanmyieform)
	ON_COMMAND(ID_RTAB_ADDALIAS, OnRtabAddalias)
	ON_COMMAND(ID_RTAB_ADDSC, OnRtabAddsc)
	ON_COMMAND(ID_RTAB_AUTOSAVE, OnRtabAutosave)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_MYIEGOHOME, OnUpdateOptionsMyiegohome)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_MYIESTARTUP_OPENBLANKPAGE, OnUpdateOptionsMyiestartupOpenblankpage)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_MYIESTARTUP_OPENFAVORITEPAGES, OnUpdateOptionsMyiestartupOpenfavoritepages)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_MYIESTARTUP_OPENMOSTFAVORITEPAGES, OnUpdateOptionsMyiestartupOpenmostfavoritepages)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_MYIESTARTUP_OPENSTARTUPGROUP, OnUpdateOptionsMyiestartupOpenstartupgroup)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_MYIESTARTUP_RESUMELASTVISITEDPAGES, OnUpdateOptionsMyiestartupResumelastvisitedpages)
	ON_COMMAND(ID_TOOLS_MOUSE_SCR, OnToolsMouseScr)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_MOUSE_SCR, OnUpdateToolsMouseScr)
	ON_COMMAND(ID_TOOLS_URL_FILTER, OnToolsUrlFilter)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_URL_FILTER, OnUpdateToolsUrlFilter)
	ON_COMMAND(ID_OPTIONS_MYIESTARTUP_NONE, OnOptionsMyiestartupNone)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_MYIESTARTUP_NONE, OnUpdateOptionsMyiestartupNone)
	ON_COMMAND(ID_TOOLS_BP, OnToolsBp)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_BP, OnUpdateToolsBp)
	ON_COMMAND(ID_VIEW_LOCKTOOLBAR, OnViewLocktoolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LOCKTOOLBAR, OnUpdateViewLocktoolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EXPLOREBAR_RESOURCE, OnUpdateViewExp)
	ON_COMMAND(ID_TOOLS_AUTOSCROLL_ALL, OnToolsAutoscrollAll)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_AUTOSCROLL_ALL, OnUpdateToolsAutoscrollAll)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_VIEW_WIN_PRE, ID_VIEW_WIN_CLOSE, OnViewWinCust)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_WIN_PRE, ID_VIEW_WIN_CLOSE, OnUpdateViewWinCust)
	ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, OnDropDown)
	ON_COMMAND(IDOK, OnNewAddressEnter)
	ON_COMMAND(ID_FONT_DROPDOWN, DoNothing)
	ON_COMMAND(ID_ENCODE_DROPDOWN, DoNothing)
	ON_COMMAND(ID_BMODE_DROPDOWN, DoNothing)
	ON_COMMAND(ID_FAVORITES_DROPDOWN, OnViewFavorite)
	ON_CBN_SELENDOK(AFX_IDW_TOOLBAR + 1,OnNewAddress)
	ON_COMMAND_RANGE(0x1380, 0x6000, OnFavorite)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_PROCESS, OnUpdateProgress)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_WINS, OnUpdateWins)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_IP, OnUpdateIP)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_BP, OnUpdateBP)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_RES, OnUpdateRES)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_SECU, OnUpdateSecu)
	ON_NOTIFY(TCN_SELCHANGE, AFX_IDW_TOOLBAR + 2, OnSelChange)
	ON_NOTIFY(NM_RCLICK, AFX_IDW_TOOLBAR + 2, OnRClick)
	ON_COMMAND_RANGE(0x8044, 0x8047, OnEncoding)
	ON_COMMAND_RANGE(ID_VIEW_ENCODING_JANESEEUC, ID_VIEW_ENCODING_JANESEAUTO, OnEncoding)
	ON_COMMAND_RANGE(ID_VIEW_FONTS_LARGEST , ID_VIEW_FONTS_SMALLEST, OnFonts)
	ON_COMMAND_RANGE(0xd00, 0xdf0, OnUtils)
	ON_MESSAGE(WM_USER_SHELL_OPEN, OnShellOpen) 
	ON_COMMAND(ID_LINK_POPUP, OnLinkPopup)
	ON_COMMAND(ID_FAV_POPUP, OnFavPopup)
	ON_MESSAGE(WM_UPDATE_FAV, OnUpdateFav) 
	ON_MESSAGE(WM_UPDATE_TAB, OnUpdateTab) 
	ON_COMMAND_RANGE(0x1000, 0x10ff, OnAllFav)
	ON_MESSAGE(WM_UPDATE_TAB_TIP, OnUpdateTabTips) 
	ON_MESSAGE(WM_ACTIVATE_WINDOW, OnActivateWindow) 
	ON_MESSAGE(WM_SEL_TAB, OnSelTab) 
	ON_NOTIFY(TTN_NEEDTEXT, 0, OnTabNeedText)
	//ON_NOTIFY_EX( TTN_NEEDTEXT, 0, memberFxn )
	ON_COMMAND_RANGE(ID_TOOLS_SETBACKGROUNDPIC_NONE, ID_TOOLS_SETBACKGROUNDPIC_NONE, OnToolsSetbackgroundpic)
	ON_COMMAND_RANGE(0xe00, 0xfff, OnToolsSetbackgroundpic)
	ON_MESSAGE(WM_UPDATE_TOOLBAR, OnUpdateToolbar) 
	ON_COMMAND_RANGE(0x6320, 0x6700, OnChangeProxy)
	ON_COMMAND_RANGE(0x1200, 0x127F, OnChangeWebProxy)
	ON_COMMAND_RANGE(0x1100, 0x11FF, OnChangeWebProxy)
	ON_COMMAND_RANGE(0x1280, 0x137F, OnMostFav)
	ON_COMMAND_RANGE(0x6220, 0x631F, OnAddThisFav)
	ON_MESSAGE(WSM_TRAY, OnNotifyTray)
	ON_COMMAND_RANGE(ID_F_2, ID_F_12, OnF2)
	ON_WM_MENUSELECT()
	ON_UPDATE_COMMAND_UI(ID_TAB_PRE, OnUpdateMenu)
	ON_UPDATE_COMMAND_UI(ID_TAB_NEXT, OnUpdateMenu)
	ON_COMMAND_RANGE(ID_C_1, ID_C_0, OnSwitchWins)
	ON_UPDATE_COMMAND_UI(ID_EDIT_EDIT, OnUpdateMenu)
	ON_MESSAGE(WM_HOTKEY, OnHotKey) 
	ON_UPDATE_COMMAND_UI(ID_VIEW_FOLDER, OnUpdateViewExp)
	ON_REGISTERED_MESSAGE(MSWHELL_ROLLMSG, OnScRll) 
	ON_COMMAND_RANGE(ID_TOOLS_AUTOSCROLL_SLOW, ID_TOOLS_AUTOSCROLL_FAST, OnToolsAutoscrollSpeed)
	ON_COMMAND(ID_VIEW_STOP, OnViewStop)
	ON_COMMAND(ID_VIEW_REFRESH, OnViewRefresh)
	ON_COMMAND(ID_FILE_PRINTPREVIEW, OnFilePrintpreview)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND_RANGE(0x6001, 0x60ff, OnOpenGroup)
	ON_COMMAND_RANGE(0x6101, 0x61ff, OnAddtoGroup)
	ON_COMMAND_RANGE(0xc00, 0xcff, OnWindowList)
	ON_COMMAND_RANGE(0x6200, 0x620F, OnUndoList)
	ON_MESSAGE(WM_DELAYLOAD_CONFIG, OnDelayLoadConfig) 
END_MESSAGE_MAP()

//0xc00 - 0xcff   widowlist
//0xd00- 0xdfo	utilities
//0xe00 - 0xfff   background
//0x1000- 0x10ff	open all favorites
//0x1100 - 0x11FF	translation
//0x1200 - 0x127F web proxy
//0x1280 - 0x137F open all most favorites
//0x1380 - 0x6000	favorites
//0x6001 - 0x60ff groups
//0x6101 - 0x61ff add to groups
//0x6200 - 0x620F undo list
//0x6220 - 0x631F add to this favorite folder
//0x6320 - 0x6700 proxy

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_PROCESS,
	ID_INDICATOR_TOOLS,
	ID_INDICATOR_SECU,
	ID_INDICATOR_IP,
	ID_INDICATOR_BP,
	ID_INDICATOR_RES,
	ID_INDICATOR_WINS,
};

UINT toolbarbuttons[]=
{
	ID_NEWWINDOW,
	0,
	ID_GO_BACK,
	ID_GO_FORWARD,
	ID_VIEW_STOP,
	ID_VIEW_REFRESH,
	ID_GO_START_PAGE,
	0,
	ID_VIEW_EXPLOREBAR_SEARCH,
	ID_FAVORITES_DROPDOWN,
	ID_VIEW_HIS,
	ID_VIEW_FOLDER,
	ID_VIEW_EXPLOREBAR_RESOURCE,
	0,
	ID_BMODE_DROPDOWN,
	ID_FILE_PRINT,
	ID_TOOLS_EMAIL,
	ID_FONT_DROPDOWN,
	ID_ENCODE_DROPDOWN,
	ID_OPTIONS_LOADIMAGE,
	ID_VIEW_FULLSCREEN,
	ID_WINDOW_TILE_VERT,
	ID_WINDOW_TILE_HORZ,
	ID_OPTIONS_USE_WEBP,
	ID_OPTIONS_USE_PROXY,
	ID_FILE_WORKOFFLINE,
	0,
	ID_FILE_OPENF,
	ID_EDIT_EDIT,
	ID_FILE_STOP_OPENALL,
	ID_TOOLS_AUTOSCROLL_USEAUTOSCROLLING,
	ID_FILE_SAVEAS,
	ID_EDIT_UNDOCLOSE,
	ID_EDIT_COPY,
	ID_OPTIONS_MYIEOPTIONS,
	ID_VIEW_STOP_ALL,
	ID_VIEW_REFRESHALL,
	ID_TOOLS_SETBG,
	ID_GROUP,
	ID_EDIT_FIND,
	ID_OPTIONS_NEWTAB,
	ID_FILE_PRINTPREVIEW,
	ID_TOOLS_ALL,
	ID_OPTIONS_SET_PROXY,
};

UINT toolbarlabels[]=
{
	IDS_NEW_WINDOW,
	0,
	IDS_BACK,
	IDS_FORWARD,
	IDS_STOP,
	IDS_REFRESH,
	IDS_HOME,
	0,
	IDS_SEARCH,
	IDS_FAVORITES,
	IDS_TITLE_HIS,
	IDS_TITLE_SHELL,
	IDS_TITLE_RESOURCE,
	0,
	IDS_ROWSE_MODE,
	IDS_PRINT,
	ID_TOOLS_EMAIL,
	IDS_FONT,
	IDS_ENCODE,
	IDS_IMAGE,
	ID_VIEW_FULLSCREEN,
	IDS_VTILE,
	IDS_HTILE,
	IDS_WEB_PROXY,
	IDS_PROXY,
	ID_FILE_WORKOFFLINE,
	0,
	ID_FILE_OPENF,
	ID_EDIT_EDIT,
	ID_FILE_STOP_OPENALL,
	IDS_AUTO_SCROLL,
	IDS_SAVE_AS,
	IDS_UNDO_CLOSE,
	ID_EDIT_COPY,
	ID_OPTIONS_MYIEOPTIONS,
	ID_VIEW_STOP_ALL,
	ID_VIEW_REFRESHALL,
	ID_TOOLS_SETBG,
	ID_GROUP,
	ID_EDIT_FIND,
	ID_OPTIONS_NEWTAB,
	ID_FILE_PRINTPREVIEW,
	ID_TOOLS_ALL,
	ID_OPTIONS_SET_PROXY,
};

static UINT toolbarbuttonstyles[] = 
{
	TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
	TBBS_SEPARATOR,
	TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
	TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
	TBSTYLE_BUTTON,
	TBSTYLE_BUTTON,
	TBSTYLE_BUTTON,
	TBBS_SEPARATOR,
	TBSTYLE_BUTTON,
	TBSTYLE_BUTTON,
	TBSTYLE_BUTTON,
	TBSTYLE_BUTTON,
	TBSTYLE_BUTTON,
	TBBS_SEPARATOR,
	TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
	TBSTYLE_BUTTON,
	TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
	TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
	TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
	TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
	TBSTYLE_BUTTON,
	TBSTYLE_BUTTON,
	TBSTYLE_BUTTON,
	TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
	TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
	TBSTYLE_BUTTON,
	TBBS_SEPARATOR,
	TBSTYLE_BUTTON,
	TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
	TBSTYLE_BUTTON,
	TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
	TBSTYLE_BUTTON,
	TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
	TBSTYLE_BUTTON,
	TBSTYLE_BUTTON,
	TBSTYLE_BUTTON,
	TBSTYLE_BUTTON,
	TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
	TBSTYLE_BUTTON,
	TBSTYLE_BUTTON,
	TBSTYLE_BUTTON,
	TBSTYLE_BUTTON,
	TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
	TBSTYLE_BUTTON,
};

UINT toolbarbuttonimgs[] = 
{
	9,0,0,1,2,3,4,0,5,6,12,23,24,0,25,7,13,8,14,10,11,15,16,17,18,19,0,21,20,22,26,28,27,29,30,31,32,33,34,35,36,37,38,39,
};

UINT toolbarselectivelabels[]=
{
	0,0,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	nTotalTabs = 0;
	m_pwndExpObj = NULL;
	m_bFullScreenMode = FALSE;
	m_bFavDirty=FALSE;
	m_bMultiLineTab = FALSE;
	m_strLastIP = "127.0.0.1";
	m_bmBGPic = NULL;
//	m_bWin9x = TRUE;
	m_bNoModemSpeed = TRUE;
	m_bRemPsw = TRUE;
	m_nProtectNum = 0;
	m_nTimerID = 0;
	m_nUpdateTimer = 0;
	m_bRemPsw = TRUE;
	m_hEdit = NULL;
	undoStart = 0;
	undoEnd = 0;
	hResInst = NULL;
	nDelay = 0;
	m_nNeedIB = 0;
	m_bIsSecure = m_bLastIsSecure = FALSE;
	m_bFBTab = TRUE;
	m_DispBP = FALSE;
	m_bNeedUpdProxy = TRUE;
	m_bAllScroll = FALSE;

	Expstr1.LoadString(IDS_TITLE_SEARCH);
	Expstr2.LoadString(IDS_TITLE_HIS);
	Expstr3.LoadString(IDS_TITLE_FAVORITE);
	Expstr4.LoadString(IDS_TITLE_SHELL);
	Expstr5.LoadString(IDS_TITLE_RESOURCE);

	m_bSaveConfig = FALSE;
	nWin=0;

	m_nSwitch = 0;

	CString str="yes";
	bstryes = str.AllocSysString();

//	m_tDBClick.LowPart = 0;
//	dbtime=(float)GetDoubleClickTime()/1000;
	m_nScrollWidth = GetSystemMetrics(SM_CXVSCROLL);
	m_nScrollArrowH = GetSystemMetrics(SM_CYVSCROLL);

	pCollectDlg = NULL;
	pLinkListDlg = NULL;
}

CMainFrame::~CMainFrame()
{
	try{
	m_astrFavoriteFolders.RemoveAll();
//	m_astrFavoriteURLs.RemoveAll();
//	m_astrPopup.RemoveAll();
//	m_lstrUtilCmds.RemoveAll();
//	m_lstrUtilNames.RemoveAll();
	m_ExternalUtilList.RemoveAll();

//	m_astrProxy.RemoveAll();
//	m_astrWebProxy.RemoveAll();
	m_astrBGPics.RemoveAll();

	m_aProxyCategories.RemoveAll();

	if(m_pwndExpObj)
		delete m_pwndExpObj;

	if(m_bmBGPic)
		DeleteObject(m_bmBGPic);

	if(m_nTimerID)
		KillTimer(m_nTimerID);

	if(m_nAnimateIconTimer)
		KillTimer(m_nAnimateIconTimer);

	SysFreeString(bstryes);

	pMainFrame = NULL;

	}catch(...)
	{
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
//	ReleaseMutex(m_hMutex);
    if (!AfxOleInit()) {
         TRACE("Fail in OLE initialating\n");
        }


	CImageList img;
	CString str;
	CWinApp* app;
	app = AfxGetApp();

	::SetProp(m_hWnd, app->m_pszAppName, (HANDLE)1);
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//-----2.5 create BHO for download tools
	/* create the BHO when initialize a web browser
	The BHO has two main functions:
		IObjectWithSiteImpl::SetSite(IUnknown*); pass the IWebBrowser2 to this function.
		IDispatchImpl::Invoke(...); pass the Events to the BHO through this function.
	*/

	//load download tools
	m_DList.LoadDownloadTools();

	CDownloadTool* pdl;

	pDownload = NULL;

	CString tmpstrDefaultDLT = app->GetProfileString("DownloadTool", "DefaultTool", "FlashGet");
	m_strDownloadFileExt = app->GetProfileString("DownloadTool", "FileExt", ".ZIP;.EXE;.BIN;.TGZ;.GZ;.Z;.TAR;.ARJ;.LZH;.MP3;.IMP;.COM;.RAR;.ACE;.RM;.A??");

	if(tmpstrDefaultDLT!="")
	{
		int i = m_DList.GetIDFromName(tmpstrDefaultDLT);
		if(i>=0)
		{
			pdl = m_DList.m_DownloadTools.GetAt(i);
			if(!SetDownloadTool(pdl->ProgID))
				m_strDefaultDLT = "";
			else
				m_strDefaultDLT = tmpstrDefaultDLT;
		}
		else
			m_strDefaultDLT = "";
	}

	//read backgorund pic name
	m_strBGPic = app->GetProfileString("Settings", "Background", "");

//	CBitmap    bitmap;
	CRect rectToolBar;

//========create main menu

	CMenuBar& mb = m_wndMenuBar;
	VERIFY(mb.CreateEx(this));
	mb.ModifyStyle(0, TBSTYLE_TRANSPARENT|TBSTYLE_FLAT| TBSTYLE_LIST);
	mb.LoadMenu(IDR_MAINFRAME);
//	bitmap.LoadBitmap(IDB_BITMAP1);
	HBITMAP hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP1));
	m_wndMenuBar.m_ImgList.Create(16, 16, ILC_COLORDDB|ILC_MASK, 7, 1);
	ImageList_AddMasked(m_wndMenuBar.m_ImgList.GetSafeHandle(), hbmp, RGB(255,0,255));
//	bitmap.Detach();
	GetMenu()->CheckMenuItem(ID_VIEW_MENUBAR, MF_CHECKED);
	//check menu
	if(GetMenu()->GetSubMenu(8)->GetMenuItemID(3)!=ID_HELP_HOMEPAGE)
		return -1;
	GetMenu()->GetSubMenu(8)->EnableMenuItem(ID_HELP_HOMEPAGE, MF_ENABLED);
//============


//============sysmenu
	if (!m_SysMenuBar.CreateEx(this))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	
	//load win buttons
	char key[5]="W";
	for(int i=0; i<6; i++)
	{
		itoa(i, key+1, 10);
		m_abWinButtons[i] = app->GetProfileInt("WinButtons", key, 1);
	}

	hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP6));
	img.Create(15, 15, ILC_COLORDDB|ILC_MASK, 6, 1);
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
	m_SysMenuBar.GetToolBarCtrl().SetImageList(&img);
	img.Detach();


/*	m_SysMenuBar.SetButtonInfo(0, ID_TAB_PRE, TBSTYLE_BUTTON, 5);
	m_SysMenuBar.SetButtonInfo(1, ID_TAB_NEXT, TBSTYLE_BUTTON, 4);
	m_SysMenuBar.SetButtonInfo(2,0,TBBS_SEPARATOR,0);
	m_SysMenuBar.SetButtonInfo(3, ID_FILE_CLOSEALL, TBSTYLE_BUTTON, 3);
	m_SysMenuBar.SetButtonInfo(4,0,TBBS_SEPARATOR,0);
	m_SysMenuBar.SetButtonInfo(5, ID_WINDOW_MIN, TBSTYLE_BUTTON, 0);
	m_SysMenuBar.SetButtonInfo(6, ID_WINDOW_RESTOR, TBSTYLE_BUTTON, 1);
	m_SysMenuBar.SetButtonInfo(7, ID_FILE_CLOSE_1, TBSTYLE_BUTTON, 2);*/

	m_SysMenuBar.SetBarStyle(m_SysMenuBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED);

//=============


//=============link bar
	if (!m_LinkBar.CreateEx(this))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
//	bitmap.LoadBitmap(IDB_BITMAP1);
	img.Create(16, 16, ILC_COLORDDB|ILC_MASK, 7, 1);
	hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP1));
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));

	m_LinkBar.GetToolBarCtrl().SetImageList(&img);
	img.Detach();
//	bitmap.Detach();

	m_LinkBar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | TBSTYLE_LIST|TBSTYLE_TOOLTIPS|CCS_NORESIZE );

	m_LinkBar.SetBarStyle(m_LinkBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	//add open all links
	int iString;
	TBBUTTON tbb;
	str.LoadString(IDS_LINKS);
	memset(&tbb, 0, sizeof(tbb));
	iString = m_LinkBar.GetToolBarCtrl().AddStrings(str);
	tbb.iString = iString;
	tbb.fsState = TBSTATE_ENABLED;
	tbb.fsStyle = TBSTYLE_AUTOSIZE;
	tbb.iBitmap = 3;
	tbb.idCommand = ID_TOOLS_OPENALLLINKS;
	VERIFY(m_LinkBar.GetToolBarCtrl().AddButtons(1, &tbb));
//================


//================Favorite bar
	//compact fav bar
	bCompactFavBar = app->GetProfileInt("Settings", "CompactFavBar", FALSE);

	if (!m_FavBar.CreateEx(this))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if(!bCompactFavBar)
	{
//		bitmap.LoadBitmap(IDB_BITMAP1);
		img.Create(16, 16, ILC_COLORDDB|ILC_MASK, 7, 1);
		hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP1));
		ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
		m_FavBar.GetToolBarCtrl().SetImageList(&img);
		img.Detach();
//		bitmap.Detach();
	}
	else
		m_FavBar.GetToolBarCtrl().SetImageList(NULL);


	m_FavBar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | TBSTYLE_LIST|TBSTYLE_TOOLTIPS );

	m_FavBar.SetBarStyle(m_FavBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
//===================

	//max menu size
	m_nMenuWidth = app->GetProfileInt("Settings", "MenuSize", 26);
	m_bDisplayOpenAll = app->GetProfileInt("Settings", "DisplayOpenAll", TRUE);
	m_bDisplayAddFav = app->GetProfileInt("Settings", "DisplayAddFav", TRUE);

	//single column menu
	bSingleMenu = app->GetProfileInt("Settings", "SingleMenu", FALSE);

	// first get rid of bogus submenu items.
	InitFavorites();

	m_LinkBar.GetToolBarCtrl().GetToolTips()->UpdateTipText(ID_TOOLS_OPENALLLINKS, &m_LinkBar, ID_TOOLS_OPENALLLINKS);

	GetMenu()->CheckMenuItem(ID_VIEW_LINKS, MF_CHECKED);

	GetMenu()->CheckMenuItem(ID_VIEW_FAVS, MF_CHECKED);


	
//============Tool Bar
	if (!m_wndToolBar.CreateEx(this))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolBar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT |TBSTYLE_TOOLTIPS );
	m_wndToolBar.ModifyStyleEx(0, WS_EX_TOOLWINDOW);
	m_wndToolBar.GetToolBarCtrl().SetButtonWidth(22, 150);
	m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);


	// Create and set the hot toolbar image list.
	m_bSmallToolBar = app->GetProfileInt("Settings", "SmallToolbar", 1);

//====================

//===============AddressBar
	if (!m_AddressBar.CreateEx(this))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
//	bitmap.LoadBitmap(IDB_BITMAP7);
	img.Create(16, 16, ILC_COLORDDB|ILC_MASK, 1, 1);
	hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP7));
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
	m_AddressBar.SendMessage(TB_SETIMAGELIST, 0, (LPARAM)img.m_hImageList);
	img.Detach();
//	bitmap.Detach();
//	bitmap.LoadBitmap(IDB_BITMAP8);
	/*3.1 b454
	img.Create(16, 15, ILC_COLORDDB|ILC_MASK, 1, 1);
	hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP8));
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
	m_AddressBar.SendMessage(TB_SETHOTIMAGELIST, 0, (LPARAM)img.m_hImageList);
	img.Detach();*/
//	bitmap.Detach();

	m_AddressBar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT |TBSTYLE_TOOLTIPS );
	m_AddressBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);

	m_AddressBar.SetButtons(NULL, 2);
	m_AddressBar.SetButtonInfo(0, 0, TBBS_SEPARATOR, 0);
	m_AddressBar.SetButtonInfo(1, IDOK, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, 0);
	m_AddressBar.GetToolBarCtrl().GetToolTips()->UpdateTipText("Go", &m_AddressBar, IDOK);

	m_AddressBar.Init();

	//get the reference to the comboboxex.
	m_wndAddress = m_AddressBar.GetAddressBox();
	GetMenu()->CheckMenuItem(ID_VIEW_ADDRESSBAR, MF_CHECKED);


	//load typed URLs
	LoadTypedURL();
//=======================


//===============external tool bar
	if (!m_ExternalTool.CreateEx(this))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	GetMenu()->CheckMenuItem(ID_VIEW_EX_TOOL, MF_CHECKED);
	m_ExternalTool.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | TBSTYLE_LIST|TBSTYLE_TOOLTIPS );

	m_ExternalTool.SetBarStyle(m_ExternalTool.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	SHFILEINFO shFinfo;
	HIMAGELIST hImgList = NULL;

	hImgList = (HIMAGELIST)SHGetFileInfo( "C:\\",
							  0,
							  &shFinfo,
							  sizeof( shFinfo ),
							  SHGFI_SYSICONINDEX | 
							  SHGFI_SMALLICON );
	if ( !hImgList )
	{
		return FALSE;
	}

	m_ExternalTool.SendMessage(TB_SETIMAGELIST, 0, (LPARAM)hImgList);

	//build external toolbar
	//get Utils name list
	CString wp;
//	char * tmp;
	CExternalUtilItem* eui;
	wp = app->GetProfileString("ExUtils", "name1", NULL);
	if(wp == "" || wp.IsEmpty())
	{
/*3.2		tmp = NULL;
		UINT len;

		BOOL r = app->GetProfileBinary("ExUtils","names", (LPBYTE*)(&tmp), &len);
		if(r)
		{
			//load name list
			char *token;
			char seps[] = "\n";
			token = strtok( tmp, seps);
			while( token != NULL )
			{
				eui = new CExternalUtilItem;
				eui->m_strUtilName = token;
				  /* While there are tokens in "string" * /
				m_ExternalUtilList.m_UtilList.Add(eui);
				  /* Get next token: * /
				token = strtok( NULL, seps );
			}
			delete[] tmp;

		}
		//get Utils command list
		tmp = NULL;
		r = app->GetProfileBinary("ExUtils","cmds", (LPBYTE*)(&tmp), &len);
		if(r)
		{
			//load pop list
			char *token;
			char seps[] = "\n";
			int i = 0;
			token = strtok( tmp, seps);
			while( token != NULL )
			{
				  /* While there are tokens in "string" * /
				m_ExternalUtilList.m_UtilList.GetAt(i)->m_strUtilCmd = token;
				  /* Get next token: * /
				 token = strtok( NULL, seps );

				  //add start up
				m_ExternalUtilList.m_UtilList.GetAt(i)->m_bUtilStart = FALSE;
				m_ExternalUtilList.m_UtilList.GetAt(i)->m_bUtilClose = FALSE;
				 i++;
			}
			delete[] tmp;

		}*/
	}
	else
	{
		//new format
		char Keyname1[10] = "name", Keyname2[10] = "cmds", Keyname3[10] = "sta", Keyname4[10] = "cls";
		CString cmd;
		BOOL bstart, bClose;
		int i=1;
		cmd = app->GetProfileString("ExUtils", "cmds1", NULL);
		bstart = app->GetProfileInt("ExUtils", "sta1", FALSE);
		bClose = app->GetProfileInt("ExUtils", "cls1", bstart);
		while(wp != "" && !wp.IsEmpty())
		{
			eui = new CExternalUtilItem;
			eui->m_strUtilName = wp;
			eui->m_strUtilCmd = "\""+cmd+"\"";
			eui->m_bUtilStart = bstart;
			eui->m_bUtilClose = bClose;
			m_ExternalUtilList.m_UtilList.Add(eui);
			i++;
			itoa(i, Keyname1+4, 10);
			itoa(i, Keyname2+4, 10);
			itoa(i, Keyname3+3, 10);
			itoa(i, Keyname4+3, 10);
			wp = app->GetProfileString("ExUtils", Keyname1, NULL);
			cmd = app->GetProfileString("ExUtils", Keyname2, NULL);
			bstart = app->GetProfileInt("ExUtils", Keyname3, FALSE);
			bClose = app->GetProfileInt("ExUtils", Keyname4, bstart);
		}
	}

	BuildUtilMenu();
// ======================OK

// ==============Groups ==========
	InitGroups();


//==========create rebar
	m_bLockToolbar = app->GetProfileInt("Settings", "LockToolbar", FALSE);

	if (!m_wndReBar.Create(this,RBS_BANDBORDERS|RBS_DBLCLKTOGGLE | RBS_REGISTERDROP |RBS_VARHEIGHT ,CCS_NODIVIDER |CCS_NOPARENTALIGN |WS_BORDER |WS_CHILD |WS_CLIPCHILDREN |WS_CLIPSIBLINGS |WS_VISIBLE | CBRS_TOP))
	{
		return -1;      // fail to create
	}

//========add to rebar
	m_wndReBar.AddBar(&m_wndMenuBar, NULL,NULL, RBBS_FIXEDBMP | RBBS_BREAK |RBBS_GRIPPERALWAYS|RBBS_USECHEVRON);
	m_wndReBar.AddBar(&m_SysMenuBar, NULL, NULL, RBBS_FIXEDBMP |RBBS_GRIPPERALWAYS);
	m_wndReBar.AddBar(&m_wndToolBar, NULL,NULL, RBBS_FIXEDBMP | RBBS_BREAK |RBBS_GRIPPERALWAYS|RBBS_USECHEVRON);
	str.LoadString(IDS_ADDRESS);
	m_wndReBar.AddBar(&m_AddressBar, str,NULL, RBBS_FIXEDBMP | RBBS_BREAK |RBBS_GRIPPERALWAYS );


	// set up min/max sizes and ideal sizes for pieces of the rebar
	REBARBANDINFO rbbi;
	
	m_wndMenuBar.GetItemRect(1, &rectToolBar);
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE|RBBIM_ID;//|RBBIM_STYLE
	rbbi.wID = ID_MENU_BAR;
	rbbi.cxMinChild = rectToolBar.Width();
	rbbi.cyMinChild = rectToolBar.Height();
	m_wndMenuBar.GetToolBarCtrl().GetItemRect(m_wndMenuBar.GetToolBarCtrl().GetButtonCount()-1,&rectToolBar);
	/*rbbi.cx =*/ rbbi.cxIdeal = rectToolBar.right;
	m_wndReBar.GetReBarCtrl().SetBandInfo(0, &rbbi);

	//system menu
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_ID ;//|RBBIM_STYLE
	rbbi.wID = ID_SYSMENU_BAR;
	m_wndReBar.GetReBarCtrl().SetBandInfo(1, &rbbi);
	BuildWinButtons();

	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_ID ;//|RBBIM_STYLE
	rbbi.wID = ID_VIEW_TOOLBAR;
	m_wndReBar.GetReBarCtrl().SetBandInfo(2, &rbbi);
	m_nTextLabel = app->GetProfileInt("Settings", "TextLabel", 0);
	m_bColorIcon = app->GetProfileInt("Settings", "ColorIcon", TRUE);
	InitialToolBar();
	rbbi.cxMinChild = 0;

	CRect rectAddress;

	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE| RBBIM_ID;
	rbbi.wID = ID_VIEW_ADDRESSBAR;
	m_wndAddress->GetWindowRect(&rectAddress); //GetEditCtrl()->GetWindowRect(&rectAddress);
	rbbi.cyMinChild = rectAddress.Height();
	rbbi.cxMinChild = 20;
/*	rbbi.cx =*/ rbbi.cxIdeal = 20;
	m_wndReBar.GetReBarCtrl().SetBandInfo(3, &rbbi);


	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetProgressBarWidth(80);
	m_wndStatusBar.SetRange(0,100);
	m_wndStatusBar.SetPaneStyle(ID_SEPARATOR, SBPS_STRETCH);
	m_wndStatusBar.SetPaneInfo(3, ID_INDICATOR_SECU , SBPS_NORMAL, 10);
	if(app->GetProfileInt("Bands", "Status", 1)==0)
		m_wndStatusBar.ShowWindow(SW_HIDE);
	m_hSecuIcon = (HICON)LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_SECURE),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	UINT sbstyle, sbID;
	m_wndStatusBar.GetPaneInfo(4, sbID, sbstyle, m_SBIPWidth);
	m_wndStatusBar.GetPaneInfo(6, sbID, sbstyle, m_SBMemWidth);
	m_wndStatusBar.SetPaneText(4, "");
	m_wndStatusBar.SetPaneText(6, "");
	m_wndStatusBar.SetPaneText(2, "");
	m_wndStatusBar.SetPaneInfo(2, ID_INDICATOR_TOOLS , SBPS_NORMAL, m_wndStatusBar.m_nToolbarWidth-4);

	//display ip
	m_bDisplayIP = app->GetProfileInt("Settings", "DisplayIP", 0);

	//display mem
	m_bDispMem = app->GetProfileInt("Settings", "DisplayMem", 1);

	if(!m_bDisplayIP)
		m_wndStatusBar.SetPaneInfo(4, ID_INDICATOR_IP, SBPS_NORMAL, 0);
	if(!m_bDispMem)
		m_wndStatusBar.SetPaneInfo(6, ID_INDICATOR_RES, SBPS_NORMAL, 0);


	//link bar
	// set up toolbar button sizes
	m_LinkBar.GetItemRect(0, &rectToolBar);

	m_wndReBar.AddBar(&m_LinkBar, NULL,NULL, RBBS_FIXEDBMP | RBBS_BREAK |RBBS_GRIPPERALWAYS|RBBS_USECHEVRON);

	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE |RBBIM_ID ;
	rbbi.wID = ID_VIEW_LINKS;
	rbbi.cxMinChild = rectToolBar.Width()+2;
	rbbi.cyMinChild = 22;
	m_LinkBar.GetToolBarCtrl().GetItemRect(m_LinkBar.GetToolBarCtrl().GetButtonCount()-1,&rectToolBar);
	/*rbbi.cx =*/ rbbi.cxIdeal = rectToolBar.right;
	m_wndReBar.GetReBarCtrl().SetBandInfo(4, &rbbi);

	//Favorate bar
	// set up toolbar button sizes
	m_FavBar.GetItemRect(0, &rectToolBar);

	str.LoadString(IDS_FAVORITES);
	m_wndReBar.AddBar(&m_FavBar, str,NULL, RBBS_FIXEDBMP | RBBS_BREAK |RBBS_GRIPPERALWAYS|RBBS_USECHEVRON);

	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE |RBBIM_ID ;
	rbbi.wID = ID_VIEW_FAVS;
	rbbi.cxMinChild = rectToolBar.Width()+2;
	rbbi.cyMinChild = 22;
	m_FavBar.GetToolBarCtrl().GetItemRect(m_FavBar.GetToolBarCtrl().GetButtonCount()-1,&rectToolBar);
	/*rbbi.cx =*/ rbbi.cxIdeal = rectToolBar.right;
	m_wndReBar.GetReBarCtrl().SetBandInfo(5, &rbbi);

	//exteranl tool bar
	// set up toolbar button sizes
	m_ExternalTool.GetItemRect(0, &rectToolBar);

	str.LoadString(IDS_EX_TOOL);
	m_wndReBar.AddBar(&m_ExternalTool, str,NULL, RBBS_FIXEDBMP | RBBS_BREAK |RBBS_GRIPPERALWAYS|RBBS_USECHEVRON);

	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE |RBBIM_ID ;
	rbbi.wID = ID_VIEW_EX_TOOL;
	rbbi.cxMinChild = rectToolBar.Width()+2;
	rbbi.cyMinChild = 22;
	m_ExternalTool.GetToolBarCtrl().GetItemRect(m_ExternalTool.GetToolBarCtrl().GetButtonCount()-1,&rectToolBar);
	/*rbbi.cx =*/ rbbi.cxIdeal = rectToolBar.right;
	m_wndReBar.GetReBarCtrl().SetBandInfo(6, &rbbi);

	
	//create bottom bar
	if (!m_wndReBar2.Create(this,RBS_BANDBORDERS,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS| WS_CLIPCHILDREN | CBRS_BOTTOM))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

//add tab
	//multiline tab
	m_bMultiLineTab = app->GetProfileInt("Settings", "MultiLineTab", 1);
	m_bAutoTab = app->GetProfileInt("Settings", "AutoTab", 1);
	m_bFBTab = app->GetProfileInt("Settings", "FBTab", TRUE);
	//Tab bar
	//Create Tab Bar
	CRect rect(0,0,300,19);
	m_wndTab.m_bTop = m_bTopTab;
	DWORD dwStyle = TCS_BUTTONS|TCS_FLATBUTTONS|TCS_RAGGEDRIGHT;
	if(!m_bFBTab)
		if(!m_bTopTab)
			dwStyle = TCS_BOTTOM|WS_CHILD|TCS_OWNERDRAWFIXED;
		else
			dwStyle = WS_CHILD|TCS_OWNERDRAWFIXED;
	if(m_bMultiLineTab)
	{
		if(!m_bAutoTab)
			m_wndTab.Create(WS_VISIBLE|dwStyle|TCS_FOCUSNEVER|TCS_FORCELABELLEFT|TCS_TOOLTIPS|TCS_MULTILINE, rect,this, AFX_IDW_TOOLBAR + 2);
		else
			m_wndTab.Create(WS_VISIBLE|dwStyle|TCS_FOCUSNEVER|TCS_FORCELABELLEFT|TCS_TOOLTIPS|TCS_MULTILINE|TCS_FIXEDWIDTH, rect,this, AFX_IDW_TOOLBAR + 2);
		GetMenu()->CheckMenuItem(ID_OPTION_MULTI_TAB, MF_CHECKED);
	}
	else if(!m_bAutoTab)
		m_wndTab.Create(WS_VISIBLE|dwStyle|TCS_FOCUSNEVER|TCS_FORCELABELLEFT|TCS_TOOLTIPS, rect,this, AFX_IDW_TOOLBAR + 2);
	else
		m_wndTab.Create(WS_VISIBLE|dwStyle|TCS_FOCUSNEVER|TCS_FORCELABELLEFT|TCS_TOOLTIPS|TCS_FIXEDWIDTH , rect,this, AFX_IDW_TOOLBAR + 2);
	DWORD dwExStyle= m_wndTab.GetExtendedStyle();
	m_wndTab.SetExtendedStyle(dwExStyle  | TCS_EX_FLATSEPARATORS);
	img.Create(11, 9, ILC_COLORDDB|ILC_MASK, 8, 1);
	hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP5));
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
	m_TabImgList.Create(11, 9, ILC_COLORDDB|ILC_MASK, 8, 1);
	hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP5));
	ImageList_AddMasked(m_TabImgList.GetSafeHandle(), hbmp, RGB(255,0,255));
	m_wndTab.SetImageList(&img);
	img.Detach();
	m_wndTab.pImageList = &m_TabImgList;

	CSize size;
	if(m_bFBTab)
	{
		size.cx = FBPADDINGX;
		size.cy = FBPADDINGY;
	}
	else
	{
		size.cx = TABPADDINGX;
		size.cy = TABPADDINGY;
	}
	m_wndTab.SetPadding( size );
	//m_wndTab.SetItemSize(size);
	m_wndTab.SetFont(m_wndReBar.GetFont());
	GetMenu()->CheckMenuItem(ID_VIEW_TASKBAR, MF_CHECKED);
	if(!m_bFBTab)
		m_wndTab.SetFonts();

	// set up min/max sizes and ideal sizes for pieces of the rebar

	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE  |RBBIM_ID;
	rbbi.wID = ID_VIEW_TASKBAR;
	rbbi.cxMinChild = 0;
//	m_wndAddress->GetEditCtrl()->GetWindowRect(&rectAddress);
	m_wndTab.GetWindowRect(&rectAddress);
	rbbi.cyMinChild = rectAddress.Height();
	rbbi.cxIdeal = 200;

	//add tab to top or bottom
	if(m_bTopTab)
	{
		m_wndReBar.AddBar(&m_wndTab,NULL, NULL, RBBS_FIXEDBMP | RBBS_BREAK |RBBS_GRIPPERALWAYS );
		m_wndReBar.GetReBarCtrl().SetBandInfo(7, &rbbi);
		m_wndTab.ModifyStyle(0, CCS_NORESIZE);
	}
	else
	{
		m_wndReBar2.AddBar(&m_wndTab);
		if(!m_bFBTab)
		{
			rbbi.fMask |=  RBBIM_STYLE;
			rbbi.fStyle = RBBS_FIXEDBMP | RBBS_NOGRIPPER; 
		}
		m_wndReBar2.GetReBarCtrl().SetBandInfo(0, &rbbi);
	}

	//load band position
	char temp[7]="band", rectstr[10]="bandrc";
	RECT *pbandrect;//, realrect
	UINT bands;
	i=-1;
	UINT len = 0;
	DWORD pos;
	BOOL show;
	i=-1;
	do
	{
		i++;
		itoa(i, temp+4, 10);
		bands = app->GetProfileInt("Settings", temp, 0);
		show = app->GetProfileInt("Bands", temp, 1);
		itoa(i, rectstr+6,10);
		if(bands!=0 && show)
		{
			BOOL r = app->GetProfileBinary("Settings", rectstr, (LPBYTE*)&pbandrect, &len);
			if(r)
			{
				//if(bands!=ID_VIEW_FAVS)
				{
					int index=m_wndReBar.GetReBarCtrl().IDToIndex(bands);
					//m_wndReBar.GetReBarCtrl().GetRect(index, &realrect);
					//if(realrect.left != pbandrect->left || realrect.top != pbandrect->top)
					{
						rbbi.fMask = RBBIM_SIZE;
						rbbi.cx=pbandrect->right-pbandrect->left;
						m_wndReBar.GetReBarCtrl().SetBandInfo(index, &rbbi);

						index=m_wndReBar.GetReBarCtrl().IDToIndex(bands);
						m_wndReBar.GetReBarCtrl().BeginDrag(index);
						pos=((pbandrect->top+8)<<16)+pbandrect->left;
						m_wndReBar.GetReBarCtrl().DragMove(pos);
						m_wndReBar.GetReBarCtrl().EndDrag();
					}
				}
				delete pbandrect;
			}
		}
		else if(bands!=0)
		{
			int index=m_wndReBar.GetReBarCtrl().IDToIndex(bands);
			m_wndReBar.GetReBarCtrl().ShowBand(index, FALSE);
			GetMenu()->CheckMenuItem(bands, MF_UNCHECKED);
		}

	}while(bands!=0);


	//bottom bar
	if(app->GetProfileInt("Bands", "bandb0", 1)==0)
	{
		m_wndReBar2.GetReBarCtrl().ShowBand(0, FALSE);
		GetMenu()->CheckMenuItem(ID_VIEW_TASKBAR, MF_UNCHECKED);

	}
	//fix tb
	if(m_bLockToolbar)
		FixToolbar(TRUE);

	//auto complete
	HINSTANCE hIns = LoadLibrary("shlwapi.dll");
	if(hIns != NULL)
	{
		LPFNDLLFUNC1 lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hIns, "SHAutoComplete");
		if(lpfnDllFunc1!=NULL)
			lpfnDllFunc1(m_wndAddress->GetEditCtrl()->m_hWnd, 0xe);
	
		FreeLibrary(hIns);
	}

	hResInst = LoadLibrary("RSRC32.dll");
	pGetRes = (GETRES) GetProcAddress(hResInst, "_MyGetFreeSystemResources32@4"); 

	ShowBackground(TRUE);

	//2.4 animation title icon
	m_animIcon.SetImageList(IDB_BITMAP11,8,RGB(0,0,0));
	m_nAnimateIconTimer = SetTimer(99, 250, NULL);

	m_wndTab.ModifyStyle(CCS_NORESIZE, 0);

	//get host name
	addr[0]='\0';
	gethostname(addr,255);

	//get homepage
	GetHomePage(m_strHome);


	return 0;
}



/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::SetAddress(LPCTSTR lpszUrl)
{
	// This is called when the browser has completely loaded the new location,
	// so make sure the text in the address bar is up to date and stop the
	try
	{
	m_wndAddress->SetWindowText(lpszUrl);

	COMBOBOXEXITEM item;

	item.mask = CBEIF_IMAGE|CBEIF_SELECTEDIMAGE ;
	item.iItem = -1;
	item.iImage = 1;
	item.iSelectedImage = 1;
	m_wndAddress->SetItem(&item);
	}
/*	catch(CMemoryException * e)
	{
		if(e!=NULL)e->Delete();
//		AfxMessageBox("setaddress error");
	}*/
	catch(...)
	{
	}
}

void CMainFrame::OnNewAddress()
{
	// gets called when an item in the Address combo box is selected
	// just navigate to the newly selected location.
	CString str;

	try{

	int i = m_AddressBar.m_wndAddress.GetCurSel();
	if(i<0)
		return;

	m_AddressBar.GetSelText(i, str);
	if(m_bSecure)
	{
		int i = str.Find(':');
		CString po = str.Left(i);
		po.MakeLower();
		if(i==1 || po == "file")
			return;
	}

	CChildFrame* tcf = (CChildFrame*)MDIGetActive();

	//decode the escape characters
//	DecodeEscap(str);
	short ks=GetKeyState(VK_CONTROL), ss=GetKeyState(VK_SHIFT);
	if(m_bUse3721 && ks>=0 && ss>=0)
	{
		//use 3721
		if(str.Find(':')<0 && str.Find('.')<0)
		{
			//not a url
			str = "http://cns.3721.com/cns.dll?platform=pc&fl=1&fd=1&name="+str; // www.3721.com/***
		}
	}

	//create new if not exist
	if(tcf==NULL || bAddressInNewWindow)
		tcf = NewChildWindow(0);

	//web proxy
	if (tcf!=NULL && tcf->m_pView!=NULL)
	{
		tcf->m_pView->ToNavigate(str, 0, NULL);

	}
	
	}/*catch(CMemoryException * e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
//		AfxMessageBox("new address error");
	}

}


void CMainFrame::OnNewAddressEnter()
{
	// gets called when an item is entered manually into the edit box portion
	// of the Address combo box.
	// navigate to the newly selected location and also add this address to the
	// list of addresses in the combo box.
	//static CString str;
	CString oldstr, url;
	CString str, str2;
	COMBOBOXEXITEM item;

	try
	{

	//focus
	m_wndAddress->SetFocus();

	m_wndAddress->GetEditCtrl()->GetWindowText(str2);
	str2.TrimLeft();str2.TrimRight();
	if(m_bSecure)
	{
		int i = str2.Find(':');
		CString po = str2.Left(i);
		po.MakeLower();
		if(i==1 || po == "file")
			return;
	}

	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	short ks=GetKeyState(VK_CONTROL), ss=GetKeyState(VK_SHIFT), as=GetKeyState(VK_MENU);
	
	BOOL use3721 = FALSE;

	if(ks>=0 && ss>=0) // only Enter need to expand alias and other shortcuts
	{
		//alisa
		if(m_bUseAlias)
			if(m_mapAlias.Lookup(str2, url))
				str2 = url;
			else
				use3721=TRUE;
		else
			use3721=TRUE;

		//use quick search
		if(use3721 && str2.Find(' ')>0)
		{
			int i = str2.Find(' ');
			CString qs = str2.Left(i);
			CString keyw = str2.Mid(i+1);
			keyw.TrimLeft();
			qs.TrimLeft();qs.TrimRight();
			if(m_QuickSearchMap.Lookup(qs, url))
			{
				str2 = url;
				str2.Replace("%s", keyw);
				use3721 = FALSE;
			}
		}
	}

	if(m_bUse3721 && use3721 && ks>=0 && ss>=0)
	{
		//use 3721
		if(str2.Find(':')<0 && str2.Find('.')<0)
		{
			//not a url
			url = str2;
			//http://cns.3721.com/cns.dll?platform=pc&fl=1&fd=1&name=
			str2 = "http://cns.3721.com/cns.dll?type=cns&fa=1&name="+str2; // www.3721.com/***
		}
		else
			use3721 = FALSE;
	}
	else
		use3721 = FALSE;

	oldstr = str2;


	item.mask = CBEIF_TEXT|CBEIF_IMAGE|CBEIF_SELECTEDIMAGE ;
	item.iItem = 0;

	//format the URL string
	if(!use3721)
	{
		str2.TrimLeft();
		str2.TrimRight();
	}
	int co = str2.Find(':');
	if(co > 1)
		str2.Replace('\\','/');
	if(co < 0)
	{
		if(ks<0 && ss>=0 && as>=0)
			str2 = m_strCE1+str2+m_strCE2;
		else if(ks>=0 && ss<0 && as>=0)
			str2 = m_strSE1+str2+m_strSE2;
		else if(ks<0 && ss<0 && as>=0)
			str2 = m_strCSE1 + str2 + m_strCSE2;
		else
			str2 = "http://"+str2;

		if(ks<0 || ss<0 || as<0)
		{
			oldstr = str2;
			m_wndAddress->GetEditCtrl()->SetWindowText(str2);
		}
	}


	if(use3721)
		str2 = url;
	
	str =str2;


//	short as=GetKeyState(VK_MENU);
	//create new if not exist and always new
	if(tcf==NULL || bAddressInNewWindow || as<0)
		tcf = NewChildWindow(0);

	//web proxy
	if (tcf!=NULL && tcf->m_pView!=NULL)
	{
		tcf->m_pView->ToNavigate(oldstr, 0, NULL);
	}

	if(m_AddressBar.FindStringExact(-1,str) == CB_ERR)
	{
		if (str.Right(1)=='/')
			str = str.Left(str.GetLength()-1);
		else
			str += '/';

		if(m_AddressBar.FindStringExact(-1, str) == CB_ERR)
		{
			//the URL is new
			item.pszText = (LPTSTR)(LPCTSTR)str2;
			item.iImage = 1;
			item.iSelectedImage = 1;
			m_wndAddress->InsertItem(&item);

		}
	}

	}
/*	catch(CMemoryException * e)
	{
		if(e!=NULL)e->Delete();
//		AfxMessageBox("new address enter error");
	}*/
	catch(...)
	{
	}
}

void CMainFrame::DoNothing()
{
	// this is here only so that the toolbar buttons for the dropdown menus
	// will have a callback, and thus will not be disabled.
}


int CMainFrame::BuildFavoritesMenu(LPCTSTR pszPath, int nStartPos, CMenu* pMenu,int nFixMenu, int FirstLevel, int& nFavs, int& nMostFavs)
{
	CString         strPath; 
	CString         strPath2;
	CString         str, str2;
	WIN32_FIND_DATA wfd;
	HANDLE          h;
	int             nPos;
	int             nEndPos;
	int             nLastDir;
//	TCHAR           buf[INTERNET_MAX_PATH_LENGTH];
	CStringArray    astrFavorites;
	CArray<int,int>	anFavID;
	CStringArray    astrDirs;
//	CMenu*          pSubMenu;
	BOOL			isLink = FALSE;
	static CString			strOpenAll, strOpenMost, strAddFav;
	int				nSubFavs = 0, nSubMostFavs = 0;


	try
	{
	strPath = pszPath;

	nFavs = 0; nMostFavs = 0;
	
	strOpenAll.LoadString(IDS_OPEN_ALL_FAV);
	strOpenMost.LoadString(IDS_OPEN_ALL_MOSTFAV);
	strAddFav.LoadString(IDS_ADDFAV);

	// make sure there's a trailing backslash
	if(strPath[strPath.GetLength() - 1] != _T('\\'))
		strPath += _T('\\');
	if(strPath==m_strLinkFolder)
		isLink = TRUE;

	strPath2 = strPath;
	strPath += "*.*";

	nLastDir = 0;
	// now scan the directory, first for .URL files and then for subdirectories
	// that may also contain .URL files
	h = FindFirstFile(strPath, &wfd);
	DWORD fileattr;
	if(m_dwHidden == 1)
		fileattr = FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_SYSTEM;
	else
		fileattr = FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_SYSTEM;
	if(h != INVALID_HANDLE_VALUE)
	{
		nEndPos = nStartPos;
		do
		{
			if((wfd.dwFileAttributes & fileattr)==0)
			{
				str = wfd.cFileName; str2= str;
				str.MakeLower();
				if(str.Right(4) == _T(".url"))
				{
					nPos = nEndPos;
					astrFavorites.Add( str2);
					anFavID.Add(nPos);
				//	m_astrFavoriteURLs.InsertAt(nPos, buf);
					++nEndPos;
					nFavs ++;
				}
				else if(str.Right(4) == _T(".lnk"))
				{
					//if(ResolveLink(strPath2 + str, buf))
					{

						// scan through the array and perform an insertion sort
						// to make sure the menu ends up in alphabetic order
						/*for(nPos = nStartPos ; nPos < nEndPos ; ++nPos)
						{
							if(str.CompareNoCase(astrFavorites[nPos]) < 0)
								break;
						}*/
						nPos = nEndPos;
						//m_astrFavoriteURLs.InsertAt(nPos, buf);
				//		m_astrFavoriteURLs.InsertAt(nPos, strPath2 + str);
						//str2 = str2.Left(str2.GetLength() - 4);
						astrFavorites.Add(str2);
						anFavID.Add(nPos);
						++nEndPos;
						nFavs ++;
					}
				}
			}
			else if((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (m_dwHidden==1 || (wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)==0))
			{
				// ignore the current and parent directory entries
				if(lstrcmp(wfd.cFileName, _T(".")) == 0 || lstrcmp(wfd.cFileName, _T("..")) == 0 )//|| lstrcmp(wfd.cFileName, strcha) == 0 || lstrcmp(wfd.cFileName, strcha2) == 0
					continue;

				astrDirs.Add(wfd.cFileName); 
				nLastDir++;	
			}
		} while(FindNextFile(h, &wfd));
		FindClose(h);

	}


		
		
		if(isLink)
		{
			//remove all link buttons
			REBARBANDINFO rbbi;
			int bandid = m_wndReBar.GetReBarCtrl().IDToIndex(ID_VIEW_LINKS);

			rbbi.cbSize = sizeof(rbbi);
			rbbi.fMask = RBBIM_SIZE|RBBIM_STYLE;
			m_wndReBar.GetReBarCtrl().GetBandInfo(bandid, &rbbi);
			rbbi.fStyle &= ~RBBS_NOGRIPPER;

			int b = m_LinkBar.GetToolBarCtrl().GetButtonCount();
			for(int i = 1; i<b;i++)
				m_LinkBar.GetToolBarCtrl().DeleteButton(1);

			rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE |RBBIM_ID |RBBIM_STYLE ;
			rbbi.wID = ID_VIEW_LINKS;
			rbbi.cxMinChild = 2;
			rbbi.cyMinChild = 22;
			/*rbbi.cx =*/ rbbi.cxIdeal = 2;

			m_wndReBar.GetReBarCtrl().SetBandInfo(bandid, &rbbi);
			
		}

		if(FirstLevel)
		{
			REBARBANDINFO rbbi;
			int bandid = m_wndReBar.GetReBarCtrl().IDToIndex(ID_VIEW_FAVS);

			rbbi.cbSize = sizeof(rbbi);
			rbbi.fMask = RBBIM_SIZE;
			m_wndReBar.GetReBarCtrl().GetBandInfo(bandid, &rbbi);

			//remove all Fav buttons
			int b = m_FavBar.GetToolBarCtrl().GetButtonCount();
			for(int i = 0; i<b;i++)
				m_FavBar.GetToolBarCtrl().DeleteButton(0);

			rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE |RBBIM_ID ;
			rbbi.wID = ID_VIEW_FAVS;
			rbbi.cxMinChild = 2;
			rbbi.cyMinChild = 22;
			/*rbbi.cx =*/ rbbi.cxIdeal = 2;

			m_wndReBar.GetReBarCtrl().SetBandInfo(bandid, &rbbi);


		}

//////////////////////////////// Now add these items to the menu
		CMenuOrder mo;
		BOOL found;
		int j;
		WORD type;
		if(GetFavoriteFolder(strPath))
		{
			strPath = strPath2.Mid(strPath.GetLength()+1);
			if(mo.LoadMenuOrder(strPath))
			{
				for(int i = 0; i<mo.len; i++)
				{
					str = mo.mois[mo.index[i]].longname;
					type = mo.mois[mo.index[i]].filetype;
					//find in url first
					found = FALSE;
					for(j = 0; j<astrFavorites.GetSize() && !found && (type==0x20 || type==0); j++)
					{
						if(astrFavorites[j].CompareNoCase(str)==0)
						{
							found = TRUE;
							//add
							FavMenuAddURL(astrFavorites[j], strPath2, isLink, pMenu, anFavID[j], nMostFavs);
							astrFavorites[j].Empty();
						}
					}
					for(j = 0; j<astrDirs.GetSize() && !found && (type==0x10 || type==0x14 || type==0); j++)
					{
						if(astrDirs[j].CompareNoCase(str)==0)
						{
							//add
							FavMenuAddFolder(astrDirs[j], strPath2, FirstLevel, isLink, pMenu, nEndPos);
							astrDirs[j].Empty();
						}
					}

				}
			}
		}

		//sort dirs and favs
		CString tmp;
		int n = astrDirs.GetSize();
		int i, ch, m;
		ch = n;
		while(ch>0)
		{
			m = ch-1; ch = 0;
			for(i=1; i<=m; i++)
			{
				if(lstrcmp(astrDirs[i-1], astrDirs[i])>0)
				{
					tmp = astrDirs[i-1];
					astrDirs[i-1] = astrDirs[i];
					astrDirs[i] = tmp;
					ch = i;
				}
			}
		}
		
		n = astrFavorites.GetSize();
		ch = n;
		int ntmp;
		while(ch>0)
		{
			m = ch-1; ch = 0;
			for(i=1; i<=m; i++)
			{
				if(lstrcmpiA(astrFavorites[i-1], astrFavorites[i])>0)
				{
					tmp = astrFavorites[i-1];
					astrFavorites[i-1] = astrFavorites[i];
					astrFavorites[i] = tmp;
					ntmp = anFavID[i-1];
					anFavID[i-1] = anFavID[i];
					anFavID[i] = ntmp;
					ch = i;
				}
			}
		}	

		for(i = 0; i<astrDirs.GetSize(); i++)
		{
			if(astrDirs[i].IsEmpty())
				continue;
			FavMenuAddFolder(astrDirs[i], strPath2, FirstLevel, isLink, pMenu, nEndPos);
		}		
				
		for(i = 0 ; i < astrFavorites.GetSize() ; i++)
		{
			if(astrFavorites[i].IsEmpty())
				continue;
			FavMenuAddURL(astrFavorites[i], strPath2, isLink, pMenu, anFavID[i], nMostFavs);
		}
		
		

/////////////////////////////////////////////////////////////////



		
	//add a break if needed
	if(!bSingleMenu)
		AddMenuBreak(pMenu);

	if(isLink)
		{
			CRect rectToolBar;
			REBARBANDINFO rbbi;
			m_LinkBar.GetItemRect(0, &rectToolBar);
			//m_LinkBar.SetSizes(rectToolBar.Size(), CSize(16,100));

			rbbi.cbSize = sizeof(rbbi);
			rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE |RBBIM_ID |RBBIM_STYLE;
			rbbi.wID = ID_VIEW_LINKS;
			rbbi.cxMinChild = rectToolBar.Width()+2;
			if(rectToolBar.Height()<22)
				rbbi.cyMinChild = 22;
			else
				rbbi.cyMinChild = rectToolBar.Height();
			
			int bc=m_LinkBar.GetToolBarCtrl().GetButtonCount();
			if(bc>0)
			{
				m_LinkBar.GetToolBarCtrl().GetItemRect(bc-1,&rectToolBar);
				/*rbbi.cx =*/ rbbi.cxIdeal = rectToolBar.right;
			}
			else
				/*rbbi.cx =*/ rbbi.cxIdeal = 2;

			int bandid = m_wndReBar.GetReBarCtrl().IDToIndex(ID_VIEW_LINKS);
			m_wndReBar.GetReBarCtrl().SetBandInfo(bandid, &rbbi);
			if(m_bLockToolbar)
			{
				rbbi.fMask = RBBIM_STYLE;
				m_wndReBar.GetReBarCtrl().GetBandInfo(bandid, &rbbi);
				rbbi.fStyle |= RBBS_NOGRIPPER;
				m_wndReBar.GetReBarCtrl().SetBandInfo(bandid, &rbbi);
			}
		}

		if(FirstLevel)
		{
			CRect rectToolBar;
			REBARBANDINFO rbbi;
			m_FavBar.GetItemRect(0, &rectToolBar);

			rbbi.cbSize = sizeof(rbbi);
			rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE |RBBIM_ID ;
			rbbi.wID = ID_VIEW_FAVS;
			rbbi.cxMinChild = 2;
			if(rectToolBar.Height()<22)
				rbbi.cyMinChild = 22;
			else
				rbbi.cyMinChild = rectToolBar.Height();
			
			int bc=m_FavBar.GetToolBarCtrl().GetButtonCount();
			if(bc>0)
			{
				m_FavBar.GetToolBarCtrl().GetItemRect(bc-1,&rectToolBar);
				/*rbbi.cx =*/ rbbi.cxIdeal = rectToolBar.right;
			}
			else
				/*rbbi.cx =*/ rbbi.cxIdeal = 2;

			int bandid = m_wndReBar.GetReBarCtrl().IDToIndex(ID_VIEW_FAVS);
			m_wndReBar.GetReBarCtrl().SetBandInfo(bandid, &rbbi);
		}
	}
/*	catch(CMemoryException* e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}

	return nEndPos;
}

void CMainFrame::OnFavorite(UINT nID)
{
	short ks;
	ks =GetKeyState(VK_CONTROL);

	//static CString url;
	static TCHAR buf[INTERNET_MAX_PATH_LENGTH];
	try
	{
	if(nID>=0x1380)
	{
	//	url = m_astrFavoriteURLs[nID-0x1380];
		CMenu * pMenu = GetMenu();

		MENUITEMINFO mii; 
		if(pMenu!=NULL)
		{
			mii.fMask = MIIM_DATA|MIIM_TYPE;    // get the state of the menu item
			mii.dwTypeData = NULL;
			mii.cch = 0;
			mii.dwItemData = NULL;
			mii.cbSize = sizeof(mii);
			pMenu->GetMenuItemInfo(nID, &mii, FALSE); 
			CMenuData* pmd = (CMenuData*)(mii.dwItemData);
			if(mii.fType & MFT_OWNERDRAW && pmd!=NULL)
				if(pmd->m_szURL!=NULL)
					favurl=pmd->m_szURL;
		}

		if(favurl.Right(4) == _T(".lnk"))
		{
				if(ResolveLink(favurl, buf))
				{
					favurl = buf;
					//is exe?
					if(favurl.Right(4)==".EXE" || favurl.Right(4)==".exe")
					{
						ShellExecute(NULL, "open", favurl , "", NULL, SW_SHOW );
						return;
					}
				}
				else
					favurl = "about:blank";
		}
	}


	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	if (tcf!=NULL && tcf->m_pView!=NULL && !bFavInNewWindow && ks>=0)
		((CMyIEView*)tcf->m_pView)->ToNavigate(favurl, 0, NULL);
	else
	{
		tcf = NewChildWindow(0);
		if(tcf!=NULL)
			((CMyIEView*)tcf->m_pView)->ToNavigate(favurl, 0, NULL);
	}
	}
/*	catch(CMemoryException* e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}
}

void CMainFrame::OnFileNewBlank() 
{
	// TODO: Add your command handler code here
	//nOpenType = 0;
	//PostMessage(WM_COMMAND, ID_FILE_NEW_S);
	NewChildWindow(2,0, NULL, NULL, TRUE);
//	if(m_bActiveNewWindow)
		m_wndAddress->SetFocus();
}

void CMainFrame::OnFileNewCurrentpage() 
{
	// TODO: Add your command handler code here
//	nOpenType = 2;
	try{

	CString strURL;
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	if (tcf!=NULL && tcf->m_pView!=NULL)
		// ((CMyIEView*)tcf->m_pView)->GetLocationURL(strURL);
		strURL = ((CMyIEView*)tcf->m_pView)->m_lpszUrl;
	else
		strURL = "about:blank";

	//PostMessage(WM_COMMAND, ID_FILE_NEW_S);
	NewChildWindow(1,2,strURL, NULL, TRUE);
//	if(m_bActiveNewWindow)
		m_wndAddress->SetFocus();
	}catch(...)
	{
	}
}


void CMainFrame::OnUpdateProgress(CCmdUI* pCmdUI) 
{

	try
	{
	if(percent<0)
	{
		if(m_wndStatusBar.IsProgressBarVisible())
			m_wndStatusBar.ShowProgressBar(false);
		//pCmdUI->SetText("");
	}
	else
	{
		if(!m_wndStatusBar.IsProgressBarVisible())
			m_wndStatusBar.ShowProgressBar(true);
		m_wndStatusBar.SetPos(percent);
		static TCHAR str[8];
        sprintf(str, "%d%%", percent);
       m_wndStatusBar.SetWindowText(str);
	}
	}
/*	catch(CMemoryException* e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	//	AfxMessageBox("update progress error");
	}
}


void CMainFrame::OnUpdateWins(CCmdUI* pCmdUI) 
{
	static char temp[5];
	itoa(nWin, temp, 10);
	pCmdUI->SetText(temp);
}

void CMainFrame::OnUpdateIP(CCmdUI* pCmdUI) 
{
//	CString ip;
  //  CString addre;
	struct hostent * pHostent;
	pHostent = NULL;

	try{

	if(!m_bDisplayIP)
	{
		pCmdUI->SetText("");
		return;
	}


	CTime t;
	CTimeSpan ts;

	//ip ="";
	t = CTime::GetCurrentTime();
	ts = t - m_tLastIPUpdate;  
	int mini = 5, sec = ts.GetTotalSeconds();
	if(sec>mini && m_strLastIP!="127.0.0.1")
		mini=20;
	if(sec > mini)
	{
		m_tLastIPUpdate = t;
		//is any window is downloading
//11/5
/*		BOOL IsDownloading = FALSE;
		CChildFrame* tcf;
		int n = m_wndTab.GetItemCount();
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;
		for(int l=n-1;l>=0;l--)
		{
			TabCtrlItem.lParam = 0;
			m_wndTab.GetItem(l, &TabCtrlItem);
			tcf = ((CChildFrame*)TabCtrlItem.lParam);
			if(tcf != NULL && tcf->m_pView != NULL)
			{
				if(tcf->m_pView->m_nProgress>=0)
					IsDownloading = TRUE;
			}
		}
		if(IsDownloading)
			return;
*/
		try{
		//get IP Address
		int i=0,j=0;
//		if(gethostname(addr,255)!=SOCKET_ERROR )
		{
			pHostent=gethostbyname(addr);
			if(pHostent!=NULL && pHostent->h_addr_list[0]!= NULL)
			{
				int i = 0;
				while(pHostent->h_addr_list[i]!= NULL)
				{
					//ip = "";
					m_strLastIP.Empty();
					gaddre.Empty();
 					for( j = 0; j < pHostent->h_length; j++ )
 					{
 
 						if( j > 0 )
 							m_strLastIP += ".";
 
 						gaddre.Format("%u", (unsigned int)((unsigned
		 				char*)pHostent->h_addr_list[i])[j]);
						m_strLastIP += gaddre;
 					}
					i++;
				}

				pCmdUI->SetText(m_strLastIP);
				//m_strLastIP = ip;

			}
		}
		
		}
		catch(...)
		{

		}
		
	/*	try{
			if(pHostent!=NULL)
				delete pHostent; //?
		}catch(...)
		{
		}*/

	}

	}catch(...)
	{
	}
}


int CMainFrame::AddNewTab(CWnd* lpMvw, int CurID, LPTSTR label, BOOL bAddLast)
{
	if(lpMvw==NULL)
		return 0;
	try
	{
	//static CString str;

	if (bAddLast)
		CurID = nTotalTabs;
	else
		CurID ++;

	nTotalTabs ++;

	int presel = m_wndTab.GetCurSel();
	if(m_bNextTab)
		CurID = presel+1;

	
	TCITEM TabCtrlItem;
	tab.LoadString(IDS_UNTITLED);
	TabCtrlItem.mask = TCIF_TEXT|TCIF_PARAM;
	if(label==NULL)
		TabCtrlItem.pszText = tab.GetBuffer(30);
	else
		TabCtrlItem.pszText = label;
	TabCtrlItem.lParam = (LPARAM)lpMvw;

	m_wndTab.InsertItem( CurID, &TabCtrlItem );

	tab.ReleaseBuffer();

	//select this tab
	if(((CChildFrame*)lpMvw)->m_pView!=NULL)
		if(((CChildFrame*)lpMvw)->m_pView->IsActive)//m_bActiveNewWindow
			m_wndTab.SetCurSel(CurID);
		else
			m_wndTab.SetCurSel(presel);

	PostMessage(WM_UPDATE_TAB, TRUE);
//	PostMessage(WM_UPDATE_TAB_TIP);

	nWin = m_wndTab.GetItemCount();

	if(nWin<=2)
		m_SysMenuBar.PostMessage(WM_IDLEUPDATECMDUI, TRUE, 0);

//	SaveLastVisit();
	bVerbose = TRUE;

	}
/*	catch(CMemoryException * e)
	{
//		AfxMessageBox("add tab error");
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}

	return CurID;

}


void CMainFrame::OnSelChange(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	//PostMessage(WM_SEL_TAB);
	OnSelTab(0,0);
	*pResult = 0;
}

void CMainFrame::OnSelTab(WPARAM wParam, LPARAM lParam)
{
	try
	{
	m_wndTab.m_bDragging = FALSE;

	TCITEM TabCtrlItem;

	int n = m_wndTab.GetCurSel();
	if(n>=0)
	{
		TabCtrlItem.mask = TCIF_PARAM;
		m_wndTab.GetItem(n, &TabCtrlItem);
		if(TabCtrlItem.lParam != NULL)
		{	
	/*3.1		BOOL bMax;
	
			CChildFrame * tcf;
			tcf = (CChildFrame*)MDIGetActive(&bMax);*/
			((CChildFrame*)TabCtrlItem.lParam)->m_bSkipSel = TRUE;
			((CChildFrame*)TabCtrlItem.lParam)->nCurTabID = n;

			if(((CChildFrame*)TabCtrlItem.lParam)->bProtect)
				PostMessage(WM_ACTIVATE_WINDOW, 0, TabCtrlItem.lParam);
			else
				MDIActivate((CChildFrame*)TabCtrlItem.lParam);
	/*3.1		if(tcf!=NULL && bMax)
			{
				m_bMax = 1;
			}*/

		}
		
	}
	}
/*	catch(CMemoryException* e)
	{
//		AfxMessageBox("sel change error");
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}
}

void CMainFrame::OnRClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	CPoint point;

	// translate the current toolbar item rectangle into screen coordinates
	// so that we'll know where to pop up the menu
	if(GetKeyState(VK_CONTROL)<0)
		return;

	GetCursorPos(&point);

//	int n = m_wndTab.GetItemCount();
	BOOL found = false;
	int i=0;
	CRect rect;

/*	while (i<n && !found)
	{
		m_wndTab.GetItemRect(i, &rect);
		m_wndTab.ClientToScreen(&rect);

		if(rect.PtInRect(point))
		{	
			found = true;				
		}
		else
			i++;
	}*/

	try{

	m_wndTab.ScreenToClient(&point);
	TCHITTESTINFO hti;
	hti.pt = point;
	hti.flags = TCHT_ONITEM | TCHT_ONITEMICON | TCHT_ONITEMLABEL ;
	nRTabID = m_wndTab.HitTest(&hti);

	m_wndTab.ClientToScreen(&point);


	CMenu menu;
	CMenu* pPopup, *pGroups = NULL;

	if(nRTabID>=0)
	{
		menu.LoadMenu(IDR_RTAB);
		pPopup = menu.GetSubMenu(0);
		
		//if is locked
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;

		m_wndTab.GetItem(nRTabID, &TabCtrlItem);
		if(TabCtrlItem.lParam != NULL && pPopup!=NULL)
		{
			if(((CChildFrame*)TabCtrlItem.lParam)->bLock)
				pPopup->CheckMenuItem(ID_RTAB_LOCK, MF_CHECKED );

			if(((CChildFrame*)TabCtrlItem.lParam)->bProtect)
				pPopup->CheckMenuItem(ID_RTAB_PROTECT, MF_CHECKED );

			if(m_bTopTab)
			{
				CString str;
				pPopup->GetMenuString(ID_FILE_CLOSE_2, str, MF_BYCOMMAND);
				pPopup->DeleteMenu(ID_FILE_CLOSE_2, MF_BYCOMMAND);
				pPopup->InsertMenu(0, MF_BYPOSITION, ID_FILE_CLOSE_2, str);
				pPopup->InsertMenu(1, MF_BYPOSITION|MF_SEPARATOR);
			}
		}

		//load group list
		if(m_bTopTab)
			pGroups = pPopup->GetSubMenu(2)->GetSubMenu(2);
		else
			pGroups = pPopup->GetSubMenu(0)->GetSubMenu(2);
		MenuLoadGroupList(pGroups);

		m_bRTab = TRUE;

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
	}

	}catch(...)
	{
	}

	*pResult = 0;
}

//#pragma optimize( "s", on)

void CMainFrame::SaveTypedURL()
{
	TCHAR           sz[MAX_PATH];
	HKEY            hKey;
	TCHAR			id[9] = "url";
	int				i, l=0 ;

	try{
	RegDeleteKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\TypedUrls"));

	if(RegCreateKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\TypedUrls"), &hKey) != ERROR_SUCCESS)
		return ;


	COMBOBOXEXITEM item;
	item.mask = CBEIF_TEXT;
	item.pszText = (LPTSTR)sz;
	item.cchTextMax = MAX_PATH;

	int nc = m_wndAddress->SendMessage(CB_GETCOUNT, 0, 0);
	if (nc>24)
		nc=24;
	for(i=0;i<nc;i++)
	{
		item.iItem = i;
		sz[0]=0;
		m_wndAddress->GetItem(&item);
		itoa(l+1, id+3, 10);

		if(item.pszText!=NULL && strlen(item.pszText)>0)
		{
			RegSetValueEx(hKey, _T(id), NULL, REG_SZ, (LPBYTE)(item.pszText), strlen(item.pszText));
			l++;
		}
	}
	RegCloseKey(hKey);

	}catch(...)
	{
	}
}

////#pragma optimize( "s", on )

void CMainFrame::OnDropDown(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	// this function handles the dropdown menus from the toolbar
	NMTOOLBAR* pNMToolBar = (NMTOOLBAR*)pNotifyStruct;
	CRect rect;

	try{

	// translate the current toolbar item rectangle into screen coordinates
	// so that we'll know where to pop up the menu
	if(pNMToolBar->iItem == IDOK)
		m_AddressBar.GetToolBarCtrl().GetRect(pNMToolBar->iItem, &rect);
	else
		m_wndToolBar.GetToolBarCtrl().GetRect(pNMToolBar->iItem, &rect);
	rect.top = rect.bottom;
	::ClientToScreen(pNMToolBar->hdr.hwndFrom, &rect.TopLeft());
	if(pNMToolBar->iItem == ID_FONT_DROPDOWN)
	{
		CMenu menu;
		CMenu* pPopup;

		// the font popup is stored in a resource
		menu.LoadMenu(IDR_FONT_POPUP);
		pPopup = menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.top + 1, AfxGetMainWnd());
	}
	else if(pNMToolBar->iItem == ID_NEWWINDOW)
	{
		CMenu* pPopup;
		pPopup = GetMenu()->GetSubMenu(0)->GetSubMenu(0);
		pPopup->SetDefaultItem(m_nNewWindow, TRUE);

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.top + 1, AfxGetMainWnd());
	}
	else if(pNMToolBar->iItem == ID_TOOLS_SETBG)
	{
		CMenu* pPopup;
		pPopup = GetMenu()->GetSubMenu(6)->GetSubMenu(20);

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.top + 1, AfxGetMainWnd());
	}
	else if(pNMToolBar->iItem == ID_OPTIONS_LOADIMAGE)
	{
		CMenu* pPopup;
		pPopup = GetMenu()->GetSubMenu(5)->GetSubMenu(0);

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.top + 1, AfxGetMainWnd());
	}
	else if(pNMToolBar->iItem == ID_EDIT_UNDOCLOSE)
	{
		CMenu* pPopup;
		pPopup = GetMenu()->GetSubMenu(1)->GetSubMenu(0);

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.top + 1, AfxGetMainWnd());
	}
	else if(pNMToolBar->iItem == ID_TOOLS_ALL)
	{
		CMenu* pPopup;
		pPopup = GetMenu()->GetSubMenu(6)->GetSubMenu(11);

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.top + 1, AfxGetMainWnd());
	}
	else if(pNMToolBar->iItem == ID_OPTIONS_USE_PROXY)
	{
		CMenu pPopup;
		INTERNET_PROXY_INFO ipi;
		DWORD dwSize=sizeof(ipi);
		BOOL bUseProxya = FALSE;

		//This call determines the buffer size needed
		InternetQueryOption(NULL,INTERNET_OPTION_PROXY,&ipi,&dwSize);
		if (ipi.dwAccessType != INTERNET_OPEN_TYPE_DIRECT )
			bUseProxya = TRUE;
		else
			bUseProxya = FALSE;

		pPopup.CreatePopupMenu();
//		POSITION pos1=m_aProxySpeed.GetHeadPosition(), pos = m_astrProxy.GetHeadPosition(), pos2 = m_astrProxyName.GetHeadPosition();
		int i=0;
		CString str, str1;
/*		while(pos!=NULL && pos1!=NULL && pos2!=NULL)
		{
			str = m_astrProxyName.GetAt(pos2);
			str = str.Left(50);
			str1.LoadString(IDS_SP_INVALID + m_aProxySpeed.GetAt(pos1));
			str1 = "(" + str1 + ")";
			if(str.Left(1)=="-")
			{
				pPopup.AppendMenu(MF_GRAYED|MF_STRING|MF_DISABLED, 0, str);
			}
			else
				pPopup.AppendMenu(MF_STRING, 0x1100 + i, str+str1);
			if(bUseProxy && m_strCurProxy == m_astrProxy.GetAt(pos))
				pPopup.CheckMenuItem(0x1100 + i, MF_CHECKED | MF_BYCOMMAND );
			m_astrProxy.GetNext(pos);
			m_aProxySpeed.GetNext(pos1);
			m_astrProxyName.GetNext(pos2);
			i++;
		}*/
		CProxyList* pl;
		CMenu submenu;
		int pindex = 0;
		int pcates = 0;
		for(int l = 0;l <= m_aProxyCategories.m_ProxyCategory.GetUpperBound();l++)
		{	
			pl = m_aProxyCategories.m_ProxyCategory.GetAt(l);
			if(pl!=NULL && !pl->m_bIsWebProxy)
				pcates++;
		}
		for(l = 0;l <= m_aProxyCategories.m_ProxyCategory.GetUpperBound();l++)
		{
			pl = m_aProxyCategories.m_ProxyCategory.GetAt(l);
			if(pl!=NULL && !pl->m_bIsWebProxy)
			{
				if(pcates>1)
					submenu.CreatePopupMenu();
				for(i=0; i<=pl->m_Proxies.GetUpperBound(); i++)
				{
					str = pl->m_Proxies.ElementAt(i)->m_strProxyName;
					if(str.Left(2)=="--")
					{
						if(pcates>1)
							//submenu.AppendMenu(MF_GRAYED|MF_STRING|MF_DISABLED, 0, str);
							submenu.AppendMenu(MF_SEPARATOR);
						else if(pcates==1)
							//pPopup.AppendMenu(MF_GRAYED|MF_STRING|MF_DISABLED, 0, str);
							pPopup.AppendMenu(MF_SEPARATOR);
					}
					else
					{
						str = str.Left(50);
						str1.LoadString(IDS_SP_INVALID + pl->m_Proxies.ElementAt(i)->m_nProxySpeed);
						str1 = "(" + str1 + ")";

						if(pcates>1)
							submenu.AppendMenu(MF_STRING, 0x6320 + pindex, str+str1);
						else if(pcates==1)
							pPopup.AppendMenu(MF_STRING, 0x6320 + pindex, str+str1);
						if(bUseProxya)
						{
							if(m_strCurProxy == pl->m_Proxies.ElementAt(i)->m_strProxy)
							{
								if(pcates>1)
									submenu.CheckMenuItem(0x6320 + pindex, MF_CHECKED | MF_BYCOMMAND );
								else if(pcates==1)
									pPopup.CheckMenuItem(0x6320 + pindex, MF_CHECKED | MF_BYCOMMAND );
							}
						}
						pl->m_Proxies.ElementAt(i)->index = pindex;
						pindex++;
					}
				}
				if(pcates>1)
				{
					pPopup.AppendMenu(MF_POPUP, (UINT)submenu.m_hMenu, pl->m_strCategoryName);
					submenu.Detach();
				}
			}
		}

		pPopup.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.top + 1, AfxGetMainWnd());
		pPopup.DestroyMenu();
	}
	else if(pNMToolBar->iItem == ID_OPTIONS_USE_WEBP || pNMToolBar->iItem == IDOK)
	{
		m_dwWebProxyFrom = pNMToolBar->iItem;
		CMenu pPopup;
		pPopup.CreatePopupMenu();

		BuildWebProxyMenu(&pPopup, 2);
		
		pPopup.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.top + 1, AfxGetMainWnd());
		pPopup.DestroyMenu();
	}
	else if(pNMToolBar->iItem == ID_ENCODE_DROPDOWN)
	{
		CMenu menu;
		CMenu* pPopup;

		// the font popup is stored in a resource
		menu.LoadMenu(IDR_ENCODING);
		pPopup = menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.top + 1, AfxGetMainWnd());
	}
	else if(pNMToolBar->iItem == ID_BMODE_DROPDOWN)
	{
		CMenu* pPopup;
		pPopup = GetMenu()->GetSubMenu(5)->GetSubMenu(9);

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.top + 1, AfxGetMainWnd());
	}
	else if(pNMToolBar->iItem == ID_TOOLS_AUTOSCROLL_USEAUTOSCROLLING)
	{
		CMenu* pPopup;
		pPopup = GetMenu()->GetSubMenu(6)->GetSubMenu(19);

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.top + 1, AfxGetMainWnd());
	}
	else if(pNMToolBar->iItem == ID_GO_BACK)
	{
		try{

		CMenu pPopup;
		CChildFrame* tcf = (CChildFrame*)MDIGetActive();
		if (tcf!=NULL && tcf->m_pView!=NULL)
		{
			// the font popup is stored in a resource
			pPopup.CreatePopupMenu();
			SessionHistory(&pPopup, ID_GO_BACK, tcf);
			
			int cmd = pPopup.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, rect.left, rect.top + 1, this);
			pPopup.DestroyMenu();

			//2.2
			if(cmd>0)
				tcf->m_pView->GoBack(cmd);

		/*	if(cmd>0)
			{
				LPDISPATCH pDisp = NULL;
				IHTMLDocument2 *pHTMLDoc = NULL;
				IHTMLWindow2 *pHTMLWnd = NULL;
				IOmHistory  *pHistory = NULL;
				VARIANT v;
				v.vt = VT_I2;
				v.iVal = cmd;
				try{
					//get history length
					pDisp = tcf->m_pView->GetHtmlDocument();
					if( pDisp )
					{
						if(SUCCEEDED(pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc)))
						{
							if(SUCCEEDED(pHTMLDoc->get_parentWindow( &pHTMLWnd )))
							{
								if(SUCCEEDED(pHTMLWnd->get_history( &pHistory )))
								{
									pHistory->back(&v);
								}
							}
						}
					}
				}catch(...)
				{
				}
				try{
					if(pHistory != NULL)
						pHistory->Release();
					if(pHTMLWnd != NULL)
						pHTMLWnd->Release();
					if(pHTMLDoc != NULL)
						pHTMLDoc->Release();
					if(pDisp != NULL)
						pDisp->Release();
				}
				catch(...)
				{
				}
			}*/

			tcf->m_pView->m_nBackSteps = cmd;
		}

		}catch(...)
		{
		}

	}
	else if(pNMToolBar->iItem == ID_GO_FORWARD)
	{
		try{

		CMenu pPopup;
		CChildFrame* tcf = (CChildFrame*)MDIGetActive();
		if (tcf!=NULL && tcf->m_pView!=NULL)
		{
			// the font popup is stored in a resource
			pPopup.CreatePopupMenu();
			SessionHistory(&pPopup, ID_GO_FORWARD, tcf);

			
			int cmd = pPopup.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, rect.left, rect.top + 1, AfxGetMainWnd());
			pPopup.DestroyMenu();

			//for(i=0; i<cmd; i++)
			//2.2
			if(cmd>0)
				tcf->m_pView->GoForward(cmd);
			tcf->m_pView->m_nBackSteps = -cmd;
		}

		}catch(...)
		{
		}

	}
	else if(pNMToolBar->iItem == ID_TOOLS_EMAIL)
	{
		try{

		CMenu pPopup;
		pPopup.CreatePopupMenu();
			
		//get all email clients from registry
		static TCHAR           sz[MAX_PATH+1];
		HKEY            hKey;
		DWORD           dwSize = MAX_PATH+1;
		int i = 0;

			
		// find out from the registry where the mail clients are located.
		if(RegOpenKey(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Clients\\Mail"), &hKey) == ERROR_SUCCESS)
		{
			while(RegEnumKey(hKey, i, sz, dwSize) == ERROR_SUCCESS)
			{
				pPopup.AppendMenu(MF_STRING, i+1, sz);
				i++;
			}

			
			int cmd = pPopup.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, rect.left, rect.top + 1, AfxGetMainWnd());
			pPopup.DestroyMenu();

			if(cmd>0 && RegEnumKey(hKey, cmd-1, sz, dwSize) == ERROR_SUCCESS)
			{
				LaunchEmail(sz);
			}
			RegCloseKey(hKey);
		}

		}catch(...)
		{
		}

	}
	else if(pNMToolBar->iItem == ID_EDIT_EDIT)
	{

		CMenu pPopup;
		pPopup.CreatePopupMenu();
			
		//get all email clients from registry
		static TCHAR           sz[MAX_PATH+1];
		HKEY            hKey, hKey2;
		DWORD           dwSize = MAX_PATH+1;
		int i = 0, l = 1;
		CString key;
		CStringArray editors;

			
		// find out from the registry where the editor clients are located.
		if(RegOpenKey(HKEY_CLASSES_ROOT, _T(".htm\\OpenWithList"), &hKey) == ERROR_SUCCESS)
		{
			while(RegEnumKey(hKey, i, sz, dwSize) == ERROR_SUCCESS)
			{
				key = "Applications\\";
				key += sz;
				key += "\\shell";
				if(RegOpenKey(HKEY_CLASSES_ROOT, key, &hKey2) == ERROR_SUCCESS)
				{
					RegQueryValueEx(hKey2, _T("FriendlyCache"), NULL, NULL, (LPBYTE)sz, &dwSize);
					editors.Add(key);
					pPopup.AppendMenu(MF_STRING, l, sz);
					l++;
					RegCloseKey(hKey2);
				}
				dwSize = MAX_PATH+1;
				i++;
			}

			
			int cmd = pPopup.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, rect.left, rect.top + 1, AfxGetMainWnd());
			pPopup.DestroyMenu();
			RegCloseKey(hKey);

			if(cmd>0)
			{
				key = editors.GetAt(cmd-1);
				key += "\\edit\\command";
				if(RegOpenKey(HKEY_CLASSES_ROOT, key, &hKey2) == ERROR_SUCCESS)
				{
					dwSize = MAX_PATH+1;
					RegQueryValueEx(hKey2, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);
					RegCloseKey(hKey2);
					CChildFrame* tcf = (CChildFrame*)MDIGetActive();
					if (tcf!=NULL && tcf->m_pView!=NULL)
					{
						LaunchEditor(sz, tcf->m_pView->m_lpszUrl);
					}

				}
			}
		}
	}


	}catch(...)
	{
	}

	*pResult = TBDDRET_DEFAULT;
}

void CMainFrame::OnViewAddressbar() 
{
	// TODO: Add your command handler code here
	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_ID|RBBIM_STYLE;
	int nb = m_wndReBar.GetReBarCtrl( ).IDToIndex(ID_VIEW_ADDRESSBAR);
	m_wndReBar.GetReBarCtrl().GetBandInfo(nb, &rbbi);

	if( rbbi.fStyle & RBBS_HIDDEN  || !m_wndAddress->IsWindowVisible())
	{
		m_AddressBar.ShowWindow(SW_SHOW);
		m_wndReBar.GetReBarCtrl( ).ShowBand(nb, TRUE);
		GetMenu()->CheckMenuItem(ID_VIEW_ADDRESSBAR, MF_CHECKED);

	}
	else
	{
		m_wndReBar.GetReBarCtrl( ).ShowBand(nb,FALSE);
		m_AddressBar.ShowWindow(SW_HIDE);
		GetMenu()->CheckMenuItem(ID_VIEW_ADDRESSBAR, MF_UNCHECKED);
	}
}

void CMainFrame::OnViewTaskbar() 
{
	// TODO: Add your command handler code here
	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_ID|RBBIM_STYLE;
	int nb;
	if(m_bTopTab)
	{
		nb = m_wndReBar.GetReBarCtrl( ).IDToIndex(ID_VIEW_TASKBAR);
		m_wndReBar.GetReBarCtrl().GetBandInfo(nb, &rbbi);
	}
	else
	{
		nb=0;
		m_wndReBar2.GetReBarCtrl().GetBandInfo(nb, &rbbi);
	}

	if(rbbi.fStyle & RBBS_HIDDEN)
	{
		if(m_bTopTab)
		{
			m_wndReBar.GetReBarCtrl( ).ShowBand(nb,TRUE);
		}
		else
		{
			m_wndReBar2.GetReBarCtrl( ).ShowBand(0,TRUE);
		}
		m_wndTab.ShowWindow(SW_SHOW);
		GetMenu()->CheckMenuItem(ID_VIEW_TASKBAR, MF_CHECKED);

	}
	else
	{
		if(m_bTopTab)
		{
			m_wndReBar.GetReBarCtrl( ).ShowBand(nb,FALSE);
		}
		else
		{
			m_wndReBar2.GetReBarCtrl( ).ShowBand(0,FALSE);
		}
		m_wndTab.ShowWindow(SW_HIDE);
		GetMenu()->CheckMenuItem(ID_VIEW_TASKBAR, MF_UNCHECKED);

	}

}



void CMainFrame::OnFileWorkoffline() 
{
	// TODO: Add your command handler code here

	//Get global offline setting
	DWORD           sz;
	//TCHAR           szPath[MAX_PATH];
	HKEY            hKey;
	DWORD           dwSize = sizeof(DWORD);

	if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"), &hKey) != ERROR_SUCCESS)
	{
		if(RegCreateKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"), &hKey) != ERROR_SUCCESS)
			return;
	}

	if(RegQueryValueEx(hKey, _T("GlobalUserOffline"), NULL, NULL, (LPBYTE)&sz, &dwSize) == ERROR_SUCCESS)
	{
		//set new offline value
		if(sz != 0)
			sz = 0;
		else
			sz = 1;
	}
	else
		sz = 1;

	RegSetValueEx(hKey, _T("GlobalUserOffline"), NULL, REG_DWORD, (LPBYTE)(&sz), dwSize);

	if(sz!=0)
	{
		GetMenu()->CheckMenuItem(ID_FILE_WORKOFFLINE, MF_CHECKED);
	}
	else
	{
		GetMenu()->CheckMenuItem(ID_FILE_WORKOFFLINE, MF_UNCHECKED);
	}

	RegCloseKey(hKey);

	//infor the system that setting is changed.
	InternetSetOption(
		NULL,
		INTERNET_OPTION_SETTINGS_CHANGED,
		NULL,
		0
	);

}

void CMainFrame::LoadTypedURL()
{
	TCHAR           sz[MAX_PATH];
	//TCHAR           szPath[MAX_PATH];
	HKEY            hKey;
	DWORD           dwSize;
	TCHAR			id[9] = "url";
	int				i = 1;

	if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\TypedUrls"), &hKey) != ERROR_SUCCESS)
	{
		TRACE0("Typed URLs not found\n");
		return ;
	}
	dwSize = MAX_PATH-1;

	itoa(i, id+3, 10);

	COMBOBOXEXITEM item;

	item.mask = CBEIF_TEXT|CBEIF_IMAGE|CBEIF_SELECTEDIMAGE ;
	item.iImage = 1;
	item.iSelectedImage = 1;


	while(RegQueryValueEx(hKey, _T(id), NULL, NULL, (LPBYTE)sz, &dwSize) == ERROR_SUCCESS)
	{
		if(strlen(sz)>0)
		{
			item.iItem = i-1;
			item.pszText = (LPTSTR)(LPCTSTR)sz;

			m_wndAddress->InsertItem(&item);
		}

		i++;
		itoa(i, id+3, 10);
		dwSize = MAX_PATH - 1;
	}
	RegCloseKey(hKey);
}


void CMainFrame::OnRtabLock() 
{
	// TODO: Add your command handler code here
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;

	m_wndTab.GetItem(nRTabID, &TabCtrlItem);
	if(TabCtrlItem.lParam != NULL)
	{
		((CChildFrame*)TabCtrlItem.lParam)->bLock =!((CChildFrame*)TabCtrlItem.lParam)->bLock;

		if(((CChildFrame*)TabCtrlItem.lParam)->m_pView->m_nProgress<0)
		{
			if(((CChildFrame*)TabCtrlItem.lParam)->bLock)
				((CChildFrame*)TabCtrlItem.lParam)->m_pView->oldIcon = SetTabIcon( 4, nRTabID );
			else
				((CChildFrame*)TabCtrlItem.lParam)->m_pView->oldIcon = SetTabIcon( -1, nRTabID );

			//update tab bar yab0
			if(m_bFBTab)
				PostMessage(WM_UPDATE_TAB, FALSE);
		}
	}
}

void CMainFrame::OnToolsAddtopopupfilter() 
{
	// TODO: Add your command handler code here
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	
	if (tcf!=NULL)
	{
		AddToPopup(tcf);
		//tcf->MDIDestroy();
		//tcf->SendMessage(WM_CLOSE);
	}

}

void CMainFrame::OnRtabAddtopopupfilter() 
{
	// TODO: Add your command handler code here
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;

	m_wndTab.GetItem(nRTabID, &TabCtrlItem);
	if(TabCtrlItem.lParam != NULL)
	{
		AddToPopup(((CChildFrame*)TabCtrlItem.lParam));
		//((CChildFrame*)TabCtrlItem.lParam)->MDIDestroy();
		//((CChildFrame*)TabCtrlItem.lParam)->SendMessage(WM_CLOSE);
	}
	
}

void CMainFrame::AddToPopup(CChildFrame *pChFrm)
{
	try{
	CMyIEView* pView = (CMyIEView*)pChFrm->m_pView;
	if(pView!=NULL)
	{
		CString strURL, str;
		str.LoadString(IDS_POPUP_CONF);
		pView->GetLocationURL(strURL);
		if(strURL.IsEmpty())
			strURL = pView->m_lpszUrl;
		str =  strURL + str;
		if(AfxMessageBox(str, MB_YESNO|MB_ICONQUESTION)==IDNO)
			return;
		if(m_astrPopup.Find(strURL)==NULL)
			m_astrPopup.AddTail(strURL);
		pChFrm->m_pView->m_bToClose = TRUE;
		pChFrm->SendMessage(WM_CLOSE);

		m_bSaveConfig = TRUE;
	}
	}catch(...)
	{
	}
}

void CMainFrame::OnToolsEablepopupfilter() 
{
	// TODO: Add your command handler code here

	if(EnablePopupFilter)
	{
		GetMenu()->CheckMenuItem(ID_TOOLS_EABLEPOPUPFILTER,MF_UNCHECKED);
		EnablePopupFilter = FALSE;
	}
	else
	{
		GetMenu()->CheckMenuItem(ID_TOOLS_EABLEPOPUPFILTER,MF_CHECKED);
		EnablePopupFilter = TRUE;
	}
}



void CMainFrame::OnToolsEditpopupfilterlist() 
{
	// TODO: Add your command handler code here
	CPopupDlg dlg;
	CUrlFilterProp urlfprop;
	CPropertySheet sheet(IDS_FILTERSHEET,this);
	sheet.AddPage(&dlg);
	sheet.AddPage(&urlfprop);

	dlg.m_EnableFilter = EnablePopupFilter;
	dlg.m_EnableDupFilter = EnableDupFilter;
	dlg.m_EnableAutoFilter = EnableAutoFilter;
	
	urlfprop.m_bUseUrlFilter = m_bUseUrlFilter;

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	if(sheet.DoModal()==IDOK)
	{
		if(m_bUseUrlFilter != urlfprop.m_bUseUrlFilter)
		{
			m_bUseUrlFilter = urlfprop.m_bUseUrlFilter;
			if(m_bUseUrlFilter)
			{
				InitialNameSpace();
			}
			else
				UninitialNameSpace();
		}

		EnablePopupFilter = dlg.m_EnableFilter;
		if(EnablePopupFilter)
			GetMenu()->CheckMenuItem(ID_TOOLS_EABLEPOPUPFILTER,MF_CHECKED);
		else
			GetMenu()->CheckMenuItem(ID_TOOLS_EABLEPOPUPFILTER,MF_UNCHECKED);
		EnableDupFilter = dlg.m_EnableDupFilter;
		EnableAutoFilter = dlg.m_EnableAutoFilter;

		m_bSaveConfig = TRUE;
	}
}

/*void CMainFrame::OnOptionsMyiegohome() 
{
	// TODO: Add your command handler code here
	bGoHome = 1;
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIEGOHOME,MF_CHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_OPENBLANKPAGE,MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_RESUMELASTVISITEDPAGES,MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_OPENFAVORITEPAGES,MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_OPENMOSTFAVORITEPAGES,MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_OPENSTARTUPGROUP,MF_UNCHECKED);
}
*/
void CMainFrame::OnToolsEmail() 
{
	// TODO: Add your command handler code here
	TCHAR           sz[MAX_PATH];
	HKEY            hKey;
	DWORD           dwSize = MAX_PATH;

	
	// find out from the registry where the favorites are located.
	if(RegOpenKey(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Clients\\Mail"), &hKey) != ERROR_SUCCESS)
	{
		TRACE0("mail client not found\n");
		return ;
	}
	dwSize = sizeof(sz);
	RegQueryValueEx(hKey, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);
	if(dwSize>0)
	{
		LaunchEmail(sz);
	}
	RegCloseKey(hKey);


}

void CMainFrame::OnToolsAddr() 
{
	// TODO: Add your command handler code here
	TCHAR           sz[MAX_PATH];
	TCHAR           szPath[MAX_PATH];
	HKEY            hKey;
	DWORD           dwSize = MAX_PATH;

	
	// find out from the registry where the favorites are located.
	if(RegOpenKey(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Clients\\Contacts"), &hKey) != ERROR_SUCCESS)
	{
		TRACE0("address book client not found\n");
		return ;
	}
	dwSize = sizeof(sz);
	RegQueryValueEx(hKey, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);
	if(dwSize>0)
	{
		RegCloseKey(hKey);
		CString key = "SOFTWARE\\Clients\\Contacts\\";
		key += sz;
		key += "\\Shell\\open\\command";
		if(RegOpenKey(HKEY_LOCAL_MACHINE, key, &hKey) == ERROR_SUCCESS)
		{
			//name found, to get the path
			dwSize = MAX_PATH;
			RegQueryValueEx(hKey, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);
			if(strlen(sz)==0)
			{
				AfxMessageBox(IDS_FAIL_ADDR_BOOK);
				return;
			}
			ExpandEnvironmentStrings(sz, szPath, MAX_PATH);

			//run
			//AfxMessageBox(szPath);
			char * ch, *ch1 = szPath;
			ch = strchr(szPath, '\"');
			if(ch!=NULL)
			{
				ch1 = ch+1;
				ch = strchr(ch1, '\"');
				if(ch!=NULL)
					*ch ='\0';
			}
			else
			{
				ch = strchr(szPath, ' ');
				if(ch!=NULL)
					*ch = '\0';
			}
			if(ch!=NULL)
				ch++;
			ShellExecute(NULL, "open", ch1 , ch, ".", SW_SHOW );
		}
	}
	RegCloseKey(hKey);
}

void CMainFrame::OnToolsCleanCookie() 
{
	// TODO: Add your command handler code here
	DelTempFiles(COOKIE_CACHE_ENTRY);
}

void CMainFrame::OnToolsCache() 
{
	// TODO: Add your command handler code here
	DelTempFiles(COOKIE_CACHE_ENTRY, FALSE);
}

void CMainFrame::OnToolsAll() 
{
	// TODO: Add your command handler code here
	if(AfxMessageBox(IDS_CLEAN_ALL, MB_YESNO|MB_ICONQUESTION)==IDNO)
		return;
	OnToolsCleanaddr();
	DelTempFiles(0);
	OnToolsHistory();
	OnToolCleanmyieform();
}

void CMainFrame::OnToolsCleanaddr() 
{
	// TODO: Add your command handler code here
		//clear history in memory
	m_wndAddress->ResetContent();

	//clean typed urls
	HKEY            hKey;
	TCHAR			id[9] = "url";
	int				i = 1;

	if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\TypedUrls"), &hKey) != ERROR_SUCCESS)
	{
		TRACE0("Typed URLs not found\n");
		return ;
	}

	itoa(i, id+3, 10);

	while(RegDeleteValue(hKey, _T(id)) == ERROR_SUCCESS)
	{
		i++;
		itoa(i, id+3, 10);
	}
	RegCloseKey(hKey);	
}

void CMainFrame::OnToolsHistory() 
{
	// TODO: Add your command handler code here
	IUrlHistoryStg2* pUrlHistoryStg2 = NULL;
    HRESULT hr = CoCreateInstance(CLSID_CUrlHistory,
        NULL, CLSCTX_INPROC, IID_IUrlHistoryStg2,
        (void**)&pUrlHistoryStg2);
    if (SUCCEEDED(hr))
    {     
        hr = pUrlHistoryStg2->ClearHistory(); 
        pUrlHistoryStg2->Release();
		//clean history list
		if(HasExpBar(IDS_TITLE_HIS))
			m_pwndExpObj->Initialize();
    }


	//clean typed urls
	HKEY            hKey;
	TCHAR			id[9] = "url";
	int				i = 1;

	if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\TypedUrls"), &hKey) != ERROR_SUCCESS)
	{
		TRACE0("Typed URLs not found\n");
		return ;
	}

	itoa(i, id+3, 10);

	while(RegDeleteValue(hKey, _T(id)) == ERROR_SUCCESS)
	{
		i++;
		itoa(i, id+3, 10);
		//clear history in memory
		m_wndAddress->DeleteString(0);
	}
	RegCloseKey(hKey);
	
//more history under HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Internet Settings\5.0\Cache\Extensible Cache\MSHist******

}

BOOL CMainFrame::DelTempFiles(DWORD CacheType, BOOL op)
{
    BOOL bResult = FALSE;
    BOOL bDone = FALSE;
    LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry = NULL;  
 
    DWORD  dwTrySize, dwEntrySize = 4096; // start buffer size    
    HANDLE hCacheDir = NULL;    
    DWORD  dwError = ERROR_INSUFFICIENT_BUFFER;
    
    do 
    {                               
        switch (dwError)
        {
            // need a bigger buffer
            case ERROR_INSUFFICIENT_BUFFER: 
                delete [] lpCacheEntry;            
                lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwEntrySize];
                lpCacheEntry->dwStructSize = dwEntrySize;
                dwTrySize = dwEntrySize;
                BOOL bSuccess;
                if (hCacheDir == NULL)                
                  
                    bSuccess = (hCacheDir 
                      = FindFirstUrlCacheEntry(NULL, lpCacheEntry,
                      &dwTrySize)) != NULL;
                else
                    bSuccess = FindNextUrlCacheEntry(hCacheDir, lpCacheEntry, &dwTrySize);

                if (bSuccess)
                    dwError = ERROR_SUCCESS;    
                else
                {
                    dwError = GetLastError();
                    dwEntrySize = dwTrySize; // use new size returned
                }
                break;

             // we are done
            case ERROR_NO_MORE_ITEMS:
                bDone = TRUE;
                bResult = TRUE;                
                break;

             // we have got an entry
            case ERROR_SUCCESS:                       
        
                // if CacheType=0 then delete all, else only delete CacheType
				if(op)
				{
					if (CacheType==0 || (lpCacheEntry->CacheEntryType & CacheType))                
                          DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
				}
				else
				{
					if (CacheType==0 || !(lpCacheEntry->CacheEntryType & CacheType))                
                          DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
				}

                // get ready for next entry
                dwTrySize = dwEntrySize;
                if (FindNextUrlCacheEntry(hCacheDir, lpCacheEntry, &dwTrySize))
                    dwError = ERROR_SUCCESS;          
     
                else
                {
                    dwError = GetLastError();
                    dwEntrySize = dwTrySize; // use new size returned
                }                    
                break;

            // unknown error
            default:
                bDone = TRUE;                
                break;
        }

        if (bDone)
        {   
            delete [] lpCacheEntry; 
            if (hCacheDir)
                FindCloseUrlCache(hCacheDir);         
                                  
        }
    } while (!bDone);
	return bResult;

}

//#pragma optimize( "s", off)

void CMainFrame::OnFonts(UINT nID)
{
	CMyIEView * pvw = NULL;
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	if (tcf!=NULL)
		pvw = (CMyIEView*)tcf->m_pView;
	if(pvw == NULL)
		return;

	nDefFontSize = pvw->m_nFontSize = nID-ID_VIEW_FONTS_LARGEST;
	long font = 4- nDefFontSize;
	COleVariant vaZoomFactor(font);

	pvw->ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);

}


/*
void CMainFrame::OnOptionTabTop() 
{
	// TODO: Add your command handler code here
	int r = m_wndTab.GetRowCount();
	if(r==0)
		r=1;
	m_nPreLines = r;

	if(m_bTopTab==0)
	{//AfxMessageBox(IDS_CHANGE_POS);


		//hide the bottom bar
		m_wndReBar2.GetReBarCtrl( ).DeleteBand(0);

		m_wndReBar.AddBar(&m_wndTab,NULL, NULL,  RBBS_FIXEDBMP | RBBS_BREAK |RBBS_GRIPPERALWAYS);
		GetMenu()->CheckMenuItem(ID_VIEW_TASKBAR, MF_CHECKED);

		REBARBANDINFO rbbi;
		// set up min/max sizes and ideal sizes for pieces of the rebar
		CRect rectAddress;

		m_wndAddress->GetEditCtrl()->GetWindowRect(&rectAddress);

		rbbi.cbSize = sizeof(rbbi);
		rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE |RBBIM_ID;
		rbbi.wID = ID_VIEW_TASKBAR;
		rbbi.cxMinChild = 10;
		m_wndAddress->GetEditCtrl()->GetWindowRect(&rectAddress);
		rbbi.cyMinChild = (rectAddress.Height() + 10)*r;
		/*rbbi.cx=* /rbbi.cxIdeal = 10;
		m_wndReBar.GetReBarCtrl().SetBandInfo(7, &rbbi);
		ShowBackground(TRUE);
	}

	m_bTopTab = 1 ;

}

void CMainFrame::OnOptionTabBot() 
{
	// TODO: Add your command handler code here
	int r = m_wndTab.GetRowCount();
	if(r==0)
		r=1;
	m_nPreLines = r;

	if(m_bTopTab ==1)
	{
		//AfxMessageBox(IDS_CHANGE_POS);
		//deleete the band in top bar
		int nb = m_wndReBar.GetReBarCtrl( ).IDToIndex(ID_VIEW_TASKBAR);
		if(nb<0)
			return;

		m_wndReBar.GetReBarCtrl( ).DeleteBand(nb);

		m_wndReBar2.AddBar(&m_wndTab);
		GetMenu()->CheckMenuItem(ID_VIEW_TASKBAR, MF_CHECKED);

		REBARBANDINFO rbbi;
		// set up min/max sizes and ideal sizes for pieces of the rebar
		CRect rectAddress;

		m_wndAddress->GetEditCtrl()->GetWindowRect(&rectAddress);

		rbbi.cbSize = sizeof(rbbi);
		rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE |RBBIM_ID;
		rbbi.wID = ID_VIEW_TASKBAR;
		rbbi.cxMinChild = 0;
		m_wndAddress->GetEditCtrl()->GetWindowRect(&rectAddress);
		rbbi.cyMinChild = (rectAddress.Height() + 10)*r;
		rbbi.cxIdeal = 200;
		m_wndReBar2.GetReBarCtrl().SetBandInfo(0, &rbbi);
	}

	m_bTopTab = 0;
}*/

void CMainFrame::OnToolsExternalUtilitymanager() 
{
	// TODO: Add your command handler code here
	CExternalUtils dlg;
	if(dlg.DoModal()==IDOK)
	{
		//update menus
		BuildUtilMenu();

		m_bSaveConfig = TRUE;
	}
}

//#pragma optimize( "s", on)

void CMainFrame::OnOptionsSetasdefaultbrowser() 
{
	// TODO: Add your command handler code here
		// find out from the registry where the favorites are located.
	TCHAR           sz[MAX_PATH];
	HKEY            hKey;
	DWORD           dwSize = MAX_PATH;

	BOOL setdef=FALSE;
	int re = AfxMessageBox(IDS_SET_DEFAULT, MB_YESNOCANCEL|MB_ICONQUESTION);
	if(re==IDYES)
	{
		setdef=TRUE;
	}
	else if(re == IDCANCEL)
		return;


	//htm,html
	if(RegOpenKey(HKEY_CLASSES_ROOT, _T(".htm"), &hKey) != ERROR_SUCCESS)
	{
	}
	else
	{
		dwSize = sizeof(sz);
//		RegQueryValueEx(hKey, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);

//		if(strcmp(sz, "htmlfile")!=0)
		{
			//set back the default value.
			RegSetValueEx(hKey, _T(""), NULL, REG_SZ, (LPBYTE)("htmlfile"), 9);

		}
		RegCloseKey(hKey);
	}

	if(RegOpenKey(HKEY_CLASSES_ROOT, _T(".html"), &hKey) != ERROR_SUCCESS)
	{
	}
	else
	{
		dwSize = sizeof(sz);
		*sz='\0';
//		RegQueryValueEx(hKey, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);

//		if(strcmp(sz, "htmlfile")!=0)
		{
			//set back the default value.
			RegSetValueEx(hKey, _T(""), NULL, REG_SZ, (LPBYTE)("htmlfile"), 9);

		}
		RegCloseKey(hKey);
	}

	//shell
	if(RegOpenKey(HKEY_CLASSES_ROOT, _T("htmlfile\\shell"), &hKey) != ERROR_SUCCESS)
	{
	}
	else
	{
		if(setdef)
		{
			//save the old one
			dwSize = sizeof(sz);
			*sz='\0';
			RegQueryValueEx(hKey, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);

			RegSetValueEx(hKey, _T(""), NULL, REG_SZ, (LPBYTE)("open"), 4);
				
			if(strcmp(sz, "open")!=0)
				AfxGetApp()->WriteProfileString("Settings", "oldshell", sz);
		}
		else
		{
			CString t = AfxGetApp()->GetProfileString("Settings", "oldshell");
			if(!t.IsEmpty())
				RegSetValueEx(hKey, _T(""), NULL, REG_SZ, (LPBYTE)((LPCSTR)t), t.GetLength()+1);
		}

		RegCloseKey(hKey);
	}

	//set the key value of default browser, open
	SetDefault(_T("htmlfile\\shell"), setdef);

	//url
	if(RegOpenKey(HKEY_CLASSES_ROOT, _T(".url"), &hKey) != ERROR_SUCCESS)
	{
	}
	else
	{
		dwSize = sizeof(sz);
		RegQueryValueEx(hKey, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);

		if(strcmp(sz, "InternetShortcut")!=0)
		{
			//set back the default value.
			RegSetValueEx(hKey, _T(""), NULL, REG_SZ, (LPBYTE)("InternetShortcut"), 16);

		}
		RegCloseKey(hKey);
	}
	SetDefault(_T("InternetShortcut\\shell"), setdef);


	//mht, mhtml
	if(RegOpenKey(HKEY_CLASSES_ROOT, _T(".mht"), &hKey) != ERROR_SUCCESS)
	{
	}
	else
	{
		dwSize = sizeof(sz);
		RegQueryValueEx(hKey, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);

		if(strcmp(sz, "mhtmlfile")!=0)
		{
			//set back the default value.
			RegSetValueEx(hKey, _T(""), NULL, REG_SZ, (LPBYTE)("mhtmlfile"), 10);

		}
		RegCloseKey(hKey);
	}


	if(RegOpenKey(HKEY_CLASSES_ROOT, _T(".mhtml"), &hKey) != ERROR_SUCCESS)
	{
	}
	else
	{
		dwSize = sizeof(sz);
		*sz='\0';
		RegQueryValueEx(hKey, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);

		if(strcmp(sz, "mhtmlfile")!=0)
		{
			//set back the default value.
			RegSetValueEx(hKey, _T(""), NULL, REG_SZ, (LPBYTE)("mhtmlfile"), 10);

		}
		RegCloseKey(hKey);
	}

	//set the key value of default browser, open
	SetDefault(_T("mhtmlfile\\shell"), setdef);

	//http
	SetDefault( _T("http\\shell"), setdef);

	//https
	SetDefault( _T("https\\shell"), setdef);

	//file
	SetDefault( _T("file\\shell"), setdef);
		
	//ftp
	SetDefault( _T("ftp\\shell"), setdef);

	//shtm, shtml
	if(setdef)
	{
		if(RegOpenKey(HKEY_CLASSES_ROOT, _T(".shtm"), &hKey) != ERROR_SUCCESS)
		{
			return ;
		}
		dwSize = sizeof(sz);
		RegQueryValueEx(hKey, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);

		if(strcmp(sz, "htmlfile")!=0)
		{
			//set back the default value.
			RegSetValueEx(hKey, _T(""), NULL, REG_SZ, (LPBYTE)("htmlfile"), 9);

		}
		RegSetValueEx(hKey, _T("Content Type"), NULL, REG_SZ, (LPBYTE)("text/html"), 10);
		RegCloseKey(hKey);

		if(RegOpenKey(HKEY_CLASSES_ROOT, _T(".shtml"), &hKey) != ERROR_SUCCESS)
		{
			return ;
		}
		dwSize = sizeof(sz);
		*sz='\0';
		RegQueryValueEx(hKey, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);

		if(strcmp(sz, "htmlfile")!=0)
		{
			//set back the default value.
			RegSetValueEx(hKey, _T(""), NULL, REG_SZ, (LPBYTE)("htmlfile"), 9);

		}
		RegSetValueEx(hKey, _T("Content Type"), NULL, REG_SZ, (LPBYTE)("text/html"), 10);
		RegCloseKey(hKey);

	}


}

void CMainFrame::OnAddrDrop() 
{
	// TODO: Add your command handler code here
	m_wndAddress->SetFocus();
	m_wndAddress->ShowDropDown(!m_wndAddress->GetDroppedState());
}

void CMainFrame::OnAddrFocus() 
{
	// TODO: Add your command handler code here
	m_wndAddress->SetFocus();
}


void CMainFrame::OnHelpHelp() 
{
	// TODO: Add your command handler code here
    HINSTANCE hInstance = AfxGetResourceHandle();
    ASSERT(hInstance != NULL);

    BOOL bRetVal = TRUE;
//    LPTSTR lpszModule = new TCHAR[_MAX_PATH];
	TCHAR lpszModule[_MAX_PATH];

    if (GetModuleFileName(hInstance, lpszModule, _MAX_PATH))
    {
		CString strHelp(lpszModule);
		int i = strHelp.ReverseFind('\\');
		strHelp = strHelp.Left(i+1);
		strHelp += "myiehelp.htm";
		CString strURL = strHelp;
		//nOpenType=2;
		//PostMessage(WM_COMMAND, ID_FILE_NEW_S);
		NewChildWindow(1,2,strURL);
    }

 //   delete [] lpszModule;
}

void CMainFrame::OnOptionsActivenewwindow() 
{
	// TODO: Add your command handler code here
	if(m_bActiveNewWindow )
		m_bActiveNewWindow = FALSE;
	else
		m_bActiveNewWindow = TRUE;

	m_bmUserDefault.m_bActiveNewWindow = m_bActiveNewWindow;
	m_bmUserDefault.m_bAlwaysNewWindow = bAddressInNewWindow;
	m_bmUserDefault.m_nSeq = m_nSeq;
	m_bmUserDefault.bLinkInNewWin = bLinkInNewWindow;
}


/*void CMainFrame::OnOptionsMyiestartupOpenblankpage() 
{
	// TODO: Add your command handler code here
	bGoHome = 0;
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIEGOHOME,MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_OPENBLANKPAGE,MF_CHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_RESUMELASTVISITEDPAGES,MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_OPENFAVORITEPAGES,MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_OPENMOSTFAVORITEPAGES,MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_OPENSTARTUPGROUP,MF_UNCHECKED);
}

void CMainFrame::OnOptionsMyiestartupOpenfavoritepages() 
{
	// TODO: Add your command handler code here
	bGoHome = 3;
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIEGOHOME,MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_OPENBLANKPAGE,MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_RESUMELASTVISITEDPAGES,MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_OPENFAVORITEPAGES,MF_CHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_OPENMOSTFAVORITEPAGES,MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_OPENSTARTUPGROUP,MF_UNCHECKED);
}

void CMainFrame::OnOptionsMyiestartupResumelastvisitedpages() 
{
	// TODO: Add your command handler code here
	bGoHome = 2;
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIEGOHOME,MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_OPENBLANKPAGE,MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_RESUMELASTVISITEDPAGES,MF_CHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_OPENFAVORITEPAGES,MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_OPENMOSTFAVORITEPAGES,MF_UNCHECKED);
}*/

BOOL CMainFrame::SetDefault(LPCSTR lpszKey, BOOL setdef)
{
	TCHAR           sz[MAX_PATH];
	HKEY            hKey, hSubKey, hSubKey2;
	DWORD           dwSize = MAX_PATH;
	if(RegOpenKey(HKEY_CLASSES_ROOT, lpszKey, &hKey) != ERROR_SUCCESS)
	{
		return FALSE;
	}
	else
	{
		if(setdef)
		{
			DWORD dwRes;
			dwSize = sizeof(sz);
			*sz='\0';
			char lpszModule[_MAX_PATH];
			HINSTANCE hInstance = AfxGetInstanceHandle();

			RegSetValueEx(hKey, _T(""), NULL, REG_SZ, (LPBYTE)("MyIE"),5);

			//open
			RegCreateKeyEx(hKey, "open", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hSubKey, &dwRes);
			RegCreateKeyEx(hSubKey, "command", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hSubKey2, &dwRes);

			if (GetModuleFileName(hInstance, lpszModule+1, _MAX_PATH))
			{
				*lpszModule = '\"';
				strcat(lpszModule, "\" \"%1\"");
				RegSetValueEx(hSubKey2, _T(""), NULL, REG_SZ, (LPBYTE)(lpszModule), strlen(lpszModule));
				
			}

			RegCloseKey(hSubKey);
			RegCloseKey(hSubKey2);
			

			//myie
			RegCreateKeyEx(hKey, "MyIE", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hSubKey, &dwRes);
			RegCloseKey(hKey);
			RegCreateKeyEx(hSubKey, "command", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hKey, &dwRes);


			if (GetModuleFileName(hInstance, lpszModule+1, _MAX_PATH))
			{
				*lpszModule = '\"';
				strcat(lpszModule, "\" \"%1\"");
				RegSetValueEx(hKey, _T(""), NULL, REG_SZ, (LPBYTE)(lpszModule), strlen(lpszModule));
				
			}

			RegCloseKey(hKey);
			RegCloseKey(hSubKey);


		}
		else
		{
			CString t = AfxGetApp()->GetProfileString("Settings", "oldshell");
			if(strcmp(lpszKey, "mhtmlfile\\shell")==0)
				t="";
			if(!t.IsEmpty())
				RegSetValueEx(hKey, _T(""), NULL, REG_SZ, (LPBYTE)((LPCSTR)t), t.GetLength()+1);
		}

		RegCloseKey(hKey);

		return TRUE;
	}

}


void CMainFrame::OnViewLinks() 
{
	// TODO: Add your command handler code here
	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_ID|RBBIM_STYLE;
	int nb = m_wndReBar.GetReBarCtrl( ).IDToIndex(ID_VIEW_LINKS);
	m_wndReBar.GetReBarCtrl().GetBandInfo(nb, &rbbi);

	if( rbbi.fStyle & RBBS_HIDDEN  || !m_LinkBar.IsWindowVisible())
	{
		m_LinkBar.ShowWindow(SW_SHOW);
		m_wndReBar.GetReBarCtrl( ).ShowBand(nb, TRUE);
		GetMenu()->CheckMenuItem(ID_VIEW_LINKS, MF_CHECKED);

	}
	else
	{
		m_wndReBar.GetReBarCtrl( ).ShowBand(nb,FALSE);
		m_LinkBar.ShowWindow(SW_HIDE);
		GetMenu()->CheckMenuItem(ID_VIEW_LINKS, MF_UNCHECKED);
	}
	
}

void CMainFrame::OnViewToolbar() 
{
	// TODO: Add your command handler code here
	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_ID|RBBIM_STYLE;
	int nb = m_wndReBar.GetReBarCtrl( ).IDToIndex(ID_VIEW_TOOLBAR);
	m_wndReBar.GetReBarCtrl().GetBandInfo(nb, &rbbi);

	if( rbbi.fStyle & RBBS_HIDDEN)
	{
		m_wndToolBar.ShowWindow(SW_SHOW);
		m_wndReBar.GetReBarCtrl( ).ShowBand(nb, TRUE);
		GetMenu()->CheckMenuItem(ID_VIEW_TOOLBAR, MF_CHECKED);

	}
	else
	{
		m_wndReBar.GetReBarCtrl( ).ShowBand(nb,FALSE);
		m_wndToolBar.ShowWindow(SW_HIDE);
		GetMenu()->CheckMenuItem(ID_VIEW_TOOLBAR, MF_UNCHECKED);
	}
}

void CMainFrame::OnHelpMail() 
{
	// TODO: Add your command handler code here

	DefaultMail("mailto:myie@gmx.net");
}

void CMainFrame::OnHelpForum() 
{
	// TODO: Add your command handler code here
	NewChildWindow(1,2,"http://forum.isfamous.com/");
}

void CMainFrame::OnFileNewClip() 
{
	// TODO: Add your command handler code here
	if ( !OpenClipboard() )
	{
		AfxMessageBox( "Cannot open the Clipboard" );
		return;
	}

	HGLOBAL hData;

	// For the appropriate data formats...
	hData = ::GetClipboardData( CF_TEXT);
	if ( hData == NULL )
	{
		//AfxMessageBox( "Unable to get Clipboard data" );
		CloseClipboard();
		//GlobalFree(hData);
		NewChildWindow(1,2,"about:blank", NULL, TRUE);
		return;
	}
	// ...
	char *strURL = (char*)GlobalLock(hData);

	NewChildWindow(1,2,strURL);
	GlobalUnlock(hData);
	CloseClipboard();
	GlobalFree(hData);
	m_wndAddress->SetFocus();
}



void CMainFrame::OnViewMenubar() 
{
	// TODO: Add your command handler code here
	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_ID|RBBIM_STYLE;
	int nb = m_wndReBar.GetReBarCtrl( ).IDToIndex(ID_MENU_BAR);
	m_wndReBar.GetReBarCtrl().GetBandInfo(nb, &rbbi);

	if( rbbi.fStyle & RBBS_HIDDEN )
	{
		m_wndMenuBar.ShowWindow(SW_SHOW);
		m_wndReBar.GetReBarCtrl( ).ShowBand(nb, TRUE);
		GetMenu()->CheckMenuItem(ID_VIEW_MENUBAR, MF_CHECKED);

	}
	else
	{
		m_wndReBar.GetReBarCtrl( ).ShowBand(nb,FALSE);
		m_wndMenuBar.ShowWindow(SW_HIDE);
	}

}

void CMainFrame::ShowBackground(BOOL bShow)
{
	if(m_strBGPic == "" || m_strBGPic.IsEmpty())
		bShow = FALSE;
	
	try
	{

	if (bShow) {
		// show bitmap: load if needed
		if (m_bmBGPic) {
			DeleteObject(m_bmBGPic);
			m_bmBGPic= NULL;
		}

			// load bitmap
		m_bmBGPic = (HBITMAP)LoadImage(NULL, ((CMyIEApp*)AfxGetApp())->m_strBitmapPath+ m_strBGPic, IMAGE_BITMAP, 0, 0, 
				LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE); 
	} else {
		// hide bitmap: delete if needed
		if (m_bmBGPic) {
			DeleteObject(m_bmBGPic); // delete bitmap
		m_bmBGPic = NULL;
		m_strBGPic = "";
		}
	}

	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);
	int n = m_wndReBar.GetReBarCtrl().GetBandCount();
	for (int i=0; i< n; i++) {
		rbbi.fMask  = RBBIM_STYLE;
		m_wndReBar.GetReBarCtrl().GetBandInfo(i, &rbbi);
		rbbi.fMask  |= RBBIM_BACKGROUND;
		rbbi.fStyle |= RBBS_FIXEDBMP;
		rbbi.hbmBack = m_bmBGPic;
		m_wndReBar.GetReBarCtrl().SetBandInfo(i, &rbbi);

		rbbi.fMask = RBBIM_CHILD | RBBIM_ID;
		m_wndReBar.GetReBarCtrl().GetBandInfo ( i, &rbbi );
		CWnd * pWnd = CWnd::FromHandle(rbbi.hwndChild);
		if(pWnd!=NULL )
			pWnd->Invalidate();
	}
	m_wndReBar.Invalidate();
	m_wndAddress->GetEditCtrl()->Invalidate();
	}
/*	catch(CMemoryException * e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}
}

void CMainFrame::OnToolsSetbackgroundpic(UINT nID) 
{
	// TODO: Add your command handler code here
	int n = m_astrBGPics.GetCount();
	if(nID == ID_TOOLS_SETBACKGROUNDPIC_NONE)
		ShowBackground(FALSE);
	else
	{
		int m = nID - 0xe00;
		if(m>=0 && m<n)
		{
			POSITION pos = m_astrBGPics.FindIndex(m);
			if(pos !=NULL)
				m_strBGPic = m_astrBGPics.GetAt(pos);
		}
		ShowBackground(TRUE);
	}
	CMenu* pMenu = GetMenu();
	if(pMenu!=NULL)
	{
		pMenu->CheckMenuItem(ID_TOOLS_SETBACKGROUNDPIC_NONE, MF_UNCHECKED);
		for(int i=0; i<n; i++)
			pMenu->CheckMenuItem(0xe00+i, MF_UNCHECKED);
		pMenu->CheckMenuItem(nID, MF_CHECKED);
	}
}


void CMainFrame::OnShortcutSc() 
{
	// TODO: Add your command handler code here
	CSetSCDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnShortcutManalias() 
{
	// TODO: Add your command handler code here
	CSetAliasDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnViewToolbarsCustomize() 
{
	// TODO: Add your command handler code here
	try{
	CCustomizeToolbar dlg;
	dlg.m_nTextOpt = m_nTextLabel;
	dlg.m_bColorIcon = m_bColorIcon;
	dlg.m_bSmallToolBar = m_bSmallToolBar;
	if(dlg.DoModal()==IDOK)
	{
		m_nTextLabel = dlg.m_nTextOpt;
		if(m_bColorIcon != dlg.m_bColorIcon || dlg.m_bSmallToolBar != m_bSmallToolBar)
		{
			m_bSmallToolBar = dlg.m_bSmallToolBar;
			m_bColorIcon = dlg.m_bColorIcon;
			InitialToolBar(TRUE);
		}
		else
		{
			m_bSmallToolBar = dlg.m_bSmallToolBar;
			m_bColorIcon = dlg.m_bColorIcon;
			InitialToolBar();
		}

	}
	}catch(...)
	{
	}
}

void CMainFrame::OnOptionsMyieoptions() 
{
	// TODO: Add your command handler code here
	CWindowProp winprop;
	CTabProp tabprop;
	CGeneralProp genprop;
	CDownloadProp downprop;
	COtherProp otherprop;
	CSaveProp saveprop;
	CPropertySheet sheet(IDS_MYIESHEET,this);

	sheet.AddPage(&genprop);
	sheet.AddPage(&winprop);
	sheet.AddPage(&tabprop);
	sheet.AddPage(&downprop);
	sheet.AddPage(&saveprop);
	sheet.AddPage(&otherprop);


	HKEY            hKey;
	TCHAR           sz[MAX_PATH];
	DWORD           dwSize = MAX_PATH;

	if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\Main"), &hKey) != ERROR_SUCCESS)
	{
		TRACE0("Main setting not found\n");
		return ;
	}
	dwSize = sizeof(sz);
	RegQueryValueEx(hKey, _T("Display Inline Images"), NULL, NULL, (LPBYTE)sz, &dwSize);
	if(strcmp(sz, "yes")==0)
		downprop.m_bDLImage=TRUE;
	
	dwSize = MAX_PATH;
	RegQueryValueEx(hKey, _T("Display Inline Videos"), NULL, NULL, (LPBYTE)sz, &dwSize);
	if(strcmp(sz, "yes")==0)
		downprop.m_bDLVideo = TRUE;

	dwSize = MAX_PATH;
	RegQueryValueEx(hKey, _T("Play_Background_Sounds"), NULL, NULL, (LPBYTE)sz, &dwSize);
	if(strcmp(sz, "yes")==0)
		downprop.m_bDLSound = TRUE;

	dwSize = MAX_PATH;
	RegQueryValueEx(hKey, _T("Play_Animations"), NULL, NULL, (LPBYTE)sz, &dwSize);
	if(strcmp(sz, "yes")==0)
		downprop.m_bDLAnimation = TRUE;

	RegCloseKey(hKey);

	if(!AfxGetApp()->GetProfileInt("Settings", "NoJava", 0))
		downprop.m_bDLApplet = TRUE;

	if(!AfxGetApp()->GetProfileInt("Settings", "NoActivex", 0))
		downprop.m_bDLActivex = TRUE;

	if(!AfxGetApp()->GetProfileInt("Settings", "NoScript", 0))
		downprop.m_bDLScript = TRUE;
	downprop.m_nSelectedID = m_DList.GetIDFromName(m_strDefaultDLT)+1;
	
	
	genprop.m_bConfirmExit = bConfirmExit;
	genprop.m_bSingleMenu = bSingleMenu;
	genprop.m_bSysTray = bSysTray;
	genprop.m_bAniIcon = m_bAnimatIcon;
	genprop.m_bCompactFavBar = bCompactFavBar;
	genprop.m_nMenuSize = m_nMenuWidth;
	genprop.m_bKeepSilent = bSilent;
	genprop.m_bConfirmCloseAll = bConfirmCloseAll;
	genprop.m_bOneInstance = bOneInstance;
	genprop.m_bDisplayOpenAll = m_bDisplayOpenAll;
	genprop.m_bAddFav = m_bDisplayAddFav;
	genprop.m_bMouseGesture = m_bAllowMouseGestures;
	genprop.m_bLockHomepage = bLockHomepage;
	genprop.m_strSkinPath = ((CMyIEApp*)AfxGetApp())->m_strBitmapPath;
	genprop.m_strGroupPath = ((CMyIEApp*)AfxGetApp())->m_strGroupPath;

	tabprop.m_nMaxChars = m_nTitleLen;
	tabprop.m_nMaxWidth = m_nMaxTabWidth;
	tabprop.m_nMinWidth = m_nMinTabWidth;
	tabprop.m_nLDBClick	= m_nAllowDBC;
	tabprop.m_nMCLICK = m_nAllowMC;
	tabprop.m_nRClick = m_nAllowRC;
	tabprop.m_bAutoFitTab = m_bAutoTab;
	tabprop.m_nTabPos = m_bTopTab;
	tabprop.m_bMultiTab = m_bMultiLineTab;
	tabprop.m_bNextTab = m_bNextTab;
	tabprop.m_bFBTab = m_bFBTab;

	saveprop.m_strSavePath = ((CMyIEApp*)AfxGetApp())->m_strDefaultDir;
	saveprop.m_strImagePath = ((CMyIEApp*)AfxGetApp())->m_strImagePath;
	saveprop.m_nAutoSave = m_nAutoSave;
	saveprop.m_bCateWithSite = m_bCateWithSite;


	otherprop.m_strCE1 = m_strCE1;
	otherprop.m_strCE2 = m_strCE2;
	otherprop.m_strSE1 = m_strSE1;
	otherprop.m_strSE2 = m_strSE2;
	otherprop.m_strCSE1 = m_strCSE1;
	otherprop.m_strCSE2 = m_strCSE2;
	otherprop.m_nStartup = bGoHome;

	winprop.m_nMaxWins = nMaxWins;
	winprop.m_nInterval = m_nInterval;
	winprop.m_nOpenNew = m_nNewWindow;
	winprop.m_nOpenNew2 = m_nNewWindow2;
	winprop.m_bActiveNew = m_bActiveNewWindow;
	winprop.m_bOpenAddrInNew = bAddressInNewWindow;
	winprop.m_bOpenFavInNew = bFavInNewWindow;
	winprop.m_bOpenFileInNew = bFileInNewWindow;
	winprop.m_bOpenHisInNew = bHisInNewWindow;
	winprop.m_bOpenLinkInNew = bLinkInNewWindow;
	winprop.m_nSeq = m_nSeq;


	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	if(sheet.DoModal() == IDOK)
	{

		if(downprop.m_nSelectedID==0)
		{
			m_strDefaultDLT = "";
			SetDownloadTool(m_strDefaultDLT);
		}
		else
		{
			if(SetDownloadTool(m_DList.m_DownloadTools.GetAt(downprop.m_nSelectedID-1)->ProgID))
				m_strDefaultDLT = m_DList.m_DownloadTools.GetAt(downprop.m_nSelectedID-1)->DName;
		}
		if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\Main"), &hKey) != ERROR_SUCCESS)
		{
			TRACE0("Main setting not found\n");
			return ;
		}
	
		char yes[] = "yes", no[]="no";

		if(downprop.m_bDLImage)
			RegSetValueEx(hKey, _T("Display Inline Images"), 0, REG_SZ, (LPBYTE)yes, 3);
		else
			RegSetValueEx(hKey, _T("Display Inline Images"), 0, REG_SZ, (LPBYTE)no, 2);
		if(downprop.m_bDLVideo)
			RegSetValueEx(hKey, _T("Display Inline Videos"), 0, REG_SZ, (LPBYTE)yes, 3);
		else
			RegSetValueEx(hKey, _T("Display Inline Videos"), 0, REG_SZ, (LPBYTE)no, 2);
		if(downprop.m_bDLSound)
			RegSetValueEx(hKey, _T("Play_Background_Sounds"), 0, REG_SZ, (LPBYTE)yes, 3);
		else
			RegSetValueEx(hKey, _T("Play_Background_Sounds"), 0, REG_SZ, (LPBYTE)no, 2);
		if(downprop.m_bDLAnimation)
			RegSetValueEx(hKey, _T("Play_Animations"), 0, REG_SZ, (LPBYTE)yes, 3);
		else
			RegSetValueEx(hKey, _T("Play_Animations"), 0, REG_SZ, (LPBYTE)no, 2);


		AfxGetApp()->WriteProfileInt("Settings", "NoScript", !downprop.m_bDLScript);
		AfxGetApp()->WriteProfileInt("Settings", "NoActivex", !downprop.m_bDLActivex);
		AfxGetApp()->WriteProfileInt("Settings", "NoJava", !downprop.m_bDLApplet);

		if(m_bDisplayOpenAll != genprop.m_bDisplayOpenAll)
			m_bFavDirty = TRUE;
		m_bDisplayOpenAll = genprop.m_bDisplayOpenAll;
		if(m_bDisplayAddFav != genprop.m_bAddFav)
			m_bFavDirty = TRUE;
		m_bDisplayAddFav = genprop.m_bAddFav;
		bConfirmExit = genprop.m_bConfirmExit;
		if(bSingleMenu != genprop.m_bSingleMenu)
			m_bFavDirty = TRUE;
		bSingleMenu = genprop.m_bSingleMenu;
		bSysTray = genprop.m_bSysTray;
		if(m_bAnimatIcon && m_bAnimatIcon != genprop.m_bAniIcon)
			m_animIcon.ShowFirstImage();
		m_bAnimatIcon = genprop.m_bAniIcon;
		if(bCompactFavBar != genprop.m_bCompactFavBar)
		{
			m_bFavDirty = TRUE;
			if(bCompactFavBar)
			{
				//CBitmap bitmap;
				//bitmap.LoadBitmap(IDB_BITMAP1);
				CImageList img;
				img.Create(16, 16, ILC_COLORDDB|ILC_MASK, 7, 1);
				HBITMAP hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP1));
				ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
				m_FavBar.GetToolBarCtrl().SetImageList(&img);
				img.Detach();
				//bitmap.Detach();
			}
			else
				m_FavBar.GetToolBarCtrl().SetImageList(NULL);
		}
		bCompactFavBar = genprop.m_bCompactFavBar;
		m_nMenuWidth = genprop.m_nMenuSize;
		if(bSilent != genprop.m_bKeepSilent && genprop.m_bKeepSilent)
			RegSetValueEx(hKey, _T("Disable Script Debugger"), 0, REG_SZ, (LPBYTE)yes, 3);

		bSilent = genprop.m_bKeepSilent;
		RegCloseKey(hKey);		

		bConfirmCloseAll = genprop.m_bConfirmCloseAll;
		bOneInstance = genprop.m_bOneInstance;
		m_bAllowMouseGestures = genprop.m_bMouseGesture;
		if(genprop.m_bLockHomepage != bLockHomepage && genprop.m_bLockHomepage)
		{
			GetHomePage(m_strHome);
		}
		bLockHomepage = genprop.m_bLockHomepage;

		m_nAllowDBC	= tabprop.m_nLDBClick;
		m_nAllowMC = tabprop.m_nMCLICK;
		m_nAllowRC = tabprop.m_nRClick;
		m_nTitleLen = tabprop.m_nMaxChars;
		m_nMaxTabWidth = tabprop.m_nMaxWidth;
		if(tabprop.m_nMaxWidth>tabprop.m_nMinWidth)
			m_nMinTabWidth = tabprop.m_nMinWidth;
		//update tab position...
		if(m_bTopTab != tabprop.m_nTabPos)
		{
			ChangTabPos(tabprop.m_nTabPos);
		}
		m_wndTab.m_bTop = m_bTopTab = tabprop.m_nTabPos;
		//update tab style...
		if(m_bMultiLineTab != tabprop.m_bMultiTab)
		{
			OnOptionMultiTab();
			m_bMultiLineTab = tabprop.m_bMultiTab;
		}
		if(m_bAutoTab != tabprop.m_bAutoFitTab)
		{
			OnOptionAutotab();
			m_bAutoTab = tabprop.m_bAutoFitTab;
		}
		if(m_bFBTab != tabprop.m_bFBTab)
		{
			OnOptionFBtab();
			m_bFBTab = tabprop.m_bFBTab;
		}
		m_bNextTab = tabprop.m_bNextTab;

		if(genprop.m_strGroupPath.Right(1) != "\\")
			genprop.m_strGroupPath += "\\";
		((CMyIEApp*)AfxGetApp())->m_strGroupPath = genprop.m_strGroupPath;
		if(genprop.m_strSkinPath.Right(1) != "\\")
			genprop.m_strSkinPath += "\\";
		if(((CMyIEApp*)AfxGetApp())->m_strBitmapPath != genprop.m_strSkinPath)
			AfxGetApp()->WriteProfileString("Settings", "SkinPath", genprop.m_strSkinPath);
		((CMyIEApp*)AfxGetApp())->m_strBitmapPath = genprop.m_strSkinPath;

		if(saveprop.m_strImagePath.Right(1) != "\\")
			saveprop.m_strImagePath += "\\";
		((CMyIEApp*)AfxGetApp())->m_strImagePath = saveprop.m_strImagePath;
		if(!saveprop.m_strSavePath.IsEmpty() && saveprop.m_strSavePath.Right(1) != "\\")
			saveprop.m_strSavePath += "\\";
		((CMyIEApp*)AfxGetApp())->m_strDefaultDir = saveprop.m_strSavePath;
		AfxGetApp()->WriteProfileString("Settings", "DefaultDir", saveprop.m_strSavePath);
		m_nAutoSave = saveprop.m_nAutoSave;
		m_bCateWithSite = saveprop.m_bCateWithSite;

		m_strCE1 = otherprop.m_strCE1;
		m_strCE2 = otherprop.m_strCE2;
		m_strSE1 = otherprop.m_strSE1;
		m_strSE2 = otherprop.m_strSE2;
		m_strCSE1 = otherprop.m_strCSE1;
		m_strCSE2 = otherprop.m_strCSE2;
		bGoHome = otherprop.m_nStartup;

		m_nNewWindow = winprop.m_nOpenNew;
		m_nNewWindow2 = winprop.m_nOpenNew2;
		m_nInterval = 	winprop.m_nInterval;
		nMaxWins = winprop.m_nMaxWins;
		m_bActiveNewWindow = winprop.m_bActiveNew;
		bAddressInNewWindow = winprop.m_bOpenAddrInNew;
		bFavInNewWindow = winprop.m_bOpenFavInNew;
		bFileInNewWindow = winprop.m_bOpenFileInNew;
		bHisInNewWindow = winprop.m_bOpenHisInNew;
		bLinkInNewWindow = winprop.m_bOpenLinkInNew;
		m_nSeq = winprop.m_nSeq;

		//user mode
		m_bmUserDefault.m_bActiveNewWindow = m_bActiveNewWindow;
		m_bmUserDefault.m_bAlwaysNewWindow = bAddressInNewWindow;
		m_bmUserDefault.m_nSeq = m_nSeq;
		m_bmUserDefault.bLinkInNewWin = bLinkInNewWindow;

		CChildFrame* tcf = (CChildFrame*)MDIGetActive();
		if(tcf != NULL && tcf->m_pView!=NULL)
			tcf->m_pView->ChangeProperty();

		m_bSaveConfig = TRUE;
	}
}
/*
void CMainFrame::OnOptionsMyiestartupOpenmostfavpages() 
{
	// TODO: Add your command handler code here
	bGoHome = 4;
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIEGOHOME,MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_OPENBLANKPAGE,MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_RESUMELASTVISITEDPAGES,MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_OPENFAVORITEPAGES,MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_OPENMOSTFAVORITEPAGES,MF_CHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_OPENSTARTUPGROUP,MF_UNCHECKED);
}*/

void CMainFrame::OnRtabStickname() 
{
	// TODO: Add your command handler code here
	CStickName dlg;

	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;

	m_wndTab.GetItem(nRTabID, &TabCtrlItem);
	CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
	
	if(tcf!=NULL)
	{
		dlg.m_strStickName = tcf->m_strStickName;
		if(dlg.DoModal() == IDOK)
		{
			tcf->m_strStickName = dlg.m_strStickName;
			if(tcf->m_strStickName != "")
			{
				SetTabTitle((LPCSTR)dlg.m_strStickName, FindTab(tcf));
				tcf->SetWindowText(dlg.m_strStickName);
			}
			else
			{
				SetTabTitle((LPCSTR)tcf->m_Title, FindTab(tcf));
				tcf->SetWindowText(tcf->m_Title);
			}
		}
	}
}

void CMainFrame::OnViewFavs() 
{
	// TODO: Add your command handler code here
	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_ID|RBBIM_STYLE;
	int nb = m_wndReBar.GetReBarCtrl( ).IDToIndex(ID_VIEW_FAVS);
	m_wndReBar.GetReBarCtrl().GetBandInfo(nb, &rbbi);

	if( rbbi.fStyle & RBBS_HIDDEN  || !m_FavBar.IsWindowVisible())
	{
		m_FavBar.ShowWindow(SW_SHOW);
		m_wndReBar.GetReBarCtrl( ).ShowBand(nb, TRUE);
		GetMenu()->CheckMenuItem(ID_VIEW_FAVS, MF_CHECKED);

	}
	else
	{
		m_wndReBar.GetReBarCtrl( ).ShowBand(nb,FALSE);
		m_FavBar.ShowWindow(SW_HIDE);
		GetMenu()->CheckMenuItem(ID_VIEW_FAVS, MF_UNCHECKED);
	}		
}


void CMainFrame::OnViewExTool() 
{
	// TODO: Add your command handler code here
	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_ID|RBBIM_STYLE;
	int nb = m_wndReBar.GetReBarCtrl( ).IDToIndex(ID_VIEW_EX_TOOL);
	m_wndReBar.GetReBarCtrl().GetBandInfo(nb, &rbbi);

	if( rbbi.fStyle & RBBS_HIDDEN  || !m_ExternalTool.IsWindowVisible())
	{
		m_ExternalTool.ShowWindow(SW_SHOW);
		m_wndReBar.GetReBarCtrl( ).ShowBand(nb, TRUE);
		GetMenu()->CheckMenuItem(ID_VIEW_EX_TOOL, MF_CHECKED);

	}
	else
	{
		m_wndReBar.GetReBarCtrl( ).ShowBand(nb,FALSE);
		m_ExternalTool.ShowWindow(SW_HIDE);
		GetMenu()->CheckMenuItem(ID_VIEW_EX_TOOL, MF_UNCHECKED);
	}
}

void CMainFrame::OnOptionsBrowsemodesDefault() 
{
	// TODO: Add your command handler code here
	bLinkInNewWindow = m_bmUserDefault.bLinkInNewWin;
	m_bActiveNewWindow = m_bmUserDefault.m_bActiveNewWindow;
	bAddressInNewWindow = m_bmUserDefault.m_bAlwaysNewWindow;
	m_nSeq = m_bmUserDefault.m_nSeq;
}

void CMainFrame::OnOptionsBrowsemodesNews() 
{
	// TODO: Add your command handler code here
	bLinkInNewWindow = m_bmNews.bLinkInNewWin;
	m_bActiveNewWindow = m_bmNews.m_bActiveNewWindow;
	bAddressInNewWindow = m_bmNews.m_bAlwaysNewWindow;
	m_nSeq = m_bmNews.m_nSeq;
	
}

void CMainFrame::OnOptionsBrowsemodesStandard() 
{
	// TODO: Add your command handler code here
	bLinkInNewWindow = m_bmStandard.bLinkInNewWin;
	m_bActiveNewWindow = m_bmStandard.m_bActiveNewWindow;
	bAddressInNewWindow = m_bmStandard.m_bAlwaysNewWindow;
	m_nSeq = m_bmStandard.m_nSeq;
	
}

void CMainFrame::OnAddtoGroup(UINT nID)
{
	CChildFrame * tcf = NULL;
	if(m_bRTab)
	{
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;

		m_wndTab.GetItem(nRTabID, &TabCtrlItem);
		tcf = (CChildFrame*)TabCtrlItem.lParam;
	}
	else
		tcf = (CChildFrame*)MDIGetActive();
	
	if(tcf!=NULL && tcf->m_pView!=NULL)
	{
		CString strName, strURL;
		tcf->GetWindowText(strName);
		tcf->m_pView->GetLocationURL(strURL);
		AddtoGroup(strName, strURL, nID); 
	}
}

void CMainFrame::AddtoGroup(LPCSTR pszName, LPCSTR pszURL, UINT nID)
{
	CGroupUrlDlg dlg;
	dlg.m_strName = pszName;
	dlg.m_strUrl = pszURL;
	dlg.m_bNotEmpty = TRUE;
	if(dlg.DoModal()==IDOK)
	{
		CMenu* pmenu = GetMenu()->GetSubMenu(4)->GetSubMenu(0)->GetSubMenu(2);
		if(pmenu == NULL)
			return;
		CString filename;	
		pmenu->GetMenuString(nID, filename, MF_BYCOMMAND);

		filename = filename.Mid(filename.Find(' ')+1);
		if(((CMyIEApp*)AfxGetApp())->m_strGroupPath.Right(1) != "\\")
			((CMyIEApp*)AfxGetApp())->m_strGroupPath += "\\";
		filename = ((CMyIEApp*)AfxGetApp())->m_strGroupPath + filename+".cgp";

		dlg.m_strName.Replace('=', '_');
		dlg.m_strName.TrimLeft();
		dlg.m_strName.TrimRight();
		dlg.m_strUrl.TrimLeft();
		WritePrivateProfileString("CaptorGroup", dlg.m_strName, dlg.m_strUrl, filename);
	}
}
/*
void CMainFrame::OnOptionsMyiestartupGroup() 
{
	// TODO: Add your command handler code here
	bGoHome = 5;
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIEGOHOME,MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_OPENBLANKPAGE,MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_RESUMELASTVISITEDPAGES,MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_OPENFAVORITEPAGES,MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_OPENMOSTFAVORITEPAGES,MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_OPENSTARTUPGROUP,MF_CHECKED);
}
*/
void CMainFrame::OnToolsSelbg() 
{
	// TODO: Add your command handler code here
	try{

	CColorDialog colordlg;
	colordlg.m_cc.Flags |= CC_RGBINIT;
	colordlg.m_cc.rgbResult = m_colBGC;
	if(colordlg.DoModal() == IDOK)
	{
		m_colBGC = colordlg.GetColor();
		BYTE b;
		char tmp[4];
		m_strBGColor = "#";
		b = GetRValue(m_colBGC);
		itoa(b, tmp, 16);
		m_strBGColor += tmp;
		b = GetGValue(m_colBGC);
		itoa(b, tmp, 16);
		m_strBGColor += tmp;
		b = GetBValue(m_colBGC);
		itoa(b, tmp, 16);
		m_strBGColor += tmp;

		OnToolsSetbg();
	}

	}catch(...)
	{
	}
}

void CMainFrame::OnToolsSetbg() 
{
	// TODO: Add your command handler code here
	CMyIEView * pvw = NULL;
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	if (tcf!=NULL)
		pvw = (CMyIEView*)tcf->m_pView;
	if(pvw == NULL)
		return;

	VARIANT color;
	color.vt = VT_BSTR;
	color.bstrVal = m_strBGColor.AllocSysString();

	SetBackground(pvw, color);

	SysFreeString(color.bstrVal);

}


//#pragma optimize( "s", off)

void CMainFrame::OnToolsIp() 
{
	// TODO: Add your command handler code here
	m_bDisplayIP = !m_bDisplayIP;
	//2.4
	if(m_bDisplayIP)
		m_wndStatusBar.SetPaneInfo(4, ID_INDICATOR_IP, SBPS_NORMAL, m_SBIPWidth);
	else
		m_wndStatusBar.SetPaneInfo(4, ID_INDICATOR_IP, SBPS_NORMAL, 0);

	m_wndStatusBar.AdjustToolbarPosition();
	m_wndStatusBar.AdjustProgressBarPosition();
}
void CMainFrame::OnOptionsExportproxylist() 
{
	// TODO: Add your command handler code here
	try{
	//select category first
	CSelProxyCatDlg dlg;
	dlg.m_bClear = FALSE;
	dlg.m_bDisableClear = TRUE;
	int i=0;
	if(dlg.DoModal()==IDOK)
	{
		CProxyList * pl=NULL;
		for(int l = 0;l <= ((CMainFrame*)pMainFrame)->m_aProxyCategories.m_ProxyCategory.GetUpperBound();l++)
		{
			pl = ((CMainFrame*)pMainFrame)->m_aProxyCategories.m_ProxyCategory.GetAt(l);
			if(!pl->m_bIsWebProxy)
			{
				if(i == dlg.m_SelCates)
					break;
				i++;
			}
		}
		if(pl!=NULL)
		{
			static char BASED_CODE pszFilter[] = "Result Files (*.txt)|*.txt|All Files (*.*)|*.*||";
 			CFileDialog dlg(FALSE, ".txt", pl->m_strCategoryName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, pszFilter);
			if(dlg.DoModal() != IDOK)
				return;

			CString filename = dlg.GetPathName();
			CStdioFile file;
			if(file.Open(filename, CFile::modeCreate|CFile::modeWrite))
			{
				CString line, type;
			//	POSITION pos = m_astrProxy.GetHeadPosition();
			//	POSITION pos1 = m_astrProxyName.GetHeadPosition();
				for( l=0;l <= pl->m_Proxies.GetUpperBound();l++)
				{
					line = pl->m_Proxies.ElementAt(l)->m_strProxy;
					//m_astrProxy.GetNext(pos);
					if(line.Left(2) != "--")
					{
						i = line.Find('=');
						if(i>0)
						{
							type = line.Left(i);
							type.MakeUpper();
							line = line.Mid(i+1);
							line = line + '@';
							line = line + type;
						}
						else
						{
							line = line + '@';
							line = line + "HTTP";
						}
						//type = m_astrProxyName.GetNext(pos1);
						type=pl->m_Proxies.ElementAt(l)->m_strProxyName;
						line += '#';
						line += type;
						line += "\n";
						file.WriteString(line);
					}
				//	else
				//		m_astrProxyName.GetNext(pos1);

				}

				file.Close();
			}
			
		}
	}
	

	}catch(...)
	{
	}
}

void CMainFrame::OnOptionsImportproxylist() 
{
	// TODO: Add your command handler code here
	try{

	static char BASED_CODE szFilters[] = "Result Files (*.txt)|*.txt|All Files (*.*)|*.*||";
 	CFileDialog fdlg(TRUE, ".txt", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilters);
	if(fdlg.DoModal() != IDOK)
		return;


	CSelProxyCatDlg dlg;
	dlg.m_bClear = FALSE;
	dlg.m_bDisableClear = FALSE;
	int i=0, p;
	if(dlg.DoModal()==IDOK)
	{
		//save orignal settings first
		((CMyIEApp*)AfxGetApp())->SaveConfg();

		CProxyItem* pi;
		CProxyList * pl=NULL;
		for(int l = 0;l <= ((CMainFrame*)pMainFrame)->m_aProxyCategories.m_ProxyCategory.GetUpperBound();l++)
		{
			pl = ((CMainFrame*)pMainFrame)->m_aProxyCategories.m_ProxyCategory.GetAt(l);
			if(!pl->m_bIsWebProxy)
			{
				if(i == dlg.m_SelCates)
					break;
				i++;
			}
		}
		if(pl!=NULL)
		{
		//	int re = AfxMessageBox(IDS_IMPORT_PROXY, MB_YESNO|MB_ICONQUESTION);
			if(dlg.m_bClear)//re==IDYES
			{
				CProxyItem* pi;
				for (int i=0;i <= pl->m_Proxies.GetUpperBound();i++)
				{
				   pi = pl->m_Proxies.GetAt(i);
				   delete pi;
				}
				pl->m_Proxies.RemoveAll();
			}

			CString filename = fdlg.GetPathName();
			CStdioFile file;
			int  state;
		//	POSITION pos, pos1;
			if(file.Open(filename, CFile::modeRead|CFile::shareDenyWrite))
			{
				CString line, addr, type, timing, status, annotation;
				int i;
				char c;
				while(file.ReadString(line))
				{
					i = line.FindOneOf("@$&#");
					if(i>0)
					{
						c = line.GetAt(i);
						addr = line.Left(i);

						while(i>0)
						{
							line = line.Mid(i+1);
							i = line.FindOneOf("@$&#");
							switch(c)
							{
							case '@': //type
									if(i>0)
										type = line.Left(i);
									else
										type = line;
									break;
							case '$': //status
									if(i>0)
										status = line.Left(i);
									else
										status = line;
									break;
							case '&': //timing
									if(i>0)
										timing = line.Left(i);
									else
										timing = line;
									break;
							case '#': //annotation
									if(i>0)
										annotation = line.Left(i);
									else
										annotation = line;
									break;
							}
							if(i>=0)
								c = line.GetAt(i);
						}
					}
					else
					{
						addr = line;
					}

					line = "";
					addr.TrimLeft();
					if(!addr.IsEmpty())
					{
						pi = new CProxyItem;

						if(!type.IsEmpty() && type!="HTTP")
						{
							type.MakeLower();
							line = type + '=';
						}
						line += addr;

						//pos = m_astrProxy.Find(line);
						p = pl->Find(line);
						if(p == -1)
						{
							//m_astrProxy.AddTail(line);
							pi->m_strProxy = line;
						//	isNew = -1;
						}
						/*else
						{
							isNew = 0;
							pos1 = m_astrProxy.GetHeadPosition();
							while(pos1 != pos)
							{
								isNew ++;
								m_astrProxy.GetNext(pos1);
							}
						}*/

						if(annotation.IsEmpty())
							annotation = addr;
						if(p<0)
						{
							//m_astrProxyName.AddTail(annotation);
							pi->m_strProxyName = annotation;
						}
						else
						{
						//	pos1 = m_astrProxyName.FindIndex(isNew);
						//	m_astrProxyName.SetAt(pos1, annotation);
							pl->m_Proxies.ElementAt(p)->m_strProxyName = annotation;
						}

						if(status == "6" || status.IsEmpty())
							state = 3;
						else
							state = 0;
						if(p<0)
						{
							//m_aProxySpeed.AddTail(state);
							pi->m_nProxySpeed = state;
						}
						else
						{
						//	pos1 = m_aProxySpeed.FindIndex(isNew);
						//	m_aProxySpeed.SetAt(pos1, state);
							pl->m_Proxies.ElementAt(p)->m_nProxySpeed = state;
						}

						try{
						if(p<0)
							pl->m_Proxies.Add(pi);
						else
							delete pi;
						}catch(...)
						{
						}
					}

					line.Empty();
					type.Empty();
					status.Empty();
					annotation.Empty();
					addr.Empty();
				}

				file.Close();
			}
		}
	}

	m_bSaveConfig = TRUE;
	bProxyDirty = TRUE;
	//rebuild menu
	CMenu* pMenu = GetMenu()->GetSubMenu(5)->GetSubMenu(2);
	BuildWebProxyMenu(pMenu, 1); //translate
	pMenu = GetMenu()->GetSubMenu(5)->GetSubMenu(3);
	BuildWebProxyMenu(pMenu, 0); //web proxy

	}catch(...)
	{
	}
}



void CMainFrame::SetBackground(CMyIEView *pview, VARIANT& color)
{
	try{

	IHTMLDocument2 * pDoc = NULL;
	IHTMLElement *pElem = NULL;
	IHTMLBodyElement * pBody = NULL;
	BSTR bstrSrc = NULL;

	pDoc = (IHTMLDocument2*)(pview->GetHtmlDocument());
	if(pDoc!=NULL)
	{
		pElem = NULL;
		pDoc->get_body(&pElem);
		if(pElem!=NULL)
		{
			pElem->QueryInterface(&pBody);
			if(pBody!=NULL)
			{
				pBody->put_background(bstrSrc);
				pBody->put_bgColor(color);
			}
		}
	}

	try{
		if(pBody != NULL)
			pBody->Release();
		if(pDoc != NULL)
			pDoc->Release();
		if(pElem != NULL)
			pElem->Release();
	}
	catch(...)
	{
	}

	}catch(...)
	{
	}
}

//#pragma optimize( "s", on)

void CMainFrame::OnToolsQuicksearch() 
{
	// TODO: Add your command handler code here
	CQuickSearchDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnToolsSync() 
{
	// TODO: Add your command handler code here
	ShellExecute(NULL, "open", "mobsync.exe" , "", NULL, SW_SHOW);
}

BOOL CMainFrame::SetDownloadTool(CString &ProgID)
{
	CLSID clsID;
	HRESULT hr;

	try{

	int i = m_DList.GetIDFromName(m_strDefaultDLT);
	if(i>=0)
	{
		CDownloadTool*	pdl = m_DList.m_DownloadTools.GetAt(i);
		if(ProgID == pdl->ProgID)
			return TRUE;
	}

	if(pDownload!=NULL)
		pDownload->Release();
	pDownload = NULL;

	CoFreeUnusedLibraries();

	if(ProgID=="")
		return TRUE;

	//CLSIDFromString...
	BSTR s = ProgID.AllocSysString();
	hr= CLSIDFromProgID(s, &clsID);//
	SysFreeString(s);
	if(FAILED(hr))
	{
	   //AfxMessageBox("Retrieval of ProgID failed");
		return FALSE;
	}
	else
	{
		hr = CoCreateInstance(clsID, NULL,
						CLSCTX_INPROC_SERVER,
						IID_IDispatch,
						(void**)&pDownload);
		if(SUCCEEDED(hr))
		{
/*			//update all ini flag;
			int n = nWin; //m_wndTab.GetItemCount();
			TCITEM TabCtrlItem;

			try{

			for(int i=0; i<n; i++)
			{
				TabCtrlItem.mask = TCIF_PARAM;
				TabCtrlItem.lParam = 0;
				m_wndTab.GetItem(i, &TabCtrlItem);
				if(TabCtrlItem.lParam != NULL)
					//((CChildFrame *)TabCtrlItem.lParam)->GetWindowText(str);
					((CChildFrame *)TabCtrlItem.lParam)->m_pView->m_bIniDL= TRUE;
			}
			}
			catch(...)
			{
			}*/
			return TRUE;
		}
		else
			return FALSE;
	}

	}catch(...)
	{
	}
	return FALSE;
}

//#pragma optimize( "s", off)

void CMainFrame::OnToolsCollect() 
{
	// TODO: Add your command handler code here
	if(pCollectDlg==NULL)
	{
		pCollectDlg = new CCollectorDlg();
		if(pCollectDlg!=NULL)
		{
			pCollectDlg->Create(IDD_COLLECTOR);
			CWinApp* app = AfxGetApp();
			//window pos
			RECT rect;
			int top = rect.top = app->GetProfileInt("Settings", "collecttop", 400);
			int left = rect.left = app->GetProfileInt("Settings", "collectleft", 600);
			int width = app->GetProfileInt("Settings", "collectwidth", 180);
			int height = app->GetProfileInt("Settings", "collectheight", 140);
			rect.bottom = top+height;
			rect.right = left+width;
			if( width!=0 && top<1000 && top>0)
			{
				pCollectDlg->SendMessage(WM_SIZE, 0, ((height)<<16)+width);   
				pCollectDlg->MoveWindow(&rect);
			}

			//register drag drop
			if (!pCollectDlg->ms_dropTarget.Register(pCollectDlg)) {
				TRACE("Fail in registing drop target\n");
			}

			//load saved contents
			CFile f;
			if(f.Open(((CMyIEApp*)app)->m_strSaveTxtFile, CFile::modeRead|CFile::shareDenyNone))
			{
				DWORD len = f.GetLength();
				char* tmp = new char[len+1];
				f.Read(tmp, len);
				tmp[len]='\0';
				pCollectDlg->m_Editor.SetWindowText(tmp);
				delete[] tmp;
			}
			f.Close();
			pCollectDlg->ShowWindow(SW_SHOW);
		}
	}
	else if(!pCollectDlg->IsWindowVisible())
		pCollectDlg->ShowWindow(SW_SHOW);
	else
		pCollectDlg->SendMessage(WM_CLOSE);
}




//#pragma optimize( "", on )


void CMainFrame::OnAllPageLinks() 
{
	// TODO: Add your command handler code here
	 CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	 if(tcf == NULL || tcf->m_pView == NULL)
		return;

	if(pLinkListDlg==NULL)
	{
		pLinkListDlg = new CLinkListDlg();
		if(pLinkListDlg!=NULL)
		{
			pLinkListDlg->Create(IDD_LINKLIST);
			CWinApp* app = AfxGetApp();
			//window pos
			RECT rect;
			int top = rect.top = app->GetProfileInt("Settings", "LinkListtop", 100);
			int left = rect.left = app->GetProfileInt("Settings", "LinkListleft", 40);
			int width = app->GetProfileInt("Settings", "LinkListwidth", 300);
			int height = app->GetProfileInt("Settings", "LinkListheight", 430);
			rect.bottom = top+height;
			rect.right = left+width;
			if( width!=0 && top<1000 && top>0)
			{
				pLinkListDlg->SendMessage(WM_SIZE, 0, ((height)<<16)+width);   
				pLinkListDlg->MoveWindow(&rect);
			}

		//	pLinkListDlg->ShowWindow(SW_SHOW);
		}
	}
	
	if(pLinkListDlg!=NULL)
	{
		try{

		//get all links
		GetAllLinks(tcf->m_pView, strlinks);
		CFile f;
		CString HTML;
		if(f.Open(((CMyIEApp*)AfxGetApp())->m_strLinkListPath, CFile::modeRead))
		{
			DWORD len = f.GetLength();
			char* tmp = new char[len+1];
			f.Read(tmp, len);
			tmp[len]='\0';
			HTML = tmp;
			delete[] tmp;
			HTML.Replace("<!--LINK LIST HERE-->", strlinks);
			f.Close();

			if(f.Open(((CMyIEApp*)AfxGetApp())->m_strLinkListPath+"tmp", CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
			{
				f.Write(HTML, HTML.GetLength());
				f.Close();
			}
			pLinkListDlg->m_wndLinks.Navigate(((CMyIEApp*)AfxGetApp())->m_strLinkListPath+"tmp", 0, NULL, NULL, NULL);
			pLinkListDlg->GetWindowText(HTML);
			int i = HTML.Find(' ');
			if(i>0)
				HTML = HTML.Left(i);
			HTML += " - ";
			HTML += tcf->m_pView->m_lpszUrl;
			pLinkListDlg->SetWindowText(HTML);
		}

		}catch(...)
		{
		}

		if(pLinkListDlg->IsIconic())
			pLinkListDlg->ShowWindow(SW_RESTORE);
		pLinkListDlg->ShowWindow(SW_SHOW);	

	}
}

BOOL CMainFrame::GetAllLinks(CMyIEView *pView, CString & strlinks)
{
	IHTMLDocument2 * pDoc = NULL;
	IHTMLElementCollection  * pAllElem = NULL;
	IHTMLElement *pElem = NULL;
	IHTMLAnchorElement * pAnchor = NULL;
	IHTMLElement* ptext;
	long p;
	BSTR bstrSrc;
	VARIANT name;
	name.vt = VT_I4;
	CString mailto;
	BOOL mail1=TRUE, link1=TRUE;

	try{

	strlinks="var urllinks = new LinkArray(\r\n";
	mailto="\r\nvar mailtos = new MailtoArray(\r\n";

	pDoc = (IHTMLDocument2*)(pView->GetHtmlDocument());
	if(pDoc!=NULL)
	{
		pDoc->get_links(&pAllElem);
		if(pAllElem!=NULL)
		{
			pAllElem->get_length(&p);
			if(p>0)
			{
			//save all links
				for(int i=0; i<p; i++)
				{
					name.lVal = i;
					if(pAllElem->item(name, name, (LPDISPATCH*)&pElem)==S_OK)
					{
						if(pElem != NULL)
						{
							pElem->QueryInterface(&pAnchor);
							if(pAnchor!=NULL)
							{
								pAnchor->get_href(&bstrSrc);
								CString s1(bstrSrc);
								SysFreeString(bstrSrc);

								pAnchor->QueryInterface(&ptext);
								ptext->get_innerText(&bstrSrc);
								CString s(bstrSrc);
								SysFreeString(bstrSrc);
								ptext->Release();
								s.TrimLeft();s.TrimRight();
								s.Replace("\n", "\\n");
								s.Replace("\r", "\\r");
								s.Replace("\"", "\\\"");
								s.Replace("\'", "\\\'");
								
								if(s.IsEmpty())
									s = s1;
								if(s1.Left(7)=="mailto:")
									{
										if(!mail1)
										{
											mailto+=",\r\n";
										}
										else
											mail1=FALSE;

										mailto+="\""+s+"\",";
										mailto+="\""+s1+"\"";
									}
								else
									{
										if(!link1)
										{
											strlinks+=",\r\n";
										}
										else
											link1=FALSE;

										strlinks+="\""+s+"\",";
										strlinks+="\""+s1+"\"";
									}

								pAnchor->Release();
								pAnchor = NULL;
							}

							pElem->Release();
							pElem = NULL;
						}
					}
				}
			}
		}
	}
	}
	catch(...)
	{
	}

	try{
		if(pDoc != NULL)
			pDoc->Release();
		if(pAllElem != NULL)
			pAllElem->Release();
	}
	catch(...)
	{
	}

	strlinks += ");";
	mailto += ");";
	strlinks += mailto;

	return TRUE;
}

void CMainFrame::OnToolsAllBk() 
{
	// TODO: Add your command handler code here
	VARIANT color;
	color.vt = VT_BSTR;
	color.bstrVal = m_strBGColor.AllocSysString();

	CChildFrame* tcf;
	UINT n = m_wndTab.GetItemCount();
	nWin = n;
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;

	for(UINT i=0;i<n;i++)
	{
		
		tcf = NULL;
		TabCtrlItem.lParam = NULL;
		m_wndTab.GetItem(i, &TabCtrlItem);
		tcf = ((CChildFrame*)TabCtrlItem.lParam);
		if(tcf != NULL && tcf->m_pView!=NULL)
		{
			SetBackground(tcf->m_pView, color);
		}

	}

	SysFreeString(color.bstrVal);
	
}

/*void CMainFrame::OnFileSaveas() 
{
	// TODO: Add your command handler code here
	//COleVariant vaZoomFactor(3l);
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	if (tcf!=NULL && tcf->m_pView!=NULL)
	{
		tcf->m_pView->ExecWB(OLECMDID_SAVEAS, OLECMDEXECOPT_DODEFAULT,
		   NULL, NULL);
	}

}



void CMainFrame::OnFileSavehtml() 
{
	// TODO: Add your command handler code here
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	if (tcf!=NULL && tcf->m_pView!=NULL)
	{
		tcf->m_pView->ExecWB(OLECMDID_SAVEAS, OLECMDEXECOPT_DONTPROMPTUSER,
		   NULL, NULL);
	}

}*/

void CMainFrame::OnFileCloseall() 
{
	// TODO: Add your command handler code here
	if(bConfirmCloseAll)
		if(AfxMessageBox(IDS_CLOSE_ALL_CONF, MB_YESNO|MB_ICONQUESTION)==IDNO)
			return;

	CChildFrame* tcf;
	try
	{
	int n = m_wndTab.GetItemCount();
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;
	int l =n-1;

	for(int i=n-1;i>=0;i--)
	{
		m_wndTab.GetItem(l, &TabCtrlItem);
		tcf = ((CChildFrame*)TabCtrlItem.lParam);
		if(tcf != NULL)
		{
			if(!tcf->bLock && !tcf->m_pView->m_bToClose)
			{
				tcf->m_pView->m_bToClose = TRUE;
				//2.4 post
				tcf->PostMessage(WM_CLOSE);
			}
		}
		l--;
	}
	}
/*	catch(CMemoryException* e)
	{
//		AfxMessageBox("close all error");
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}
}

void CMainFrame::SetTabTitle(LPCSTR lpszTitle, int nItem)
{
	//set the new title of tab
	TCITEM TabCtrlItem;
	if(lpszTitle==NULL)
		return;

	try{

/*3.0		char *temp = NULL;
		try
		{
		temp = new char[m_nTitleLen+4];
		}
		catch(...)
		{
			return;
		}
		if(temp==NULL)
			return;
*/
		static char temp[256];

		try
		{
		TabCtrlItem.mask = TCIF_TEXT;
		if(strlen(lpszTitle)>m_nTitleLen)
		{
			strncpy(temp, lpszTitle, m_nTitleLen);
			temp[m_nTitleLen]='\0';
			TabCtrlItem.pszText = temp;
		}
		else
		{
			strcpy(temp, lpszTitle);
			TabCtrlItem.pszText = temp;
		}

		m_wndTab.SetItem( nItem, &TabCtrlItem );
		m_wndTab.GetToolTips()->UpdateTipText(lpszTitle, &m_wndTab, nItem);
		//PostMessage(WM_UPDATE_TAB_TIP);
		}
		catch(...)
		{
		}

/*3.0		try{
		delete[] temp;
		}
		catch(...)
		{
		}*/

	}catch(...)
	{
	}
}



int CMainFrame::FindTab(CChildFrame *lpFrame, BOOL bCleanBlank)
{
	int n;
	BOOL found;
	register int i=0;
	TCITEM TabCtrlItem;


//	11/7
/*	if(bCleanBlank)
	{
		while (i<n && !found)
		{
			TabCtrlItem.lParam = 0;
			m_wndTab.GetItem(i, &TabCtrlItem);
			if(TabCtrlItem.lParam != 0)
				if(((CChildFrame*)TabCtrlItem.lParam)->m_pBlankChild == lpFrame)
				{	
					((CChildFrame*)TabCtrlItem.lParam)->m_pBlankChild = NULL;
					found = true;				
				}
				else
					i++;
			else
				i++;
		}
	}
*/	
	try
	{
	i=0; 
	n = m_wndTab.GetItemCount();
	found = false;
	TabCtrlItem.mask = TCIF_PARAM;

	while (i<n && !found)
	{
		TabCtrlItem.lParam = 0;
		m_wndTab.GetItem(i, &TabCtrlItem);
		if(TabCtrlItem.lParam == (LPARAM)lpFrame)
		{	
			found = true;				
		}
		else
			i++;
	}
	}
/*	catch(CMemoryException* e)
	{
//		AfxMessageBox("find tab error");
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}

	if(!found)
		i=-2;

	return i;
}



void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	try{

	if(MDIGetActive()!=NULL  && bConfirmExit)
		if(AfxMessageBox(IDS_CLOSE_ALL_CONF, MB_YESNO|MB_ICONQUESTION)==IDNO)
			return;

	DestroyIcon(m_hSecuIcon);

	if(ms_TabDropTarget!=NULL)
		delete ms_TabDropTarget;

	//2.5 release BHO for download
	if(pDownload!=NULL)
		pDownload->Release();

	//2.5 txt collector
	if(pCollectDlg!=NULL)
	{
		pCollectDlg->DestroyWindow();
		delete pCollectDlg;
	}

	//2.5 link list
	if(pLinkListDlg!=NULL)
	{
		pLinkListDlg->DestroyWindow();
		delete pLinkListDlg;
	}


	//kill update timer
	if(m_nUpdateTimer != 0)
		KillTimer(m_nUpdateTimer);
	m_nUpdateTimer = 0;

	if(m_nAnimateIconTimer)
		KillTimer(m_nAnimateIconTimer);
	m_nAnimateIconTimer=0;

	if(hResInst != NULL)
		FreeLibrary(hResInst);
	hResInst = NULL;

	CWinApp* app = AfxGetApp();

	if(app != NULL)
	{

	//save full screen
	app->WriteProfileInt("Settings", "FullScreen", m_bFullScreenMode);
	
	if(m_bFullScreenMode)
		FullScreenModeOff();

	//window position
	CRect wrect;
	if(IsZoomed())
		app->WriteProfileInt("Settings", "Zoomed", TRUE);
	else if(!IsIconic())
	{
		app->WriteProfileInt("Settings", "Zoomed", FALSE);

		GetWindowRect(&wrect);
		app->WriteProfileInt("Settings", "top", wrect.top);
		app->WriteProfileInt("Settings", "left", wrect.left);
		app->WriteProfileInt("Settings", "width", wrect.Width());
		app->WriteProfileInt("Settings", "Height", wrect.Height());
	}

	//erase running flag
	app->WriteProfileInt("Settings", "Running", 0);

	//is proxy enabled?
	INTERNET_PROXY_INFO ipi;
	DWORD dwSize=sizeof(ipi);
	InternetQueryOption(NULL,INTERNET_OPTION_PROXY,&ipi,&dwSize);
	if(ipi.dwAccessType != INTERNET_OPEN_TYPE_DIRECT)
		app->WriteProfileInt("Settings", "EnableProxy", 1);
	else
		app->WriteProfileInt("Settings", "EnableProxy", 0);

	//band position
	char temp[7]="band", rect[10]="bandrc";
	UINT i;
	RECT bandrect;
	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_ID|RBBIM_STYLE;
	for(i=0; i< m_wndReBar.GetReBarCtrl().GetBandCount(); i++)
	{
		itoa(i, temp+4, 10);
		m_wndReBar.GetReBarCtrl().GetBandInfo(i, &rbbi);
		app->WriteProfileInt("Settings", temp, rbbi.wID);
		itoa(i, rect+6, 10);
		m_wndReBar.GetReBarCtrl().GetRect(i, &bandrect);
		app->WriteProfileBinary("Settings", rect, (LPBYTE)&bandrect, sizeof(LONG)*4);
		//show hide bands
		if(rbbi.fStyle & RBBS_HIDDEN )
			app->WriteProfileInt("Bands", temp, 0);
		else
			app->WriteProfileInt("Bands", temp,1);
	}
	itoa(i, temp+4, 10);
	app->WriteProfileInt("Settings", temp, 0);

	//bottom bar
	char temp2[8]="bandb";
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_ID|RBBIM_STYLE;
	for(i=0; i< m_wndReBar2.GetReBarCtrl().GetBandCount(); i++)
	{
		itoa(i, temp2+5, 10);
		m_wndReBar2.GetReBarCtrl().GetBandInfo(i, &rbbi);
		//show hide bands
		if(rbbi.fStyle & RBBS_HIDDEN )
			app->WriteProfileInt("Bands", temp2, 0);
		else
			app->WriteProfileInt("Bands", temp2,1);
	}

	//save download tool
	app->WriteProfileString("DownloadTool", "DefaultTool", m_strDefaultDLT);
	app->WriteProfileString("DownloadTool", "FileExt", m_strDownloadFileExt);

	//status bar
	if(m_wndStatusBar.IsWindowVisible())
		app->WriteProfileInt("Bands", "Status", 1);
	else
		app->WriteProfileInt("Bands", "Status" ,0);

	//2.2 save last visited urls
	CString allpopups;
	CChildFrame* tcf;
	UINT n = m_wndTab.GetItemCount();
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;

	CString lasturl;
	for(i=0;i<n;i++)
	{
		m_wndTab.GetItem(i, &TabCtrlItem);
		tcf = ((CChildFrame*)TabCtrlItem.lParam);
		if(tcf != NULL && tcf->m_pView!=NULL)
		{
//			tcf->m_pView->GetLocationURL(lasturl);
			allpopups += tcf->m_pView->m_lpszUrl;
			allpopups += "\n";			
		}
	}
	app->WriteProfileBinary("Settings","lastvisit", (LPBYTE)((LPCSTR)allpopups), allpopups.GetLength()+1);
//2.2 close all window
/*2.3
	try
	{
	ShowWindow(SW_HIDE);
	TabCtrlItem.mask = TCIF_PARAM;
	int l =n-1;

	for(int i=n-1;i>=0;i--)
	{
		m_wndTab.GetItem(l, &TabCtrlItem);
		tcf = ((CChildFrame*)TabCtrlItem.lParam);
		if(tcf != NULL)
		{
			tcf->SendMessage(WM_CLOSE);
		}
		l--;
	}
	}
	catch(...)
	{
	}
*/	

	//Close external util started with myie
	CloseExternalUtil();


	//save other config
	((CMyIEApp*)AfxGetApp())->SaveConfg();
	}

	SaveTypedURL();

//	::RemoveProp(m_hWnd, AfxGetApp()->m_pszAppName);
	//namespace
	UninitialNameSpace();



	}
	catch(...)
	{
	}

	
	try{
	CMDIFrameWnd::OnClose();
	pMainFrame = NULL;
	}catch(...)
	{
	}

	//3.01
	ClearSpeedMonitor();
}

void CMainFrame::OnViewSource() 
{
	// TODO: Add your command handler code here
	try{
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	
	if (tcf!=NULL && tcf->m_pView!=NULL)
	{
		CMyIEView* pvw = (CMyIEView*)tcf->m_pView;
		ExecCmdTarget(HTMLID_VIEWSOURCE, pvw);
	}
	}catch(...)
	{
	}
}


void CMainFrame::OnFileClose() 
{
	// TODO: Add your command handler code here
	try
	{
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	
	if (tcf!=NULL)
	{
//2.2 313
		tcf->m_pView->m_bToClose = TRUE;
		tcf->PostMessage(WM_CLOSE);
		//tcf->MDIDestroy();
	}
	}
/*	catch(CMemoryException* e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}
}

void CMainFrame::OnFileClose2() 
{
	// TODO: Add your command handler code here
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;

	try
	{
	m_wndTab.GetItem(nRTabID, &TabCtrlItem);
	if(TabCtrlItem.lParam != NULL)
	{
		CChildFrame* tcf = ((CChildFrame*)TabCtrlItem.lParam);
//2.2 313
		tcf->m_pView->m_bToClose = TRUE;
		tcf->PostMessage(WM_CLOSE);
		//tcf->MDIDestroy();
	}
	}
/*	catch(CMemoryException* e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}
}


//Used to Execute Find, View Source and Internet Option commands.
HRESULT CMainFrame::ExecCmdTarget(DWORD nCmdID, CFixedHtmlView* pHVW)
{
	LPDISPATCH lpDispatch = NULL;
    LPOLECOMMANDTARGET lpOleCommandTarget = NULL;

	if(pHVW != NULL)
	{
		try
		{
         lpDispatch = pHVW->GetHtmlDocument();
		 if(lpDispatch == NULL)
			 return S_OK;
         ASSERT(lpDispatch);

         // Get an IDispatch pointer for the IOleCommandTarget interface.
         lpDispatch->QueryInterface(IID_IOleCommandTarget, (void**)&lpOleCommandTarget);
         ASSERT(lpOleCommandTarget);

         lpDispatch->Release();
		 lpDispatch = NULL;

         // Invoke the given command id for the WebBrowser control
		 if(lpOleCommandTarget != NULL)
		 {
			 HRESULT hr = lpOleCommandTarget->Exec(&CGID_IWebBrowser, nCmdID, 0,
                                         NULL, NULL);
			 lpOleCommandTarget->Release();
			 lpOleCommandTarget = NULL;
			 return hr;
		 }
		 else
			 return S_OK;
		}
		/*catch(CMemoryException* e)
		{
			if(e!=NULL)e->Delete();
		}*/
		catch(...)
		{//2.1
		//	if(lpDispatch != NULL)
		//		lpDispatch->Release();
		//	if(lpOleCommandTarget != NULL)
		//		lpOleCommandTarget->Release();
		}
	}
	return S_OK;
}

void CMainFrame::OnToolsInternetoptions() 
{
	// TODO: Add your command handler code here
	try
	{
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	
	if (tcf!=NULL)
	{
		CMyIEView* pvw = (CMyIEView*)tcf->m_pView;
		if(pvw!=NULL)
			ExecCmdTarget(HTMLID_OPTIONS, pvw);

		GetHomePage(m_strHome);
	}
	}
/*	catch(CMemoryException* e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}
}


//typedef BOOL (CALLBACK* LPFNADDFAV)(HWND, TCHAR*, UINT, TCHAR*, UINT, 
//                                    LPITEMIDLIST);

void CMainFrame::OnFavoritesAddtofavorites() 
{
	// TODO: Add your command handler code here
	
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	AddFavorite(tcf);
	
}

void CMainFrame::OnFavoritesExportfavorites() 
{
	// TODO: Add your command handler code here
   IShellUIHelper* pShellUIHelper=NULL;
   HRESULT hr = CoCreateInstance(CLSID_ShellUIHelper, NULL, 
                                 CLSCTX_INPROC_SERVER, IID_IShellUIHelper, 
                                 (LPVOID*)&pShellUIHelper);

   if (SUCCEEDED(hr) && pShellUIHelper!=NULL)
   {
	
//		if (tcf!=NULL)
	   try
		{

			VARIANT_BOOL fImport=VARIANT_FALSE;
			CString strImpExpPath = "";
			pShellUIHelper->ImportExportFavorites( fImport, strImpExpPath.AllocSysString());

		}
	   catch(...)
	   {
	   }

		pShellUIHelper->Release();
   }	
}

void CMainFrame::OnFavoritesImportfavorites() 
{
	// TODO: Add your command handler code here
   IShellUIHelper* pShellUIHelper = NULL;
   HRESULT hr = CoCreateInstance(CLSID_ShellUIHelper, NULL, 
                                 CLSCTX_INPROC_SERVER, IID_IShellUIHelper, 
                                 (LPVOID*)&pShellUIHelper);

   if (SUCCEEDED(hr) && pShellUIHelper!=NULL)
   {

//		if (tcf!=NULL)
		try	
	   {
			VARIANT_BOOL fImport=VARIANT_TRUE;
			CString strImpExpPath = "";
			pShellUIHelper->ImportExportFavorites( fImport, strImpExpPath.AllocSysString());

		}
		catch(...)
		{
		}

		pShellUIHelper->Release();

		//rebuild favorite menu
		m_bFavDirty = TRUE;
   }
}


typedef UINT (CALLBACK* LPFNORGFAV)(HWND, LPTSTR);

void CMainFrame::OnFavoritesOrgnizefavorites() 
{
	// TODO: Add your command handler code here

   HMODULE hMod = (HMODULE)LoadLibrary("shdocvw.dll");
   if (hMod)
   {
      LPFNORGFAV lpfnDoOrganizeFavDlg = (LPFNORGFAV)GetProcAddress(hMod, 
                                                    "DoOrganizeFavDlg");
      if (lpfnDoOrganizeFavDlg)
      {
		CString           szPath;

		if(!GetFavoriteFolder(szPath))
			return ;
		lpfnDoOrganizeFavDlg(m_hWnd, szPath.GetBuffer(100));
      }

      FreeLibrary(hMod);
   }
   //rebuild favorites menu
   m_bFavDirty = TRUE;

   	//update favorites list
	if(HasExpBar(IDS_TITLE_FAVORITE))
		m_pwndExpObj->Update(1);

}

void CMainFrame::InitFavorites(BOOL bOnlyLink)
{
	// set up Favorites menu
	static TCHAR           sz[MAX_PATH];
	CString           szPath, link;
	CMenu*          pMenu = NULL;
//	BOOL			bMax;

	// first get rid of bogus submenu items.
	try
	{
	pMenu = GetMenu()->GetSubMenu(3);

	if(pMenu == NULL)
		return;

	DestroyMenuData(pMenu->m_hMenu);
	while(pMenu->DeleteMenu(nStart, MF_BYPOSITION));



	// find out from the registry where the favorites are located.
	if(!GetFavoriteFolder(szPath))
		return ;

	HKEY            hKey;
	DWORD           dwSize = MAX_PATH;

	if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\Toolbar"), &hKey) == ERROR_SUCCESS)
	{

		if(RegQueryValueEx(hKey, _T("LinksFolderName"), NULL, NULL, (LPBYTE)&sz, &dwSize) == ERROR_SUCCESS)
		{
			link = sz;
		}
		RegCloseKey(hKey);	
	}

	//does display hidden files
	if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced"), &hKey) == ERROR_SUCCESS)
	{

		RegQueryValueEx(hKey, _T("Hidden"), NULL, NULL, (LPBYTE)&m_dwHidden, &dwSize);
		RegCloseKey(hKey);	
	}

	BOOL foundlink = FALSE;
	if(!link.IsEmpty())
	{
		strcpy(sz, (LPCSTR)szPath);
		strcat(sz, (LPCSTR)link);
		if(SetCurrentDirectory(sz))
			foundlink = TRUE;
	}
	
	if(!foundlink)
	{
		strcpy(sz, (LPCSTR)szPath);
		strcat(sz, "\\Á´½Ó\\");

		if(!SetCurrentDirectory(sz))
		{
			strcpy(sz, (LPCSTR)szPath);
			strcat(sz, "\\Links\\");
			if(!SetCurrentDirectory(sz))
			{
				strcpy(sz, (LPCSTR)szPath);
				strcat(sz, "\\³sµ²\\");
				if(!SetCurrentDirectory(sz))
				{
					strcpy(sz, (LPCSTR)szPath);
					strcat(sz, "\\links\\");
				}
			}
		}
	}

	m_strLinkFolder = sz;

	m_astrFavoriteFolders.RemoveAll(); 
//	m_astrFavoriteURLs.RemoveAll();
	m_nFavSize=0;
	m_astrLinks.RemoveAll();
	m_astrFavs.RemoveAll(); 

	if(bOnlyLink)
	{
//		BuildFavoritesMenu(sz,0 , pMenu, nStart);
	}
	else
	{
		int nFavs, nMostFavs, fs;
		m_nFavSize = BuildFavoritesMenu(szPath,0 , pMenu, nStart, TRUE, nFavs, nMostFavs);
		if(m_bDisplayOpenAll || m_bDisplayAddFav)
		{
			if(nFavs || nMostFavs || m_bDisplayAddFav)
				pMenu->InsertMenu(4,MF_BYPOSITION | MF_SEPARATOR);
			fs=m_astrFavoriteFolders.GetCount();
			m_astrFavoriteFolders.AddTail(szPath+"\\");
		}
		if(m_bDisplayOpenAll)
		{
			//add open all
			CString strOpenAll, strOpenMost;
			strOpenAll.LoadString(IDS_OPEN_ALL_FAV);
			strOpenMost.LoadString(IDS_OPEN_ALL_MOSTFAV);

			if(nMostFavs)
				pMenu->InsertMenu(4,MF_BYPOSITION| MF_STRING ,0x1280+fs, strOpenMost);
			if(nFavs)
				pMenu->InsertMenu(4,MF_BYPOSITION| MF_STRING ,0x1000+fs, strOpenAll);
		}
		if(m_bDisplayAddFav)
		{
			//add fav
			CString strAddFav;
			strAddFav.LoadString(IDS_ADDFAV);

			pMenu->InsertMenu(4,MF_BYPOSITION| MF_STRING ,0x6220+fs, strAddFav);
		}
	}
	}
/*	catch(CMemoryException* e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}

}

//Get pointer to text SK
const char *lpstrText = "Texthgp";//SK: we use const to prevent misuse
void CMainFrame::AddMenuBreak(CMenu *pMenu, BOOL bFavMenu)
{
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
    CDC *pDC = GetDC();              // Get device context
    CFont* pFont=NULL;    // Select menu font in...
    
 //   if (IsNewShell())
    pFont = pDC->SelectObject (&m_fontMenu);// Select menu font in...
        
    
        
    SIZE ssize;
	::GetTextExtentPoint32(pDC->m_hDC,lpstrText,
             strlen(lpstrText),&ssize);

    CSize t = CSize(ssize);
//    if(IsNewShell())
    pDC->SelectObject (pFont);  // Select old font in
    ReleaseDC(pDC);  // Release the DC

    // Set width and height:
	int temp = t.cy+4;
    temp = temp>16+4 ? temp : 16+4;
    m_fontMenu.DeleteObject();
	

	int ms = t.cy+6; //GetSystemMetrics(SM_CYMENU);

	int mc = pMenu->GetMenuItemCount(); 
	if(bFavMenu)
	{
		ms = temp;
		//ms++;
		mc+=3;//add the open all menu
	}
	int size = ms*mc;
	int scr = GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYMENU) - GetSystemMetrics(SM_CYCAPTION);
	int start = 0;
	CString strMenu;
	while(size > scr)
	{
		//need a break bar
		int i = scr/ms + start;
		if(bFavMenu)
			i-=3; //conside the open all menu
		MENUITEMINFO info;
		info.cch =0;
		info.dwTypeData = 0;
	    info.cbSize = sizeof (MENUITEMINFO); // must fill up this field
	    info.fMask = MIIM_DATA|MIIM_TYPE|MIIM_ID|MIIM_SUBMENU;             // get the state of the menu item
	    pMenu->GetMenuItemInfo(i, &info, TRUE);
		pMenu->GetMenuString(i, strMenu, MF_BYPOSITION);
		if(info.fType & MFT_OWNERDRAW)
			if(IsMenu(info.hSubMenu))
				pMenu->ModifyMenu(i, MF_BYPOSITION | MF_MENUBARBREAK| MF_POPUP|MF_OWNERDRAW, (UINT)info.hSubMenu, (char*)info.dwItemData);
			else
				pMenu->ModifyMenu(i, MF_BYPOSITION | MF_MENUBARBREAK|MF_OWNERDRAW , info.wID, (char*)info.dwItemData);
		else
			if(IsMenu(info.hSubMenu))
				pMenu->ModifyMenu(i, MF_BYPOSITION | MF_MENUBARBREAK| MF_POPUP|MF_STRING, (UINT)info.hSubMenu, strMenu);
			else
				pMenu->ModifyMenu(i, MF_BYPOSITION | MF_MENUBARBREAK|MF_STRING , info.wID, strMenu);

		size = (mc-i)*ms;
		start = i+1;
	}
 
}



void CMainFrame::OnUpdateFileWorkoffline(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
		//Get global offline setting
	DWORD           sz;
	//TCHAR           szPath[MAX_PATH];
	HKEY            hKey;
	DWORD           dwSize = sizeof(DWORD);

	try
	{
	if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"), &hKey) != ERROR_SUCCESS)
	{
		return ;
	}

	if(RegQueryValueEx(hKey, _T("GlobalUserOffline"), NULL, NULL, (LPBYTE)&sz, &dwSize) == ERROR_SUCCESS)
	{
		//set new offline value
		if(pCmdUI->m_pOther && pCmdUI->m_pOther==&m_wndToolBar)
			ToolBarSetCheck(pCmdUI, sz!=0);
		else
			pCmdUI->SetCheck(sz!=0);

	}
	}
/*	catch(CMemoryException* e)
	{
//		AfxMessageBox("update offline error");
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}

	RegCloseKey(hKey);
}



void CMainFrame::OnUpdateMenu(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//2.2	try{
	if(nWin == 0)
		pCmdUI->Enable(FALSE);	
	else
		pCmdUI->Enable();	
//	}catch(...)
//	{
//	}
}


void CMainFrame::OnUpdateOptionsUseProxy(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here

	try{

	INTERNET_PROXY_INFO ipi;
	DWORD dwSize=sizeof(ipi);

	CTime t;
	CTimeSpan ts;
//	static CString str;
	
	t = CTime::GetCurrentTime();
	ts = t - m_tLastProxyUpdate;  
	if((m_bNeedUpdProxy && ts.GetTotalSeconds()>= 1) || ts.GetTotalSeconds()>= 10 || pCmdUI->m_pMenu!=NULL)
	{
		m_bNeedUpdProxy = FALSE;

		//This call determines the buffer size needed
		InternetQueryOption(NULL,INTERNET_OPTION_PROXY,&ipi,&dwSize);

		ToolBarSetCheck(pCmdUI, ipi.dwAccessType != INTERNET_OPEN_TYPE_DIRECT);
		/*
		if (ipi.dwAccessType != INTERNET_OPEN_TYPE_DIRECT )
		{
			if(m_strBGPic == "" || pCmdUI->m_pMenu !=NULL)
				pCmdUI->SetCheck();
			else
			{
				pCmdUI->SetCheck(0);
				m_wndToolBar.GetToolBarCtrl().PressButton(pCmdUI->m_nID, TRUE);
			}
		}
		else
		{
			if(m_strBGPic == "" || pCmdUI->m_pMenu !=NULL)
				pCmdUI->SetCheck(0);
			else
			{
				pCmdUI->SetCheck(0);
				m_wndToolBar.GetToolBarCtrl().PressButton(pCmdUI->m_nID, FALSE);
			}
		}*/

		//Free the allocated memory

		//Rename the menu
		useproxy.LoadString(IDS_USE_PROXY);
		if(!m_strCurProxyName.IsEmpty() && m_strCurProxyName!="")
		{
			useproxy += "(";
			useproxy += m_strCurProxyName;
			useproxy = useproxy.Left(35);
			useproxy += ")";
		}
		pCmdUI->SetText(useproxy);
		m_tLastProxyUpdate = t;
	}

	}/*catch(CMemoryException * e)
	{
		if(e!=NULL)e->Delete();
//		AfxMessageBox("update proxy error");
	}*/
	catch(...)
	{
	}
}

void CMainFrame::OnOptionsUseProxy() 
{
	// TODO: Add your command handler code here

	//get current setting
	INTERNET_PROXY_INFO ipi;
	DWORD dwSize2 = sizeof(ipi);
	CString proxy, bypass, usero, pwdo;

	m_bNeedUpdProxy = TRUE;

	//Insert code to use the proxy information
	if(!GetProxySetting(proxy,bypass, usero, pwdo))
	{
		//set a proxy now
		if(m_strCurProxy.IsEmpty())
			AfxMessageBox(IDS_NO_PROXY);
		else
		{
			//get proxy, username, password
/*			CString user="", pwd="";
			POSITION pp, pos = m_astrProxy.Find(m_strCurProxy);
			int index = -1;
			if(pos !=NULL)
			{
				//find index
				for( pp = pos; pp != NULL; )
				{
					m_astrProxy.GetPrev(pp);
					index++;
				}
				//get user name
			}*/

			ipi.dwAccessType = INTERNET_OPEN_TYPE_PROXY ;
			ipi.lpszProxy = m_strCurProxy;
			ipi.lpszProxyBypass = m_strProxyByPass;
			if(!InternetSetOption(NULL, INTERNET_OPTION_PROXY, &ipi, dwSize2))
				AfxMessageBox(IDS_PROXY_FAIL);
		}
	}
	else
	{
		//save the proxy setting and clear it.
		m_strCurProxy = proxy;
	//	m_strCurProxyName = proxy;
	//	m_strProxyByPass = bypass;


		//save the curproxy
/*2.3		if(!m_astrProxy.Find(m_strCurProxy))
		{
			m_astrProxy.AddTail(m_strCurProxy);
			m_astrProxyName.AddTail(m_strCurProxy);
			m_aProxySpeed.AddTail(3);
		}
		else*/
		{
			m_strCurProxyName = GetProxyName(m_strCurProxy);
		}

		ipi.dwAccessType = INTERNET_OPEN_TYPE_DIRECT ;
		if(!InternetSetOption(NULL, INTERNET_OPTION_PROXY, &ipi, dwSize2))
			AfxMessageBox(IDS_PROXY_FAIL);
	}



}



void CMainFrame::OnOptionsSetProxy() 
{
	// TODO: Add your command handler code here
	CSetProxy dlg;

	try{

	INTERNET_PROXY_INFO ipi;
	DWORD dwSize=sizeof(ipi);
	//This call determines the buffer size needed
	InternetQueryOption(NULL,INTERNET_OPTION_PROXY,&ipi,&dwSize);
	if (ipi.dwAccessType == INTERNET_OPEN_TYPE_DIRECT )
		dlg.m_bUseProxy = FALSE;
	else
		dlg.m_bUseProxy = TRUE;

	dlg.m_SelProxy = m_strCurProxy;
//	dlg.m_strByPass = m_strProxyByPass;
	dlg.m_nSelProxyList = m_nCurProxyList;
	
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	CMyIEView* pvw;
	if(tcf!=NULL)
	{
		pvw = (CMyIEView*)tcf->m_pView;
		dlg.m_bUseWebProxy = pvw->m_bEnWebProxy;
		dlg.m_SelWebProxy = pvw->m_strWebProxy;
	}
	if (dlg.m_SelWebProxy.IsEmpty())
		dlg.m_SelWebProxy = m_strDefaultWebProxy;
	
	if(dlg.DoModal()==IDOK)
	{
		m_bSaveConfig = TRUE;
		bProxyDirty = TRUE;
		m_bNeedUpdProxy = TRUE;

		//dlg.m_SelWebProxy.MakeLower();
		dlg.m_SelWebProxy.TrimLeft();
		dlg.m_SelWebProxy.TrimRight();
		if(tcf!=NULL)
		{
			pvw->m_bEnWebProxy = dlg.m_bUseWebProxy;
			pvw->m_strWebProxy = dlg.m_SelWebProxy;
		}
		if(!dlg.m_SelWebProxy.IsEmpty())
		{
			//save as default


			m_strDefaultWebProxy = dlg.m_SelWebProxy;

			//save to web proxy list if it is new.
		}
		else if(tcf!=NULL)
			pvw->m_bEnWebProxy = FALSE;

		//get the current setting
		INTERNET_PROXY_INFO ipi;
		DWORD  dwSize2 = sizeof(ipi);
		CString proxy, bypass, user, pwd;
		
		GetProxySetting(proxy,bypass,user, pwd);

		/*		if(GetProxySetting(proxy,bypass,user, pwd))
		{
			//save the curproxy
			if(!m_astrProxy.Find(proxy))
			{
				m_astrProxy.AddTail(proxy);
				m_astrProxyUser.AddTail(user);
				m_astrProxyPwd.AddTail(pwd);
			}
		}*/

		user=""; pwd="";

		dlg.m_SelProxy.MakeLower();
		dlg.m_SelProxy.TrimLeft();
		dlg.m_SelProxy.TrimRight();
	
//		m_strProxyByPass = dlg.m_strByPass;

		m_strCurProxy = dlg.m_SelProxy;
		m_strCurProxyName = GetProxyName(m_strCurProxy);
		if(!dlg.m_SelProxy.IsEmpty() && dlg.m_SelProxy!="" && dlg.m_bUseProxy && dlg.m_SelProxy.Left(1) != "-")
		{
			//apply the proxy setting
			m_nCurProxyList = dlg.m_nSelProxyList;
			m_strProxyByPass = m_aProxyCategories.m_ProxyCategory.GetAt(m_nCurProxyList)->m_strProxyByPass;

			if(m_strProxyByPass.Find("<local>")<0)
			{
				m_strProxyByPass += " <local>";
			}
			

			ipi.dwAccessType = INTERNET_OPEN_TYPE_PROXY ;
			ipi.lpszProxy = dlg.m_SelProxy;
		//	ipi.lpszProxyBypass = dlg.m_SelProxy+" <local>";
			ipi.lpszProxyBypass = m_strProxyByPass;
		//	m_strProxyByPass = ipi.lpszProxyBypass;
			if(!InternetSetOption(NULL, INTERNET_OPTION_PROXY, &ipi, dwSize2))
				AfxMessageBox(IDS_PROXY_FAIL);
			
/*2.3			POSITION pp, pos = m_astrProxy.Find(m_strCurProxy);
			int index = -1;
			if(pos !=NULL)
			{
				//find index
				for( pp = pos; pp != NULL; )
				{
					m_astrProxy.GetPrev(pp);
					index++;
				}
				//get user name
			}
			if(user!="" && !user.IsEmpty())
			{
				int l = user.GetLength()+1;
				InternetSetOption(NULL, INTERNET_OPTION_PROXY_USERNAME, 
					user.GetBuffer(20), l);
				user.ReleaseBuffer();

				//strPassword is the buffer that contains the proxy password
				l = pwd.GetLength()+1;
				InternetSetOption(NULL, INTERNET_OPTION_PROXY_PASSWORD, 
					pwd.GetBuffer(20), l);
				pwd.ReleaseBuffer();
			}*/
		}
		else
		{
			//clear the proxy setting
			if(proxy.IsEmpty() && dlg.m_SelProxy.Left(1)!="-")
			{
				m_strCurProxy = dlg.m_SelProxy;
				m_strCurProxyName = GetProxyName(m_strCurProxy);
				
				m_nCurProxyList = dlg.m_nSelProxyList;
				m_strProxyByPass = m_aProxyCategories.m_ProxyCategory.GetAt(m_nCurProxyList)->m_strProxyByPass;

				if(m_strProxyByPass.Find("<local>")<0)
				{
					m_strProxyByPass += " <local>";
				}
			}
			else
			{
				m_strCurProxy = proxy;
				m_strCurProxyName = GetProxyName(m_strCurProxy);
//				m_strProxyByPass = bypass;
			}

			ipi.dwAccessType = INTERNET_OPEN_TYPE_DIRECT ;
			if(!InternetSetOption(NULL, INTERNET_OPTION_PROXY, &ipi, dwSize2))
				AfxMessageBox(IDS_PROXY_FAIL);
		}

		//rebuild menu
		CMenu* pMenu = GetMenu()->GetSubMenu(5)->GetSubMenu(2);
		BuildWebProxyMenu(pMenu, 1); //translate
		pMenu = GetMenu()->GetSubMenu(5)->GetSubMenu(3);
		BuildWebProxyMenu(pMenu, 0); //web proxy

	}
	}/*catch(CMemoryException * e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}
}


void CMainFrame::OnEncoding(UINT nID)
{
	CMyIEView * pvw = NULL;
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	if (tcf!=NULL)
		pvw = (CMyIEView*)tcf->m_pView;
	if(pvw == NULL)
		return;

	LPDISPATCH pDisp = NULL;

	pDisp = pvw->GetHtmlDocument();
	IHTMLDocument2 *pHTMLDoc;
	if( pDisp )
	{
		if (SUCCEEDED(pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc)))
		{

			BSTR bstr;

		   switch (nID)
		   {
		   case ID_VIEW_ENCODING_WESTERNEUROPEAN:
			   //western
				bstr = SysAllocString(L"iso-8859-1");
				pHTMLDoc->put_charset( bstr );
				break;
		   case ID_VIEW_ENCODING_CHINESESIMPLIFIED:
			   //simpilified
				bstr = SysAllocString(L"iso-ir-58");
				pHTMLDoc->put_charset( bstr );
				break;
		   case ID_VIEW_ENCODING_CHINESETRADITIONAL:
			   //traditional 
				bstr = SysAllocString(L"big5");
				pHTMLDoc->put_charset( bstr );
				break;
		   case ID_VIEW_ENCODING_JANESE:
			   //japanese jis
				bstr = SysAllocString(L"iso-2022-jp");
				pHTMLDoc->put_charset( bstr );
				break;
		   case ID_VIEW_ENCODING_JANESEEUC:
			   //japanese euc 
				bstr = SysAllocString(L"euc-jp");
				pHTMLDoc->put_charset( bstr );
				break;
		   case ID_VIEW_ENCODING_JANESEAUTO:
			   //japanese auto
				bstr = SysAllocString(L"_autodetect");
				pHTMLDoc->put_charset( bstr );
				break;
		   }
			

		   SysFreeString(bstr);
		   pHTMLDoc->Release();

		   pvw->Refresh2(1);
		}
	   pDisp->Release();
	}

}





/*void CMainFrame::OnUpdateOptionTabPos(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//2.2	try{
	if(pCmdUI->m_nID == ID_OPTION_TOP_TAB)
	{
		if(m_bTopTab)
			pCmdUI->SetCheck();
		else
			pCmdUI->SetCheck(0);
	}
	else
	{
		if(m_bTopTab)
			pCmdUI->SetCheck(0);
		else
			pCmdUI->SetCheck(1);
	}
	/*catch(CMemoryException * e)
	{
		if(e!=NULL)e->Delete();
	}* /
//	}catch(...)
//	{
//	}

}*/



void CMainFrame::BuildUtilMenu()
{
	int nStart = 2;
	CMenu *		pMenu;

	//delete all menus
	BOOL			bMax;

	// first get rid of bogus submenu items.
	MDIGetActive(&bMax);
	
	// for the favorties popup, just steal the menu from the main window
	pMenu = GetMenu()->GetSubMenu(6)->GetSubMenu(13);
	while(pMenu->DeleteMenu(nStart, MF_BYPOSITION));
			
	//remove all link buttons
	int b = m_LinkBar.GetToolBarCtrl().GetButtonCount();
	REBARBANDINFO rbbi;

	int bandid = m_wndReBar.GetReBarCtrl().IDToIndex(ID_VIEW_EX_TOOL);
	rbbi.fMask = RBBIM_SIZE|RBBIM_STYLE;
	m_wndReBar.GetReBarCtrl().GetBandInfo(bandid, &rbbi);
	for(int i = 1; i<b;i++)
		m_ExternalTool.GetToolBarCtrl().DeleteButton(0);


	//add menus d00-df0
	int nPos = 0;
	SHFILEINFO shFinfo;
	int iIcon;
	CString  strTemp;
	CExternalUtilItem* eui;
	for (i=0;i <= m_ExternalUtilList.m_UtilList.GetUpperBound();i++)
	{
		eui = m_ExternalUtilList.m_UtilList.GetAt(i);
		if(eui!=NULL)
		{
			pMenu->AppendMenu(MF_STRING | MF_ENABLED , 0xd00 + nPos, eui->m_strUtilName);
			if(eui->m_bUtilStart)
				pMenu->CheckMenuItem(0xd00 + nPos, MF_CHECKED);
		}

		//get tool icon
		if(eui!=NULL)
			strTemp = eui->m_strUtilCmd;
		else
			strTemp ="";

		if(strTemp.Left(1) == '\"')
			strTemp = strTemp.Mid(1);
		if(strTemp.Right(1) == '\"')
			strTemp = strTemp.Left(strTemp.GetLength()-1);
		if ( strTemp.Right(1) != '\\' )
			strTemp += "\\";
		if ( SHGetFileInfo( strTemp,
							0,
							&shFinfo,
							sizeof( shFinfo ),
							SHGFI_ICON | 
							SHGFI_SMALLICON ) )
		{

			iIcon = shFinfo.iIcon;

			// we only need the index from the system image list

			DestroyIcon( shFinfo.hIcon );

			m_ExternalTool.SetButtonInfo(nPos,0xd00 + nPos,TBSTYLE_AUTOSIZE,iIcon);
			m_ExternalTool.SetButtonText(nPos, "") ; //m_lstrUtilNames.GetAt(pos));
	
			strTemp = eui->m_strUtilName;
			if(eui->m_bUtilStart)
				strTemp = "*"+strTemp;
			strTemp += "\n";
			strTemp += eui->m_strUtilCmd;
			m_ExternalTool.GetToolBarCtrl().GetToolTips()->UpdateTipText(strTemp, &m_ExternalTool, 0xd00+nPos);
		}

		nPos++;
	}

	CRect rectToolBar;
	m_ExternalTool.GetItemRect(0, &rectToolBar);

	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE |RBBIM_ID |RBBIM_STYLE;
	rbbi.wID = ID_VIEW_EX_TOOL;
	rbbi.cxMinChild = 2;
	if(rectToolBar.Height()<22)
		rbbi.cyMinChild = 22;
	else
		rbbi.cyMinChild = rectToolBar.Height();
	rbbi.fStyle &= ~RBBS_NOGRIPPER;	

	int bc=m_ExternalTool.GetToolBarCtrl().GetButtonCount();
	if(bc>0)
	{
		m_ExternalTool.GetToolBarCtrl().GetItemRect(bc-1,&rectToolBar);
		rbbi.cxIdeal = rectToolBar.right;
	}
	else
		rbbi.cxIdeal = 2;

	m_wndReBar.GetReBarCtrl().SetBandInfo(bandid, &rbbi);

}

void CMainFrame::OnUtils(UINT nID)
{
	//get command
	CString cmd, dir;
	int id = nID-0xd00;
	if(	id <= m_ExternalUtilList.m_UtilList.GetUpperBound() && id>=0)
	{
		cmd = m_ExternalUtilList.m_UtilList.GetAt(id)->m_strUtilCmd;
		dir = cmd;
		dir = dir.Left(dir.ReverseFind('\\')+1);

		char * ch, *ch1 = cmd.GetBuffer(MAX_PATH);
		char * szPath = ch1;
		ch = strchr(szPath, '\"');
		if(ch!=NULL)
		{
			ch1 = ch+1;
			ch = strchr(ch1, '\"');
			if(ch!=NULL)
				*ch ='\0';
		}
		else
		{
			ch = strchr(szPath, ' ');
			if(ch!=NULL)
				*ch = '\0';
		}
		if(ch!=NULL)
			ch++;
		if((UINT)ShellExecute(NULL, "open", ch1 , ch, dir, SW_SHOW )<=32)
			AfxMessageBox(IDS_UTIL_FAIL);

		cmd.ReleaseBuffer();
	}
}

//EXPLORE BAR

void CMainFrame::BuildExpBar(LPCSTR lpszTitle)
{
	try{

	m_wndInstantBar.SetVertWidth(m_nExpWidth);
	if (!m_wndInstantBar.Create(_T(lpszTitle), this, 127))
	{
		TRACE0("Failed to create instant bar\n");
		return ;		// fail to create
	}
	m_wndInstantBar.SetSCBStyle(m_wndInstantBar.GetSCBStyle() |
		SCBS_SIZECHILD);
	m_wndInstantBar.SetBarStyle(m_wndInstantBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndInstantBar.EnableDocking(CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT);
	EnableDocking(CBRS_ALIGN_ANY);

	DockControlBar(&m_wndInstantBar, AFX_IDW_DOCKBAR_LEFT);

	#ifdef _SCB_REPLACE_MINIFRAME
    //m_pFloatingFrameClass = RUNTIME_CLASS(CSCBMiniDockFrameWnd);
	#endif //_SCB_REPLACE_MINIFRAME

	ShowControlBar(&m_wndInstantBar, FALSE, FALSE);

	}catch(...)
	{
	}
}

void CMainFrame::OnViewHis() 
{
	// TODO: Add your command handler code here
	CString str,str1;
//	ShowExpBar(str);
	try{

	if(m_wndInstantBar.GetSafeHwnd()==NULL)
		BuildExpBar("");

	BOOL bNeedInI=false;
	if(m_pwndExpObj == NULL)
		bNeedInI = true;
	else
		str.LoadString(IDS_TITLE_HIS);
	m_wndInstantBar.GetWindowText(str1);
	if(bNeedInI || str1!=str)
	{
		RemoveExp();
		try
		{
		m_pwndExpObj = new CHistoryTree();
		}
		catch(...)
		{
			m_pwndExpObj = NULL;
			return;
		}

		m_pwndExpObj->Create(&m_wndInstantBar);
		str = m_pwndExpObj->GetTitle();

		CRect rect;
		m_wndInstantBar.GetClientRect(&rect);
		m_wndInstantBar.SendMessage(WM_SIZE, 0, ((rect.Height())<<16)+rect.Width());

		m_wndInstantBar.SetWindowText(str);
		if(!m_wndInstantBar.IsVisible())
		{
			ShowControlBar(&m_wndInstantBar, TRUE, FALSE);
		}
		m_pwndExpObj->Initialize();
	}
	else
	{
		if(!m_wndInstantBar.IsVisible())
			m_pwndExpObj->Update(0);
		ShowControlBar(&m_wndInstantBar, !m_wndInstantBar.IsVisible(), FALSE);
	}

	}catch(...)
	{
	}
}

void CMainFrame::OnViewSearch() 
{
	// TODO: Add your command handler code here
	CString str,str1;
//	ShowExpBar(str);
	try{

	if(m_wndInstantBar.GetSafeHwnd()==NULL)
		BuildExpBar("");

	BOOL bNeedInI=false;
	if(m_pwndExpObj == NULL)
		bNeedInI = true;
	else
		str.LoadString(IDS_TITLE_SEARCH);
	m_wndInstantBar.GetWindowText(str1);
	if(bNeedInI || str1!=str)
	{
		RemoveExp();
		try
		{
		m_pwndExpObj = new CSearchbar;
		}
		catch(...)
		{
			m_pwndExpObj = NULL;
			return;
		}

		m_pwndExpObj->Create(&m_wndInstantBar);
		m_pwndExpObj->Initialize();
		str = m_pwndExpObj->GetTitle();

		CRect rect;
		m_wndInstantBar.GetClientRect(&rect);
		m_wndInstantBar.SendMessage(WM_SIZE, 0, ((rect.Height())<<16)+rect.Width());

		m_wndInstantBar.SetWindowText(str);
		if(!m_wndInstantBar.IsVisible())
		{
			ShowControlBar(&m_wndInstantBar, TRUE, FALSE);
		}
	}
	else
	{
		ShowControlBar(&m_wndInstantBar, !m_wndInstantBar.IsVisible(), FALSE);
	}

	}catch(...)
	{
	}
}


void CMainFrame::OnUpdateViewExp(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	try{
		if (pCmdUI->m_nID == ID_VIEW_FOLDER )
		{
			if(m_bSecure)
				pCmdUI->Enable(!m_bSecure);
		}

	BOOL checked = FALSE;
	if(m_wndInstantBar.GetSafeHwnd()!=NULL)
	{
		if(m_wndInstantBar.IsVisible())
		{
			m_wndInstantBar.GetWindowText(ExpStr);
			if(pCmdUI->m_nID == ID_VIEW_EXPLOREBAR_SEARCH )
			{
				if(Expstr1==ExpStr)
				{
					checked = TRUE;
					//pCmdUI->SetCheck();
				}
			}
			else if (pCmdUI->m_nID == ID_VIEW_HIS )
			{	if(Expstr2==ExpStr)
				{
					checked = TRUE;
					//pCmdUI->SetCheck();
				}
			}
			else if (pCmdUI->m_nID == ID_FAVORITES_DROPDOWN)
			{	if(Expstr3==ExpStr)
				{
					checked = TRUE;
					//pCmdUI->SetCheck();
				}
			}
			else if (pCmdUI->m_nID == ID_VIEW_FOLDER )
			{
				if(m_bSecure)
					pCmdUI->Enable(!m_bSecure);
				else if(Expstr4==ExpStr)
				{
					checked = TRUE;
					//pCmdUI->SetCheck();
				}
			}
			else if (pCmdUI->m_nID == ID_VIEW_EXPLOREBAR_RESOURCE )
			{	if(Expstr5==ExpStr)
				{
					checked = TRUE;
					//pCmdUI->SetCheck();
				}
			}
			ToolBarSetCheck(pCmdUI, checked);
			/*
			if(checked)
				if(m_strBGPic == "" || pCmdUI->m_pMenu !=NULL)
					pCmdUI->SetCheck();
				else
				{
					pCmdUI->SetCheck(0);
					m_wndToolBar.GetToolBarCtrl().PressButton(pCmdUI->m_nID, TRUE);
				}*/

		}
		else
		{
			ToolBarSetCheck(pCmdUI, FALSE);
		/*	pCmdUI->SetCheck(0);
			m_wndToolBar.GetToolBarCtrl().PressButton(pCmdUI->m_nID, FALSE);*/
		}
	}

	}/*catch(CMemoryException * e)
	{
//		AfxMessageBox("update view exp error");
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}

}



void CMainFrame::RemoveExp()
{
	try{

	if(m_pwndExpObj != NULL)
	{
		m_pwndExpObj->Destroy();
		delete m_pwndExpObj;
		m_pwndExpObj = NULL;
	}

	}/*catch(CMemoryException * e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}

	//ShowControlBar(&m_wndInstantBar, FALSE, FALSE);
}

void CMainFrame::FullScreenModeOn()
{
	// available only if there is an active doc
	CMDIChildWnd* pChild=MDIGetActive();
	if(!pChild) return;

	m_bLinkBarWasVisible=(m_LinkBar.IsWindowVisible()!=0);
	m_LinkBar.ShowWindow(SW_HIDE);
	GetMenu()->CheckMenuItem(ID_VIEW_LINKS, MF_UNCHECKED);
	m_bFavBarWasVisible=(m_FavBar.IsWindowVisible()!=0);
	m_FavBar.ShowWindow(SW_HIDE);
	GetMenu()->CheckMenuItem(ID_VIEW_FAVS, MF_UNCHECKED);
	m_bExternalBarWasVisible=(m_ExternalTool.IsWindowVisible()!=0);
	m_ExternalTool.ShowWindow(SW_HIDE);
	GetMenu()->CheckMenuItem(ID_VIEW_EX_TOOL, MF_UNCHECKED);

	m_bAddressBarWasVisible=(m_AddressBar.IsWindowVisible()!=0);
	m_AddressBar.ShowWindow(SW_HIDE);
	GetMenu()->CheckMenuItem(ID_VIEW_ADDRESSBAR, MF_UNCHECKED);
	m_bStatusBarWasVisible=(m_wndStatusBar.IsWindowVisible()!=0);
	m_wndStatusBar.ShowWindow(SW_HIDE);

//	m_wndReBar.ShowWindow(SW_HIDE);

	// first create the new toolbar
	// this will contain the full-screen off button
/*	m_pwndFullScreenBar=new CToolBar;
	m_pwndFullScreenBar->Create(this);
	m_pwndFullScreenBar->LoadToolBar(IDR_FULLSCREEN);
	m_pwndFullScreenBar->SetBarStyle(m_pwndFullScreenBar->GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	// to look better:
	m_pwndFullScreenBar->ModifyStyle(0, TBSTYLE_FLAT);
	m_pwndFullScreenBar->EnableDocking(0);
	// place the full-screen off button somewhere:
	CPoint pt(300,200);
	FloatControlBar(m_pwndFullScreenBar,pt);*/
	
	// now save the old positions of the main and child windows
	GetWindowRect(&m_mainRect);
	// remove the caption of the mainWnd:
	LONG style=::GetWindowLong(m_hWnd,GWL_STYLE);
	style&=~WS_CAPTION;
	::SetWindowLong(m_hWnd,GWL_STYLE,style);
	int screenx=GetSystemMetrics(SM_CXSCREEN);
	int screeny=GetSystemMetrics(SM_CYSCREEN);
	// resize:
	SetWindowPos(NULL,-4,-3,screenx+8,screeny+6,SWP_NOZORDER);
	style=::GetWindowLong(pChild->m_hWnd,GWL_STYLE);
	m_bChildMax=(style & WS_MAXIMIZE)?true:false;
	// note here: m_bMainMax is not needed since m_hWnd only
	// changed its caption...

//2.3 autohide task bar
/*	APPBARDATA abd;
	abd.cbSize = sizeof(abd);
	// Ignored members: hwnd, uCallbackMessage, uEdge, rc, 
	// lParam
//	 BOOL fTaskBarIsAlwaysOnTop =    SHAppBarMessage(ABM_GETSTATE, &abd) & ABS_ALWAYSONTOP;
	fTaskBarIsAutohide =    SHAppBarMessage(ABM_GETSTATE, &abd) & ABS_AUTOHIDE;
	SHAppBarMessage(ABM_GETTASKBARPOS, &abd);
	uEdge = abd.uEdge;
	
	if(!fTaskBarIsAutohide)
	{
		HWND hTaskBars=::FindWindowEx(0,0,"Shell_TrayWnd",NULL);
		if(hTaskBars!=NULL)
		{AfxMessageBox("dd");
			abd.cbSize = sizeof(abd);
			abd.hWnd = hTaskBars;
			abd.uEdge = uEdge;
			abd.lParam = TRUE;    // Make us autohide
			// Ignored members: uCallbackMessage, rc
			SHAppBarMessage(ABM_SETAUTOHIDEBAR, &abd);
		}
	}*/

	// and maximize the child window
	// it will remove its caption, too.
	pChild->ShowWindow(SW_SHOWMAXIMIZED);
	RecalcLayout();
	m_bFullScreenMode=TRUE;
}

void CMainFrame::FullScreenModeOff()
{
	// You can use SaveBarState() in OnClose(),
	// so remove the newly added toolbar entirely
	// in order SaveBarState() not
	// to save its state. That is why I used dynamic
	// allocation
//	delete m_pwndFullScreenBar;
	LONG style=::GetWindowLong(m_hWnd,GWL_STYLE);
	style|=WS_CAPTION;
	::SetWindowLong(m_hWnd,GWL_STYLE,style);
	
	if(m_bLinkBarWasVisible)
	{
		m_LinkBar.ShowWindow(SW_SHOW);
		GetMenu()->CheckMenuItem(ID_VIEW_LINKS, MF_CHECKED);
	}
	if(m_bFavBarWasVisible)
	{
		m_FavBar.ShowWindow(SW_SHOW);
		GetMenu()->CheckMenuItem(ID_VIEW_FAVS, MF_CHECKED);
	}
	if(m_bAddressBarWasVisible)
	{
		m_AddressBar.ShowWindow(SW_SHOW);
		GetMenu()->CheckMenuItem(ID_VIEW_ADDRESSBAR, MF_CHECKED);
	}
	if(m_bExternalBarWasVisible)
	{
		m_ExternalTool.ShowWindow(SW_SHOW);
		GetMenu()->CheckMenuItem(ID_VIEW_EX_TOOL, MF_CHECKED);
	}

	if(m_bStatusBarWasVisible)
		m_wndStatusBar.ShowWindow(SW_SHOW);

//	m_wndReBar.ShowWindow(SW_SHOW);

	MoveWindow(&m_mainRect);
	RecalcLayout();
	CMDIChildWnd* pChild=MDIGetActive();
	// pchild can be NULL if the USER closed all the
	// childs during Full Screen Mode:
	if(pChild){
		if(m_bChildMax)
			MDIMaximize(pChild);
		else MDIRestore(pChild);
	}
	m_bFullScreenMode=FALSE;
}




void CMainFrame::OnViewFullscreen() 
{
	// TODO: Add your command handler code here
	if(m_bFullScreenMode)
		FullScreenModeOff();
	else FullScreenModeOn();	
}

void CMainFrame::OnUpdateViewFullscreen(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//	pCmdUI->SetCheck(m_bFullScreenMode);
	try{
	ToolBarSetCheck(pCmdUI, m_bFullScreenMode);
	}/*catch(CMemoryException * e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}

	/*
	if(m_bFullScreenMode)
	{
		if(m_strBGPic == "" || pCmdUI->m_pMenu !=NULL)
			pCmdUI->SetCheck();
		else
		{
			pCmdUI->SetCheck(0);
			m_wndToolBar.GetToolBarCtrl().PressButton(pCmdUI->m_nID, TRUE);
		}
	}
	else
	{
//		m_wndToolBar.GetToolBarCtrl().PressButton(pCmdUI->m_nID, FALSE);
		pCmdUI->SetCheck(0);
	}*/
}

BOOL CMainFrame::GetProxySetting(CString &proxy, CString &bypass, CString &user, CString &pwd)
{
	LPINTERNET_PROXY_INFO lpipi=NULL;
	DWORD dwSize;

	try{

	//This call determines the buffer size needed
	InternetQueryOption(NULL,INTERNET_OPTION_PROXY,NULL,&dwSize);

	//allocate the necessary memory
	try
	{
	lpipi = (LPINTERNET_PROXY_INFO)new char[dwSize];
	}
	catch(...)
	{
//		if(e!=NULL)e->Delete();
		return FALSE;
	}

	if(lpipi !=NULL)
	{
		//Call InternetQueryOption again with the buffer provided
		InternetQueryOption(NULL, INTERNET_OPTION_PROXY,lpipi,&dwSize);

		if(lpipi->dwAccessType == INTERNET_OPEN_TYPE_PROXY)
		{
					//save the proxy setting and clear it.
			proxy = lpipi->lpszProxy;
			bypass = lpipi->lpszProxyBypass;

			//save user and password
	/*		DWORD dwSize;
			char * lpszData;
			//This call determines the buffer size needed
			InternetQueryOption(NULL,INTERNET_OPTION_PROXY_USERNAME,NULL,&dwSize);

			//allocate the necessary memory
			lpszData = new char[dwSize];

			//Call InternetQueryOption again with the buffer provided
			InternetQueryOption(NULL, INTERNET_OPTION_PROXY_USERNAME,lpszData,&dwSize);

			//Insert code to user the user agent string information
			user = lpszData;
			//Free the allocated memory
			delete [] lpszData;
			//This call determines the buffer size needed
			InternetQueryOption(NULL,INTERNET_OPTION_PROXY_PASSWORD,NULL,&dwSize);

			//allocate the necessary memory
			lpszData = new char[dwSize];

			//Call InternetQueryOption again with the buffer provided
			InternetQueryOption(NULL, INTERNET_OPTION_PROXY_PASSWORD,lpszData,&dwSize);

			//Insert code to user the user agent string information
			pwd = lpszData;
			//Free the allocated memory
			delete [] lpszData;*/
			delete[] lpipi;
			lpipi = NULL;

			return TRUE;
		}
	

//		delete[] lpipi;
//		lpipi = NULL;
	}

	}
/*	catch(CMemoryException* e)
	{
//		AfxMessageBox("get proxy error");
		if(lpipi)
			delete[] lpipi;
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}

	try{
		if(lpipi)
			delete[] lpipi;
	}
	catch(...)
	{
	}

	return FALSE;
}


//message from another instance
void CMainFrame::OnShellOpen(WPARAM wParam,LPARAM)
{
	try{

	/*beign :user share memory*/
	LPSTR lpData;
	HANDLE hMapping = CreateFileMapping
	((HANDLE)0xFFFFFFFF,NULL,
	PAGE_READWRITE,0,2560,"MY2IESHARE");
	if (hMapping==NULL){
		AfxMessageBox("Fail to create share memory!");
		return;
	}
	lpData = (LPSTR) MapViewOfFile(hMapping,FILE_MAP_ALL_ACCESS,0,0,0);
	if (lpData==NULL){
		AfxMessageBox("MapViewOfFile Fail");
		CloseHandle(hMapping);
		return;
	}
	//open file
//	nOpenType=2;
	CString	strURL = lpData, tmp;
	//AfxGetApp()->OpenDocumentFile(lpData);
	//release memory
	UnmapViewOfFile(lpData);
	CloseHandle(hMapping);
	/*end :user share memory*/
	//PostMessage(WM_COMMAND, ID_FILE_NEW_S);

	//remove the tray icon
	if(bSysTray && !IsWindowVisible())
	{
		NOTIFYICONDATA nid;
		nid.cbSize=sizeof(nid);
		nid.hWnd=m_hWnd;
		nid.uID=IDR_MAINFRAME + nTrayIconID;
		nid.uFlags=NIF_ICON|NIF_TIP|NIF_MESSAGE;
		nid.uCallbackMessage=WSM_TRAY;
		Shell_NotifyIcon(NIM_DELETE, &nid);
		ShowWindow(SW_SHOW);
		ShowWindow(SW_RESTORE);
	}

	TCHAR           buf[INTERNET_MAX_PATH_LENGTH];
	tmp = strURL.Right(4);
	tmp.MakeLower();
	if(tmp == _T(".url"))
	{
		// an .URL file is formatted just like an .INI file, so we can
		// use GetPrivateProfileString() to get the information we want
		::GetPrivateProfileString(_T("InternetShortcut"), _T("URL"),
								  _T(""), buf, INTERNET_MAX_PATH_LENGTH,
								  strURL);

		strURL =  buf;
	}
	NewChildWindow(1,2,strURL, NULL, TRUE);

	}
	catch(...)
	{
	}
}




BOOL CMainFrame::ResolveLink(LPCSTR lpszLinkFile, LPSTR lpszPath)
{
    static char szGotPath[MAX_PATH]; 
    static char szDescription[MAX_PATH]; 
    static WCHAR wsz[MAX_PATH]; 

	if(lpszLinkFile == NULL)
		return FALSE;

    HRESULT hres; 
    IShellLink* psl = NULL; 
    IPersistFile* ppf = NULL; 
    WIN32_FIND_DATA wfd; 

	try
	{
    *lpszPath = 0; // assume failure 
 
    // Get a pointer to the IShellLink interface. 
    hres = CoCreateInstance(CLSID_ShellLink, NULL, 
        CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID *) &psl); 
    if (SUCCEEDED(hres)) { 
        // Get a pointer to the IPersistFile interface. 
        hres = psl->QueryInterface(IID_IPersistFile, 
            (void**)&ppf); 
        if (SUCCEEDED(hres)) { 
 
            // Ensure that the string is Unicode. 
            MultiByteToWideChar(CP_ACP, 0, lpszLinkFile, -1, wsz, 
                MAX_PATH); 

            // Load the shortcut. 
            hres = ppf->Load(wsz, STGM_READ); 
            if (SUCCEEDED(hres)) { 
 
               // Resolve the link. 
                hres = psl->Resolve(this->m_hWnd, SLR_ANY_MATCH); 
                if (SUCCEEDED(hres)) { 
 
                    // Get the path to the link target. 
                    hres = psl->GetPath(szGotPath, 
                        MAX_PATH, (WIN32_FIND_DATA *)&wfd, 
                        SLGP_SHORTPATH ); 
                    //if (FAILED(hres)) 
                        //HandleErr(hres); // application-defined function 

                    // Get the description of the target. 
                    hres = psl->GetDescription(szDescription, MAX_PATH); 
                    //if (FAILED(hres)) 
                       // HandleErr(hres); 
                    lstrcpy(lpszPath, szGotPath); 
                } 
            } 
        // Release the pointer to the IPersistFile interface. 
        ppf->Release(); 
		ppf = NULL;
        } 
    // Release the pointer to the IShellLink interface. 
    psl->Release(); 
	psl = NULL;
    } 

	}
/*	catch(CMemoryException * e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{//2.1
//		AfxMessageBox("resolve link error");
//		if(ppf)
//			ppf->Release();
//		if(psl)
//			psl->Release();
	}

    return SUCCEEDED(hres); 

}



void CMainFrame::OnFileNew() 
{
	// TODO: Add your command handler code here
	NewChildWindow(1,1,NULL, NULL, TRUE);
//	if(m_bActiveNewWindow)
		m_wndAddress->SetFocus();
}



CChildFrame* CMainFrame::NewChildWindow(int nIniLevel, int nOpenType, LPCSTR strUrl, CMyIEView * pParentView, BOOL bForceActive)
{
	//find if there any unsed window then reuse it.
	int n = nWin; //m_wndTab.GetItemCount();
	BOOL found = FALSE;
	int i=0;
	TCITEM TabCtrlItem;
	CString str;
	CChildFrame *pWnd = NULL;
	CString url;

	try{

	TabCtrlItem.mask = TCIF_PARAM;
	str = "";

	if(nIniLevel <2)
	{
		//check the blank window first;
		if(pParentView != NULL && bFilteBlank)
		{
			try{

			CChildFrame* tcf = ((CChildFrame*)pParentView->GetParentFrame())->m_pBlankChild;
			if(tcf!=NULL)
			{
				tcf->m_pView->GetLocationURL(url);
//				url = tcf->m_pView->m_lpszUrl;
				double secs = 1;
				if(url == "")
				{
					//get time
					LARGE_INTEGER liFinish;
					QueryPerformanceCounter(&liFinish);
					LARGE_INTEGER liFreq;
					QueryPerformanceFrequency(&liFreq);
					secs = (double)(liFinish.LowPart - ((CChildFrame*)pParentView->GetParentFrame())->m_tBlankChild.LowPart)/(double)liFreq.LowPart;
				}
				if(url=="about:blank" || secs<0.25) //if the time is too short we assume it as an blank window.
				{
					pWnd = tcf;
					found = TRUE;
					pWnd->m_pView->m_nProgress = 0;
				}
			}
			}catch(...)
			{
				found = FALSE;
			}
		}

		i=0;
		while (i<n && !found)
		{
			TabCtrlItem.mask = TCIF_PARAM;
			TabCtrlItem.lParam = 0;
			m_wndTab.GetItem(i, &TabCtrlItem);
			if(TabCtrlItem.lParam != NULL)
			{	
				if(((CChildFrame *)TabCtrlItem.lParam)->m_pView->m_nProgress<0)
				{
					str = ((CChildFrame *)TabCtrlItem.lParam)->m_pView->m_lpszUrl;
					if(str.IsEmpty())
						((CChildFrame *)TabCtrlItem.lParam)->GetWindowText(str);
					if(str == "about:blank")
					{
						pWnd = ((CChildFrame *)TabCtrlItem.lParam);
						found = TRUE;
						pWnd->m_pView->m_nProgress = -1;
						pWnd->m_pView->m_bReUsed = TRUE;
					}
					else
						i++;
				}
				else
					i++;
			}
			else
				i++;
		}
	}

	if(!found)
	{
		try
		{
		pWnd = new CChildFrame;
		}
		catch(...)
		{
			return NULL;
		}

		if(pWnd == NULL)
			return NULL;
		pWnd->m_bForceActive = bForceActive;

		if (!pWnd->Create(_T("ChildFrame"), "",
			WS_CHILD  | WS_OVERLAPPEDWINDOW, rectDefault, this))
		{
			delete pWnd;
			return NULL;
		}

		if(bForceActive)
			pWnd->ShowWindow(SW_SHOW);
	}

	if(pWnd!=NULL && pWnd->m_pView !=NULL)
	{
		pWnd->m_pView->m_bFirst = TRUE;

		pWnd->m_pView->m_nProgress = -1;

//		if(strUrl == "http://changyou.tsx.org/")
//			pWnd->m_pView->m_bAutoFilter=TRUE;

		//inherite parent's settings
		if(pParentView!=NULL)
		{
			pWnd->m_pView->m_strWebProxy = pParentView->m_strWebProxy;
			pWnd->m_pView->m_bEnWebProxy = pParentView->m_bEnWebProxy;
			pWnd->m_pView->dwProperty = pParentView->dwProperty;
			pWnd->m_pView->m_nSrSpeed = pParentView->m_nSrSpeed; //1-slow, 2-medium, 3-fast
			pWnd->m_pView->m_bScrollAfterDL = pParentView->m_bScrollAfterDL;
			pWnd->m_pView->m_bUseAutoScroll = pParentView->m_bUseAutoScroll;

		}
		else
		{
			pWnd->m_pView->m_nSrSpeed = m_nDefSrSpeed; //1-slow, 2-medium, 3-fast
			pWnd->m_pView->m_bScrollAfterDL = m_bDefScrollAfterDL;
			pWnd->m_pView->m_bUseAutoScroll = FALSE;
		}

		if(nIniLevel && nOpenType == 2)
			pWnd->m_pView->m_lpszUrl = strUrl;

		//if(!m_bActiveNewWindow)
		if(!found)
		{
		//	if(!pWnd->m_pView->IsActive)
		//		MDINext();
		}
		else
			if( nTotalTabs==0 || bForceActive)//m_bActiveNewWindow ||
			{
				pWnd->m_pView->IsActive=TRUE;
				MDIActivate(pWnd);
				m_wndTab.SetCurSel(i);
			}
			else
			{	
				//pWnd->m_pView->IsActive=FALSE;
			}

		if(nIniLevel)
			pWnd->m_pView->OnInitialUpdate(nOpenType, strUrl);

		//this cause the page isn't added in history
		//pWnd->m_pView->m_wndBrowser.SetSilent(TRUE);
	}

	}/*catch(CMemoryException * e)
	{
//		AfxMessageBox("new window1 error");
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}

	return pWnd;//?
}


void CMainFrame::OnUpdateOptionsActivenewwindow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//2.2	try{
	pCmdUI->SetCheck(m_bActiveNewWindow );
//	}catch(...)
//	{
//	}

}


void CMainFrame::OnViewFavorite()
{
	CString str,str1;
//	ShowExpBar(str);
	try
	{

	if(m_wndInstantBar.GetSafeHwnd()==NULL)
		BuildExpBar("");

	BOOL bNeedInI=false;
	if(m_pwndExpObj == NULL)
		bNeedInI = true;
	else
		str.LoadString(IDS_TITLE_FAVORITE);
	m_wndInstantBar.GetWindowText(str1);
	if(bNeedInI || str1!=str)
	{
		RemoveExp();
		try{
		m_pwndExpObj = new CFavTree;
		}
		catch(...)
		{
			m_pwndExpObj = NULL;
			return;
		}

		m_pwndExpObj->Create(&m_wndInstantBar);
		m_pwndExpObj->Initialize();
		str = m_pwndExpObj->GetTitle();

		CRect rect;
		m_wndInstantBar.GetClientRect(&rect);
		m_wndInstantBar.SendMessage(WM_SIZE, 0, ((rect.Height())<<16)+rect.Width());

		m_wndInstantBar.SetWindowText(str);
		if(!m_wndInstantBar.IsVisible())
		{
			ShowControlBar(&m_wndInstantBar, TRUE, FALSE);
		}
	}
	else
	{
		ShowControlBar(&m_wndInstantBar, !m_wndInstantBar.IsVisible(), FALSE);
	}

	}
	catch(...)
	{
//		if(e!=NULL)e->Delete();
//		return;
	}
}

BOOL CMainFrame::HasExpBar(int nStr)
{
//	static CString str,str1;
	try{
	if(m_wndInstantBar.GetSafeHwnd()==NULL)
		return FALSE;

	if(m_pwndExpObj == NULL)
		return FALSE;
	else
		ExpStr.LoadString(nStr);
	m_wndInstantBar.GetWindowText(gString2);
	if(ExpStr!=gString2)
		return FALSE;
	else
		return TRUE;
	}catch(...)
	{
		return FALSE;
	}

}

void CMainFrame::OnCloseOther() 
{
	// TODO: Add your command handler code here
	if(AfxMessageBox(IDS_CLOSE_ALL_CONF, MB_YESNO|MB_ICONQUESTION)==IDNO)
		return;

	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;

	try
	{
	m_wndTab.GetItem(nRTabID, &TabCtrlItem);
	long cparam=TabCtrlItem.lParam;
	CChildFrame* tcf;
	int n = m_wndTab.GetItemCount();
	TabCtrlItem.mask = TCIF_PARAM;
	int l =n-1;

	for(int i=n-1;i>=0;i--)
	{
		m_wndTab.GetItem(l, &TabCtrlItem);
		tcf = ((CChildFrame*)TabCtrlItem.lParam);
		if(tcf != NULL && ((long)tcf)!=cparam)
		{
			if(!tcf->bLock)
			{
				tcf->m_pView->m_bToClose = TRUE;
				tcf->SendMessage(WM_CLOSE);
			}
		}
		l--;
	}
	}
/*	catch(CMemoryException * e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}

}

void CMainFrame::OnRtabAddfav() 
{
	// TODO: Add your command handler code here
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;

		m_wndTab.GetItem(nRTabID, &TabCtrlItem);
		CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
		AddFavorite(tcf);
}

void CMainFrame::AddFavorite(CChildFrame* tcf)
{
	try{
	CString strURL;
	if (tcf!=NULL)
	{
		CMyIEView* pView = (CMyIEView*)tcf->m_pView;

		if(pView != NULL)
		{
			VARIANT vtTitle;
			vtTitle.vt=VT_BSTR;
			tcf->GetWindowText(strURL);
			vtTitle.bstrVal = strURL.AllocSysString();
				
			pView->GetLocationURL(strURL);
			BSTR URL = strURL.AllocSysString();
			AddtoFavorite(URL, &vtTitle);
			SysFreeString(URL);
			SysFreeString(vtTitle.bstrVal);

		}
	}
		
	}catch(...)
	{
	}

}



int CMainFrame::OpenAllFavs(LPCSTR lpszPath, BOOL bMostFav)
{
	WIN32_FIND_DATA wfd;
	HANDLE          h;
//	static CString         str;
	CString         strPath2(lpszPath);
	static TCHAR           buf[INTERNET_MAX_PATH_LENGTH], tmp[10];
	int				nWins = 0;
	CString strPath(lpszPath);

//	strPath2 = lpszPath;
//	strPath = lpszPath;
	strPath += "*.*";

	if(strPath2=="" || strPath2.IsEmpty())
		return 0;


	// now scan the directory, first for .URL files and then for subdirectories
	// that may also contain .URL files
	h = FindFirstFile(strPath, &wfd);
	if(h != INVALID_HANDLE_VALUE)
	{
		try
		{
		do
		{
			if((wfd.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_SYSTEM))==0)
			{
				gString2 = wfd.cFileName;
				if(gString2.Right(4) == _T(".url"))
				{
					tmp[0]=0;
					// an .URL file is formatted just like an .INI file, so we can
					// use GetPrivateProfileString() to get the information we want
					::GetPrivateProfileString(_T("InternetShortcut"), _T("URL"),
											  _T(""), buf, INTERNET_MAX_PATH_LENGTH,
											  strPath2 + gString2);
					::GetPrivateProfileString(_T("MyIE"), _T("MostFav"),
											  _T(""), tmp, 10,
											  strPath2 + gString2);
					//str = str.Left(str.GetLength() - 4);

					// scan through the array and perform an insertion sort
					// to make sure the menu ends up in alphabetic order
					/*for(nPos = nStartPos ; nPos < nEndPos ; ++nPos)
					{
						if(str.CompareNoCase(astrFavorites[nPos]) < 0)
							break;
					}*/
					if(!bMostFav || (bMostFav && tmp[0]=='1'))
					{
						if(nWins !=0)
							m_astrOpenAll.AddTail(buf);
						else
							NewChildWindow(1,2, buf);
						nWins ++;
					}
				}
				else  
				{
					if(!bMostFav && ResolveLink(strPath2 + gString2, buf))
					{
						//str = str.Left(str.GetLength() - 4);

						// scan through the array and perform an insertion sort
						// to make sure the menu ends up in alphabetic order
						/*for(nPos = nStartPos ; nPos < nEndPos ; ++nPos)
						{
							if(str.CompareNoCase(astrFavorites[nPos]) < 0)
								break;
						}*/
						if(nWins !=0)
							m_astrOpenAll.AddTail(buf);
						else
							NewChildWindow(1,2, buf);
						nWins ++;
					}
				}
			}
		} while(FindNextFile(h, &wfd) && (nWins<nMaxWins || nMaxWins==0));
		FindClose(h);
		}
		catch(...)
		{
		}

		if(m_nInterval && nWins>1)
			m_nTimerID = SetTimer(1000, m_nInterval*1000, NULL);
		else if(nWins>1)
			m_nTimerID = SetTimer(1000, 20, NULL);
		//enable toolbar button
		m_wndToolBar.GetToolBarCtrl().EnableButton(ID_FILE_STOP_OPENALL);
	} 
	return 1;
}

BOOL CMainFrame::GetFavoriteFolder(CString & strFav)
{
//	TCHAR           sz[MAX_PATH];
	static TCHAR           szPath[MAX_PATH];
//	HKEY            hKey;
//	DWORD           dwSize;

	BOOL r = TRUE;
/*	HINSTANCE hIns = LoadLibrary("shell32.dll");
	if(hIns== NULL)
		r = FALSE;
	else
	{
		LPFNDLLFUNC2 lpfnDllFunc2 = (LPFNDLLFUNC2)GetProcAddress(hIns, "SHGetSpecialFolderPathA");
		if(lpfnDllFunc2!=NULL)
			r = lpfnDllFunc2(m_hWnd,szPath,	CSIDL_FAVORITES,FALSE);
		else
			r = FALSE;
	}

	FreeLibrary(hIns);

	if(r)
	{
		strFav = szPath;
	}
	else */
	{
		try
		{
	   LPITEMIDLIST pidlFavorites = NULL;
	   // Get a pointer to the IMalloc interface
	   IMalloc * m_pMalloc = NULL;
	   HRESULT hr = SHGetMalloc(&m_pMalloc);
	   if (SUCCEEDED(hr))
	   {
		   HRESULT hr = SHGetSpecialFolderLocation(NULL, CSIDL_FAVORITES, &pidlFavorites);
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

	   if(r)
	   {
		   strFav = szPath;
	   }
	   else
	   {
			// find out from the registry where the favorites are located.
/*			if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\User Shell Folders"), &hKey) != ERROR_SUCCESS)
			{
				if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"), &hKey) != ERROR_SUCCESS)
				{
					AfxMessageBox("Favorites folder not found\n");
					return FALSE;
				}
			}
			sz[0]=0;
			dwSize = sizeof(sz);
			RegQueryValueEx(hKey, _T("Favorites"), NULL, NULL, (LPBYTE)sz, &dwSize);
		
			if(dwSize == 0 || strlen(sz)==0)
			{
				RegCloseKey(hKey);
				if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"), &hKey) != ERROR_SUCCESS)
				{
					AfxMessageBox("Favorites folder not found\n");
					return FALSE;
				}
				sz[0]=0;
				dwSize = sizeof(sz);
				RegQueryValueEx(hKey, _T("Favorites"), NULL, NULL, (LPBYTE)sz, &dwSize);
			}

			ExpandEnvironmentStrings(sz, szPath, MAX_PATH);
			RegCloseKey(hKey);

			if(dwSize>0 && strlen(sz)>0)
				strFav = szPath;
			else */
			{AfxMessageBox("Can't get path of favorite.");
				::GetWindowsDirectory (szPath, MAX_PATH); 
				strFav = szPath;
				strFav += "\\favorites";
			}
	   }

		}
/*		catch(CMemoryException * e)
		{
			if(e!=NULL)e->Delete();
		}*/
		catch(...)
		{
		}
	}

	return TRUE;

}

/*void CMainFrame::OnUpdateFavoritesOrgnizefavorites(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_bFavDirty)
	{
		InitFavorites();
		m_bFavDirty = FALSE;
	}
	
}*/

void CMainFrame::OnLinkPopup() 
{
	CPoint pt,pt2;
	GetCursorPos(&pt);
	m_LinkBar.ScreenToClient(&pt);
	int iButton = m_LinkBar.GetToolBarCtrl().HitTest(&pt);
	if (iButton >= 0 && iButton<m_LinkBar.GetToolBarCtrl().GetButtonCount()) // if mouse is over a button:
	{
		TBBUTTON tbb;
		CRect rect;
		if( m_LinkBar.GetToolBarCtrl().GetButton( iButton,  &tbb ))
		{
			m_LinkBar.GetToolBarCtrl().GetItemRect(iButton, &rect);
			m_LinkBar.ClientToScreen(&rect);

			HMENU hmenu = (HMENU)tbb.dwData;
			CToolMenu * menu = NULL;
			try
			{
			menu = new CToolMenu;
			menu->Attach(hmenu);
			//::TrackPopupMenu(hmenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left,rect.bottom+1, 0, m_hWnd,NULL);
			menu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left,rect.bottom+1, this);
			menu->Detach();
			}
			catch(...)
			{
//				if(e!=NULL)e->Delete();
			}
			try{
			if(menu)
				delete menu;
			}catch(...)
			{
			}
		}
	}
}

void CMainFrame::OnFavPopup() 
{
	CPoint pt,pt2;
	GetCursorPos(&pt);
	m_FavBar.ScreenToClient(&pt);
	int iButton = m_FavBar.GetToolBarCtrl().HitTest(&pt);
	if (iButton >= 0 && iButton<m_FavBar.GetToolBarCtrl().GetButtonCount()) // if mouse is over a button:
	{
		TBBUTTON tbb;
		CRect rect;
		if( m_FavBar.GetToolBarCtrl().GetButton( iButton,  &tbb ))
		{
			m_FavBar.GetToolBarCtrl().GetItemRect(iButton, &rect);
			m_FavBar.ClientToScreen(&rect);

			HMENU hmenu = (HMENU)tbb.dwData;
			CToolMenu menu;
			menu.Attach(hmenu);
			//::TrackPopupMenu(hmenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left,rect.bottom+1, 0, m_hWnd,NULL);
			menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left,rect.bottom+1, this);
			menu.Detach();
		}
	}
}




void CMainFrame::OnUpdateViewToolbar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	try{

	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_ID|RBBIM_STYLE;
	int nb = m_wndReBar.GetReBarCtrl( ).IDToIndex(ID_VIEW_TOOLBAR);
	m_wndReBar.GetReBarCtrl().GetBandInfo(nb, &rbbi);
	if( rbbi.fStyle & RBBS_HIDDEN )
		pCmdUI->SetCheck(0);
	else
		pCmdUI->SetCheck();
	}catch(...)
	{
//		AfxMessageBox("update view toolbar error");
	}

}

void CMainFrame::OnUpdateFav(WPARAM wParam,LPARAM)
{
	try{

	InitFavorites();

	}
/*	catch(CMemoryException * e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	try{
	CMDIFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	PostMessage(WM_UPDATE_TAB, TRUE);

	}catch(...)
	{
	}
}

void CMainFrame::OnOptionMultiTab() 
{
	// TODO: Add your command handler code here
//	AfxMessageBox(IDS_WARNING);
	if(m_bMultiLineTab)
	{
		m_wndTab.ModifyStyle(TCS_MULTILINE, 0);
		m_bMultiLineTab = FALSE;
	}
	else
	{
		m_wndTab.ModifyStyle(0, TCS_MULTILINE);
		m_bMultiLineTab = TRUE;
	}
	PostMessage(WM_UPDATE_TAB);
}


void CMainFrame::OnUpdateTab(WPARAM wParam,LPARAM)
{
//	if(!m_bMultiLineTab)
//		return;

//	m_wndTab.ModifyStyle(TCS_MULTILINE, TCS_FIXEDWIDTH);
//	
	try{
/*3.2	CRect rectAddress;
	

	m_wndTab.GetItemRect(0, &rectAddress);
	int y = rectAddress.Height();
	if(y==0)
		y=19; //21;*/

	int y = AdjustTabWidth(!wParam);

	int r = m_wndTab.GetRowCount();
	if(r==0)
		r=1;
	if(m_nPreLines==r)
		return;
	m_nPreLines = r;
	
	//adjust tab height
	if(r==1 || !m_bFBTab)
		y+=0; //1
	else
		y+=3;

	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE |RBBIM_ID;
	rbbi.wID = ID_VIEW_TASKBAR;
	rbbi.cxMinChild = 0;
	rbbi.cyMinChild = y*r;// + 10
	rbbi.cxIdeal = 200;

	//add tab to top or bottom
	if(m_bTopTab)
	{
		int nb=m_wndReBar.GetReBarCtrl().IDToIndex(ID_VIEW_TASKBAR);
		m_wndReBar.GetReBarCtrl().SetBandInfo(nb, &rbbi);
	}
	else
	{
		m_wndReBar2.GetReBarCtrl().SetBandInfo(0, &rbbi);
	}

	}/*catch(CMemoryException * e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}

}



void CMainFrame::OnAllFav(UINT nID)
{
	int i = nID-0x1000;
	POSITION pos = m_astrFavoriteFolders.FindIndex(i);
	if(pos!=NULL)
		OpenAllFavs(m_astrFavoriteFolders.GetAt(pos)+"\\");
}

void CMainFrame::OnToolsOpenalllinks() 
{
	// TODO: Add your command handler code here
	try{

	OpenAllFavs(m_strLinkFolder+"\\");

	}/*catch(CMemoryException * e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}
}

void CMainFrame::OnOptionAlwaysNew() 
{
	// TODO: Add your command handler code here
	if(bAddressInNewWindow)
		bAddressInNewWindow = FALSE;
	else
		bAddressInNewWindow = TRUE;

	m_bmUserDefault.m_bActiveNewWindow = m_bActiveNewWindow;
	m_bmUserDefault.m_bAlwaysNewWindow = bAddressInNewWindow;
	m_bmUserDefault.m_nSeq = m_nSeq;
	m_bmUserDefault.bLinkInNewWin = bLinkInNewWindow;

}

void CMainFrame::OnUpdateOptionAlwaysNew(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//2.2	try{
	pCmdUI->SetCheck(bAddressInNewWindow );
//	}catch(...)
//	{
//	}

}

void CMainFrame::OnFileCloseKeepcur() 
{
	// TODO: Add your command handler code here
	if(AfxMessageBox(IDS_CLOSE_ALL_CONF, MB_YESNO|MB_ICONQUESTION)==IDNO)
		return;

	CChildFrame* tcf, *cur;
	cur = (CChildFrame*)MDIGetActive();
	int n = m_wndTab.GetItemCount();
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;
	int l =n-1;

	for(int i=n-1;i>=0;i--)
	{
		m_wndTab.GetItem(l, &TabCtrlItem);
		tcf = ((CChildFrame*)TabCtrlItem.lParam);
		if(tcf != NULL)
		{
			if(!tcf->bLock && tcf!=cur)
			{
				tcf->m_pView->m_bToClose = TRUE;
				tcf->SendMessage(WM_CLOSE);
			}
		}
		l--;
	}
	
}

/*void CMainFrame::OnOptionsNewtab() 
{
	// TODO: Add your command handler code here
	if(m_bNextTab )
		m_bNextTab = FALSE;
	else
		m_bNextTab = TRUE;
}

void CMainFrame::OnUpdateOptionsNewtab(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//2.2	try{
	pCmdUI->SetCheck(m_bNextTab );
//	}catch(...)
//	{
//	}

}*/

void CMainFrame::OnViewRefreshall() 
{
	// TODO: Add your command handler code here
	CChildFrame* tcf;
	int n = m_wndTab.GetItemCount();
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;
	int l =n-1;

	try{

	for(int i=n-1;i>=0;i--)
	{
		m_wndTab.GetItem(l, &TabCtrlItem);
		tcf = ((CChildFrame*)TabCtrlItem.lParam);
		if(tcf != NULL && tcf->m_pView!=NULL)
		{
				tcf->m_pView->Refresh2(REFRESH_COMPLETELY);
		}
		l--;
	}

	}catch(...)
	{
	}
}

void CMainFrame::OnViewStopAll() 
{
	// TODO: Add your command handler code here
	CChildFrame* tcf;
	int n = m_wndTab.GetItemCount();
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;
	int l =n-1;

	for(int i=n-1;i>=0;i--)
	{
		m_wndTab.GetItem(l, &TabCtrlItem);
		tcf = ((CChildFrame*)TabCtrlItem.lParam);
		if(tcf != NULL && tcf->m_pView!=NULL)
		{
				tcf->m_pView->Stop();
		}
		l--;
	}	
}

void CMainFrame::OnHelpHomepage() 
{
	// TODO: Add your command handler code here
	NewChildWindow(1,2,"http://changyou.mainpage.net/");
	
}



int CMainFrame::SetTabIcon(int nIcon, int nItem)
{
//	BOOL fail = FALSE;
	try
	{
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_IMAGE;
	TabCtrlItem.iImage = nIcon;
	m_wndTab.SetItem( nItem, &TabCtrlItem );
	}
/*	catch(CMemoryException * e)
	{
//		AfxMessageBox("set icon error");
		if(e!=NULL)e->Delete();
		fail = TRUE;
	}*/
	catch(...)
	{
//		fail = TRUE;
	}

	//2.0
/*	try{
	if(fail)
	{
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_IMAGE;
		m_wndTab.GetItem( nItem, &TabCtrlItem );
		nIcon = TabCtrlItem.iImage;
	}
	}catch(...)
	{
	}
*/
	return nIcon;
}

void CMainFrame::OnUpdateTabTips(WPARAM wParam,LPARAM)
{
	CChildFrame* tcf;
	int n = nWin;//m_wndTab.GetItemCount();
	int l =n-1;
//	static CString str;
	TCITEM TabCtrlItem;

	try
	{
	TabCtrlItem.mask = TCIF_PARAM;
	CToolTipCtrl* pTT = m_wndTab.GetToolTips();
	if(pTT !=NULL)
		for(int i=n-1;i>=1;i--)
		{
			TabCtrlItem.lParam = 0;
			m_wndTab.GetItem(l, &TabCtrlItem);
			tcf = ((CChildFrame*)TabCtrlItem.lParam);
			if(tcf != NULL && tcf->m_pView != NULL && !tcf->m_pView->m_bToClose)
			{
				tcf->GetWindowText(tip);
				pTT->UpdateTipText(tip, &m_wndTab, i);
			}
			l--;
		}
	}
/*	catch(CMemoryException * e)
	{
	//	AfxMessageBox("update tip error");
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}
}


void CMainFrame::OnWindowCascade() 
{
	// TODO: Add your command handler code here
	if(m_nProtectNum>0)
	{
		AfxMessageBox(IDS_CANT_DO);
		return;
	}
	m_bMax = 2;
	CMDIFrameWnd::OnMDIWindowCmd(ID_WINDOW_CASCADE);
}

void CMainFrame::OnWindowTileHorz() 
{
	// TODO: Add your command handler code here
	if(m_nProtectNum>0)
	{
		AfxMessageBox(IDS_CANT_DO);
		return;
	}
	m_bMax = 2;
	CMDIFrameWnd::OnMDIWindowCmd(ID_WINDOW_TILE_HORZ);
}

void CMainFrame::OnWindowTileVert() 
{
	// TODO: Add your command handler code here
	if(m_nProtectNum>0)
	{
		AfxMessageBox(IDS_CANT_DO);
		return;
	}
	m_bMax = 2;
	CMDIFrameWnd::OnMDIWindowCmd(ID_WINDOW_TILE_VERT);
}

void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	CMDIFrameWnd::OnSettingChange(uFlags, lpszSection);
	
	// TODO: Add your message handler code here
	try{
	m_wndTab.SetFont(m_wndReBar.GetFont());
//	InitialToolBar();
	if(m_nTextLabel==0)
		m_wndToolBar.GetToolBarCtrl().SetButtonSize(CSize(24,22));
//	dbtime=(float)GetDoubleClickTime()/1000;
	m_nScrollWidth = GetSystemMetrics(SM_CXVSCROLL);
	m_nScrollArrowH = GetSystemMetrics(SM_CYVSCROLL);

	}catch(...)
	{
	}
}

void CMainFrame::OnTabNeedText(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	try{
//	static CString str;

    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNotifyStruct;
    UINT nID =pNotifyStruct->idFrom;
    if (pTTT!=NULL  &&  ! (pTTT->uFlags & TTF_IDISHWND))
    {
        // idFrom is actually the HWND of the tool
        if(nID==0)
        {
			CChildFrame* tcf;
			TCITEM TabCtrlItem;
			TabCtrlItem.mask = TCIF_PARAM;

			m_wndTab.GetItem(0, &TabCtrlItem);
			tcf = ((CChildFrame*)TabCtrlItem.lParam);
			if(tcf != NULL)
			{
				if(tcf->m_pView!=NULL)
				{
					tcf->GetWindowText(tip);
					//pTTT->lpszText = str.GetBuffer(1);
					strncpy(pTTT->szText, (LPCSTR)tip, 79);
					pTTT->szText[79]='\0';
				}
			}
        }
    }
    }/*catch(CMemoryException * e)
	{
//		AfxMessageBox("need tip error");
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}
}


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
   ks =GetKeyState(VK_CONTROL);
/*   if (pMsg->message == WM_KEYDOWN && ks>=0)
   {
      if ((m_wndAddress.GetSafeHwnd()==pMsg->hwnd || (::IsChild(m_wndAddress.GetSafeHwnd(), pMsg->hwnd))) && pMsg->wParam == VK_TAB )*/
   if (((pMsg->message == WM_KEYDOWN && ks>=0) ))
   {  
	   hAddr = m_wndAddress->GetSafeHwnd();

      if ( (hAddr==pMsg->hwnd || (::IsChild(hAddr, pMsg->hwnd))) && pMsg->wParam == VK_TAB 
		  || ((hAddr!=pMsg->hwnd && (!::IsChild(hAddr, pMsg->hwnd))) 
		  && (pMsg->wParam ==VK_DOWN || pMsg->wParam == VK_UP ||pMsg->wParam ==VK_PRIOR || pMsg->wParam==VK_NEXT || pMsg->wParam==VK_HOME || pMsg->wParam==VK_END  || ((pMsg->wParam == VK_RIGHT || pMsg->wParam == VK_LEFT)  && (!::IsChild(m_wndInstantBar.GetSafeHwnd(), pMsg->hwnd)) ) ))) //pMsg->hwnd != m_hEdit

      {
         LPDISPATCH lpDispatch;
         IOleInPlaceActiveObject* lpOleInPlaceActiveObj;
		 lpDispatch = NULL;
		 lpOleInPlaceActiveObj= NULL;

		 
		 CChildFrame* tcf;
		 tcf = (CChildFrame*)MDIGetActive();
		 if(tcf != NULL && tcf->m_pView != NULL)
		 {
			 HRESULT hrTranslate;
			 try
			 {
			 lpDispatch = (tcf->m_pView)->GetHtmlDocument();
			 ASSERT(lpDispatch);

			 // Get an IDispatch pointer for the IOleInPlaceActiveObject interface.
			 if(lpDispatch!=NULL)
			 {
				 lpDispatch->QueryInterface(IID_IOleInPlaceActiveObject, (LPVOID*)&lpOleInPlaceActiveObj);
				 ASSERT(lpOleInPlaceActiveObj);

			 }

			 // Pass the keydown command to IOleInPlaceActiveObject::TranslateAccelerator 
			 if(lpOleInPlaceActiveObj != NULL)
			 {
				 hrTranslate = lpOleInPlaceActiveObj->TranslateAccelerator(pMsg);
			 }

			 }
			 catch(...)
			 {//?
			 }

			 try{
				 if(lpDispatch)
					lpDispatch->Release();
				 if(lpOleInPlaceActiveObj)
					lpOleInPlaceActiveObj->Release();
			 }catch(...)
			 {
			 }

			 if (hrTranslate == S_OK)
				return TRUE;
		 }

      }
   }

   try
   {
	m_wndMenuBar.TranslateFrameMessage(pMsg);

	//2.5 not scroll address bar
	if(pMsg->message == WM_MOUSEWHEEL)
	{
		hAddr = m_wndAddress->GetEditCtrl()->GetSafeHwnd();
		if(hAddr==pMsg->hwnd)
		{
			PostMessage(WM_MOUSEWHEEL, pMsg->wParam, pMsg->lParam);
			return TRUE;
		}
	}
	return CMDIFrameWnd::PreTranslateMessage(pMsg);

   }
	catch(...)
	{
		return FALSE;
	}
}



void CMainFrame::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	if(lpMeasureItemStruct->CtlType==ODT_MENU)
	{
	//	if(IsMenu((HMENU)lpMeasureItemStruct->itemID))
		{
			gmenu.MeasureItem(lpMeasureItemStruct);
		}
	}


	CMDIFrameWnd::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

BOOL CMainFrame::DecodeEscap(CString &strUrl)
{
//11/7 save time

	CString strUrl2 = strUrl;
	strUrl2.MakeLower();
	int n = strUrl2.GetLength();
	if(n==0 || strUrl.Find('%')<0)
		return FALSE;
	char c = strUrl.GetAt(0), ch1,ch2;
	int i=0;
	CString strTemp;
	while(c!='#' && c!='?' && i<n)
	{
		if(c!='%' || i+2>=n)
			strTemp+=c;
		else
		{
			ch1 = strUrl2.GetAt(i+1);
			ch2 = strUrl2.GetAt(i+2);
		//ch1!='2' && ch2!='0' && 
			if(((ch1>='0' && ch1<='9')||(ch1>='a' && ch1<='f')) && ((ch2>='0' && ch2<='9')||(ch2>='a' && ch2<='f')))
			{
				if(ch1>='0' && ch1<='9')
					c= (ch1-'0')*16;
				else
					c= (ch1-'a'+10)*16;
				if(ch2>='0' && ch2<='9')
					c+= ch2-'0';
				else
					c+= ch2-'a'+10;
				i+=2;
				strTemp+=c;
			}
			else
				strTemp+=c;
		}
		i++;
		c = strUrl.GetAt(i);
	}

	for(int l=i; l<n; l++)
		strTemp+= strUrl.GetAt(l);

	strUrl = strTemp;

	return TRUE;
}



void CMainFrame::OnUpdateToolsIp(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bDisplayIP);
}

CMenu* CMainFrame::GetMenu()
{
	return m_wndMenuBar.GetMenu();
}

void CMainFrame::OnWindowRestore() 
{
	// TODO: Add your command handler code here
	if(m_nProtectNum>0)
	{
		AfxMessageBox(IDS_CANT_DO);
		return;
	}
	m_bMax = 3;
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	
	if (tcf!=NULL)
	{
		MDIRestore(tcf);
	}
}



void CMainFrame::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	try{
	if(lpDrawItemStruct->CtlType==ODT_MENU)
	{
	//	if(IsMenu((HMENU)lpMeasureItemStruct->itemID))
		{
			gmenu.DrawItem(lpDrawItemStruct);
		}
	}
	}
/*	catch(CMemoryException * e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}
	
	CMDIFrameWnd::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CMainFrame::OnWindowMin() 
{
	// TODO: Add your command handler code here
	if(m_nProtectNum>0)
	{
		AfxMessageBox(IDS_CANT_DO);
		return;
	}
	m_bMax = 3;
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	
	if (tcf!=NULL)
	{
		tcf->ShowWindow(SW_MINIMIZE);
	}

}


void CMainFrame::OnUpdateToolsSetbackgroundpic(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	try{
//	static CString psz;
	static char cur[MAX_PATH];
	static char tmp[5]; 
//	static CString strMenu;

	if(m_strBGPic == "" || m_strBGPic.IsEmpty())
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);

	//load bmp names
	bmppath = ((CMyIEApp*)AfxGetApp())->m_strBitmapPath;
	cur[0]=0;
	GetCurrentDirectory(MAX_PATH, cur);
	if(!SetCurrentDirectory(bmppath))
		CreateDirectory(bmppath, NULL);
	else
		SetCurrentDirectory(cur);

	m_astrBGPics.RemoveAll();
	WIN32_FIND_DATA wfd;
	HANDLE          h;
	bmppath += "*.bmp";
	CMenu* pmenu = GetMenu()->GetSubMenu(6)->GetSubMenu(17);
	if(pmenu == NULL)
		return;

	while(pmenu->DeleteMenu(1, MF_BYPOSITION));

	int i=0;
	h = FindFirstFile(bmppath, &wfd);
	m_strBGPic.MakeLower();
	if(h != INVALID_HANDLE_VALUE)
	{
		do{
			itoa(i+1, tmp, 10);
			gString1 = tmp;
			gString1 += ". "; 
			gString1 += wfd.cFileName;
			gString1 = gString1.Left(gString1.GetLength()-4);
			pmenu->AppendMenu( MF_ENABLED, 0xe00 + i , gString1);
			m_astrBGPics.AddTail(wfd.cFileName);
			gString1 = wfd.cFileName; gString1.MakeLower();
			if(gString1 == m_strBGPic)
				pmenu->CheckMenuItem(0xe00 + i, MF_CHECKED);
			i++;
		}
		while(FindNextFile(h, &wfd) && i<511);
	} 

	FindClose(h);
	AddMenuBreak(pmenu, FALSE);

	}/*catch(CMemoryException * e)
	{
		if(e!=NULL)e->Delete();
//		AfxMessageBox("update bk error");
	}*/
	catch(...)
	{
	}

}

void CMainFrame::InitialToolBar(BOOL bReplaceIcon)
{
	// Create and set the normal toolbar image list.
	CImageList img, *oldimg;
//	CBitmap    bitmap;
	HBITMAP hbmp;
	oldimg = m_wndToolBar.GetToolBarCtrl().GetHotImageList();
	if(oldimg!=NULL && bReplaceIcon)
		oldimg->DeleteImageList();

	if(oldimg == NULL || bReplaceIcon)
	{
		if(!m_bSmallToolBar)
		{
			//bitmap.LoadBitmap(IDB_HOTTOOLBAR);
			hbmp = (HBITMAP)LoadImage(NULL, ((CMyIEApp*)AfxGetApp())->m_strMyIERoot+"\\hottoolb.bmp", IMAGE_BITMAP, 0, 0, 
					LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE); 
//			if(hbmp==NULL)
//474				hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_HOTTOOLBAR));
			img.Create(21, 21, ILC_COLORDDB|ILC_MASK, 40, 1);
		}
		else
		{
			//bitmap.LoadBitmap(IDB_HOTSM);
			hbmp = (HBITMAP)LoadImage(NULL, ((CMyIEApp*)AfxGetApp())->m_strMyIERoot+"\\hottoolbsm.bmp", IMAGE_BITMAP, 0, 0, 
					LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE); 
//			if(hbmp==NULL)
//474				hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_HOTSM));
			img.Create(16, 16, ILC_COLORDDB|ILC_MASK, 40, 1);
		}

		ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
		m_wndToolBar.SendMessage(TB_SETHOTIMAGELIST, 0, (LPARAM)img.m_hImageList);
		img.Detach();
		//bitmap.Detach();
	}
	
	oldimg = m_wndToolBar.GetToolBarCtrl().GetImageList();
	if(oldimg!=NULL && bReplaceIcon)
		oldimg->DeleteImageList();

	if(oldimg == NULL || bReplaceIcon)
	{
		if(!m_bSmallToolBar)
		{
			if(m_bColorIcon)
			{	//bitmap.LoadBitmap(IDB_HOTTOOLBAR);
				hbmp = (HBITMAP)LoadImage(NULL, ((CMyIEApp*)AfxGetApp())->m_strMyIERoot+"\\hottoolb.bmp", IMAGE_BITMAP, 0, 0, 
						LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE); 
//				if(hbmp==NULL)
//474					hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_HOTTOOLBAR));
			}
			else
			{
				//bitmap.LoadBitmap(IDB_COLDTOOLBAR);
				hbmp = (HBITMAP)LoadImage(NULL, ((CMyIEApp*)AfxGetApp())->m_strMyIERoot+"\\coldtool.bmp", IMAGE_BITMAP, 0, 0, 
						LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE); 
//				if(hbmp == NULL)
//474					hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_COLDTOOLBAR));
			}
			img.Create(21, 21, ILC_COLORDDB|ILC_MASK, 40, 1);
		}
		else
		{
			if(m_bColorIcon)
			{
				//bitmap.LoadBitmap(IDB_HOTSM);
				hbmp = (HBITMAP)LoadImage(NULL, ((CMyIEApp*)AfxGetApp())->m_strMyIERoot+"\\hottoolbsm.bmp", IMAGE_BITMAP, 0, 0, 
						LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE); 
//				if(hbmp==NULL)
//474					hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_HOTSM));
			}
			else
			{
				//bitmap.LoadBitmap(IDB_COLDSM);
				hbmp = (HBITMAP)LoadImage(NULL, ((CMyIEApp*)AfxGetApp())->m_strMyIERoot+"\\coldtoolsm.bmp", IMAGE_BITMAP, 0, 0, 
						LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE); 
//				if(hbmp == NULL)
//474					hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_COLDSM));
			}
			img.Create(16, 16, ILC_COLORDDB|ILC_MASK, 40, 1);
		}

		ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
		m_wndToolBar.SendMessage(TB_SETIMAGELIST, 0, (LPARAM)img.m_hImageList);
		img.Detach();
		//bitmap.Detach();
	}

	//remove all buttons
	int b = m_wndToolBar.GetToolBarCtrl().GetButtonCount();
	int bandid = m_wndReBar.GetReBarCtrl().IDToIndex(ID_VIEW_TOOLBAR);
	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_SIZE|RBBIM_STYLE;
	m_wndReBar.GetReBarCtrl().GetBandInfo(bandid, &rbbi);
	rbbi.fStyle &= ~RBBS_NOGRIPPER;

	for(int i = 0; i<b;i++)
		m_wndToolBar.GetToolBarCtrl().DeleteButton(0);
	CRect rectToolBar;

	if(m_nTextLabel<2)
		m_wndToolBar.ModifyStyle(TBSTYLE_LIST , TBSTYLE_FLAT | TBSTYLE_TRANSPARENT );
	else
		m_wndToolBar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT| TBSTYLE_LIST );

//	m_wndToolBar.SetButtons(NULL, 17);

	// set up toolbar button sizes
//	m_wndToolBar.GetItemRect(0, &rectToolBar);
//	m_wndToolBar.SetSizes(rectToolBar.Size(), CSize(22,20));
	// set up each toolbar button
	int nDefBtn = sizeof(toolbarbuttons)/sizeof(UINT), nMaxBtn = nDefBtn;
	int ids[50], l=0, id, buttonum;
	static char tmp[6]; tmp[0]='B'; 
	CString str;

	id = (int)AfxGetApp()->GetProfileInt("ToolBar", "B0", -5);
	if(id == -5)
	{
		//no toolbar configuration
		buttonum = nDefBtn;
	}
	else
		buttonum = 50;
	id=0;l=0;
	while(l<buttonum && id!=-1)
	{
		itoa(l,tmp+1,10);
		id = (int)AfxGetApp()->GetProfileInt("ToolBar", tmp, -5);
		if(id==-5)
		{
			ids[l] = l;
			buttonum=nDefBtn;
		}
		else
			ids[l]=id;

		l++;
	}
//	m_wndToolBar.SetTextMode(m_bTextLabel);
	int n=0;
	str.Empty();
	//2.5 victor
	m_wndToolBar.SetButtons(NULL, l-1);
	for(i=0; i<l; i++)
	{
		itoa(i,tmp+1,10);
		id = ids[i];
		if(id>=0 && id<nMaxBtn)
		{
			if(m_nTextLabel == 2 && toolbarbuttonstyles[id]!=TBBS_SEPARATOR)
				m_wndToolBar.SetButtonInfo(n,toolbarbuttons[id],toolbarbuttonstyles[id]| TBSTYLE_AUTOSIZE,toolbarbuttonimgs[id]);
			else
				m_wndToolBar.SetButtonInfo(n,toolbarbuttons[id],toolbarbuttonstyles[id],toolbarbuttonimgs[id]);

			if(m_nTextLabel==1 || (m_nTextLabel==2 && toolbarselectivelabels[id]==1))
			{
				str.LoadString(toolbarlabels[id]);
				m_wndToolBar.SetButtonText(n, str.Right(str.GetLength()-str.Find('\n')-1));
			}
			else
				m_wndToolBar.SetButtonText(n, str);

			n++;
			str.Empty();
		}
		else if(id>=nMaxBtn)
			continue;
		else
			break;
	}

	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		 CBRS_FLYBY | CBRS_SIZE_DYNAMIC);//CBRS_TOOLTIPS |
	if(m_nTextLabel!=1)
		if(m_bSmallToolBar)
			m_wndToolBar.GetToolBarCtrl().SetButtonSize(CSize(20,18));
		else
			m_wndToolBar.GetToolBarCtrl().SetButtonSize(CSize(24,22));
	else if(!m_bSmallToolBar)
	{
		if(m_sTBTextSizeSM.cx != 0 && m_sTBTextSize.cx == 0)
		{
			m_sTBTextSize.cx = m_sTBTextSizeSM.cx;
			m_sTBTextSize.cy = m_sTBTextSizeSM.cy+4;
		}
		if(m_sTBTextSize.cx != 0)
			m_wndToolBar.GetToolBarCtrl().SetButtonSize(m_sTBTextSize);
	}
	else
	{
		if(m_sTBTextSize.cx != 0 && m_sTBTextSizeSM.cx == 0)
		{
			m_sTBTextSizeSM.cx = m_sTBTextSize.cx;
			m_sTBTextSizeSM.cy = m_sTBTextSize.cy-4;
		}
		if(m_sTBTextSizeSM.cx != 0)
			m_wndToolBar.GetToolBarCtrl().SetButtonSize(m_sTBTextSizeSM);
	}


	if(m_nTextLabel==1)
	{
		if(m_bSmallToolBar)
			m_sTBTextSizeSM = m_wndToolBar.GetToolBarCtrl().GetButtonSize();
		else
			m_sTBTextSize = m_wndToolBar.GetToolBarCtrl().GetButtonSize();
	}

	m_wndToolBar.GetItemRect(0, &rectToolBar);
	

	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE |RBBIM_ID |RBBIM_STYLE;
	rbbi.wID = ID_VIEW_TOOLBAR;
	rbbi.cxMinChild = rectToolBar.Width()+2;
	if(rectToolBar.Height()<18)
		rbbi.cyMinChild = 18;
	else
		rbbi.cyMinChild = rectToolBar.Height();
			
	int bc=m_wndToolBar.GetToolBarCtrl().GetButtonCount();
	if(bc>0)
	{
		m_wndToolBar.GetToolBarCtrl().GetItemRect(bc-1,&rectToolBar);
		/*rbbi.cx =*/ rbbi.cxIdeal = rectToolBar.right;
	}
	else
		rbbi.cx = rbbi.cxIdeal = 2;

	m_wndReBar.GetReBarCtrl().SetBandInfo(bandid, &rbbi);

	if(m_bLockToolbar)
	{
		rbbi.fStyle |= RBBS_NOGRIPPER;
		m_wndReBar.GetReBarCtrl().SetBandInfo(bandid, &rbbi);
	}
}

void CMainFrame::OnUpdateToolbar(WPARAM wParam,LPARAM)
{
	try{
	InitialToolBar();
	}/*catch(CMemoryException * e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}

}


void CMainFrame::OnFileNewmyie() 
{
	// TODO: Add your command handler code here
    HINSTANCE hInstance = AfxGetResourceHandle();
    ASSERT(hInstance != NULL);
    LPTSTR lpszModule = new TCHAR[_MAX_PATH];
    if (GetModuleFileName(hInstance, lpszModule, _MAX_PATH))
    {
		if((UINT)ShellExecute(NULL, "open", lpszModule , "", ".", SW_SHOW )<=32)
			AfxMessageBox(IDS_UTIL_FAIL);
    }
    delete [] lpszModule;

}

void CMainFrame::OnFileOpen() 
{
	// TODO: Add your command handler code here
	CWebFileOpenDlg fileDlg;

	if(fileDlg.DoModal() == IDOK && fileDlg.m_strAddress!="")
	{
		CChildFrame* tcf = (CChildFrame*)MDIGetActive();

		//create new if not exist
		if(tcf==NULL || bAddressInNewWindow)
			tcf = NewChildWindow(0);

		if (tcf!=NULL && tcf->m_pView!=NULL)
		{
			tcf->m_pView->m_lpszUrl=fileDlg.m_strAddress;
			tcf->m_pView->Navigate2(fileDlg.m_strAddress, 0, NULL);
		}
	}
}

void CMainFrame::OnViewToolbartextlabel() 
{
	// TODO: Add your command handler code here
	if(m_nTextLabel==1)
		m_nTextLabel = 0;
	else
		m_nTextLabel = 1;
	InitialToolBar();
}

void CMainFrame::OnUpdateViewToolbartextlabel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_nTextLabel==1);
}

void CMainFrame::OnToolsOpenstart() 
{
	// TODO: Add your command handler code here
	try{
	if(m_strStartFavFolder != "" && !m_strStartFavFolder.IsEmpty())
		OpenAllFavs(m_strStartFavFolder+"\\");
	}
	catch(...)
	{
	}
}

void CMainFrame::OnChangeProxy(UINT nID)
{
	m_bNeedUpdProxy = TRUE;

	INTERNET_PROXY_INFO ipi;
	DWORD dwSize = sizeof(ipi);
	int i = nID- 0x6320;
//	POSITION pos = m_astrProxy.FindIndex(i);
	CString old = m_strCurProxy;
	CProxyList* pl;
	int index;
	CProxyItem* pi = m_aProxyCategories.FindIndex(i, &pl, &index);
	if(pi->index == i)
	{
		m_strCurProxy = pi->m_strProxy;
		m_strCurProxyName = pi->m_strProxyName;
		m_strProxyByPass = pl->m_strProxyByPass;
		m_nCurProxyList = index;
	}
	else
		return;

	InternetQueryOption(NULL,INTERNET_OPTION_PROXY,&ipi,&dwSize);
	if (ipi.dwAccessType == INTERNET_OPEN_TYPE_DIRECT )
		OnOptionsUseProxy(); //set proxy
	else
	{	DWORD dwSize = sizeof(ipi);
		ipi.dwAccessType = INTERNET_OPEN_TYPE_DIRECT ;
		InternetSetOption(NULL, INTERNET_OPTION_PROXY, &ipi, dwSize);
		if(old != m_strCurProxy)
			OnOptionsUseProxy();
	}
}

void CMainFrame::OnChangeWebProxy(UINT nID)
{
	int i; 
//	POSITION pos = m_astrWebProxy.FindIndex(i);

	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	CProxyList* pl;
	if(nID >= 0x1200)
	{
		pl = m_aProxyCategories.GetWebProxyList();
		i = nID - 0x1200;
	}
	else
	{
		pl = m_aProxyCategories.GetWebProxyList(TRUE);
		i= nID - 0x1100;
	}

	if (tcf!=NULL && tcf->m_pView!=NULL && pl!=NULL)
	{
		if(((CMyIEView*)tcf->m_pView)->m_bEnWebProxy && ((CMyIEView*)tcf->m_pView)->m_strWebProxy == pl->m_Proxies.GetAt(i)->m_strProxy)
			((CMyIEView*)tcf->m_pView)->m_bEnWebProxy = FALSE;
		else
		{
			((CMyIEView*)tcf->m_pView)->m_bEnWebProxy = TRUE;
			((CMyIEView*)tcf->m_pView)->m_strWebProxy = pl->m_Proxies.GetAt(i)->m_strProxy;
			m_strDefaultWebProxy = pl->m_Proxies.GetAt(i)->m_strProxy;
			if(m_dwWebProxyFrom == ID_OPTIONS_USE_WEBP)
			{
				//auto refresh
				if(!((CMyIEView*)tcf->m_pView)->m_lpszUrl.IsEmpty() && ((CMyIEView*)tcf->m_pView)->m_lpszUrl!="about:blank")
				{
					((CMyIEView*)tcf->m_pView)->ToNavigate(((CMyIEView*)tcf->m_pView)->m_lpszUrl, 0, NULL, NULL, NULL, 0, TRUE);
				}
			}
			else if(m_dwWebProxyFrom == IDOK)
			{
				//start new address
				OnNewAddressEnter();				
				((CMyIEView*)tcf->m_pView)->m_bEnWebProxy = FALSE;
			}
		}

	}
}

/*void CMainFrame::OnUpdateOptionMultiTab(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//2.2	try{
	pCmdUI->SetCheck(m_bMultiLineTab);
//	}catch(...)
//	{
//	}

}*/

void CMainFrame::OnOptionAutotab() 
{
	// TODO: Add your command handler code here
	if(m_bAutoTab)
	{
		if(m_bMultiLineTab)
			m_wndTab.ModifyStyle(TCS_FIXEDWIDTH, TCS_MULTILINE);
		else
			m_wndTab.ModifyStyle(TCS_FIXEDWIDTH|TCS_MULTILINE, 0);
		m_bAutoTab = FALSE;
	}
	else
	{
		m_wndTab.ModifyStyle(/*TCS_MULTILINE*/ 0, TCS_FIXEDWIDTH);
		m_bAutoTab = TRUE;
	}
	PostMessage(WM_UPDATE_TAB,TRUE);
}

/*void CMainFrame::OnUpdateOptionAutotab(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//2.2	try{
	pCmdUI->SetCheck(m_bAutoTab);
//	}catch(...)
//	{
//	}

}*/

int CMainFrame::AdjustTabWidth(BOOL bNoReCalSize)
{
	CRect rect;
	int x, y, oldx;

	try{
	
/*	if(!m_bFBTab)
		y = 22;
	else
		y = 19;
	if(!bNoReCalSize)*/
	{
		m_wndTab.GetItemRect(0, &rect);
		y = rect.Height();
		oldx=rect.Width();
		
		if(y==0)
			y=19; //21;
	}

	if(m_bAutoTab)
	{
		m_wndTab.GetWindowRect(&rect);
		if(!bNoReCalSize)
		{
			int c = m_wndTab.GetItemCount();
			if(c==0)
				x = m_nMaxTabWidth;
			else
				if(m_bFBTab)
					x = rect.Width()/c-11;
				else
				{
					x = (rect.Width()-5)/c;
				}
			if(x>m_nMaxTabWidth)
				x=m_nMaxTabWidth;
			else if(x<m_nMinTabWidth)
				x=m_nMinTabWidth;

			if(x != oldx)
			{
				CSize size(x,y);
				m_wndTab.SetItemSize(size);
			}
		}
		if(m_bFBTab)
		{
			try{
				m_wndTab.SetWindowPos(&wndTop, 0,0, rect.Width()+1, rect.Height(), SWP_NOMOVE|SWP_NOOWNERZORDER|SWP_NOZORDER|SWP_NOREDRAW|SWP_NOSENDCHANGING|SWP_NOACTIVATE);
				m_wndTab.SetWindowPos(&wndTop, 0,0, rect.Width(), rect.Height(), SWP_NOMOVE|SWP_NOOWNERZORDER|SWP_NOZORDER|SWP_NOREDRAW|SWP_NOSENDCHANGING|SWP_NOACTIVATE);
			}catch(...)
			{
			}
		}
	}

	}/*catch(CMemoryException * e)
	{
//		AfxMessageBox("adjust error");
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}

	return y;
}

void CMainFrame::OnShortcutMfl() 
{
	// TODO: Add your command handler code here
	try{

	OpenAllFavs(m_strLinkFolder+"\\", TRUE);
	}catch(...)
	{
	}
}

void CMainFrame::OnMostFav(UINT nID)
{
	try{

	int i = nID-0x1280;
	POSITION pos = m_astrFavoriteFolders.FindIndex(i);
	if(pos!=NULL)
		OpenAllFavs(m_astrFavoriteFolders.GetAt(pos)+"\\", TRUE);

	}catch(...)
	{
	}
}


void CMainFrame::OnAddThisFav(UINT nID)
{
	try{

	CMyIEView * pvw = NULL;
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	if (tcf!=NULL)
		pvw = (CMyIEView*)tcf->m_pView;
	if(pvw == NULL)
		return;

	int i = nID-0x6220;
	POSITION pos = m_astrFavoriteFolders.FindIndex(i);
	if(pos!=NULL)
	{
		CString folder = m_astrFavoriteFolders.GetAt(pos);
		if(folder.Right(1)!="\\")
			folder += "\\";
		CString title, url, root;
		tcf->GetWindowText(title);
		pvw->GetLocationURL(url);
		CAddToThisFavDlg dlg;
		dlg.m_strFolder = folder;
		GetFavoriteFolder(root);
		dlg.m_strFolder = dlg.m_strFolder.Mid(root.GetLength());
		title.Replace('\\', '-');
		title.Replace('/', '-');
		title.Replace(':', '_');
		dlg.m_strName = title;
		dlg.m_strURL = url;
		if(dlg.DoModal() == IDOK)
		{
			title = dlg.m_strName; url = dlg.m_strURL;
			title.Replace('\\', '-');
			title.Replace('/', '-');
			title.Replace(':', '_');
			WritePrivateProfileString(_T("InternetShortcut"), _T("URL"), url, folder+title+".url");
		    m_bFavDirty = TRUE;
		}
	}

	}catch(...)
	{
	}
}

void CMainFrame::OnF2(UINT nID) 
{
	// TODO: Add your command handler code here
	if(m_bUseSC)
	{
		short ks=GetKeyState(VK_CONTROL);

		CString url = m_strShortCuts[nID-ID_F_2];
		if(url=="" || url.IsEmpty())
		{
			if(nID == ID_F_2)
				OnTabPre();
			else if(nID == ID_F_3)
				OnTabNext();
			else if(nID == ID_F_4)
				OnAddrDrop();
			else if(nID == ID_F_5)
				SendMessage(WM_COMMAND, ID_VIEW_REFRESH);
			else if(nID == ID_F_11)
				OnViewFullscreen();
			return;
		}

		CChildFrame* tcf = (CChildFrame*)MDIGetActive();
		if (tcf!=NULL && tcf->m_pView!=NULL && !bAddressInNewWindow && ks>=0)
			((CMyIEView*)tcf->m_pView)->ToNavigate(url, 0, NULL);
		else
		{
			tcf = NewChildWindow(0);
			((CMyIEView*)tcf->m_pView)->ToNavigate(url, 0, NULL);
		}

	}
	else
	{
		if(nID == ID_F_2)
			OnTabPre();
		else if(nID == ID_F_3)
			OnTabNext();
		else if(nID == ID_F_4)
			OnAddrDrop();
		else if(nID == ID_F_5)
			SendMessage(WM_COMMAND, ID_VIEW_REFRESH);
		else if(nID == ID_F_11)
			OnViewFullscreen();
	}
}

void CMainFrame::OnShortcutUseSc() 
{
	// TODO: Add your command handler code here
	m_bUseSC = !m_bUseSC;
}

void CMainFrame::OnUpdateShortcutUseSc(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bUseSC);
}



void CMainFrame::OnShortcutAlias() 
{
	// TODO: Add your command handler code here
	m_bUseAlias = !m_bUseAlias;
}

void CMainFrame::OnUpdateShortcutAlias(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bUseAlias);
}





void CMainFrame::OnUpdateBP(CCmdUI* pCmdUI) 
{
//	static CString bp;

	if(glo_fInitializing)
		return;

	float interval = 1.0;
	if(m_bNoModemSpeed)
	{
		interval = 5.0;
	}

	try{

	LARGE_INTEGER liFinish;
    QueryPerformanceCounter(&liFinish);
    LARGE_INTEGER liFreq;
    QueryPerformanceFrequency(&liFreq);
	float secs = (float)(liFinish.LowPart - m_tPreBytes.LowPart)/(float)liFreq.LowPart;
	if(secs >= interval)
	{
		if(!m_DispBP)
		{	
			pCmdUI->SetText("");
			m_bNoModemSpeed = TRUE;
			return;
		}

		if(!glo_fInitialized)
		{
			AfxBeginThread(TInitialMonitor, NULL);
			pCmdUI->SetText("");
			return;
		}

		bp ="";
		long bytes; 
		GetRASSpeed((DWORD*)&bytes);
		if(bytes == -999)
			m_bNoModemSpeed = TRUE;
		else
			m_bNoModemSpeed = FALSE;

		m_tPreBytes = liFinish;
		//get ras BPs
		float bps = ((float)(bytes - m_lPreBytes))/secs;
		if(bytes>=0)
			m_lPreBytes = bytes;
		

		//just for test
		//bps = bytes;

		if(bps>1024)
		{
			bps = bps/1024;
			if(bps>=10)
				bp.Format("%4.1fK", bps);
			else
				bp.Format("%4.2fK", bps);
		}
		else if(bps>0)
			bp.Format("%4.0f", bps);

		pCmdUI->SetText(bp);
	}

	}/*catch(CMemoryException * e)
	{
//		AfxMessageBox("update bp error");
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}
}

#define GFSR_SYSTEMRESOURCES	0
#define GFSR_GDIRESOURCES		1
#define GFSR_USERRESOURCES		2


MEMORYSTATUS  ms;

void CMainFrame::OnUpdateRES(CCmdUI* pCmdUI) 
{
	if(!m_bDispMem)
	{
		pCmdUI->SetText("");
		return;
	}

/*	if(!m_bWin9x)
	{
		pCmdUI->SetText("");
		return;
	}
*/
//	static CString res;
	CTime t;
	CTimeSpan ts;

	try
	{
	res = "";
	t = CTime::GetCurrentTime();
	ts = t - m_tLastRes;  
	int mini = 5;
	if(ts.GetTotalSeconds()>= mini)//m_bWin9x && 
	{
		m_tLastRes = t;

		if (hResInst != NULL && pGetRes != NULL)
		{

		//	if (pGetRes)
			{
				long lSysRes = (*pGetRes)(GFSR_SYSTEMRESOURCES);
			//	long lGdiRes = (*pGetRes)(GFSR_GDIRESOURCES);
			//	long lUsrRes = (*pGetRes)(GFSR_USERRESOURCES);
				res.Format("%d%%", lSysRes);
				pCmdUI->SetText(res);
				return;
			}

			//m_bWin9x = FALSE;
		//	pCmdUI->SetText("");
		//	return;
		}
		else
		{
			GlobalMemoryStatus(&ms);
			res.Format("%dM", ms.dwAvailPhys/1024/1024);
			pCmdUI->SetText(res);
			return;
		}
		//m_bWin9x = FALSE;
	//	pCmdUI->SetText("");

	}
	}
/*	catch(CMemoryException * e)
	{
//		AfxMessageBox("update res error");
		if(e!=NULL)e->Delete();
//		m_bWin9x = TRUE;
	}*/
	catch(...)
	{
	}
}

//2.4
void CMainFrame::OnUpdateSecu(CCmdUI* pCmdUI) 
{
	if(m_bLastIsSecure != m_bIsSecure)
	{
		if(m_bIsSecure)
			m_wndStatusBar.GetStatusBarCtrl().SetIcon(3, m_hSecuIcon);
		else
			m_wndStatusBar.GetStatusBarCtrl().SetIcon(3, NULL);

		m_bLastIsSecure = m_bIsSecure;
	}
}

void CMainFrame::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	CMDIFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	static CString			strOpenAll, strOpenMost, strAddFav;
	
	// TODO: Add your message handler code here
	try{
	if(pPopupMenu == NULL)
		return;

		MENUITEMINFO mii; 

		mii.fMask = MIIM_DATA|MIIM_TYPE;    // get the state of the menu item
	    mii.dwTypeData = NULL;
		mii.cch = 0;
		mii.dwItemData = NULL;
		mii.cbSize = sizeof(mii);
		pPopupMenu->GetMenuItemInfo(0, &mii, TRUE); 
		CMenuData* pmd = (CMenuData*)(mii.dwItemData);
		if(mii.fType & MFT_OWNERDRAW && pmd!=NULL)
		{
			//get the ini flag
			if(!pmd->m_bIni)
			{
				//get path
			//	if(pmd->m_szMenuFolder!=NULL)
				if(pmd->m_szMenuText!=NULL)
				{
					int nFavs, nMostFavs, fs;
					pPopupMenu->DeleteMenu(0, TRUE);
					m_nFavSize = BuildFavoritesMenu(pmd->m_szMenuText, m_nFavSize, pPopupMenu, 0, FALSE, nFavs, nMostFavs);

					//add open all
					if(m_bDisplayOpenAll || m_bDisplayAddFav)
					{
						if(nFavs || nMostFavs || m_bDisplayAddFav)
							pPopupMenu->InsertMenu(0,MF_BYPOSITION | MF_SEPARATOR);
						fs=m_astrFavoriteFolders.GetCount();
						m_astrFavoriteFolders.AddTail(pmd->m_szMenuText);
					}	
					if(m_bDisplayOpenAll)
					{
						strOpenAll.LoadString(IDS_OPEN_ALL_FAV);
						strOpenMost.LoadString(IDS_OPEN_ALL_MOSTFAV);

						if(nMostFavs)
							pPopupMenu->InsertMenu(0,MF_BYPOSITION| MF_STRING ,0x1280+fs, strOpenMost);
						if(nFavs)
							pPopupMenu->InsertMenu(0,MF_BYPOSITION| MF_STRING ,0x1000+fs, strOpenAll);
					}
					if(m_bDisplayAddFav)
					{
						//add fav
						strAddFav.LoadString(IDS_ADDFAV);

						pPopupMenu->InsertMenu(0,MF_BYPOSITION| MF_STRING ,0x6220+fs, strAddFav);
					}

					delete pmd;
					pmd=NULL;
				}
			}
		}


	}/*catch(CMemoryException * e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}
}



void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default

	try{

	if(nID==SC_MINIMIZE && bSysTray)
	{
		CMDIFrameWnd::OnSysCommand(nID, lParam);

		NOTIFYICONDATA nid;
		nid.cbSize=sizeof(nid);
		nid.hWnd=m_hWnd;
		nid.uID=IDR_MAINFRAME + nTrayIconID;
		nid.uFlags=NIF_ICON|NIF_TIP|NIF_MESSAGE;
		nid.hIcon=LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_TRAY));
		nid.uCallbackMessage=WSM_TRAY;
		strcpy(nid.szTip,"MyIE");
		Shell_NotifyIcon(NIM_ADD, &nid);
		ShowWindow(SW_HIDE);

	}
	else if(nID == SC_RESTORE)
	{
		if(IsIconic())
		{
			BOOL bMax;
			CChildFrame* tcf = (CChildFrame*)MDIGetActive(&bMax);
			if(tcf!=NULL && tcf->bProtect)
			{
				CInputPassword dlg;
				BOOL validate = FALSE;
				if(dlg.DoModal()==IDOK)
				{
					if(dlg.m_strPassword == tcf->m_strPsw)
						validate = TRUE;
					else
						AfxMessageBox(IDS_PASSWORD_ERROR);
				}
				if(!validate)
				{
					return;
				}
			}
		}
		CMDIFrameWnd::OnSysCommand(nID, lParam);
	}
	else
		CMDIFrameWnd::OnSysCommand(nID, lParam);

	}catch(...)
	{
	}
}


void CMainFrame::OnNotifyTray(WPARAM wParam, LPARAM lParam)
{
	//AfxMessageBox("tray");
	if(lParam==WM_LBUTTONUP)
	{
		ShowWindow(SW_SHOW);
		SetForegroundWindow();
		BOOL bMax;
		CChildFrame* tcf = (CChildFrame*)MDIGetActive(&bMax);
		if(tcf!=NULL && tcf->bProtect)
		{
			CInputPassword dlg;
			BOOL validate = FALSE;
			if(dlg.DoModal()==IDOK)
			{
				if(dlg.m_strPassword == tcf->m_strPsw)
					validate = TRUE;
				else
					AfxMessageBox(IDS_PASSWORD_ERROR);
			}
			if(!validate)
			{
				ShowWindow(SW_HIDE);
				return;
			}
		}

		NOTIFYICONDATA nid;
		nid.cbSize=sizeof(nid);
		nid.hWnd=m_hWnd;
		nid.uID=IDR_MAINFRAME + nTrayIconID;
		nid.uFlags=NIF_ICON|NIF_TIP|NIF_MESSAGE;
		nid.uCallbackMessage=WSM_TRAY;
		Shell_NotifyIcon(NIM_DELETE, &nid);
		ShowWindow(SW_RESTORE);
	}
}


void CMainFrame::OnShortcutsUse3721()
{
	m_bUse3721 = !m_bUse3721;
}

void CMainFrame::OnUpdateShortcutsUse3721(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bUse3721);
}


/*void CMainFrame::OnOptionSeqDefault() 
{
	// TODO: Add your command handler code here
	m_nSeq = 0;

	m_bmUserDefault.m_bActiveNewWindow = m_bActiveNewWindow;
	m_bmUserDefault.m_bAlwaysNewWindow = bAddressInNewWindow;
	m_bmUserDefault.m_nSeq = m_nSeq;
	m_bmUserDefault.bLinkInNewWin = bLinkInNewWindow;
}

void CMainFrame::OnUpdateOptionSeqDefault(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//2.2	try{
	if(m_nSeq == 0)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
//	}catch(...)
//	{
//	}

}

void CMainFrame::OnOptionSeqNext() 
{
	// TODO: Add your command handler code here
	m_nSeq = 1;

	m_bmUserDefault.m_bActiveNewWindow = m_bActiveNewWindow;
	m_bmUserDefault.m_bAlwaysNewWindow = bAddressInNewWindow;
	m_bmUserDefault.m_nSeq = m_nSeq;
	m_bmUserDefault.bLinkInNewWin = bLinkInNewWindow;
}

void CMainFrame::OnUpdateOptionSeqNext(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//2.2	try{
	if(m_nSeq == 1)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
//	}catch(...)
//	{
//	}

}

void CMainFrame::OnOptionSeqPre() 
{
	// TODO: Add your command handler code here
	m_nSeq = 2;

	m_bmUserDefault.m_bActiveNewWindow = m_bActiveNewWindow;
	m_bmUserDefault.m_bAlwaysNewWindow = bAddressInNewWindow;
	m_bmUserDefault.m_nSeq = m_nSeq;
	m_bmUserDefault.bLinkInNewWin = bLinkInNewWindow;

}

void CMainFrame::OnUpdateOptionSeqPre(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//2.2	try{
	if(m_nSeq == 2)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
//	}catch(...)
//	{
//	}


}*/

void CMainFrame::OnTabPre() 
{
	// TODO: Add your command handler code here
	try{
	int nTabID = m_wndTab.GetCurSel();
	nTabID --;
	if(nTabID<0)
		nTabID = nWin - 1;
	if(nTabID>=0)
	{
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;

		m_wndTab.GetItem(nTabID, &TabCtrlItem);
		CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
		if(tcf!=NULL)
		{
			MDIActivate(tcf);
			m_wndTab.SetCurSel(nTabID);
		}
	}
	}/*catch(CMemoryException * e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}
}

void CMainFrame::OnTabNext() 
{
	// TODO: Add your command handler code here
	try{
	int nTabID = m_wndTab.GetCurSel();
	nTabID ++;
	if(nTabID>=0)
	{
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;
		TabCtrlItem.lParam = NULL;

		m_wndTab.GetItem(nTabID, &TabCtrlItem);
		CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
		if(tcf == NULL)
		{
			nTabID = 0;
			TabCtrlItem.mask = TCIF_PARAM;
			TabCtrlItem.lParam = NULL;

			m_wndTab.GetItem(nTabID, &TabCtrlItem);
			tcf = (CChildFrame*)TabCtrlItem.lParam;
		}

		if(tcf!=NULL)
		{
			MDIActivate(tcf);
			m_wndTab.SetCurSel(nTabID);
		}
	}
	}
/*	catch(CMemoryException * e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}
}


void CMainFrame::OnOptionsUrlinstatus() 
{
	// TODO: Add your command handler code here
	bURLOnly = !bURLOnly;
}

void CMainFrame::OnUpdateOptionsUrlinstatus(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//2.2	try{
	pCmdUI->SetCheck(bURLOnly);
//	}catch(...)
//	{
//	}

}


void CMainFrame::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu) 
{
	CMDIFrameWnd::OnMenuSelect(nItemID, nFlags, hSysMenu);
	
	// TODO: Add your message handler code here
	nLastItemID = nItemID;
	hLastMenu = hSysMenu;
	nLastFlag = nFlags;

//	static CString strTipText;
	strTipText="";

	//the same text as in the status bar is used.
	//Feel free to modify
	if(nLastItemID>=0x1380 && nLastItemID <= 0x6000)
	{
		if(nLastItemID-0x1380<m_nFavSize && hSysMenu!=NULL)
		{
		//	strTipText = m_astrFavoriteURLs[nLastItemID-0x1380];
			MENUITEMINFO mii; 

			mii.fMask = MIIM_DATA|MIIM_TYPE;    // get the state of the menu item
			mii.dwTypeData = NULL;
			mii.cch = 0;
			mii.dwItemData = NULL;
			mii.cbSize = sizeof(mii);
			::GetMenuItemInfo(hSysMenu, nItemID, FALSE, &mii); 
			CMenuData* pmd = (CMenuData*)(mii.dwItemData);
			if(mii.fType & MFT_OWNERDRAW && pmd!=NULL)
			{
				strTipText = pmd->m_szURL;
				favurl=strTipText;
			}
			if(favurl.IsEmpty())
			{
				CMenu * pMenu = GetMenu();

				MENUITEMINFO mii; 
				if(pMenu!=NULL)
				{
					mii.fMask = MIIM_DATA|MIIM_TYPE;    // get the state of the menu item
					mii.dwTypeData = NULL;
					mii.cch = 0;
					mii.dwItemData = NULL;
					mii.cbSize = sizeof(mii);
					pMenu->GetMenuItemInfo(nItemID, &mii, FALSE); 
					CMenuData* pmd = (CMenuData*)(mii.dwItemData);
					if(mii.fType & MFT_OWNERDRAW && pmd!=NULL)
					{
						strTipText = pmd->m_szURL;
						favurl=strTipText;
					}
				}
			}
		}

	/*	if(strTipText.Right(4) == _T(".lnk"))
		{
			TCHAR buf[INTERNET_MAX_PATH_LENGTH];
			if(ResolveLink(strTipText, buf))
			{
				strTipText = buf;
				m_astrFavoriteURLs[nLastItemID-0x1380] = strTipText;
			}
			else
				strTipText = "about:blank";
		}*/
	}
	else if(nLastItemID>=0x6200 && nLastItemID <= 0x620F)
	{
		strTipText = m_strUndoURL[nLastItemID - 0x6200];
	}
	SetMessageText(strTipText);

/*	if(MenuItemFromPoint(m_hWnd, hSysMenu,pt)>=0)
	{
		hMenu = hSysMenu;
		int mc = GetMenuItemCount(hSysMenu); 
		int ms = GetSystemMetrics(SM_CYMENU);

		int size = ms*mc;
		int scr = GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYMENU) - GetSystemMetrics(SM_CYCAPTION);
		if(size > scr)
			bLong = TRUE;
		else
			bLong = FALSE;
	}*/
}

void CMainFrame::OnSwitchWins(UINT nID)
{
	try{

	int n = nID-ID_C_1;

	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;

	m_wndTab.GetItem(n, &TabCtrlItem);
	CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
	if(tcf!=NULL)
	{
		MDIActivate(tcf);
		m_wndTab.SetCurSel(n);
	}
	}catch(...)
	{
	}
}


void CMainFrame::OnRtabStop() 
{
	// TODO: Add your command handler code here
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;

	m_wndTab.GetItem(nRTabID, &TabCtrlItem);
	CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
	
	if(tcf!=NULL && tcf->m_pView!=NULL)
	{
		tcf->m_pView->Stop();
		tcf->m_pView->m_nProgress = -10;
		if(tcf->m_pView->IsActive)
			percent = -1;

		//set icon
		int nTabID = nRTabID;
		int nIcon =0;
		if(!tcf->m_pView->IsActive)
			tcf->m_pView->oldIcon = SetTabIcon( 5, nTabID );
		else if(tcf->bLock)
			tcf->m_pView->oldIcon = SetTabIcon( 4, nTabID );
		else if(tcf->bProtect)
			tcf->m_pView->oldIcon = SetTabIcon( 6, nTabID );
		else
		{
			tcf->m_pView->oldIcon = SetTabIcon( -1, nTabID );
			nIcon = -1;
		}

		//update tab bar tab0
		if(m_bFBTab)
			PostMessage(WM_UPDATE_TAB);
	}
}

void CMainFrame::OnRtabRefresh() 
{
	// TODO: Add your command handler code here
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;

	m_wndTab.GetItem(nRTabID, &TabCtrlItem);
	CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
	
	if(tcf!=NULL && tcf->m_pView!=NULL)
	{
		tcf->m_pView->Refresh2(REFRESH_COMPLETELY);
	}
}



void CMainFrame::OnRtabProtect() 
{
	// TODO: Add your command handler code here
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;

	m_wndTab.GetItem(nRTabID, &TabCtrlItem);
	if(TabCtrlItem.lParam != NULL)
	{
		CInputPassword dlg;
		if(!((CChildFrame*)TabCtrlItem.lParam)->bProtect)
		{
			dlg.m_strPassword = m_strProtectPwd;
			dlg.m_bRemPsw = m_bRemPsw;
		
			if(dlg.DoModal()==IDOK && dlg.m_strPassword!="")
			{
				((CChildFrame*)TabCtrlItem.lParam)->m_strPsw = dlg.m_strPassword;
				((CChildFrame*)TabCtrlItem.lParam)->bProtect =TRUE;
				m_bRemPsw = dlg.m_bRemPsw;
				m_nProtectNum++;
				if(dlg.m_bRemPsw)
					m_strProtectPwd = dlg.m_strPassword;

				if(((CChildFrame*)TabCtrlItem.lParam)->m_pView->m_nProgress<0)
				{
					((CChildFrame*)TabCtrlItem.lParam)->m_pView->oldIcon = ((CMainFrame*)pMainFrame)->SetTabIcon( 6, nRTabID );

					//update tab bar
					if(m_bFBTab)
						PostMessage(WM_UPDATE_TAB);
				}
			}
		}
		else
		{
			if(dlg.DoModal()==IDOK)
			{
				if(((CChildFrame*)TabCtrlItem.lParam)->m_strPsw == dlg.m_strPassword)
				{
					((CChildFrame*)TabCtrlItem.lParam)->bProtect =FALSE;
					m_nProtectNum--;
					m_bRemPsw = dlg.m_bRemPsw;
					if(dlg.m_bRemPsw)
						m_strProtectPwd = dlg.m_strPassword;

					if(((CChildFrame*)TabCtrlItem.lParam)->m_pView->m_nProgress<0)
					{
						if(((CChildFrame*)TabCtrlItem.lParam)->bLock)
							((CChildFrame*)TabCtrlItem.lParam)->m_pView->oldIcon = ((CMainFrame*)pMainFrame)->SetTabIcon( 4, nRTabID );
						else
							((CChildFrame*)TabCtrlItem.lParam)->m_pView->oldIcon = ((CMainFrame*)pMainFrame)->SetTabIcon( -1, nRTabID );

						//update tab bar
						if(m_bFBTab)
							PostMessage(WM_UPDATE_TAB);
					}
				}
				else
					AfxMessageBox(IDS_PASSWORD_ERROR);
			}
		}
	}	
}

void CMainFrame::OnActivateWindow(WPARAM wParam, LPARAM lParam)
{
//	try{
	MDIActivate((CChildFrame*)lParam);
//	}
	/*catch(CMemoryException * e)
	{
		if(e!=NULL)e->Delete();
	}*/
//	catch(...)
//	{
//	}
}


void CMainFrame::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
// ??Drag&Drop?????
    WORD wNumFilesDropped = DragQueryFile(hDropInfo, -1, NULL, 0);

    CString firstFile="", tmp;
	char buf[2048];
    // 
    for (WORD x = 0 ; x < wNumFilesDropped; x++) {

        // ??PathName???,????????NULL,??????????????
        WORD wPathnameSize = DragQueryFile(hDropInfo, x, NULL, 0);

        // ??memory
        char * npszFile = (char *) LocalAlloc(LPTR, wPathnameSize += 1);

        // ???
        if (npszFile == NULL) continue;

        // ???????
        DragQueryFile(hDropInfo, x, npszFile, wPathnameSize);

        firstFile=npszFile;

		tmp = firstFile.Right(4);
		tmp.MakeLower();
		if(tmp == _T(".lnk"))
		{ // ???shortCut?????????????,expandedFile???????
           if(ResolveLink(firstFile, buf))
			  firstFile = buf;
		}
		else if(tmp == _T(".url"))
		{ // ???shortCut?????????????,expandedFile???????
 			::GetPrivateProfileString(_T("InternetShortcut"), _T("URL"),
									  _T(""), buf, 2048,
									  firstFile);
          firstFile = buf;
		}

        // ??memory
        LocalFree(npszFile);

		short ks = GetKeyState(VK_CONTROL);
		if(ks<0 || x>0)
			NewChildWindow(1,2, firstFile);
		else
		{
			 CChildFrame* tcf = (CChildFrame*)MDIGetActive();
			 if(tcf != NULL && tcf->m_pView != NULL)
			 {
				 tcf->m_pView->Navigate(firstFile);
				 tcf->m_pView->m_nProgress = 0;
			 }
			 else
				 NewChildWindow(1,2, firstFile);

		}
    }

    //
    DragFinish(hDropInfo);


	
}


void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	try{
//	static CString url;

	if(nIDEvent == m_nUpdateTimer)
	{
		// send WM_IDLEUPDATECMDUI to the main window
		if (m_hWnd != NULL && IsWindowVisible() && !IsIconic())
		{
				//2.4 move in
				if(m_wndInstantBar.GetSafeHwnd()!=NULL)
					if(m_wndInstantBar.IsVisible())
					{
						AfxCallWndProc(this, m_hWnd,
							WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0);
						m_wndInstantBar.SendMessage(WM_IDLEUPDATECMDUI, TRUE, 0);
					}

			if(m_nSwitch%2==0 && m_nState != WA_INACTIVE)
			{
				try{

				//update all
//				m_wndReBar.SendMessageToDescendants(WM_IDLEUPDATECMDUI,
//					(WPARAM)TRUE, 0, FALSE, FALSE);
				m_wndToolBar.PostMessage(WM_IDLEUPDATECMDUI, TRUE, 0);
				m_wndStatusBar.PostMessage(WM_IDLEUPDATECMDUI, TRUE, 0);
				m_wndStatusBar.IdleUpdateUI();
//				m_wndStatusBar.SendMessageToDescendants(WM_IDLEUPDATECMDUI,
//					(WPARAM)TRUE, 0, FALSE, FALSE);
				if(bVerbose && m_nSwitch==40)
				{
					SaveLastVisit();
					bVerbose = FALSE;
				}

				}catch(...)
				{
				}

			}
			else
			{
				//only status bar
				m_wndStatusBar.PostMessage(WM_IDLEUPDATECMDUI, TRUE, 0);
			}

			m_nSwitch = m_nSwitch++;
			if(m_nSwitch > 40)
				m_nSwitch = 0;

	/*3.2		if(m_bNeedActive)
			{
				if(nDelay>4)
				{
					//active myie
					SetActiveWindow();
					m_bNeedActive = FALSE;
					nDelay = 0;
				}
				else
					nDelay ++;
			}*/


//			if(bUpdateTab)
//				SendMessage(WM_UPDATE_TAB);
//			bUpdateTab = FALSE;
		}
	}	
	else if(nIDEvent == m_nAnimateIconTimer )
	{
		if(m_bAnimatIcon)
		{
			//animate icon
			if(m_nNeedIB==6)
			{
				m_bIB = IsBrowsing();
				m_nNeedIB = 0;
				if(!m_bIB && m_bLastIB)
					m_animIcon.ShowFirstImage();
				m_bLastIB = m_bIB;
			}
			else
				m_nNeedIB ++;

			if(m_bLastIB)
				m_animIcon.ShowNextImage();
		}

	}
	else if(nIDEvent == m_nTimerID)
	{
		if(m_astrOpenAll.IsEmpty())
		{
			KillTimer(m_nTimerID);
			m_nTimerID = 0;
		}
		else
		{
			int max = 6;
			if(m_nInterval)
				max = 1;
			try
			{
			while(!m_astrOpenAll.IsEmpty() && max>0)
			{
				gString1= m_astrOpenAll.GetHead();
				m_astrOpenAll.RemoveHead();
				//is exe?
				if(gString1.Right(4)==".EXE" || gString1.Right(4)==".exe")
					ShellExecute(NULL, "open", gString1 , "", NULL, SW_SHOW );
				else
					NewChildWindow(1,2,gString1);
				max --;
			}
			}
		/*	catch(CMemoryException * e)
			{
				if(e!=NULL)e->Delete();
			}*/
			catch(...)
			{
			}
			
			if(m_astrOpenAll.IsEmpty())
			{
				KillTimer(m_nTimerID);
				m_nTimerID = 0;
			}

		}
	}

	CMDIFrameWnd::OnTimer(nIDEvent);

	}/*catch(CMemoryException * e)
	{
//		AfxMessageBox("timer error");
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}
}


void CMainFrame::OnFileStopOpenall() 
{
	// TODO: Add your command handler code here
	m_astrOpenAll.RemoveAll();
}

void CMainFrame::OnUpdateFileStopOpenall(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//2.2	try{
	if(m_nTimerID)
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);
//	}catch(...)
//	{
//	}

}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
/*	WNDCLASS ws;
	ws.style = CS_DBLCLKS; 
	ws.lpfnWndProc = AfxWndProc;
	ws.cbClsExtra = 0;
	ws.cbWndExtra = 0;
	ws.hIcon = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	ws.hCursor = LoadCursor(NULL,IDC_ARROW);
	ws.hbrBackground = NULL;
	ws.lpszMenuName = NULL;
	ws.lpszClassName = "MyIEWebBrowser";
	ws.hInstance = AfxGetInstanceHandle();

	RegisterClass(&ws);

	cs.lpszClass = "MyIEWebBrowser";*/

	
	cs.x = 0;
	cs.y = 0;
	cs.cx = GetSystemMetrics(SM_CXSCREEN);//800;
	cs.cy = GetSystemMetrics(SM_CYSCREEN); //600;

	return CMDIFrameWnd::PreCreateWindow(cs);
}



void CMainFrame::OnFileHide() 
{
	// TODO: Add your command handler code here
	if(IsWindowVisible())
	{
		//ShowWindow(SW_MINIMIZE);
		ShowWindow(SW_HIDE);
		RegisterHotKey (m_hWnd, 0, MOD_ALT , 192);
		bHidden = TRUE;
	}
}

void CMainFrame::OnHotKey(WPARAM wParam,LPARAM)
{
	if(bHidden)
	{
		ShowWindow(SW_MINIMIZE);
		SetForegroundWindow();
		BOOL bMax;
		CChildFrame* tcf = (CChildFrame*)MDIGetActive(&bMax);
		if(tcf!=NULL && tcf->bProtect)
		{
			CInputPassword dlg;
			BOOL validate = FALSE;
			if(dlg.DoModal()==IDOK)
			{
				if(dlg.m_strPassword == tcf->m_strPsw)
					validate = TRUE;
				else
					AfxMessageBox(IDS_PASSWORD_ERROR);
			}
			if(!validate)
			{
				ShowWindow(SW_HIDE);
				return;
			}
		}

		//restore
		UnregisterHotKey (m_hWnd, 0);
		bHidden = FALSE;
		ShowWindow(SW_RESTORE);
	}
}

void CMainFrame::OnViewFolder() 
{
	// TODO: Add your command handler code here
	if(m_bSecure)
		return;

	try{

	CString str,str1;
//	ShowExpBar(str);
	if(m_wndInstantBar.GetSafeHwnd()==NULL)
		BuildExpBar("");

	BOOL bNeedInI=false;
	if(m_pwndExpObj == NULL)
		bNeedInI = true;
	else
		str.LoadString(IDS_TITLE_SHELL);
	m_wndInstantBar.GetWindowText(str1);
	if(bNeedInI || str1!=str)
	{
		RemoveExp();
		try
		{
		m_pwndExpObj = new CShellTree;
		}
		catch(...)
		{
			m_pwndExpObj = NULL;
			return;
		}

		m_pwndExpObj->Create(&m_wndInstantBar);
		m_pwndExpObj->Initialize();
		str = m_pwndExpObj->GetTitle();
		m_hEdit = ((CShellTree*)m_pwndExpObj)->m_wndShell.m_fType.GetSafeHwnd();

		CRect rect;
		m_wndInstantBar.GetClientRect(&rect);
		m_wndInstantBar.SendMessage(WM_SIZE, 0, ((rect.Height())<<16)+rect.Width());

		m_wndInstantBar.SetWindowText(str);
		if(!m_wndInstantBar.IsVisible())
		{
			ShowControlBar(&m_wndInstantBar, TRUE, FALSE);
		}
	}
	else
	{
		ShowControlBar(&m_wndInstantBar, !m_wndInstantBar.IsVisible(), FALSE);
		if(m_wndInstantBar.IsVisible())
			m_hEdit = m_hEdit = ((CShellTree*)m_pwndExpObj)->m_wndShell.m_fType.GetSafeHwnd();
		else
			m_hEdit = NULL;
	}	

	}catch(...)
	{
	}
}

void CMainFrame::OnOptionsOpenlinkinnewwindow() 
{
	// TODO: Add your command handler code here
	bLinkInNewWindow = !bLinkInNewWindow;

	m_bmUserDefault.m_bActiveNewWindow = m_bActiveNewWindow;
	m_bmUserDefault.m_bAlwaysNewWindow = bAddressInNewWindow;
	m_bmUserDefault.m_nSeq = m_nSeq;
	m_bmUserDefault.bLinkInNewWin = bLinkInNewWindow;
}

void CMainFrame::OnUpdateOptionsOpenlinkinnewwindow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//2.2	try{
	pCmdUI->SetCheck(bLinkInNewWindow);
//	}catch(...)
//	{
//	}

}


void CMainFrame::OnScRll(WPARAM wParam,LPARAM lParam)
{
	try{

	 CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	 if(tcf != NULL && tcf->m_pView != NULL)
	 {
		tcf->m_pView->m_wndBrowser.SendMessage(MSWHELL_ROLLMSG, wParam, lParam);
	 }
	}catch(...)
	{
	}
}


void CMainFrame::OnUpdateOptionsBrowsemodesDefault(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	try{
	if(bLinkInNewWindow == m_bmUserDefault.bLinkInNewWin &&
		m_bActiveNewWindow == m_bmUserDefault.m_bActiveNewWindow &&
		bAddressInNewWindow == m_bmUserDefault.m_bAlwaysNewWindow &&
		m_nSeq == m_bmUserDefault.m_nSeq)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
	}catch(...)
	{
	}
}



void CMainFrame::OnUpdateOptionsBrowsemodesNews(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	try{
	if(bLinkInNewWindow == m_bmNews.bLinkInNewWin &&
		m_bActiveNewWindow == m_bmNews.m_bActiveNewWindow &&
		bAddressInNewWindow == m_bmNews.m_bAlwaysNewWindow &&
		m_nSeq == m_bmNews.m_nSeq)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
	}catch(...)
	{
	}
}



void CMainFrame::OnUpdateOptionsBrowsemodesStandard(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	try{
	if(bLinkInNewWindow == m_bmStandard.bLinkInNewWin &&
		m_bActiveNewWindow == m_bmStandard.m_bActiveNewWindow &&
		bAddressInNewWindow == m_bmStandard.m_bAlwaysNewWindow &&
		m_nSeq == m_bmStandard.m_nSeq)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
	}catch(...)
	{
	}
	
}

void CMainFrame::OnViewExplorebarResource() 
{
	// TODO: Add your command handler code here
	CString str,str1;
//	ShowExpBar(str);
	try{

	if(m_wndInstantBar.GetSafeHwnd()==NULL)
		BuildExpBar("");

	BOOL bNeedInI=false;
	if(m_pwndExpObj == NULL)
		bNeedInI = true;
	else
		str.LoadString(IDS_TITLE_RESOURCE);
	m_wndInstantBar.GetWindowText(str1);
	if(bNeedInI || str1!=str)
	{
		RemoveExp();
		try
		{
		m_pwndExpObj = new CResourcebar;
		}
		catch(...)
		{
			m_pwndExpObj = NULL;
			return;
		}

		m_pwndExpObj->Create(&m_wndInstantBar);
		m_pwndExpObj->Initialize();
		str = m_pwndExpObj->GetTitle();

		CRect rect;
		m_wndInstantBar.GetClientRect(&rect);
		m_wndInstantBar.SendMessage(WM_SIZE, 0, ((rect.Height())<<16)+rect.Width());

		m_wndInstantBar.SetWindowText(str);
		if(!m_wndInstantBar.IsVisible())
		{
			ShowControlBar(&m_wndInstantBar, TRUE, FALSE);
		}
	}
	else
	{
		if(m_wndInstantBar.IsVisible())
			m_pwndExpObj->Update(1);
		else
			m_pwndExpObj->Update(0);

		ShowControlBar(&m_wndInstantBar, !m_wndInstantBar.IsVisible(), FALSE);
	}	

	}catch(...)
	{
	}
}



BOOL CMainFrame::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default

	LPDISPATCH pDisp = NULL;
	IHTMLDocument2 *pHTMLDoc = NULL;
	IHTMLWindow2 *pHTMLWnd = NULL;

	try{

	 CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	 if(tcf != NULL && tcf->m_pView != NULL)
	 {

		pDisp = tcf->m_pView->GetHtmlDocument();
		if( pDisp )
		{
			if (SUCCEEDED(pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc)))
			{
				if(SUCCEEDED(pHTMLDoc->get_parentWindow( &pHTMLWnd )))
				{
					if(zDelta>0)
					{
						pHTMLWnd->scrollBy(0, -20);
						pHTMLWnd->scrollBy(0, -30);
					}
					else
					{
						pHTMLWnd->scrollBy(0, 20);
						pHTMLWnd->scrollBy(0, 30);
					}
				//2.1
					//pHTMLWnd->Release();
					//pHTMLWnd=NULL;
				}
				//pHTMLDoc->Release();
				//pHTMLDoc = NULL;
			}
			//pDisp->Release();
			//pDisp = NULL;
		}
	 }
	}
	catch(...)
	{
	}

	try{
		if(pHTMLWnd != NULL)
			pHTMLWnd->Release();
		if(pHTMLDoc != NULL)
			pHTMLDoc->Release();
		if(pDisp != NULL)
			pDisp->Release();

	 return CMDIFrameWnd::OnMouseWheel(nFlags, zDelta, pt);
	}
	catch(...)
	{
		return TRUE;
	}

}


void CMainFrame::OnEditUndoclose() 
{
	// TODO: Add your command handler code here
	int c =	undoEnd-1;
	if(c<0)
		c += 16;
	if(undoEnd != undoStart && m_strUndoURL[c] != "")
	{
		NewChildWindow(1,2,m_strUndoURL[c]);
		undoEnd = c;
		UpdateUndoMenu();
	}
}

void CMainFrame::OnUpdateEditUndoclose(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//	try{
	if(undoEnd == undoStart)
		pCmdUI->Enable(FALSE);
	else
	{
		pCmdUI->Enable();
	}
//	}catch(...)
//	{
//	}

}


void CMainFrame::OnEditEdit() 
{
	// TODO: Add your command handler code here
	static TCHAR           sz[MAX_PATH];
	HKEY            hKey;
	DWORD           dwSize = MAX_PATH;

	
	 CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	 if(tcf == NULL || tcf->m_pView == NULL)
		return;

	// find out from the registry where the editor are located.
	if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\Default HTML Editor\\shell\\edit\\command"), &hKey) != ERROR_SUCCESS)
	{
		if(RegOpenKey(HKEY_CLASSES_ROOT, _T("InternetShortcut\\shell\\edit\\command"), &hKey) != ERROR_SUCCESS)
		{
			//use view source instead
			ExecCmdTarget(HTMLID_VIEWSOURCE, tcf->m_pView);
			return ;
		}
	}
	dwSize = sizeof(sz);
	RegQueryValueEx(hKey, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);
	RegCloseKey(hKey);
	if(dwSize>0)
	{
		LaunchEditor(sz, tcf->m_pView->m_lpszUrl);
	}
	else
		//use view source instead
		ExecCmdTarget(HTMLID_VIEWSOURCE, tcf->m_pView);

}

void CMainFrame::FavMenuAddFolder(LPCSTR path, LPCSTR root, BOOL FirstLevel, BOOL IsLink, CMenu* pMenu, int& nEndPos)
{
	int nMenuLen = m_nMenuWidth; 
	int iString;
	TBBUTTON tbb;
	//CString fstr;
	static char mff[10];
	static CString			strOpenAll, strOpenMost, strAddFav;
//	static CString temp;

	if(strlen(path)==0)
		return;

	BOOL rec = FALSE;
	gString2 = root;
	gString2+= path;
	gString2+= "\\";
	if(gString2.Find(m_strLinkFolder)==0)
	{//only in or is link folder we will recurse
			rec = TRUE;
	}

	int				nSubFavs = 0, nSubMostFavs = 0, fs;
	CMenu * pSubMenu = NULL;
	try
	{
		pSubMenu = new CMenu;
	}
	catch(...)
	{
//		if(e!=NULL)e->Delete();
		return;
	}

	pSubMenu->CreatePopupMenu();
	if(rec)
	{
			// call this function recursively.
			gString2 = root;
			gString2 += path;
			nEndPos = BuildFavoritesMenu(gString2, nEndPos, pSubMenu, 0, FALSE, nSubFavs, nSubMostFavs);
	}
	else
	{
		try
		{
			CMenuData * md= new CMenuData;
			md->m_pImgList = &m_wndMenuBar.m_ImgList;
			md->m_nBitmap = -1;
			SetMenuText(gString2,md);
			md->m_bIni = FALSE;

			pSubMenu->AppendMenu(MF_OWNERDRAW|MF_DISABLED|MF_GRAYED , 0, (char*)md );
		}
		catch(...)
		{
//			if(e!=NULL)e->Delete();
		}
	}

	try
	{
	// only intert a submenu if there are in fact .URL files in the subdirectory
	fstr = path;
	if(fstr.GetLength()>nMenuLen)
	{
		fstr = fstr.Left(nMenuLen-3);
		fstr += "...";
	}
	mff[0]=0;

	gString2 = root;
	gString2 += path;
	gString2 += "\\";
	GetPrivateProfileString("MyIE", "MostFav", "0", mff, 2, gString2+"myiefolder.mic");
	CMenuData * md= new CMenuData;

	md->m_pImgList = &m_wndMenuBar.m_ImgList;
	if(mff[0]=='1')
		md->m_nBitmap = 5;
	else
		md->m_nBitmap = 0;
	
	SetMenuText(fstr, md);
	//pMenu->InsertMenu(nPos+nFixMenu, MF_BYPOSITION | MF_POPUP | MF_OWNERDRAW, (UINT)pSubMenu->m_hMenu , (char*)md );
	pMenu->AppendMenu(MF_BYPOSITION | MF_POPUP | MF_OWNERDRAW, (UINT)pSubMenu->m_hMenu , (char*)md );



	//add favorites bar
	if(FirstLevel)
	{
		memset(&tbb, 0, sizeof(tbb));
		iString = m_FavBar.GetToolBarCtrl().AddStrings(fstr);
		tbb.iString = iString;
		tbb.fsState = TBSTATE_ENABLED;
		tbb.fsStyle = TBSTYLE_AUTOSIZE;
		if(mff[0]=='1')
			tbb.iBitmap = 5;
		else
			tbb.iBitmap = 0;
		if(bCompactFavBar)
			tbb.iBitmap = -1;
		tbb.idCommand = ID_FAV_POPUP;
		tbb.dwData = (UINT)pSubMenu->m_hMenu;
		VERIFY(m_FavBar.GetToolBarCtrl().AddButtons(1, &tbb));
		m_astrFavs.Add(path);
	}

	if(IsLink)
	{
		memset(&tbb, 0, sizeof(tbb));
		iString = m_LinkBar.GetToolBarCtrl().AddStrings(fstr);
		tbb.iString = iString;
		tbb.fsState = TBSTATE_ENABLED;
		tbb.fsStyle = TBSTYLE_AUTOSIZE;
		if(mff[0]=='1')
			tbb.iBitmap = 5;
		else
			tbb.iBitmap = 0;
		tbb.idCommand = ID_LINK_POPUP;
		tbb.dwData = (UINT)pSubMenu->m_hMenu;
		VERIFY(m_LinkBar.GetToolBarCtrl().AddButtons(1, &tbb));
		m_astrLinks.Add(path);
	}
	if(rec)
	{
		strOpenAll.LoadString(IDS_OPEN_ALL_FAV);
		strOpenMost.LoadString(IDS_OPEN_ALL_MOSTFAV);
		strAddFav.LoadString(IDS_ADDFAV);

		//add open all favorites menu
		if(m_bDisplayOpenAll || m_bDisplayAddFav)
		{
			if(nSubFavs || nSubMostFavs || m_bDisplayAddFav)
				pSubMenu->InsertMenu(0,MF_BYPOSITION | MF_SEPARATOR);
			fs=m_astrFavoriteFolders.GetCount();
			gString2 = root;
			m_astrFavoriteFolders.AddTail(gString2 + path);
		}
		if(m_bDisplayOpenAll)
		{
			if(nSubMostFavs)
				pSubMenu->InsertMenu(0,MF_BYPOSITION| MF_STRING ,0x1280+fs, strOpenMost);
			if(nSubFavs)
				pSubMenu->InsertMenu(0,MF_BYPOSITION| MF_STRING ,0x1000+fs, strOpenAll);
		}
		if(m_bDisplayAddFav)
		{
			//add fav
			pSubMenu->InsertMenu(0,MF_BYPOSITION| MF_STRING ,0x6220+fs, strAddFav);
		}
	}
	pSubMenu->Detach();
	delete pSubMenu;

	}
/*	catch(CMemoryException* e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}
}

void CMainFrame::FavMenuAddURL(LPCSTR path, LPCSTR root, BOOL IsLink, CMenu *pMenu, int nPos, int& nMostFavs)
{
	int nMenuLen = m_nMenuWidth; 
	TBBUTTON tbb;
	static TCHAR           buf[INTERNET_MAX_PATH_LENGTH];
//	static CString str;
//	static CString fstr;

	//pMenu->AppendMenu(MF_STRING | MF_ENABLED , 0xe00 + nPos, fstr);
	CMenuData * md = NULL;
	try
	{
		md= new CMenuData;
	}
	//CATCH(CMemoryException, e)
	catch(...)
	{
//		if(e!=NULL)e->Delete();
		return;
	}
//	END_CATCH

	try{

	md->m_pImgList = &m_wndMenuBar.m_ImgList;

	fstr = path;
	gString2 = path;
	fstr.MakeLower();
	buf[0]=0;
	if(fstr.Right(4)==".url")
	{
		::GetPrivateProfileString(_T("MyIE"), _T("MostFav"),
						  _T(""), buf, 2048,
						  root + fstr);
		if(buf[0]!='1')
			md->m_nBitmap = 1;
		else
		{
			md->m_nBitmap = 4;
			nMostFavs ++;
		}
		::GetPrivateProfileString(_T("InternetShortcut"), _T("URL"),
				  _T(""), buf, INTERNET_MAX_PATH_LENGTH,
				  root + fstr);
		SetURL(buf, md);
	}
	else
	{
		md->m_nBitmap = 1;
		SetURL(path, md);
	}

	gString2 = gString2.Left(gString2.GetLength()-4);
	if(gString2.GetLength()>nMenuLen)
	{
		gString2 = gString2.Left(nMenuLen-3);
		gString2 += "...";
	}
//	gString2.Replace("&", "&&");
	SetMenuText(gString2, md);

//			md->nID = 0xe00 + nPos;
//			md->nFlags = MF_ENABLED;
	pMenu->AppendMenu ( MF_ENABLED|MF_OWNERDRAW, 0x1380 + nPos , (char*)md );

	if(IsLink)
	{
/*		m_LinkBar.SetButtonInfo(ind,0x1380 + nPos,TBSTYLE_AUTOSIZE,md->m_nBitmap);
		m_LinkBar.SetButtonText(ind, fstr);
*/
		memset(&tbb, 0, sizeof(tbb));
		int iString = m_LinkBar.GetToolBarCtrl().AddStrings(gString2);
		tbb.iString = iString;
		tbb.fsState = TBSTATE_ENABLED;
		tbb.fsStyle = TBSTYLE_AUTOSIZE;
		tbb.iBitmap = md->m_nBitmap;
		tbb.idCommand = 0x1380 + nPos;
		tbb.dwData = 0;
		VERIFY(m_LinkBar.GetToolBarCtrl().AddButtons(1, &tbb));

		m_astrLinks.Add(gString2);
		m_LinkBar.GetToolBarCtrl().GetToolTips()->UpdateTipText(buf, &m_LinkBar, 0x1380+nPos);
	}

	}catch(...)
	{
	}
}


void CMainFrame::OnToolsAutoscrollSpeed(UINT nID) 
{
	// TODO: Add your command handler code here
	CMyIEView * pvw = NULL;
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	if (tcf!=NULL)
		pvw = (CMyIEView*)tcf->m_pView;
	if(pvw == NULL)
		return;

	pvw->m_nSrSpeed = nID - ID_TOOLS_AUTOSCROLL_SLOW+1;

	m_nDefSrSpeed = pvw->m_nSrSpeed;

	pvw->ChangeSpeed(pvw->m_nSrSpeed);
}


void CMainFrame::OnViewRefresh()
{
	try{

	CMyIEView * pvw = NULL;
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	if (tcf!=NULL)
		pvw = (CMyIEView*)tcf->m_pView;
	if(pvw == NULL)
		return;

//	if(!pvw->m_bForceRefresh || pvw->m_lpszUrl.IsEmpty())
	{
//2.5		short ks=GetKeyState(VK_CONTROL);
//		if(ks<0)
		{
			pvw->Refresh2(REFRESH_COMPLETELY);
		}
//		else
//			pvw->Refresh2(REFRESH_NORMAL);
	}
/*	else
	{
		pvw->Navigate2(pvw->m_lpszUrl, 0, NULL);
		pvw->m_bForceRefresh = FALSE;
	}*/

	}/*catch(CMemoryException * e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}
}

void CMainFrame::OnViewStop()
{
	try{

	CMyIEView * pvw = NULL;
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	if (tcf!=NULL)
		pvw = (CMyIEView*)tcf->m_pView;
	if(pvw == NULL)
		return;

	pvw->Stop();
	pvw->m_nProgress = -10;
	if(pvw->IsActive)
		percent = pvw->m_nProgress;


		//set icon

		int nTabID = FindTab(tcf);
		int nIcon =0;
		if(!pvw->IsActive)
			pvw->oldIcon = SetTabIcon( 5, nTabID );
		else if(tcf->bLock)
			pvw->oldIcon = SetTabIcon( 4, nTabID );
		else if(tcf->bProtect)
			pvw->oldIcon = SetTabIcon( 6, nTabID );
		else
		{
			pvw->oldIcon = SetTabIcon( -1, nTabID );
			nIcon = -1;
		}

		//update tab bar
		if(m_bFBTab)
			PostMessage(WM_UPDATE_TAB);

	}/*catch(CMemoryException * e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{
	}
}


void CMainFrame::OnFilePrintpreview() 
{
	// TODO: Add your command handler code here
	CMyIEView * pvw = NULL;
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	if (tcf!=NULL)
		pvw = (CMyIEView*)tcf->m_pView;
	if(pvw == NULL)
		return;

	pvw->ExecCommand(OLECMDID_PRINTPREVIEW);
}

void CMainFrame::OnFilePrint() 
{
	// TODO: Add your command handler code here
	CMyIEView * pvw = NULL;
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	if (tcf!=NULL)
		pvw = (CMyIEView*)tcf->m_pView;
	if(pvw == NULL)
		return;

	pvw->ExecCommand(OLECMDID_PRINT);
}


void CMainFrame::ToolBarSetCheck(CCmdUI *pCmdUI, BOOL bCheck)
{
		if(m_strBGPic.IsEmpty() || pCmdUI->m_pMenu !=NULL)
			pCmdUI->SetCheck(bCheck);
		else
		{
			pCmdUI->SetCheck(0);
			if(m_wndToolBar.GetToolBarCtrl().IsButtonPressed(pCmdUI->m_nID) != bCheck)
				m_wndToolBar.GetToolBarCtrl().PressButton(pCmdUI->m_nID, bCheck);
		}
}



void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	m_nState = nState;
}

void CMainFrame::InitGroups()
{
	//load group names
	if(((CMyIEApp*)AfxGetApp())->m_strGroupPath.Right(1) != "\\")
		((CMyIEApp*)AfxGetApp())->m_strGroupPath += "\\";
	CString psz = ((CMyIEApp*)AfxGetApp())->m_strGroupPath;
	char cur[MAX_PATH*2];
	cur[0]=0;
	GetCurrentDirectory(MAX_PATH*2, cur);
	if(!SetCurrentDirectory(psz))
		CreateDirectory(psz, NULL);
	else
		SetCurrentDirectory(cur);

	WIN32_FIND_DATA wfd;
	HANDLE          h;
	psz += "*.cgp";
	CMenu* pmenu = GetMenu()->GetSubMenu(4)->GetSubMenu(0);
	if(pmenu == NULL)
		return;
	while(pmenu->DeleteMenu(4, MF_BYPOSITION));


	int i=0;
	char tmp[5]; 
	CString strMenu;
	h = FindFirstFile(psz, &wfd);
	if(h != INVALID_HANDLE_VALUE)
	{
		do{
			itoa(i+1, tmp, 10);
			strMenu = tmp;
			strMenu += ". "; 
			strMenu += wfd.cFileName;
			strMenu = strMenu.Left(strMenu.GetLength()-4);
			pmenu->AppendMenu( MF_ENABLED, 0x6001 + i , strMenu);
			i++;
		}
		while(FindNextFile(h, &wfd) && i<255);
	} 

	FindClose(h);
	AddMenuBreak(pmenu, FALSE);

	//update add to group menus
	pmenu = GetMenu()->GetSubMenu(4)->GetSubMenu(0)->GetSubMenu(2);
	MenuLoadGroupList(pmenu);
}


void CMainFrame::OnOpenGroup(UINT nID)
{
	CString filename;	
	CMenu* pmenu = GetMenu()->GetSubMenu(4)->GetSubMenu(0);
	if(pmenu == NULL)
		return;
	pmenu->GetMenuString(nID, filename, MF_BYCOMMAND);

	filename = filename.Mid(filename.Find(' ')+1);

	OpenGroup(filename);

}

void CMainFrame::OnOrggroup() 
{
	// TODO: Add your command handler code here
	COrgGroupDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		InitGroups();
	}
}

void CMainFrame::MenuLoadGroupList(CMenu *pMenu)
{
	//load group names
	if(pMenu == NULL)
		return;

	if(((CMyIEApp*)AfxGetApp())->m_strGroupPath.Right(1) != "\\")
		((CMyIEApp*)AfxGetApp())->m_strGroupPath += "\\";
	CString psz = ((CMyIEApp*)AfxGetApp())->m_strGroupPath;

	WIN32_FIND_DATA wfd;
	HANDLE          h;
	psz += "*.cgp";

	while(pMenu->DeleteMenu(0, MF_BYPOSITION));
	
	int i=0;
	char tmp[5]; 
	CString strMenu;
	h = FindFirstFile(psz, &wfd);
	if(h != INVALID_HANDLE_VALUE)
	{
		do{
			itoa(i+1, tmp, 10);
			strMenu = tmp;
			strMenu += ". "; 
			strMenu += wfd.cFileName;
			strMenu = strMenu.Left(strMenu.GetLength()-4);
			pMenu->AppendMenu( MF_ENABLED, 0x6101 + i , strMenu);
			i++;
		}
		while(FindNextFile(h, &wfd) && i<255);
	} 

	FindClose(h);
	if(i==0)
		pMenu->AppendMenu(MF_GRAYED, 0, "Empty");
	else
		AddMenuBreak(pMenu, FALSE);
}



void CMainFrame::OpenGroup(LPCSTR gname)
{
//	static CString filename;

	if(gname == NULL || strlen(gname)==0)
		return;

	gString2 = ((CMyIEApp*)AfxGetApp())->m_strGroupPath + gname+".cgp";

	char * valuepair = NULL;
	int nsize = 0;
	int nWins = 0;
	HANDLE hfile = INVALID_HANDLE_VALUE;
	try{
		hfile = CreateFile(gString2, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hfile == INVALID_HANDLE_VALUE)
			return;
		
		nsize = GetFileSize(hfile, NULL);
		CloseHandle(hfile);
		hfile = INVALID_HANDLE_VALUE;

		if(nsize>0)
		{
			valuepair = new char[nsize+1];
			GetPrivateProfileSection("CaptorGroup", valuepair, nsize, gString2);

			char * value;
			value = valuepair;
			while(value!=NULL && strlen(value)>0)
			{
				value = strchr(value, '=');
				if(value!=NULL)
				{
					value ++;
					if(nWins == 0)
						NewChildWindow(1,2, value);
					else
						m_astrOpenAll.AddTail(value);
					nWins++;
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
//		if(e!=NULL)e->Delete();
	}

	if(m_nInterval && nWins>1)
		m_nTimerID = SetTimer(1000, m_nInterval*1000, NULL);
	else if(nWins>1)
		m_nTimerID = SetTimer(1000, 20, NULL);
	//enable toolbar button
	m_wndToolBar.GetToolBarCtrl().EnableButton(ID_FILE_STOP_OPENALL);

}

void CMainFrame::AddtoFavorite(BSTR bstrURL, VARIANT *pTitle)
{
	IShellUIHelper* pShellUIHelper = NULL;
	try
	{
	   HRESULT hr = CoCreateInstance(CLSID_ShellUIHelper, NULL, 
									 CLSCTX_INPROC_SERVER, IID_IShellUIHelper, 
									 (LPVOID*)&pShellUIHelper);

	   if (SUCCEEDED(hr))
	   {
			pShellUIHelper->AddFavorite(bstrURL, pTitle);
//2.1
			//pShellUIHelper->Release();
			//pShellUIHelper = NULL;

		   //rebuild favorites menu
			//InitFavorites();
		    m_bFavDirty = TRUE;
			
			//update favorites list
			if(m_pwndExpObj!=NULL && HasExpBar(IDS_TITLE_FAVORITE))
				m_pwndExpObj->Update(0);
	   }

	}
/*	catch(CMemoryException * e)
	{
		if(e!=NULL)e->Delete();
	}*/
	catch(...)
	{//?
	}
	try{
		if(pShellUIHelper)
			pShellUIHelper->Release();
	}catch(...)
	{
	}
}

/*void CMainFrame::OnParentNotify(UINT message, LPARAM lParam) 
{
	CMDIFrameWnd::OnParentNotify(message, lParam);
	
	// TODO: Add your message handler code here
	if( (message==WM_LBUTTONDOWN && m_nAllowDBC>0) || (message==WM_MBUTTONDOWN && m_nAllowMC>0))
	{
		try{

		tabpt2.y = HIWORD(lParam);
		tabpt2.x = LOWORD(lParam);

		ClientToScreen(&tabpt2);
		HWND  hWnd = ::WindowFromPoint(tabpt2);
		if(hWnd== NULL || hWnd != m_wndTab.GetSafeHwnd())
			return;

		BOOL dbc = FALSE, mc = FALSE;

		if(message==WM_LBUTTONDOWN)
		{
			LARGE_INTEGER liFinish;
			QueryPerformanceCounter(&liFinish);
			LARGE_INTEGER liFreq;
			QueryPerformanceFrequency(&liFreq);
			float secs = (float)(liFinish.LowPart - m_tDBClick.LowPart)/liFreq.LowPart;

			if(abs(tabpt2.y-tabpt.y)<2 && abs(tabpt2.x-tabpt.x)<2 && secs <= dbtime && secs>0)
			{
				 //is dbclick
				dbc = TRUE;	
				m_tDBClick.LowPart = 0;	
			}
			else
			{
				m_tDBClick = liFinish;
				tabpt = tabpt2;
			}
		}
		else
		{
			//middel button up
			mc = TRUE;
		}

		if(dbc || mc)
		{
			m_wndTab.ScreenToClient(&tabpt2);
			TCHITTESTINFO hti;
			hti.pt = tabpt2;
			hti.flags = TCHT_ONITEM | TCHT_ONITEMICON | TCHT_ONITEMLABEL ;
			int nTabID = m_wndTab.HitTest(&hti);
			TCITEM TabCtrlItem;
			TabCtrlItem.mask = TCIF_PARAM;

			m_wndTab.GetItem(nTabID, &TabCtrlItem);
			if(TabCtrlItem.lParam != NULL)
			{
				CChildFrame* tcf = ((CChildFrame*)TabCtrlItem.lParam);
				if((dbc && m_nAllowDBC==1) || (mc && m_nAllowMC==1))
				{
					//2.2 313 post instead send
					tcf->PostMessage(WM_CLOSE);
					tcf->m_pView->m_bToClose = TRUE;
					//tcf->MDIDestroy();
				}
				else if((dbc && m_nAllowDBC==2) || (mc && m_nAllowMC==2))
				{
					//save as
					tcf->m_pView->PostMessage(WM_COMMAND, ID_FILE_SAVEAS, 0);
				}
				else if((dbc && m_nAllowDBC==3) || (mc && m_nAllowMC==3))
				{
					//save html
					tcf->m_pView->PostMessage(WM_COMMAND, ID_FILE_SAVEHTML, 0);
				}
			}
		}

		}catch(...)
		{
			//m_tDBClick.LowPart = 0;	
		}

	}

}*/

void CMainFrame::OnWindowList(UINT nID)
{
	try{

	int i = nID-0xc00;
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;

	m_wndTab.GetItem(i, &TabCtrlItem);
	CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
	if(tcf!=NULL)
	{
		MDIActivate(tcf);
		m_wndTab.SetCurSel(i);
	}
	}catch(...)
	{
	}
}


void CMainFrame::OnUpdateWindowMin(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	try{
	if(nWin == 0)
		pCmdUI->Enable(FALSE);	
	else
		pCmdUI->Enable();	

	CMenu* pPopupMenu=pCmdUI->m_pMenu;
	if(pPopupMenu != NULL)
		{
			//window list
			DestroyMenuData(pPopupMenu->m_hMenu);
			while(pPopupMenu->DeleteMenu(7, MF_BYPOSITION));

			CChildFrame* tcf;
			nWin=m_wndTab.GetItemCount();
			int n = nWin;
			int sel = m_wndTab.GetCurSel();
			CString strm;
			TCITEM TabCtrlItem;
			n=n>250?250:n;

			CMenuData * md;
			TabCtrlItem.mask = TCIF_PARAM|TCIF_IMAGE;
			for(int i=0;i<n;i++)
			{
				TabCtrlItem.lParam = 0;
				TabCtrlItem.iImage = 0;
				m_wndTab.GetItem(i, &TabCtrlItem);
				tcf = ((CChildFrame*)TabCtrlItem.lParam);
				if(tcf != NULL && tcf->m_pView != NULL)
				{
					tcf->GetWindowText(strm);
					md = NULL;
					md= new CMenuData;
					if(md != NULL)
					{
						md->m_pImgList = &m_TabImgList;
						md->m_nBitmap=TabCtrlItem.iImage;
						if(strm.GetLength()>50)
							strm=strm.Left(50);
						if(strm.IsEmpty())
							strm.LoadString(IDS_UNTITLED);
						SetMenuText(strm, md);
						pPopupMenu->AppendMenu ( MF_ENABLED|MF_OWNERDRAW, 0xc00 + i , (char*)md );
						if(sel == i)
							pPopupMenu->SetDefaultItem(0xc00+i);
					}
				}
			}

		}

	}catch(...)
	{
	}
}

void CMainFrame::SaveLastVisit()
{
	//save last visited urls
	try{
	lastvisits.Empty();
	CChildFrame* tcf;
	UINT n = m_wndTab.GetItemCount();
	nWin = n;
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;

	try{
	for(UINT i=0;i<n;i++)
	{
		
		tcf = NULL;
		TabCtrlItem.lParam = NULL;
		m_wndTab.GetItem(i, &TabCtrlItem);
		tcf = ((CChildFrame*)TabCtrlItem.lParam);
		if(tcf != NULL && tcf->m_pView!=NULL)
		{
			if(!tcf->m_pView->m_lpszUrl.IsEmpty() && !tcf->m_pView->m_bToClose)
			{
				lastvisits += tcf->m_pView->m_lpszUrl;
				lastvisits += "\n";	
			}
		}

	}
	}catch(...)
	{
	}

	AfxGetApp()->WriteProfileBinary("Settings","lastvisit", (LPBYTE)((LPCSTR)lastvisits), lastvisits.GetLength()+1);
	}catch(...)
	{
	}
}

void CMainFrame::OnNewwindow() 
{
	// TODO: Add your command handler code here
	switch(m_nNewWindow)
	{
	case 0: OnFileNewBlank();
		break;
	case 1: OnFileNew();
		break;
	case 2: OnFileNewCurrentpage();
	}
}

void CMainFrame::OnNewwindowFav() 
{
	// TODO: Add your command handler code here
	if(bFavInNewWindow)
		bFavInNewWindow = FALSE;
	else
		bFavInNewWindow = TRUE;
}

void CMainFrame::OnUpdateNewwindowFav(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//2.2	try{
	pCmdUI->SetCheck(bFavInNewWindow );
//	}catch(...)
//	{
//	}
	
}

void CMainFrame::OnNewwindowHistory() 
{
	// TODO: Add your command handler code here
	if(bHisInNewWindow)
		bHisInNewWindow = FALSE;
	else
		bHisInNewWindow = TRUE;
}

void CMainFrame::OnUpdateNewwindowHistory(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	try{
	pCmdUI->SetCheck(bHisInNewWindow );
	}catch(...)
	{
	}
	
}


void CMainFrame::OnNewwindowFile() 
{
	// TODO: Add your command handler code here
	if(bFileInNewWindow)
		bFileInNewWindow = FALSE;
	else
		bFileInNewWindow = TRUE;
}

void CMainFrame::OnUpdateNewwindowFile(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	try{
	pCmdUI->SetCheck(bFileInNewWindow );
	}catch(...)
	{
	}
}

void CMainFrame::OnFileSendpagebyemail() 
{
	// TODO: Add your command handler code here
	try{

	CMyIEView * pvw = NULL;
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	if (tcf!=NULL)
		pvw = (CMyIEView*)tcf->m_pView;
	if(pvw == NULL)
		return;

	//add more
	IHTMLDocument2 *pHTMLDoc = NULL;
	IPersistFile *pPersistFile = NULL;
	LPDISPATCH pDisp = NULL;

	//save the file to temp
	CString filename;
	CString filename2;
	CString base;
	TCHAR tmp[256];
	BOOL IsLocalFile = FALSE;
	int i;
	try{
	pvw->GetLocationURL(base);
	if(base.Left(5)=="file:")
		IsLocalFile = TRUE;
	if(!IsLocalFile)
		pvw->GetLocationName(filename);
	else
	{
		filename = base;
		filename = filename.Mid(8);
		filename.Replace('/', '\\');
	}
	if(filename.Left(5) =="file:" || filename.Left(5)=="http:")
	{
		if(filename.Right(1) == "/")
			filename = filename.Left(filename.GetLength()-1);

		i = filename.ReverseFind('/');
		filename = filename.Mid(i+1);
		filename.Replace(':', '_');
	}

	filename2 = filename;
	if(!IsLocalFile)
	{
		filename += ".htm0";
		filename2 +=".html";
		GetTempPath(255, tmp);
		filename = tmp + filename;
		filename2 = tmp + filename2;
	}
	i = base.ReverseFind('/');
	base = base.Left(i+1);
	}catch(...)
	{
	}

	if(!IsLocalFile)
	{
		BSTR bfilename = filename.AllocSysString();
		try{
			pDisp = pvw->GetHtmlDocument();
			if( pDisp )
			{
				if (SUCCEEDED(pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc)))
				{
					if (SUCCEEDED(pDisp->QueryInterface(IID_IPersistFile, (void**)&pPersistFile)))
					{
						pPersistFile->Save(bfilename, FALSE);
					}
				}
			}
		}catch(...)
		{
		}


		try{
			SysFreeString(bfilename);
			if(pPersistFile!=NULL)
				pPersistFile->Release();
			if(pHTMLDoc!=NULL)
				pHTMLDoc->Release();
			if(pDisp!=NULL)
				pDisp->Release();
		}
		catch(...)
		{
		}
	}



	//add base url
	if(!IsLocalFile)
	{
		CStdioFile inFile(filename, CFile::modeRead), outFile(filename2, CFile::modeCreate | CFile::modeWrite);

		CString buf;
		outFile.WriteString("<BASE HREF=\"");
		outFile.WriteString(base);
		outFile.WriteString("\">\n");
		while(inFile.ReadString(buf))
		{
			outFile.WriteString(buf);
			outFile.WriteString("\n");
		}

		outFile.Close();
		inFile.Close();
		DeleteFile(filename);
	}

	//send email
	try{
	sDocuments *pMailDocs = new sDocuments;
	strncpy(pMailDocs->filename, (LPCSTR)filename2, 255);
	pMailDocs->IsLocalFile = IsLocalFile;

	AfxBeginThread(SendDocuments, (LPVOID)pMailDocs);
	}catch(...)
	{
	}

	}catch(...)
	{
	}
}

void CMainFrame::OnFileQuicksave() 
{
	// TODO: Add your command handler code here
	try{

	CString filename, root, htmlname, ext;
	CMyIEView * pvw = NULL;
	if(m_bIsAutoSave)
		pvw = m_pCurView;
	else
	{
		CChildFrame* tcf = (CChildFrame*)MDIGetActive();
		if (tcf!=NULL)
			pvw = (CMyIEView*)tcf->m_pView;
	}
	if(pvw == NULL)
		return;
	pvw->GetLocationURL(filename);
	if(filename.Left(5)=="file:")
	{
		pvw->PostMessage(WM_COMMAND, ID_FILE_SAVEAS, 0);
		return;
	}
	
	IHTMLDocument2 * pDoc = NULL;
	IHTMLElementCollection  * pAllElem = NULL;
	IHTMLElement *pElem = NULL;
	IHTMLImgElement * pImg = NULL;
	IHTMLInputImage * pInputImg = NULL;
	IHTMLBodyElement * pBody = NULL;
	IPersistFile *pPersistFile = NULL;
	VARIANT name;
	long p;
	BSTR bstrSrc;
	int l;
	sSaveFiles *SaveFiles = new sSaveFiles;
	CString rt;

	name.vt = VT_I4;

	//save dialog

	filename.Replace('/', '\\');
	if(filename.Right(1) == "\\")
		filename = filename.Left(filename.GetLength()-1);

	l = filename.ReverseFind('\\');
	filename = filename.Mid(l+1);
	filename.Replace(':', '_');
	l = filename.Find('?');
	if(l>0)
		filename = filename.Left(l);

	//auto save
	if(m_bIsAutoSave)
	{
		int l = 0;
		l = filename.ReverseFind('.');
		if(l>0)
		{
			ext = filename.Right(filename.GetLength()-l);
			ext.MakeLower();
			filename = filename.Left(l);
		}
		if(ext != ".htm" && ext != ".html")
			ext = ".htm";

		HFILE hf; OFSTRUCT of; char tmp[4];
		htmlname = filename + ext;
		hf = OpenFile(m_strAutoSavePath + htmlname, &of, OF_EXIST);
		l=0;
		while(hf!=HFILE_ERROR)
		{
			l++;
			htmlname = filename;
			htmlname += '[';
			itoa(l, tmp, 10);
			htmlname += tmp;
			htmlname += ']';
			htmlname += ext;
			hf = OpenFile(m_strAutoSavePath + htmlname, &of, OF_EXIST);
		}
		htmlname = m_strAutoSavePath + htmlname;
		root = htmlname.Left(htmlname.GetLength() - ext.GetLength()); 
	}	
	else
	{
		static char BASED_CODE szFilter[] = "HTML Files (*.htm;*.html)|*.htm;*.html|All Files (*.*)|*.*||";
 		CFileDialog dlg(FALSE, ".htm", filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
		if(m_bIsAutoSave)
			dlg.m_ofn.lpstrInitialDir = m_strAutoSavePath;

		
		if(dlg.DoModal() != IDOK)
			return;

		htmlname = dlg.GetPathName();
		root = dlg.GetPathName();
		ext = dlg.GetFileExt();
		ext.MakeLower();

		if(ext == "htm" || ext == "html")
			root = root.Left(root.GetLength() - ext.GetLength()-1);
		else
			htmlname += ".htm";
	}


	root += "_files\\";
//	CreateDirectory(root, NULL);
	SaveFiles->root = root;
	SaveFiles->filename = htmlname;

	CString base;
	int i;
	pvw->GetLocationURL(base);
	i = base.ReverseFind('/');
	base = base.Left(i+1);
	SaveFiles->base = base;

	pDoc = (IHTMLDocument2*)(pvw->GetHtmlDocument());
	if(pDoc!=NULL)
	{
		pDoc->get_images(&pAllElem);
		if(pAllElem!=NULL)
		{
			pAllElem->get_length(&p);
			if(p>0)
			{
				//save all images
				for(int i=0; i<p; i++)
				{
					name.lVal = i;
					if(pAllElem->item(name, name, (LPDISPATCH*)&pElem)==S_OK)
					{
						if(pElem != NULL)
						{
							pElem->QueryInterface(&pImg);
							if(pImg!=NULL)
							{
								pImg->get_href(&bstrSrc);
								CString s(bstrSrc);
								SysFreeString(bstrSrc);

								if(!SaveFiles->mapDLFiles.Lookup(s, rt))
								{
		//							URLDownloadToCacheFile(NULL, s, f, URLOSTRM_USECACHEDCOPY_ONLY, 0, NULL);
		//							filename = f;
		//							l = filename.ReverseFind('\\');
		//							filename = filename.Mid(l+1);

		//							CopyFile(f, root+filename, FALSE);
									SaveFiles->mapDLFiles.SetAt(s, "");
								}
								pImg->Release();
								pImg = NULL;
							}

							pElem->Release();
							pElem = NULL;
						}
					}
				}
			}
		}

		//get background
		pElem = NULL;
		pDoc->get_body(&pElem);
		if(pElem!=NULL)
		{
			pElem->QueryInterface(&pBody);
			if(pBody!=NULL)
			{
				pBody->get_background(&bstrSrc);
				CString s(bstrSrc);
				SysFreeString(bstrSrc);

				if(!s.IsEmpty())
				{
					if(s.Find(':')<0)
					{
						s = base + s;
					}

					if(!SaveFiles->mapDLFiles.Lookup(s, rt))
					{
	//					URLDownloadToCacheFile(NULL, s, f, URLOSTRM_USECACHEDCOPY_ONLY, 0, NULL);
	//					filename = f;
	//					l = filename.ReverseFind('\\');
	//					filename = filename.Mid(l+1);

	//					CopyFile(f, root+filename, FALSE);

						SaveFiles->mapDLFiles.SetAt(s, "");
					}
				}

				pBody->Release();
			}
			pElem->Release();
		}
					
		//get scripts

		//get style sheet
	}

   
   // Get the HTML.
   //
	BSTR bfilename = htmlname.AllocSysString();
	if (SUCCEEDED(pDoc->QueryInterface(IID_IPersistFile, (void**)&pPersistFile)))
		pPersistFile->Save(bfilename, FALSE);
	SysFreeString(bfilename);

	try{
		if(pPersistFile!=NULL)
			pPersistFile->Release();
		if(pDoc != NULL)
			pDoc->Release();
		if(pAllElem != NULL)
			pAllElem->Release();
	}
	catch(...)
	{
	}


	try{
	AfxBeginThread(QuickSaveDocuments, (LPVOID)SaveFiles);
	}catch(...)
	{
	}


	}catch(...)
	{
	}
//  while HTMLDocument.readyState <> 'complete' do
  //  Application.ProcessMessages;

}

UINT _cdecl SendDocuments(LPVOID file)
{
	if(file == NULL)
		return 0;

	sDocuments *pMailDocs = (sDocuments*)file;
	try{

	HINSTANCE hIns = LoadLibrary("mapi32.dll");
	if(hIns != NULL)
	{
		LPMAPISendDocuments lpMAPISendDocuments = (LPMAPISendDocuments)GetProcAddress(hIns, "MAPISendDocuments");
		if(lpMAPISendDocuments!=NULL)
			lpMAPISendDocuments(0, ";", pMailDocs->filename, "", 0);
		else
			AfxMessageBox("This function need MAPI");
	
	
		FreeLibrary(hIns);
	}
	else
		AfxMessageBox("This function need MAPI");
	if(!pMailDocs->IsLocalFile)
		DeleteFile(pMailDocs->filename);

	}catch(...)
	{
	}

	delete pMailDocs;

	return 0;
}

void CMainFrame::OnFileSendlinkbyemail() 
{
	// TODO: Add your command handler code here
	try{

	CMyIEView * pvw = NULL;
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	if (tcf!=NULL)
		pvw = (CMyIEView*)tcf->m_pView;
	if(pvw == NULL)
		return;
	
	CString mail("mailto:?subject=");

	CString filename;
	CString base;
	int i;
	BOOL IsLocalFile = FALSE;
	try{
	pvw->GetLocationURL(base);
	if(base.Left(5)=="file:")
		IsLocalFile = TRUE;
	if(!IsLocalFile)
		pvw->GetLocationName(filename);
	else
	{
		filename = base;
		filename = filename.Mid(8);
		filename.Replace('/', '\\');
	}
	if(filename.Left(5) =="file:" || filename.Left(5)=="http:")
	{
		if(filename.Right(1) == "/")
			filename = filename.Left(filename.GetLength()-1);

		i = filename.ReverseFind('/');
		filename = filename.Mid(i+1);
		filename.Replace(':', '_');
	}

	if(!IsLocalFile)
	{
		filename += ".htm";
	}
	}catch(...)
	{
	}
	mail += filename;
	mail += "&body=";
	mail += base;
	DefaultMail(mail);

	}catch(...)
	{
	}
}

void CMainFrame::DefaultMail(LPCSTR mailto)
{
	TCHAR           sz[MAX_PATH];
	TCHAR           szPath[MAX_PATH];
	HKEY            hKey;
	DWORD           dwSize = MAX_PATH;

	// find out from the registry where the favorites are located.
	if(RegOpenKey(HKEY_CLASSES_ROOT, _T("mailto\\shell\\"), &hKey) != ERROR_SUCCESS)
	{
		AfxMessageBox("mail client not found\n");
		return ;
	}
	dwSize = sizeof(sz);
	sz[0]=0;
	RegQueryValueEx(hKey, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);
	if(strlen(sz)==0 || dwSize==0)
		strcpy(sz, "open");
	RegCloseKey(hKey);
	CString key = "mailto\\shell\\";
	key += sz;
	key += "\\command";
	if(RegOpenKey(HKEY_CLASSES_ROOT, key, &hKey) == ERROR_SUCCESS)
	{
		//name found, to get the path
		dwSize = MAX_PATH;
		RegQueryValueEx(hKey, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);
		if(strlen(sz)==0)
		{
			AfxMessageBox(IDS_FAIL_EMAIL);
			return;
		}
		ExpandEnvironmentStrings(sz, szPath, MAX_PATH);

		//run
		//AfxMessageBox(szPath);
		char * ch, *ch1 = szPath;
		ch = strchr(szPath, '\"');
		if(ch!=NULL)
		{
			ch1 = ch+1;
			ch = strchr(ch1, '\"');
			if(ch!=NULL)
				*ch ='\0';
		}
		else
		{
			ch = strchr(szPath, ' ');
			if(ch!=NULL)
				*ch = '\0';
		}
		if(ch!=NULL)
			ch++;
		
		CString para(ch),tmp;
		int i = para.Find("%1");
		tmp = para.Right(para.GetLength()-i-2);
		para = para.Left(i);
		para += mailto;
		para += tmp;
		ShellExecute(NULL, "open", ch1 , para, ".", SW_SHOW );

	}
	RegCloseKey(hKey);
}

CString CMainFrame::GetProxyName(CString& proxy)
{
/*
	POSITION pp, pos = m_astrProxy.Find(proxy);
	int index = -1;
	if(pos !=NULL)
	{
		//find index
		for( pp = pos; pp != NULL; )
		{
			m_astrProxy.GetPrev(pp);
			index++;
		}
		//get proxy name
		pos = m_astrProxyName.FindIndex(index);
		return m_astrProxyName.GetAt(pos);
	}
	else
		return proxy;*/

	if(proxy=="")
		return "";

	CProxyList* pl;
	int l;
	for (int i=0;i <= m_aProxyCategories.m_ProxyCategory.GetUpperBound();i++)
	{
	   pl = m_aProxyCategories.m_ProxyCategory.GetAt(i);
	   if((l=pl->Find(proxy))>=0)
	   {
			return pl->m_Proxies.GetAt(l)->m_strProxyName;
	   }
	}
	
	return proxy;
}

void ResolveRelativePath(LPCSTR base, LPCSTR site, CString& path)
{
	if(path.Find(':')<0 && path.Left(2)!="\\\\")
	{
		if(path.Left(1) == "/")
			path = site + path;
		else if(path.Left(3) == "../")
		{
			CString Base(base);
			Base = Base.Left(Base.GetLength()-1);
			int i = Base.ReverseFind('/');
			Base = Base.Left(i+1);
			path = path.Right(path.GetLength()-3);
			path = Base + path;
		}
		else if(path.Left(2) == "./")
		{
			path = path.Right(path.GetLength()-2);
			path = base + path;
		}
		else
			path = base + path;
	}
}

UINT _cdecl QuickSaveDocuments(LPVOID savefiles)
{
	CString filename, root, url, ext, dir, site;
	int l, i;
	sSaveFiles* pSaveFiles = (sSaveFiles*)savefiles;
    DWORD dwEntrySize=0;
    LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry;

	try{


	if(pSaveFiles==NULL )
		return 0;
	if(pSaveFiles->mapDLFiles.IsEmpty())
	{
		delete pSaveFiles;
		return 0;
	}


	root = pSaveFiles->root;
	CreateDirectory(root, NULL);

	dir = root.Left(root.GetLength()-1);
	l = dir.ReverseFind('\\');
	dir = dir.Mid(l+1);
	dir += '/';

	site = pSaveFiles->base;
	l = -1;
	for(i=0; i<3; i++)
		l = site.Find('/', l+1);
	site = site.Left(l);

	CString downloadfrom("<!--Download from ");
	downloadfrom += pSaveFiles->base;
	downloadfrom += " -->\n";

	//get input images and scripts
	CSimpleSAH sah;
	CString attri, val;
	CStringList tags;
	tags.AddTail("input");
	tags.AddTail("script");
	sah.LoadHTML(pSaveFiles->filename, FALSE);
	while(sah.GetNextTags(tags))
	{
		attri = sah.GetAttribute("src");
		if(!attri.IsEmpty())
		{
			ResolveRelativePath(pSaveFiles->base, site, attri);
			if(!pSaveFiles->mapDLFiles.Lookup(attri, val))
				pSaveFiles->mapDLFiles.SetAt(attri, "");
		}
	}
	sah.Close();

	//get table background
	tags.RemoveAll();
	tags.AddTail("table");
	tags.AddTail("tr");
	tags.AddTail("td");
	sah.LoadHTML(pSaveFiles->filename, FALSE);
	while(sah.GetNextTags(tags))
	{
		attri = sah.GetAttribute("background");
		if(!attri.IsEmpty())
		{
			ResolveRelativePath(pSaveFiles->base, site, attri);
			if(!pSaveFiles->mapDLFiles.Lookup(attri, val))
				pSaveFiles->mapDLFiles.SetAt(attri, "");
		}
	}
	sah.Close();

	//get external CSS
	tags.RemoveAll();
	tags.AddTail("link");
	sah.LoadHTML(pSaveFiles->filename, FALSE);
	while(sah.GetNextTags(tags))
	{
		attri = sah.GetAttribute("href");
		if(!attri.IsEmpty())
		{
			ext = attri.Right(4);
			ext.MakeLower();
			if( ext == ".css")
			{
				ResolveRelativePath(pSaveFiles->base, site, attri);
				if(!pSaveFiles->mapDLFiles.Lookup(attri, val))
					pSaveFiles->mapDLFiles.SetAt(attri, "");
			}
		}
	}
	sah.Close();


	POSITION pos = pSaveFiles->mapDLFiles.GetStartPosition();
	//save all component files
	while(pos!=NULL)
	{
		pSaveFiles->mapDLFiles.GetNextAssoc(pos, url, ext);

//		URLDownloadToCacheFile(NULL, url, f, URLOSTRM_USECACHEDCOPY_ONLY, 0, NULL);
//		filename = f;
//		l = filename.ReverseFind('\\');
//		filename = filename.Mid(l+1);

		if (!GetUrlCacheEntryInfo(url,NULL,&dwEntrySize))
        {
			
           if (GetLastError()==ERROR_INSUFFICIENT_BUFFER)
           {
                        
			   lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) 
											new char[dwEntrySize];

				if (GetUrlCacheEntryInfo(url,lpCacheEntry,&dwEntrySize))
				{
				//	filename = lpCacheEntry->lpszLocalFileName;
					filename = url;
					filename.Replace('/', '\\');
					if(!filename.IsEmpty())
					{
						l = filename.ReverseFind('\\');
						filename = filename.Mid(l+1);
						//fix filename, remove [x]
					/*	l = filename.ReverseFind('[');
						i = filename.ReverseFind(']');
						ext = filename.Mid(i+1);
						filename = filename.Left(l);
						filename += ext;*/

						CopyFile(lpCacheEntry->lpszLocalFileName, root+filename, FALSE);

						pSaveFiles->mapDLFiles.SetAt(url, filename);
					}
				}

				delete lpCacheEntry;
				lpCacheEntry = NULL;
				dwEntrySize = 0;
		   }
		}
	}

	//save html file
	sah.LoadHTML(pSaveFiles->filename);
	//add download from
	sah.AddText(downloadfrom);

	tags.RemoveAll();
	tags.AddTail("img");
	tags.AddTail("input");
	tags.AddTail("script");
	tags.AddTail("image");
	while(sah.GetNextTags(tags))
	{
		attri = sah.GetAttribute("src");
		if(!attri.IsEmpty())
		{
			ResolveRelativePath(pSaveFiles->base, site, attri);
			if(pSaveFiles->mapDLFiles.Lookup(attri, val))
				sah.SetAttribute("src", dir+val);
		}
	}
	sah.Close();
	//background
	sah.LoadHTML(pSaveFiles->filename);
	tags.RemoveAll();
	tags.AddTail("body");
	tags.AddTail("table");
	tags.AddTail("tr");
	tags.AddTail("td");
	while(sah.GetNextTags(tags))
	{
		attri = sah.GetAttribute("background");
		ResolveRelativePath(pSaveFiles->base, site, attri);
		if(pSaveFiles->mapDLFiles.Lookup(attri, val))
			sah.SetAttribute("background", dir+val);
	}
	sah.Close();

	//CSS
	sah.LoadHTML(pSaveFiles->filename);
	tags.RemoveAll();
	tags.AddTail("link");
	while(sah.GetNextTags(tags))
	{
		attri = sah.GetAttribute("href");
		ResolveRelativePath(pSaveFiles->base, site, attri);
		if(pSaveFiles->mapDLFiles.Lookup(attri, val))
			sah.SetAttribute("href", dir+val);
	}
	sah.Close();


	}catch(...)
	{
	}

	delete pSaveFiles;
	return 0;
}


void CMainFrame::OnToolsMem() 
{
	// TODO: Add your command handler code here
	m_bDispMem = !m_bDispMem;
	//2.4
	if(m_bDispMem)
		m_wndStatusBar.SetPaneInfo(6, ID_INDICATOR_RES, SBPS_NORMAL, m_SBMemWidth);
	else
		m_wndStatusBar.SetPaneInfo(6, ID_INDICATOR_RES, SBPS_NORMAL, 0);

	m_wndStatusBar.AdjustToolbarPosition();
	m_wndStatusBar.AdjustProgressBarPosition();
}

void CMainFrame::OnUpdateToolsMem(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bDispMem);
	
}

void CMainFrame::LaunchEmail(LPCSTR mailclient)
{
	HKEY hKey;
	DWORD dwSize;
	TCHAR           sz[MAX_PATH];
	TCHAR           szPath[MAX_PATH];
	
		CString key = "SOFTWARE\\Clients\\Mail\\";
		key += mailclient;
		key += "\\Shell\\open\\command";
		if(RegOpenKey(HKEY_LOCAL_MACHINE, key, &hKey) == ERROR_SUCCESS)
		{
			//name found, to get the path
			dwSize = MAX_PATH;
			RegQueryValueEx(hKey, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);
			if(strlen(sz)==0)
			{
				AfxMessageBox(IDS_FAIL_EMAIL);
				return;
			}
			ExpandEnvironmentStrings(sz, szPath, MAX_PATH);

			//run
			//AfxMessageBox(szPath);
			char * ch, *ch1 = szPath;
			ch = strchr(szPath, '\"');
			if(ch==szPath)
			{
				ch1 = ch+1;
				ch = strchr(ch1, '\"');
				if(ch!=NULL)
					*ch ='\0';
			}
			else
			{
				ch = strchr(szPath, ' ');
				if(ch!=NULL)
					*ch = '\0';
			}
			if(ch!=NULL)
				ch++;
			ShellExecute(NULL, "open", ch1 , ch, ".", SW_SHOW );

			RegCloseKey(hKey);
		}
}

void CMainFrame::LaunchEditor(LPCSTR editor, LPCSTR file)
{
	static TCHAR           szPath[MAX_PATH];
	CString url(file);

	int i = url.ReverseFind('#');
	if(i>0)
		url = url.Left(i);

	ExpandEnvironmentStrings(editor, szPath, MAX_PATH);

		//run
		char * ch, *ch1 = szPath;
		ch = strchr(szPath, '\"');
		if(ch==szPath)
		{
			ch1 = ch+1;
			ch = strchr(ch1, '\"');
			if(ch!=NULL)
				*ch ='\0';
		}
		else
		{
			ch = strchr(szPath, ' ');
			if(ch!=NULL)
				*ch = '\0';
		}
		if(ch!=NULL)
			ch++;

		//AfxMessageBox(ch1);
		//AfxMessageBox(ch);
		if(url.Left(5) == "file:")
		{
			url = url.Mid(8);
			url.Replace('/', '\\');
			url.Replace("%20", " ");
		}
		else
		{
			_strupr(ch1);
			if(strstr(ch1, "WORD.EXE")==NULL && strstr(ch1, "DREAMWEAVER.EXE")==NULL && strstr(ch1, "FRONTPAGE")==NULL)
			{
				//get cache file path
				DWORD dwEntrySize=0;
				LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry;
				if (!GetUrlCacheEntryInfo(url,NULL,&dwEntrySize))
				{
					
				   if (GetLastError()==ERROR_INSUFFICIENT_BUFFER)
				   {
                        
					   lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) 
													new char[dwEntrySize];

						if (GetUrlCacheEntryInfo(url,lpCacheEntry,&dwEntrySize))
						{
							url = lpCacheEntry->lpszLocalFileName;

						}

						delete lpCacheEntry;
						lpCacheEntry = NULL;
				   }
				}
			}
		}

		url = "\""+url;
		url += "\"";

		ShellExecute(NULL, "open", ch1 , url, NULL, SW_SHOW );

}

void CMainFrame::UpdateUndoMenu()
{
	try{
		//update menu items
		CMenu * pmenu = GetMenu()->GetSubMenu(1)->GetSubMenu(0);
		while(pmenu->DeleteMenu(2, MF_BYPOSITION));

		int nCur = (undoEnd+15)%16;
		CString str;
		while(undoEnd != undoStart)
		{
			str = m_strUndoName[nCur];
			str = str.Left(40);
			pmenu->AppendMenu(MF_ENABLED, 0x6200+nCur, str);
			if(nCur==undoStart)
				break;
			nCur = (nCur+15)%16;
		}
	}catch(...)
	{
	}
}

void CMainFrame::OnUndoList(UINT nID)
{
	int i = nID - 0x6200, c = (i+1)%16;
	NewChildWindow(1,2,m_strUndoURL[i]);
	while(c!=undoEnd)
	{
		m_strUndoURL[i] = m_strUndoURL[c];
		m_strUndoName[i] = m_strUndoName[c];
		i = c;
		c = (c+1)%16;
	}
	undoEnd = i;
	UpdateUndoMenu();
}

void CMainFrame::OnRtabAutoref() 
{
	// TODO: Add your command handler code here
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;

	m_wndTab.GetItem(nRTabID, &TabCtrlItem);
	CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
	
	if(tcf!=NULL && tcf->m_pView!=NULL)
	{
		tcf->m_pView->SendMessage(WM_COMMAND, ID_TOOLS_AUTOREF, 0);
	}	
}

void CMainFrame::OnUpdateRtabAutoref(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;

	m_wndTab.GetItem(nRTabID, &TabCtrlItem);
	CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
	
	if(tcf!=NULL && tcf->m_pView!=NULL)
	{
		pCmdUI->SetCheck(tcf->m_pView->m_bUseAutoRef);

		static char sec[10];
		if(pCmdUI->m_pMenu!=NULL)
		{
			itoa(tcf->m_pView->m_nRefreshInterval, sec, 10);
			CString menu;
			pCmdUI->m_pMenu->GetMenuString(pCmdUI->m_nID, menu, MF_BYCOMMAND);
			int i = menu.Find('(');
			if(i>0)
				menu = menu.Left(i);
			menu += "(";
			menu += sec;
			menu += ")";
			pCmdUI->SetText(menu);
		}
	}
}

void CMainFrame::OnRtabReffre() 
{
	// TODO: Add your command handler code here
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;

	m_wndTab.GetItem(nRTabID, &TabCtrlItem);
	CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
	
	if(tcf!=NULL && tcf->m_pView!=NULL)
	{
		tcf->m_pView->SendMessage(WM_COMMAND, ID_TOOLS_REFFRE, 0);
	}	
}

void CMainFrame::OnViewLock() 
{
	// TODO: Add your command handler code here
	CMyIEView * pvw = NULL;
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	if (tcf!=NULL && tcf->m_pView!=NULL)
	{
		tcf->bLock =!tcf->bLock;

		if(tcf->m_pView->m_nProgress<0)
		{
			int nID = FindTab(tcf);
			if(tcf->bLock)
				tcf->m_pView->oldIcon = SetTabIcon( 4, nID );
			else
				tcf->m_pView->oldIcon = SetTabIcon( -1, nID );

			//update tab bar
			if(m_bFBTab)
				PostMessage(WM_UPDATE_TAB, FALSE);
		}
	}	
}

int CMainFrame::FindDupURL(LPCSTR lpszURL, CChildFrame* pSelf)
{
	int n = nWin; //m_wndTab.GetItemCount();
	BOOL found = FALSE;
	int i=0;
	TCITEM TabCtrlItem;
	int l=-1;

	try{

	if(!EnableDupFilter)
		return l;

	while (i<n && !found)
	{
			TabCtrlItem.mask = TCIF_PARAM;
			TabCtrlItem.lParam = 0;
			m_wndTab.GetItem(i, &TabCtrlItem);
			if(TabCtrlItem.lParam != NULL && (CChildFrame *)TabCtrlItem.lParam != pSelf)
			{	
					//((CChildFrame *)TabCtrlItem.lParam)->GetWindowText(str);
					if(((CChildFrame *)TabCtrlItem.lParam)->m_pView->m_lpszUrl == lpszURL)
					{
						found = TRUE;
						l=i;
					}
					else
						i++;
			}
			else
				i++;
	}
	}
	catch(...)
	{
	}

	return l;
}

//close external programs
/*  typedef struct
   {
      DWORD   dwID ;
      DWORD   dwThread ;
   } TERMINFO ;

   BOOL CALLBACK Terminate16AppEnum( HWND hwnd, LPARAM lParam )
   {
      DWORD      dwID ;
      DWORD      dwThread ;
      TERMINFO   *termInfo ;

      termInfo = (TERMINFO *)lParam ;

      dwThread = GetWindowThreadProcessId(hwnd, &dwID) ;

      if(dwID == termInfo->dwID && termInfo->dwThread == dwThread )
      {
         PostMessage(hwnd, WM_CLOSE, 0, 0) ;
      }

      return TRUE ;
   }  
   
   DWORD WINAPI Terminate16App( DWORD dwPID, DWORD dwThread,
                        WORD w16Task, DWORD dwTimeout )
   {
      HINSTANCE      hInstLib ;
      TERMINFO      info ;

      // You will be calling the functions through explicit linking
      // so that this code will be binary compatible across
      // Win32 platforms.
      BOOL (WINAPI *lpfVDMTerminateTaskWOW)(DWORD dwProcessId,
         WORD htask) ;

      hInstLib = LoadLibraryA( "VDMDBG.DLL" ) ;
      if( hInstLib == NULL )
         return 1 ;

      // Get procedure addresses.
      lpfVDMTerminateTaskWOW = (BOOL (WINAPI *)(DWORD, WORD ))
         GetProcAddress( hInstLib, "VDMTerminateTaskWOW" ) ;

      if( lpfVDMTerminateTaskWOW == NULL )
      {
         FreeLibrary( hInstLib ) ;
         return 1 ;
      }

      // Post a WM_CLOSE to all windows that match the ID and the
      // thread.
      info.dwID = dwPID ;
      info.dwThread = dwThread ;
      EnumWindows((WNDENUMPROC)Terminate16AppEnum, (LPARAM) &info) ;

      // Wait.
 /*     Sleep( dwTimeout ) ;

      // Then terminate.
      lpfVDMTerminateTaskWOW(dwPID, w16Task) ;

      FreeLibrary( hInstLib ) ;* /
      return 0 ;
   }

*/



BOOL CALLBACK TerminateAppEnum( HWND hwnd, LPARAM lParam )
   {
      DWORD dwID ;

      // If we can't open the process with PROCESS_TERMINATE rights,
      // then we give up immediately.

      GetWindowThreadProcessId(hwnd, &dwID) ;

  	  if(dwID == (DWORD)lParam)
			PostMessage(hwnd, WM_CLOSE, 0, 0) ;

      return TRUE ;
   }

DWORD WINAPI TerminateApp( DWORD dwPID, DWORD dwTimeout )
   {
      DWORD   dwRet = 0;

	  HANDLE hProc = OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE, FALSE,
         dwPID);

      if(hProc == NULL)
      {
         return 1 ;
      }

      // TerminateAppEnum() posts WM_CLOSE to all windows whose PID
      // matches your process's.
      EnumWindows((WNDENUMPROC)TerminateAppEnum, (LPARAM) dwPID) ;

      // Wait on the handle. If it signals, great. If it times out,
      // then you kill it.
/*      if(WaitForSingleObject(hProc, dwTimeout)!=WAIT_OBJECT_0)
         dwRet=(TerminateProcess(hProc,0)?TA_SUCCESS_KILL:TA_FAILED);
      else
         dwRet = TA_SUCCESS_CLEAN ;
*/
      CloseHandle(hProc) ;

      return dwRet ;
   }


void CMainFrame::CloseExternalUtil()
{
	try{

	CExternalUtilItem* eui;
	for (int i=0;i <= m_ExternalUtilList.m_UtilList.GetUpperBound();i++)
	{
		eui = m_ExternalUtilList.m_UtilList.GetAt(i);
		if(eui!=NULL)
		{
			if(eui->m_iProcess!=0 && eui->m_bUtilClose)
			{
				TerminateApp(eui->m_iProcess, 500);
			}
		}
	}

	}catch(...)
	{
	}
}

void CMainFrame::StartUtil(int i)
{
	CExternalUtilItem* eui = m_ExternalUtilList.m_UtilList.GetAt(i);

	if(eui!=NULL)
	{
		CString cmd;
		cmd = eui->m_strUtilCmd;

		char *ch1 = cmd.GetBuffer(64); //* ch, 
/*		char * szPath = ch1;
		ch = strchr(szPath, '\"');
		if(ch!=NULL)
		{
			ch1 = ch+1;
			ch = strchr(ch1, '\"');
			if(ch!=NULL)
				*ch ='\0';
		}
		else
		{
			ch = strchr(szPath, ' ');
			if(ch!=NULL)
				*ch = '\0';
		}
		if(ch!=NULL)
			ch++;
*/

	/*	SHELLEXECUTEINFO ShExecInfo;

						ZeroMemory ( &ShExecInfo, sizeof ( SHELLEXECUTEINFO)); 
						ShExecInfo.cbSize = sizeof ( SHELLEXECUTEINFO); 
						  ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS; 
						  ShExecInfo.hwnd = NULL; 
						  ShExecInfo.lpVerb = "open"; 
						  ShExecInfo.lpFile = ch1;
						  ShExecInfo.lpParameters = ch; 
						  ShExecInfo.lpDirectory = "."; 
						  ShExecInfo.nShow = SW_SHOW; 
						  ShExecInfo.hInstApp = NULL; 
						  ShellExecuteEx ( &ShExecInfo); 

						eui->m_hProcess = ShExecInfo.hProcess;*/

						//if((UINT)ShellExecute(NULL, "open", ch1 , ch, ".", SW_SHOW )<=32)
						//	AfxMessageBox(IDS_UTIL_FAIL);
						

		STARTUPINFO si; 
		PROCESS_INFORMATION pi; 

		ZeroMemory ( &si, sizeof ( STARTUPINFO)); 

		si.cb = sizeof ( STARTUPINFO); 
		si.dwFlags = STARTF_USESHOWWINDOW; 
		si.wShowWindow = SW_SHOWNORMAL; // use SW_SHOWNORMAL if you want the spawned app to be visible 

		BOOL bRes = CreateProcess ( NULL, 
				ch1, // command line 
				NULL, 
				NULL, 
				TRUE, 
				NORMAL_PRIORITY_CLASS, 
				GetEnvironmentStrings (), 
				NULL, 
				&si, 
				&pi 
				);

		cmd.ReleaseBuffer();

		CloseHandle( pi.hProcess); 
		CloseHandle( pi.hThread); 

		eui->m_iProcess = pi.dwProcessId;
	}
}

BOOL CMainFrame::IsBrowsing()
{
	BOOL ib = FALSE;
	try{
	CChildFrame* tcf;
	UINT n = m_wndTab.GetItemCount();
	nWin = n;
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;

	for(UINT i=0;i<n;i++)
	{
		
		tcf = NULL;
		TabCtrlItem.lParam = NULL;
		m_wndTab.GetItem(i, &TabCtrlItem);
		tcf = ((CChildFrame*)TabCtrlItem.lParam);
		if(tcf != NULL && tcf->m_pView!=NULL)
		{
			if(tcf->m_pView->m_nProgress>0 && tcf->m_pView->m_nProgress<100)
			{
				ib=TRUE;
				break;
			}
		}

	}
	}catch(...)
	{
	}

	return ib;
}


void CMainFrame::OnScFillform() 
{
	// TODO: Add your command handler code here
	try{
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	if (tcf!=NULL && tcf->m_pView!=NULL)
	{
		CString filename = ((CMyIEApp*)AfxGetApp())->m_strFormDataPath;
		CMyIEView* pvw = tcf->m_pView;
		IHTMLDocument2 * pDoc = NULL;
		IHTMLElementCollection  * pAllElem = NULL;
		IHTMLElement *pElem = NULL;
		IHTMLInputTextElement * pInput = NULL;
		VARIANT name;
		long p;
		BSTR bstrSrc;
		CString rt, env;
		char buf[2048];

		name.vt = VT_I4;

		CString base;
		pvw->GetLocationURL(base);
		if(base.IsEmpty())
			return;

		p = GetPrivateProfileSection(base, buf, 20, filename);
		if(p==0)
			base = "General";

		pDoc = (IHTMLDocument2*)(pvw->GetHtmlDocument());
		if(pDoc!=NULL)
		{
			pDoc->get_all(&pAllElem);
			if(pAllElem!=NULL)
			{
				pAllElem->get_length(&p);
				if(p>0)
				{
					//get all input
					for(int i=0; i<p; i++)
					{
						name.lVal = i;
						if(pAllElem->item(name, name, (LPDISPATCH*)&pElem)==S_OK)
						{
							if(pElem != NULL)
							{
								pElem->QueryInterface(&pInput);
								if(pInput!=NULL)
								{
									pInput->get_name(&bstrSrc);
									CString s(bstrSrc);
									SysFreeString(bstrSrc);

									GetPrivateProfileString(base, s, _T(""), buf, 2048, filename);
									Decoding(buf, env);
	
									bstrSrc = env.AllocSysString();
									pInput->put_value(bstrSrc);
									SysFreeString(bstrSrc);


									pInput->Release();
									pInput = NULL;
								}

								pElem->Release();
								pElem = NULL;
							}
						}
					}
				}
				pAllElem->Release();
			}
			pDoc->Release();
		}
	}

	}catch(...)
	{
	}	
}

void CMainFrame::OnScSaveform() 
{
	// TODO: Add your command handler code here
	SaveForm(FALSE);
}

void CMainFrame::OnScSavegenform() 
{
	// TODO: Add your command handler code here
	SaveForm(TRUE);
}

void CMainFrame::SaveForm(BOOL bIsGeneral)
{
	try{
	int inputs = 0;
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	if (tcf!=NULL && tcf->m_pView!=NULL)
	{
		CString filename = ((CMyIEApp*)AfxGetApp())->m_strFormDataPath;
		CMyIEView* pvw = tcf->m_pView;
		IHTMLDocument2 * pDoc = NULL;
		IHTMLElementCollection  * pAllElem = NULL;
		IHTMLElement *pElem = NULL;
		IHTMLInputTextElement * pInput = NULL;
		VARIANT name;
		long p;
		BSTR bstrSrc;
		CString rt, env;

		name.vt = VT_I4;

		CString base;
		if(bIsGeneral)
			base = "General";
		else
		{
			pvw->GetLocationURL(base);
			if(base.IsEmpty())
				return;
			WritePrivateProfileSection(base, NULL, filename);
		}

		pDoc = (IHTMLDocument2*)(pvw->GetHtmlDocument());
		if(pDoc!=NULL)
		{
			pDoc->get_all(&pAllElem);
			if(pAllElem!=NULL)
			{
				pAllElem->get_length(&p);
				if(p>0)
				{
					//get all input
					for(int i=0; i<p; i++)
					{
						name.lVal = i;
						if(pAllElem->item(name, name, (LPDISPATCH*)&pElem)==S_OK)
						{
							if(pElem != NULL)
							{
								pElem->QueryInterface(&pInput);
								if(pInput!=NULL)
								{
									pInput->get_name(&bstrSrc);
									CString s(bstrSrc);
									SysFreeString(bstrSrc);

									pInput->get_value(&bstrSrc);
									CString value(bstrSrc);
									SysFreeString(bstrSrc);

									Encoding(value, env);
									WritePrivateProfileString(base, s, env, filename);
									inputs++;

									pInput->Release();
									pInput = NULL;
								}

								pElem->Release();
								pElem = NULL;
							}
						}
					}
				}
				pAllElem->Release();

				if(inputs>0)
					AfxMessageBox(IDS_SAVEDFORM);
			}
			pDoc->Release();
		}
	}

	}catch(...)
	{
	}	
}

/*
BOOL CMainFrame::GetCurSecureInfo()
{
	BOOL secure = FALSE;
	try{

	//security flag
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	if (tcf!=NULL && tcf->m_pView!=NULL)
	{
		if(!tcf->m_pView->m_bToClose)
		{
			if(tcf->m_pView->m_lpszUrl.Left(5)=="https")
			{
				LPDISPATCH pDisp = NULL;

				IHTMLDocument2 *pHTMLDoc = NULL;
				BSTR p;

				try{
				pDisp = tcf->m_pView->GetHtmlDocument();
				if( pDisp )
				{
					if (SUCCEEDED(pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc)))
					{
						if(SUCCEEDED(pHTMLDoc->get_security( &p )))
						{
							CString s(p);
	AfxMessageBox(s);
							SysFreeString(p);
						}
					}
				}
				}
				catch(...)
				{
				}

				try{
					if(pHTMLDoc!=NULL)
						pHTMLDoc->Release();
					if(pDisp!=NULL)
						pDisp->Release();
				}
				catch(...)
				{
				}
			}
		}
	}

	}catch(...)
	{
	}

	return secure;
}
*/


void CMainFrame::OnToolsAutosetbg() 
{
	// TODO: Add your command handler code here
	bAutoSetBG = !bAutoSetBG;
}

void CMainFrame::OnUpdateToolsAutosetbg(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bAutoSetBG);
}

void CMainFrame::ChangTabPos(int npos)
{
/*3.2	int r = m_wndTab.GetRowCount();
	if(r==0)
		r=1;
	m_nPreLines = r;
*/
		REBARBANDINFO rbbi;
		// set up min/max sizes and ideal sizes for pieces of the rebar
//3.2		CRect rectAddress;

		//m_wndAddress->GetEditCtrl()->GetWindowRect(&rectAddress);
//3.2		m_wndTab.GetWindowRect(&rectAddress);

		rbbi.cbSize = sizeof(rbbi);
		rbbi.fMask = RBBIM_ID;//RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE |
		rbbi.wID = ID_VIEW_TASKBAR;
/*3.2		rbbi.cxMinChild = 10;
		if(m_bFBTab)
			if(r>1)
				rbbi.cyMinChild = (rectAddress.Height() + 10)*r;
			else
				rbbi.cyMinChild = rectAddress.Height()+6;
		else
			rbbi.cyMinChild = rectAddress.Height();
		rbbi.cxIdeal = 200;*/

	if(m_bTopTab ==1)
	{
		//will move to bottom
		m_wndTab.ModifyStyle(0, TCS_BOTTOM);
		//AfxMessageBox(IDS_CHANGE_POS);
		//deleete the band in top bar
		int nb = m_wndReBar.GetReBarCtrl( ).IDToIndex(ID_VIEW_TASKBAR);
		if(nb<0)
			return;

		m_wndReBar.GetReBarCtrl( ).DeleteBand(nb);

		m_wndReBar2.AddBar(&m_wndTab);
	//	GetMenu()->CheckMenuItem(ID_VIEW_TASKBAR, MF_CHECKED);

		if(!m_bFBTab)
		{
			rbbi.fMask |=  RBBIM_STYLE;
			rbbi.fStyle = RBBS_FIXEDBMP | RBBS_NOGRIPPER; 
		}
		m_wndReBar2.GetReBarCtrl().SetBandInfo(0, &rbbi);
	}
	else if(m_bTopTab==0)
	{//AfxMessageBox(IDS_CHANGE_POS);

		//will move to top
		m_wndTab.ModifyStyle(TCS_BOTTOM,0);

		//hide the bottom bar
		m_wndReBar2.GetReBarCtrl( ).DeleteBand(0);

		m_wndReBar.AddBar(&m_wndTab,NULL, NULL,  RBBS_FIXEDBMP | RBBS_BREAK |RBBS_GRIPPERALWAYS);
	//	GetMenu()->CheckMenuItem(ID_VIEW_TASKBAR, MF_CHECKED);

		if(m_bLockToolbar)
		{
			rbbi.fMask |=  RBBIM_STYLE;
			rbbi.fStyle = RBBS_FIXEDBMP | RBBS_BREAK |RBBS_NOGRIPPER; 
		}

		m_wndReBar.GetReBarCtrl().SetBandInfo(7, &rbbi);
		ShowBackground(TRUE);
	}
}

void CMainFrame::OnViewEncodingAuto() 
{
	// TODO: Add your command handler code here
	//Get global offline setting
	DWORD           sz;
	//TCHAR           szPath[MAX_PATH];
	HKEY            hKey;
	DWORD           dwSize = sizeof(DWORD);

	if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\International"), &hKey) != ERROR_SUCCESS)
	{
		TRACE0("Global settings not found\n");
		return ;
	}

	if(RegQueryValueEx(hKey, _T("AutoDetect"), NULL, NULL, (LPBYTE)&sz, &dwSize) == ERROR_SUCCESS)
	{
		//set new offline value
		if(sz != 0)
			sz = 0;
		else
			sz = 1;
		RegSetValueEx(hKey, _T("AutoDetect"), NULL, REG_DWORD, (LPBYTE)(&sz), dwSize);
		
		CMyIEView * pvw = NULL;
		CChildFrame* tcf = (CChildFrame*)MDIGetActive();
		if (tcf!=NULL)
			pvw = (CMyIEView*)tcf->m_pView;
		if(pvw != NULL)
			pvw->Refresh2(1);

	}
	RegCloseKey(hKey);

}

void CMainFrame::OnUpdateViewEncodingAuto(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DWORD           sz;
	//TCHAR           szPath[MAX_PATH];
	HKEY            hKey;
	DWORD           dwSize = sizeof(DWORD);

	if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\International"), &hKey) != ERROR_SUCCESS)
	{
		TRACE0("Global settings not found\n");
		return ;
	}

	if(RegQueryValueEx(hKey, _T("AutoDetect"), NULL, NULL, (LPBYTE)&sz, &dwSize) == ERROR_SUCCESS)
	{
		//set new offline value
		pCmdUI->SetCheck(sz!=0);
	}
	RegCloseKey(hKey);	
}

void CMainFrame::OnSaveasGroup() 
{
	// TODO: Add your command handler code here
	nWin=m_wndTab.GetItemCount();
	if(nWin==0)
		return;

	CString filename;
	static char BASED_CODE szcFilter[] = "Group Files (*.cgp)|*.cgp|";
 	CFileDialog dlg(FALSE, ".cgp", filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szcFilter);
	dlg.m_ofn.lpstrInitialDir = ((CMyIEApp*)AfxGetApp())->m_strGroupPath;
	if(dlg.DoModal() != IDOK)
		return;
	filename = dlg.GetPathName();
	CString ext = dlg.GetFileExt();
	if(ext!="cgp")
		filename += ".cgp";

	CChildFrame* tcf;
	int n = nWin;
	TCHAR strm[8] = "blank";
	TCITEM TabCtrlItem;

	WritePrivateProfileSection("CaptorGroup", NULL, filename);
	TabCtrlItem.mask = TCIF_PARAM;
	for(int i=0;i<n;i++)
	{
		TabCtrlItem.lParam = 0;
		m_wndTab.GetItem(i, &TabCtrlItem);
		tcf = ((CChildFrame*)TabCtrlItem.lParam);
		if(tcf != NULL && tcf->m_pView != NULL)
		{
			itoa(i+1, strm+5, 10);
			WritePrivateProfileString("CaptorGroup", strm, tcf->m_pView->m_lpszUrl, filename);
		}
	}
	InitGroups();

}

void CMainFrame::OnGroup() 
{
	// TODO: Add your command handler code here
	CMenu* pm = GetMenu()->GetSubMenu(4)->GetSubMenu(0);
	CRect rect;
	CPoint pt;
	GetCursorPos(&pt);
	m_wndToolBar.ScreenToClient(&pt);
	int i = m_wndToolBar.GetToolBarCtrl().HitTest(&pt);
	m_wndToolBar.GetItemRect(i, &rect);
	rect.top = rect.bottom;
	m_wndToolBar.ClientToScreen(&rect);

	pm->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON , rect.left, rect.top + 1, this);
}


void CMainFrame::GetHomePage(LPTSTR homepage)
{
	HKEY            hKey;
	DWORD           dwSize = MAX_PATH;
	try{
	if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\Main"), &hKey) != ERROR_SUCCESS)
	{
		TRACE0("Main setting not found\n");
	}
	else
	{
		RegQueryValueEx(hKey, _T("Start Page"), NULL, NULL, (LPBYTE)homepage, &dwSize);

		RegCloseKey(hKey);
	}
	}catch(...)
	{
	}
}

void CMainFrame::SetHomePage(LPCTSTR homepage)
{
	HKEY            hKey;

	try{

	if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\Main"), &hKey) != ERROR_SUCCESS)
	{
		TRACE0("Typed URLs not found\n");
	}
	else
	{
			RegSetValueEx(hKey, _T("Start Page"), NULL, REG_SZ, (LPBYTE)(homepage), strlen(homepage));
			RegCloseKey(hKey);
	}

	}catch(...)
	{
	}
}


void CMainFrame::OnUpdateFileOpenf(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
			if(m_bSecure)
				pCmdUI->Enable(!m_bSecure);
	
}

void CMainFrame::OnToolCleanmyieform() 
{
	// TODO: Add your command handler code here
	DeleteFile(((CMyIEApp*)AfxGetApp())->m_strFormDataPath);
}


void CMainFrame::OnDelayLoadConfig(WPARAM wParam,LPARAM)
{
	CMyIEApp* app = (CMyIEApp*)AfxGetApp();
	BOOL r = TRUE;
	int i=0;

	//load proxy lists
	m_aProxyCategories.LoadProxies();

	CMenu* pMenu = GetMenu()->GetSubMenu(5)->GetSubMenu(2);
	BuildWebProxyMenu(pMenu, 1); //translate
	pMenu = GetMenu()->GetSubMenu(5)->GetSubMenu(3);
	BuildWebProxyMenu(pMenu, 0); //web proxy

	//load popup filter list
//	UINT len;
	char *tmp = NULL;
	CString pop = app->GetProfileString("Popups", "popup1", NULL);
	if(pop == "" || pop.IsEmpty())
	{
/*3.2		r = app->GetProfileBinary("Settings","Popups", (LPBYTE*)(&tmp), &len);
		if(r)
		{
			//load pop list
			char *token;
			char seps[] = "\n";
			token = strtok( tmp, seps);
			while( token != NULL )
			{
				  /* While there are tokens in "string" * /
				  m_astrPopup.AddTail(token);
				  /* Get next token: * /
				  token = strtok( NULL, seps );
			}
			delete[] tmp;

		}*/
	}
	else
	{
		//load new one
		char Keyname[12] = "popup";
		int i=1;
		while(pop != "" && !pop.IsEmpty())
		{
			m_astrPopup.AddTail(pop);
			i++;
			itoa(i, Keyname+5, 10);
			pop = app->GetProfileString("Popups", Keyname, NULL);
		}
	}

	//load url filter
	TCHAR buf[1024];
	DWORD rlens;
	i = 0;
	char Keyname[12] = "url";
	CString filename = app->m_strMyIERoot + "\\urlfilter.ini";
	r = TRUE;
	while(r)
	{
		itoa(i, Keyname+3, 10);
		rlens = ::GetPrivateProfileString("Filter", Keyname, _T(""), buf, 1024,
									  filename);
		if(rlens==0)
			r=FALSE;
		else
		{
			pop = buf;
			pop.TrimLeft();
			pop.TrimRight();
			m_astrUrlFilter.Add(pop);
		}
		i++;
	}
	m_bUrlFilterDirty = TRUE;
	InitialUrlFilter();

	TCHAR key[10];
	CString url, alias;
	r = TRUE;
	i = 0;
	//2.4 load quich search
	while(r)
	{
		key[0] = 'N';
		itoa(i, key+1, 10);
		rlens = ::GetPrivateProfileString("QuickSearch", key, _T(""), buf, 1024,
									  app->m_strQuickSearchPath);
		if(rlens==0)
			r=FALSE;
		else
		{
			alias = buf;
			key[0] = 'S';
			itoa(i, key+1, 10);
			rlens = ::GetPrivateProfileString("QuickSearch", key, _T(""), buf, 1024,
										  app->m_strQuickSearchPath);
			if(rlens==0)
				r=FALSE;
			else
			{
				url = buf;
				alias.TrimLeft();
				alias.TrimRight();
				m_QuickSearchMap.SetAt(alias, url);
			}
		}
		i++;
	}

	//load alias
	i=0; //int j;
	r = TRUE;
	while(r)
	{
		key[0] = 'A';
		itoa(i, key+1, 10);
		alias = app->GetProfileString("Alias", key);
		if(alias=="" || alias.IsEmpty())
			r=FALSE;
		else
		{
			alias.TrimLeft();
			alias.TrimRight();
			key[0] = 'U';
			itoa(i, key+1, 10);
			url = app->GetProfileString("Alias", key);
			if(url=="" || url.IsEmpty())
				r=FALSE;
			else
				m_mapAlias.SetAt(alias, url);
		}
		i++;
	}


	//register drag
	ms_TabDropTarget = new CTabDropTarget;
	ms_TabDropTarget->Register(&m_wndTab);

	//namespace handle
	m_pSession = NULL;
	m_pFactory = NULL;
	if(m_bUseUrlFilter)
		InitialNameSpace();


	m_tLastIPUpdate = m_tLastRes = /*m_tLastWinsUpdate =*/ m_tLastProxyUpdate = CTime::GetCurrentTime();

	//load bytes
//	((CMainFrame*)m_pMainWnd)->m_lPreBytes = LoadBytes();
	QueryPerformanceCounter(&m_tPreBytes);

	//start update timer
	m_nUpdateTimer = SetTimer(1530, 750, NULL);

	m_bNeedUpdProxy = TRUE;

}

UINT _cdecl TSaveConfig(LPVOID con)
{
	((CMyIEApp*)AfxGetApp())->SaveConfg();
	return 0;
}

void CMainFrame::BuildWebProxyMenu(CMenu *pMenu, int nType)
{
	while(pMenu->DeleteMenu(0, MF_BYPOSITION));

		CString strCurWebProxy;
		CChildFrame* tcf = (CChildFrame*)MDIGetActive();
		if (tcf!=NULL && tcf->m_pView!=NULL)
		{
			if(((CMyIEView*)tcf->m_pView)->m_bEnWebProxy)
			{
				strCurWebProxy = ((CMyIEView*)tcf->m_pView)->m_strWebProxy;
			}
		}

		CProxyList* pl;
		int i=0;
		CString str, str1;
	
		if(nType == 0 || nType == 2)
		{
			//pure web proxy

			pl = m_aProxyCategories.GetWebProxyList();
			if(pl!=NULL)
				for(i=0; i<=pl->m_Proxies.GetUpperBound(); i++)
				{
					str = pl->m_Proxies.ElementAt(i)->m_strProxyName;
					if(str.Left(2)=="--")
						pMenu->AppendMenu(MF_SEPARATOR);
					else
					{
						str = str.Left(50);
						str1.LoadString(IDS_SP_INVALID + pl->m_Proxies.ElementAt(i)->m_nProxySpeed);
						str1 = "(" + str1 + ")";

						pMenu->AppendMenu(MF_STRING, 0x1200 + i, str+str1);
						if(strCurWebProxy == pl->m_Proxies.ElementAt(i)->m_strProxy)
							pMenu->CheckMenuItem(0x1200 + i, MF_CHECKED | MF_BYCOMMAND );
					}
				}
		}

		if(nType == 2 )
			pMenu->AppendMenu(MF_SEPARATOR);

		if(nType == 1 || nType == 2)
		{
			//translate
			i = 0;
			pl = m_aProxyCategories.GetWebProxyList(TRUE);
			if(pl!=NULL)
				for(i=0; i<=pl->m_Proxies.GetUpperBound(); i++)
				{
					str = pl->m_Proxies.ElementAt(i)->m_strProxyName;
					if(str.Left(2)=="--")
						pMenu->AppendMenu(MF_SEPARATOR);
					else
					{
						str = str.Left(50);
						str1.LoadString(IDS_SP_INVALID + pl->m_Proxies.ElementAt(i)->m_nProxySpeed);
						str1 = "(" + str1 + ")";

						pMenu->AppendMenu(MF_STRING, 0x1100 + i, str+str1);
						if(strCurWebProxy == pl->m_Proxies.ElementAt(i)->m_strProxy)
							pMenu->CheckMenuItem(0x1100 + i, MF_CHECKED | MF_BYCOMMAND );
					}
				}
		}
}

void CMainFrame::OnRtabAddalias() 
{
	// TODO: Add your command handler code here
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;

	m_wndTab.GetItem(nRTabID, &TabCtrlItem);
	CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
	if(tcf!=NULL)
	{
		CString url, alias;
		CInputAlias dlg;
		dlg.m_url = tcf->m_pView->m_lpszUrl;;
		dlg.m_alias = tcf->m_Title;
		if(dlg.DoModal()==IDOK)
		{
			alias = dlg.m_alias; url = dlg.m_url;
			alias.TrimLeft();
			url.TrimLeft();
			if(alias !="" && !alias.IsEmpty() && url!="" && !url.IsEmpty())
				m_mapAlias.SetAt(alias, url);
		}
	}
}

void CMainFrame::OnRtabAddsc() 
{
	// TODO: Add your command handler code here
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;

	m_wndTab.GetItem(nRTabID, &TabCtrlItem);
	CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
	if(tcf!=NULL)
	{
		CSetSCDlg dlg;
		dlg.m_strEdit = tcf->m_pView->m_lpszUrl;
		dlg.DoModal();
	}	
}

void CMainFrame::OnRtabAutosave() 
{
	// TODO: Add your command handler code here
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;

		m_wndTab.GetItem(nRTabID, &TabCtrlItem);
		CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
		if(tcf!=NULL)
		{
			tcf->m_pView->SendMessage(WM_COMMAND, ID_FILE_AUTOSAVE);
		}
}


void CMainFrame::OnOptionsMyiestartupOpenblankpage() 
{
	// TODO: Add your command handler code here
	bGoHome = 0;
}

void CMainFrame::OnOptionsMyiestartupOpenfavoritepages() 
{
	// TODO: Add your command handler code here
	bGoHome = 3;
}

void CMainFrame::OnOptionsMyiestartupResumelastvisitedpages() 
{
	// TODO: Add your command handler code here
	bGoHome = 2;
}

void CMainFrame::OnOptionsMyiestartupOpenmostfavpages() 
{
	// TODO: Add your command handler code here
	bGoHome = 4;
}

void CMainFrame::OnOptionsMyiestartupGroup() 
{
	// TODO: Add your command handler code here
	bGoHome = 5;
}

void CMainFrame::OnOptionsMyiegohome() 
{
	// TODO: Add your command handler code here
	bGoHome = 1;
}

void CMainFrame::OnUpdateOptionsMyiegohome(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bGoHome == 1);
}

void CMainFrame::OnUpdateOptionsMyiestartupOpenblankpage(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bGoHome == 0);
	
}

void CMainFrame::OnUpdateOptionsMyiestartupOpenfavoritepages(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bGoHome == 3);
}

void CMainFrame::OnUpdateOptionsMyiestartupOpenmostfavoritepages(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bGoHome == 4);
}

void CMainFrame::OnUpdateOptionsMyiestartupOpenstartupgroup(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bGoHome == 5);
}

void CMainFrame::OnUpdateOptionsMyiestartupResumelastvisitedpages(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bGoHome == 2);
}

void CMainFrame::OnViewWinCust(UINT nID) 
{
	// TODO: Add your command handler code here
	m_abWinButtons[nID - ID_VIEW_WIN_PRE] = !m_abWinButtons[nID - ID_VIEW_WIN_PRE];
	BuildWinButtons();
}

void CMainFrame::OnUpdateViewWinCust(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_abWinButtons[pCmdUI->m_nID - ID_VIEW_WIN_PRE]);

}


void CMainFrame::BuildWinButtons()
{
	//remove all buttons
	int b = m_SysMenuBar.GetToolBarCtrl().GetButtonCount();
	for(int i = 0; i<b;i++)
		m_SysMenuBar.GetToolBarCtrl().DeleteButton(0);

	m_SysMenuBar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT  );

	if(m_abWinButtons[0])
		m_SysMenuBar.SetButtonInfo(0, ID_TAB_PRE, TBSTYLE_BUTTON, 5);
	if(m_abWinButtons[1])
		m_SysMenuBar.SetButtonInfo(1, ID_TAB_NEXT, TBSTYLE_BUTTON, 4);
	if(m_abWinButtons[0] || m_abWinButtons[1])
		m_SysMenuBar.SetButtonInfo(2,0,TBBS_SEPARATOR,0);
	if(m_abWinButtons[2])
	{	
		m_SysMenuBar.SetButtonInfo(3, ID_FILE_CLOSEALL, TBSTYLE_BUTTON, 3);
		m_SysMenuBar.SetButtonInfo(4,0,TBBS_SEPARATOR,0);
	}
	if(m_abWinButtons[3])
		m_SysMenuBar.SetButtonInfo(5, ID_WINDOW_MIN, TBSTYLE_BUTTON, 0);
	if(m_abWinButtons[4])
		m_SysMenuBar.SetButtonInfo(6, ID_WINDOW_RESTOR, TBSTYLE_BUTTON, 1);
	if(m_abWinButtons[5])
		m_SysMenuBar.SetButtonInfo(7, ID_FILE_CLOSE_1, TBSTYLE_BUTTON, 2);


	//rebar
	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);
	int bandid = m_wndReBar.GetReBarCtrl().IDToIndex(ID_SYSMENU_BAR);
	rbbi.fMask = RBBIM_STYLE;
	m_wndReBar.GetReBarCtrl().GetBandInfo(bandid, &rbbi);
	rbbi.fStyle &= ~RBBS_NOGRIPPER;	

	CRect rectToolBar;
	m_SysMenuBar.GetItemRect(0, &rectToolBar);
	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE|RBBIM_ID|RBBIM_STYLE;
	rbbi.wID = ID_SYSMENU_BAR;
	rbbi.cyMinChild = rectToolBar.Height();
	m_SysMenuBar.GetToolBarCtrl().GetItemRect(m_SysMenuBar.GetToolBarCtrl().GetButtonCount()-1,&rectToolBar);
	rbbi.cxMinChild = /*rbbi.cx = */rbbi.cxIdeal = rectToolBar.right+2;
	m_wndReBar.GetReBarCtrl().SetBandInfo(bandid, &rbbi);

	if(m_bLockToolbar)
	{
		rbbi.fMask = RBBIM_STYLE;
		rbbi.fStyle |= RBBS_NOGRIPPER;
		m_wndReBar.GetReBarCtrl().SetBandInfo(bandid, &rbbi);
	}

}

void CMainFrame::OnNewTab() 
{
	// TODO: Add your command handler code here
	m_bNextTab = !m_bNextTab;
}

void CMainFrame::OnUpdateNewTab(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	ToolBarSetCheck(pCmdUI, m_bNextTab);
}
void CMainFrame::OnToolsMouseScr() 
{
	// TODO: Add your command handler code here
	m_bMouseCtrlScrSpeed = !m_bMouseCtrlScrSpeed;

	if(!m_bMouseCtrlScrSpeed)
	{
		CMyIEView * pvw = NULL;
		CChildFrame* tcf = (CChildFrame*)MDIGetActive();
		if (tcf!=NULL)
			pvw = (CMyIEView*)tcf->m_pView;
		if(pvw == NULL)
			return;

		pvw->ChangeSpeed(pvw->m_nSrSpeed);
	}
}

void CMainFrame::OnUpdateToolsMouseScr(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bMouseCtrlScrSpeed);
}

void CMainFrame::OnToolsUrlFilter() 
{
	// TODO: Add your command handler code here
	m_bUseUrlFilter = !m_bUseUrlFilter;
	if(m_bUseUrlFilter)
	{
		InitialNameSpace();
	}
	else
		UninitialNameSpace();
}

void CMainFrame::OnUpdateToolsUrlFilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bUseUrlFilter);
}

void CMainFrame::OnOptionsMyiestartupNone() 
{
	// TODO: Add your command handler code here
	bGoHome = 6;
}

void CMainFrame::OnUpdateOptionsMyiestartupNone(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bGoHome == 6);
	
}

void CMainFrame::InitialUrlFilter()
{

	if(m_bUrlFilterDirty)
	{
		try{
			mapUrlFilter.RemoveAll();
			aUrlPattern.RemoveAll();
			aUrlPattern.FreeExtra();
			int i;
			CString tmp;
			for(i = 0; i<=m_astrUrlFilter.GetUpperBound(); i++)
			{
				tmp = m_astrUrlFilter.GetAt(i);
				tmp.MakeLower();tmp.TrimLeft(); tmp.TrimRight();
				if(tmp.Find('*')>=0)
					aUrlPattern.Add(tmp);
				else
					mapUrlFilter.SetAt(tmp, NULL);
			}
			m_bUrlFilterDirty = FALSE;
		}catch(...)
		{
		}
	}

}

void CMainFrame::OnToolsBp() 
{
	// TODO: Add your command handler code here
	if(m_DispBP )
	{
		if(!glo_fInitializing)
		{
			ClearSpeedMonitor();
			m_DispBP = FALSE;
		}
	}
	else
	{
		m_bNoModemSpeed = FALSE;
		m_DispBP = TRUE;
	}
}
UINT _cdecl TInitialMonitor(LPVOID con)
{
	InitSpeedMonitor();
	GetRASSpeed((DWORD*)&pMainFrame->m_lPreBytes);
	QueryPerformanceCounter(&pMainFrame->m_tPreBytes);
	glo_fInitializing = FALSE;
	return 0;
}

void CMainFrame::OnUpdateToolsBp(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_DispBP);
}

void CMainFrame::InitialNameSpace()
{
	HRESULT hr = CoGetClassObject(CLSID_MyIENSHandle, CLSCTX_SERVER, NULL, IID_IClassFactory, (void**)&m_pFactory);
	if(hr==S_OK)
	{
		if(CoInternetGetSession(0, &m_pSession, 0)==S_OK)
		{
			m_pSession->RegisterNameSpace(m_pFactory, CLSID_MyIENSHandle, L"http", 0, NULL, 0);
			m_pSession->RegisterNameSpace(m_pFactory, CLSID_MyIENSHandle, L"https", 0, NULL, 0);
		}
	}
}

void CMainFrame::UninitialNameSpace()
{
	if(m_pSession!=NULL)
	{
		m_pSession->UnregisterNameSpace(m_pFactory, L"http");
		m_pSession->UnregisterNameSpace(m_pFactory, L"https");
		m_pSession->Release();
		m_pSession = NULL;
	}
	if(m_pFactory!=NULL)
		m_pFactory->Release();
	m_pFactory = NULL;
}

void CMainFrame::OnViewLocktoolbar() 
{
	// TODO: Add your command handler code here
	m_bLockToolbar = !m_bLockToolbar;
	FixToolbar(m_bLockToolbar);
}

void CMainFrame::OnUpdateViewLocktoolbar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bLockToolbar);
}

void CMainFrame::FixToolbar(BOOL bFixed)
{
	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_STYLE;
//	if(m_bLockToolbar)
//		rbbi.fStyle = RBBS_FIXEDBMP |RBBS_NOGRIPPER|RBBS_USECHEVRON;
//	else
//		rbbi.fStyle = RBBS_FIXEDBMP | RBBS_BREAK |RBBS_GRIPPERALWAYS|RBBS_USECHEVRON;

	for(int i=0; i<8; i++)
	{
		m_wndReBar.GetReBarCtrl().GetBandInfo(i, &rbbi);
		if(m_bLockToolbar)
			rbbi.fStyle |= RBBS_NOGRIPPER;
		else
			rbbi.fStyle &= ~RBBS_NOGRIPPER;
		m_wndReBar.GetReBarCtrl().SetBandInfo(i, &rbbi);
	}
}

void CMainFrame::OnOptionFBtab()
{
	if(m_bFBTab)
	{
		m_wndTab.ModifyStyle(TCS_BUTTONS|TCS_FLATBUTTONS|TCS_RAGGEDRIGHT, TCS_OWNERDRAWFIXED);
		if(m_bTopTab)
			m_wndTab.ModifyStyle(TCS_BOTTOM,0);
		else
			m_wndTab.ModifyStyle(0, TCS_BOTTOM);
		m_wndTab.SetFonts();
		m_bFBTab = FALSE;
	}
	else
	{
		m_wndTab.ModifyStyle(TCS_OWNERDRAWFIXED, TCS_BUTTONS|TCS_FLATBUTTONS|TCS_RAGGEDRIGHT);
		m_wndTab.SetFont(m_wndReBar.GetFont());
		m_bFBTab = TRUE;
	}
	CSize size;
	if(m_bFBTab)
	{
		size.cx = FBPADDINGX;
		size.cy = FBPADDINGY;
	}
	else
	{
		size.cx = TABPADDINGX;
		size.cy = TABPADDINGY;
	}
	//m_wndTab.SetItemSize(size);
//	m_wndTab.SetFont(m_wndReBar.GetFont());
	m_wndTab.SetPadding( size );

	PostMessage(WM_UPDATE_TAB, TRUE);

}

void CMainFrame::SessionHistory(CMenu *pMenu, int ID, CChildFrame* pcf)
{
	ITravelLogStg * pITL = NULL;
	pITL = pcf->m_pView->m_pITravelLog;
	int i=0, l=0;
	if(pITL!=NULL)
	{
		IEnumTravelLogEntry * pEnum;
		ITravelLogEntry* pEntry;
		ULONG celt;
		LPOLESTR postr;
		CString str;
		DWORD flag;
		if(ID == ID_GO_BACK)
			flag = TLEF_RELATIVE_BACK;
		else
			flag = TLEF_RELATIVE_FORE;
		if(pITL->EnumEntries(flag, &pEnum)==S_OK)
		{
				while(pEnum->Next(1, &pEntry, &celt)==S_OK && l<9)
				{
					pEntry->GetTitle(&postr);
					str = postr;
					pMenu->AppendMenu(MF_STRING, l+1, str);
					l++;
					CoTaskMemFree(postr);
					pEntry->Release();
				}
				pEnum->Release();
			}
		}
		else
		{
			if(ID == ID_GO_BACK)
				for(i=pcf->m_pView->m_nCurHistory-1; i>=0 && l<9; i--)
				{
						if(pcf->m_pView->m_astrHistory.GetAt(i).IsEmpty())
							pMenu->AppendMenu(MF_STRING, l+1, pcf->m_pView->m_astrHistoryURL.GetAt(i));
						else
							pMenu->AppendMenu(MF_STRING, l+1, pcf->m_pView->m_astrHistory.GetAt(i));

						l++;
				}
			else
				for(i=pcf->m_pView->m_nCurHistory+1; i<pcf->m_pView->m_nHistoryLength && l<9; i++)
				{
					if(pcf->m_pView->m_astrHistory.GetAt(i).IsEmpty())
						pMenu->AppendMenu(MF_STRING, l + 1, pcf->m_pView->m_astrHistoryURL.GetAt(i));
					else
						pMenu->AppendMenu(MF_STRING, l + 1, pcf->m_pView->m_astrHistory.GetAt(i));
					l++;
				}
		}
}

void CMainFrame::OnToolsAutoscrollAll() 
{
	// TODO: Add your command handler code here
	m_bAllScroll = !m_bAllScroll;

	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	if(tcf == NULL || tcf->m_pView == NULL)
		return;
	if(m_bAllScroll)
	{
		 tcf->m_pView->ChangeSpeed(STOP_TIMER);
	}
	else if(!tcf->m_pView->m_bUseAutoScroll)
	{
		if(tcf->m_pView->m_nScrollTimer)
			KillTimer(tcf->m_pView->m_nScrollTimer);
		tcf->m_pView->m_nScrollTimer = 0;
	}
}

void CMainFrame::OnUpdateToolsAutoscrollAll(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bAllScroll);
}
