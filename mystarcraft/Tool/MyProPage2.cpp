// MyProPage2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyProPage2.h"


// CMyProPage2 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMyProPage2, CPropertyPage)

CMyProPage2::CMyProPage2()
	: CPropertyPage(CMyProPage2::IDD)
{

}

CMyProPage2::~CMyProPage2()
{
}

void CMyProPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMyProPage2, CPropertyPage)
END_MESSAGE_MAP()


// CMyProPage2 메시지 처리기입니다.
