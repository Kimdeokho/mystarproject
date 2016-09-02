#pragma once


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
};
