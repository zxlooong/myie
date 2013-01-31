#if !defined(AFX_GROUPURLDLG_H__4D1B9EF4_D552_11D4_9B6A_0000E85300AE__INCLUDED_)
#define AFX_GROUPURLDLG_H__4D1B9EF4_D552_11D4_9B6A_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GroupUrlDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGroupUrlDlg dialog

class CGroupUrlDlg : public CDialog
{
// Construction
public:
	BOOL m_bNotEmpty;
	CGroupUrlDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGroupUrlDlg)
	enum { IDD = IDD_GROUP_URL };
	CEdit	m_edName;
	CEdit	m_edURL;
	CButton	m_btnOK;
	CString	m_strName;
	CString	m_strUrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGroupUrlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGroupUrlDlg)
	afx_msg void OnChangeName();
	afx_msg void OnChangeUrl();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GROUPURLDLG_H__4D1B9EF4_D552_11D4_9B6A_0000E85300AE__INCLUDED_)
