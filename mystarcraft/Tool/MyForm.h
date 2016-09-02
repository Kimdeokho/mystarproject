#pragma once



// MyForm 폼 뷰입니다.

//#include "MyProSheet.h"
class CMyProSheet;
class MyForm : public CFormView
{
	DECLARE_DYNCREATE(MyForm)

protected:
	MyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~MyForm();

public:
	enum { IDD = IDD_MYFORM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

private:
	CMyProSheet*	m_pProSheet;
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
};


