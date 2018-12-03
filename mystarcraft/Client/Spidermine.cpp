#include "StdAfx.h"
#include "Spidermine.h"

#include "ScrollMgr.h"
#include "TimeMgr.h"
#include "MouseMgr.h"

#include "Com_fog.h"
#include "Com_Pathfind.h"
#include "Com_MineAnim.h"
#include "Com_Minesearch.h"
#include "Com_WMine.h"
#include "Com_CC.h"

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

#include "UI_Energy_bar.h"
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
	m_ecategory = CATEGORY_UNIT;
	m_eOBJ_NAME = OBJ_VULTURE;

	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.state = IDLE;
	m_unitinfo.esize = SIZE_SMALL;
	m_unitinfo.erace = OBJRACE_MECHANIC;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_SMALL;
	m_unitinfo.hp = 20;
	m_unitinfo.maxhp = m_unitinfo.hp;
	m_unitinfo.mp = 0;
	m_unitinfo.maxmp = 0;
	m_unitinfo.fspeed = 400;
	m_unitinfo.attack_range = 0*32;
	m_unitinfo.air_attack_range = 0*32;
	m_unitinfo.search_range = 3*32;
	m_unitinfo.fog_range = 3*32;
	m_unitinfo.eAttackType = ATTACK_ONLY_GROUND;


	m_vertex.left = 7.5;
	m_vertex.right = 7.5;
	m_vertex.top =  7.5;
	m_vertex.bottom = 7.5;

	m_com_anim = new CCom_MineAnim(m_matWorld);
	m_com_collision = new CCom_Collision(m_vPos , m_rect , m_vertex , false);
	m_com_cc = new CCom_CC();

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_CC ,  m_com_cc )) ;	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));		
	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , m_com_collision ) ) ;
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_WEAPON ,  new CCom_WMine()) );


	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);


	m_select_ui = new CUI_Select(L"Select22" , m_vPos , 4);
	m_select_ui->Initialize();

	m_energybar_ui = new CUI_Energy_bar(this , 22);
	m_energybar_ui->Initialize();

	m_plant_time = 0.f;
	m_bplant_init = true;
	m_unitinfo.is_active = false;
	m_unitinfo.is_hide = false;
}

void CSpidermine::Update(void)
{

	if(m_bplant_init)
		m_plant_time += GETTIME;

	if(m_plant_time > 1.0f)
	{
		if(BURROW == m_unitinfo.state)
		{
			for(int i = 0; i < TEAM_END; ++i)
			{
				if(i != m_eteamnumber)
					m_unitinfo.detect[i] -= 1;
			}
			m_unitinfo.is_hide = true;

			m_com_pathfind = new CCom_Pathfind(m_vPos , m_rect , 32 ,16);
			m_com_targetsearch = new CCom_Minesearch(SEARCH_ONLY_ENEMY);
			m_componentlist.insert(COMPONENT_PAIR::value_type(COM_TARGET_SEARCH , m_com_targetsearch ));
			m_componentlist.insert(COMPONENT_PAIR::value_type(COM_PATHFINDE , m_com_pathfind ));	

			m_com_pathfind->Initialize(this);
			m_com_targetsearch->Initialize(this);
			m_unitinfo.is_active = true;

			m_bplant_init = false;
			m_plant_time = 0.f;

			return;
		}
		m_unitinfo.state = PLANT;

		m_sortID = SORT_CORPSE;
	}


	CObj::area_update();	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();


	if(IDLE == m_unitinfo.state)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
		m_sortID = SORT_GROUND;
	}
	else if(PLANT == m_unitinfo.state)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"PLANT");
	}
	else if(BURROW == m_unitinfo.state)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"BURROW");
	}
	else if(PULL == m_unitinfo.state)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"PULL");
		m_sortID = SORT_GROUND;

		for(int i = 0; i < TEAM_END; ++i)
			m_unitinfo.detect[i] += 1;
	}
	else if(MOVE == m_unitinfo.state)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"MOVE");
		m_sortID = SORT_GROUND;
	}

	m_select_ui->Update();
	m_energybar_ui->Update();
}

void CSpidermine::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_select_ui->Render();
	m_com_anim->Render();
	m_com_cc->Render();
	m_energybar_ui->Render();

	//m_com_pathfind->Render();
	CLineMgr::GetInstance()->collisionbox_render(m_rect);
}

void CSpidermine::Inputkey_reaction(const int& nkey)
{
}

void CSpidermine::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}

void CSpidermine::Release(void)
{
	CObj::area_release();

}

void CSpidermine::Dead(void)
{
	CObj* pobj = new CGeneraEff(L"MINE_BOOM" , m_vPos , D3DXVECTOR2(1.f,1.f) , SORT_GROUND ,2);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);
}
