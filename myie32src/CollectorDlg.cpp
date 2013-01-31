// CollectorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "myie.h"
#include "CollectorDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCollectorDlg dialog

//#pragma optimize( "s", on)

CCollectorDlg::CCollectorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCollectorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCollectorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCollectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCollectorDlg)
	DDX_Control(pDX, IDC_TXT, m_Editor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCollectorDlg, CDialog)
	//{{AFX_MSG_MAP(CCollectorDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCollectorDlg message handlers





void CCollectorDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	SaveCollector();

	CDialog::OnClose();
}

void CCollectorDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	SaveCollector();
}


void CCollectorDlg::SaveCollector()
{
	//colecter window position
	CRect wrect;
	CWinApp* app=AfxGetApp();
	GetWindowRect(&wrect);
	app->WriteProfileInt("Settings", "collecttop", wrect.top);
	app->WriteProfileInt("Settings", "collectleft", wrect.left);
	app->WriteProfileInt("Settings", "collectwidth", wrect.Width());
	app->WriteProfileInt("Settings", "collectheight", wrect.Height());

	CString txt;
	m_Editor.GetWindowText(txt);
	if(txt.IsEmpty())
		return;

	CFile f;
	if(f.Open(((CMyIEApp*)app)->m_strSaveTxtFile, CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
	{
		f.Write((void*)(LPCSTR)txt, txt.GetLength());
		f.Close();
	}
}


void CCollectorDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_Editor.SetWindowPos(&CWnd::wndTop, 0,0, cx,cy, 0);
}


/*************************************************************
CTxtDropTarget
**************************************************************/
/*BOOL CTxtDropTarget::Register(CComboBoxEx* pComboBox)
{
    ms_pCtl=pComboBox;
    return COleDropTarget::Register(pComboBox);
}*/

DROPEFFECT CTxtDropTarget::OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point)
{

 //      if (dwKeyState & MK_CONTROL)
            return DROPEFFECT_SCROLL | DROPEFFECT_COPY | DROPEFFECT_LINK;
 //       else
 //           return DROPEFFECT_SCROLL | DROPEFFECT_MOVE;
}

DROPEFFECT CTxtDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, 
                                      DWORD dwKeyState, CPoint point)
{
//    if (!pDataObject->IsDataAvailable(CF_TEXT)) 
 //       return DROPEFFECT_NONE;

    DROPEFFECT dwEffect;
	dwEffect=DROPEFFECT_COPY;
    
//    if (dwKeyState & MK_CONTROL)
 //       dwEffect=DROPEFFECT_COPY;
 //   else
 //       dwEffect=DROPEFFECT_MOVE;

    //we set focus to current window such that the caret will be shown
 //   ms_pCtl->SetFocus();

    return dwEffect;
}

void CTxtDropTarget::OnDragLeave(CWnd* pWnd)
{
}

DROPEFFECT CTxtDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, 
                                     DWORD dwKeyState, CPoint point)
{

    DROPEFFECT dwEffect;
    
	dwEffect=DROPEFFECT_COPY;

//    if (dwKeyState & MK_CONTROL)
  //      dwEffect=DROPEFFECT_COPY;
 //   else
 //       dwEffect=DROPEFFECT_MOVE;

 //   ms_pCtl->DrawCaretByCursor();
    return dwEffect;
}

BOOL CTxtDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
                           DROPEFFECT dropEffect, CPoint point)
{

	try{

    HGLOBAL hData=pDataObject->GetGlobalData(CF_TEXT);
    if (hData)
	{

	/*	pDataObject->BeginEnumFormats();
		FORMATETC fc;
		char tmp[20];
		while(pDataObject->GetNextFormat(&fc))
		{
			if(pDataObject->IsDataAvailable(fc.cfFormat))
			{
				GetClipboardFormatName(fc.cfFormat, tmp, 20);
				AfxMessageBox(tmp);
				itoa(fc.cfFormat, tmp, 10);
				AfxMessageBox(tmp);
			}
		}*/

		LPCSTR lpcszData=(LPCSTR)GlobalLock(hData);
	//	ms_pCtl->SetWindowText(lpcszData);
	//	SendMessage(pMainFrame->GetSafeHwnd(), WM_COMMAND, IDOK, 0);

		CString str2(lpcszData);
		GlobalUnlock(hData);

		//if it is a link?
		HGLOBAL hLinkData=pDataObject->GetGlobalData(RegisterClipboardFormat("FileGroupDescriptor"));
		LPCSTR lpcszLink;
		if(hLinkData)
		{
		   lpcszLink=((LPCSTR)GlobalLock(hLinkData)) + 76;
		   str2 += "\r\n";
		   str2 += lpcszLink;
		   str2 = str2.Left(str2.GetLength()-4);
	/*	int si = GlobalSize(hLinkData);
		CFile f;
		f.Open("d:\\tmp.txt", CFile::modeCreate|CFile::modeWrite);
		f.Write(lpcszLink-76, si);
		f.Close();*/
		   GlobalUnlock(hLinkData);
		}




		str2+="\r\n\r\n";
		if(((CMainFrame*)pMainFrame)->pCollectDlg)
		{
			int len = ((CMainFrame*)pMainFrame)->pCollectDlg->m_Editor.GetWindowTextLength( );
			((CMainFrame*)pMainFrame)->pCollectDlg->m_Editor.SetSel(len, len);
			((CMainFrame*)pMainFrame)->pCollectDlg->m_Editor.ReplaceSel(str2);
		}
		else
		{
			CFile f;
			if(f.Open(((CMyIEApp*)AfxGetApp())->m_strSaveTxtFile, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::shareDenyNone))
			{
				f.SeekToEnd();
				f.Write((void*)(LPCSTR)str2, str2.GetLength());
				f.Close();
			}
		}
	}
	else
	{
		hData=pDataObject->GetGlobalData(CF_HDROP);
		if (!hData) {
			TRACE("Fail in getting data\n");
			return FALSE;
		}

		HDROP hdrop =(HDROP)GlobalLock(hData);

		DragQueryFile(hdrop, 0, lpfiles, 1024);
		GlobalUnlock(hData);

		CString filename, rfile;
		filename = lpfiles;
		int l = filename.ReverseFind('\\');
		filename = filename.Mid(l+1);
		//fix filename, remove [x]
		l = filename.ReverseFind('[');
		int i = filename.ReverseFind(']');
		CString ext = filename.Mid(i+1);
		filename = filename.Left(l);
		filename = ((CMyIEApp*)AfxGetApp())->m_strImagePath + filename;
//		if(((CMyIEApp*)AfxGetApp())->m_bNeedCreateDir)
		{
			CreateDirectory(((CMyIEApp*)AfxGetApp())->m_strImagePath, NULL);
//			((CMyIEApp*)AfxGetApp())->m_bNeedCreateDir = FALSE;
		}
		
		//compare file size
		DWORD sz1, sz2=0;
		HANDLE hfile = INVALID_HANDLE_VALUE;
		hfile = CreateFile(lpfiles, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hfile != INVALID_HANDLE_VALUE)
			sz1 = GetFileSize(hfile, NULL);
		CloseHandle(hfile);
		hfile = INVALID_HANDLE_VALUE;
		hfile = CreateFile(filename+ext, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hfile != INVALID_HANDLE_VALUE)
			sz2 = GetFileSize(hfile, NULL);
		CloseHandle(hfile);

		rfile = filename + ext;
		if(sz1!=sz2 && sz2!=0)
		{
			char tmp[4];
			l = 0;
			HFILE hf; OFSTRUCT of;
			hf = OpenFile(rfile, &of, OF_EXIST);
			while(hf!=HFILE_ERROR)
			{
				l++;
				rfile = filename;
				rfile += '[';
				itoa(l, tmp, 10);
				rfile += tmp;
				rfile += ']';
				rfile += ext;
				hf = OpenFile(rfile, &of, OF_EXIST);
			}
		}

		if(!CopyFile(lpfiles, rfile, FALSE))
		{
			DWORD dwError = GetLastError();
			if(dwError == ERROR_PATH_NOT_FOUND)
				AfxMessageBox(IDS_SAVEIMAGEFAIL);
		}

	}

	}catch(...)
	{
	}

    return TRUE;
}


//#pragma optimize( "s", off)

