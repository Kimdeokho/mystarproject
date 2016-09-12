#pragma once
#include "afxwin.h"


// CMyProPage 대화 상자입니다.

class CMyProPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CMyProPage)

public:
	CMyProPage();
	virtual ~CMyProPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MYPROPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

public:
	CListBox m_maptileListbox;
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	virtual BOOL OnSetActive();
	afx_msg void OnEnable(BOOL bEnable);
};
