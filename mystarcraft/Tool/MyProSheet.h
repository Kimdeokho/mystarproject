#pragma once



// CMyProSheet
#include "MyProPage.h"
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
protected:
	DECLARE_MESSAGE_MAP()
};


