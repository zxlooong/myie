// SetQuickSearch.cpp : implementation file
//

#include "stdafx.h"
#include "myie.h"
#include "SetQuickSearch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// CSetQuickSearch dialog


CSetQuickSearch::CSetQuickSearch(CWnd* pParent /*=NULL*/)
	: CDialog(CSetQuickSearch::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetQuickSearch)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSetQuickSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetQuickSearch)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetQuickSearch, CDialog)
	//{{AFX_MSG_MAP(CSetQuickSearch)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetQuickSearch message handlers

#pragma optimize( "s", off )

