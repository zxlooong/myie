// MyIEHelper.h: interface for the CMyIEHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYIEHELPER_H__74460E23_1598_11D4_9BCE_0000E85300AE__INCLUDED_)
#define AFX_MYIEHELPER_H__74460E23_1598_11D4_9BCE_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyIEView.h"

//function used for Exp obj to get the views
//LPDISPATCH GetCurrentIDSP(int nFlag = 0);
CMyIEView* GetCurrentView(int nFlag = 0, BOOL bInNewWindow = FALSE);
CMyIEView * CreateNewView();

#endif // !defined(AFX_MYIEHELPER_H__74460E23_1598_11D4_9BCE_0000E85300AE__INCLUDED_)
