#if !defined(AFX_INPUTUTIL_H__85FDD5A6_1357_11D4_9BCD_0000E85300AE__INCLUDED_)
#define AFX_INPUTUTIL_H__85FDD5A6_1357_11D4_9BCD_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputUtil.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputUtil dialog

class CInputUtil : public CDialog
{
// Construction
public:
	CInputUtil(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputUtil)
	enum { IDD = IDD_INPUT_UTIL };
	CButton	m_btnStart;
	CButton	m_btnClose;
	CString	m_Command;
	CString	m_Name;
	BOOL	m_bStartup;
	BOOL	m_bClose;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputUtil)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputUtil)
	afx_msg void OnBrowserFile();
	virtual BOOL OnInitDialog();
	afx_msg void OnStartup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTUTIL_H__85FDD5A6_1357_11D4_9BCD_0000E85300AE__INCLUDED_)
