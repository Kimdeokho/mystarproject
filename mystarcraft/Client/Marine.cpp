#include "StdAfx.h"
#include "Marine.h"

#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "TimeMgr.h"
#include "MouseMgr.h"


#include "Com_fog.h"
#include "Com_Pathfind.h"
#include "Com_MarineAnim.h"
#include "Com_Wmarine.h"
#include "Com_Distancesearch.h"

#include "Skill_SP.h"

#include "ObjMgr.h"
#include "LineMgr.h"
#include "TileManager.h"
#include "Com_Collision.h"
#include "FontMgr.h"
#include "UI_Select.h"

#include "Corpse.h"
#include "UnitMgr.h"
#include "Area_Mgr.h"
#include "MyMath.h"

#include "ComanderMgr.h"

#include "UI_Wireframe.h"
CMarine::CMarine(void)
{
}

CMarine::~CMarine(void)
{
	Release();
}

void CMarine::Initialize(void)
{
	CObj::unit_area_Initialize();
	CUnit::Initialize(); //맵 디스플레이

	m_curtex = NULL;
	m_com_pathfind = NULL;
	m_com_targetsearch = NULL;

	
	m_sortID = SORT_GROUND;	
	m_ecategory = UNIT;
	m_eOBJ_NAME = OBJ_MARINE;
	m_eteamnumber = TEAM_0;

	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.estate = IDLE;
	m_unitinfo.eorder = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_SMALL;
	m_unitinfo.hp = 40;
	m_unitinfo.maxhp = m_unitinfo.hp;
	m_unitinfo.mp = 0;
	m_unitinfo.maxmp = 0;
	m_unitinfo.fspeed = 68;
	m_unitinfo.attack_range = 4*32;
	m_unitinfo.air_attack_range = 4*32;
	m_unitinfo.search_range = 6*32;
	m_unitinfo.fog_range = 512;


	m_vertex.left = 8.5;
	m_vertex.right = 8.5;
	m_vertex.top =  10;
	m_vertex.bottom = 10;

	m_com_pathfind = new CCom_Pathfind(m_vPos , m_rect , 32 ,32);
	m_com_weapon = new CCom_Wmarine();
	m_com_targetsearch = new CCom_Distancesearch(SEARCH_ONLY_ENEMY);
	m_com_anim = new CCom_MarineAnim(m_matWorld);
	m_skill_sp = new CSkill_SP(this , m_com_weapon);
	m_com_collision = new CCom_Collision(m_vPos , m_rect , m_vertex);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_TARGET_SEARCH ,  m_com_targetsearch ) );	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , m_com_collision ) ) ;	
	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_PATHFINDE ,  m_com_pathfind) );
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_WEAPON ,  m_com_weapon )) ;	


	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);


	m_select_ui = new CUI_Select(L"Select22" , m_vPos , 13);
	m_select_ui->Initialize();
	CObjMgr::GetInstance()->AddSelect_UI(m_select_ui , MOVE_GROUND);

}

void CMarine::Update(void)
{
	CObj::area_update();	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();


	m_select_ui->Update();
	m_skill_sp->Update();


	if(IDLE == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
	}
	else if(ATTACK == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"ATTACK");
	}
	else if(MOVE == m_unitinfo.estate || COLLISION == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"MOVE");
	}
}

void CMarine::Render(void)
{
	if( BOARDING == m_unitinfo.estate ||
		ORDER_BUNKER_BOARDING == m_unitinfo.eorder)
		return;

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_com_anim->Render();

	m_com_pathfind->Render();
	CLineMgr::GetInstance()->collisionbox_render(m_rect);
}

void CMarine::Inputkey_reaction(const int& nkey)
{
	if( BOARDING == m_unitinfo.estate )
		return;

	if(VK_RBUTTON == nkey)
	{
		m_unitinfo.estate = MOVE;
		m_unitinfo.eorder = ORDER_MOVE;

		CObj* ptarget = CArea_Mgr::GetInstance()->GetChoiceTarget();
		((CCom_Targetsearch*)m_com_targetsearch)->SetTarget(ptarget);

		if(NULL != m_com_pathfind)
		{
			D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();

			((CCom_Pathfind*)m_com_pathfind)->SetGoalPos(goalpos , m_bmagicbox);
			((CCom_Pathfind*)m_com_pathfind)->SetFlowField();
			((CCom_Pathfind*)m_com_pathfind)->StartPathfinding();
			m_bmagicbox = false;
		}
	}
	if('Q' == nkey)
	{
		//m_fspeed = 30;
		//m_eteamnumber = TEAM_0;
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

void CMarine::Inputkey_reaction(const int& firstkey , const int& secondkey)
{
	if( BOARDING == m_unitinfo.estate )
		return;

	if('A' == firstkey && VK_LBUTTON == secondkey)
	{
		m_unitinfo.eorder = ORDER_MOVE_ATTACK;
		m_unitinfo.estate = MOVE;
		((CCom_Targetsearch*)m_com_targetsearch)->SetTarget(CArea_Mgr::GetInstance()->GetChoiceTarget());

		if(NULL != m_com_pathfind)
		{
			D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();


			((CCom_Pathfind*)m_com_pathfind)->SetGoalPos(goalpos , m_bmagicbox);
			((CCom_Pathfind*)m_com_pathfind)->SetFlowField();
			((CCom_Pathfind*)m_com_pathfind)->StartPathfinding();
			m_bmagicbox = false;
		}
	}
}

void CMarine::Release(void)
{
	CObj::area_release();

	m_com_pathfind = NULL;
	m_com_weapon = NULL;
	Safe_Delete(m_skill_sp);
	
}

void CMarine::Dead(void)
{
	CObj* pobj = new CCorpse(L"MARINEDEAD" , L"MARINEWRECKAGE");
	pobj->SetPos(m_vPos.x , m_vPos.y);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddCorpse(pobj);

}
void CMarine::Update_Cmdbtn(void)
{
	CComanderMgr::GetInstance()->Create_Cmdbtn(0 , L"BTN_MOVE" , BTN_MOVE);
	CComanderMgr::GetInstance()->Create_Cmdbtn(1 , L"BTN_STOP" , BTN_STOP);
	CComanderMgr::GetInstance()->Create_Cmdbtn(2 , L"BTN_ATTACK" , BTN_ATTACK);
	CComanderMgr::GetInstance()->Create_Cmdbtn(3 , L"BTN_PATROL" , BTN_PATROL);
	CComanderMgr::GetInstance()->Create_Cmdbtn(4 , L"BTN_HOLD" , BTN_HOLD);

	CComanderMgr::GetInstance()->Create_Cmdbtn(6 , L"BTN_STEAMPACK" , BTN_STEAMPACK);
}

void CMarine::Update_Wireframe(void)
{
	if(true == CComanderMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.estate))
	{
		CUI* pui = NULL;
		pui = new CUI_Wireframe(L"WIRE_MARINE" , D3DXVECTOR2(280,545));
		pui->Initialize();
		CComanderMgr::GetInstance()->add_wireframe_ui(pui);

		CFontMgr::GetInstance()->SetInfomation_font(L"Terran Marine" , 400 , 510 );
	}
}
