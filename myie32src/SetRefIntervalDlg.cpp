// SetRefIntervalDlg.cpp : implementation file
//

#include "stdafx.h"
#include "myie.h"
#include "SetRefIntervalDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// CSetRefIntervalDlg dialog


CSetRefIntervalDlg::CSetRefIntervalDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetRefIntervalDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetRefIntervalDlg)
	m_nRefInt = 0;
	//}}AFX_DATA_INIT
}


void CSetRefIntervalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetRefIntervalDlg)
	DDX_Text(pDX, IDC_REFINTERVAL, m_nRefInt);
	DDV_MinMaxUInt(pDX, m_nRefInt, 1, 10000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetRefIntervalDlg, CDialog)
	//{{AFX_MSG_MAP(CSetRefIntervalDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetRefIntervalDlg message handlers

//#pragma optimize( "s", off )

