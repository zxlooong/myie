// CustomizeToolbar.cpp : implementation file
//

#include "stdafx.h"
#include "myie.h"
#include "CustomizeToolbar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// CCustomizeToolbar dialog

extern  UINT toolbarbuttons[];
extern  UINT toolbarlabels[];
extern  UINT toolbarbuttonimgs[];


CCustomizeToolbar::CCustomizeToolbar(CWnd* pParent /*=NULL*/)
	: CDialog(CCustomizeToolbar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCustomizeToolbar)
	m_bColorIcon = FALSE;
	m_nTextOpt = -1;
	m_bSmallToolBar = -1;
	//}}AFX_DATA_INIT
}


void CCustomizeToolbar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomizeToolbar)
	DDX_Control(pDX, IDC_CURBTN, m_curbtn);
	DDX_Control(pDX, IDC_AVABTN, m_avabtn);
	DDX_Check(pDX, IDC_COLOR_ICON, m_bColorIcon);
	DDX_CBIndex(pDX, IDC_TEXT_OPT, m_nTextOpt);
	DDX_CBIndex(pDX, IDC_ICON_OPT, m_bSmallToolBar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCustomizeToolbar, CDialog)
	//{{AFX_MSG_MAP(CCustomizeToolbar)
	ON_BN_CLICKED(IDC_MOVWDOWN, OnMovwdown)
	ON_BN_CLICKED(IDC_MOVEUP, OnMoveup)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_NOTIFY(NM_DBLCLK, IDC_AVABTN, OnDblclkAvabtn)
	ON_NOTIFY(NM_DBLCLK, IDC_CURBTN, OnDblclkCurbtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomizeToolbar message handlers

BOOL CCustomizeToolbar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int nDefBtn = 44, nMaxBtn = nDefBtn;
	int n=0;
	// TODO: Add extra initialization here
	CImageList img;
//	CBitmap bitmap;
/*	img.Create(IDB_COLDTOOLBAR, 21, 0, RGB(255, 0, 255));
	m_avabtn.SetImageList(&img, LVSIL_SMALL);
	img.Detach();
*/
//	bitmap.LoadBitmap(IDB_COLDTOOLBAR);
	img.Create(21, 21, ILC_COLORDDB|ILC_MASK, 40, 1);
//	HBITMAP hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_COLDTOOLBAR));
	HBITMAP	hbmp = (HBITMAP)LoadImage(NULL, ((CMyIEApp*)AfxGetApp())->m_strMyIERoot+"\\coldtool.bmp", IMAGE_BITMAP, 0, 0, 
						LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE); 
//	if(hbmp==NULL)
//		hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_COLDTOOLBAR));
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));

	m_avabtn.SetImageList(&img, LVSIL_SMALL);
	img.Detach();
//	bitmap.Detach();

	str.LoadString(IDS_SEPARATOR);
	CRect rect;
	m_avabtn.InsertColumn(0,"");
	m_avabtn.GetWindowRect(rect);
	m_avabtn.SetColumnWidth(0, rect.Width()-10);

	m_avabtn.InsertItem(0, str, -1);
	m_avabtn.SetItemData(0, 1);
	n=1;
	for(int i=0; i<nDefBtn; i++)
	{
		if(toolbarbuttons[i]!=0)
		{
			str.LoadString(toolbarlabels[i]);
			m_avabtn.InsertItem(n, str.Right(str.GetLength()-str.Find('\n')-1), toolbarbuttonimgs[i]);
			m_avabtn.SetItemData(n, i);
			n++;
		}
	}
	
	
/*	img.Create(IDB_COLDTOOLBAR, 21, 0, RGB(255, 0, 255));
	m_curbtn.SetImageList(&img, LVSIL_SMALL);
	img.Detach();
*/
//	bitmap.LoadBitmap(IDB_COLDTOOLBAR);
	img.Create(21, 21, ILC_COLORDDB|ILC_MASK, 40, 1);
//	hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_COLDTOOLBAR));
	hbmp = (HBITMAP)LoadImage(NULL, ((CMyIEApp*)AfxGetApp())->m_strMyIERoot+"\\hottoolb.bmp", IMAGE_BITMAP, 0, 0, 
					LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE); 
//	if(hbmp==NULL)
//		hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_COLDTOOLBAR));
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));

	m_curbtn.SetImageList(&img, LVSIL_SMALL);
	img.Detach();
//	bitmap.Detach();

	m_curbtn.InsertColumn(0,"");
	m_curbtn.GetWindowRect(rect);
	m_curbtn.SetColumnWidth(0, rect.Width()-12);

	int ids[50], l=0, id, buttonum;
	char tmp[6]; tmp[0]='B'; 

	id = (int)AfxGetApp()->GetProfileInt("ToolBar", "B0", -5);
	if(id == -5)
	{
		//no toolbar configuration
		buttonum = nDefBtn;
	}
	else
		buttonum = 50;
	id=0;l=0;
	while(l<buttonum && id!=-1)
	{
		itoa(l,tmp+1,10);
		id = (int)AfxGetApp()->GetProfileInt("ToolBar", tmp, -5);
		if(id==-5)
		{
			ids[l] = l;
			buttonum=nDefBtn;
		}
		else
			ids[l]=id;

		l++;
	}
	LVFINDINFO info;
	int index;
	n=0;
	for(i=0; i<l; i++)
	{
		itoa(i,tmp+1,10);
		id = ids[i];
		if(id>=0 && id<nMaxBtn)
		{
			if(toolbarlabels[id]>0)
			{
				str.LoadString(toolbarlabels[id]);
				m_curbtn.InsertItem(n, str.Right(str.GetLength()-str.Find('\n')-1), toolbarbuttonimgs[id]);
				m_curbtn.SetItemData(n, id);
				//delete from available list
				info.flags = LVFI_PARAM;
				info.lParam = id;
				index=m_avabtn.FindItem(&info);
				if(index != -1)
				{
					m_avabtn.DeleteItem(index);
				}
			}
			else
			{
				str.LoadString(IDS_SEPARATOR);
				m_curbtn.InsertItem(n, str, -1);
				m_curbtn.SetItemData(n, 1);
			}
			n++;
		}
		else if(id>=nMaxBtn)
			continue;
		else
			break;
	}
	m_curbtn.InsertItem(n, "                 ", -1);
	m_curbtn.SetItemData(n, -1);
	m_curbtn.SetItemState(n, LVIS_SELECTED, LVIS_SELECTED);
	m_curbtn.EnsureVisible(n, TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCustomizeToolbar::OnOK() 
{
	// TODO: Add extra validation here
	int id;
	char tmp[6]; tmp[0]='B'; 

	id=0;
	int n = m_curbtn.GetItemCount();
	for( int i=0; i<n; i++)
	{
		itoa(i,tmp+1,10);
		id = m_curbtn.GetItemData(i);
		AfxGetApp()->WriteProfileInt("ToolBar", tmp, id);
	}

	CDialog::OnOK();
}

void CCustomizeToolbar::OnMovwdown() 
{
	// TODO: Add your control notification handler code here
//	int j;
//	CString str;
	POSITION pos = m_curbtn.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int i = m_curbtn.GetNextSelectedItem(pos);
		int id;
		id = m_curbtn.GetItemData(i);
		if(id>=0 && i<m_curbtn.GetItemCount()-2)
		{
			m_curbtn.DeleteItem(i);
			str.LoadString(toolbarlabels[id]);
			m_curbtn.InsertItem(i+1, str.Right(str.GetLength()-str.Find('\n')-1), toolbarbuttonimgs[id]);
			m_curbtn.SetItemData(i+1, id);
			m_curbtn.EnsureVisible(i+1, TRUE);
			m_curbtn.SetItemState(i+1, LVIS_SELECTED, LVIS_SELECTED);
			m_curbtn.SetFocus();
		}
	}		
}

void CCustomizeToolbar::OnMoveup() 
{
	// TODO: Add your control notification handler code here
//	int j;
//	CString str;
	POSITION pos = m_curbtn.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int i = m_curbtn.GetNextSelectedItem(pos);
		int id;
		id = m_curbtn.GetItemData(i);
		if(id>=0 && i>0)
		{
			m_curbtn.DeleteItem(i);
			str.LoadString(toolbarlabels[id]);
			m_curbtn.InsertItem(i-1, str.Right(str.GetLength()-str.Find('\n')-1), toolbarbuttonimgs[id]);
			m_curbtn.SetItemData(i-1, id);
			m_curbtn.EnsureVisible(i-1, TRUE);
			m_curbtn.SetItemState(i-1, LVIS_SELECTED, LVIS_SELECTED);
			m_curbtn.SetFocus();
		}
	}		
}

void CCustomizeToolbar::OnRemove() 
{
	// TODO: Add your control notification handler code here
	int j;
//	CString str;
	POSITION pos = m_curbtn.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int i = m_curbtn.GetNextSelectedItem(pos);
		int id;
		id = m_curbtn.GetItemData(i);
		if(id>=0)
		{
			j = m_avabtn.GetItemCount();
			if(id!=1) //not a separator
			{
				str.LoadString(toolbarlabels[id]);
				m_avabtn.InsertItem(j, str.Right(str.GetLength()-str.Find('\n')-1), toolbarbuttonimgs[id]);
				m_avabtn.SetItemData(j, id);
				m_curbtn.DeleteItem(i);
			}
			else
			{
				m_curbtn.DeleteItem(i);
			}
			m_curbtn.EnsureVisible(i, TRUE);
		}
	}	
}

void CCustomizeToolbar::OnAdd() 
{
	// TODO: Add your control notification handler code here
	int j;
//	CString str;
	POSITION pos = m_avabtn.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int i = m_avabtn.GetNextSelectedItem(pos);
		int id;
		id = m_avabtn.GetItemData(i);
		//get current selected item in curbtns
		POSITION pos1 = m_curbtn.GetFirstSelectedItemPosition();
		if(pos1!=NULL)
		{
			j = m_curbtn.GetNextSelectedItem(pos1);
		}
		else
			j = m_curbtn.GetItemCount()-1;

		if(i>0)
		{
			str.LoadString(toolbarlabels[id]);
			m_curbtn.InsertItem(j, str.Right(str.GetLength()-str.Find('\n')-1), toolbarbuttonimgs[id]);
			m_curbtn.SetItemData(j, id);
			m_avabtn.DeleteItem(i);
		}
		else
		{
			str.LoadString(IDS_SEPARATOR);
			m_curbtn.InsertItem(j, str, -1);
			m_curbtn.SetItemData(j, 1);
		}
		m_curbtn.EnsureVisible(j, TRUE);
	}	
}

void CCustomizeToolbar::OnDblclkAvabtn(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnAdd();
	*pResult = 0;
}

void CCustomizeToolbar::OnDblclkCurbtn(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnRemove();
	*pResult = 0;
}

//#pragma optimize( "s", off)

