// MyProSheet.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyProSheet.h"


// CMyProSheet

IMPLEMENT_DYNAMIC(CMyProSheet, CPropertySheet)

CMyProSheet::CMyProSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CMyProSheet::CMyProSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

}

CMyProSheet::CMyProSheet(CWnd* pParentWnd)
{
	AddPage(&m_page1);
}

CMyProSheet::~CMyProSheet()
{
}


BEGIN_MESSAGE_MAP(CMyProSheet, CPropertySheet)
END_MESSAGE_MAP()


// CMyProSheet �޽��� ó�����Դϴ�.
