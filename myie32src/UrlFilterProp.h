#if !defined(AFX_URLFILTERPROP_H__F106DCC1_C4D3_11D5_9BB3_8154E77F87B7__INCLUDED_)
#define AFX_URLFILTERPROP_H__F106DCC1_C4D3_11D5_9BB3_8154E77F87B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UrlFilterProp.h : header file
//

extern CMapStringToPtr mapUrlFilter;
extern CStringArray aUrlPattern;

/////////////////////////////////////////////////////////////////////////////
// CUrlFilterProp dialog

class CUrlFilterProp : public CPropertyPage
{
	DECLARE_DYNCREATE(CUrlFilterProp)

// Construction
public:
	CUrlFilterProp();
	~CUrlFilterProp();

// Dialog Data
	//{{AFX_DATA(CUrlFilterProp)
	enum { IDD = IDD_URL_FILTER };
	CListCtrl	m_cURLList;
	BOOL	m_bUseUrlFilter;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CUrlFilterProp)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CUrlFilterProp)
	afx_msg void OnInsert();
	afx_msg void OnEdit();
	afx_msg void OnDelete();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkUrlList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditUrlList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_URLFILTERPROP_H__F106DCC1_C4D3_11D5_9BB3_8154E77F87B7__INCLUDED_)
