// MyProSheet.cpp : 구현 파일입니다.
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
	AddPage(&m_page2);
}

CMyProSheet::~CMyProSheet()
{
}


BEGIN_MESSAGE_MAP(CMyProSheet, CPropertySheet)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMyProSheet 메시지 처리기입니다.

void CMyProSheet::OnSize(UINT nType, int cx, int cy)
{
	CPropertySheet::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

}

BOOL CMyProSheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO:  여기에 특수화된 코드를 추가합니다.

	HWND hWnd = (HWND)::GetDlgItem(m_hWnd, AFX_IDC_TAB_CONTROL); 
	CRect rectOld; 
	::GetWindowRect(hWnd, &rectOld); 
	ScreenToClient(rectOld); 
	int nCy = rectOld.Width()/2; 
	rectOld.right -= nCy; 
	rectOld.bottom += rectOld.Height();

	// move tab control 
	::SetWindowPos(hWnd, NULL, 0, 0,rectOld.Width(), rectOld.Height(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE); 
	GetWindowRect(&rectOld); 
	rectOld.right -= nCy; 
	//SetWindowPos(NULL, 0, 0, rectOld.Width(), rectOld.Height(),	SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE); 

	SetPageTitle(0 , L"Map");
	SetPageTitle(1 , L"Object");
	return bResult;
}
BOOL CMyProSheet::SetPageTitle (int nPage, LPTSTR pszText)  
{  
	CTabCtrl* pTab = GetTabControl();  
	ASSERT (pTab);  

	TC_ITEM ti;  
	ti.mask = TCIF_TEXT;  
	ti.pszText = pszText;  
	VERIFY (pTab->SetItem (nPage, &ti));  

	return TRUE;  
}  
