#include "StdAfx.h"
#include "Turret_head.h"

#include "Component.h"
#include "Com_TurretheadAnim.h"
#include "Com_Distancesearch.h"
#include "Com_WTurret.h"

#include "ScrollMgr.h"
#include "MyMath.h"
#include "Area_Mgr.h"
#include "UnitMgr.h"
#include "ObjMgr.h"
#include "GeneraEff.h"

#include "LineMgr.h"
#include "FontMgr.h"
#include "Corpse.h"
#include "ComanderMgr.h"
CTurret_head::CTurret_head(void)
{
}

CTurret_head::~CTurret_head(void)
{
}

void CTurret_head::Initialize(void)
{
	m_com_pathfind = NULL;

	m_vertex.left = 16.f;
	m_vertex.right = 17.f;
	m_vertex.top =  32.f;
	m_vertex.bottom = 17.f;

	//CTerran_building::building_area_Initialize(2 , 2);
	CTerran_building::building_pos_Initialize(2 , 2);

	m_sortID = SORT_GROUND;	
	m_ecategory = BUILDING;
	m_eOBJ_NAME = OBJ_TURRET;
	m_eteamnumber = TEAM_0;

	m_unitinfo.eAttackType = ATTACK_ANYTHING; //°øÁßÀ¸·Î ¹Ù²Ü²¨
	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.estate = IDLE;
	m_unitinfo.eorder = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_LARGE;
	m_unitinfo.hp = 1;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 0;
	m_unitinfo.air_attack_range = 7*32;
	m_unitinfo.attack_range = 7*32;
	m_unitinfo.search_range = 224;
	m_unitinfo.fog_range = 512;
	m_unitinfo.fbuildtime = 1.f;

	m_com_anim = new CCom_TurretheadAnim(m_matWorld );

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_TARGET_SEARCH ,new CCom_Distancesearch(SEARCH_ONLY_ENEMY)));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_WEAPON , new CCom_WTurret()));

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

}

void CTurret_head::Update(void)
{
	if(BUILD == m_unitinfo.estate)
		return;

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();

	if(IDLE == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
	}
	else if(ATTACK == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"ATTACK");
	}

}

void CTurret_head::Render(void)
{
	if(BUILD == m_unitinfo.estate)
		return;

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_com_anim->Render();
}

void CTurret_head::Release(void)
{

}

void CTurret_head::Dead(void)
{

}

void CTurret_head::Inputkey_reaction(const int& nkey)
{

}

void CTurret_head::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}

