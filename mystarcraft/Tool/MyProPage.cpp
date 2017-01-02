// MyProPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyProPage.h"
#include "TileMgr.h"
#include "TerrainBrushMgr.h"
#include "TileDebug.h"
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
	ON_BN_CLICKED(IDC_BUTTON1, &CMyProPage::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyProPage::OnBnClickedLoadBtn)
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

	CTileMgr::GetInstance()->SetRohmbusRender(true);
	CTileDebug::GetInstance()->SetDebugGroup(DBG_GAS , false);
	CTileDebug::GetInstance()->SetDebugGroup(DBG_MINERAL , false);

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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//저장

	
	// 파일 형식 콤보박스에 등록할 확장자 필터를 정의한다. (*.*, *.txt)
	// 확장자 필터의 첫번째 부분은 콤보박스에 출력될 문자열을 명시하며,
	// 두번째 부분은 파일 다이얼로그에 출력될 파일들의 확장자를 명시한다.
	// 원하는 확장자 필터를 아래와 같이 " | " 구분자를 이용하여 여러개를 등록할 수 있으며
	// 문자열의 끝에는 반드시 구분자가 명시되어야 한다.

	//WCHAR name_filter[] = L"All Files (*.*)|*.*|Text Files (*.txt)|*.txt|";

	// FALSE -> 저장대화상자
	// "txt" -> 사용자가 확장자 없이 파일명만 입력했을때 자동으로 추가될 확장자명이다.
	// 즉, tipssoft 까지만 입력하면 tipssoft.txt라고 입력한것과 동일하게 하고 싶을때 사용한다.
	// "*.txt" 파일이름을 입력하는 에디트 박스에 출력될 기본 문자열이다. 
	// OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT -> 파일 대화상자에 추가적으로 사용할 속성
	// name_filter -> 파일 형식 콤보박스에 등록할 필터정보를 담고있는 메모리의 주소이다.

	//CFileDialog ins_dlg(FALSE, "txt", "*.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
	//	OFN_NOCHANGEDIR, name_filter, NULL);


	// 파일 형식 콤보박스에 나열된 필터들 중에서 2번째 항목(*.txt)을 선택한다.
	//ins_dlg.m_ofn.nFilterIndex = 2;

	// 다이얼로그를 띄운다.
	//if(ins_dlg.DoModal() == IDOK){
	//	// 선택한 파일의 이름을 에디트 박스에 출력한다.
	//	SetDlgItemText(IDC_FILE_NAME_EDIT, ins_dlg.GetFileName());
	//	// 선택한 파일의 이름을 포함한 경로를 에디트 박스에 출력한다.
	//	SetDlgItemText(IDC_FILE_PATH_EDIT, ins_dlg.GetPathName());
	//}



	WCHAR name_filter[] = L"All Files (*.*)|*.*|";

	CFileDialog save_dlg(FALSE/*FALSE 저장 , TRUE 불러오기*/, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
		OFN_NOCHANGEDIR, name_filter, NULL);

	// 다이얼로그를 띄운다.

	//HANDLE	hFile = CreateFile(L"Data.dat"
	//	, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS
	//	, FILE_ATTRIBUTE_NORMAL, NULL);

	if(save_dlg.DoModal() == IDOK)
	{
		/*여기에 파일 데이터 입력*/
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//불러오기
	WCHAR name_filter[] = L"All Files (*.*)|*.*|";

	CFileDialog load_dlg(TRUE/*FALSE 저장 , TRUE 불러오기*/, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
		OFN_NOCHANGEDIR, name_filter, NULL);

	// 다이얼로그를 띄운다.
	if(load_dlg.DoModal() == IDOK)
	{
		/*여기에 파일 데이터 입력*/
		CString FilePath = load_dlg.GetPathName();

		HANDLE hFile = CreateFile(FilePath
			, GENERIC_READ, 0, NULL, OPEN_EXISTING
			, FILE_ATTRIBUTE_NORMAL, NULL);

		CTileMgr::GetInstance()->LoadTile(hFile);
		CloseHandle(hFile);
	}
}
