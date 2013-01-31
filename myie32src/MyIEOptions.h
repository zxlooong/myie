#if !defined(AFX_MYIEOPTIONS_H__9DC8E053_7252_11D4_9BF5_0000E85300AE__INCLUDED_)
#define AFX_MYIEOPTIONS_H__9DC8E053_7252_11D4_9BF5_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
// CWindowProp dialog

class CWindowProp : public CPropertyPage
{
	DECLARE_DYNCREATE(CWindowProp)

// Construction
public:
	CWindowProp();
	~CWindowProp();

// Dialog Data
	//{{AFX_DATA(CWindowProp)
	enum { IDD = IDD_PROPPAGE_WINDOW };
	BOOL	m_bActiveNew;
	int		m_nOpenNew;
	int		m_nSeq;
	BOOL	m_bOpenFavInNew;
	BOOL	m_bOpenHisInNew;
	BOOL	m_bOpenLinkInNew;
	BOOL	m_bOpenFileInNew;
	BOOL	m_bOpenAddrInNew;
	UINT	m_nInterval;
	UINT	m_nMaxWins;
	int		m_nOpenNew2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWindowProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWindowProp)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CTabProp dialog

class CTabProp : public CPropertyPage
{
	DECLARE_DYNCREATE(CTabProp)

// Construction
public:
	CTabProp();
	~CTabProp();

// Dialog Data
	//{{AFX_DATA(CTabProp)
	enum { IDD = IDD_PROPPAGE_TAB };
	BOOL	m_bAutoFitTab;
	BOOL	m_bNextTab;
	UINT	m_nMaxChars;
	UINT	m_nMaxWidth;
	UINT	m_nMinWidth;
	BOOL	m_bMultiTab;
	int		m_nTabPos;
	int		m_nLDBClick;
	int		m_nMCLICK;
	int		m_nRClick;
	BOOL	m_bFBTab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTabProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTabProp)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CGeneralProp dialog

class CGeneralProp : public CPropertyPage
{
	DECLARE_DYNCREATE(CGeneralProp)

// Construction
public:
	CGeneralProp();
	~CGeneralProp();

// Dialog Data
	//{{AFX_DATA(CGeneralProp)
	enum { IDD = IDD_PROPPAGE_GENERAL };
	BOOL	m_bConfirmExit;
	BOOL	m_bCompactFavBar;
	BOOL	m_bConfirmCloseAll;
	BOOL	m_bAniIcon;
	BOOL	m_bDisplayOpenAll;
	BOOL	m_bKeepSilent;
	UINT	m_nMenuSize;
	BOOL	m_bOneInstance;
	BOOL	m_bSingleMenu;
	BOOL	m_bSysTray;
	BOOL	m_bAddFav;
	BOOL	m_bMouseGesture;
	BOOL	m_bLockHomepage;
	CString	m_strSkinPath;
	CString	m_strGroupPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CGeneralProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CGeneralProp)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CDownloadProp dialog

class CDownloadProp : public CPropertyPage
{
	DECLARE_DYNCREATE(CDownloadProp)

// Construction
public:
	CDownloadProp();
	~CDownloadProp();

// Dialog Data
	//{{AFX_DATA(CDownloadProp)
	enum { IDD = IDD_PROPPAGE_DOWNLOAD };
	CComboBox	m_SelectTools;
	BOOL	m_bDLActivex;
	BOOL	m_bDLApplet;
	BOOL	m_bDLImage;
	BOOL	m_bDLScript;
	BOOL	m_bDLSound;
	BOOL	m_bDLVideo;
	int		m_nSelectedID;
	BOOL	m_bDLAnimation;
	int		m_nAutoSave;
	BOOL	m_bCateWithSite;
	CString	m_strImagePath;
	CString	m_strSavePath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDownloadProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDownloadProp)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CStartupProp dialog

/*class CStartupProp : public CPropertyPage
{
	DECLARE_DYNCREATE(CStartupProp)

// Construction
public:
	CStartupProp();
	~CStartupProp();

// Dialog Data
	//{{AFX_DATA(CStartupProp)
	enum { IDD = IDD_PROPPAGE_STARTUP };
	int		m_nStartup;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStartupProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CStartupProp)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};*/
/////////////////////////////////////////////////////////////////////////////
// COtherProp dialog

class COtherProp : public CPropertyPage
{
	DECLARE_DYNCREATE(COtherProp)

// Construction
public:
	COtherProp();
	~COtherProp();

// Dialog Data
	//{{AFX_DATA(COtherProp)
	enum { IDD = IDD_PROPPAGE_OTHERS };
	CComboBox	m_Addresses;
	CString	m_strCE1;
	CString	m_strCE2;
	CString	m_strCSE1;
	CString	m_strCSE2;
	CString	m_strSE1;
	CString	m_strSE2;
	int		m_nStartup;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COtherProp)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COtherProp)
	virtual BOOL OnInitDialog();
	afx_msg void OnDelete();
	afx_msg void OnDeleteall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CSaveProp dialog

class CSaveProp : public CPropertyPage
{
	DECLARE_DYNCREATE(CSaveProp)

// Construction
public:
	CSaveProp();
	~CSaveProp();

// Dialog Data
	//{{AFX_DATA(CSaveProp)
	enum { IDD = IDD_PROPPAGE_SAVE };
	CString	m_strSavePath;
	CString	m_strImagePath;
	int		m_nAutoSave;
	BOOL	m_bCateWithSite;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSaveProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSaveProp)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYIEOPTIONS_H__9DC8E053_7252_11D4_9BF5_0000E85300AE__INCLUDED_)
