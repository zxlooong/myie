// ProxyInfo.cpp : implementation file
//

#include "stdafx.h"
#include "myie.h"
#include "ProxyInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CProxyInfo dialog


CProxyInfo::CProxyInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CProxyInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProxyInfo)
	m_ProxyName = _T("");
	m_Proxy = _T("");
	m_Speed = 3;
	//}}AFX_DATA_INIT
}


void CProxyInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProxyInfo)
	DDX_Text(pDX, IDC_NAME, m_ProxyName);
	DDX_Text(pDX, IDC_PROXY, m_Proxy);
	DDX_CBIndex(pDX, IDC_SPEED, m_Speed);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProxyInfo, CDialog)
	//{{AFX_MSG_MAP(CProxyInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProxyInfo message handlers


