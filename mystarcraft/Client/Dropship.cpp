#include "StdAfx.h"
#include "Dropship.h"

#include "ScrollMgr.h"
#include "TimeMgr.h"
#include "MouseMgr.h"

#include "Com_fog.h"
#include "Com_DropAnim.h"
#include "Com_AirPathfind.h"
#include "Com_AirCollision.h"

#include "ObjMgr.h"
#include "LineMgr.h"
#include "FontMgr.h"
#include "UI_Select.h"

#include "UnitMgr.h"
#include "Area_Mgr.h"

#include "GeneraEff.h"
#include "MyMath.h"
CDropship::CDropship(void)
{
}

CDropship::~CDropship(void)
{
	Release();
}

void CDropship::Initialize(void)
{
	CObj::unit_area_Initialize();
	CUnit::Initialize(); //맵 디스플레이

	m_sortID = SORT_AIR;	
	m_ecategory = UNIT;
	m_eteamnumber = TEAM_0;
	m_eOBJ_NAME = OBJ_DROPSHIP;

	m_unitinfo.eMoveType = MOVE_AIR;
	m_unitinfo.estate = IDLE;
	m_unitinfo.eorder = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_LARGE;
	m_unitinfo.hp = 150;
	m_unitinfo.maxhp = 150;
	m_unitinfo.mp = 0;
	m_unitinfo.maxmp = 0;
	m_unitinfo.fspeed = 93;
	m_unitinfo.attack_range = 0*32;
	m_unitinfo.air_attack_range = 0*32;
	m_unitinfo.search_range = 255;
	m_unitinfo.fog_range = 512;

	m_vertex.left = 24.5f;
	m_vertex.right = 24.5f;
	m_vertex.top =  18.5f;
	m_vertex.bottom = 18.5f;

	m_com_anim = new CCom_DropAnim(m_matWorld);
	m_com_pathfind = new CCom_AirPathfind(m_vPos);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_PATHFINDE , m_com_pathfind));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_AirCollision(m_vPos , m_rect , m_vertex)));

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_select_ui = new CUI_Select(L"Select32" , m_vPos , 13);
	m_select_ui->Initialize();
	CObjMgr::GetInstance()->AddSelect_UI(m_select_ui);
}

void CDropship::Update(void)
{
	CObj::area_update();	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();

	m_select_ui->Update();
}

void CDropship::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_com_anim->Render();

	CLineMgr::GetInstance()->collisionbox_render(m_rect);
}

void CDropship::Inputkey_reaction(const int& nkey)
{
	if( BOARDING == m_unitinfo.estate )
		return;

	if(VK_RBUTTON == nkey)
	{
		m_unitinfo.estate = MOVE;
		m_unitinfo.eorder = ORDER_MOVE;

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

void CDropship::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}

void CDropship::Release(void)
{
	CObj::area_release();

	CUnitMgr::GetInstance()->clear_destroy_unitlist(this);
}

void CDropship::Dead(void)
{
	CObj* pobj = new CGeneraEff(L"SMALLBANG" , m_vPos , D3DXVECTOR2(1.2f ,1.2f) , SORT_AIR );
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);
}
