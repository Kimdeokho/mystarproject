#pragma once


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
};
