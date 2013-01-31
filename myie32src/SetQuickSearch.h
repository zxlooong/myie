#if !defined(AFX_SETQUICKSEARCH_H__5705A970_69DD_4D94_AE89_69034EC3A45F__INCLUDED_)
#define AFX_SETQUICKSEARCH_H__5705A970_69DD_4D94_AE89_69034EC3A45F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetQuickSearch.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetQuickSearch dialog

class CSetQuickSearch : public CDialog
{
// Construction
public:
	CSetQuickSearch(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetQuickSearch)
	enum { IDD = IDD_QUICKSEARCH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetQuickSearch)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetQuickSearch)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETQUICKSEARCH_H__5705A970_69DD_4D94_AE89_69034EC3A45F__INCLUDED_)
