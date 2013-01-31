#if !defined(AFX_LINKLISTDLG_H__0D39EF01_730C_11D5_9BB3_8D1C1810813E__INCLUDED_)
#define AFX_LINKLISTDLG_H__0D39EF01_730C_11D5_9BB3_8D1C1810813E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LinkListDlg.h : header file
//
#include "HtmlWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CLinkListDlg dialog

class CLinkListDlg : public CDialog
{
// Construction
public:
	CLinkListDlg(CWnd* pParent = NULL);   // standard constructor
	CHtmlWnd m_wndLinks;

// Dialog Data
	//{{AFX_DATA(CLinkListDlg)
	enum { IDD = IDD_LINKLIST };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLinkListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLinkListDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINKLISTDLG_H__0D39EF01_730C_11D5_9BB3_8D1C1810813E__INCLUDED_)
