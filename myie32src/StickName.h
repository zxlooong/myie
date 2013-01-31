#if !defined(AFX_STICKNAME_H__657745E1_8678_11D4_9BB1_859E490CD578__INCLUDED_)
#define AFX_STICKNAME_H__657745E1_8678_11D4_9BB1_859E490CD578__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// StickName.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStickName dialog

class CStickName : public CDialog
{
// Construction
public:
	CStickName(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStickName)
	enum { IDD = IDD_STICKNAME };
	CString	m_strStickName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStickName)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStickName)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STICKNAME_H__657745E1_8678_11D4_9BB1_859E490CD578__INCLUDED_)
