// StickName.cpp : implementation file
//

#include "stdafx.h"
#include "myie.h"
#include "StickName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// CStickName dialog


CStickName::CStickName(CWnd* pParent /*=NULL*/)
	: CDialog(CStickName::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStickName)
	m_strStickName = _T("");
	//}}AFX_DATA_INIT
}


void CStickName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStickName)
	DDX_Text(pDX, IDC_STICKNAME, m_strStickName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStickName, CDialog)
	//{{AFX_MSG_MAP(CStickName)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStickName message handlers

//#pragma optimize( "s", off )

