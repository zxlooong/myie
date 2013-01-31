#if !defined(AFX_CHEVBAR_H__ADA7D7F6_E2C1_11D3_84C7_0080450EA020__INCLUDED_)
#define AFX_CHEVBAR_H__ADA7D7F6_E2C1_11D3_84C7_0080450EA020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//all AfxGetComCtlVersion && AfxChildWindowFromPoint are removed in version 2.2

//DWORD AFXAPI _AfxGetComCtlVersion();
//HWND AFXAPI _AfxChildWindowFromPoint(HWND, POINT);
// for determining version of COMCTL32.DLL
#define VERSION_WIN4    MAKELONG(0, 4)
#define VERSION_IE3     MAKELONG(70, 4)
#define VERSION_IE4     MAKELONG(71, 4)
#define VERSION_IE401   MAKELONG(72, 4)
//extern int _afxComCtlVersion;
//DWORD AFXAPI _AfxGetComCtlVersion();

#define AfxDeferRegisterClass(fClass) AfxEndDeferRegisterClass(fClass)

BOOL AFXAPI AfxEndDeferRegisterClass(LONG fToRegister);

#define AFX_WNDCOMMCTL_COOL_REG         0x08000

#define RBBS_USECHEVRON				0x00000200 
#define RBN_CHEVRONPUSHED			(RBN_FIRST - 10)
typedef struct tagNMREBARCHEVRON
{
    NMHDR hdr;
    UINT uBand;
    UINT wID;
    LPARAM lParam;
    RECT rc;
    LPARAM lParamNM;

} NMREBARCHEVRON, *LPNMREBARCHEVRON;

//add by shu
extern	CStringArray m_astrLinks;
extern  CStringArray m_astrFavs;

class CReBarCtrl;
class CChevBar : public CControlBar
{
	DECLARE_DYNAMIC(CChevBar)

// Construction
public:
	CChevBar();
	BOOL Create(CWnd* pParentWnd, DWORD dwCtrlStyle = RBS_BANDBORDERS,
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_TOP,
		UINT nID = AFX_IDW_REBAR);

// Attributes
public:
	// for direct access to the underlying common control
 	CReBarCtrl& GetReBarCtrl() const
		{ return *(CReBarCtrl*)this; }

// Operations
public:
	HMENU CopyMenu(HMENU hMenu);
	BOOL AddBar(CWnd* pBar, LPCTSTR pszText = NULL, CBitmap* pbmp = NULL,
		DWORD dwStyle = RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP);
	BOOL AddBar(CWnd* pBar, COLORREF clrFore, COLORREF clrBack,
		LPCTSTR pszText = NULL, DWORD dwStyle = RBBS_GRIPPERALWAYS);

// Implementation
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
//	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual CSize CalcDynamicLayout(int nLength, DWORD nMode);
#ifdef _DEBUG
//2.3	void EnableDocking(DWORD dwDockStyle);
#endif

protected:
	CString strText, str, str1, strCurWebProxy;

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	BOOL _AddBar(CWnd* pBar, REBARBANDINFO* pRBBI);

	void OnChevronPushed( NMHDR * pNotifyStruct, LRESULT* result );
	//{{AFX_MSG(CReBar)
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT);
	afx_msg void OnPaint();
	afx_msg void OnHeightChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNcPaint();
	afx_msg void OnNcCalcSize(BOOL, NCCALCSIZE_PARAMS*);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnShowBand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRecalcParent();
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_CHEVBAR_H__ADA7D7F6_E2C1_11D3_84C7_0080450EA020__INCLUDED_)
