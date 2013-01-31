// AddToThisFavDlg.cpp : implementation file
//

#include "stdafx.h"
#include "myie.h"
#include "AddToThisFavDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddToThisFavDlg dialog


CAddToThisFavDlg::CAddToThisFavDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddToThisFavDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddToThisFavDlg)
	m_strFolder = _T("");
	m_strName = _T("");
	m_strURL = _T("");
	//}}AFX_DATA_INIT
}


void CAddToThisFavDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddToThisFavDlg)
	DDX_Text(pDX, IDC_FOLDER, m_strFolder);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_URL, m_strURL);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddToThisFavDlg, CDialog)
	//{{AFX_MSG_MAP(CAddToThisFavDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddToThisFavDlg message handlers
