#if !defined(AFX_ADVTABCTRL_H__AD8C3BA1_8844_11D5_9BB3_E13549EA421E__INCLUDED_)
#define AFX_ADVTABCTRL_H__AD8C3BA1_8844_11D5_9BB3_E13549EA421E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdvTabCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdvTabCtrl window

class CAdvTabCtrl : public CTabCtrl
{
// Construction
public:
	CAdvTabCtrl();

// Attributes
public:
	BOOL m_bTop;
	CImageList* pImageList; 

// Operations
public:
	void SetFonts(CFont* pSelFont, CFont* pUnselFont);
	void SetFonts(int nSelWeight=FW_SEMIBOLD, BOOL bSelItalic=FALSE,   BOOL bSelUnderline=FALSE,
				  int nUnselWeight=FW_MEDIUM, BOOL bUnselItalic=FALSE, BOOL bUnselUnderline=FALSE);

	void SetColours(COLORREF bSelColour, COLORREF bUnselColour);
	void SetBKColours(COLORREF bSelBKColour, COLORREF bUnselBKColour);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdvTabCtrl)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_bDragging;
	virtual ~CAdvTabCtrl();

// Implementation
protected:
	COLORREF m_crSelColour, m_crUnselColour;
	COLORREF m_crSelBKColour, m_crUnselBKColour;
	CFont    m_SelFont,	m_UnselFont;

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdvTabCtrl)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
//	int m_id;
	void ProcessClick(UINT nFlags, CPoint point, int nClickButton);
	int GetTabIDFromPoint(CPoint & point);
	int m_nDragTabID;
	HCURSOR m_hOldCursor;
	HCURSOR m_hDragCursor;
	TCITEM TabCtrlItem;
	char pszText[256];
	BOOL m_bSel;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADVTABCTRL_H__AD8C3BA1_8844_11D5_9BB3_E13549EA421E__INCLUDED_)
