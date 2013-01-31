// FavTree.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE.h"
#include "FavTree.h"
#include "MainFrm.h"
#include "MenuOrder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// CFavTree

CFavTree::CFavTree()
{
	//does display hidden files
	HKEY hKey;
	DWORD dwSize;
	if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced"), &hKey) == ERROR_SUCCESS)
	{

		RegQueryValueEx(hKey, _T("Hidden"), NULL, NULL, (LPBYTE)&m_dwHidden, &dwSize);
		RegCloseKey(hKey);	
	}
	
	m_bFirstInitial = TRUE;
}

CFavTree::~CFavTree()
{
	try{
	m_wndFav.m_FavTreeCtrl.DeleteAllItems();
	m_wndFav.m_FavTreeCtrl.m_astrFav.RemoveAll();
	}catch(...)
	{
	}

}



/////////////////////////////////////////////////////////////////////////////
// CFavTree message handlers

LPCSTR CFavTree::GetTitle()
{
	strTitle.LoadString(IDS_TITLE_FAVORITE);
	return (LPCSTR)strTitle;
}

BOOL CFavTree::Initialize()
{
	// TODO: Add your command handler code here
	CWaitCursor wc;

	CString           szPath;

	// find out from the registry where the favorites are located.
	if(!((CMainFrame*)pMainFrame)->GetFavoriteFolder(szPath))
		return FALSE;


	if(m_bFirstInitial)
	{
		m_wndFav.m_bBest = ((CMainFrame*)pMainFrame)->m_bMostFavChecked;
		m_wndFav.m_ToolBar.GetToolBarCtrl().CheckButton(ID_FAVORITES_BEST, m_wndFav.m_bBest);
		m_bFirstInitial = FALSE;
	}

	HTREEITEM hRoot;
	CString fstr;
	fstr.LoadString(IDS_TITLE_FAVORITE);
	hRoot = m_wndFav.m_FavTreeCtrl.InsertItem(fstr, 0, 2, NULL,TVI_FIRST);
	m_wndFav.m_FavTreeCtrl.SetItemData(hRoot, -1);

	BuildFavorites(szPath,0, hRoot);
	m_wndFav.m_FavTreeCtrl.Expand(hRoot,TVE_EXPAND);

	if(szPath == ((CMainFrame*)pMainFrame)->m_strStartFavFolder)
	{
		m_wndFav.m_FavTreeCtrl.SetItemState(hRoot,TVIS_BOLD,TVIS_BOLD);
		m_wndFav.m_FavTreeCtrl.m_hStartFolder = hRoot;
	}


	m_wndFav.m_FavTreeCtrl.m_bDragging = FALSE;

	return TRUE;
}

BOOL CFavTree::Create(CWnd* pParent)
{
	m_wndFav.Create(NULL,NULL, WS_CHILD|WS_VISIBLE,CRect(0,0,0,0), pParent, 100);
	m_wndFav.m_FavTreeCtrl.m_pFavTree = this;
	return TRUE;
}

CWnd* CFavTree::GetWindow()
{
	return (CWnd*)&m_wndFav;
}

void CFavTree::Destroy()
{
	try{
	m_wndFav.m_FavTreeCtrl.DeleteAllItems();
	m_wndFav.m_FavTreeCtrl.m_astrFav.RemoveAll();
	((CMainFrame*)pMainFrame)->m_bMostFavChecked = m_wndFav.m_bBest;

	}catch(...)
	{
	}
}


BOOL CFavTree::Update(int nLevel)
{
	m_wndFav.LockWindowUpdate();
	m_wndFav.m_FavTreeCtrl.DeleteAllItems();
	m_wndFav.m_FavTreeCtrl.m_astrFav.RemoveAll();
	Initialize();
	m_wndFav.UnlockWindowUpdate();
	m_wndFav.m_FavTreeCtrl.m_bDragging = FALSE;

	return TRUE;
}

int CFavTree::BuildFavorites(LPCTSTR pszPath, int nStartPos, HTREEITEM hItem, int nLevel)
{
	CString         strPath(pszPath);
	CString         strPath2;
	CString         str,str2;
	WIN32_FIND_DATA wfd;
	HANDLE          h;
	int             nPos;
	int             nEndPos;
//	int             nNewEndPos;
	int             nLastDir;
	TCHAR           buf[2048], tmp[10];
	CStringArray    astrFavorites;
	CArray<int, int>	anFavID;
	CStringArray    astrDirs;
	HTREEITEM		hnode;
	BOOL			bBest = m_wndFav.m_bBest;


	// make sure there's a trailing backslash
	if(strPath[strPath.GetLength() - 1] != _T('\\'))
		strPath += _T('\\');
	strPath2 = strPath;
	strPath += "*.*";

	nLastDir = 0;
//3.01	CString strcha = "ÆµµÀ";
//3.01	CString strcha2; strcha2.LoadString(IDS_CHANNEL_E);
	// now scan the directory, first for .URL files and then for subdirectories
	// that may also contain .URL files
	h = FindFirstFile(strPath, &wfd);
	DWORD fileattr;
	if(m_dwHidden == 1)
		fileattr = FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_SYSTEM;
	else
		fileattr = FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_SYSTEM;
	if(h != INVALID_HANDLE_VALUE)
	{
		nEndPos = nStartPos;
		do
		{
			if((wfd.dwFileAttributes & fileattr)==0)
			{
				str = wfd.cFileName;str2=str;
				str.MakeLower();
				if(str.Right(4) == _T(".url"))
				{
					tmp[0]=0;
					// an .URL file is formatted just like an .INI file, so we can
					// use GetPrivateProfileString() to get the information we want
					::GetPrivateProfileString(_T("InternetShortcut"), _T("URL"),
											  _T(""), buf, 2048,
											  strPath2 + str);
					::GetPrivateProfileString(_T("MyIE"), _T("MostFav"),
											  _T(""), tmp, 10,
											  strPath2 + str);
					//str2 = str2.Left(str2.GetLength() - 4);


					// scan through the array and perform an insertion sort
					// to make sure the menu ends up in alphabetic order
					/*for(nPos = nStartPos ; nPos < nEndPos ; ++nPos)
					{
						if(str.CompareNoCase(astrFavorites[nPos]) < 0)
							break;
					}*/
					if(!bBest || (bBest && tmp[0]=='1'))
					{
						nPos = nEndPos;
						astrFavorites.Add(str2);
						anFavID.Add(nPos);
						m_wndFav.m_FavTreeCtrl.m_astrFav.AddTail(buf);
						++nEndPos;
					}
				}
				else  if(!bBest && str.Right(4) == _T(".lnk"))
				{
					//if(!bBest && ((CMainFrame*)pMainFrame)->ResolveLink(strPath2 + str, buf))
					{

						nPos = nEndPos;
						m_wndFav.m_FavTreeCtrl.m_astrFav.AddTail(strPath2 + str);
						//str2 = str2.Left(str2.GetLength() - 4);
						astrFavorites.Add(str2);
						anFavID.Add(nPos);
						++nEndPos;
					}
				}
			}
			else if((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (m_dwHidden==1 || (wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)==0) )
			{
				// ignore the current and parent directory entries
				if(lstrcmp(wfd.cFileName, _T(".")) == 0 || lstrcmp(wfd.cFileName, _T("..")) == 0)
					continue;

			//	nPos = nLastDir;

				astrDirs.Add(wfd.cFileName);
				++nLastDir;
			}

		} while(FindNextFile(h, &wfd));
		FindClose(h);



		//sort and add to tree
		CMenuOrder mo;
		HTREEITEM hLastDir=TVI_LAST;
		BOOL found;
		int j;
		WORD type;
		char mff[10];
		if(((CMainFrame*)pMainFrame)->GetFavoriteFolder(strPath))
		{
			strPath = strPath2.Mid(strPath.GetLength()+1);
			if(mo.LoadMenuOrder(strPath))
			{
				for(int i = 0; i<mo.len; i++)
				{
					str = mo.mois[mo.index[i]].longname;
					type = mo.mois[mo.index[i]].filetype;
					//find in url first
					found = FALSE;
					for(j = 0; j<astrFavorites.GetSize() && !found && (type==0x20 || type==0); j++)
					{
						if(astrFavorites[j].CompareNoCase(str)==0)
						{
							found = TRUE;
							//add
							str = astrFavorites[j];
							str.MakeLower();
							buf[0]='\0';
							if(str.Right(4)==".url")
								::GetPrivateProfileString(_T("MyIE"), _T("MostFav"),
										  _T(""), buf, 2048,
										  strPath2 + str);
							str = astrFavorites[j].Left(str.GetLength()-4);
							if(buf[0]!='1' && !bBest)
								hnode = m_wndFav.m_FavTreeCtrl.InsertItem(str, 1, 1, hItem,TVI_LAST);
							else if(buf[0]=='1')
								hnode = m_wndFav.m_FavTreeCtrl.InsertItem(str, 4, 4, hItem,TVI_LAST);
							m_wndFav.m_FavTreeCtrl.SetItemData(hnode, anFavID[j]);
							astrFavorites[j].Empty();
						}
					}
					for(j = 0; j<astrDirs.GetSize() && !found && (type==0x10 || type==0x14 || type==0); j++)
					{
						if(astrDirs[j].CompareNoCase(str)==0)
						{
							//add
							mff[0]='\0';
							GetPrivateProfileString("MyIE", "MostFav", "0", mff, 2, strPath2+astrDirs[j]+"\\"+"myiefolder.mic");
							if(mff[0]=='1')
								hnode = m_wndFav.m_FavTreeCtrl.InsertItem(astrDirs[j], 5, 6, hItem,hLastDir);
							else
								hnode = m_wndFav.m_FavTreeCtrl.InsertItem(astrDirs[j], 0, 2, hItem,hLastDir);

							m_wndFav.m_FavTreeCtrl.SetItemData(hnode, -1);

//3.01							if((astrDirs[j]==strcha || astrDirs[j]==strcha2))//nNewEndPos == nEndPos &&
//								m_wndFav.m_FavTreeCtrl.DeleteItem(hnode);
//							else
							{
								
								if(strPath2+astrDirs[j]+"\\" == ((CMainFrame*)pMainFrame)->m_strStartFavFolder)
								{
									m_wndFav.m_FavTreeCtrl.m_hStartFolder = hnode;
									m_wndFav.m_FavTreeCtrl.SetItemState(hnode,TVIS_BOLD,TVIS_BOLD);
								}
							}
							astrDirs[j].Empty();
						}
					}

				}
			}
		}

		//sort dirs and favs
		CString tmp;
		int n = astrDirs.GetSize();
		int i, ch, m;
		ch = n;
		while(ch>0)
		{
			m = ch-1; ch = 0;
			for(i=1; i<=m; i++)
			{
				if(lstrcmp(astrDirs[i-1], astrDirs[i])>0)
				{
					tmp = astrDirs[i-1];
					astrDirs[i-1] = astrDirs[i];
					astrDirs[i] = tmp;
					ch = i;
				}
			}
		}
		
		n = astrFavorites.GetSize();
		ch = n;
		int ntmp;
		while(ch>0)
		{
			m = ch-1; ch = 0;
			for(i=1; i<=m; i++)
			{
				if(lstrcmpiA(astrFavorites[i-1], astrFavorites[i])>0)
				{
					tmp = astrFavorites[i-1];
					astrFavorites[i-1] = astrFavorites[i];
					astrFavorites[i] = tmp;
					ntmp = anFavID[i-1];
					anFavID[i-1] = anFavID[i];
					anFavID[i] = ntmp;
					ch = i;
				}
			}
		}	
		
		
		for(i = 0; i<astrDirs.GetSize(); i++)
		{
			if(astrDirs[i].IsEmpty())
				continue;

			mff[0]='\0';
			GetPrivateProfileString("MyIE", "MostFav", "0", mff, 2, strPath2+astrDirs[i]+"\\"+"myiefolder.mic");
			if(mff[0]=='1')
				hnode = m_wndFav.m_FavTreeCtrl.InsertItem(astrDirs[i], 5, 6, hItem,hLastDir);
			else
				hnode = m_wndFav.m_FavTreeCtrl.InsertItem(astrDirs[i], 0, 2, hItem,hLastDir);

			m_wndFav.m_FavTreeCtrl.SetItemData(hnode, -1);

//3.01			if((astrDirs[i]==strcha || astrDirs[i]==strcha2))//nNewEndPos == nEndPos &&
//				m_wndFav.m_FavTreeCtrl.DeleteItem(hnode);
//			else
			{
			//	hLastDir = hnode;
				if(strPath2+astrDirs[i]+"\\" == ((CMainFrame*)pMainFrame)->m_strStartFavFolder)
				{
					m_wndFav.m_FavTreeCtrl.m_hStartFolder = hnode;
					m_wndFav.m_FavTreeCtrl.SetItemState(hnode,TVIS_BOLD,TVIS_BOLD);
				}
			}
		}
		
		
		for(i = 0; i<astrFavorites.GetSize(); i++)
		{
			if(astrFavorites[i].IsEmpty())
				continue;

			str = astrFavorites[i];
			str.MakeLower();
			buf[0]='\0';
			if(str.Right(4)==".url")
				::GetPrivateProfileString(_T("MyIE"), _T("MostFav"),
						  _T(""), buf, 2048,
						  strPath2 + str);
			str = astrFavorites[i].Left(str.GetLength()-4);
			if(buf[0]!='1' && !bBest)
				hnode = m_wndFav.m_FavTreeCtrl.InsertItem(str, 1, 1, hItem,TVI_LAST);
			else if(buf[0]=='1')
				hnode = m_wndFav.m_FavTreeCtrl.InsertItem(str, 4, 4, hItem,TVI_LAST);
			m_wndFav.m_FavTreeCtrl.SetItemData(hnode, anFavID[i]);
		}
		


	}
	return nEndPos;

}

/*
BOOL CFavTree::ResolveLink(LPCSTR lpszLinkFile, LPSTR lpszPath)
{

    HRESULT hres; 
    IShellLink* psl; 
    char szGotPath[MAX_PATH]; 
    char szDescription[MAX_PATH]; 
    WIN32_FIND_DATA wfd; 
 
    *lpszPath = 0; // assume failure 
 
    // Get a pointer to the IShellLink interface. 
    hres = CoCreateInstance(CLSID_ShellLink, NULL, 
        CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID *) &psl); 
    if (SUCCEEDED(hres)) { 
        IPersistFile* ppf; 
        // Get a pointer to the IPersistFile interface. 
        hres = psl->QueryInterface(IID_IPersistFile, 
            (void**)&ppf); 
        if (SUCCEEDED(hres)) { 
            WCHAR wsz[MAX_PATH]; 
 
            // Ensure that the string is Unicode. 
            MultiByteToWideChar(CP_ACP, 0, lpszLinkFile, -1, wsz, 
                MAX_PATH); 

            // Load the shortcut. 
            hres = ppf->Load(wsz, STGM_READ); 
            if (SUCCEEDED(hres)) { 
 
               // Resolve the link. 
                hres = psl->Resolve(this->m_wndFav.m_hWnd, SLR_ANY_MATCH); 
                if (SUCCEEDED(hres)) { 
 
                    // Get the path to the link target. 
                    hres = psl->GetPath(szGotPath, 
                        MAX_PATH, (WIN32_FIND_DATA *)&wfd, 
                        SLGP_SHORTPATH ); 
                    //if (FAILED(hres)) 
                        //HandleErr(hres); // application-defined function 

                    // Get the description of the target. 
                    hres = psl->GetDescription(szDescription, MAX_PATH); 
                    //if (FAILED(hres)) 
                       // HandleErr(hres); 
                    lstrcpy(lpszPath, szGotPath); 
                } 
            } 
        // Release the pointer to the IPersistFile interface. 
        ppf->Release(); 
        } 
    // Release the pointer to the IShellLink interface. 
    psl->Release(); 
    } 
    return SUCCEEDED(hres); 

}
*/

//#pragma optimize( "s", off)

