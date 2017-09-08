#include "StdAfx.h"
#include "GasResource.h"

#include "TextureMgr.h"
#include "Device.h"
#include "ScrollMgr.h"
CGasResource::CGasResource(void)
{
}

CGasResource::~CGasResource(void)
{
}

void CGasResource::Initialize(void)
{
	CObj::Initialize();

	m_rect.left		= m_vPos.x - 64;
	m_rect.right	= m_vPos.x + 64;
	m_rect.top		= m_vPos.y - 32;
	m_rect.bottom	= m_vPos.y + 32;


	m_sortID = SORT_GROUND;
}
void CGasResource::Update(void)
{

}
void CGasResource::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;


}
void CGasResource::Release(void)
{

}
