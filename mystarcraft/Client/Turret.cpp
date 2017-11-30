#include "StdAfx.h"
#include "Turret.h"

#include "Component.h"
#include "Com_TurretAnim.h"
#include "Com_Collision.h"
#include "Com_fog.h"

#include "ScrollMgr.h"
#include "MyMath.h"
#include "Area_Mgr.h"
#include "UnitMgr.h"
#include "ObjMgr.h"
#include "GeneraEff.h"

#include "UI_Select.h"
#include "LineMgr.h"
#include "FontMgr.h"
#include "Corpse.h"
#include "ComanderMgr.h"

#include "Turret_head.h"

CTurret::CTurret(void)
{
}

CTurret::~CTurret(void)
{
	Release();
}

void CTurret::Initialize(void)
{
	m_com_pathfind = NULL;

	m_vertex.left = 16.f;
	m_vertex.right = 17.f;
	m_vertex.top =  32.f;
	m_vertex.bottom = 17.f;

	CTerran_building::building_area_Initialize(2 , 2);
	CTerran_building::building_pos_Initialize(2 , 2);

	m_sortID = SORT_GROUND;	
	m_ecategory = BUILDING;
	m_eOBJ_NAME = OBJ_TURRET;
	m_eteamnumber = TEAM_0;

	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.estate = BUILD;
	m_unitinfo.eorder = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_LARGE;
	m_unitinfo.hp = 200;
	m_unitinfo.maxhp = m_unitinfo.hp;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 0;
	m_unitinfo.search_range = 0;
	m_unitinfo.fog_range = 512;
	m_unitinfo.fbuildtime = 10.f;

	m_com_anim = new CCom_TurretAnim(m_matWorld , m_unitinfo.fbuildtime);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_Collision(m_vPos , m_rect , m_vertex , false) ) ) ;	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_select_ui = new CUI_Select(L"Select62" , m_vPos , 10);
	m_select_ui->Initialize();
	CObjMgr::GetInstance()->AddSelect_UI(m_select_ui);

	CTerran_building::fire_eff_initialize();

	m_turrethead = new CTurret_head;
	m_turrethead->SetPos(m_vPos - m_weight);
	m_turrethead->Initialize();
	m_turrethead->SetState(BUILD);
}

void CTurret::Update(void)
{
	CTerran_building::building_area_update();	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();

	m_select_ui->Update();
	m_turrethead->Update();


	if(IDLE == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
	}
	else if(BUILD == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"BUILD");
	}

	CTerran_building::fire_eff_update();
}

void CTurret::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_com_anim->Render();
	m_turrethead->Render();

	CTerran_building::fire_eff_render();

	CLineMgr::GetInstance()->collisionbox_render(m_rect);
}

void CTurret::Release(void)
{
	CTerran_building::area_release();
	CUnitMgr::GetInstance()->clear_destroy_unitlist(this);

	Safe_Delete(m_turrethead);
}

void CTurret::Dead(void)
{
	CObj* pobj = new CGeneraEff(L"XLARGEBANG" , m_vPos , D3DXVECTOR2(1.f,1.f) , SORT_GROUND);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);


	pobj = new CCorpse(L"" , L"TBDSMALL_WRECKAGE");
	pobj->SetPos(m_vPos.x , m_vPos.y);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddCorpse(pobj);
}

void CTurret::Inputkey_reaction(const int& nkey)
{

}

void CTurret::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}

void CTurret::SetTurretHead(void)
{
	m_turrethead->SetState(IDLE);
}
