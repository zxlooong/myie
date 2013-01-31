#if !defined(AFX_EXTERNALUTILS_H__85FDD5A5_1357_11D4_9BCD_0000E85300AE__INCLUDED_)
#define AFX_EXTERNALUTILS_H__85FDD5A5_1357_11D4_9BCD_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExternalUtils.h : header file
//

#include "ExternalUtilItem.h"

/////////////////////////////////////////////////////////////////////////////
// CExternalUtils dialog

class CExternalUtils : public CDialog
{
// Construction
public:
	BOOL m_bSkip;
	CExternalUtils(CWnd* pParent = NULL);   // standard constructor
	CExternalUtilList m_ExternalList;

// Dialog Data
	//{{AFX_DATA(CExternalUtils)
	enum { IDD = IDD_EXTERN_UTIL };
	CButton	m_cStartup;
	CButton	m_cClose;
	CEdit	m_cName;
	CEdit	m_cCommand;
	CListCtrl	m_UtilList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExternalUtils)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExternalUtils)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnAddUtil();
	afx_msg void OnModUtil();
	afx_msg void OnDelUtil();
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditExternUtil(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedExternUtil(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBrowserFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTERNALUTILS_H__85FDD5A5_1357_11D4_9BCD_0000E85300AE__INCLUDED_)
