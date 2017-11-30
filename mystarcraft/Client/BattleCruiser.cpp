#include "StdAfx.h"
#include "BattleCruiser.h"

#include "ScrollMgr.h"
#include "TimeMgr.h"
#include "MouseMgr.h"

#include "Com_fog.h"
#include "Com_Airsearch.h"
#include "Com_BattleAnim.h"
#include "Com_AirPathfind.h"
#include "Com_WBattle.h"
#include "Com_AirCollision.h"

#include "ObjMgr.h"
#include "LineMgr.h"
#include "FontMgr.h"
#include "UI_Select.h"

#include "Corpse.h"
#include "UnitMgr.h"
#include "Area_Mgr.h"

#include "GeneraEff.h"
#include "MyMath.h"

CBattleCruiser::CBattleCruiser(void)
{
}

CBattleCruiser::~CBattleCruiser(void)
{
	Release();
}

void CBattleCruiser::Initialize(void)
{
	CObj::unit_area_Initialize();
	CUnit::Initialize(); //맵 디스플레이

	m_sortID = SORT_AIR;	
	m_ecategory = UNIT;
	m_eteamnumber = TEAM_0;
	m_eOBJ_NAME = OBJ_BATTLE;

	m_unitinfo.eMoveType = MOVE_AIR;
	m_unitinfo.estate = IDLE;
	m_unitinfo.eorder = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_LARGE;
	m_unitinfo.hp = 500;
	m_unitinfo.maxhp = 500;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 60;
	m_unitinfo.attack_range = 6*32;
	m_unitinfo.air_attack_range = 6*32;
	m_unitinfo.search_range = 256;
	m_unitinfo.fog_range = 512;

	m_vertex.left = 37.5f;
	m_vertex.right = 37.5f;
	m_vertex.top =  28;
	m_vertex.bottom = 28;

	m_com_targetsearch = new CCom_Airsearch();
	m_com_anim = new CCom_BattleAnim(m_matWorld);
	m_com_pathfind = new CCom_AirPathfind(m_vPos);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_TARGET_SEARCH ,  m_com_targetsearch ) );
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_PATHFINDE , m_com_pathfind));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_WEAPON , new CCom_WBattle()));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_AirCollision(m_vPos , m_rect , m_vertex)));

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_select_ui = new CUI_Select(L"Select94" , m_vPos , 13);
	m_select_ui->Initialize();
	CObjMgr::GetInstance()->AddSelect_UI(m_select_ui);
}

void CBattleCruiser::Update(void)
{
	CObj::area_update();	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();

	m_select_ui->Update();

	CFontMgr::GetInstance()->Setbatch_Font(L"%d" , (int)(m_unitinfo.hp) , m_matWorld._41 , m_matWorld._42);
}

void CBattleCruiser::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_com_anim->Render();

	CLineMgr::GetInstance()->collisionbox_render(m_rect);
}

void CBattleCruiser::Inputkey_reaction(const int& nkey)
{
	if( BOARDING == m_unitinfo.estate )
		return;

	if(VK_RBUTTON == nkey)
	{
		m_unitinfo.estate = MOVE;
		m_unitinfo.eorder = ORDER_MOVE;

		CObj* ptarget = CArea_Mgr::GetInstance()->GetChoiceTarget();
		((CCom_Targetsearch*)m_com_targetsearch)->SetTarget(ptarget);

		D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();
		((CCom_AirPathfind*)m_com_pathfind)->SetGoalPos(goalpos , m_bmagicbox);
		m_bmagicbox = true;
	}
	if('W' == nkey)
	{
		m_eteamnumber = TEAM_1;
		//m_skill_sp->use();
	}
	if('A' == nkey)
	{		
	}
	if('Z' == nkey)
	{
	}
}

void CBattleCruiser::Inputkey_reaction(const int& firstkey , const int& secondkey)
{
	if('A' == firstkey && VK_LBUTTON == secondkey)
	{
		m_unitinfo.eorder = ORDER_MOVE_ATTACK;
		m_unitinfo.estate = MOVE;
		((CCom_Targetsearch*)m_com_targetsearch)->SetTarget(CArea_Mgr::GetInstance()->GetChoiceTarget());

		if(NULL != m_com_pathfind)
		{
			D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();
			((CCom_AirPathfind*)m_com_pathfind)->SetGoalPos(goalpos , m_bmagicbox);

			m_bmagicbox = false;
		}
	}
}

void CBattleCruiser::Release(void)
{
	CObj::area_release();

	CUnitMgr::GetInstance()->clear_destroy_unitlist(this);
}
void CBattleCruiser::Dead(void)
{
	CObj* pobj = new CGeneraEff(L"LARGEBANG" , m_vPos , D3DXVECTOR2(1.0f ,1.0f) , SORT_AIR );
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);

}