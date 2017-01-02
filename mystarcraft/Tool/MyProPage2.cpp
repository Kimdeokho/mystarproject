// MyProPage2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyProPage2.h"
#include "TileMgr.h"
#include "TileDebug.h"

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
	DDX_Control(pDX, IDC_LIST2, m_objListBox);
}


BEGIN_MESSAGE_MAP(CMyProPage2, CPropertyPage)
	ON_LBN_SELCHANGE(IDC_LIST2, &CMyProPage2::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CMyProPage2 메시지 처리기입니다.

BOOL CMyProPage2::OnSetActive()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CTileMgr::GetInstance()->SetRohmbusRender(false);
	CTileDebug::GetInstance()->SetDebugGroup(DBG_HILL , false);
	return CPropertyPage::OnSetActive();
}

void CMyProPage2::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int idx = m_objListBox.GetCurSel();
	CString str;
	m_objListBox.GetText(idx, str);

	CTileDebug::GetInstance()->SetDebug_Allinit();

	if(!str.Compare(L"Gas"))
	{
		CTileDebug::GetInstance()->SetDebugGroup(DBG_GAS , true);
	}
	else if(!str.Compare(L"Mineral"))
	{
		CTileDebug::GetInstance()->SetDebugGroup(DBG_MINERAL , true);
	}
	else if(!str.Compare(L"Startbase"))
	{
		CTileDebug::GetInstance()->SetDebugGroup(DBG_STARTBASE , true);
	}
}
