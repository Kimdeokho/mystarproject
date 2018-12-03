#include "StdAfx.h"
#include "Thorn_Bullet.h"

#include "Com_ThornAnim.h"


#include "ScrollMgr.h"

CThorn_Bullet::CThorn_Bullet(void)
{
}

CThorn_Bullet::~CThorn_Bullet(void)
{
	Release();
}

void CThorn_Bullet::Initialize(void)
{
	m_sortID = SORT_GROUND_EFF;
	m_eteamnumber = TEAM_NONE;

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , new CCom_ThornAnim(m_matWorld , L"LURK_BULLET")));

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);
}

void CThorn_Bullet::Update(void)
{
	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;
}

void CThorn_Bullet::Render(void)
{
	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Render();
}

void CThorn_Bullet::Dead(void)
{

}

void CThorn_Bullet::Release(void)
{

}
