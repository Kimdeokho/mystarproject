// Minimapview.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "Minimapview.h"

#include "Device.h"
// CMinimapview

IMPLEMENT_DYNCREATE(CMinimapview, CView)

CMinimapview::CMinimapview()
{

}

CMinimapview::~CMinimapview()
{
	CDevice::GetInstance()->DestroyInstance();
}

BEGIN_MESSAGE_MAP(CMinimapview, CView)
END_MESSAGE_MAP()


// CMinimapview �׸����Դϴ�.

void CMinimapview::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.

	CDevice::GetInstance()->Render_Begin();
	CDevice::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);;

	CDevice::GetInstance()->GetSprite()->End();
	CDevice::GetInstance()->Render_End(m_hWnd);
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
