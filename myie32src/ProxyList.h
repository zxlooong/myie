// ProxyCategories.h: interface for the CProxyCategories class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROXYCATEGORIES_H__04AF675F_3E8C_4216_8158_4B138DE19CBC__INCLUDED_)
#define AFX_PROXYCATEGORIES_H__04AF675F_3E8C_4216_8158_4B138DE19CBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CProxyItem
{
public:
	int index;
	CProxyItem();
	virtual ~CProxyItem();
	CString m_strProxyName;
	CString m_strProxy;
	int m_nProxySpeed;
};

class CProxyList
{
public:
	int FindIndex(int index);
	void Add(LPCSTR proxy, LPCSTR name, int speed);
	int Find(LPCSTR proxy);
	CProxyList();
	virtual ~CProxyList();
	CArray<CProxyItem*, CProxyItem*> m_Proxies;
	CString m_strProxyByPass;
	CString m_strCategoryName;
	BOOL m_bIsWebProxy, m_bIsTransProxy;
};

class CProxyCategory
{
public:
	void Copy(CProxyCategory& pc);
	CProxyItem* FindIndex(int index, CProxyList** plist, int* item);
	void SaveProxies();
	void LoadProxies();
	CProxyList* GetWebProxyList(BOOL bTrans = FALSE);
	void RemoveAll();
	CProxyCategory();
	CProxyCategory(CProxyCategory& pc);
	virtual ~CProxyCategory();
	CArray<CProxyList*, CProxyList*> m_ProxyCategory;
};

class CDownloadTool
{
public:
	CString DName;
	CString ProgID;
};

class CDownloadToolList
{
public:
	BOOL LoadDownloadTools();
	int GetIDFromName(LPCSTR name);
	CDownloadToolList();
	virtual ~CDownloadToolList();
	CArray<CDownloadTool*, CDownloadTool*> m_DownloadTools;
};

#endif // !defined(AFX_PROXYCATEGORIES_H__04AF675F_3E8C_4216_8158_4B138DE19CBC__INCLUDED_)
