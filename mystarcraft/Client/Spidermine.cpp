#include "StdAfx.h"
#include "Spidermine.h"

#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "TimeMgr.h"
#include "MouseMgr.h"

#include "Com_fog.h"
#include "Com_Pathfind.h"
#include "Com_VultureAnim.h"
#include "Com_Distancesearch.h"
#include "Com_WVulture.h"

#include "ObjMgr.h"
#include "LineMgr.h"
#include "TileManager.h"
#include "Com_Collision.h"
#include "FontMgr.h"
#include "UI_Select.h"

#include "UnitMgr.h"
#include "Area_Mgr.h"
#include "MyMath.h"
#include "GeneraEff.h"
CSpidermine::CSpidermine(void)
{
}

CSpidermine::~CSpidermine(void)
{
	Release();
}

void CSpidermine::Initialize(void)
{
	CObj::unit_area_Initialize();
	CUnit::Initialize(); //맵 디스플레이

	m_curtex = NULL;
	m_com_pathfind = NULL;
	m_com_targetsearch = NULL;


	m_sortID = SORT_GROUND;	
	m_ecategory = UNIT;
	m_eOBJ_NAME = OBJ_VULTURE;
	m_eteamnumber = TEAM_0;

	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.estate = IDLE;
	m_unitinfo.eorder = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_MEDIUM;
	m_unitinfo.hp = 20;
	m_unitinfo.maxhp = m_unitinfo.hp;
	m_unitinfo.mp = 0;
	m_unitinfo.maxmp = 0;
	m_unitinfo.fspeed = 155;
	m_unitinfo.attack_range = 0*32;
	m_unitinfo.air_attack_range = 0*32;
	m_unitinfo.search_range = 3*32;
	m_unitinfo.fog_range = 96;
	m_unitinfo.eAttackType = ATTACK_ONLY_GROUND;


	m_vertex.left = 7.5;
	m_vertex.right = 7.5;
	m_vertex.top =  7.5;
	m_vertex.bottom = 7.5;

	m_com_pathfind = new CCom_Pathfind(m_vPos , m_rect , 32 ,32);
	m_com_targetsearch = new CCom_Distancesearch(SEARCH_ONLY_ENEMY);
	m_com_anim = new CCom_VultureAnim(m_matWorld);
	m_com_collision = new CCom_Collision(m_vPos , m_rect , m_vertex);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_TARGET_SEARCH ,  m_com_targetsearch ) );	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , m_com_collision ) ) ;	

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_PATHFINDE ,  m_com_pathfind) );	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_WEAPON ,  new CCom_WVulture()) );


	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);


	m_select_ui = new CUI_Select(L"Select32" , m_vPos , 13);
	m_select_ui->Initialize();
	CObjMgr::GetInstance()->AddSelect_UI(m_select_ui);
}

void CSpidermine::Update(void)
{
	CObj::area_update();	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();


	m_select_ui->Update();

	if(IDLE == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
	}
	else if(ATTACK == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
	}
	else if(MOVE == m_unitinfo.estate || COLLISION == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
	}
}

void CSpidermine::Render(void)
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

void CSpidermine::Inputkey_reaction(const int& nkey)
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

	if('W' == nkey)
	{
		m_eteamnumber = TEAM_1;
		//m_skill_sp->use();
	}
}

void CSpidermine::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}

void CSpidermine::Release(void)
{
	CObj::area_release();

	m_com_pathfind = NULL;
}

void CSpidermine::Dead(void)
{
	CObj* pobj = new CGeneraEff(L"SMALLBANG" , m_vPos , D3DXVECTOR2(1.f,1.f) , SORT_GROUND ,1.4f);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);
}
