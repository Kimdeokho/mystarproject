#pragma once
#include "afxwin.h"


// CMyProPage2 ��ȭ �����Դϴ�.

class CMyProPage2 : public CPropertyPage
{
	DECLARE_DYNAMIC(CMyProPage2)

public:
	CMyProPage2();
	virtual ~CMyProPage2();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MYPROPAGE2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();

public:
	CListBox m_objListBox;
	afx_msg void OnLbnSelchangeList1();
};
