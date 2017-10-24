#include "StdAfx.h"
#include "T_gas.h"

#include "Component.h"
#include "Com_T_gasAnim.h"
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
#include "GasResource.h"
CT_gas::CT_gas(void)
{
}

CT_gas::CT_gas(CObj* resource_gas)
{
	m_pgas_resource = resource_gas;
}

CT_gas::~CT_gas(void)
{
	Release();
}

void CT_gas::Initialize(void)
{
	m_vertex.left = 56.f;
	m_vertex.right = 57.f;
	m_vertex.top =  32.f;
	m_vertex.bottom = 32.f;

	CGasBuilding::building_area_Initialize(2, 4);

	m_sortID = SORT_GROUND;	
	m_ecategory = BUILDING;
	m_eOBJ_NAME = OBJ_GASBUILDING;
	m_eteamnumber = TEAM_0;

	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.estate = BUILD;
	m_unitinfo.eorder = ORDER_NONE;
	m_unitinfo.eDamageType = DAMAGE_NOMAL;
	m_unitinfo.eArmorType = ARMOR_SMALL;
	m_unitinfo.damage = 0;
	m_unitinfo.hp = 1;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 0;
	m_unitinfo.attack_range = 0;
	m_unitinfo.search_range = 0;
	m_unitinfo.fog_range = 512;

	float buildtime = 1.f;

	m_com_anim = new CCom_T_gasAnim(m_matWorld , m_curtex , buildtime);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_Collision(m_vPos , m_rect , m_vertex , false) ) ) ;	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_select_ui = new CUI_Select(L"Select122" , m_vPos , 13);
	m_select_ui->Initialize();
	CObjMgr::GetInstance()->AddSelect_UI(m_select_ui);
}

void CT_gas::Update(void)
{
	CGasBuilding::Update();

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


	D3DXVECTOR2 vpos = CMyMath::index_to_Pos(m_curidx32 , 128 , 32);
	vpos.x -= CScrollMgr::m_fScrollX;
	vpos.y -= CScrollMgr::m_fScrollY;
	CFontMgr::GetInstance()->Setbatch_Font(L"@" , vpos.x , vpos.y);
}

void CT_gas::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_matshadow = m_matWorld;
	m_matshadow._41 -= 8;
	m_matshadow._42 -= 8;

	m_pSprite->SetTransform(&m_matshadow);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0) , NULL , D3DCOLOR_ARGB(100,0,0,0));

	m_pSprite->SetTransform(&m_matWorld);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0)
		, NULL , D3DCOLOR_ARGB(255,255,255,255));

	CLineMgr::GetInstance()->collisionbox_render(m_rect);
}

void CT_gas::Release(void)
{
	CGasBuilding::area_release();

	CUnitMgr::GetInstance()->clear_destroy_unitlist(this);
}

void CT_gas::Dead(void)
{
	CObj* pobj = new CGeneraEff(L"XLARGEBANG" , m_vPos , D3DXVECTOR2(1.f,1.f) , SORT_GROUND);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);

	if( NULL != m_pgas_resource)
	{
		((CGasResource*)m_pgas_resource)->building_area_Initialize();
		m_pgas_resource = NULL;
	}

	if( NULL != m_pworkman)
	{
		m_pworkman->SetOrder(ORDER_NONE);
		m_pworkman->SetState(IDLE);
		m_pworkman->unit_area_Initialize();
	}
}
