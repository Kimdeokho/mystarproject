// MyForm.cpp : 구현 파일입니다.
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


// MyForm 진단입니다.

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


// MyForm 메시지 처리기입니다.

void MyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	m_pProSheet = new CMyProSheet(this);
	m_pProSheet->Create(this , WS_CHILD | WS_VISIBLE);

	m_pProSheet->MoveWindow(0,0,300,500,false);
	m_pProSheet->SetActivePage(0);
}

void MyForm::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	delete m_pProSheet;
	m_pProSheet = NULL;
}
