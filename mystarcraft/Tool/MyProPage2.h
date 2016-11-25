#pragma once
#include "afxwin.h"


// CMyProPage2 대화 상자입니다.

class CMyProPage2 : public CPropertyPage
{
	DECLARE_DYNAMIC(CMyProPage2)

public:
	CMyProPage2();
	virtual ~CMyProPage2();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MYPROPAGE2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();

public:
	CListBox m_objListBox;
	afx_msg void OnLbnSelchangeList1();
};
