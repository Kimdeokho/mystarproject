#pragma once



// CMyProSheet
#include "Include.h"
#include "MyProPage.h"
#include "MyProPage2.h"

class CMyProSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMyProSheet)

public:
	CMyProSheet(CWnd* pParentWnd);
	CMyProSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CMyProSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CMyProSheet();
public:
	CMyProPage	m_page1;
	CMyProPage2	m_page2;
public:
	BOOL SetPageTitle (int nPage, LPTSTR pszText);  
	void AddString(const TCHAR* _wstr , int _order);
	void SetCursel(int page, int iidx);
	bool FindStr(const TCHAR* _str , int _idx);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
};


