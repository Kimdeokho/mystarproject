// MyProPage.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyProPage.h"


// CMyProPage ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMyProPage, CPropertyPage)

CMyProPage::CMyProPage()
	: CPropertyPage(CMyProPage::IDD)
{

}

CMyProPage::~CMyProPage()
{
}

void CMyProPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMyProPage, CPropertyPage)
END_MESSAGE_MAP()


// CMyProPage �޽��� ó�����Դϴ�.
