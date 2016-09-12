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
	AddPage(&m_page2);
}

CMyProSheet::~CMyProSheet()
{
}


BEGIN_MESSAGE_MAP(CMyProSheet, CPropertySheet)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMyProSheet �޽��� ó�����Դϴ�.

void CMyProSheet::OnSize(UINT nType, int cx, int cy)
{
	CPropertySheet::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

}

BOOL CMyProSheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO:  ���⿡ Ư��ȭ�� �ڵ带 �߰��մϴ�.

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

void CMyProSheet::AddString(const TCHAR* _wstr , int _order)
{
	switch(_order)
	{
	case 0:
		{
			m_page1.m_maptileListbox.AddString(_wstr);
			break;
		}
	case 1:
		{
			break;
		}
	}
}
void CMyProSheet::SetCursel(int page , int iidx)
{
	switch(page)
	{
	case 0:
		{
			m_page1.m_maptileListbox.SetCurSel(iidx);
			break;
		}
	}
}
bool CMyProSheet::FindStr(const TCHAR* _str , int _idx)
{
	bool bresult = false;
	if(_idx == 0)
	{
		if( 0 > m_page1.m_maptileListbox.FindString(0 , _str))
			bresult = false;
		else
			bresult = true;
	}
	else if(_idx == 1)
	{

	}
	return bresult;
}