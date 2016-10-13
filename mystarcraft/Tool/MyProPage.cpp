// MyProPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyProPage.h"
#include "TileMgr.h"
#include "TerrainBrushMgr.h"
// CMyProPage 대화 상자입니다.

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
	DDX_Control(pDX, IDC_LIST1, m_maptileListbox);
}


BEGIN_MESSAGE_MAP(CMyProPage, CPropertyPage)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_ACTIVATE()
	ON_WM_ENABLE()
	ON_LBN_SELCHANGE(IDC_LIST1, &CMyProPage::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CMyProPage 메시지 처리기입니다.

BOOL CMyProPage::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CPropertyPage::PreCreateWindow(cs);
}

void CMyProPage::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CMyProPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMyProPage::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CPropertyPage::OnGetMinMaxInfo(lpMMI);
}

void CMyProPage::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CPropertyPage::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CMyProPage::OnSetActive()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	//다이얼로그의 활성화 여부를 판단한다

	CTileMgr::GetInstance()->SetRohmbusRender(TRUE);
	return CPropertyPage::OnSetActive();
}

void CMyProPage::OnEnable(BOOL bEnable)
{
	CPropertyPage::OnEnable(bEnable);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CMyProPage::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int idx = m_maptileListbox.GetCurSel();

	CTerrainBrushMgr::GetInstance()->SetTerrain_ID(idx);
}
