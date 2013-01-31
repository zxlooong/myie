// MyIEOptions.cpp : implementation file
//

#include "stdafx.h"
#include "myie.h"
#include "MyIEOptions.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#pragma optimize( "s", on )


/////////////////////////////////////////////////////////////////////////////
// CWindowProp property page

IMPLEMENT_DYNCREATE(CWindowProp, CPropertyPage)

CWindowProp::CWindowProp() : CPropertyPage(CWindowProp::IDD)
{
	//{{AFX_DATA_INIT(CWindowProp)
	m_bActiveNew = FALSE;
	m_nOpenNew = -1;
	m_nSeq = -1;
	m_bOpenFavInNew = FALSE;
	m_bOpenHisInNew = FALSE;
	m_bOpenLinkInNew = FALSE;
	m_bOpenFileInNew = FALSE;
	m_bOpenAddrInNew = FALSE;
	m_nInterval = 0;
	m_nMaxWins = 0;
	m_nOpenNew2 = -1;
	//}}AFX_DATA_INIT
}

CWindowProp::~CWindowProp()
{
}

void CWindowProp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWindowProp)
	DDX_Check(pDX, IDC_ACTIVATE_NEWWIN, m_bActiveNew);
	DDX_Radio(pDX, IDC_BLANKWIN, m_nOpenNew);
	DDX_Radio(pDX, IDC_DEFAULT_SEQ, m_nSeq);
	DDX_Check(pDX, IDC_FAV_LNK, m_bOpenFavInNew);
	DDX_Check(pDX, IDC_HISTORY, m_bOpenHisInNew);
	DDX_Check(pDX, IDC_LINKSINPAGE, m_bOpenLinkInNew);
	DDX_Check(pDX, IDC_LOCALFILE, m_bOpenFileInNew);
	DDX_Check(pDX, IDC_NEW_ADDRESS, m_bOpenAddrInNew);
	DDX_Text(pDX, IDC_INTERVAL, m_nInterval);
	DDX_Text(pDX, IDC_MAXWIN, m_nMaxWins);
	DDX_Radio(pDX, IDC_BLANKWIN2, m_nOpenNew2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWindowProp, CPropertyPage)
	//{{AFX_MSG_MAP(CWindowProp)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindowProp message handlers
/////////////////////////////////////////////////////////////////////////////
// CTabProp property page

IMPLEMENT_DYNCREATE(CTabProp, CPropertyPage)

CTabProp::CTabProp() : CPropertyPage(CTabProp::IDD)
{
	//{{AFX_DATA_INIT(CTabProp)
	m_bAutoFitTab = FALSE;
	m_bNextTab = FALSE;
	m_nMaxChars = 0;
	m_nMaxWidth = 0;
	m_nMinWidth = 0;
	m_bMultiTab = FALSE;
	m_nTabPos = -1;
	m_nLDBClick = -1;
	m_nMCLICK = -1;
	m_nRClick = -1;
	m_bFBTab = FALSE;
	//}}AFX_DATA_INIT
}

CTabProp::~CTabProp()
{
}

void CTabProp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabProp)
	DDX_Check(pDX, IDC_AUTOFIT_TAB, m_bAutoFitTab);
	DDX_Check(pDX, IDC_DISP_NEXT_TAB, m_bNextTab);
	DDX_Text(pDX, IDC_MAXCHARS, m_nMaxChars);
	DDX_Text(pDX, IDC_MAXWIDTH, m_nMaxWidth);
	DDX_Text(pDX, IDC_MINWIDTH, m_nMinWidth);
	DDX_Check(pDX, IDC_MULTILINE_TAB, m_bMultiTab);
	DDX_Radio(pDX, IDC_BOTTOM_TAB, m_nTabPos);
	DDX_CBIndex(pDX, IDC_LDBCLICK, m_nLDBClick);
	DDX_CBIndex(pDX, IDC_MCLIK, m_nMCLICK);
	DDX_CBIndex(pDX, IDC_RCLIK, m_nRClick);
	DDX_Check(pDX, IDC_FLATBUTAB, m_bFBTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabProp, CPropertyPage)
	//{{AFX_MSG_MAP(CTabProp)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabProp message handlers
/////////////////////////////////////////////////////////////////////////////
// CGeneralProp property page

IMPLEMENT_DYNCREATE(CGeneralProp, CPropertyPage)

CGeneralProp::CGeneralProp() : CPropertyPage(CGeneralProp::IDD)
{
	//{{AFX_DATA_INIT(CGeneralProp)
	m_bConfirmExit = FALSE;
	m_bCompactFavBar = FALSE;
	m_bConfirmCloseAll = FALSE;
	m_bAniIcon = FALSE;
	m_bDisplayOpenAll = FALSE;
	m_bKeepSilent = FALSE;
	m_nMenuSize = 0;
	m_bOneInstance = FALSE;
	m_bSingleMenu = FALSE;
	m_bSysTray = FALSE;
	m_bAddFav = FALSE;
	m_bMouseGesture = FALSE;
	m_bLockHomepage = FALSE;
	m_strSkinPath = _T("");
	m_strGroupPath = _T("");
	//}}AFX_DATA_INIT
}

CGeneralProp::~CGeneralProp()
{
}

void CGeneralProp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGeneralProp)
	DDX_Check(pDX, IDC_COMFIRMEXIT, m_bConfirmExit);
	DDX_Check(pDX, IDC_COMPACTFAVBAR, m_bCompactFavBar);
	DDX_Check(pDX, IDC_CONFIRMCLOSEALL, m_bConfirmCloseAll);
	DDX_Check(pDX, IDC_DISP_ANIICON, m_bAniIcon);
	DDX_Check(pDX, IDC_DISPLAY_OPENALL, m_bDisplayOpenAll);
	DDX_Check(pDX, IDC_KEEPSILENT, m_bKeepSilent);
	DDX_Text(pDX, IDC_MAXMENU, m_nMenuSize);
	DDX_Check(pDX, IDC_ONLYONE, m_bOneInstance);
	DDX_Check(pDX, IDC_SINGLEMENU, m_bSingleMenu);
	DDX_Check(pDX, IDC_SYSTRAY, m_bSysTray);
	DDX_Check(pDX, IDC_ADDFAV, m_bAddFav);
	DDX_Check(pDX, IDC_MOUSEGESTURE, m_bMouseGesture);
	DDX_Check(pDX, IDC_LOCKHOME, m_bLockHomepage);
	DDX_Text(pDX, IDC_SKINPATH, m_strSkinPath);
	DDX_Text(pDX, IDC_GROUP_PATH, m_strGroupPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGeneralProp, CPropertyPage)
	//{{AFX_MSG_MAP(CGeneralProp)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeneralProp message handlers
/////////////////////////////////////////////////////////////////////////////
// CDownloadProp property page

IMPLEMENT_DYNCREATE(CDownloadProp, CPropertyPage)

CDownloadProp::CDownloadProp() : CPropertyPage(CDownloadProp::IDD)
{
	//{{AFX_DATA_INIT(CDownloadProp)
	m_bDLActivex = FALSE;
	m_bDLApplet = FALSE;
	m_bDLImage = FALSE;
	m_bDLScript = FALSE;
	m_bDLSound = FALSE;
	m_bDLVideo = FALSE;
	m_nSelectedID = -1;
	m_bDLAnimation = FALSE;
	//}}AFX_DATA_INIT
}

CDownloadProp::~CDownloadProp()
{
}

void CDownloadProp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDownloadProp)
	DDX_Control(pDX, IDC_SELECTTOOLS, m_SelectTools);
	DDX_Check(pDX, IDC_DLACTIVEX, m_bDLActivex);
	DDX_Check(pDX, IDC_DLAPPLET, m_bDLApplet);
	DDX_Check(pDX, IDC_DLIMG, m_bDLImage);
	DDX_Check(pDX, IDC_DLJS, m_bDLScript);
	DDX_Check(pDX, IDC_DLSOUND, m_bDLSound);
	DDX_Check(pDX, IDC_DLVIDEO, m_bDLVideo);
	DDX_CBIndex(pDX, IDC_SELECTTOOLS, m_nSelectedID);
	DDX_Check(pDX, IDC_PLAY_ANIMATION, m_bDLAnimation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDownloadProp, CPropertyPage)
	//{{AFX_MSG_MAP(CDownloadProp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDownloadProp message handlers
/////////////////////////////////////////////////////////////////////////////
// CStartupProp property page
/*
IMPLEMENT_DYNCREATE(CStartupProp, CPropertyPage)

CStartupProp::CStartupProp() : CPropertyPage(CStartupProp::IDD)
{
	//{{AFX_DATA_INIT(CStartupProp)
	m_nStartup = -1;
	//}}AFX_DATA_INIT
}

CStartupProp::~CStartupProp()
{
}

void CStartupProp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStartupProp)
	DDX_Radio(pDX, IDC_START_BLANK, m_nStartup);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStartupProp, CPropertyPage)
	//{{AFX_MSG_MAP(CStartupProp)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/
/////////////////////////////////////////////////////////////////////////////
// CStartupProp message handlers
/////////////////////////////////////////////////////////////////////////////
// COtherProp property page

IMPLEMENT_DYNCREATE(COtherProp, CPropertyPage)

COtherProp::COtherProp() : CPropertyPage(COtherProp::IDD)
{
	//{{AFX_DATA_INIT(COtherProp)
	m_strCE1 = _T("");
	m_strCE2 = _T("");
	m_strCSE1 = _T("");
	m_strCSE2 = _T("");
	m_strSE1 = _T("");
	m_strSE2 = _T("");
	m_nStartup = -1;
	//}}AFX_DATA_INIT
}

COtherProp::~COtherProp()
{
}

void COtherProp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COtherProp)
	DDX_Control(pDX, IDC_ADDRS, m_Addresses);
	DDX_Text(pDX, IDC_CE1, m_strCE1);
	DDX_Text(pDX, IDC_CE2, m_strCE2);
	DDX_Text(pDX, IDC_CSE1, m_strCSE1);
	DDX_Text(pDX, IDC_CSE2, m_strCSE2);
	DDX_Text(pDX, IDC_SE1, m_strSE1);
	DDX_Text(pDX, IDC_SE2, m_strSE2);
	DDX_Radio(pDX, IDC_START_BLANK, m_nStartup);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COtherProp, CPropertyPage)
	//{{AFX_MSG_MAP(COtherProp)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_DELETEALL, OnDeleteall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COtherProp message handlers



BOOL CDownloadProp::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_SelectTools.AddString("              ");
	for(int i=0; i<=((CMainFrame*)pMainFrame)->m_DList.m_DownloadTools.GetUpperBound(); i++)
	{
		m_SelectTools.AddString(((CMainFrame*)pMainFrame)->m_DList.m_DownloadTools.GetAt(i)->DName);
	}

	m_SelectTools.SetCurSel(m_nSelectedID);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// CSaveProp property page

IMPLEMENT_DYNCREATE(CSaveProp, CPropertyPage)

CSaveProp::CSaveProp() : CPropertyPage(CSaveProp::IDD)
{
	//{{AFX_DATA_INIT(CSaveProp)
	m_nAutoSave = -1;
	m_bCateWithSite = FALSE;
	m_strImagePath = _T("");
	m_strSavePath = _T("");
	//}}AFX_DATA_INIT
}

CSaveProp::~CSaveProp()
{
}

void CSaveProp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveProp)
	DDX_CBIndex(pDX, IDC_AUTOSAVE, m_nAutoSave);
	DDX_Check(pDX, IDC_CATEWITHSITE, m_bCateWithSite);
	DDX_Text(pDX, IDC_IMAGEPATH, m_strImagePath);
	DDX_Text(pDX, IDC_SAVEPATH, m_strSavePath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSaveProp, CPropertyPage)
	//{{AFX_MSG_MAP(CSaveProp)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaveProp message handlers


//#pragma optimize( "s", off)

BOOL COtherProp::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	TCHAR           sz[MAX_PATH];

	COMBOBOXEXITEM item;
	item.mask = CBEIF_TEXT;
	item.pszText = (LPTSTR)sz;
	item.cchTextMax = MAX_PATH;

	int nc = pMainFrame->m_wndAddress->SendMessage(CB_GETCOUNT, 0, 0);
	for(int i=0;i<nc;i++)
	{
		item.iItem = i;
		pMainFrame->m_wndAddress->GetItem(&item);
		m_Addresses.AddString(sz);
	}
	
	m_Addresses.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COtherProp::OnDelete() 
{
	// TODO: Add your control notification handler code here
	int n = m_Addresses.GetCurSel();
	if(n>=0)
	{
		m_Addresses.DeleteString(n);
		m_Addresses.SetCurSel(n);
	}
}

void COtherProp::OnDeleteall() 
{
	// TODO: Add your control notification handler code here
	m_Addresses.ResetContent();
}

void COtherProp::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	TCHAR           sz[MAX_PATH];

	COMBOBOXEXITEM item;
	item.mask = CBEIF_TEXT|CBEIF_IMAGE|CBEIF_SELECTEDIMAGE ;
	item.iImage = 1;
	item.iSelectedImage = 1;
	item.pszText = (LPTSTR)sz;
	item.cchTextMax = MAX_PATH;

	int nc = m_Addresses.GetCount();
	pMainFrame->m_wndAddress->ResetContent();

	for(int i=0;i<nc;i++)
	{
		item.iItem = i;
		m_Addresses.GetLBText(i, sz);
		pMainFrame->m_wndAddress->InsertItem(&item);
	}	
	pMainFrame->SaveTypedURL();

	CPropertyPage::OnOK();
}
