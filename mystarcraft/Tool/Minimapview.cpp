// Minimapview.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "Minimapview.h"

#include "Device.h"
#include "TileMgr.h"
// CMinimapview
#include "MainFrm.h"
#include "ToolView.h"

IMPLEMENT_DYNCREATE(CMinimapview, CView)

CMinimapview::CMinimapview()
{

}

CMinimapview::~CMinimapview()
{
}

BEGIN_MESSAGE_MAP(CMinimapview, CView)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CMinimapview �׸����Դϴ�.

void CMinimapview::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.

	CDevice::GetInstance()->GetDevice()->Clear(0, NULL
		, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL
		, D3DCOLOR_XRGB(0,0,255)/*0xff0000ff*/, 1.f, 0);

	CDevice::GetInstance()->Render_Begin();

	CTileMgr::GetInstance()->MinimapRender();

	CDevice::GetInstance()->Render_End();

	//CDevice::GetInstance()->GetDevice()->Present(NULL, NULL, m_hWnd, NULL);

	CTileMgr::GetInstance()->SetMinimapupdate();

	CTileMgr::GetInstance()->MinimapSquare();

	CDevice::GetInstance()->GetDevice()->Present(NULL, NULL, m_hWnd, NULL);
}


// CMinimapview �����Դϴ�.

#ifdef _DEBUG
void CMinimapview::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMinimapview::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMinimapview �޽��� ó�����Դϴ�.

void CMinimapview::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	m_pMainfrm = (CMainFrame*)AfxGetMainWnd();
	m_pToolView = m_pMainfrm->m_pToolView;

	m_bLbtnClick = false;
}

void CMinimapview::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CView::OnLButtonDblClk(nFlags, point);
}

void CMinimapview::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CView::OnLButtonDown(nFlags, point);

	m_bLbtnClick = true;

	int x = point.x;
	int y = point.y;

	CTileMgr::GetInstance()->SetMinimapPoint(point);
	m_pToolView->SetScrollPos(0 , x*16 - TOOLSIZE_X/2);
	m_pToolView->SetScrollPos(1 , y*16 - TOOLSIZE_Y/2);

	
	m_pToolView->Invalidate(FALSE);
	Invalidate(FALSE);
}

void CMinimapview::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CView::OnMouseMove(nFlags, point);

	if(true == m_bLbtnClick)
	{
		int x = point.x;
		int y = point.y;

		CTileMgr::GetInstance()->SetMinimapPoint(point);
		m_pToolView->SetScrollPos(0 , x*16 - TOOLSIZE_X/2);
		m_pToolView->SetScrollPos(1 , y*16 - TOOLSIZE_Y/2);
	}
	m_pToolView->Invalidate(FALSE);
	Invalidate(FALSE);
}

void CMinimapview::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CView::OnLButtonUp(nFlags, point);

	m_bLbtnClick = false;
}
