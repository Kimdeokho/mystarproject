#include "StdAfx.h"
#include "Battle_addon.h"

#include "Component.h"
#include "Com_PartAnim.h"
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
#include "TimeMgr.h"
#include "ComanderMgr.h"
#include "Comandcenter.h"
CBattle_addon::CBattle_addon(CObj* pobj)
{
	m_mainbuilding = pobj;
}

CBattle_addon::~CBattle_addon(void)
{
	Release();
}

void CBattle_addon::Initialize(void)
{
	m_com_pathfind = NULL;

	CTerran_building::building_area_Initialize(2 , 2);
	CTerran_building::building_pos_Initialize(2 , 2);

	m_vertex.left = 32.f;//원래 37이던데 이유는 흠...
	m_vertex.right = 29.f;
	m_vertex.top =  24.f;
	m_vertex.bottom = 23.f;

	m_sortID = SORT_GROUND;	
	m_ecategory = BUILDING;
	m_eOBJ_NAME = OBJ_GHOST_ADDON;
	m_eteamnumber = TEAM_0;

	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.estate = BUILD;
	m_unitinfo.eorder = ORDER_NONE;
	m_unitinfo.eDamageType = DAMAGE_NOMAL;
	m_unitinfo.eArmorType = ARMOR_LARGE;
	m_unitinfo.damage = 0;
	m_unitinfo.hp = 1;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 0;
	m_unitinfo.attack_range = 0;
	m_unitinfo.search_range = 0;
	m_unitinfo.fog_range = 512;
	m_unitinfo.fbuildtime = 1.f;


	m_com_anim = new CCom_PartAnim(L"T_BATTLE_ADDON", L"BATTLE_LINK" , m_matWorld );

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_Collision(m_vPos , m_rect , m_vertex , false) ) ) ;	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_select_ui = new CUI_Select(L"Select94" , m_vPos , 13);
	m_select_ui->Initialize();
	CObjMgr::GetInstance()->AddSelect_UI(m_select_ui);

	CTerran_building::fire_eff_initialize();
}

void CBattle_addon::Update(void)
{
	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();

	m_select_ui->Update();

	if(IDLE == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
	}
	else if(BUILD == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"BUILD");
	}

	D3DXVECTOR2 vpos;
	vpos = CMyMath::index_to_Pos(m_curidx32 , 128 , 32);
	vpos.x -= CScrollMgr::m_fScrollX;
	vpos.y -= CScrollMgr::m_fScrollY;
	CFontMgr::GetInstance()->Setbatch_Font(L"@" , m_vPos.x - CScrollMgr::m_fScrollX, 
		m_vPos.y - CScrollMgr::m_fScrollY);

	CTerran_building::fire_eff_update();
}

void CBattle_addon::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_com_anim->Render();

	CTerran_building::fire_eff_render();

	CLineMgr::GetInstance()->collisionbox_render(m_rect);
}

void CBattle_addon::Release(void)
{
	CTerran_building::area_release();
	CUnitMgr::GetInstance()->clear_destroy_unitlist(this);
}

void CBattle_addon::Dead(void)
{
	CObj* pobj = new CGeneraEff(L"XLARGEBANG" , m_vPos , D3DXVECTOR2(1.f,1.f) , SORT_GROUND);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);

	pobj = new CCorpse(L"" , L"TBDSMALL_WRECKAGE");
	pobj->SetPos(m_vPos.x , m_vPos.y);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddCorpse(pobj);

	((CTerran_building*)m_mainbuilding)->SetPartBuilding(NULL);
}

void CBattle_addon::Inputkey_reaction(const int& nkey)
{

}

void CBattle_addon::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}

void CBattle_addon::Setlink(bool blink)
{
	if(true == blink)
	{
		((CCom_PartAnim*)m_com_anim)->play_direction(1);
	}
	else
	{
		((CCom_PartAnim*)m_com_anim)->play_direction(-1);
	}
}
