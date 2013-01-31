// MyIEHelper.cpp: implementation of the CMyIEHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyIE.h"
#include "MyIEHelper.h"
#include "MyIEView.h"
#include "ChildFrm.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



CMyIEView * CreateNewView()
{
    CWinApp* pApp = AfxGetApp();

    // Get the correct document template
/*    CDocTemplate* pDocTemplate;
    POSITION pos = pApp->GetFirstDocTemplatePosition();
    pDocTemplate = pApp->GetNextDocTemplate(pos);

    ASSERT(pDocTemplate);

	//Create new document
	CDocument* pDoc = pDocTemplate->CreateNewDocument();
    // Create the new frame
    CFrameWnd* pNewFrame = pDocTemplate->CreateNewFrame(pDoc,
                                   (CFrameWnd*)AfxGetMainWnd());
    ASSERT(pNewFrame);

    // Activate the frame and set its active view
    pDocTemplate->InitialUpdateFrame(pNewFrame, NULL);

    CFixedHtmlView* pWBVw = (CFixedHtmlView*)pNewFrame->GetActiveView();
    ASSERT(pWBVw);
*/
	CChildFrame *pWnd;
	pWnd = ((CMainFrame*)pApp->m_pMainWnd)->NewChildWindow(0);

	return pWnd->m_pView;
}

CMyIEView* GetCurrentView(int nFlag, BOOL bInNewWindow)
{
	short ks=GetKeyState(VK_CONTROL);
	CChildFrame* tcf = (CChildFrame*)((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	if(tcf!=NULL && !bInNewWindow && ks>=0)
	{
		return (CMyIEView*)tcf->m_pView;
	}
	else
	{
		if(nFlag == 1 || bInNewWindow || ks<0) //create new 
			return CreateNewView();
		return NULL;
	}
}

/*LPDISPATCH GetCurrentIDSP(int nFlag)
{
	short ks=GetKeyState(VK_CONTROL);
	CChildFrame* tcf = (CChildFrame*)((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	if(tcf!=NULL && tcf->m_pView!=NULL && !((CMainFrame*)pMainFrame)->m_bAlwaysNewWindow && ks>=0)
	{
		return ((CMyIEView*)tcf->m_pView)->GetApplication();
	}
	else
	{
		if(nFlag == 1 || ((CMainFrame*)pMainFrame)->m_bAlwaysNewWindow || ks<0) //create new
			return CreateNewView()->GetApplication();
		return NULL;
	}
}
*/


