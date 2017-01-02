// MyProPage.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyProPage.h"
#include "TileMgr.h"
#include "TerrainBrushMgr.h"
#include "TileDebug.h"
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
	ON_BN_CLICKED(IDC_BUTTON1, &CMyProPage::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyProPage::OnBnClickedLoadBtn)
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

	CTileMgr::GetInstance()->SetRohmbusRender(true);
	CTileDebug::GetInstance()->SetDebugGroup(DBG_GAS , false);
	CTileDebug::GetInstance()->SetDebugGroup(DBG_MINERAL , false);

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
	CString str;
	m_maptileListbox.GetText(idx, str);

	CTerrainBrushMgr::GetInstance()->InitBrush(str);

	if(!str.Compare(L"HillR") || !str.Compare(L"HillL"))
	{
		if(!str.Compare(L"HillR"))
			CTileDebug::GetInstance()->SetLR(HILL_R);
		else if(!str.Compare(L"HillL"))
			CTileDebug::GetInstance()->SetLR(HILL_L);

		CTileDebug::GetInstance()->SetDebugGroup(DBG_HILL , true);
		CTileDebug::GetInstance()->SetDebugGroup(DBG_GROUP , false);
		CTileMgr::GetInstance()->SetRohmbusRender(false);
	}
	else
	{
		CTileDebug::GetInstance()->SetDebugGroup(DBG_HILL , false);
		CTileMgr::GetInstance()->SetRohmbusRender(true);
	}
}

void CMyProPage::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	//����

	
	// ���� ���� �޺��ڽ��� ����� Ȯ���� ���͸� �����Ѵ�. (*.*, *.txt)
	// Ȯ���� ������ ù��° �κ��� �޺��ڽ��� ��µ� ���ڿ��� ����ϸ�,
	// �ι�° �κ��� ���� ���̾�α׿� ��µ� ���ϵ��� Ȯ���ڸ� ����Ѵ�.
	// ���ϴ� Ȯ���� ���͸� �Ʒ��� ���� " | " �����ڸ� �̿��Ͽ� �������� ����� �� ������
	// ���ڿ��� ������ �ݵ�� �����ڰ� ��õǾ�� �Ѵ�.

	//WCHAR name_filter[] = L"All Files (*.*)|*.*|Text Files (*.txt)|*.txt|";

	// FALSE -> �����ȭ����
	// "txt" -> ����ڰ� Ȯ���� ���� ���ϸ� �Է������� �ڵ����� �߰��� Ȯ���ڸ��̴�.
	// ��, tipssoft ������ �Է��ϸ� tipssoft.txt��� �Է��ѰͰ� �����ϰ� �ϰ� ������ ����Ѵ�.
	// "*.txt" �����̸��� �Է��ϴ� ����Ʈ �ڽ��� ��µ� �⺻ ���ڿ��̴�. 
	// OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT -> ���� ��ȭ���ڿ� �߰������� ����� �Ӽ�
	// name_filter -> ���� ���� �޺��ڽ��� ����� ���������� ����ִ� �޸��� �ּ��̴�.

	//CFileDialog ins_dlg(FALSE, "txt", "*.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
	//	OFN_NOCHANGEDIR, name_filter, NULL);


	// ���� ���� �޺��ڽ��� ������ ���͵� �߿��� 2��° �׸�(*.txt)�� �����Ѵ�.
	//ins_dlg.m_ofn.nFilterIndex = 2;

	// ���̾�α׸� ����.
	//if(ins_dlg.DoModal() == IDOK){
	//	// ������ ������ �̸��� ����Ʈ �ڽ��� ����Ѵ�.
	//	SetDlgItemText(IDC_FILE_NAME_EDIT, ins_dlg.GetFileName());
	//	// ������ ������ �̸��� ������ ��θ� ����Ʈ �ڽ��� ����Ѵ�.
	//	SetDlgItemText(IDC_FILE_PATH_EDIT, ins_dlg.GetPathName());
	//}



	WCHAR name_filter[] = L"All Files (*.*)|*.*|";

	CFileDialog save_dlg(FALSE/*FALSE ���� , TRUE �ҷ�����*/, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
		OFN_NOCHANGEDIR, name_filter, NULL);

	// ���̾�α׸� ����.

	//HANDLE	hFile = CreateFile(L"Data.dat"
	//	, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS
	//	, FILE_ATTRIBUTE_NORMAL, NULL);

	if(save_dlg.DoModal() == IDOK)
	{
		/*���⿡ ���� ������ �Է�*/
		CString FilePath = save_dlg.GetPathName();

		HANDLE	hFile = CreateFile(FilePath
			, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS
			, FILE_ATTRIBUTE_NORMAL, NULL);

		CTileMgr::GetInstance()->SaveTile(hFile);

		CloseHandle(hFile);
	}
}

void CMyProPage::OnBnClickedLoadBtn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//�ҷ�����
	WCHAR name_filter[] = L"All Files (*.*)|*.*|";

	CFileDialog load_dlg(TRUE/*FALSE ���� , TRUE �ҷ�����*/, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
		OFN_NOCHANGEDIR, name_filter, NULL);

	// ���̾�α׸� ����.
	if(load_dlg.DoModal() == IDOK)
	{
		/*���⿡ ���� ������ �Է�*/
		CString FilePath = load_dlg.GetPathName();

		HANDLE hFile = CreateFile(FilePath
			, GENERIC_READ, 0, NULL, OPEN_EXISTING
			, FILE_ATTRIBUTE_NORMAL, NULL);

		CTileMgr::GetInstance()->LoadTile(hFile);
		CloseHandle(hFile);
	}
}
