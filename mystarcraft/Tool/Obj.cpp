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
	InitCollRC();
	CDevice::GetInstance()->Render_End();
	CDevice::GetInstance()->Render_Begin();

	D3DXVECTOR2 vtemp[5];

	for(int i = 0; i < 5; ++i)
	{
		vtemp[i].x = m_vRectPoint[i].x;
		vtemp[i].y = m_vRectPoint[i].y;
	}

	CDevice::GetInstance()->GetLine()->SetWidth(1.0f);
	CDevice::GetInstance()->GetLine()->Draw(vtemp , 5 , D3DCOLOR_ARGB(255, 0 , 255, 0));

	CDevice::GetInstance()->Render_End();
	CDevice::GetInstance()->Render_Begin();
}

const RECT& CObj::GetRect(void)
{
	return m_collRc;
}

BOOL CObj::PtCollCheck(const CPoint& _pt)
{
	InitCollRC();
	BOOL bvalue = PtInRect(&m_collRc , _pt);
	return bvalue;
}
void CObj::InitCollRC(void)
{
	float X = m_vPos.x - CMyMouse::GetInstance()->GetScrollPt().x;
	float Y = m_vPos.y - CMyMouse::GetInstance()->GetScrollPt().y;

	m_collRc.left = long(X - m_vertex.left);
	m_collRc.right = long(X + m_vertex.right);
	m_collRc.top = long(Y - m_vertex.top);
	m_collRc.bottom = long(Y + m_vertex.bottom);

	m_vRectPoint[0] = D3DXVECTOR2( float(m_collRc.left) , float(m_collRc.top) );
	m_vRectPoint[1] = D3DXVECTOR2( float(m_collRc.right) , float(m_collRc.top) );
	m_vRectPoint[2] = D3DXVECTOR2( float(m_collRc.right) , float(m_collRc.bottom) );
	m_vRectPoint[3] = D3DXVECTOR2( float(m_collRc.left) , float(m_collRc.bottom) );
	m_vRectPoint[4] = m_vRectPoint[0];
}
void CObj::SaveInfo(HANDLE h)
{
	DWORD dwbyte;
	WriteFile(h , &m_vPos , sizeof(D3DXVECTOR2) , &dwbyte , NULL);
	WriteFile(h , m_ObjName , sizeof(WCHAR)*MIN_STR , &dwbyte , NULL);
}

void CObj::MinmapRender(const float fratioX , const float fratioY)
{

}
