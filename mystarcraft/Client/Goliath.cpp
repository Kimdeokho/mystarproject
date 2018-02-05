#include "StdAfx.h"
#include "Goliath.h"

#include "Tankbody.h"
#include "Tankbarrel.h"

#include "Siegebody.h"
#include "Siegebarrel.h"

#include "Com_fog.h"
#include "Com_Collision.h"
#include "Com_Pathfind.h"
#include "Com_GollegAnim.h"

#include "TileManager.h"

#include "ScrollMgr.h"
#include "LineMgr.h"
#include "FontMgr.h"
#include "MyMath.h"
#include "UnitMgr.h"
#include "GeneraEff.h"
#include "Area_Mgr.h"
#include "ObjMgr.h"
#include "UI_Select.h"

#include "Goliath_part.h"
CGoliath::CGoliath(void)
{
}

CGoliath::~CGoliath(void)
{
	Release();
}

void CGoliath::Initialize(void)
{
	m_vcurdir = CMyMath::dgree_to_dir(2*22.5f);

	CObj::unit_area_Initialize();
	CUnit::Initialize();

	m_sortID = SORT_GROUND;	
	m_ecategory = UNIT;
	m_eteamnumber = TEAM_0;
	m_eOBJ_NAME = OBJ_GOLIATH;

	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.estate = IDLE;
	m_unitinfo.eorder = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_LARGE;
	m_unitinfo.hp = 125;
	m_unitinfo.maxhp = m_unitinfo.hp;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 77;
	m_unitinfo.fog_range = 512;

	m_vertex.left = 16;
	m_vertex.right = 16;
	m_vertex.top =  16;
	m_vertex.bottom = 16;

	m_pgoliath_arm = new CGoliath_part(this);
	m_pgoliath_arm->SetPos(m_vPos);
	m_pgoliath_arm->SetObjID(m_obj_id);

	m_com_pathfind = new CCom_Pathfind(m_vPos , m_rect , 32 , 32);
	m_com_collision = new CCom_Collision(m_vPos , m_rect , m_vertex , true);
	m_com_anim = new CCom_GollegAnim(m_matWorld);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , m_com_collision ) ) ;	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_PATHFINDE ,  m_com_pathfind) );
	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_pgoliath_arm->Initialize();

	m_select_ui = new CUI_Select(L"Select48" , m_vPos , 8);
	m_select_ui->Initialize();
	CObjMgr::GetInstance()->AddSelect_UI(m_select_ui , MOVE_GROUND);
}

void CGoliath::Update(void)
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
		m_pgoliath_arm->SetState(IDLE);
	}
	else if(MOVE == m_unitinfo.estate || COLLISION == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"MOVE");
		m_pgoliath_arm->Setdir(m_vcurdir);
		m_pgoliath_arm->SetState(MOVE);
	}

	if(ORDER_NONE == m_unitinfo.eorder)
	{
		m_pgoliath_arm->SetOrder(ORDER_NONE);
	}

	sync_arm();
	m_pgoliath_arm->Update();	
}

void CGoliath::Render(void)
{
	if( BOARDING == m_unitinfo.estate )
		return;

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;


	m_com_anim->Render();

	m_pgoliath_arm->Render();

	if(NULL != m_com_pathfind)
		m_com_pathfind->Render();
}

void CGoliath::Release(void)
{
	CObj::area_release();

	CUnitMgr::GetInstance()->clear_destroy_unitlist(this);

	Safe_Delete(m_pgoliath_arm);
}

void CGoliath::Inputkey_reaction(const int& nkey)
{
	if( 'W' == nkey)
	{
		m_eteamnumber = TEAM_1;
		m_pgoliath_arm->SetTeamNumber(TEAM_1);
	}
	if(VK_RBUTTON == nkey)
	{
			if(TRANSFORMING != m_unitinfo.estate)
			{
				m_unitinfo.estate = MOVE;
				m_unitinfo.eorder = ORDER_MOVE;
				m_pgoliath_arm->SetOrder(ORDER_MOVE);
				m_pgoliath_arm->SetState(MOVE);

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

	m_pgoliath_arm->Inputkey_reaction(nkey);
}

void CGoliath::Inputkey_reaction(const int& firstkey , const int& secondkey)
{
	if('A' == firstkey && VK_LBUTTON == secondkey)
	{

		m_unitinfo.eorder = ORDER_MOVE_ATTACK;
		m_unitinfo.estate = MOVE;
		m_pgoliath_arm->SetOrder(ORDER_MOVE_ATTACK);
		m_pgoliath_arm->SetState(MOVE);				

		if(NULL != m_com_pathfind)
		{
			D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();

			((CCom_Pathfind*)m_com_pathfind)->SetGoalPos(goalpos , m_bmagicbox);
			((CCom_Pathfind*)m_com_pathfind)->SetFlowField();
			((CCom_Pathfind*)m_com_pathfind)->StartPathfinding();
			m_bmagicbox = false;
		}
	}

	m_pgoliath_arm->Inputkey_reaction(firstkey , secondkey);
}

void CGoliath::Dead(void)
{
	CObj* pobj = new CGeneraEff(L"SMALLBANG" , m_vPos , D3DXVECTOR2(0.85f,0.85f) , SORT_GROUND );
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);
}

void CGoliath::sync_arm(void)
{
	((CGoliath_part*)m_pgoliath_arm)->setarm_pos(m_vPos);
}
