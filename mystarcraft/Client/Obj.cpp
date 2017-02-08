#include "StdAfx.h"
#include "Obj.h"

CObj::CObj(void)
{
	D3DXMatrixIdentity(&m_matWorld);
	m_ePosKind = STATIC_OBJ;
	m_pSprite = NULL;
}

CObj::~CObj(void)
{
}

void CObj::Initialize(void)
{

}

void CObj::Update(void)
{

}

void CObj::Render(void)
{

}

void CObj::Release(void)
{

}
void CObj::SetPos(const float x , const float y, OBJ_POS_KIND ekind /*= STATIC_OBJ*/)
{
	m_ePosKind = ekind;
	m_vPos.x = x;
	m_vPos.y = y;
	m_matWorld._41 = x;
	m_matWorld._42 = y;
}
