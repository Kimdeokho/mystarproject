#pragma once



// MyForm �� ���Դϴ�.

//#include "MyProSheet.h"
class CMyProSheet;
class MyForm : public CFormView
{
	DECLARE_DYNCREATE(MyForm)

protected:
	MyForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

private:
	CMyProSheet*	m_pProSheet;
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
};


