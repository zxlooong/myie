// MyIE.h : main header file for the MyIE application
//

#if !defined(AFX_MyIE_H__19E497BD_4ECF_11D3_9B1D_0000E85300AE__INCLUDED_)
#define AFX_MyIE_H__19E497BD_4ECF_11D3_9B1D_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "MyIE_i.h"

#define WM_USER_SHELL_OPEN (WM_USER + 0x1000) 
#define WM_UPDATE_FAV	(WM_USER+0x1001)
#define WM_UPDATE_TAB	(WM_USER + 0x1002)
#define WM_UPDATE_TAB_TIP (WM_USER + 0x1003)
#define WM_UPDATE_TOOLBAR	(WM_USER + 0x1004)
#define WM_ACTIVATE_WINDOW	(WM_USER + 0x1005)
#define WM_DELAYLOAD_CONFIG	(WM_USER + 0x1006)
#define WM_SEL_TAB	(WM_USER + 0x1007)

/////////////////////////////////////////////////////////////////////////////
// CMyIEApp:
// See MyIE.cpp for the implementation of this class
//

//long LoadBytes();

class CMyIEApp : public CWinApp
{
public:
	CString m_strMyIERoot;
	BOOL m_bNeedCreateDir;
	CString m_strLinkListPath;
	CString m_strQuickSearchPath;
	CString m_strFormDataPath;
	CString m_strGroupPath;
	CString m_strImagePath;

	int m_nAutoStart;
//	POSITION m_posAutoStart;
	int nDelay;
	CString m_strDefaultDir;
	CString m_strBitmapPath;
	CString m_strProfile;
	CString m_strSaveTxtFile;
//	BOOL m_hasEmptyDoc;
	CMyIEApp();
	void SaveConfg();
	void LoadConfg();

	class CImpIDispatch* m_pDispOM; 
	class CImpIDropTarget* m_pDropTarget;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyIEApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMyIEApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bATLInited;
	BOOL InitATL();
	BOOL ThreadOnIdle(LONG lCount);
	BOOL FirstRun();
};

extern CMyIEApp theApp; 



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MyIE_H__19E497BD_4ECF_11D3_9B1D_0000E85300AE__INCLUDED_)
