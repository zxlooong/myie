// WebFileOpenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "myie.h"
#include "WebFileOpenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#pragma optimize( "s", on)
/////////////////////////////////////////////////////////////////////////////
// CWebFileOpenDlg dialog


CWebFileOpenDlg::CWebFileOpenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWebFileOpenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWebFileOpenDlg)
	m_strAddress = _T("");
	//}}AFX_DATA_INIT
}


void CWebFileOpenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWebFileOpenDlg)
	DDX_Control(pDX, IDC_ADDRESS, m_conAddress);
	DDX_CBString(pDX, IDC_ADDRESS, m_strAddress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWebFileOpenDlg, CDialog)
	//{{AFX_MSG_MAP(CWebFileOpenDlg)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebFileOpenDlg message handlers
typedef HRESULT (CALLBACK* LPFNDLLFUNC1)(HWND ,DWORD);

BOOL CWebFileOpenDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//CBitmap bitmap;
	CImageList img;
	//bitmap.LoadBitmap(IDB_BITMAP1);
	img.Create(16, 16, ILC_COLORDDB|ILC_MASK, 7, 1);
	HBITMAP hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP1));
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
	m_conAddress.SetImageList(&img);
	img.Detach();
	//bitmap.Detach();


	m_conAddress.SetExtendedStyle(0, m_conAddress.GetExtendedStyle()|CBES_EX_NOSIZELIMIT);

	//auto complete
	HINSTANCE hIns = LoadLibrary("shlwapi.dll");
//		AfxMessageBox(IDS_SHLWAPI_FAIL);
//	else
	if(hIns != NULL)
	{
		LPFNDLLFUNC1 lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hIns, "SHAutoComplete");
		if(lpfnDllFunc1!=NULL)
			lpfnDllFunc1(m_conAddress.GetEditCtrl()->m_hWnd, 0xe);
		FreeLibrary(hIns);
	}

	//typed urls
	TCHAR           sz[MAX_PATH];
	//TCHAR           szPath[MAX_PATH];
	HKEY            hKey;
	DWORD           dwSize;
	TCHAR			id[9] = "url";
	int				i = 1;

	if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\TypedUrls"), &hKey) != ERROR_SUCCESS)
	{
		TRACE0("Typed URLs not found\n");
		return TRUE;
	}
	dwSize = MAX_PATH-1;

	itoa(i, id+3, 10);

	COMBOBOXEXITEM item;

	item.mask = CBEIF_TEXT|CBEIF_IMAGE|CBEIF_SELECTEDIMAGE ;
	item.iImage = 1;
	item.iSelectedImage = 1;
	while(RegQueryValueEx(hKey, _T(id), NULL, NULL, (LPBYTE)sz, &dwSize) == ERROR_SUCCESS)
	{
		item.iItem = i-1;
		item.pszText = (LPTSTR)(LPCTSTR)sz;
		
		m_conAddress.InsertItem(&item);
		i++;
		itoa(i, id+3, 10);
		dwSize = MAX_PATH - 1;
	}
	RegCloseKey(hKey);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWebFileOpenDlg::OnBrowse() 
{
	// TODO: Add your control notification handler code here
	CString str;

	str.LoadString(IDS_FILETYPES);

	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, str);
	if(fileDlg.DoModal() == IDOK)
	{
	//	m_conAddress.InsertString(0, fileDlg.GetPathName());
	//	m_conAddress.SetCurSel(0);
		m_conAddress.GetEditCtrl()->SetWindowText(fileDlg.GetPathName());
	}
}

//#pragma optimize( "s", off)

