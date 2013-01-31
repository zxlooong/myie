// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__19E497C1_4ECF_11D3_9B1D_0000E85300AE__INCLUDED_)
#define AFX_MAINFRM_H__19E497C1_4ECF_11D3_9B1D_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//Define the CommandTarget ids
#define HTMLID_FIND 1
#define HTMLID_VIEWSOURCE 2
#define HTMLID_OPTIONS 3 


#include "StatusBarWithProgress.h"
//#include "MyIEDoc.h"
#include "MyIEView.h"
#include "ChildFrm.h"

#include "BaseExpObj.h"
//#include "OOExToolBar.hpp"
#include "ChevBar.h"
#include "MenuBar.h"
#include "AddressBarEx.h"
#include "BMode.h"
#include "TabDropTarget.h"
#include "ProxyList.h"
#include "ExternalUtilItem.h"
#include "AnimateIcon.h"	// Added by ClassView
#include "AdvTabCtrl.h"


class CCollectorDlg;
class CLinkListDlg;
class CMainFrame;

//extern int nOpenType;
//extern CString strURL;
extern int percent;
extern CMainFrame* pMainFrame;
extern BOOL EnablePopupFilter, EnableAutoFilter, EnableDupFilter;
extern int bGoHome;
extern 	BOOL m_bTopTab;
extern	CStringArray m_astrLinks;
extern	CStringArray m_astrFavs;
extern 	int m_bMax;
extern BOOL bConfirmExit;
extern BOOL bSingleMenu;
extern BOOL bSysTray;
extern int nMaxWins;
extern BOOL bSilent;
extern int nNextTab;
extern int nDefFontSize;
extern BOOL bURLOnly;
extern int nTrayIconID;
extern BOOL bFilteBlank;
extern BOOL m_bAutoStart;
//extern 	HANDLE m_hMutex;
extern BOOL bLinkInNewWindow, bFileInNewWindow, bAddressInNewWindow, bFavInNewWindow,  bHisInNewWindow;
extern float gSeed;
extern const UINT MSWHELL_ROLLMSG;
extern BOOL bCompactFavBar;
extern BOOL bConfirmCloseAll;
extern BOOL bOneInstance;
//extern BOOL bUpdateTab;
extern BSTR bstryes;
extern BOOL bVerbose;
//extern BOOL bHideBars;
extern BOOL bAutoSetBG,bLockHomepage;
extern char lpfiles[1024];
extern BOOL bProxyDirty;

#define WSM_TRAY	WM_USER+3
#define BORDER  2

//work threads
UINT _cdecl SendDocuments(LPVOID file);
typedef struct stagDocuments
{
	char filename[256];
	BOOL IsLocalFile;
}sDocuments;
UINT _cdecl QuickSaveDocuments(LPVOID savefiles);
typedef struct stagSaveFiles
{
	CString base;
	CString filename;
	CMapStringToString mapDLFiles;
	CString root;
}sSaveFiles;

UINT _cdecl TSaveConfig(LPVOID con);


class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
	CStringList m_astrFavoriteFolders;
//	CStringArray m_astrFavoriteURLs;
	CStringList m_astrPopup;
	CMapStringToString m_QuickSearchMap;
	CStringArray m_astrUrlFilter;
	BOOL m_bUrlFilterDirty;

// Operations
public:
	friend CTxtDropTarget;
	void SetBackground(CMyIEView* pview, VARIANT& color);
	TCHAR           m_strHome[MAX_PATH], m_strCurHome[MAX_PATH];
	void SetHomePage(LPCTSTR homepage);
	void GetHomePage(LPTSTR homepage);
	CString m_strAutoSavePath;
	BOOL m_bIsAutoSave;
	void InitialUrlFilter();

private:
	void SessionHistory(CMenu* pMenu, int ID, CChildFrame* pcf);
	void BuildWinButtons();
	void ChangTabPos(int npos);
	BOOL GetAllLinks(CMyIEView* pView, CString & strlinks);
	BOOL SetDownloadTool(CString& ProgID);
	CCollectorDlg* pCollectDlg;
	CLinkListDlg* pLinkListDlg;
	int m_SBIPWidth, m_SBMemWidth;
	HICON m_hSecuIcon;
	void SaveForm(BOOL bIsGeneral);
	BOOL m_bLastIB, m_bIB;
	int m_nNeedIB;
	BOOL IsBrowsing();
	void CloseExternalUtil();
	BOOL fTaskBarIsAutohide;
	UINT uEdge;
	void LaunchEditor(LPCSTR editor, LPCSTR file);
	void LaunchEmail(LPCSTR mailclient);
	CTabDropTarget* ms_TabDropTarget;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//control used by search

// Implementation
public:
	BOOL m_bAllScroll;
	BOOL m_bNeedUpdProxy;
	void OnOptionFBtab();
	void FixToolbar(BOOL bFixed);
	BOOL m_bLockToolbar;
	void UninitialNameSpace();
	void InitialNameSpace();
	DWORD m_dwHidden;
	BOOL m_DispBP;
	BOOL m_bUseUrlFilter;
	IInternetSession* m_pSession;
	IClassFactory* m_pFactory;
	int m_nScrollArrowH;
	int m_nScrollWidth;
	BOOL m_bMouseCtrlScrSpeed;
	void BuildWebProxyMenu(CMenu* pMenu, int nType);
	BOOL m_bMostFavChecked;
	CMyIEView* m_pCurView;
	BOOL m_bCateWithSite;
	int m_nAutoSave;
	BOOL m_bAnimatIcon;
	//2.5 for integrate with download tools
	IDispatch * pDownload; 

	DWORD m_dwWebProxyFrom;
	BOOL m_bIsSecure, m_bLastIsSecure;
//	BOOL GetCurSecureInfo();
	CAnimateIcon m_animIcon;
	BOOL m_bSecure;
	BOOL m_bNeedActive;
	int nDelay;
	void StartUtil(int i);
	int FindDupURL(LPCSTR lpszURL, CChildFrame* pSelf);
	BOOL m_bSmallToolBar;
	int m_nDefRefreshInterval;
	void UpdateUndoMenu();
	int m_nExpWidth;
	BOOL m_bDispMem;
	BOOL m_bDisplayOpenAll, m_bDisplayAddFav;
	CString m_strBGColor;
	COLORREF m_colBGC;
	CString GetProxyName(CString& proxy);
	void DefaultMail(LPCSTR mailto);
	int m_nAllowDBC, m_nAllowMC, m_nAllowRC;
	int m_nNewWindow, m_nNewWindow2;
	void SaveLastVisit();
	void AddtoFavorite(BSTR bstrURL, VARIANT* pTitle);
	void OpenGroup(LPCSTR gname);
	void AddtoGroup(LPCSTR pszName, LPCSTR pszURL, UINT nID);
	void MenuLoadGroupList(CMenu* pMenu);
	void InitGroups();
	UINT m_nState;
	UINT nWin;
	BOOL m_bSaveConfig;
	void ToolBarSetCheck(CCmdUI* pCmdUI, BOOL bCheck);
	UINT m_nFavSize;
	void FavMenuAddURL(LPCSTR path, LPCSTR root, BOOL IsLink, CMenu *pMenu, int nPos, int& nMostFavs);
	void FavMenuAddFolder(LPCSTR path, LPCSTR root, BOOL FirstLevel, BOOL IsLink, CMenu* pMenu, int& nEndPos);
	CString m_strUndoURL[16];
	CString m_strUndoName[16];
	int undoStart, undoEnd;
	int m_nDefSrSpeed; //1-slow, 2-medium, 3-fast
	BOOL m_bDefScrollAfterDL;
	BOOL m_bDefUseAutoScroll;
	CBmode m_bmUserDefault, m_bmStandard, m_bmNews;

	HWND m_hEdit;
	int m_nInterval;
	UINT m_nTimerID, m_nUpdateTimer, m_nAnimateIconTimer;
	CStringList m_astrOpenAll;
	int m_nProtectNum;
	BOOL m_bRemPsw;
	CString m_strProtectPwd;
	int m_nSeq; //0-default, 1-next, 2- pre
	BOOL m_bUse3721;
	int m_nMenuWidth;
	CTime m_tLastRes, m_tLastProxyUpdate; //m_tLastWinsUpdate, 
//	BOOL m_bWin9x;
	BOOL m_bNoModemSpeed;
	long m_lPreBytes;
    LARGE_INTEGER m_tPreBytes; //, m_tDBClick;
	CString	m_strCSE2;
	CString	m_strCSE1;
	CString	m_strCE2;
	CString	m_strCE1;
	CString	m_strSE1;
	CString	m_strSE2;
	CSize m_sTBTextSize, m_sTBTextSizeSM;
	CMapStringToString m_mapAlias;
	CString m_strShortCuts[12];
	BOOL m_bUseAlias;
	BOOL m_bUseSC;
	int m_nMaxTabWidth;
	int m_nMinTabWidth;
	int AdjustTabWidth(BOOL bNoReCalSize = FALSE);
	BOOL m_bAutoTab, m_bFBTab;
	int  m_nTextLabel; //0-no label, 1-show label, 2-selective text
	BOOL m_bColorIcon;
	void InitialToolBar(BOOL bReplaceIcon=FALSE);
	CStringList m_astrBGPics;
	void ShowBackground(BOOL bShow);
	HBITMAP m_bmBGPic;
	CString m_strBGPic;
	CString m_strLastIP;
	CTime m_tLastIPUpdate;
	CMenu* GetMenu();
	BOOL m_bDisplayIP;
	BOOL DecodeEscap(CString& strUrl);
	int SetTabIcon(int nIcon, int nItem);
	BOOL m_bNextTab;
	int m_nPreLines;
	BOOL m_bMultiLineTab;
	BOOL m_bFavDirty;
	BOOL GetFavoriteFolder(CString& strFav);
	int OpenAllFavs(LPCSTR lpszPath, BOOL bMostFav=FALSE);
	BOOL SetDefault(LPCSTR lpszKey, BOOL setdef);
	void AddFavorite(CChildFrame* tcf);
	CString	m_strStartFavFolder;
	CString m_strStartGroup;
	CString m_strLinkFolder;
	BOOL HasExpBar(int nStr);
	BOOL m_bActiveNewWindow;
	CChildFrame* NewChildWindow(int nIniLevel=1, int nOpenType=1, LPCSTR strUrl=NULL, CMyIEView * pParentView=NULL, BOOL bForceActive=FALSE);
	BOOL ResolveLink(LPCSTR lpszLinkFile, LPSTR lpszPath);
	BOOL GetProxySetting(CString& proxy, CString& bypass, CString& user, CString& pwd);
//	CStringList m_astrProxyPwd;
//	CStringList m_astrProxyUser;
	CProxyCategory m_aProxyCategories;
//2.3CList<BYTE, BYTE> m_aProxySpeed;	, m_aWebProxySpeed;
	CRect m_mainRect;
	BOOL m_bChildMax;
	BOOL m_bStatusBarWasVisible;
	BOOL m_bLinkBarWasVisible;
	BOOL m_bFavBarWasVisible;
	BOOL m_bAddressBarWasVisible;
	BOOL m_bExternalBarWasVisible;
	void FullScreenModeOff();
	void FullScreenModeOn();
	BOOL m_bFullScreenMode;
	void RemoveExp();
	void BuildExpBar(LPCSTR lpszTitle);
	void BuildUtilMenu();
/*2.3	CStringList m_lstrUtilCmds;
	CStringList m_lstrUtilNames;
	CList<BOOL, BOOL> m_lbUtilStart, m_lbUtilClose;*/
	CExternalUtilList m_ExternalUtilList;
//2.3	CStringList m_astrProxy;
	CString m_strDefaultWebProxy;
//2.3	CStringList m_astrWebProxy;
	CString m_strProxyByPass;
	CString m_strCurProxy, m_strCurProxyName;
	int m_nCurProxyList;
//2.3	CStringList m_astrProxyName;
//2.3	CStringList m_astrWebProxyName;
	BOOL DelTempFiles(DWORD CacheType, BOOL op=TRUE);
	void AddToPopup(CChildFrame* pChFrm);
	void AddMenuBreak(CMenu * pMenu,BOOL bFavMenu=TRUE);
	void InitFavorites(BOOL bOnlyLink=FALSE);
	HRESULT ExecCmdTarget(DWORD nCmdID,CFixedHtmlView* pHVW);
	int nRTabID;
	void SaveTypedURL();
	void LoadTypedURL();
	int FindTab(CChildFrame* lpFrame, BOOL bCleanBlank=FALSE);
	UINT m_nTitleLen;
	void SetTabTitle(LPCSTR lpszTitle, int nItem);
	int nTotalTabs;
	int AddNewTab(CWnd* lpMvw, int CurID=-1, LPTSTR label=NULL, BOOL bAddLast=TRUE);
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void SetAddress(LPCTSTR lpszUrl);
	int BuildFavoritesMenu(LPCTSTR pszPath, int nStartPos, CMenu* pMenu, int nFixMenu /*=0*/, int FirstLevel /*= FALSE*/, int& nFavs, int& nMostFavs);
	CStatusBarWithProgress  m_wndStatusBar;
	CAdvTabCtrl	m_wndTab;
	CChevBar      m_wndReBar;
	CChevBar		m_wndReBar2;
	baseCMyBar	m_wndInstantBar;
	CBaseExpObj*	m_pwndExpObj;
	CToolBar    m_wndToolBar;
	CComboBoxEx* m_wndAddress;
	CAddressBarEx m_AddressBar;
	BOOL m_bStatusBlank;
	CImageList m_TabImgList;
	CDownloadToolList m_DList;
	CString m_strDefaultDLT, m_strDownloadFileExt;
	BOOL m_bAllowMouseGestures;
	BOOL m_abWinButtons[6];
	CToolBar	m_SysMenuBar;

protected:  // control bar embedded members
	CToolBar	m_LinkBar;
	CMenuBar	m_wndMenuBar;
	CToolBar	m_FavBar;
	CToolBar	m_ExternalTool;
	HINSTANCE   hResInst;

	CString ExpStr, Expstr1, Expstr2, Expstr3, Expstr4, Expstr5;
	CString tab, strTipText, gString1, gString2, gaddre, res, url, useproxy, tip, bmppath, fstr, bp,favurl, lastvisits, lasturl, strlinks;
//	POINT tabpt, tabpt2;
//	float dbtime;
	short ks;
	HWND hAddr;

// Generated message map functions
protected:
	void OnOptionMultiTab();
	void OnOptionAutotab();
	int m_nSwitch;
//	CComboBoxEx m_wndCombo;
	//CAnimateCtrl m_wndAnimate;
	afx_msg void OnOptionsMyiestartupGroup();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileNewBlank();
	afx_msg void OnFileNewCurrentpage();
	afx_msg void OnViewAddressbar();
	afx_msg void OnViewTaskbar();
//	afx_msg void OnFileSaveas();
//	afx_msg void OnFileSavehtml();
	afx_msg void OnFileCloseall();
	afx_msg void OnFileWorkoffline();
	afx_msg void OnClose();
	afx_msg void OnViewSource();
	afx_msg void OnFileClose();
	afx_msg void OnFileClose2();
	afx_msg void OnToolsInternetoptions();
	afx_msg void OnFavoritesAddtofavorites();
	afx_msg void OnFavoritesExportfavorites();
	afx_msg void OnFavoritesImportfavorites();
	afx_msg void OnFavoritesOrgnizefavorites();
	afx_msg void OnRtabLock();
	afx_msg void OnToolsAddtopopupfilter();
	afx_msg void OnRtabAddtopopupfilter();
	afx_msg void OnToolsEablepopupfilter();
	afx_msg void OnToolsEditpopupfilterlist();
	afx_msg void OnUpdateFileWorkoffline(CCmdUI* pCmdUI);
	afx_msg void OnOptionsMyiegohome();
	afx_msg void OnUpdateMenu(CCmdUI* pCmdUI);
	afx_msg void OnToolsEmail();
	afx_msg void OnToolsAddr();
	afx_msg void OnUpdateOptionsUseProxy(CCmdUI* pCmdUI);
	afx_msg void OnOptionsUseProxy();
	afx_msg void OnToolsCleanCookie();
	afx_msg void OnToolsCache();
	afx_msg void OnToolsAll();
	afx_msg void OnToolsHistory();
	afx_msg void OnOptionsSetProxy();
//	afx_msg void OnOptionTabTop();
//	afx_msg void OnOptionTabBot();
//	afx_msg void OnUpdateOptionTabPos(CCmdUI* pCmdUI);
	afx_msg void OnToolsExternalUtilitymanager();
	afx_msg void OnViewHis();
	afx_msg void OnViewSearch();
	afx_msg void OnUpdateViewExp(CCmdUI* pCmdUI);
	afx_msg void OnViewFullscreen();
	afx_msg void OnUpdateViewFullscreen(CCmdUI* pCmdUI);
	afx_msg void OnOptionsSetasdefaultbrowser();
	afx_msg void OnAddrDrop();
	afx_msg void OnAddrFocus();
	afx_msg void OnFileNew();
	afx_msg void OnHelpHelp();
	afx_msg void OnOptionsActivenewwindow();
	afx_msg void OnUpdateOptionsActivenewwindow(CCmdUI* pCmdUI);
	afx_msg void OnOptionsMyiestartupOpenblankpage();
	afx_msg void OnOptionsMyiestartupOpenfavoritepages();
	afx_msg void OnOptionsMyiestartupResumelastvisitedpages();
	afx_msg void OnCloseOther();
	afx_msg void OnRtabAddfav();
	afx_msg void OnViewLinks();
	afx_msg void OnViewToolbar();
	afx_msg void OnUpdateViewToolbar(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHelpForum();
	afx_msg void OnHelpMail();
	afx_msg void OnToolsOpenalllinks();
	afx_msg void OnOptionAlwaysNew();
	afx_msg void OnUpdateOptionAlwaysNew(CCmdUI* pCmdUI);
	afx_msg void OnFileCloseKeepcur();
//	afx_msg void OnOptionsNewtab();
//	afx_msg void OnUpdateOptionsNewtab(CCmdUI* pCmdUI);
	afx_msg void OnViewRefreshall();
	afx_msg void OnViewStopAll();
	afx_msg void OnHelpHomepage();
	afx_msg void OnFileNewClip();
	afx_msg void OnWindowCascade();
	afx_msg void OnWindowTileHorz();
	afx_msg void OnWindowTileVert();
//	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnToolsIp();
	afx_msg void OnUpdateToolsIp(CCmdUI* pCmdUI);
	afx_msg void OnWindowRestore();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnWindowMin();
	afx_msg void OnViewMenubar();
	afx_msg void OnToolsSetbackgroundpic(UINT nID);
	afx_msg void OnUpdateToolsSetbackgroundpic(CCmdUI* pCmdUI);
	afx_msg void OnFileNewmyie();
	afx_msg void OnFileOpen();
	afx_msg void OnViewToolbartextlabel();
	afx_msg void OnUpdateViewToolbartextlabel(CCmdUI* pCmdUI);
	afx_msg void OnToolsOpenstart();
	afx_msg void OnChangeProxy(UINT nID);
	afx_msg void OnChangeWebProxy(UINT nID);
	afx_msg void OnMostFav(UINT nID);
//	afx_msg void OnUpdateOptionMultiTab(CCmdUI* pCmdUI);
//	afx_msg void OnUpdateOptionAutotab(CCmdUI* pCmdUI);
	afx_msg void OnShortcutMfl();
	afx_msg void OnF2(UINT nID);
	afx_msg void OnShortcutUseSc();
	afx_msg void OnUpdateShortcutUseSc(CCmdUI* pCmdUI);
	afx_msg void OnShortcutSc();
	afx_msg void OnShortcutAlias();
	afx_msg void OnUpdateShortcutAlias(CCmdUI* pCmdUI);
	afx_msg void OnShortcutManalias();
	afx_msg void OnViewToolbarsCustomize();
	afx_msg void OnOptionsMyieoptions();
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnOptionsMyiestartupOpenmostfavpages();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnNotifyTray(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelTab(WPARAM wParam, LPARAM lParam);
	afx_msg void OnShortcutsUse3721();
	afx_msg void OnUpdateShortcutsUse3721(CCmdUI* pCmdUI);
	afx_msg void OnRtabStickname();
//	afx_msg void OnOptionSeqDefault();
//	afx_msg void OnUpdateOptionSeqDefault(CCmdUI* pCmdUI);
//	afx_msg void OnOptionSeqNext();
//	afx_msg void OnUpdateOptionSeqNext(CCmdUI* pCmdUI);
//	afx_msg void OnOptionSeqPre();
//	afx_msg void OnUpdateOptionSeqPre(CCmdUI* pCmdUI);
	afx_msg void OnTabPre();
	afx_msg void OnTabNext();
	//{{AFX_MSG(CMainFrame)
	afx_msg void OnOptionsUrlinstatus();
	afx_msg void OnUpdateOptionsUrlinstatus(CCmdUI* pCmdUI);
	afx_msg void OnViewFavs();
	afx_msg void OnRtabProtect();
	afx_msg void OnViewExTool();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnFileStopOpenall();
	afx_msg void OnUpdateFileStopOpenall(CCmdUI* pCmdUI);
	afx_msg void OnFileHide();
	afx_msg void OnViewFolder();
	afx_msg void OnOptionsOpenlinkinnewwindow();
	afx_msg void OnUpdateOptionsOpenlinkinnewwindow(CCmdUI* pCmdUI);
	afx_msg void OnOptionsBrowsemodesDefault();
	afx_msg void OnUpdateOptionsBrowsemodesDefault(CCmdUI* pCmdUI);
	afx_msg void OnOptionsBrowsemodesNews();
	afx_msg void OnUpdateOptionsBrowsemodesNews(CCmdUI* pCmdUI);
	afx_msg void OnOptionsBrowsemodesStandard();
	afx_msg void OnUpdateOptionsBrowsemodesStandard(CCmdUI* pCmdUI);
	afx_msg void OnViewExplorebarResource();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnEditUndoclose();
	afx_msg void OnUpdateEditUndoclose(CCmdUI* pCmdUI);
	afx_msg void OnEditEdit();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnOrggroup();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnUpdateWindowMin(CCmdUI* pCmdUI);
	afx_msg void OnNewwindow();
	afx_msg void OnNewwindowFav();
	afx_msg void OnUpdateNewwindowFav(CCmdUI* pCmdUI);
	afx_msg void OnNewwindowHistory();
	afx_msg void OnUpdateNewwindowHistory(CCmdUI* pCmdUI);
	afx_msg void OnNewwindowFile();
	afx_msg void OnUpdateNewwindowFile(CCmdUI* pCmdUI);
	afx_msg void OnFileSendpagebyemail();
	afx_msg void OnFileQuicksave();
	afx_msg void OnFileSendlinkbyemail();
	afx_msg void OnToolsSelbg();
	afx_msg void OnToolsSetbg();
	afx_msg void OnOptionsExportproxylist();
	afx_msg void OnOptionsImportproxylist();
	afx_msg void OnToolsMem();
	afx_msg void OnUpdateToolsMem(CCmdUI* pCmdUI);
	afx_msg void OnRtabAutoref();
	afx_msg void OnUpdateRtabAutoref(CCmdUI* pCmdUI);
	afx_msg void OnRtabReffre();
	afx_msg void OnViewLock();
	afx_msg void OnToolsAllBk();
	afx_msg void OnToolsQuicksearch();
	afx_msg void OnToolsSync();
	afx_msg void OnScFillform();
	afx_msg void OnScSaveform();
	afx_msg void OnScSavegenform();
	afx_msg void OnToolsCollect();
	afx_msg void OnToolsCleanaddr();
	afx_msg void OnAllPageLinks();
	afx_msg void OnToolsAutosetbg();
	afx_msg void OnUpdateToolsAutosetbg(CCmdUI* pCmdUI);
	afx_msg void OnViewEncodingAuto();
	afx_msg void OnUpdateViewEncodingAuto(CCmdUI* pCmdUI);
	afx_msg void OnSaveasGroup();
	afx_msg void OnGroup();
	afx_msg void OnUpdateFileOpenf(CCmdUI* pCmdUI);
	afx_msg void OnToolCleanmyieform();
	afx_msg void OnRtabAddalias();
	afx_msg void OnRtabAddsc();
	afx_msg void OnRtabAutosave();
	afx_msg void OnUpdateOptionsMyiegohome(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOptionsMyiestartupOpenblankpage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOptionsMyiestartupOpenfavoritepages(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOptionsMyiestartupOpenmostfavoritepages(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOptionsMyiestartupOpenstartupgroup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOptionsMyiestartupResumelastvisitedpages(CCmdUI* pCmdUI);
	afx_msg void OnToolsMouseScr();
	afx_msg void OnUpdateToolsMouseScr(CCmdUI* pCmdUI);
	afx_msg void OnToolsUrlFilter();
	afx_msg void OnUpdateToolsUrlFilter(CCmdUI* pCmdUI);
	afx_msg void OnOptionsMyiestartupNone();
	afx_msg void OnUpdateOptionsMyiestartupNone(CCmdUI* pCmdUI);
	afx_msg void OnToolsBp();
	afx_msg void OnUpdateToolsBp(CCmdUI* pCmdUI);
	afx_msg void OnViewLocktoolbar();
	afx_msg void OnUpdateViewLocktoolbar(CCmdUI* pCmdUI);
	afx_msg void OnToolsAutoscrollAll();
	afx_msg void OnUpdateToolsAutoscrollAll(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnNewTab();
	afx_msg void OnUpdateNewTab(CCmdUI* pCmdUI);
	afx_msg void OnViewWinCust(UINT nID);
	afx_msg void OnUpdateViewWinCust(CCmdUI* pCmdUI);
	afx_msg void OnNewAddress();
	afx_msg void DoNothing();
	afx_msg void OnNewAddressEnter();
	afx_msg void OnFavorite(UINT nID);
	afx_msg void OnDropDown(NMHDR* pNotifyStruct, LRESULT* pResult);
	void OnUpdateProgress(CCmdUI* pCmdUI);
	void OnUpdateWins(CCmdUI* pCmdUI);
	void OnUpdateIP(CCmdUI* pCmdUI);
	afx_msg void OnSelChange(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnRClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnEncoding(UINT nID);
	afx_msg void OnFonts(UINT nID);
	afx_msg void OnUtils(UINT nID);
	afx_msg void OnShellOpen(WPARAM wParam,LPARAM);
	afx_msg void OnViewFavorite();
	afx_msg void OnLinkPopup();
	afx_msg void OnFavPopup();
	afx_msg void OnUpdateFav(WPARAM wParam,LPARAM);
	afx_msg void OnUpdateTab(WPARAM wParam,LPARAM);
	afx_msg void OnDelayLoadConfig(WPARAM wParam,LPARAM);
	afx_msg void OnAllFav(UINT nID);
	afx_msg void OnUpdateTabTips(WPARAM wParam,LPARAM);
	afx_msg void OnTabNeedText(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnUpdateToolbar(WPARAM wParam,LPARAM);
	void OnUpdateBP(CCmdUI* pCmdUI);
	void OnUpdateRES(CCmdUI* pCmdUI);
	void OnUpdateSecu(CCmdUI* pCmdUI);
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
	afx_msg void OnSwitchWins(UINT nID);
	afx_msg void OnRtabRefresh();
	afx_msg void OnRtabStop();
	afx_msg void OnActivateWindow(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHotKey(WPARAM wParam,LPARAM);
	afx_msg void OnScRll(WPARAM wParam, LPARAM lParam);
	afx_msg void OnToolsAutoscrollSpeed(UINT nID);
	afx_msg void OnViewStop();
	afx_msg void OnViewRefresh();
	afx_msg void OnFilePrintpreview();
	afx_msg void OnFilePrint();
	afx_msg void OnOpenGroup(UINT nID);
	afx_msg void OnAddtoGroup(UINT nID);
	afx_msg void OnWindowList(UINT nID);
	afx_msg void OnUndoList(UINT nID);
	afx_msg void OnAddThisFav(UINT nID);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__19E497C1_4ECF_11D3_9B1D_0000E85300AE__INCLUDED_)
