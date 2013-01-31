// MyIEView.h : interface of the CMyIEView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MyIEVIEW_H__19E497C7_4ECF_11D3_9B1D_0000E85300AE__INCLUDED_)
#define AFX_MyIEVIEW_H__19E497C7_4ECF_11D3_9B1D_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FixedHtmlView.h"
#include "AddressBarEx.h"

#define STOP_TIMER 950

class CMyIEView : public CFixedHtmlView
{
protected: // create from serialization only
	DECLARE_DYNCREATE(CMyIEView)

// Attributes
public:
//	CMyIEDoc* GetDocument();
	CMyIEView();

	BOOL PopFilterStar(LPCSTR lpszURL);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyIEView)
	public:
	virtual void OnInitialUpdate(int nOpenType, LPCSTR strUrl); // called first time after construct
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel);
//	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, BOOL baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	virtual void OnTitleChange(LPCTSTR lpszText);
	virtual void OnDocumentComplete(LPCTSTR lpszUrl);
	virtual void OnProgressChange(long nProgress, long nProgressMax);
	virtual void OnStatusTextChange(LPCTSTR lpszText);
	virtual BOOL OnAmbientProperty(COleControlSite* pSite, DISPID dispid, VARIANT* pvar);
	virtual void OnCommandStateChange(long nCommand, BOOL bEnable);
	virtual void OnNavigateComplete2(LPCTSTR lpszURL);
//	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL MoveDirection(CPoint& point, char* Direction);
	void ExecCommand(DWORD nCmdID);
	BOOL m_bIsCapture;
	BOOL m_bIsSecure;
	BOOL m_bAutoFilter;
	BOOL m_bReUsed;
	int m_nFilterDup;
	int m_nDupID;
	void ChangeRefresh();

	void ChangeProperty();
	int oldIcon;
	void ChangeSpeed(int nSpeed);
	int m_nSrSpeed; //1-slow, 2-medium, 3-fast
	BOOL m_bScrollAfterDL;
	BOOL m_bUseAutoScroll, m_bUseAutoRef;
	UINT m_nScrollTimer, m_nAutoRefreshTimer;
	UINT m_nRefreshInterval;

	BOOL m_bFirst;
	BOOL m_bForceRefresh;
	void ToNavigate(LPCTSTR URL, DWORD dwFlags = 0, LPCTSTR lpszTargetFrameName = NULL,  LPCTSTR lpszHeaders = NULL, LPVOID lpvPostData = NULL,  DWORD dwPostDataLen = 0, BOOL bOnlyRefresh = FALSE);
	BOOL bDelayAdd;
	UINT m_nFontSize;
	BOOL m_IsNewPopup;
	CString m_lpszUrl;
	CString m_strPossibleURL;
	CString m_strWebProxy;
	BOOL m_bEnWebProxy;
	DWORD dwProperty;
	BOOL IsPopUp;
	BOOL IsActive;
	UINT m_nLastScrSpd;
	virtual ~CMyIEView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	
protected:
//	CString url, url2;
	CString strUrl, str, str2;//, tag, strUrl2


// Generated message map functions
protected:
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditFind();
	afx_msg void OnEditPaste();
	afx_msg void OnEditSelectAll();
	afx_msg void OnOptionsLoadimage();
	afx_msg void OnUpdateOptions(CCmdUI* pCmdUI);
	afx_msg void OnOptionsAllowscripts();
	afx_msg void OnOptionsLoadsounds();
	afx_msg void OnOptionsLoadvideos();
	afx_msg void OnOptionsUseWebp();
	afx_msg void OnUpdateOptionsUseWebp(CCmdUI* pCmdUI);
	afx_msg void OnOptionsAllowactivex();
	afx_msg void OnOptionsAllowjavaapplet();
	afx_msg void OnUpdateGoBack(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGoForward(CCmdUI* pCmdUI);
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdateViewFonts(CCmdUI* pCmdUI);
	//{{AFX_MSG(CMyIEView)
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnToolsAutoref();
	afx_msg void OnUpdateToolsAutoref(CCmdUI* pCmdUI);
	afx_msg void OnToolsReffre();
	afx_msg void OnToolsAutofilterCur();
	afx_msg void OnUpdateToolsAutofilterCur(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveas();
	afx_msg void OnFileSavehtml();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnFilePrintSetup();
	afx_msg void OnFileProperty();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnFileAutosave();
	//}}AFX_MSG
	afx_msg void OnGoBack();
	afx_msg void OnGoForward();
//	afx_msg void OnGoSearchTheWeb();
	afx_msg void OnGoStartPage();
	/*afx_msg void OnHelpWebTutorial();
	afx_msg void OnHelpOnlineSupport();
	afx_msg void OnHelpMicrosoftOnTheWebFreeStuff();
	afx_msg void OnHelpMicrosoftOnTheWebFrequentlyAskedQuestions();
	afx_msg void OnHelpMicrosoftOnTheWebGetFasterInternetAccess();
	afx_msg void OnHelpMicrosoftOnTheWebMicrosoftHomePage();
	afx_msg void OnHelpMicrosoftOnTheWebSearchTheWeb();
	afx_msg void OnHelpMicrosoftOnTheWebSendFeedback();
	afx_msg void OnHelpMicrosoftOnTheWebInternetStartPage();
	afx_msg void OnHelpMicrosoftOnTheWebBestOfTheWeb();*/
//	afx_msg void OnViewFontsLargest();
//	afx_msg void OnViewFontsLarge();
//	afx_msg void OnViewFontsMedium();
//	afx_msg void OnViewFontsSmall();
//	afx_msg void OnViewFontsSmallest();
//	afx_msg void OnFileOpen();
	afx_msg void OnToolsAutoscrollAutoscrollingafterdownloading();
	afx_msg void OnUpdateToolsAutoscrollAutoscrollingafterdownloading(CCmdUI* pCmdUI);
//	afx_msg void OnToolsAutoscrollFast();
	afx_msg void OnUpdateToolsAutoscrollSpeed(CCmdUI* pCmdUI);
//	afx_msg void OnToolsAutoscrollMedium();
//	afx_msg void OnToolsAutoscrollSlow();
	afx_msg void OnToolsAutoscrollUseautoscrolling();
	afx_msg void OnUpdateToolsAutoscrollUseautoscrolling(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
private:
	void PushMouseGesture(char gesture);
	CWnd* m_pCaptured;
	POINT m_LClickPT, m_StartPoint;
	char m_MouseGestures[4], m_SeqMG[4];
	int m_iMGLen;
	int m_iMouseGS, m_iMouseGE;
};

#ifndef _DEBUG  // debug version in MyIEView.cpp
/*inline CMyIEDoc* CMyIEView::GetDocument()
   { return (CMyIEDoc*)m_pDocument; }*/
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MyIEVIEW_H__19E497C7_4ECF_11D3_9B1D_0000E85300AE__INCLUDED_)
