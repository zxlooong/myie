// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__19E497C3_4ECF_11D3_9B1D_0000E85300AE__INCLUDED_)
#define AFX_CHILDFRM_H__19E497C3_4ECF_11D3_9B1D_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_bForceActive;
	BOOL m_bSkipSel;
	int nCurTabID;
	CString m_strStickName;
	CChildFrame* m_pBlankChild;
    LARGE_INTEGER m_tBlankChild;
//	BOOL m_bFirst;
	CString m_Title;
	CMyIEView* m_pView;
	BOOL bLock;
	BOOL bProtect;
	CString m_strPsw;
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CChildFrame)
	afx_msg void OnClose();
//	afx_msg void OnDestroy();
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__19E497C3_4ECF_11D3_9B1D_0000E85300AE__INCLUDED_)
