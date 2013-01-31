// .cpp : implementation file
//

#include "stdafx.h"
#include "MyIE.h"
#include "Searchbar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#pragma optimize( "s", on )


/////////////////////////////////////////////////////////////////////////////
// CHistoryTree

CSearchbar::CSearchbar()
{
}

CSearchbar::~CSearchbar()
{
}



/////////////////////////////////////////////////////////////////////////////
// CHistoryTree message handlers

LPCSTR CSearchbar::GetTitle()
{
	strTitle.LoadString(IDS_TITLE_SEARCH);
	return (LPCSTR)strTitle;
}

BOOL CSearchbar::Initialize()
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
    LPTSTR lpszModule = new TCHAR[_MAX_PATH];

    if (GetModuleFileName(hInstance, lpszModule, _MAX_PATH))
    {
		//check if searchres.htm exists
		CString strSearchRes(lpszModule);
		int i = strSearchRes.ReverseFind('\\');
		strSearchRes = strSearchRes.Left(i+1);
		strSearchRes += "search.htm";
		OFSTRUCT fst;
		if(OpenFile(strSearchRes,&fst ,OF_EXIST)!=HFILE_ERROR)
			m_wndSearch.Navigate(strSearchRes, 0, 0, 0,NULL);
		else
		{
			//strResourceURL.Format(_T("res://%s/%d"), lpszModule, IDR_SEARCH);
			m_wndSearch.Navigate("about:blank", 0, 0, 0,NULL);
		}
    }
    else
       bRetVal = FALSE;

    delete [] lpszModule;
    return bRetVal;

}

BOOL CSearchbar::Create(CWnd* pParent)
{
	m_wndSearch.Create(NULL, "", WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN, CRect(0,0,0,0), pParent, IDS_TITLE_SEARCH,NULL);

	return TRUE;
}

CWnd* CSearchbar::GetWindow()
{
	return (CWnd*)&m_wndSearch;
}

void CSearchbar::Destroy()
{
	//m_wndSearch.Quit();
	return;
}


BOOL CSearchbar::Update(int nLevel)
{
	return TRUE;
}

//#pragma optimize( "s", off )
