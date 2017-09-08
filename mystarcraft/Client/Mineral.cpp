#include "StdAfx.h"
#include "Mineral.h"

#include "TextureMgr.h"
#include "Device.h"
#include "ScrollMgr.h"
CMineral::CMineral(void)
{
}

CMineral::~CMineral(void)
{
}

void CMineral::Initialize(void)
{
	CObj::Initialize();

	m_rect.left		= m_vPos.x - 32;
	m_rect.right	= m_vPos.x + 32;
	m_rect.top		= m_vPos.y - 16;
	m_rect.bottom	= m_vPos.y + 16;

 

	m_sortID = SORT_GROUND;
}
void CMineral::Update(void)
{

}
void CMineral::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

}
void CMineral::Release(void)
{

}