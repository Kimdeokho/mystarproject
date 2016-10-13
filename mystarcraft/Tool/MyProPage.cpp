// MyProPage.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyProPage.h"
#include "TileMgr.h"
#include "TerrainBrushMgr.h"
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
	DDX_Control(pDX, IDC_LIST1, m_maptileListbox);
}


BEGIN_MESSAGE_MAP(CMyProPage, CPropertyPage)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_ACTIVATE()
	ON_WM_ENABLE()
	ON_LBN_SELCHANGE(IDC_LIST1, &CMyProPage::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CMyProPage �޽��� ó�����Դϴ�.

BOOL CMyProPage::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return CPropertyPage::PreCreateWindow(cs);
}

void CMyProPage::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

BOOL CMyProPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CMyProPage::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CPropertyPage::OnGetMinMaxInfo(lpMMI);
}

void CMyProPage::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CPropertyPage::OnActivate(nState, pWndOther, bMinimized);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

BOOL CMyProPage::OnSetActive()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	//���̾�α��� Ȱ��ȭ ���θ� �Ǵ��Ѵ�

	CTileMgr::GetInstance()->SetRohmbusRender(TRUE);
	return CPropertyPage::OnSetActive();
}

void CMyProPage::OnEnable(BOOL bEnable)
{
	CPropertyPage::OnEnable(bEnable);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

void CMyProPage::OnLbnSelchangeList1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	int idx = m_maptileListbox.GetCurSel();

	CTerrainBrushMgr::GetInstance()->SetTerrain_ID(idx);
}
