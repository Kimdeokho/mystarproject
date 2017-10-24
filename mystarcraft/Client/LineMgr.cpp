#include "StdAfx.h"
#include "LineMgr.h"

#include "Device.h"
#include "Area_Mgr.h"
#include "ScrollMgr.h"
#include "MyMath.h"
#include "MouseMgr.h"
#include "ComanderMgr.h"
IMPLEMENT_SINGLETON(CLineMgr)
CLineMgr::CLineMgr(void)
{
}

CLineMgr::~CLineMgr(void)
{
}
void CLineMgr::Initialize(void)
{
	m_pLine = CDevice::GetInstance()->GetLine();
	m_bSwitch = false;
}
void CLineMgr::SetRectPoint(const D3DXVECTOR2& vstart , const D3DXVECTOR2& vend)
{
	m_RectLine[0] = vstart;

	m_RectLine[1].x = vend.x;
	m_RectLine[1].y = vstart.y;

	m_RectLine[2] = vend;

	m_RectLine[3].x = vstart.x;
	m_RectLine[3].y = vend.y;

	m_RectLine[4] = vstart;
}

void CLineMgr::LineRender(void)
{
	if(true == m_bSwitch)
	{
		CDevice::GetInstance()->Render_End();
		CDevice::GetInstance()->Render_Begin();

		m_pLine->SetWidth(1.0f);
		m_pLine->Draw(m_RectLine , 5 , D3DCOLOR_ARGB(255,0,255,0));

		CDevice::GetInstance()->Render_End();
		CDevice::GetInstance()->Render_Begin();
	}
}
void CLineMgr::collisionbox_render(const MYRECT<float>& rc)
{
	D3DXVECTOR2 vpt[5];

	vpt[0].x = rc.left - CScrollMgr::m_fScrollX;
	vpt[0].y = rc.top - CScrollMgr::m_fScrollY;

	vpt[1].x = rc.right - CScrollMgr::m_fScrollX;
	vpt[1].y = rc.top - CScrollMgr::m_fScrollY;

	vpt[2].x = rc.right - CScrollMgr::m_fScrollX;
	vpt[2].y = rc.bottom - CScrollMgr::m_fScrollY;

	vpt[3].x = rc.left - CScrollMgr::m_fScrollX;
	vpt[3].y = rc.bottom - CScrollMgr::m_fScrollY;

	vpt[4] = vpt[0];

	CDevice::GetInstance()->Render_End();
	CDevice::GetInstance()->Render_Begin();

	m_pLine->SetWidth(1.0f);
	m_pLine->Draw(vpt , 5 , D3DCOLOR_ARGB(255,0,255,0));

	CDevice::GetInstance()->Render_End();
	CDevice::GetInstance()->Render_Begin();
}
void CLineMgr::minicambox_render(const MYRECT<float>& rc)
{
	D3DXVECTOR2 vpt[5];

	vpt[0].x = rc.left;
	vpt[0].y = rc.top;

	vpt[1].x = rc.right;
	vpt[1].y = rc.top;

	vpt[2].x = rc.right;
	vpt[2].y = rc.bottom;

	vpt[3].x = rc.left;
	vpt[3].y = rc.bottom;

	vpt[4] = vpt[0];

	CDevice::GetInstance()->Render_End();
	CDevice::GetInstance()->Render_Begin();

	m_pLine->SetWidth(1.0f);
	m_pLine->Draw(vpt , 5 , D3DCOLOR_ARGB(255,255,255,255));

	CDevice::GetInstance()->Render_End();
	CDevice::GetInstance()->Render_Begin();
}
void CLineMgr::Select_unit(void)
{	
	if(true == CComanderMgr::GetInstance()->GetPreview_Active())
		return;


	D3DXVECTOR2 vMousept;

	vMousept = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
	int idx = CMyMath::Pos_to_index(vMousept.x , vMousept.y , 64);


	if( (int)m_RectLine[0].x == (int)m_RectLine[2].x &&
		(int)m_RectLine[0].y == (int)m_RectLine[2].y) 
	{
		//제자리에 찍었을때
		CArea_Mgr::GetInstance()->SelectCheck(idx , vMousept);
	}
	else
	{
		MYRECT<float>	rc;

		if(m_RectLine[0].x < m_RectLine[2].x)
		{
			rc.left = m_RectLine[0].x + CScrollMgr::m_fScrollX;
			rc.right = m_RectLine[2].x + CScrollMgr::m_fScrollX;
		}
		else
		{
			rc.left = m_RectLine[2].x + CScrollMgr::m_fScrollX;
			rc.right = m_RectLine[0].x + CScrollMgr::m_fScrollX;
		}

		if(m_RectLine[0].y < m_RectLine[2].y)
		{
			rc.top = m_RectLine[0].y + CScrollMgr::m_fScrollY;
			rc.bottom = m_RectLine[2].y + CScrollMgr::m_fScrollY;
		}
		else
		{
			rc.top = m_RectLine[2].y + CScrollMgr::m_fScrollY;
			rc.bottom = m_RectLine[0].y + CScrollMgr::m_fScrollY;
		}

		CArea_Mgr::GetInstance()->DragCheck( rc);
	}
	//DragCheck()
}
void CLineMgr::RenderGrid(const int& tilesize , const int& tilecnt)
{
	const int iwidth = 1000;
	const int iheight = 800;

	int col = iwidth / tilesize + 2;
	int row = iheight / tilesize + 2;
	
	D3DXVECTOR2	vPoint[2];

	int scrollX = (int)CScrollMgr::m_fScrollX;
	int scrollY = (int)CScrollMgr::m_fScrollY;

	CDevice::GetInstance()->Render_End();
	CDevice::GetInstance()->Render_Begin();

	for(int j = 0; j < col; ++j)
	{
		int index = j + scrollX/tilesize;
		vPoint[0] = D3DXVECTOR2( float(index*tilesize) - scrollX,0 - (float)scrollY);
		vPoint[1] = D3DXVECTOR2( float(index*tilesize) - scrollX, float(tilecnt*tilesize) - scrollY);

		CDevice::GetInstance()->GetLine()->SetWidth(1.0f);
		CDevice::GetInstance()->GetLine()->Draw(vPoint , 2 , D3DCOLOR_ARGB(255,0,0,0));
	}
	for(int j = 0; j < row; ++j)
	{
		int index = j + scrollY/tilesize;
		vPoint[0] = D3DXVECTOR2(0 - (float)scrollX ,float(index*tilesize) - scrollY );
		vPoint[1] = D3DXVECTOR2(float (tilecnt*tilesize) - scrollX, float(index*tilesize) - scrollY);

		CDevice::GetInstance()->GetLine()->SetWidth(1.0f);
		CDevice::GetInstance()->GetLine()->Draw(vPoint , 2 , D3DCOLOR_ARGB(255,0,0,0));
	}

	CDevice::GetInstance()->Render_End();
	CDevice::GetInstance()->Render_Begin();
}
void CLineMgr::SetRenderSwitch(bool bswitch)
{
	m_bSwitch = bswitch;
}
