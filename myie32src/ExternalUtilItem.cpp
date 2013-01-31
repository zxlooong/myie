// ExternalUtilItem.cpp: implementation of the CExternalUtilItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "myie.h"
#include "ExternalUtilItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//#pragma optimize( "s", on )

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExternalUtilItem::CExternalUtilItem()
{
		m_iProcess = 0;
}

CExternalUtilItem::~CExternalUtilItem()
{
}

//////////////////////////////////////////////////////////////////////
// CExternalUtilList Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExternalUtilList::CExternalUtilList()
{

}

CExternalUtilList::~CExternalUtilList()
{
	RemoveAll();
}

void CExternalUtilList::RemoveAll()
{
	CExternalUtilItem* eui;
	for (int i=0;i <= m_UtilList.GetUpperBound();i++)
	{
	   eui = m_UtilList.GetAt(i);
	   delete eui;
	}
	m_UtilList.RemoveAll();
}

void CExternalUtilList::Clone(CExternalUtilList *peul)
{
	RemoveAll();
	CExternalUtilItem* eui, *cpy;
	for (int i=0;i <= peul->m_UtilList.GetUpperBound();i++)
	{
	   eui = peul->m_UtilList.GetAt(i);
	   cpy = new CExternalUtilItem;
	   cpy->m_strUtilName = eui->m_strUtilName;
	   cpy->m_strUtilCmd = eui->m_strUtilCmd;
	   cpy->m_iProcess = eui->m_iProcess;
	   cpy->m_bUtilStart = eui->m_bUtilStart;
	   cpy->m_bUtilClose = eui->m_bUtilClose;
	   m_UtilList.Add(cpy);
	}
}


//#pragma optimize( "s", off)
