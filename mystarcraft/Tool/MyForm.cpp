// MyForm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"
#include "MyProSheet.h"

// MyForm

IMPLEMENT_DYNCREATE(MyForm, CFormView)

MyForm::MyForm()
	: CFormView(MyForm::IDD)
{

}

MyForm::~MyForm()
{
}

void MyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(MyForm, CFormView)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// MyForm �����Դϴ�.

#ifdef _DEBUG
void MyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void MyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// MyForm �޽��� ó�����Դϴ�.

void MyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	m_pProSheet = new CMyProSheet(this);
	m_pProSheet->Create(this , WS_CHILD | WS_VISIBLE);

	m_pProSheet->MoveWindow(0,0,300,500,false);
	m_pProSheet->SetActivePage(0);
}

void MyForm::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	delete m_pProSheet;
	m_pProSheet = NULL;
}
