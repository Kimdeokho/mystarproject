#include "StdAfx.h"
#include "Z_gas.h"

#include "TileManager.h"
#include "MyMath.h"
#include "TimeMgr.h"
#include "MyMath.h"
#include "ScrollMgr.h"
#include "Ingame_UIMgr.h"
#include "LineMgr.h"

#include "UI_Wireframe.h"
#include "FontMgr.h"
#include "Com_fog.h"
#include "Com_Creep.h"
#include "Com_ZBuildingAnim.h"
#include "Com_Collision.h"

#include "UI_Select.h"
#include "UI_Energy_bar.h"
#include "UI_Cmd_info.h"
#include "Ingame_UIMgr.h"

CZ_gas::CZ_gas(CObj* resource_gas)
{
	m_pgas_resource = resource_gas;
}

CZ_gas::~CZ_gas(void)
{
	Release();
	CIngame_UIMgr::GetInstance()->BuildTech_Update(Z_GAS , -1);
}

void CZ_gas::Initialize(void)
{
	MYRECT<float> vtx(64.f , 64.f , 32.f , 32.f);

	m_vertex.left = 64.f;
	m_vertex.right = 64.f;
	m_vertex.top =  32.f;
	m_vertex.bottom = 32.f;

	CGasBuilding::building_area_Initialize(2, 4);

	m_eOBJ_NAME = OBJ_Z_GAS;
	m_sortID = SORT_GROUND;
	m_ecategory = CATEGORY_BUILDING;

	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.state = BUILD;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_LARGE;	
	m_unitinfo.maxhp = 750;
	m_unitinfo.hp = 0;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 0;
	m_unitinfo.search_range = 0;
	m_unitinfo.fog_range = 512;
	m_unitinfo.fbuildtime = 10.f;

	m_com_anim = new CCom_ZBuildingAnim(L"Z_GAS" , m_matWorld);
	
	m_com_anim->SetTextureName(L"Z_COCOON");
	m_com_anim->SetAnimation(L"BUILD_L");

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

void CZ_gas::Update(void)
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
		m_build_hp += m_fbuild_tick * GETTIME;		

		m_com_anim->SetTextureName(L"Z_COCOON");
		m_com_anim->SetAnimation(L"BUILD_L");

		m_unitinfo.hp = (int)m_build_hp;

		if(m_unitinfo.hp >= m_unitinfo.maxhp )
			m_com_anim->SetAnimation(L"BIRTH_L");

		if(m_build_hp >= m_unitinfo.maxhp)
		{
			m_unitinfo.hp = m_unitinfo.maxhp;
			m_unitinfo.maxhp = m_unitinfo.maxhp;
			m_unitinfo.state = STATE_NONE;

			CIngame_UIMgr::GetInstance()->BuildTech_Update(Z_GAS , 1);
		}
	}

	m_select_ui->Update();
	m_energybar_ui->Update();
}

void CZ_gas::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_select_ui->Render();
	m_com_anim->Render();
	m_energybar_ui->Render();

	CLineMgr::GetInstance()->collisionbox_render(m_rect);
}
void CZ_gas::Update_Wireframe(void)
{

}
void CZ_gas::Dead(void)
{

}
void CZ_gas::Release(void)
{
	CGasBuilding::area_release();

	if(NULL != m_select_ui)
		Safe_Delete(m_select_ui);

	if(NULL != m_energybar_ui)
		Safe_Delete(m_energybar_ui);
}

