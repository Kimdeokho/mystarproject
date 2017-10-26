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
	m_unitinfo.eDamageType = DAMAGE_NOMAL;
	m_unitinfo.eArmorType = ARMOR_SMALL;
	m_unitinfo.damage = 6;
	m_unitinfo.hp = 40;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 60;
	m_unitinfo.attack_range = 128;
	m_unitinfo.search_range = 255;
	m_unitinfo.fog_range = 512;


	m_vertex.left = 8.5;
	m_vertex.right = 8.5;
	m_vertex.top =  10;
	m_vertex.bottom = 10;

	//

	m_com_pathfind = new CCom_Pathfind(m_vPos , m_rect , 32 ,16);
	m_com_weapon = new CCom_Wmarine(m_unitinfo.damage , DAMAGE_NOMAL);
	m_com_targetsearch = new CCom_Distancesearch(&m_unitinfo.attack_range , &m_unitinfo.search_range, SEARCH_ONLY_ENEMY);
	m_com_anim = new CCom_MarineAnim(m_matWorld , m_curtex);
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
	CObjMgr::GetInstance()->AddSelect_UI(m_select_ui);

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
	if(NULL == m_curtex)
		return;

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;
	//m_pSprite->SetTransform(&m_matshadow);
	//m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0)
	//	, NULL , D3DCOLOR_ARGB(125,0,0,0));
	

	m_com_anim->Render();
	//m_pSprite->SetTransform(&m_matWorld);
	//if(TEAM_1 == m_eteamnumber)
	//{
	//	m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0)
	//		, NULL , D3DCOLOR_ARGB(255,255,0,0));
	//}
	//else
	//{
	//	m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0)
	//		, NULL , D3DCOLOR_ARGB(255,255,255,255));
	//}

	m_com_pathfind->Render();
	//CLineMgr::GetInstance()->collisionbox_render(m_rect);
}

void CMarine::Inputkey_reaction(const int& nkey)
{
	if(VK_RBUTTON == nkey)
	{
		m_unitinfo.estate = MOVE;
		m_unitinfo.eorder = ORDER_MOVE;

		if(NULL != m_com_pathfind)
		{
			D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();


			((CCom_Pathfind*)m_com_pathfind)->SetGoalPos(goalpos);
			((CCom_Pathfind*)m_com_pathfind)->SetGoalidx(CMyMath::Pos_to_index(goalpos ,32));
			((CCom_Pathfind*)m_com_pathfind)->SetFlowField();
			((CCom_Pathfind*)m_com_pathfind)->StartPathfinding(m_bmagicbox);
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
		m_skill_sp->use();
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
	if('A' == firstkey && VK_LBUTTON == secondkey)
	{
		m_unitinfo.eorder = ORDER_MOVE_ATTACK;
		m_unitinfo.estate = MOVE;
		//m_eorder = ORDER_MOVE_ATTACK;
		//m_estate = MOVE;
		((CCom_Targetsearch*)m_com_targetsearch)->SetTarget(CArea_Mgr::GetInstance()->GetChoiceTarget());

		if(NULL != m_com_pathfind)
		{
			D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();


			((CCom_Pathfind*)m_com_pathfind)->SetGoalPos(goalpos);
			((CCom_Pathfind*)m_com_pathfind)->SetGoalidx(CMyMath::Pos_to_index(goalpos ,32));
			((CCom_Pathfind*)m_com_pathfind)->SetFlowField();
			((CCom_Pathfind*)m_com_pathfind)->StartPathfinding(m_bmagicbox);
			m_bmagicbox = false;
		}

	}
}

void CMarine::Release(void)
{

	CUnit::area_release();
	CUnitMgr::GetInstance()->clear_destroy_unitlist(this);

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
