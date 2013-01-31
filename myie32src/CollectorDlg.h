#if !defined(AFX_COLLECTORDLG_H__1D7D80A0_279E_44C1_A994_2D26BD2E42BC__INCLUDED_)
#define AFX_COLLECTORDLG_H__1D7D80A0_279E_44C1_A994_2D26BD2E42BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CollectorDlg.h : header file
//
#include "TxtDropTarget.h"

/////////////////////////////////////////////////////////////////////////////
// CCollectorDlg dialog

class CCollectorDlg : public CDialog
{
// Construction
public:
	CCollectorDlg(CWnd* pParent = NULL);   // standard constructor
	CTxtDropTarget	ms_dropTarget;

// Dialog Data
	//{{AFX_DATA(CCollectorDlg)
	enum { IDD = IDD_COLLECTOR };
	CEdit	m_Editor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCollectorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCollectorDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void SaveCollector();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLLECTORDLG_H__1D7D80A0_279E_44C1_A994_2D26BD2E42BC__INCLUDED_)
