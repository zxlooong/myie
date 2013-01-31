// .cpp : implementation file
//

#include "stdafx.h"
#include "MyIE.h"
#include "Resourcebar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#pragma optimize( "s", on )


/////////////////////////////////////////////////////////////////////////////
// CResourcebarTree

CResourcebar::CResourcebar()
{
}

CResourcebar::~CResourcebar()
{
}



/////////////////////////////////////////////////////////////////////////////
// CHistoryTree message handlers

LPCSTR CResourcebar::GetTitle()
{
	strTitle.LoadString(IDS_TITLE_RESOURCE);
	return (LPCSTR)strTitle;
}

BOOL CResourcebar::Initialize()
{
	// Create an instance of Internet Explorer
    /*HRESULT hr = CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER, 
                                  IID_IWebBrowser2, (void**)&m_pIE);
    if (SUCCEEDED(hr))
    {
         // Set up the event sink
		 BOOL bAdvised = AfxConnectionAdvise(m_pIE, DIID_DWebBrowserEvents2,
                                            m_pIEEvents->GetInterface(&IID_IUnknown),
                                            TRUE, &m_dwCookie);
	}*/
    HINSTANCE hInstance = AfxGetResourceHandle();
    ASSERT(hInstance != NULL);

    CString strResourceURL;
    BOOL bRetVal = TRUE;
	LPTSTR lpszModule = NULL;
	try
	{
		 lpszModule = new TCHAR[_MAX_PATH];
	}
	catch(...)
	{
//		if(e!=NULL)e->Delete();
		return FALSE;
	}

    if (GetModuleFileName(hInstance, lpszModule, _MAX_PATH))
    {
		//check if resource.htm exists
		CString strSearchRes(lpszModule);
		int i = strSearchRes.ReverseFind('\\');
		strSearchRes = strSearchRes.Left(i+1);
		strSearchRes += "resource.htm";
		OFSTRUCT fst;
		if(OpenFile(strSearchRes,&fst ,OF_EXIST)!=HFILE_ERROR)
			m_wndResource.Navigate(strSearchRes, 0, 0, 0,NULL);
		else
		{
			m_wndResource.Navigate("about:blank", 0, 0, 0,NULL);
		}
    }
    else
       bRetVal = FALSE;

    delete [] lpszModule;
    return bRetVal;

}

BOOL CResourcebar::Create(CWnd* pParent)
{
	m_wndResource.Create(NULL, "", WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN, CRect(0,0,0,0), pParent, IDS_TITLE_RESOURCE,NULL);

	return TRUE;
}

CWnd* CResourcebar::GetWindow()
{
	return (CWnd*)&m_wndResource;
}


void CResourcebar::Destroy()
{
	return;
}


BOOL CResourcebar::Update(int nLevel)
{
/*	if(nLevel == 0)
		Initialize();
	else
		m_wndResource.Navigate("about:blank", 0, 0, 0,NULL);
*/
	return TRUE;
}


//#pragma optimize( "s", off)
