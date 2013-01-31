#if !defined(AFX_FIXEDHTMLVIEW_H__8BCE18F1_197D_11D4_9BCF_0000E85300AE__INCLUDED_)
#define AFX_FIXEDHTMLVIEW_H__8BCE18F1_197D_11D4_9BCF_0000E85300AE__INCLUDED_

//FixedHtmlView 2.3

#ifndef __AFXDISP_H__
	#include <afxdisp.h>
#endif

#ifndef __exdisp_h__
	#include <exdisp.h>
#endif

#ifdef _AFX_MINREBUILD
#pragma component(minrebuild, off)
#endif
#ifndef _AFX_FULLTYPEINFO
#pragma component(mintypeinfo, on)
#endif

/*
#ifdef _AFX_PACKING
#pragma pack(push, _AFX_PACKING)
#endif*/

#include "webbrowser2.h"	// Added by ClassView
#include <tlogstg.h>
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FixedHtmlView.h : header file
//

#ifndef _AFXDLL
#undef AFX_DATA
#define AFX_DATA AFX_OLE_DATA
#endif

/////////////////////////////////////////////////////////////////////////////
// CFixedHtmlView view

class CFixedHtmlView : public CView
{
protected:
	CFixedHtmlView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFixedHtmlView)
	DECLARE_EVENTSINK_MAP()


// Attributes
public:
//	CString GetType();
	long GetLeft() ;
	void SetLeft(long nNewValue);
	long GetTop() ;
	void SetTop(long nNewValue);
	long GetHeight() ;
	void SetHeight(long nNewValue);
	long GetWidth() ;
	void SetWidth(long nNewValue);
//	void SetVisible(BOOL bNewValue);
//	BOOL GetVisible() ;
	void GetLocationName(CString& strLocName) ;
	READYSTATE GetReadyState() ;
//	BOOL GetOffline() ;
//	void SetOffline(BOOL bNewValue);
//	BOOL GetSilent() ;
//	void SetSilent(BOOL bNewValue);
//	BOOL GetTopLevelContainer() ;
	void GetLocationURL(CString& strLocURL) ;
//	BOOL GetBusy() ;
	LPDISPATCH GetApplication() ;
//	LPDISPATCH GetParentBrowser() ;
//	LPDISPATCH GetContainer() ;
	LPDISPATCH GetHtmlDocument() ;
//	CString GetFullName() ;
//	int GetToolBar() ;
//	void SetToolBar(int nNewValue);
//	BOOL GetMenuBar() ;
//	void SetMenuBar(BOOL bNewValue);
//	BOOL GetFullScreen() ;
//	void SetFullScreen(BOOL bNewValue);
	OLECMDF QueryStatusWB(OLECMDID cmdID) ;
//	BOOL GetRegisterAsBrowser() ;
//	void SetRegisterAsBrowser(BOOL bNewValue);
//	BOOL GetRegisterAsDropTarget() ;
//	void SetRegisterAsDropTarget(BOOL bNewValue);
//	BOOL GetTheaterMode() ;
//	void SetTheaterMode(BOOL bNewValue);
//	BOOL GetAddressBar() ;
//	void SetAddressBar(BOOL bNewValue);
//	BOOL GetStatusBar() ;
//	void SetStatusBar(BOOL bNewValue);

// Operations
public:
	BOOL m_bIniTravelLog;
	ITravelLogStg * m_pITravelLog;
	int m_nProgress;
	BOOL m_bToClose;
    LARGE_INTEGER m_tLastClick, m_tLastRClick;
	BOOL m_bRClick, m_bLClick;
	BOOL m_bForceResize;
//	BOOL IsIFrame(LPDISPATCH  pDisp , LPCSTR url);
	//history
	int SetHistory(LPCSTR strCurUrl);
	DWORD m_dwLastHash;
	CString m_strLastTitle, m_strTempLastTitle, m_strLastURL;
	int m_nHistoryLength;
	int m_nCurHistory;
	CStringArray m_astrHistory, m_astrHistoryURL;
	//CStringArray m_astrLastURLs; //one page may have several urls.
	int m_nBackSteps;
	BOOL m_forward;
	BOOL m_back;
	CString ext;
	BOOL m_bIniDL;

	CWebBrowser2 m_wndBrowser;
//	CWnd m_wndBrowser;

	void GoBack(int dist=1);
	void GoForward(int dist=1);
	void GoHome();
//	void GoSearch();
	void Navigate(LPCTSTR URL, DWORD dwFlags = 0,
		LPCTSTR lpszTargetFrameName = NULL,
		LPCTSTR lpszHeaders = NULL, LPVOID lpvPostData = NULL,
		DWORD dwPostDataLen = 0);
//	void Navigate2(LPITEMIDLIST pIDL, DWORD dwFlags = 0,
//		LPCTSTR lpszTargetFrameName = NULL);
	void Navigate2(LPCTSTR lpszURL, DWORD dwFlags = 0,
		LPCTSTR lpszTargetFrameName = NULL, LPCTSTR lpszHeaders = NULL,
		LPVOID lpvPostData = NULL, DWORD dwPostDataLen = 0);
//	void Navigate2(LPCTSTR lpszURL, DWORD dwFlags,
//		CByteArray& baPostedData,
//		LPCTSTR lpszTargetFrameName = NULL, LPCTSTR lpszHeader = NULL);
	void Refresh();
	void Refresh2(int nLevel);
	void Stop();
//	void PutProperty(LPCTSTR lpszProperty, const VARIANT& vtValue);
//	void PutProperty(LPCTSTR lpszPropertyName, double dValue);
//	void PutProperty(LPCTSTR lpszPropertyName, LPCTSTR lpszValue);
//	void PutProperty(LPCTSTR lpszPropertyName, long lValue);
//	void PutProperty(LPCTSTR lpszPropertyName, short nValue);
//	BOOL GetProperty(LPCTSTR lpszProperty, CString& strValue);
//	COleVariant GetProperty(LPCTSTR lpszProperty);
	void ExecWB(OLECMDID cmdID, OLECMDEXECOPT cmdexecopt, VARIANT* pvaIn,
		VARIANT* pvaOut);
	BOOL LoadFromResource(LPCTSTR lpszResource);
	BOOL LoadFromResource(UINT nRes);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFixedHtmlView)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	LPDISPATCH glpDisp;
	virtual ~CFixedHtmlView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
//	IWebBrowser2* m_pBrowserApp;

	//2.5 move here
//	CByteArray array;
	CString strURL, strURL2;
	COleSafeArray vPostData;


	// Events
	virtual void OnNavigateComplete2(LPCTSTR strURL);
/*	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags,
		LPCTSTR lpszTargetFrameName, CByteArray& baPostedData,
		LPCTSTR lpszHeaders, BOOL* pbCancel);*/
	//2.5
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags,
		LPCTSTR lpszTargetFrameName, BOOL baPostedData,
		LPCTSTR lpszHeaders, BOOL* pbCancel);
	virtual void OnStatusTextChange(LPCTSTR lpszText);
	virtual void OnProgressChange(long nProgress, long nProgressMax);
	virtual void OnCommandStateChange(long nCommand, BOOL bEnable);
//	virtual void OnDownloadBegin();
//	virtual void OnDownloadComplete();
	virtual void OnTitleChange(LPCTSTR lpszText);
//	virtual void OnPropertyChange(LPCTSTR lpszProperty);
	virtual void OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel);
	virtual void OnDocumentComplete(LPCTSTR lpszURL);
//	virtual void OnQuit();
//	virtual void OnVisible(BOOL bVisible);
//	virtual void OnToolBar(BOOL bToolBar);
//	virtual void OnMenuBar(BOOL bMenuBar);
//	virtual void OnStatusBar(BOOL bStatusBar);
//	virtual void OnFullScreen(BOOL bFullScreen);
//	virtual void OnTheaterMode(BOOL bTheaterMode);
//328
//	virtual void OnError(SCODE scode, LPCTSTR lpszDescription, UINT nHelpID);

// Event reflectors (not normally overridden)
protected:
	virtual void NavigateComplete2(LPDISPATCH pDisp, VARIANT* URL);
	virtual void BeforeNavigate2(LPDISPATCH pDisp, VARIANT* URL,
		VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData,
		VARIANT* Headers,   BOOL* Cancel);
	virtual void DocumentComplete(LPDISPATCH pDisp, VARIANT* URL);

	// Generated message map functions
protected:
	//{{AFX_MSG(CFixedHtmlView)
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL IsDownloadable(CString& URL);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIXEDHTMLVIEW_H__8BCE18F1_197D_11D4_9BCF_0000E85300AE__INCLUDED_)
