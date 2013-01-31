// ProxyCategories.cpp: implementation of the CProxyCategories class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "myie.h"
#include "ProxyList.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProxyItem::CProxyItem()
{
	index = -1;
}

CProxyItem::~CProxyItem()
{

}

CProxyList::CProxyList()
{
	m_bIsTransProxy = FALSE;
}

CProxyList::~CProxyList()
{
	CProxyItem* pi;
	for (int i=0;i <= m_Proxies.GetUpperBound();i++)
	{
	   pi = m_Proxies.GetAt(i);
	   delete pi;
	}
	m_Proxies.RemoveAll();
}

CProxyCategory::CProxyCategory()
{

}

CProxyCategory::CProxyCategory(CProxyCategory& pc)
{
	Copy(pc);
}


CProxyCategory::~CProxyCategory()
{
	RemoveAll();
}

void CProxyCategory::RemoveAll()
{
	CProxyList* pl;
	for (int i=0;i <= m_ProxyCategory.GetUpperBound();i++)
	{
	   pl = m_ProxyCategory.GetAt(i);
	   delete pl;
	}
	m_ProxyCategory.RemoveAll();
}

CProxyList* CProxyCategory::GetWebProxyList(BOOL bTrans)
{
	CProxyList* pl;
	for (int i=0;i <= m_ProxyCategory.GetUpperBound();i++)
	{
	   pl = m_ProxyCategory.GetAt(i);
	   if((!bTrans && pl->m_bIsWebProxy && !pl->m_bIsTransProxy) || (bTrans && pl->m_bIsTransProxy))
		   return pl;
	}

	return NULL;
}

int CProxyList::Find(LPCSTR proxy)
{
	for (int i=0;i <= m_Proxies.GetUpperBound();i++)
	{
	   if(m_Proxies.ElementAt(i)->m_strProxy == proxy)
		   return i;
	}
	return -1;
}

void CProxyList::Add(LPCSTR proxy, LPCSTR name, int speed)
{
	try{
	CProxyItem* pi;
	pi = new CProxyItem;
	pi->m_nProxySpeed = speed;
	pi->m_strProxy = proxy;
	pi->m_strProxyName = name;
	m_Proxies.Add(pi);
	}catch(...)
	{
	}
}

/*void CProxyCategory::LoadProxies()
{
	UINT len;
	char *tmp = NULL;
	BOOL r;
	CWinApp* app = AfxGetApp();

	m_ProxyCategory.RemoveAll();
	//load web proxies
	//load new format first
	CProxyList* pl = new CProxyList;
	CProxyItem *pi;
	pl->m_bIsWebProxy = TRUE;
	pl->m_strCategoryName = app->GetProfileString("WebProxy", "CategoryName", "Web Proxy");
	CString wp = app->GetProfileString("WebProxy", "wp1", NULL);
	CString wpn;
	if(wp == "" || wp.IsEmpty())
	{
		//then load old one
		tmp = NULL;
		r = app->GetProfileBinary("Settings","WebProxy", (LPBYTE*)(&tmp), &len);
		if(r)
		{
			//load pop list
			char *token;
			char seps[] = "\n";
			token = strtok( tmp, seps);
			while( token != NULL )
			{
				  /* While there are tokens in "string" * /
				  //((CMainFrame*)m_pMainWnd)->m_astrWebProxy.AddTail(token);
				 // ((CMainFrame*)m_pMainWnd)->m_astrWebProxyName.AddTail(token);
				pi = new CProxyItem;
				pi->m_nProxySpeed = 3;
				pi->m_strProxy = token;
				pi->m_strProxyName = token;
				pl->m_Proxies.Add(pi);
				  /* Get next token: * /
				  token = strtok( NULL, seps );
			}
			delete[] tmp;

		}


		//load webproxy speed
		tmp = NULL;
		r = app->GetProfileBinary("Settings","WebProxySP", (LPBYTE*)(&tmp), &len);
		if(r)
		{
			//load pop list
			for( UINT i=0;i<len;i++ )
			{
				  //((CMainFrame*)m_pMainWnd)->m_aWebProxySpeed.AddTail(tmp[i]);
				pl->m_Proxies.ElementAt(i)->m_nProxySpeed = tmp[i];
			}
			delete[] tmp;

		}
	}
	else
	{
		//load new one
		char Keyname1[6] = "wp", Keyname2[7] = "wps", Keyname3[7] = "wpn";
		int sp, i=1;
		sp = app->GetProfileInt("WebProxy", "wps1", 3);
		wpn = app->GetProfileString("WebProxy", "wpn1", wp);
		while(wp != "" && !wp.IsEmpty())
		{
	//		((CMainFrame*)m_pMainWnd)->m_astrWebProxy.AddTail(wp);
	//		((CMainFrame*)m_pMainWnd)->m_aWebProxySpeed.AddTail(sp);
	//		((CMainFrame*)m_pMainWnd)->m_astrWebProxyName.AddTail(wpn);
			pi = new CProxyItem;
			pi->m_nProxySpeed = sp;
			pi->m_strProxy = wp;
			pi->m_strProxyName = wpn;
			pl->m_Proxies.Add(pi);
			i++;
			itoa(i, Keyname1+2, 10);
			itoa(i, Keyname2+3, 10);
			itoa(i, Keyname3+3, 10);
			wp = app->GetProfileString("WebProxy", Keyname1, NULL);
			sp = app->GetProfileInt("WebProxy", Keyname2, 3);
			wpn = app->GetProfileString("WebProxy", Keyname3, wp);
		}
	}
	if(pl->m_Proxies.GetUpperBound()>0)
		m_ProxyCategory.Add(pl);
	else
		delete pl;

	
	//load proxies
	//load new format first
	wp = app->GetProfileString("Proxy", "CategoryName", NULL);
	if(wp == "" || wp.IsEmpty())
	{
		pl = new CProxyList;
		pl->m_bIsWebProxy = app->GetProfileInt("Proxy", "ProxyType", FALSE);
		pl->m_strCategoryName = app->GetProfileString("Proxy", "CategoryName", "Proxy");
		//then load old one
		tmp = NULL;
		r = app->GetProfileBinary("Settings","Proxy", (LPBYTE*)(&tmp), &len);
		if(r)
		{
			//load pop list
			char *token;
			char seps[] = "\n";
			token = strtok( tmp, seps);
			while( token != NULL )
			{
				  /* While there are tokens in "string" * /
				  //((CMainFrame*)m_pMainWnd)->m_astrWebProxy.AddTail(token);
				 // ((CMainFrame*)m_pMainWnd)->m_astrWebProxyName.AddTail(token);
				pi = new CProxyItem;
				pi->m_nProxySpeed = 3;
				pi->m_strProxy = token;
				pi->m_strProxyName = token;
				pl->m_Proxies.Add(pi);
				  /* Get next token: * /
				  token = strtok( NULL, seps );
			}
			delete[] tmp;

		}


		//load proxy speed
		tmp = NULL;
		r = app->GetProfileBinary("Settings","ProxySP", (LPBYTE*)(&tmp), &len);
		if(r)
		{
			//load pop list
			for( UINT i=0;i<len;i++ )
			{
				  //((CMainFrame*)m_pMainWnd)->m_aWebProxySpeed.AddTail(tmp[i]);
				pl->m_Proxies.ElementAt(i)->m_nProxySpeed = tmp[i];
			}
			delete[] tmp;

		}
		m_ProxyCategory.Add(pl);
	}
	else
	{
		//load new one
		char Keyname1[6] = "p", Keyname2[7] = "ps", Keyname3[7] = "pn";
		int sp, i, l=1;
		char Section[10]="Proxy";
		do{
			i=1;
			sp = app->GetProfileInt(Section, "ps1", 3);
			wpn = app->GetProfileString(Section, "pn1", wp);
			wp = app->GetProfileString(Section, "p1", NULL);
			pl = new CProxyList;
			while(wp != "" && !wp.IsEmpty())
			{
		//		((CMainFrame*)m_pMainWnd)->m_astrWebProxy.AddTail(wp);
		//		((CMainFrame*)m_pMainWnd)->m_aWebProxySpeed.AddTail(sp);
		//		((CMainFrame*)m_pMainWnd)->m_astrWebProxyName.AddTail(wpn);
				pi = new CProxyItem;
				pi->m_nProxySpeed = sp;
				pi->m_strProxy = wp;
				pi->m_strProxyName = wpn;
				pl->m_Proxies.Add(pi);
				i++;
				itoa(i, Keyname1+1, 10);
				itoa(i, Keyname2+2, 10);
				itoa(i, Keyname3+2, 10);
				wp = app->GetProfileString(Section, Keyname1, NULL);
				sp = app->GetProfileInt(Section, Keyname2, 3);
				wpn = app->GetProfileString(Section, Keyname3, wp);
			}
			pl->m_strProxyByPass = app->GetProfileString(Section, "ProxyByPass", ((CMainFrame*)pMainFrame)->m_strProxyByPass);
			pl->m_bIsWebProxy = app->GetProfileInt(Section, "ProxyType", 0);
			pl->m_bIsTransProxy = app->GetProfileInt(Section, "Translation", 0);
			pl->m_strCategoryName = app->GetProfileString(Section, "CategoryName", Section);

			m_ProxyCategory.Add(pl);

			itoa(l, Section+5, 10);
			l++;
			wp = app->GetProfileString(Section, "CategoryName", NULL);

		}while(!wp.IsEmpty());
	}

	if(GetWebProxyList()==NULL)
	{
		pl = new CProxyList;

		pl->m_strProxyByPass = "";
		pl->m_bIsWebProxy = TRUE;
		pl->m_strCategoryName = "Web Proxy";
		m_ProxyCategory.Add(pl);
	}
	if(GetWebProxyList(TRUE)==NULL)
	{
		pl = new CProxyList;
		pi = new CProxyItem;
		pi->m_nProxySpeed=4;
		pi->m_strProxy.LoadString(IDS_DEFAULT_WEB_PROXY);
		pi->m_strProxyName.LoadString(IDS_DEFAULT_WEBP_NAME);
		pl->m_Proxies.Add(pi);

		pl->m_strProxyByPass = "";
		pl->m_bIsWebProxy = TRUE;
		pl->m_bIsTransProxy = TRUE;
		pl->m_strCategoryName = "Translation";
		m_ProxyCategory.Add(pl);
	}
}*/
void CProxyCategory::LoadProxies()
{
	UINT len;
	char *tmp = NULL;
	BOOL r;
	CWinApp* app = AfxGetApp();

	m_ProxyCategory.RemoveAll();

	//load proxies
	//load new format first
	CProxyList* pl;
	CProxyItem *pi;
	CString wp;
	CString wpn;
	wp = app->GetProfileString("Proxy", "CategoryName", NULL);
	if(!wp.IsEmpty())
	{
		//load new one
		char Keyname1[6] = "p", Keyname2[7] = "ps", Keyname3[7] = "pn";
		int sp, i, l=1;
		char Section[10]="Proxy";
		do{
			i=1;
			sp = app->GetProfileInt(Section, "ps1", 3);
			wpn = app->GetProfileString(Section, "pn1", wp);
			wp = app->GetProfileString(Section, "p1", NULL);
			pl = new CProxyList;
			while(wp != "" && !wp.IsEmpty())
			{
		//		((CMainFrame*)m_pMainWnd)->m_astrWebProxy.AddTail(wp);
		//		((CMainFrame*)m_pMainWnd)->m_aWebProxySpeed.AddTail(sp);
		//		((CMainFrame*)m_pMainWnd)->m_astrWebProxyName.AddTail(wpn);
				pi = new CProxyItem;
				pi->m_nProxySpeed = sp;
				pi->m_strProxy = wp;
				pi->m_strProxyName = wpn;
				pl->m_Proxies.Add(pi);
				i++;
				itoa(i, Keyname1+1, 10);
				itoa(i, Keyname2+2, 10);
				itoa(i, Keyname3+2, 10);
				wp = app->GetProfileString(Section, Keyname1, NULL);
				sp = app->GetProfileInt(Section, Keyname2, 3);
				wpn = app->GetProfileString(Section, Keyname3, wp);
			}
			pl->m_strProxyByPass = app->GetProfileString(Section, "ProxyByPass", ((CMainFrame*)pMainFrame)->m_strProxyByPass);
			pl->m_bIsWebProxy = app->GetProfileInt(Section, "ProxyType", 0);
			pl->m_bIsTransProxy = app->GetProfileInt(Section, "Translation", 0);
			pl->m_strCategoryName = app->GetProfileString(Section, "CategoryName", Section);

			m_ProxyCategory.Add(pl);

			itoa(l, Section+5, 10);
			l++;
			wp = app->GetProfileString(Section, "CategoryName", NULL);

		}while(!wp.IsEmpty());
	}

	if(GetWebProxyList()==NULL)
	{
		pl = new CProxyList;

		pl->m_strProxyByPass = "";
		pl->m_bIsWebProxy = TRUE;
		pl->m_strCategoryName = "Web Proxy";
		m_ProxyCategory.Add(pl);
	}
	if(GetWebProxyList(TRUE)==NULL)
	{
		pl = new CProxyList;
		pi = new CProxyItem;
		pi->m_nProxySpeed=4;
		pi->m_strProxy.LoadString(IDS_DEFAULT_WEB_PROXY);
		pi->m_strProxyName.LoadString(IDS_DEFAULT_WEBP_NAME);
		pl->m_Proxies.Add(pi);

		pl->m_strProxyByPass = "";
		pl->m_bIsWebProxy = TRUE;
		pl->m_bIsTransProxy = TRUE;
		pl->m_strCategoryName = "Translation";
		m_ProxyCategory.Add(pl);
	}
}

void CProxyCategory::SaveProxies()
{
	CWinApp* app = AfxGetApp();
	int i = 1;
	char Keyname1[10]="p", Keyname2[11]="ps", Keyname3[11]="pn";
	CProxyItem *pi;
	CProxyList* pl;

/*	app->WriteProfileString("WebProxy", NULL, "");
//	pos = ((CMainFrame*)m_pMainWnd)->m_astrWebProxy.GetHeadPosition();
	CProxyList * pl = GetWebProxyList();
	if(pl!=NULL)
	{
		for(i=0; i<=pl->m_Proxies.GetUpperBound(); i++)
		{
			pi = pl->m_Proxies.ElementAt(i);
			itoa(i+1, Keyname1+2, 10);
			app->WriteProfileString("WebProxy", Keyname1, pi.m_strProxy);
			itoa(i+1, Keyname2+3, 10);
			app->WriteProfileInt("WebProxy", Keyname2, pi.m_nProxySpeed);
			itoa(i+1, Keyname3+3, 10);
			app->WriteProfileString("WebProxy", Keyname3, pi.m_strProxyName);
		}
		app->WriteProfileString("WebProxy", "CategoryName", pl->m_strCategoryName);
	}*/

//	strcpy(Keyname1, "p"); strcpy(Keyname2, "ps"); strcpy(Keyname3, "pn");
	char Section[10]="Proxy";
	int l=0;
	for (i=0;i <= m_ProxyCategory.GetUpperBound();i++)
	{
	   pl = m_ProxyCategory.GetAt(i);
	   if(pl!=NULL)
	   {
		   if(l>0)
			   itoa(l, Section+5, 10);
			app->WriteProfileString(Section, NULL, "");
		//	pos = ((CMainFrame*)m_pMainWnd)->m_astrWebProxy.GetHeadPosition();
			for(int ii=0; ii<=pl->m_Proxies.GetUpperBound(); ii++)
			{
				pi = pl->m_Proxies.ElementAt(ii);
				itoa(ii+1, Keyname1+1, 10);
				app->WriteProfileString(Section, Keyname1, pi->m_strProxy);
				itoa(ii+1, Keyname2+2, 10);
				app->WriteProfileInt(Section, Keyname2, pi->m_nProxySpeed);
				itoa(ii+1, Keyname3+2, 10);
				app->WriteProfileString(Section, Keyname3, pi->m_strProxyName);
			}
			app->WriteProfileString(Section, "CategoryName", pl->m_strCategoryName);
			app->WriteProfileString(Section, "ProxyByPass", pl->m_strProxyByPass);
			app->WriteProfileInt(Section, "ProxyType",  pl->m_bIsWebProxy);
			app->WriteProfileInt(Section, "Translation",  pl->m_bIsTransProxy);
			l++;
		}
	}

	if(l>0)
	   itoa(l, Section+5, 10);
	app->WriteProfileString(Section, NULL, "");

	app->WriteProfileBinary("Settings", "WebProxy", NULL, 0);
	app->WriteProfileBinary("Settings", "WebProxySP", NULL, 0);
	app->WriteProfileString("WebProxy", NULL, "");
}

CProxyItem* CProxyCategory::FindIndex(int index, CProxyList** plist, int* item)
{
	CProxyList* pl;
	CProxyItem* pi;
	int l;
	for (int i=0;i <= m_ProxyCategory.GetUpperBound();i++)
	{
	   pl = m_ProxyCategory.GetAt(i);
	   if((l=pl->FindIndex(index))>=0)
	   {
			pi = pl->m_Proxies.GetAt(l);
			break;
	   }
	}
	*plist = pl;
	*item = i;
	return pi;
}

int CProxyList::FindIndex(int index)
{
	for (int i=0;i <= m_Proxies.GetUpperBound();i++)
	{
	   if(m_Proxies.ElementAt(i)->index == index)
		   return i;
	}
	return -1;
}


void CProxyCategory::Copy(CProxyCategory &pc)
{
	RemoveAll();

	CProxyList* pl, *plcopy;
	CProxyItem *pi;
	for (int i=0;i <= pc.m_ProxyCategory.GetUpperBound();i++)
	{
	   pl = pc.m_ProxyCategory.GetAt(i);
	   if(pl!=NULL)
	   {
		   plcopy= new CProxyList;
		   plcopy->m_bIsWebProxy = pl->m_bIsWebProxy;
		   plcopy->m_bIsTransProxy = pl->m_bIsTransProxy;
		   plcopy->m_strCategoryName = pl->m_strCategoryName;
		   plcopy->m_strProxyByPass = pl->m_strProxyByPass;
		   for(int l=0; l<=pl->m_Proxies.GetUpperBound(); l++)
		   {
			   pi = new CProxyItem;
			   pi->index = pl->m_Proxies.GetAt(l)->index;
			   pi->m_nProxySpeed = pl->m_Proxies.GetAt(l)->m_nProxySpeed;
			   pi->m_strProxy = pl->m_Proxies.GetAt(l)->m_strProxy;
			   pi->m_strProxyName = pl->m_Proxies.GetAt(l)->m_strProxyName;
			   plcopy->m_Proxies.Add(pi);
		   }
		   m_ProxyCategory.Add(plcopy);
	   }
	}
}


CDownloadToolList::CDownloadToolList()
{
	
}

CDownloadToolList::~CDownloadToolList()
{
	CDownloadTool* dl;
	for (int i=0;i <= m_DownloadTools.GetUpperBound();i++)
	{
	   dl = m_DownloadTools.GetAt(i);
	   delete dl;
	}
	m_DownloadTools.RemoveAll();
}


int CDownloadToolList::GetIDFromName(LPCSTR name)
{
	for (int i=0;i <= m_DownloadTools.GetUpperBound();i++)
	{
	   if(m_DownloadTools.ElementAt(i)->DName == name)
		   return i;
	}
	return -1;
}

BOOL CDownloadToolList::LoadDownloadTools()
{
	CDownloadTool*  pdl;
	pdl = new CDownloadTool;
	pdl->DName = "NetAnts";
	pdl->ProgID = "NetAnts.IE.Monitor";
	m_DownloadTools.Add(pdl);
	pdl = new CDownloadTool;
	pdl->DName = "FlashGet";
	pdl->ProgID = "Jccatch.IeCatch2.1";
	m_DownloadTools.Add(pdl);

	//load additonal tools
	CWinApp* app = AfxGetApp();

		char Keyname1[6] = "DT", Keyname2[7] = "PID";
		int i, l=1;
		CString dt, pid;
		char Section[]="DownloadTool";
		
		i=1;
		dt = app->GetProfileString(Section, "DT1", "");
		pid = app->GetProfileString(Section, "PID1", "");
		while(dt != "" && !dt.IsEmpty())
		{
			pdl = new CDownloadTool;
			pdl->DName = dt;
			pdl->ProgID = pid;
			m_DownloadTools.Add(pdl);
				
			i++;
			itoa(i, Keyname1+2, 10);
			itoa(i, Keyname2+3, 10);
			dt = app->GetProfileString(Section, Keyname1, "");
			pid = app->GetProfileString(Section, Keyname2, "");
		}
	
		return TRUE;
}



