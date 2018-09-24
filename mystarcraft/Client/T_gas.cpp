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
#include "Ingame_UIMgr.h"
#include "TimeMgr.h"
#include "LineMgr.h"
#include "FontMgr.h"
#include "GeneraEff.h"
#include "UI_Select.h"


#include "Corpse.h"
#include "GasResource.h"

#include "UI_Wireframe.h"
#include "UI_Energy_bar.h"
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
	CIngame_UIMgr::GetInstance()->BuildTech_Update(T_GAS , -1);
}

void CT_gas::Initialize(void)
{
	m_vertex.left = 56.f;
	m_vertex.right = 57.f;
	m_vertex.top =  32.f;
	m_vertex.bottom = 32.f;

	CGasBuilding::building_area_Initialize(2, 4);

	m_sortID = SORT_GROUND;	
	m_ecategory = CATEGORY_BUILDING;
	m_eOBJ_NAME = OBJ_T_GAS;

	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.state = BUILD;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_LARGE;
	m_unitinfo.hp = 0;
	m_unitinfo.maxhp = 750;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 0;
	m_unitinfo.search_range = 0;
	m_unitinfo.fog_range = 512;
	m_unitinfo.fbuildtime = 10.f;


	m_com_anim = new CCom_T_gasAnim(m_matWorld , m_unitinfo.fbuildtime);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_Collision(m_vPos , m_rect , m_vertex , false) ) ) ;	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_select_ui = new CUI_Select(L"Select122" , m_vPos , 13);
	m_select_ui->Initialize();

	m_energybar_ui = new CUI_Energy_bar(this , 122);
	m_energybar_ui->Initialize();

	m_fbuild_tick = float(m_unitinfo.maxhp)/m_unitinfo.fbuildtime;
}

void CT_gas::Update(void)
{
	CGasBuilding::Update();

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();


	if(IDLE == m_unitinfo.state)
	{
		m_com_anim->SetAnimation(L"IDLE");
	}
	else if(BUILD == m_unitinfo.state)
	{
		m_com_anim->SetAnimation(L"BUILD");

		m_build_hp += m_fbuild_tick * GETTIME;
		m_unitinfo.hp = (int)m_build_hp;

		if(m_unitinfo.hp >= m_unitinfo.maxhp )
		{
			m_unitinfo.hp = m_unitinfo.maxhp;
			m_unitinfo.state = IDLE;
			CIngame_UIMgr::GetInstance()->BuildTech_Update(T_GAS , 1);
		}
	}


	D3DXVECTOR2 vpos = CMyMath::index_to_Pos(m_curidx32 , 128 , 32);
	vpos.x -= CScrollMgr::m_fScrollX;
	vpos.y -= CScrollMgr::m_fScrollY;
	CFontMgr::GetInstance()->Setbatch_Font(L"@" , vpos.x , vpos.y);

	m_select_ui->Update();
	m_energybar_ui->Update();
}

void CT_gas::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_select_ui->Render();
	m_com_anim->Render();
	m_energybar_ui->Render();

	CLineMgr::GetInstance()->collisionbox_render(m_rect);
}


void CT_gas::Update_Wireframe(void)
{
	D3DXVECTOR2 interface_pos = CIngame_UIMgr::GetInstance()->GetMainInterface_pos();

	if(true == CIngame_UIMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.state))
	{		
		CUI* pui = NULL;
		pui = new CUI_Wireframe(L"WIRE_T_GAS" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));
		pui->Initialize();
		CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);

		CFontMgr::GetInstance()->SetInfomation_font(L"Terran Gas" ,interface_pos.x + 320 , interface_pos.y + 390 );

		if(BUILD == m_unitinfo.state)
		{
			CFontMgr::GetInstance()->SetInfomation_font(L"Under construction.." , interface_pos.x + 320 , interface_pos.y + 415);
		}
	}


	D3DCOLOR font_color;

	int iratio = m_unitinfo.maxhp / m_unitinfo.hp;

	if( iratio <= 1)
		font_color = D3DCOLOR_ARGB(255,0,255,0);
	else if( 1 < iratio && iratio <= 2)
		font_color = D3DCOLOR_ARGB(255,255,255,0);
	else if( 2 < iratio)
		font_color = D3DCOLOR_ARGB(255,255,0,0);

	CFontMgr::GetInstance()->Setbatch_Font(L"%d/%d" , m_unitinfo.hp , m_unitinfo.maxhp,
		interface_pos.x + 195 , interface_pos.y + 460 , font_color);

	if(BUILD == m_unitinfo.state)
	{		
		CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 260 , interface_pos.y + 435) , m_build_hp / (float)m_unitinfo.maxhp );
	}
}
void CT_gas::Release(void)
{
	CGasBuilding::area_release();

	if(NULL != m_select_ui)
		Safe_Delete(m_select_ui);

	if(NULL != m_energybar_ui)
		Safe_Delete(m_energybar_ui);
	//if( NULL != m_pgas_resource)
	//{
	//	((CGasResource*)m_pgas_resource)->building_area_Initialize();
	//	m_pgas_resource = NULL;
	//}
}

void CT_gas::Dead(void)
{
	CObj* pobj = new CGeneraEff(L"XLARGEBANG" , m_vPos , D3DXVECTOR2(1.f,1.f) , SORT_GROUND);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);

	CUnitMgr::GetInstance()->clear_destroy_unitlist(this);

	if( NULL != m_pworkman)
	{
		m_pworkman->SetActive(true);
		m_pworkman->SetOrder(ORDER_NONE);
		m_pworkman->SetState(IDLE);
		m_pworkman->unit_area_Initialize();
		m_pworkman = NULL;
	}

	if( NULL != m_pgas_resource)
	{
		((CGasResource*)m_pgas_resource)->building_area_Initialize();
		m_pgas_resource = NULL;
	}
}