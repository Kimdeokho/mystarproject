// Minimapview.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "Minimapview.h"

#include "Device.h"
#include "TileMgr.h"
// CMinimapview

IMPLEMENT_DYNCREATE(CMinimapview, CView)

CMinimapview::CMinimapview()
{

}

CMinimapview::~CMinimapview()
{
}

BEGIN_MESSAGE_MAP(CMinimapview, CView)
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
	//CDevice::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);;

	CTileMgr::GetInstance()->MinimapRender();
	//CDevice::GetInstance()->GetSprite()->End();
	CDevice::GetInstance()->Render_End();

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
