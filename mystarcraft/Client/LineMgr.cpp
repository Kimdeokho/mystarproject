#include "StdAfx.h"
#include "LineMgr.h"

#include "Device.h"
#include "Area_Mgr.h"
#include "ScrollMgr.h"
#include "MyMath.h"

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
void CLineMgr::Choose_army_unit(void)
{
	MYRECT<float>	rc;

	rc.left = m_RectLine[0].x;
	rc.top = m_RectLine[0].y;

	rc.right = m_RectLine[2].x;
	rc.bottom = m_RectLine[2].y;

	float fX = CScrollMgr::m_fScrollX + BACKBUFFER_SIZEX/2;
	float fY = CScrollMgr::m_fScrollY + BACKBUFFER_SIZEY/2;

	int idx = CMyMath::Pos_to_index(fX , fY , 256);


}
void CLineMgr::SetRenderSwitch(bool bswitch)
{
	m_bSwitch = bswitch;
}
