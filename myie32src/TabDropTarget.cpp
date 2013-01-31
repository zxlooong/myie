// TabDropTarget.cpp : implementation file
//

#include "stdafx.h"
#include "myie.h"
#include "TabDropTarget.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#pragma optimize( "s", on)

/////////////////////////////////////////////////////////////////////////////
// CTabDropTarget

/*************************************************************
CTabDropTarget
**************************************************************/
BOOL CTabDropTarget::Register(CTabCtrl* pCtrl)
{
    ms_pCtl=pCtrl;
    return COleDropTarget::Register(pCtrl);
}

DROPEFFECT CTabDropTarget::OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point)
{

 //      if (dwKeyState & MK_CONTROL)
            return DROPEFFECT_COPY | DROPEFFECT_LINK;
 //       else
 //           return DROPEFFECT_SCROLL | DROPEFFECT_MOVE;
}

DROPEFFECT CTabDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, 
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

void CTabDropTarget::OnDragLeave(CWnd* pWnd)
{
}

DROPEFFECT CTabDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, 
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

BOOL CTabDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
                           DROPEFFECT dropEffect, CPoint point)
{

    HGLOBAL hData=pDataObject->GetGlobalData(CF_TEXT);
    if (!hData) {
        TRACE("Fail in getting data\n");
        return FALSE;
    }
    LPCSTR lpcszData=(LPCSTR)GlobalLock(hData);
//	ms_pCtl->SetWindowText(lpcszData);
//	SendMessage(pMainFrame->GetSafeHwnd(), WM_COMMAND, IDOK, 0);

	CString str2(lpcszData), url;

    GlobalUnlock(hData);

	//if the protocol is javascript then ignore
	if(str2.Left(11)=="javascript:")
		return FALSE;

	//check alias and 3721
	BOOL use3721 = FALSE;
	if(((CMainFrame*)pMainFrame)->m_bUseAlias)
		if(((CMainFrame*)pMainFrame)->m_mapAlias.Lookup(str2, url))
			str2 = url;
		else
			use3721=TRUE;
	else
		use3721=TRUE;


	if(((CMainFrame*)pMainFrame)->m_bUse3721 && use3721)
	{
		//use 3721
		if(str2.Find(':')<0 && str2.Find('.')<0)
		{
			//not a url
			url = str2;
			str2 = "http://cns.3721.com/cns.dll?platform=pc&fl=1&fd=1&name="+str2; // www.3721.com/***
		}
	}

	TCHITTESTINFO hti;
	hti.pt = point;
	hti.flags = TCHT_ONITEM | TCHT_ONITEMICON | TCHT_ONITEMLABEL ;
	int nTabID = ms_pCtl->HitTest(&hti);

	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;
	TabCtrlItem.lParam = NULL;

	ms_pCtl->GetItem(nTabID, &TabCtrlItem);
	if(TabCtrlItem.lParam != NULL)
	{
		CChildFrame* tcf = ((CChildFrame*)TabCtrlItem.lParam);
		tcf->m_pView->ToNavigate(str2,0,NULL);
	}
	else
	{
		CMyIEView* pvw = CreateNewView();
		if(pvw!=NULL)
		{
			pvw->ToNavigate(str2,0,NULL);
		}
	}

    return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CTabDropTarget commands

//#pragma optimize( "s", off)
