#include "StdAfx.h"
#include "Obj.h"
#include "Device.h"
#include "MyMouse.h"

CObj::CObj(void)
{
	D3DXMatrixIdentity(&m_matWorld);

	m_collRc.bottom = m_collRc.right = m_collRc.left = m_collRc.top = 0;

	for(int i = 0; i < 5; ++i)
		m_vRectPoint[i] = D3DXVECTOR2(0.f,0.f);
}

CObj::~CObj(void)
{
}

void CObj::Initialize(void)
{
}
void CObj::Render(void)
{

}
void CObj::SetPos(const float x , const float y , OBJ_POS_KIND ekind)
{
	m_vPos.x = x + CMyMouse::GetInstance()->GetScrollPt().x;
	m_vPos.y = y + CMyMouse::GetInstance()->GetScrollPt().y;

	m_matWorld._41 = m_vPos.x;
	m_matWorld._42 = m_vPos.y;

	m_ePosKind = ekind;
}

void CObj::DrawRect(void)
{
	CDevice::GetInstance()->Render_End();
	CDevice::GetInstance()->Render_Begin();

	CDevice::GetInstance()->GetLine()->SetWidth(1.0f);
	CDevice::GetInstance()->GetLine()->Draw(m_vRectPoint , 5 , D3DCOLOR_ARGB(255, 0 , 255, 0));

	CDevice::GetInstance()->Render_End();
	CDevice::GetInstance()->Render_Begin();
}

const RECT& CObj::GetRect(void)
{
	return m_collRc;
}

BOOL CObj::PtCollCheck(const CPoint& _pt)
{
	BOOL bvalue = PtInRect(&m_collRc , _pt);
	return bvalue;
}
