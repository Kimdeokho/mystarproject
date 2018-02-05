#include "StdAfx.h"
#include "Tank.h"

#include "Tankbody.h"
#include "Tankbarrel.h"

#include "Siegebody.h"
#include "Siegebarrel.h"


#include "Com_fog.h"
#include "Com_Collision.h"
#include "Com_Pathfind.h"
#include "Com_TankbodyAnim.h"
#include "Com_SiegebodyAnim.h"

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

#include "ComanderMgr.h"

CTank::CTank(void)
{
	m_tankbarrel = NULL;

}

CTank::~CTank(void)
{
	Release();
}

void CTank::Initialize(void)
{
	m_vcurdir = CMyMath::dgree_to_dir(2*22.5f);

	CObj::unit_area_Initialize();
	CUnit::Initialize();

	m_sortID = SORT_GROUND;	
	m_ecategory = UNIT;
	m_eteamnumber = TEAM_0;
	m_eOBJ_NAME = OBJ_TANK;


	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.estate = IDLE;
	m_unitinfo.eorder = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_LARGE;
	m_unitinfo.hp = 150;
	m_unitinfo.maxhp = 150;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 68;
	m_unitinfo.fog_range = 512;

	m_vertex.left = 16;
	m_vertex.right = 16;
	m_vertex.top =  16;
	m_vertex.bottom = 16;

	m_bsiegemode = false;
	m_btransform_ready = false;


	m_tankbarrel = new CTankbarrel(this);
	m_tankbarrel->SetPos(m_vPos);
	m_tankbarrel->SetObjID(m_obj_id);



	m_com_pathfind = new CCom_Pathfind(m_vPos , m_rect , 32 , 32);
	m_com_collision = new CCom_Collision(m_vPos , m_rect , m_vertex , true);
	m_com_anim = new CCom_TankbodyAnim(m_matWorld);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , m_com_collision ) ) ;	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_PATHFINDE ,  m_com_pathfind) );

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);


	m_tankbarrel->Initialize();//포신은 제일 마지막에 초기화

	m_select_ui = new CUI_Select(L"Select62" , m_vPos , 8);
	m_select_ui->Initialize();
	CObjMgr::GetInstance()->AddSelect_UI(m_select_ui , MOVE_GROUND);
}

void CTank::Update(void)
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
		//m_tankbarrel->SetState(IDLE); //아 이거 문젠데....
	}
	else if(ATTACK == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
	}
	else if(MOVE == m_unitinfo.estate || COLLISION == m_unitinfo.estate)
	{
		if(true == m_bsiegemode )
		{
		}
		else
		{
			((CCom_Animation*)m_com_anim)->SetAnimation(L"MOVE");
			m_tankbarrel->Setdir(m_vcurdir);
		}
	}
	else if(TRANSFORMING == m_unitinfo.estate)
	{
		if(true == m_btransform_ready)
		{
			if(true == ((CCom_Animation*)m_com_anim)->GetAnimation_end())
			{
				m_unitinfo.estate = IDLE;

				if(false == m_bsiegemode)
				{
					m_bsiegemode = true;
					((CTankbarrel*)m_tankbarrel)->TransformSiegebarrel();					
				}
				else
				{
					//시즈모드 상태였다면
					m_bsiegemode = false;
					Transform_Tankbody();
					((CTankbarrel*)m_tankbarrel)->TransformTankbarrel();
				}
				//CUnitMgr::GetInstance()->Update_Cmdbtn();
				m_btransform_ready = false;
			}			
		}
		else
		{
			if(false == m_bsiegemode)
			{
				if( true == ((CCom_Animation*)m_com_anim)->GetRotationComplete() &&
					true == ((CTankbarrel*)m_tankbarrel)->GetTransformReady())
				{
					m_btransform_ready = true; // 변신 준비완료
					Transform_Siegebody();
				}
			}
		}

	}

	if(ORDER_NONE == m_unitinfo.eorder)
	{
		m_tankbarrel->SetOrder(ORDER_NONE);
	}

	barrel_sync();
	m_tankbarrel->Update();

}

void CTank::Render(void)
{
	if( BOARDING == m_unitinfo.estate )
		return;

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;


	m_com_anim->Render();

	m_tankbarrel->Render();

	if(NULL != m_com_pathfind)
		m_com_pathfind->Render();
}
void CTank::Transform(void)
{
	((CCom_Animation*)m_com_anim)->SetAnimation(L"SIEGEBODY_TRANS");
}
void CTank::Transform_Tankbody(void)
{
	//역변신
	COMPONENT_PAIR::iterator iter = m_componentlist.find(COM_ANIMATION);
	Safe_Delete(m_com_anim);
	m_componentlist.erase(iter);


	m_com_anim = new CCom_TankbodyAnim(m_matWorld);
	m_com_anim->Initialize(this);
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim));

	m_com_pathfind = new CCom_Pathfind(m_vPos , m_rect , 32, 32);
	m_com_pathfind->Initialize(this);
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_PATHFINDE , m_com_pathfind));

	((CCom_Collision*)m_com_collision)->SetCollsearch(true);

	//변신 끝
	m_btransform_ready = false;

	m_eOBJ_NAME = OBJ_TANK;
	m_tankbarrel->SetObjName(OBJ_TANK);
	
}

void CTank::Transform_Siegebody(void)
{
	COMPONENT_PAIR::iterator iter = m_componentlist.find(COM_PATHFINDE);
	m_componentlist.erase(iter);			

	iter = m_componentlist.find(COM_ANIMATION);
	m_componentlist.erase(iter);

	Safe_Delete(m_com_pathfind);
	Safe_Delete(m_com_anim);

	m_com_anim = new CCom_SiegebodyAnim(m_matWorld );
	m_com_anim->Initialize(this);
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim));

	((CCom_Collision*)m_com_collision)->SetCollsearch(false);

	m_eOBJ_NAME = OBJ_SIEGETANK;
	m_tankbarrel->SetObjName(OBJ_SIEGETANK);
}

void CTank::barrel_sync(void)
{
	D3DXVECTOR2 vbarrelpos;
	
	vbarrelpos = m_vPos - m_vcurdir*8;
	vbarrelpos.y -= 5;
	((CTankbarrel*)m_tankbarrel)->SetbarrelPos(m_vPos , vbarrelpos);

}
void CTank::Inputkey_reaction(const int& nkey)
{
	if( 'O' == nkey)
	{
		//시즈모드 업글이 되어있따면

		if(TRANSFORMING != m_unitinfo.estate)
		{
			m_unitinfo.estate = TRANSFORMING;
			if(false == m_bsiegemode)
			{				
				m_vcurdir = CMyMath::dgree_to_dir(2*22.5f);
			}
		}

	}
	if( 'W' == nkey)
	{
		m_eteamnumber = TEAM_1;
		m_tankbarrel->SetTeamNumber(TEAM_1);
	}
	if(VK_RBUTTON == nkey)
	{
		if(true == m_bsiegemode)
		{
		}
		else
		{
			if(TRANSFORMING != m_unitinfo.estate)
			{
				m_unitinfo.estate = MOVE;
				m_unitinfo.eorder = ORDER_MOVE;
				m_tankbarrel->SetOrder(ORDER_MOVE);
				m_tankbarrel->SetState(MOVE);

				if(NULL != m_com_pathfind)
				{
					CObj* ptarget = CArea_Mgr::GetInstance()->GetChoiceTarget();
					if(NULL == ptarget)
						m_bmagicbox = true;
					else
						m_bmagicbox = false;

					D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();

					((CCom_Pathfind*)m_com_pathfind)->SetGoalPos(goalpos , m_bmagicbox);
					((CCom_Pathfind*)m_com_pathfind)->SetFlowField();
					((CCom_Pathfind*)m_com_pathfind)->StartPathfinding();
					m_bmagicbox = false;
				}
			}

		}
	}

	m_tankbarrel->Inputkey_reaction(nkey);
}

void CTank::Inputkey_reaction(const int& firstkey , const int& secondkey)
{
	if('A' == firstkey && VK_LBUTTON == secondkey)
	{
		if(true == m_bsiegemode)
		{

		}
		else
		{
			if(TRANSFORMING != m_unitinfo.estate)
			{
				m_unitinfo.eorder = ORDER_MOVE_ATTACK;
				m_unitinfo.estate = MOVE;
				m_tankbarrel->SetOrder(ORDER_MOVE_ATTACK);
				m_tankbarrel->SetState(MOVE);				

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

	}

	m_tankbarrel->Inputkey_reaction(firstkey , secondkey);
}
bool CTank::GetSiegemode(void)
{
	return m_bsiegemode;
}
bool CTank::GetTransformReady(void)
{
	return m_btransform_ready;
}
void CTank::SetTransformReady(bool btransform_ready)
{
	m_btransform_ready = btransform_ready;
}
void CTank::Release(void)
{
	CObj::area_release();

	CUnitMgr::GetInstance()->clear_destroy_unitlist(this);

	Safe_Delete(m_tankbarrel);
}

void CTank::Dead(void)
{
	CObj* pobj = new CGeneraEff(L"LARGEBANG" , m_vPos , D3DXVECTOR2(0.85f,0.85f) , SORT_GROUND );
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);
}
void CTank::Update_Cmdbtn(void)
{
	CComanderMgr::GetInstance()->Create_Cmdbtn(0 , L"BTN_MOVE" , BTN_MOVE);
	CComanderMgr::GetInstance()->Create_Cmdbtn(1 , L"BTN_STOP" , BTN_STOP);
	CComanderMgr::GetInstance()->Create_Cmdbtn(2 , L"BTN_ATTACK" , BTN_ATTACK);
	CComanderMgr::GetInstance()->Create_Cmdbtn(3 , L"BTN_PATROL" , BTN_PATROL);
	CComanderMgr::GetInstance()->Create_Cmdbtn(4 , L"BTN_HOLD" , BTN_HOLD);

	if(true == m_bsiegemode)
	{
		CComanderMgr::GetInstance()->Create_Cmdbtn(6 , L"BTN_SIEGETANK" , BTN_SIEGETANK);
	}
	else
	{
		CComanderMgr::GetInstance()->Create_Cmdbtn(6 , L"BTN_SIEGEMODE" , BTN_SIEGEMODE);
	}
}
