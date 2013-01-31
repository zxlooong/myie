#if !defined(AFX_CSEARCHBAR_H__CC415E43_148C_11D4_9BB0_CFFDB04BA878__INCLUDED_)
#define AFX_CSEARCHBAR_H__CC415E43_148C_11D4_9BB0_CFFDB04BA878__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//  : header file
//

#include "BaseExpObj.h"
#include "HtmlWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CHistoryTree window

class CSearchbar : public CBaseExpObj
{
// Construction
public:
	CSearchbar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHistoryTree)
	//}}AFX_VIRTUAL
	CHtmlWnd m_wndSearch;

// Implementation
public:
	BOOL Update(int nLevel);
	virtual void Destroy();
	LPCSTR GetTitle();
	virtual ~CSearchbar();
	CString strTitle;
	virtual BOOL Initialize();
	virtual BOOL Create(CWnd* pParent);
	virtual CWnd* GetWindow();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSearchbar)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTORYTREE_H__CC415E43_148C_11D4_9BB0_CFFDB04BA878__INCLUDED_)
