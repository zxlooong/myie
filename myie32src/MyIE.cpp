// MyIE.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MyIE.h"

#include "MainFrm.h"
#include "ChildFrm.h"
//#include "MyIEDoc.h"
#include "MyIEView.h"

#include "afxpriv.h"
#include <..\src\occimpl.h>
#include "CustSite.h" 

//#include <afxsock.h>
#include <Winsock.h>
#include <afxmt.h>
#include <wininet.h>
#include "rasspdm.h"
#include <initguid.h>
#include "MyIE_i.c"
#include "MyIENSHandle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INTERNET_CONNECTION_OFFLINE         0x20 //18

char MyIEVer[]="MyIE 3.2";


/////////////////////////////////////////////////////////////////////////////
// CMyIEApp

BEGIN_MESSAGE_MAP(CMyIEApp, CWinApp)
	//{{AFX_MSG_MAP(CMyIEApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
//	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyIEApp construction

CMyIEApp::CMyIEApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
//	m_hasEmptyDoc=true;
	m_nAutoStart = 0;
	m_bNeedCreateDir= FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMyIEApp object
CMyIEApp theApp;



///////////////////////////////////////////////////////////////////
//global exception handle, not good at this time.
/*LONG __stdcall ExcepCallBack ( EXCEPTION_POINTERS * pExPtrs )
{
  // AfxMessageBox ( "crash" ) ;

	//let it crash
    //return ( EXCEPTION_CONTINUE_SEARCH ) ;

   //let it terminal
    //return ( EXCEPTION_EXECUTE_HANDLER ) ;

	if(pExPtrs->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION )
	{
		AfxMessageBox("Memory access violation. \nYou can try to increase the virtual memory and keep enough disk space for it.");
	}
	
	return EXCEPTION_CONTINUE_SEARCH;
    
}
*/

/////////////////////////////////////////////////////////////////////////////
// CMyIEApp initialization

BOOL CMyIEApp::InitInstance()
{

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

//	SetUnhandledExceptionFilter(ExcepCallBack);

	//only one instance is allowed except -m
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		return TRUE;
	}


	HANDLE hSem = CreateSemaphore(NULL, 1, 1, m_pszAppName);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		m_bAutoStart = FALSE;
	else
		m_bAutoStart = TRUE;

    HINSTANCE hInstance = AfxGetResourceHandle();
    ASSERT(hInstance != NULL);
	LPTSTR lpszModule;
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
		m_strProfile = lpszModule;
		int i = m_strProfile.ReverseFind('\\');
		m_strProfile = m_strProfile.Left(i+1);
		m_strBitmapPath = m_strProfile;
		m_strGroupPath = m_strProfile;
		m_strQuickSearchPath = m_strProfile;
		m_strFormDataPath = m_strProfile;
		m_strSaveTxtFile = m_strProfile;
		m_strLinkListPath = m_strProfile;
		m_strMyIERoot = m_strProfile;
		m_strProfile += "myie.ini";
		m_strBitmapPath += "myieskin\\";
		m_strGroupPath += "groups\\";
		m_strQuickSearchPath += "QuickSearch.ini";
		m_strFormDataPath += "FormData.ini";
		m_strSaveTxtFile += "SaveText.txt";
		m_strLinkListPath += "urllist.htm";
    }
    delete [] lpszModule;
	
	//2.5 victor
	//First free the string allocated by MFC at CWinApp startup.
	//The string is allocated before InitInstance is called.
	free((void*)m_pszProfileName);
	//Change the name of the .INI file.
	//The CWinApp destructor will free the memory.
	m_pszProfileName=_tcsdup(m_strProfile);
	//delete[]  (char*)m_pszProfileName;
	//m_pszProfileName = m_strProfile;

	HANDLE hfile = CreateFile(m_strProfile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hfile == INVALID_HANDLE_VALUE)
	{
		hfile = CreateFile(m_strProfile, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hfile != INVALID_HANDLE_VALUE)
		{
			CString def; def.LoadString(IDS_DEFAULTPOS);
			DWORD dwW;
			WriteFile(hfile, def.GetBuffer(300), def.GetLength(), &dwW, NULL);
			def.ReleaseBuffer();
			CloseHandle(hfile);
		}
	}
	else
		CloseHandle(hfile);

	bOneInstance = GetProfileInt("Settings", "OneInstance", FALSE);
	//group path
	m_strGroupPath = GetProfileString("Settings", "GroupPath", m_strGroupPath);

	if (cmdInfo.m_nShellCommand!=CCommandLineInfo::FileNew )
		{
			
			if (!m_bAutoStart)
			{
				CloseHandle(hSem);

				HWND hWndPrevious = ::GetWindow(::GetDesktopWindow(), GW_CHILD);
				while (::IsWindow(hWndPrevious))
				{
					if (::GetProp(hWndPrevious, m_pszAppName))
					{

						if (cmdInfo.m_nShellCommand==CCommandLineInfo::FileOpen)
						{
							if (::GetLastActivePopup(hWndPrevious)==hWndPrevious)
							{
								LPSTR lpData;

								//mutex
								HANDLE hMyIESem = CreateMutex(NULL, TRUE, "MyIEMutex");
								WaitForSingleObject(hMyIESem, INFINITE);

								HANDLE hMapping = CreateFileMapping
								((HANDLE)0xFFFFFFFF,NULL,
								PAGE_READWRITE,0,2560,"MY2IESHARE");
								if (hMapping==NULL)
								{
									AfxMessageBox("App:Fail to create share memory!");
								}
								else
								{
									lpData = (LPSTR) MapViewOfFile(hMapping,FILE_MAP_ALL_ACCESS,0,0,0);
									if (lpData==NULL)
									{
										AfxMessageBox("App:MapViewOfFile Fail");
									}
									else {//MapViewOfFile??
										sprintf(lpData,"%s",cmdInfo.m_strFileName);
										::SendMessage(hWndPrevious,WM_USER_SHELL_OPEN,NULL,NULL);
									}
									CloseHandle(hMapping);
								}
								ReleaseMutex(hMyIESem);
							}

						}

						if (::IsIconic(hWndPrevious))
							::ShowWindow(hWndPrevious,SW_RESTORE);
						
						//::SetForegroundWindow(hWndPrevious);
						::SetForegroundWindow(::GetLastActivePopup(hWndPrevious));
			
							return FALSE;
						}

						hWndPrevious = ::GetWindow(hWndPrevious, GW_HWNDNEXT);
				}

				return FALSE;
			}
		}
	else if(bOneInstance && !m_bAutoStart)
	{
		//only one instance
		HWND hWndPrevious = ::GetWindow(::GetDesktopWindow(), GW_CHILD);
		while (::IsWindow(hWndPrevious))
		{
			if (::GetProp(hWndPrevious, m_pszAppName))
			{
				if (::IsIconic(hWndPrevious))
					::ShowWindow(hWndPrevious,SW_RESTORE);
						
				//::SetForegroundWindow(hWndPrevious);
				::SetForegroundWindow(::GetLastActivePopup(hWndPrevious));
			
				return FALSE;
			}

			hWndPrevious = ::GetWindow(hWndPrevious, GW_HWNDNEXT);
		}	
	}

//2.2
	//if (!AfxSocketInit())
	//	return FALSE;
	// initialize Winsock library
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(1, 1);
	int nResult = WSAStartup(wVersionRequested, &wsaData);
	if (nResult == 0)
	{
		if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
		{
			WSACleanup();
		}
	}


#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
//3.1
#if _WIN32_WINNT >= 0x0400
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
#else
	CoInitialize(NULL);
#endif

	if (!InitATL())
		return FALSE;

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.

	//get a random ID
    srand( (unsigned)time( NULL ) );
	nTrayIconID = rand()/16;


	BOOL bFirst = FirstRun();

	CString bmpath;
	bmpath = GetProfileString("Settings", "SkinPath", NULL);
	if(!bmpath.IsEmpty())
		m_strBitmapPath = bmpath;
	
	m_bTopTab = GetProfileInt("Settings", "TabPos", 0);

	//IShellUIHandle
	CCustomOccManager *pMgr = new CCustomOccManager;

	// Create an IDispatch class for extending the Dynamic HTML Object Model 
	m_pDispOM = new CImpIDispatch;
	//Drop target
	m_pDropTarget = new CImpIDropTarget;

	// Set our control containment up but using our control container 
	// management class instead of MFC's default
	AfxEnableControlContainer(pMgr);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	//global main frame point
	::pMainFrame = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();


	// Enable DDE Execute open
//	EnableShellOpen();
//	RegisterShellFileTypes(TRUE);


	LoadConfg();
	// Parse command line for standard shell commands, DDE, file open
//	CCommandLineInfo cmdInfo;
//	ParseCommandLine(cmdInfo);

	int nOpenType;
	CString strURL;

	bGoHome = GetProfileInt("Settings", "goHome", 1);
/*	if(bGoHome==0)
		((CMainFrame*)m_pMainWnd)->GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_OPENBLANKPAGE,MF_CHECKED);
	if(bGoHome==1)
		((CMainFrame*)m_pMainWnd)->GetMenu()->CheckMenuItem(ID_OPTIONS_MYIEGOHOME,MF_CHECKED);
	if(bGoHome==2)
		((CMainFrame*)m_pMainWnd)->GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_RESUMELASTVISITEDPAGES,MF_CHECKED);
	if(bGoHome==3)
		((CMainFrame*)m_pMainWnd)->GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_OPENFAVORITEPAGES,MF_CHECKED);
	if(bGoHome==4)
		((CMainFrame*)m_pMainWnd)->GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_OPENMOSTFAVORITEPAGES,MF_CHECKED);
	if(bGoHome==5)
		((CMainFrame*)m_pMainWnd)->GetMenu()->CheckMenuItem(ID_OPTIONS_MYIESTARTUP_OPENSTARTUPGROUP,MF_CHECKED);
*/

	//disable the old menu
	::SetMenu(pMainFrame->m_hWnd, NULL);

	//add user agent
/*disable in 3.2
	HKEY            hKey=NULL;
//	TCHAR           sz[MAX_PATH];

	if(RegOpenKey(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\User Agent\\Post Platform"), &hKey) != ERROR_SUCCESS)
	{
		RegCreateKey(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\User Agent\\Post Platform"), &hKey);
	}
	if(hKey!=NULL)
	{
		RegSetValueEx(hKey, _T(MyIEVer), 0, REG_SZ, (LPBYTE)"IEAK", 4);
		RegCloseKey(hKey);
	}*/

	pMainFrame->ShowWindow(m_nCmdShow);

	//max wins
	nMaxWins = GetProfileInt("Settings", "MaxWins", 100);



/*	if(OpenFile(m_strProfile,&fst ,OF_EXIST)==HFILE_ERROR)
	{
		delete[]  (char*)m_pszProfileName;
		m_pszProfileName = m_strProfile;
	}
	delete[]  (char*)m_pszRegistryKey;
	m_pszRegistryKey = NULL;
*/

	

	BOOL run = 	GetProfileInt("Settings", "Running", 0);
	if(!m_bAutoStart)
		run = FALSE;

	//restore proxy server
	if(((CMainFrame*)m_pMainWnd)->m_strCurProxy.Find("127.0.0.1")<0 && ((CMainFrame*)m_pMainWnd)->m_strCurProxy.Find("localhost")<0)
	{
		int ep = GetProfileInt("Settings", "EnableProxy", -1);
		if(ep!=-1 && !((CMainFrame*)m_pMainWnd)->m_strCurProxy.IsEmpty())
		{
			//disable proxy first
			INTERNET_PROXY_INFO ipi;
			DWORD dwSize=sizeof(ipi);
			ipi.dwAccessType = INTERNET_OPEN_TYPE_DIRECT ;
			InternetSetOption(NULL, INTERNET_OPTION_PROXY, &ipi, dwSize);
		}

		if(ep==1)
		{
			if(!((CMainFrame*)m_pMainWnd)->m_strCurProxy.IsEmpty())
			{
				::SendMessage(m_pMainWnd->m_hWnd, WM_COMMAND, ID_OPTIONS_USE_PROXY, 0);
			}
		}
	}

/*	//3.0 auto detect offline
	DWORD dwOffline;
	if(InternetGetConnectedState(&dwOffline, 0))
	{
		if(dwOffline == INTERNET_CONNECTION_OFFLINE || dwOffline==0x20)
		{
			//set offline
			HKEY            hKey;
			DWORD           dwSize = sizeof(DWORD), sz;

			if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"), &hKey) != ERROR_SUCCESS)
			{
				TRACE0("Global settings not found\n");
			}
			else
			{
				sz = 1;
				RegSetValueEx(hKey, _T("GlobalUserOffline"), NULL, REG_DWORD, (LPBYTE)(&sz), dwSize);


				RegCloseKey(hKey);

				//infor the system that setting is changed.
				InternetSetOption(
					NULL,
					INTERNET_OPTION_SETTINGS_CHANGED,
					NULL,
					0
					);
			}
		}
			
	}*/



	if (cmdInfo.m_nShellCommand==CCommandLineInfo::FileOpen)
	{
		nOpenType = 2;
		strURL = cmdInfo.m_strFileName;
		TCHAR           buf[2048];
//		strURL.MakeLower();
		if(strURL.Right(4).CompareNoCase(".url") == 0)
		{
			// an .URL file is formatted just like an .INI file, so we can
			// use GetPrivateProfileString() to get the information we want
			::GetPrivateProfileString(_T("InternetShortcut"), _T("URL"),
									  _T(""), buf, 2048,
									  strURL);

			strURL =  buf;
		}
		pMainFrame->NewChildWindow(1, nOpenType, strURL);
	}
	else
	{
		//continue last error session
		if(run)
		{	pMainFrame->UpdateWindow();
			if(AfxMessageBox(IDS_RESUME, MB_YESNO)==IDYES)
				run = TRUE;
			else
				run = FALSE;
		}


		//enable go home
		nOpenType = bGoHome;
		if(bGoHome == 2|| run)// 
		{
			//load last visit
			UINT len;
			char *tmp = NULL;
			BOOL r = GetProfileBinary("Settings","lastvisit", (LPBYTE*)(&tmp), &len);
			if(r)
			{
				//load last visit list
				char *token;
				char seps[] = "\n";
				token = strtok( tmp, seps);
				while( token != NULL )
				{
					BOOL bnext = ((CMainFrame*)m_pMainWnd)->m_bNextTab;
					((CMainFrame*)m_pMainWnd)->m_bNextTab = FALSE;
					  /* While there are tokens in "string" */
					  ((CMainFrame*)m_pMainWnd)->NewChildWindow(1, 2, token);
					  /* Get next token: */
					  token = strtok( NULL, seps );
					  ((CMainFrame*)m_pMainWnd)->m_bNextTab  = bnext;
				}
				delete[] tmp;

			}
		}
		else if(bGoHome == 3)
		{
			pMainFrame->OpenAllFavs(((CMainFrame*)m_pMainWnd)->m_strStartFavFolder);
		}
		else if(bGoHome == 4)
		{
			pMainFrame->OpenAllFavs(((CMainFrame*)m_pMainWnd)->m_strStartFavFolder, TRUE);
		}
		else if(bGoHome == 5)
		{
			//start group
			pMainFrame->OpenGroup(pMainFrame->m_strStartGroup);
		}
		else if(bGoHome == 6)
		{
			//do nothing			
		}
		else if(bGoHome == 1)//3.2
			pMainFrame->PostMessage(WM_COMMAND, ID_FILE_NEW_S, 0);
		else
		//3.2	pMainFrame->NewChildWindow(1, nOpenType, strURL, NULL, TRUE);
			pMainFrame->PostMessage(WM_COMMAND, ID_FILE_NEW_BLANK, 0);
	}
	
	//full screen
	BOOL bFullScreenMode = GetProfileInt("Settings", "FullScreen", 0);
	if(bFullScreenMode)
	{
		((CMainFrame*)m_pMainWnd)->FullScreenModeOn();
	}
	
	//go my homepage when first run 
	if(bFirst)
	{
		CString strURL = "http://changyou.mainpage.net/";
		((CMainFrame*)m_pMainWnd)->NewChildWindow(1,2,strURL);
	}

	pMainFrame->UpdateWindow();

	//delay load
	((CMainFrame*)m_pMainWnd)->PostMessage(WM_DELAYLOAD_CONFIG);

	WriteProfileInt("Settings", "Running", 1);


	return TRUE;
}

//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CMyIEApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CMyIEApp message handlers


/*CDocument* CMyIEApp::OpenDocumentFile(LPCTSTR lpszFileName) 
{
	// TODO: Add your specialized code here and/or call the base class
	//load file to the empty doc which is opend when initializing.
//	ASSERT(m_pDocManager != NULL);
	POSITION pos=m_pDocManager->GetFirstDocTemplatePosition();
	if(pos!=NULL)
	{
		CDocTemplate* pTemplate = m_pDocManager->GetNextDocTemplate(pos);
		POSITION pos2 = pTemplate->GetFirstDocPosition();
		if(pos2!=NULL)
		{
			CMyIEDoc * pDoc = (CMyIEDoc*)pTemplate->GetNextDoc(pos2);
			if(pDoc!=NULL && pDoc->m_bEmpty)
			{
				pDoc->m_bEmpty=false;
				POSITION dpos = pDoc->GetFirstViewPosition();
				CFixedHtmlView* pv =((CFixedHtmlView*)(pDoc->GetNextView(dpos)));
				pv->Navigate2(lpszFileName,NULL,NULL);
				pDoc->SetPathName(lpszFileName);
				
				//bring the MDI child window to the top, 
				//we should bring the frame to top, not the view itself.
				//a view has no title bar and border.
				pv->GetParentFrame()->BringWindowToTop(); 
				return pDoc;
			}
		}
	}
	return CWinApp::OpenDocumentFile(lpszFileName);
}*/
BOOL CMyIEApp::FirstRun()
{
	int ver = 300; //3.x
	int oldver = GetProfileInt("Settings", "Version", 0);
	if(ver!=oldver)
	{
		//the first time run this version
		WriteProfileInt("Settings", "Version", ver);

		//add the code to initialize
#ifdef WEST_VER
		WriteProfileString("Settings", "Background", "dhxu1.bmp");
#endif

		return TRUE;
	}
	return FALSE;
}

//#pragma optimize( "", on )


int CMyIEApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	try{
	//2.2
	WSACleanup();

	if(m_pDispOM)
		delete m_pDispOM;

	if(m_pDropTarget)
		delete m_pDropTarget;



	//delete agent
/*3.2	HKEY            hKey;
//	TCHAR           sz[MAX_PATH];

	if(RegOpenKey(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\User Agent\\Post Platform"), &hKey) != ERROR_SUCCESS)
	{
		TRACE0("Main setting not found\n");
	}
	else
	{
		RegDeleteValue(hKey, _T(MyIEVer));
		RegCloseKey(hKey);
	}*/

	}
	catch(...)
	{
	}

//	try{
	if (m_bATLInited)
	{
		_Module.RevokeClassObjects();
		_Module.Term();
	
	//	CoUninitialize();
	}

	CoUninitialize();
	return CWinApp::ExitInstance();
//	}catch(...)
//	{
//		return -1;
//	}
}


void CMyIEApp::LoadConfg()
{
	//get pop list
//	UINT len;
	char *tmp = NULL;
	BOOL r;


	//enable popup
	EnablePopupFilter = GetProfileInt("Settings", "EnablePop", 1);
	if(EnablePopupFilter)
		((CMainFrame*)m_pMainWnd)->GetMenu()->CheckMenuItem(ID_TOOLS_EABLEPOPUPFILTER,MF_CHECKED);
	else
		((CMainFrame*)m_pMainWnd)->GetMenu()->CheckMenuItem(ID_TOOLS_EABLEPOPUPFILTER,MF_UNCHECKED);
	EnableDupFilter = GetProfileInt("Settings", "EnableDupFilter", 0);
	EnableAutoFilter = GetProfileInt("Settings", "EnableAutoFilter", 0);
	//3.2
	if(GetProfileInt("Settings", "InAutoFilter", 0))
		EnableAutoFilter = FALSE;

	//window pos
	int top = GetProfileInt("Settings", "top", 25);
	int left = GetProfileInt("Settings", "left", 10);
	int width = GetProfileInt("Settings", "width", 778);
	int height = GetProfileInt("Settings", "height", 560);
	if( width!=0 && top<2000 && top>=-20)
		m_pMainWnd->SetWindowPos(&CWnd::wndTop, left, top, width, height,SWP_HIDEWINDOW);   
	if( GetProfileInt("Settings", "Zoomed", 0))
		m_nCmdShow = SW_SHOWMAXIMIZED;
	

	((CMainFrame*)m_pMainWnd)->m_nTitleLen = GetProfileInt("Settings", "TabSize", 20);

	//======================================Proxy==========================================
	//load defaul proxy
	((CMainFrame*)m_pMainWnd)->m_strCurProxy = GetProfileString("Settings", "DefaultProxy", NULL);
	((CMainFrame*)m_pMainWnd)->m_strCurProxyName = ((CMainFrame*)m_pMainWnd)->GetProxyName(((CMainFrame*)m_pMainWnd)->m_strCurProxy);
	//load proxy by pass
	((CMainFrame*)m_pMainWnd)->m_strProxyByPass = GetProfileString("Settings", "ProxyByPass", "<local>");

	//load proxy list number
	((CMainFrame*)m_pMainWnd)->m_nCurProxyList = GetProfileInt("Settings", "ProxyList", 1);

//	((CMainFrame*)m_pMainWnd)->m_aProxyCategories.LoadProxies();

	//load defaul web proxy
	((CMainFrame*)m_pMainWnd)->m_strDefaultWebProxy = GetProfileString("Settings", "DefaultWebProxy", NULL);
/*	if(((CMainFrame*)m_pMainWnd)->m_strDefaultWebProxy.IsEmpty())
	{
		((CMainFrame*)m_pMainWnd)->m_strDefaultWebProxy.LoadString(IDS_DEFAULT_WEB_PROXY);
		((CMainFrame*)m_pMainWnd)->m_astrWebProxy.AddTail(((CMainFrame*)m_pMainWnd)->m_strDefaultWebProxy);
	    ((CMainFrame*)m_pMainWnd)->m_aWebProxySpeed.AddTail(3);
	}*/

	//load proxy servers
/*2.3	CString wp = GetProfileString("Proxy", "p1", NULL);
	if(wp == "" || wp.IsEmpty())
	{
		tmp = NULL;
		r = GetProfileBinary("Settings","Proxy", (LPBYTE*)(&tmp), &len);
		if(r)
		{
			//load pop list
			char *token;
			char seps[] = "\n";
			token = strtok( tmp, seps);
			while( token != NULL )
			{
				  /* While there are tokens in "string" * /
				  ((CMainFrame*)m_pMainWnd)->m_astrProxy.AddTail(token);
				  ((CMainFrame*)m_pMainWnd)->m_astrProxyName.AddTail(token);
				  /* Get next token: * /
				  token = strtok( NULL, seps );
			}
			delete[] tmp;

		}
*/
	//	int pn = ((CMainFrame*)m_pMainWnd)->m_astrProxy.GetCount();
		//load proxy user name
	/*	tmp = NULL; 
		r = GetProfileBinary("Settings","ProxyUser", (LPBYTE*)(&tmp), &len);
		if(r)
		{
			//load pop list
			char *token;
			char seps[] = "\n";
			token = strtok( tmp, seps);
			while( token != NULL )
			{
				  // While there are tokens in "string" 
				  ((CMainFrame*)m_pMainWnd)->m_astrProxyUser.AddTail(token);
				  // Get next token:
				  token = strtok( NULL, seps );
			}
			delete[] tmp;

		}

		//load proxy password
		tmp = NULL;
		r = GetProfileBinary("Settings","ProxyPwd", (LPBYTE*)(&tmp), &len);
		if(r)
		{
			//load pop list
			char *token;
			char seps[] = "\n";
			token = strtok( tmp, seps);
			while( token != NULL )
			{
				  // While there are tokens in "string" 
				  ((CMainFrame*)m_pMainWnd)->m_astrProxyPwd.AddTail(token);
				  // Get next token: 
				  token = strtok( NULL, seps );
			}
			delete[] tmp;

		}
	*/
		//load proxy speed
/*2.3		tmp = NULL;
		r = GetProfileBinary("Settings","ProxySP", (LPBYTE*)(&tmp), &len);
		if(r)
		{
			//load pop list
			for( UINT i=0;i<len;i++ )
			{
				  ((CMainFrame*)m_pMainWnd)->m_aProxySpeed.AddTail(tmp[i]);
			}
			delete[] tmp;

		}
	}
	else
	{
		//new format
		char Keyname1[6] = "p", Keyname2[7] = "ps", Keyname3[7] = "pn";
		int sp, i=1;
		sp = GetProfileInt("Proxy", "ps1", 3);
		CString wpn = GetProfileString("Proxy", "pn1", wp);
		while(wp != "" && !wp.IsEmpty())
		{
			((CMainFrame*)m_pMainWnd)->m_astrProxy.AddTail(wp);
			((CMainFrame*)m_pMainWnd)->m_aProxySpeed.AddTail(sp);
			((CMainFrame*)m_pMainWnd)->m_astrProxyName.AddTail(wpn);
			i++;
			itoa(i, Keyname1+1, 10);
			itoa(i, Keyname2+2, 10);
			itoa(i, Keyname3+2, 10);
			wp = GetProfileString("Proxy", Keyname1, NULL);
			sp = GetProfileInt("Proxy", Keyname2, 3);
			wpn = GetProfileString("Proxy", Keyname3, wp);
		}
	}
*/
	//adjust number of proxy list
/*	if(pn!=((CMainFrame*)m_pMainWnd)->m_astrProxyUser.GetCount())
	{
		((CMainFrame*)m_pMainWnd)->m_astrProxyUser.RemoveAll();
		for(int i=0; i<pn;i++)
			((CMainFrame*)m_pMainWnd)->m_astrProxyUser.AddHead("");
	}
	if(pn!=((CMainFrame*)m_pMainWnd)->m_astrProxyPwd.GetCount())
	{
		((CMainFrame*)m_pMainWnd)->m_astrProxyPwd.RemoveAll();
		for(int i=0; i<pn;i++)
			((CMainFrame*)m_pMainWnd)->m_astrProxyPwd.AddHead("");
	}*/
/*v2.0	delete
	if(pn>((CMainFrame*)m_pMainWnd)->m_aProxySpeed.GetCount())
	{
		((CMainFrame*)m_pMainWnd)->m_aProxySpeed.RemoveAll();
		for(int i=0; i<pn;i++)
			((CMainFrame*)m_pMainWnd)->m_aProxySpeed.AddHead(3);
	}
	pn=((CMainFrame*)m_pMainWnd)->m_astrWebProxy.GetCount();
	if(pn>((CMainFrame*)m_pMainWnd)->m_aWebProxySpeed.GetCount())
	{
		((CMainFrame*)m_pMainWnd)->m_aWebProxySpeed.RemoveAll();
		for(int i=0; i<pn;i++)
			((CMainFrame*)m_pMainWnd)->m_aWebProxySpeed.AddHead(3);
	}
*/

//=================================================================================================


	//active new window
	((CMainFrame*)m_pMainWnd)->m_bActiveNewWindow = GetProfileInt("Settings", "ActiveNew", 0);
	//always new window
	bAddressInNewWindow = GetProfileInt("Settings", "AddressInNewWindow", 0);
	bLinkInNewWindow = GetProfileInt("Settings", "LinkInNewWindow", 0);
	bFavInNewWindow = GetProfileInt("Settings", "FavInNewWindow", TRUE);
	bHisInNewWindow = GetProfileInt("Settings", "HisInNewWindow", TRUE);
	bFileInNewWindow = GetProfileInt("Settings", "FileInNewWindow", TRUE);

	//the start favorite folder
	((CMainFrame*)m_pMainWnd)->m_strStartFavFolder = GetProfileString("Settings", "StartFolder", NULL);

	//insert to next tab
	((CMainFrame*)m_pMainWnd)->m_bNextTab = GetProfileInt("Settings", "NextTab", 1);
	

	//get my document folder
	TCHAR           szPath[MAX_PATH];
	r = TRUE;

	try
	{
	   LPITEMIDLIST pidlMyDoc = NULL;
	   // Get a pointer to the IMalloc interface
	   IMalloc * m_pMalloc = NULL;
	   HRESULT hr = SHGetMalloc(&m_pMalloc);
	   if (SUCCEEDED(hr))
	   {
		   HRESULT hr = SHGetSpecialFolderLocation(NULL, CSIDL_PERSONAL, &pidlMyDoc);
		   if (NOERROR != hr)
			  r = FALSE;
		   else
		   {
			   r = SHGetPathFromIDList( pidlMyDoc,szPath);
		   }
		   m_pMalloc->Free(pidlMyDoc);
		   m_pMalloc->Release();
		   m_pMalloc = NULL;
	   }
	}catch(...)
	{
	}

	//default path
	m_strDefaultDir = GetProfileString("Settings", "DefaultDir");
	if(!m_strDefaultDir.IsEmpty() && m_strDefaultDir != "")
	{
		SetCurrentDirectory(m_strDefaultDir);

		//set save directory
		HKEY hKey;
		if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\Main"), &hKey) == ERROR_SUCCESS)
		{
			RegSetValueEx(hKey, _T("Save Directory"), 0, REG_SZ, (LPBYTE)(m_strDefaultDir.GetBuffer(10)), m_strDefaultDir.GetLength());
			m_strDefaultDir.ReleaseBuffer();
		}

	}
	else
	{
		if(r)
		   m_strDefaultDir = szPath;
	}
	if(m_strDefaultDir.Right(1) != "\\")
		m_strDefaultDir += "\\";
	
	//auto save
	((CMainFrame*)m_pMainWnd)->m_nAutoSave = GetProfileInt("Settings", "AutoSave", 0);
	((CMainFrame*)m_pMainWnd)->m_bCateWithSite = GetProfileInt("Settings", "CateWithSite", 0);


	//image path
	CString simage = GetProfileString("Settings", "ImagePath");
	if(simage.IsEmpty())
	{
		//create new
		//CreateDirectory(m_strImagePath, NULL);
		if(r)
		{
			m_strImagePath = szPath;
			m_strImagePath += "\\My Pictures\\";
		}
		m_bNeedCreateDir = TRUE;
	}
	else
		m_strImagePath = simage;

	//Exp width
	((CMainFrame*)m_pMainWnd)->m_nExpWidth = GetProfileInt("Settings", "ExpWidth", 200);

	//Exp Bar
	CString str = GetProfileString("Settings", "ExpBar");
	CString str1; 
	str1.LoadString(IDS_TITLE_SEARCH);
	if(str==str1)
		PostMessage(m_pMainWnd->m_hWnd, WM_COMMAND, ID_VIEW_EXPLOREBAR_SEARCH,0);
	str1.LoadString(IDS_TITLE_HIS);
	if(str==str1)
		PostMessage(m_pMainWnd->m_hWnd, WM_COMMAND, ID_VIEW_HIS,0);
	str1.LoadString(IDS_TITLE_FAVORITE);
	if(str==str1)
		PostMessage(m_pMainWnd->m_hWnd, WM_COMMAND, ID_FAVORITES_DROPDOWN,0);
	str1.LoadString(IDS_TITLE_SHELL);
	if(str==str1)
		PostMessage(m_pMainWnd->m_hWnd, WM_COMMAND, ID_VIEW_FOLDER,0);
	str1.LoadString(IDS_TITLE_RESOURCE);
	if(str==str1)
		PostMessage(m_pMainWnd->m_hWnd, WM_COMMAND, ID_VIEW_EXPLOREBAR_RESOURCE,0);

	//autosizing tab
	((CMainFrame*)m_pMainWnd)->m_bAutoTab = GetProfileInt("Settings", "AutoTab", 1);

	//max autosizing tab width
	((CMainFrame*)m_pMainWnd)->m_nMaxTabWidth = GetProfileInt("Settings", "MaxTabWidth", 82);
	//min autosizing tab width
	((CMainFrame*)m_pMainWnd)->m_nMinTabWidth = GetProfileInt("Settings", "MinTabWidth", 5);

	//confirm when exit
	bConfirmExit = GetProfileInt("Settings", "ConfirmExit", TRUE);

	//use shortcuts
	((CMainFrame*)m_pMainWnd)->m_bUseSC = GetProfileInt("Settings", "UseSC", FALSE);

	//use alias
	((CMainFrame*)m_pMainWnd)->m_bUseAlias = GetProfileInt("Settings", "UseAlias", TRUE);

	//load shortcuts
	char key[9] = "F";
	for(int i=0; i<11; i++)
	{
		itoa(i+2, key+1, 10);
		((CMainFrame*)m_pMainWnd)->m_strShortCuts[i] = GetProfileString("Shortcuts", key);
	}




	//Domains
	((CMainFrame*)m_pMainWnd)->m_strCE1 = GetProfileString("Domains", "CE1", "http://www.");
	((CMainFrame*)m_pMainWnd)->m_strCE2 = GetProfileString("Domains", "CE2", ".com");
	((CMainFrame*)m_pMainWnd)->m_strSE1 = GetProfileString("Domains", "SE1", "http://www.");
	((CMainFrame*)m_pMainWnd)->m_strSE2 = GetProfileString("Domains", "SE2", ".net");
	((CMainFrame*)m_pMainWnd)->m_strCSE1 = GetProfileString("Domains", "CSE1", "http://www.");
	((CMainFrame*)m_pMainWnd)->m_strCSE2 = GetProfileString("Domains", "CSE2", ".com.cn");

	//small to tray
	bSysTray = GetProfileInt("Settings", "SysTray", FALSE);

	//3721
	((CMainFrame*)m_pMainWnd)->m_bUse3721 = GetProfileInt("Settings", "Use3721", TRUE);

	//sequence
	((CMainFrame*)m_pMainWnd)->m_nSeq = GetProfileInt("Settings", "Sequence", 1);

	//silent
	bSilent = GetProfileInt("Settings", "Silent", TRUE);
	//default font size
	nDefFontSize = GetProfileInt("Settings", "DefFontSize", 2);
	//disable scroll text on status bar
	bURLOnly = GetProfileInt("Settings", "DisScrollText", FALSE);

	//filte blank popup window from same parent
	bFilteBlank = GetProfileInt("Settings", "FilteBlank", FALSE);

	//interval
	((CMainFrame*)m_pMainWnd)->m_nInterval = GetProfileInt("Settings", "Interval", 0);

	//open link in new window
//	bLinkInNewWin = GetProfileInt("Settings", "LinkInNewWin", 0);

	//browser mode
	((CMainFrame*)m_pMainWnd)->m_bmStandard.m_bActiveNewWindow = TRUE;
	((CMainFrame*)m_pMainWnd)->m_bmStandard.m_bAlwaysNewWindow = FALSE;
	((CMainFrame*)m_pMainWnd)->m_bmStandard.m_nSeq = 1;
	((CMainFrame*)m_pMainWnd)->m_bmStandard.bLinkInNewWin = FALSE;

	((CMainFrame*)m_pMainWnd)->m_bmNews.m_bActiveNewWindow = FALSE;
	((CMainFrame*)m_pMainWnd)->m_bmNews.m_bAlwaysNewWindow = TRUE;
	((CMainFrame*)m_pMainWnd)->m_bmNews.m_nSeq = 1;
	((CMainFrame*)m_pMainWnd)->m_bmNews.bLinkInNewWin = TRUE;

	((CMainFrame*)m_pMainWnd)->m_bmUserDefault.m_bActiveNewWindow = GetProfileInt("BModes", "ActiveNewWindow", ((CMainFrame*)m_pMainWnd)->m_bActiveNewWindow);
	((CMainFrame*)m_pMainWnd)->m_bmUserDefault.m_bAlwaysNewWindow = GetProfileInt("BModes", "AlwaysNewWindow", bAddressInNewWindow);
	((CMainFrame*)m_pMainWnd)->m_bmUserDefault.m_nSeq = GetProfileInt("BModes", "Seq", ((CMainFrame*)m_pMainWnd)->m_nSeq);
	((CMainFrame*)m_pMainWnd)->m_bmUserDefault.bLinkInNewWin = GetProfileInt("BModes", "LinkInNewWin", bLinkInNewWindow);

	//auto scroll
	((CMainFrame*)m_pMainWnd)->m_bDefScrollAfterDL = GetProfileInt("Settings", "AutoScrollAfterDL", FALSE);
	((CMainFrame*)m_pMainWnd)->m_bDefUseAutoScroll = GetProfileInt("Settings", "UseAutoScroll", FALSE);
	((CMainFrame*)m_pMainWnd)->m_nDefSrSpeed = GetProfileInt("Settings", "SrSpeed", 1);
	//scroll speed
	gSeed = (float)GetProfileInt("Settings", "SpeedSeed", 100);
	if(gSeed == 0)
		gSeed = 1;
	else
		gSeed = gSeed/100;

	bConfirmCloseAll = GetProfileInt("Settings", "ConfirmCloseAll", TRUE);


	//start group
	((CMainFrame*)m_pMainWnd)->m_strStartGroup = GetProfileString("Settings", "StartGroup", "");

	//new window
	((CMainFrame*)m_pMainWnd)->m_nNewWindow = GetProfileInt("Settings", "NewWindow", 0);
	((CMainFrame*)m_pMainWnd)->m_nNewWindow2 = GetProfileInt("Settings", "NewWindow2", 0);

	//allow double click to close window
	((CMainFrame*)m_pMainWnd)->m_nAllowDBC = GetProfileInt("Settings", "AllowDBC", 1);
	((CMainFrame*)m_pMainWnd)->m_nAllowMC = GetProfileInt("Settings", "AllowMC", 1);
	((CMainFrame*)m_pMainWnd)->m_nAllowRC = GetProfileInt("Settings", "AllowRC", 0);

	//default bg color
	((CMainFrame*)m_pMainWnd)->m_strBGColor = GetProfileString("Settings", "BGColor", "#FFFFFF");

	//default auto refresh
	((CMainFrame*)m_pMainWnd)->m_nDefRefreshInterval = GetProfileInt("Settings", "AutoRefresh", 60);

	//security
	((CMainFrame*)m_pMainWnd)->m_bSecure = GetProfileInt("Settings", "Secure", FALSE);

	//auto set background
	bAutoSetBG = GetProfileInt("Settings", "AutoSetBG", FALSE);

	//animate icon
	((CMainFrame*)m_pMainWnd)->m_bAnimatIcon = GetProfileInt("Settings", "AnimateIcon", TRUE);

	//mouse gestures
	((CMainFrame*)m_pMainWnd)->m_bAllowMouseGestures = GetProfileInt("Settings", "MouseGestures", TRUE);

	//lock homepage
	bLockHomepage = GetProfileInt("Settings", "LockHomepage", FALSE);

	//most favorite checked
	((CMainFrame*)m_pMainWnd)->m_bMostFavChecked = GetProfileInt("Settings", "MostFavChked", FALSE);

	//most favorite checked
	((CMainFrame*)m_pMainWnd)->m_bMouseCtrlScrSpeed = GetProfileInt("Settings", "MouseCtrlSpeed", FALSE);

	((CMainFrame*)m_pMainWnd)->m_bUseUrlFilter = GetProfileInt("Settings", "UseUrlFilter", FALSE);

	((CMainFrame*)m_pMainWnd)->m_DispBP = GetProfileInt("Settings", "DispSpeed", TRUE);


}


void CMyIEApp::SaveConfg()
{
	//save popup list
//	CString allpopups;
/*	
	POSITION pos = ((CMainFrame*)m_pMainWnd)->m_astrPopup.GetHeadPosition();
	UINT i=0;
	while(pos!=NULL)
	{
		allpopups += ((CMainFrame*)m_pMainWnd)->m_astrPopup.GetAt(pos);
		allpopups += "\n";
		((CMainFrame*)m_pMainWnd)->m_astrPopup.GetNext(pos);
	}
	WriteProfileBinary("Settings","Popups", (LPBYTE)allpopups.GetBuffer(1), allpopups.GetLength()+1);
*/
	try{
	//=======================================Proxy=========================================
	if(bProxyDirty)
		((CMainFrame*)m_pMainWnd)->m_aProxyCategories.SaveProxies();
	bProxyDirty = FALSE;

	UINT	i = 1;
	WriteProfileString("Popups", NULL, "");
	char Keyname[12] = "popup"; 
	POSITION pos = ((CMainFrame*)m_pMainWnd)->m_astrPopup.GetHeadPosition();
	while(pos!=NULL)
	{
		itoa(i, Keyname+5, 10);
		WriteProfileString("Popups", Keyname, ((CMainFrame*)m_pMainWnd)->m_astrPopup.GetAt(pos));
		((CMainFrame*)m_pMainWnd)->m_astrPopup.GetNext(pos);
		i++;
	}
	WriteProfileInt("Settings", "EnablePop", EnablePopupFilter);
	WriteProfileInt("Settings", "EnableDupFilter", EnableDupFilter);
	WriteProfileInt("Settings", "EnableAutoFilter", EnableAutoFilter);

	//url filter
	i = 1;
	CString filename = m_strMyIERoot+"\\urlfilter.ini";
	::WritePrivateProfileString("Filter", NULL, "", filename);
	strcpy(Keyname, "url"); 
	for(i=0; i<((CMainFrame*)m_pMainWnd)->m_astrUrlFilter.GetSize(); i++)
	{
		itoa(i, Keyname+3, 10);
		::WritePrivateProfileString("Filter", Keyname, ((CMainFrame*)m_pMainWnd)->m_astrUrlFilter.GetAt(i), filename);
	}
	WriteProfileInt("Settings", "UseUrlFilter", pMainFrame->m_bUseUrlFilter);


	WriteProfileInt("Settings", "goHome", bGoHome);
	WriteProfileInt("Settings", "TabSize", ((CMainFrame*)m_pMainWnd)->m_nTitleLen);

	BYTE *tmp = NULL;
//	int cn;



	//save web proxies
/*	allpopups.Empty();
	pos = ((CMainFrame*)m_pMainWnd)->m_astrWebProxy.GetHeadPosition();
	i=0;
	while(pos!=NULL)
	{
		allpopups += ((CMainFrame*)m_pMainWnd)->m_astrWebProxy.GetAt(pos);
		allpopups += "\n";
		((CMainFrame*)m_pMainWnd)->m_astrWebProxy.GetNext(pos);
	}
	WriteProfileBinary("Settings","WebProxy", (LPBYTE)allpopups.GetBuffer(1), allpopups.GetLength()+1);

  //save webproxy speed
	pos = ((CMainFrame*)m_pMainWnd)->m_aWebProxySpeed.GetHeadPosition();
	i=0;
	int cn = ((CMainFrame*)m_pMainWnd)->m_aWebProxySpeed.GetCount();
	if(cn>0)
		tmp = new BYTE[cn];
	while(pos!=NULL)
	{
		tmp[i] = ((CMainFrame*)m_pMainWnd)->m_aWebProxySpeed.GetAt(pos);
		((CMainFrame*)m_pMainWnd)->m_aWebProxySpeed.GetNext(pos);
		i++;
	}
	WriteProfileBinary("Settings","WebProxySP", tmp, cn);
	if(tmp!=NULL)
		delete tmp;
*/
	i = 1;
	char Keyname1[10]="wp", Keyname2[11]="wps", Keyname3[11]="wpn", Keyname4[11];
/*	WriteProfileString("WebProxy", NULL, "");
	pos = ((CMainFrame*)m_pMainWnd)->m_astrWebProxy.GetHeadPosition();
	while(pos!=NULL)
	{
		itoa(i, Keyname1+2, 10);
		WriteProfileString("WebProxy", Keyname1, ((CMainFrame*)m_pMainWnd)->m_astrWebProxy.GetAt(pos));
		((CMainFrame*)m_pMainWnd)->m_astrWebProxy.GetNext(pos);
		i++;
	}
	i = 1;
	pos = ((CMainFrame*)m_pMainWnd)->m_aWebProxySpeed.GetHeadPosition();
	while(pos!=NULL)
	{
		itoa(i, Keyname2+3, 10);
		WriteProfileInt("WebProxy", Keyname2, ((CMainFrame*)m_pMainWnd)->m_aWebProxySpeed.GetAt(pos));
		((CMainFrame*)m_pMainWnd)->m_aWebProxySpeed.GetNext(pos);
		i++;
	}
	i = 1;
	pos = ((CMainFrame*)m_pMainWnd)->m_astrWebProxyName.GetHeadPosition();
	while(pos!=NULL)
	{
		itoa(i, Keyname3+3, 10);
		WriteProfileString("WebProxy", Keyname3, ((CMainFrame*)m_pMainWnd)->m_astrWebProxyName.GetAt(pos));
		((CMainFrame*)m_pMainWnd)->m_astrWebProxyName.GetNext(pos);
		i++;
	}*/

	//save defaul web proxy
	WriteProfileString("Settings","DefaultWebProxy", ((CMainFrame*)m_pMainWnd)->m_strDefaultWebProxy);
	//save by pass
	WriteProfileString("Settings","ProxyByPass", ((CMainFrame*)m_pMainWnd)->m_strProxyByPass);
	//save proxy list
	WriteProfileInt("Settings","ProxyList", ((CMainFrame*)m_pMainWnd)->m_nCurProxyList);

//get current setting
/*2.0
	CString strCurProxy, bypass, user, pwd;
	if(((CMainFrame*)m_pMainWnd)->GetProxySetting(strCurProxy, bypass, user, pwd))
	{	//save the curproxy
		if(!((CMainFrame*)m_pMainWnd)->m_astrProxy.Find(strCurProxy))
		{
			((CMainFrame*)m_pMainWnd)->m_astrProxy.AddHead(strCurProxy);
			((CMainFrame*)m_pMainWnd)->m_aProxySpeed.AddHead(3);
//			((CMainFrame*)m_pMainWnd)->m_astrProxyUser.AddHead(user);
//			((CMainFrame*)m_pMainWnd)->m_astrProxyPwd.AddHead(pwd);
		}
	}

*/
	//save proxy servers
/*	allpopups.Empty();
	pos = ((CMainFrame*)m_pMainWnd)->m_astrProxy.GetHeadPosition();
	i=0;
	while(pos!=NULL)
	{
		allpopups += ((CMainFrame*)m_pMainWnd)->m_astrProxy.GetAt(pos);
		allpopups += "\n";
		((CMainFrame*)m_pMainWnd)->m_astrProxy.GetNext(pos);
	}
	WriteProfileBinary("Settings","Proxy", (LPBYTE)allpopups.GetBuffer(1), allpopups.GetLength()+1);
*/
	//save proxy users
/*	allpopups.Empty();
	pos = ((CMainFrame*)m_pMainWnd)->m_astrProxyUser.GetHeadPosition();
	i=0;
	while(pos!=NULL)
	{
		allpopups += ((CMainFrame*)m_pMainWnd)->m_astrProxyUser.GetAt(pos);
		allpopups += "\n";
		((CMainFrame*)m_pMainWnd)->m_astrProxyUser.GetNext(pos);
	}
	WriteProfileBinary("Settings","ProxyUser", (LPBYTE)allpopups.GetBuffer(1), allpopups.GetLength()+1);

	//save proxy servers password
	allpopups.Empty();
	pos = ((CMainFrame*)m_pMainWnd)->m_astrProxyPwd.GetHeadPosition();
	i=0;
	while(pos!=NULL)
	{
		allpopups += ((CMainFrame*)m_pMainWnd)->m_astrProxyPwd.GetAt(pos);
		allpopups += "\n";
		((CMainFrame*)m_pMainWnd)->m_astrProxyPwd.GetNext(pos);
	}
	WriteProfileBinary("Settings","ProxyPwd", (LPBYTE)allpopups.GetBuffer(1), allpopups.GetLength()+1);
*/
	//save proxy speed
/*	tmp = NULL;
	pos = ((CMainFrame*)m_pMainWnd)->m_aProxySpeed.GetHeadPosition();
	i=0;
	cn = ((CMainFrame*)m_pMainWnd)->m_aProxySpeed.GetCount();
	if(cn>0)
		tmp = new BYTE[cn];
	while(pos!=NULL)
	{
		tmp[i] = ((CMainFrame*)m_pMainWnd)->m_aProxySpeed.GetAt(pos);
		((CMainFrame*)m_pMainWnd)->m_aProxySpeed.GetNext(pos);
		i++;
	}
	WriteProfileBinary("Settings","ProxySP", tmp, cn);
	if(tmp!=NULL)
		delete tmp;
*/
/*2.3	i = 1;
	WriteProfileString("Proxy", NULL, "");
	Keyname1[0]='p'; Keyname2[0]='p'; Keyname2[1]='s'; Keyname3[0]='p'; Keyname3[1]='n';
	pos = ((CMainFrame*)m_pMainWnd)->m_astrProxy.GetHeadPosition();
	while(pos!=NULL)
	{
		itoa(i, Keyname1+1, 10);
		WriteProfileString("Proxy", Keyname1, ((CMainFrame*)m_pMainWnd)->m_astrProxy.GetAt(pos));
		((CMainFrame*)m_pMainWnd)->m_astrProxy.GetNext(pos);
		i++;
	}
	i = 1;
	pos = ((CMainFrame*)m_pMainWnd)->m_aProxySpeed.GetHeadPosition();
	while(pos!=NULL)
	{
		itoa(i, Keyname2+2, 10);
		WriteProfileInt("Proxy", Keyname2, ((CMainFrame*)m_pMainWnd)->m_aProxySpeed.GetAt(pos));
		((CMainFrame*)m_pMainWnd)->m_aProxySpeed.GetNext(pos);
		i++;
	}
	i = 1;
	pos = ((CMainFrame*)m_pMainWnd)->m_astrProxyName.GetHeadPosition();
	while(pos!=NULL)
	{
		itoa(i, Keyname3+2, 10);
		WriteProfileString("Proxy", Keyname3, ((CMainFrame*)m_pMainWnd)->m_astrProxyName.GetAt(pos));
		((CMainFrame*)m_pMainWnd)->m_astrProxyName.GetNext(pos);
		i++;
	}
*/
	//save defaul  proxy
	WriteProfileString("Settings","DefaultProxy", ((CMainFrame*)m_pMainWnd)->m_strCurProxy);

	//tab pos
	WriteProfileInt("Settings", "TabPos", m_bTopTab);

	//save utils names 
/*	allpopups.Empty();
	pos = ((CMainFrame*)m_pMainWnd)->m_lstrUtilNames.GetHeadPosition();
	i=0;
	while(pos!=NULL)
	{
		allpopups += ((CMainFrame*)m_pMainWnd)->m_lstrUtilNames.GetAt(pos);
		allpopups += "\n";
		((CMainFrame*)m_pMainWnd)->m_lstrUtilNames.GetNext(pos);
	}
	WriteProfileBinary("ExUtils","names", (LPBYTE)allpopups.GetBuffer(1), allpopups.GetLength()+1);
	//save utils commands
	allpopups.Empty();
	pos = ((CMainFrame*)m_pMainWnd)->m_lstrUtilCmds.GetHeadPosition();
	i=0;
	while(pos!=NULL)
	{
		allpopups += ((CMainFrame*)m_pMainWnd)->m_lstrUtilCmds.GetAt(pos);
		allpopups += "\n";
		((CMainFrame*)m_pMainWnd)->m_lstrUtilCmds.GetNext(pos);
	}
	WriteProfileBinary("ExUtils","cmds", (LPBYTE)allpopups.GetBuffer(1), allpopups.GetLength()+1);
*/
	i = 1;
	WriteProfileString("ExUtils", NULL, "");
	strcpy(Keyname1, "name"); 
	strcpy(Keyname2, "cmds");
	strcpy(Keyname3, "sta");
	strcpy(Keyname4, "cls");
	CExternalUtilItem* eui;
	for (int ir=0;ir <= ((CMainFrame*)m_pMainWnd)->m_ExternalUtilList.m_UtilList.GetUpperBound();ir++)
	{
		eui = ((CMainFrame*)m_pMainWnd)->m_ExternalUtilList.m_UtilList.GetAt(ir);
		itoa(ir+1, Keyname1+4, 10);
		itoa(ir+1, Keyname2+4, 10);
		itoa(ir+1, Keyname3+3, 10);
		itoa(ir+1, Keyname4+3, 10);
		WriteProfileString("ExUtils", Keyname1, eui->m_strUtilName);
		WriteProfileString("ExUtils", Keyname2, eui->m_strUtilCmd);
		WriteProfileInt("ExUtils", Keyname3, eui->m_bUtilStart);
		WriteProfileInt("ExUtils", Keyname4, eui->m_bUtilClose);
	}

	//active new
	WriteProfileInt("Settings", "ActiveNew", ((CMainFrame*)m_pMainWnd)->m_bActiveNewWindow);
	//active new
	WriteProfileInt("Settings", "AddressInNewWindow", bAddressInNewWindow);
	WriteProfileInt("Settings", "LinkInNewWindow", bLinkInNewWindow);
	WriteProfileInt("Settings", "FavInNewWindow", bFavInNewWindow);
	WriteProfileInt("Settings", "HisInNewWindow", bHisInNewWindow);
	WriteProfileInt("Settings", "FileInNewWindow", bFileInNewWindow);

	//start favorite folder
	WriteProfileString("Settings","StartFolder", ((CMainFrame*)m_pMainWnd)->m_strStartFavFolder);

	//multiline tab
	WriteProfileInt("Settings", "MultiLineTab", ((CMainFrame*)m_pMainWnd)->m_bMultiLineTab);

	//insert to next tab
	WriteProfileInt("Settings", "NextTab", ((CMainFrame*)m_pMainWnd)->m_bNextTab);

	//display ip
	WriteProfileInt("Settings", "DisplayIP", ((CMainFrame*)m_pMainWnd)->m_bDisplayIP);

	//display mem
	WriteProfileInt("Settings", "DisplayMem", ((CMainFrame*)m_pMainWnd)->m_bDispMem);
	
	//display bp
	WriteProfileInt("Settings", "DispSpeed", ((CMainFrame*)m_pMainWnd)->m_DispBP);

	//Background PIC
	WriteProfileString("Settings", "Background", ((CMainFrame*)m_pMainWnd)->m_strBGPic);

	//Exp Bar
	CString str="";
	if(((CMainFrame*)m_pMainWnd)->m_wndInstantBar.GetSafeHwnd()==NULL)
		str = "";
	else if(((CMainFrame*)m_pMainWnd)->m_wndInstantBar.IsVisible())
		((CMainFrame*)m_pMainWnd)->m_wndInstantBar.GetWindowText(str);
	WriteProfileString("Settings", "ExpBar", str);
	if(((CMainFrame*)m_pMainWnd)->m_wndInstantBar.GetSafeHwnd()!=NULL)
	{
		CRect rect;
		((CMainFrame*)m_pMainWnd)->m_wndInstantBar.GetWindowRect(&rect);
		WriteProfileInt("Settings", "ExpWidth", rect.Width());
	}

	//text label
	WriteProfileInt("Settings", "TextLabel", ((CMainFrame*)m_pMainWnd)->m_nTextLabel);
	//text label
	WriteProfileInt("Settings", "ColorIcon", ((CMainFrame*)m_pMainWnd)->m_bColorIcon);

	//autosizing tab
	WriteProfileInt("Settings","AutoTab", ((CMainFrame*)m_pMainWnd)->m_bAutoTab);
	//flat button tab
	WriteProfileInt("Settings","FBTab", ((CMainFrame*)m_pMainWnd)->m_bFBTab);

	//max autosizing tab width
	WriteProfileInt("Settings","MaxTabWidth", ((CMainFrame*)m_pMainWnd)->m_nMaxTabWidth);
	//min autosizing tab width
	WriteProfileInt("Settings","MinTabWidth", ((CMainFrame*)m_pMainWnd)->m_nMinTabWidth);

	//confirm when exit
	WriteProfileInt("Settings","ConfirmExit", bConfirmExit);

	//use shortcut
	WriteProfileInt("Settings","UseSC", ((CMainFrame*)m_pMainWnd)->m_bUseSC);

	//use alias
	WriteProfileInt("Settings","UseAlias", ((CMainFrame*)m_pMainWnd)->m_bUseAlias);

	//save shortcuts
	char key[9]="F";
	for(i=0;i<11;i++)
	{
		itoa(i+2, key+1, 10);
		WriteProfileString("Shortcuts", key, ((CMainFrame*)m_pMainWnd)->m_strShortCuts[i]);
	}

	//save alias
	CString alias, url;
	pos = ((CMainFrame*)pMainFrame)->m_mapAlias.GetStartPosition();
		i=0;
		while(pos!=NULL)
		{
			((CMainFrame*)pMainFrame)->m_mapAlias.GetNextAssoc(pos, alias, url);
			key[0]='A';
			itoa(i, key+1, 10);
			WriteProfileString("Alias", key, alias);
			key[0]='U';
			itoa(i, key+1, 10);
			WriteProfileString("Alias", key, url);
			i++;
		}
		key[0]='A';
		itoa(i, key+1, 10);
		WriteProfileString("Alias", key, "");
		key[0]='U';
		itoa(i, key+1, 10);
		WriteProfileString("Alias", key, "");

	//max menu width
	WriteProfileInt("Settings","MenuSize", ((CMainFrame*)m_pMainWnd)->m_nMenuWidth);

	//domains
	WriteProfileString("Domains", "CE1", ((CMainFrame*)pMainFrame)->m_strCE1);
	WriteProfileString("Domains", "CE2", ((CMainFrame*)pMainFrame)->m_strCE2);
	WriteProfileString("Domains", "SE1", ((CMainFrame*)pMainFrame)->m_strSE1);
	WriteProfileString("Domains", "SE2", ((CMainFrame*)pMainFrame)->m_strSE2);
	WriteProfileString("Domains", "CSE1", ((CMainFrame*)pMainFrame)->m_strCSE1);
	WriteProfileString("Domains", "CSE2", ((CMainFrame*)pMainFrame)->m_strCSE2);

	//single menu
	WriteProfileInt("Settings","SingleMenu", bSingleMenu);
	//sys tray
	WriteProfileInt("Settings","SysTray", bSysTray);
	//3721
	WriteProfileInt("Settings","Use3721", ((CMainFrame*)pMainFrame)->m_bUse3721);
	//max wins
	WriteProfileInt("Settings","MaxWins", nMaxWins);
	//sequence
	WriteProfileInt("Settings", "Sequence", ((CMainFrame*)m_pMainWnd)->m_nSeq);
	//silent
	WriteProfileInt("Settings", "Silent", bSilent);
	//defaut font size
	WriteProfileInt("Settings", "DefFontSize", nDefFontSize);
	//disable scroll text on statusbar
	WriteProfileInt("Settings", "DisScrollText", bURLOnly);
	//interval
	WriteProfileInt("Settings", "Interval", ((CMainFrame*)m_pMainWnd)->m_nInterval);
	//open link in new window
//	WriteProfileInt("Settings", "LinkInNewWin", bLinkInNewWin);

	//default browse mode
	WriteProfileInt("BModes", "ActiveNewWindow", ((CMainFrame*)m_pMainWnd)->m_bmUserDefault.m_bActiveNewWindow);  
	WriteProfileInt("BModes", "AlwaysNewWindow", ((CMainFrame*)m_pMainWnd)->m_bmUserDefault.m_bAlwaysNewWindow);  
	WriteProfileInt("BModes", "Seq", ((CMainFrame*)m_pMainWnd)->m_bmUserDefault.m_nSeq); 
	WriteProfileInt("BModes", "LinkInNewWin", ((CMainFrame*)m_pMainWnd)->m_bmUserDefault.bLinkInNewWin);

	//auto scroll
	WriteProfileInt("Settings", "AutoScrollAfterDL", ((CMainFrame*)m_pMainWnd)->m_bDefScrollAfterDL); 
	WriteProfileInt("Settings", "UseAutoScroll", ((CMainFrame*)m_pMainWnd)->m_bDefUseAutoScroll);
	WriteProfileInt("Settings", "SrSpeed", ((CMainFrame*)m_pMainWnd)->m_nDefSrSpeed);  

	WriteProfileInt("Settings", "ConfirmCloseAll", bConfirmCloseAll);
	WriteProfileInt("Settings", "OneInstance", bOneInstance);

	//start group
	WriteProfileString("Settings", "StartGroup", ((CMainFrame*)m_pMainWnd)->m_strStartGroup);

	//new window
	WriteProfileInt("Settings", "NewWindow", ((CMainFrame*)m_pMainWnd)->m_nNewWindow);
	WriteProfileInt("Settings", "NewWindow2", ((CMainFrame*)m_pMainWnd)->m_nNewWindow2);

	//allow double click to close window
	WriteProfileInt("Settings", "AllowDBC", ((CMainFrame*)m_pMainWnd)->m_nAllowDBC);
	WriteProfileInt("Settings", "AllowMC", ((CMainFrame*)m_pMainWnd)->m_nAllowMC);
	WriteProfileInt("Settings", "AllowRC", ((CMainFrame*)m_pMainWnd)->m_nAllowRC);

	//group path
	WriteProfileString("Settings", "GroupPath", m_strGroupPath);

	//image path
	WriteProfileString("Settings", "ImagePath", m_strImagePath);

	//default bg color
	WriteProfileString("Settings", "BGColor", ((CMainFrame*)m_pMainWnd)->m_strBGColor);

	//display open all fav
	WriteProfileInt("Settings", "DisplayOpenAll", ((CMainFrame*)m_pMainWnd)->m_bDisplayOpenAll);
	WriteProfileInt("Settings", "DisplayAddFav", ((CMainFrame*)m_pMainWnd)->m_bDisplayAddFav);

	//default auto refresh
	WriteProfileInt("Settings", "AutoRefresh", ((CMainFrame*)m_pMainWnd)->m_nDefRefreshInterval);

	//toolbar icon
	WriteProfileInt("Settings", "SmallToolbar", ((CMainFrame*)m_pMainWnd)->m_bSmallToolBar);

	//auto set background
	WriteProfileInt("Settings", "AutoSetBG", bAutoSetBG);

	WriteProfileInt("Settings", "CompactFavBar", bCompactFavBar);
	WriteProfileInt("Settings", "AnimateIcon", ((CMainFrame*)m_pMainWnd)->m_bAnimatIcon);

	WriteProfileInt("Settings", "MouseGestures", ((CMainFrame*)m_pMainWnd)->m_bAllowMouseGestures);

	WriteProfileInt("Settings", "LockHomepage", bLockHomepage);

	//auto save
	WriteProfileInt("Settings", "AutoSave", ((CMainFrame*)m_pMainWnd)->m_nAutoSave);
	WriteProfileInt("Settings", "CateWithSite", ((CMainFrame*)m_pMainWnd)->m_bCateWithSite);

	//most favorite checked
	WriteProfileInt("Settings", "MostFavChked", ((CMainFrame*)m_pMainWnd)->m_bMostFavChecked);

	//most favorite checked
	WriteProfileInt("Settings", "MouseCtrlSpeed", ((CMainFrame*)m_pMainWnd)->m_bMouseCtrlScrSpeed);

	//lock toolbar
	WriteProfileInt("Settings", "LockToolbar", ((CMainFrame*)m_pMainWnd)->m_bLockToolbar);

	//save win buttons
	key[0]='W';
	for(i=0; i <6; i++)
	{
		itoa(i, key+1, 10);
		WriteProfileInt("WinButtons", key, ((CMainFrame*)m_pMainWnd)->m_abWinButtons[i]);
	}

	//3.2 clear in autofilter flag
	WriteProfileInt("Settings", "InAutoFilter", 0);
	}
	catch(...)
	{
	}
}



BOOL CMyIEApp::OnIdle(LONG lCount) 
{
	// TODO: Add your specialized code here and/or call the base class
	try{

	if(pMainFrame!=NULL)
	{
		if(((CMainFrame*)pMainFrame)->m_bFavDirty==3 || (((CMainFrame*)pMainFrame)->m_bFavDirty && nDelay>30))
		{
			//((CMainFrame*)pMainFrame)->InitFavorites();
			((CMainFrame*)pMainFrame)->PostMessage(WM_UPDATE_FAV);
			((CMainFrame*)pMainFrame)->m_bFavDirty = FALSE;
			nDelay=0;
		}
		else if(((CMainFrame*)pMainFrame)->m_bFavDirty)
			nDelay++;

		//auto start
		if(m_bAutoStart)
		{
			int max = ((CMainFrame*)pMainFrame)->m_ExternalUtilList.m_UtilList.GetUpperBound();
			if(m_nAutoStart <= max)
			{
				while(m_nAutoStart <= max && ((CMainFrame*)pMainFrame)->m_ExternalUtilList.m_UtilList.GetAt(m_nAutoStart)->m_bUtilStart==FALSE)
				{
					m_nAutoStart++;
				}
				if(m_nAutoStart <= max)
				{
					//start the tool
					((CMainFrame*)pMainFrame)->StartUtil(m_nAutoStart);
					m_nAutoStart++;
					((CMainFrame*)pMainFrame)->m_bNeedActive = TRUE;
				}
				else
					m_bAutoStart = FALSE;
			}
			else
				m_bAutoStart = FALSE;

		}
		
		
		if(((CMainFrame*)pMainFrame)->m_bSaveConfig)
		{
			((CMainFrame*)pMainFrame)->m_bSaveConfig = FALSE;
			AfxBeginThread(TSaveConfig, NULL);
		}
	}

/*2.2

//	return CWinApp::OnIdle(lCount);
	if (lCount <=0)
	{
		// send WM_IDLEUPDATECMDUI to the main window
/*		CWnd* pMainWnd = m_pMainWnd;
		if (pMainWnd != NULL && pMainWnd->m_hWnd != NULL &&
			pMainWnd->IsWindowVisible())
		{
//			AfxCallWndProc(pMainWnd, pMainWnd->m_hWnd,
//				WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0);
		}* /
	}
	else if (lCount >= 0)
	{
		AFX_MODULE_THREAD_STATE* pState = _AFX_CMDTARGET_GETSTATE()->m_thread;
		if (pState->m_nTempMapLock == 0)
		{
			// free temp maps, OLE DLLs, etc.
			AfxLockTempMaps();
			AfxUnlockTempMaps();
		}
	//	_heapmin();

	}

	return lCount < 1;  // more to do if lCount < 1
*/
//added at 2.2
	if (lCount <= 0)
	{
		ThreadOnIdle(lCount);

		// call doc-template idle hook
/*328		POSITION pos = NULL;
		if (m_pDocManager != NULL)
			pos = m_pDocManager->GetFirstDocTemplatePosition();

		while (pos != NULL)
		{
			CDocTemplate* pTemplate = m_pDocManager->GetNextDocTemplate(pos);
			ASSERT_KINDOF(CDocTemplate, pTemplate);
			pTemplate->OnIdle();
		}

*/
	}
	else if (lCount == 1)
	{
		ThreadOnIdle(lCount);
	}
	return lCount < 1;  // more to do if lCount < 1

	}
	catch(...)
	{
		return lCount < 1;
	}

}

BOOL CMyIEApp::ThreadOnIdle(LONG lCount)
{
	ASSERT_VALID(this);
/*
#if defined(_DEBUG) && !defined(_AFX_NO_DEBUG_CRT)
	// check MFC's allocator (before idle)
	if (_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) & _CRTDBG_CHECK_ALWAYS_DF)
		ASSERT(AfxCheckMemory());
#endif*/

	try{

	if (lCount <= 0)
	{
		// send WM_IDLEUPDATECMDUI to the main window
/*		CWnd* pMainWnd = m_pMainWnd;
		if (pMainWnd != NULL && pMainWnd->m_hWnd != NULL &&
			pMainWnd->IsWindowVisible())
		{
			AfxCallWndProc(pMainWnd, pMainWnd->m_hWnd,
				WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0);
			pMainWnd->SendMessageToDescendants(WM_IDLEUPDATECMDUI,
				(WPARAM)TRUE, 0, TRUE, TRUE);
		}
		// send WM_IDLEUPDATECMDUI to all frame windows
		AFX_MODULE_THREAD_STATE* pState = _AFX_CMDTARGET_GETSTATE()->m_thread;
		CFrameWnd* pFrameWnd = pState->m_frameList;
		while (pFrameWnd != NULL)
		{
			if (pFrameWnd->m_hWnd != NULL && pFrameWnd != pMainWnd)
			{
				if (pFrameWnd->m_nShowDelay == SW_HIDE)
					pFrameWnd->ShowWindow(pFrameWnd->m_nShowDelay);
				if (pFrameWnd->IsWindowVisible() ||
					pFrameWnd->m_nShowDelay >= 0)
				{
					AfxCallWndProc(pFrameWnd, pFrameWnd->m_hWnd,
						WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0);
					pFrameWnd->SendMessageToDescendants(WM_IDLEUPDATECMDUI,
						(WPARAM)TRUE, 0, TRUE, TRUE);
				}
				if (pFrameWnd->m_nShowDelay > SW_HIDE)
					pFrameWnd->ShowWindow(pFrameWnd->m_nShowDelay);
				pFrameWnd->m_nShowDelay = -1;
			}
			pFrameWnd = pFrameWnd->m_pNextFrameWnd;
		}*/
	}
	else if (lCount >= 0)
	{
		AFX_MODULE_THREAD_STATE* pState = _AFX_CMDTARGET_GETSTATE()->m_thread;
		if (pState->m_nTempMapLock == 0)
		{
			// free temp maps, OLE DLLs, etc.
			AfxLockTempMaps();
			AfxUnlockTempMaps();
		}
	}

	}catch(...)
	{
	}

/*
#if defined(_DEBUG) && !defined(_AFX_NO_DEBUG_CRT)
	// check MFC's allocator (after idle)
	if (_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) & _CRTDBG_CHECK_ALWAYS_DF)
		ASSERT(AfxCheckMemory());
#endif*/

	return lCount < 0;  // nothing more to do if lCount >= 0
}


/*long LoadBytes()
{
	HKEY            hKey;
	long            sz = 0;
	DWORD           dwSize = 4;

	try{

	if(RegOpenKey(HKEY_DYN_DATA, _T("PerfStats\\StatData"), &hKey) != ERROR_SUCCESS)
	{
		TRACE0("Main setting not found\n");
		return -300;
	}
	dwSize = sizeof(sz);
	sz=0;
	RegQueryValueEx(hKey, _T("Dial-Up Adapter\\TotalBytesRecvd"), NULL, NULL, (LPBYTE)&sz, &dwSize);
	
	}
	catch(...)
	{
	}

	RegCloseKey(hKey);
	return sz;
}*/







	
CMyIEModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_MyIENSHandle, CMyIENSHandle)
END_OBJECT_MAP()

LONG CMyIEModule::Unlock()
{
	AfxOleUnlockApp();
	return 0;
}

LONG CMyIEModule::Lock()
{
	AfxOleLockApp();
	return 1;
}
LPCTSTR CMyIEModule::FindOneOf(LPCTSTR p1, LPCTSTR p2)
{
	while (*p1 != NULL)
	{
		LPCTSTR p = p2;
		while (*p != NULL)
		{
			if (*p1 == *p)
				return CharNext(p1);
			p = CharNext(p);
		}
		p1++;
	}
	return NULL;
}


BOOL CMyIEApp::InitATL()
{
	m_bATLInited = TRUE;
/*
#if _WIN32_WINNT >= 0x0400
	HRESULT hRes = CoInitializeEx(NULL, COINIT_MULTITHREADED);
#else
	HRESULT hRes = CoInitialize(NULL);
#endif*/

/*	if (FAILED(hRes))
	{
		m_bATLInited = FALSE;
		return FALSE;
	}*/

	_Module.Init(ObjectMap, AfxGetInstanceHandle());
	_Module.dwThreadID = GetCurrentThreadId();

	LPTSTR lpCmdLine = GetCommandLine(); //this line necessary for _ATL_MIN_CRT
	TCHAR szTokens[] = _T("-/");

	BOOL bRun = TRUE;
	LPCTSTR lpszToken = _Module.FindOneOf(lpCmdLine, szTokens);
	while (lpszToken != NULL)
	{
		if (lstrcmpi(lpszToken, _T("UnregServer"))==0)
		{
			_Module.UpdateRegistryFromResource(IDR_MYIE, FALSE);
			_Module.UnregisterServer(TRUE); //TRUE means typelib is unreg'd
			bRun = FALSE;
			break;
		}
		if (lstrcmpi(lpszToken, _T("RegServer"))==0)
		{
			_Module.UpdateRegistryFromResource(IDR_MYIE, TRUE);
			_Module.RegisterServer(TRUE);
			bRun = FALSE;
			break;
		}
		lpszToken = _Module.FindOneOf(lpszToken, szTokens);
	}

	if (!bRun)
	{
		m_bATLInited = FALSE;
		_Module.Term();
//		CoUninitialize();
		return FALSE;
	}

	HRESULT hRes = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER, 
		REGCLS_MULTIPLEUSE);
	if (FAILED(hRes))
	{
		m_bATLInited = FALSE;
//		CoUninitialize();
		return FALSE;
	}	

	return TRUE;

}
