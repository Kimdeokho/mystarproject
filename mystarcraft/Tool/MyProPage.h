#pragma once


// CMyProPage ��ȭ �����Դϴ�.

class CMyProPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CMyProPage)

public:
	CMyProPage();
	virtual ~CMyProPage();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MYPROPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
