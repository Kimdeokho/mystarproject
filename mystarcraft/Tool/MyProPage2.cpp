// MyProPage2.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyProPage2.h"
#include "TileMgr.h"
#include "TileDebug.h"

// CMyProPage2 ��ȭ �����Դϴ�.

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


// CMyProPage2 �޽��� ó�����Դϴ�.

BOOL CMyProPage2::OnSetActive()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	CTileMgr::GetInstance()->SetRohmbusRender(false);
	CTileDebug::GetInstance()->SetDebugGroup(DBG_HILL , false);
	return CPropertyPage::OnSetActive();
}

void CMyProPage2::OnLbnSelchangeList1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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
