#include "StdAfx.h"
#include "Spore.h"

#include "TileManager.h"
#include "MyMath.h"
#include "TimeMgr.h"
#include "Ingame_UIMgr.h"
#include "MyMath.h"
#include "ScrollMgr.h"
#include "Ingame_UIMgr.h"
#include "FontMgr.h"

#include "Com_fog.h"
#include "Com_Creep.h"
#include "Com_ZBuildingAnim.h"
#include "Com_Creep.h"
#include "Com_Collision.h"

#include "UI_Wireframe.h"
#include "UI_Select.h"
#include "UI_Energy_bar.h"
#include "UI_Cmd_info.h"

CSpore::CSpore(void)
{
}

CSpore::~CSpore(void)
{
	Release();
}

void CSpore::Initialize(void)
{
	m_vertex.left = 24.f;
	m_vertex.right = 24.f;
	m_vertex.top =  24.f;
	m_vertex.bottom = 24.f;

	CZerg_building::building_area_Initialize(2 , 2);
	CZerg_building::building_pos_Initialize(2 , 2);

	m_eOBJ_NAME = OBJ_SPORE;
	m_sortID = SORT_GROUND;
	m_ecategory = CATEGORY_BUILDING;

	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.state = BUILD;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_LARGE;	
	m_unitinfo.maxhp = 300;
	m_unitinfo.hp = 0;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 0;
	m_unitinfo.search_range = 0;
	m_unitinfo.fog_range = 32*10*2;
	m_unitinfo.fbuildtime = 10.f;
	m_unitinfo.armor = 2;

	m_com_anim = new CCom_ZBuildingAnim(L"Z_SPORE" , m_matWorld);

	m_com_anim->SetTextureName(L"Z_COCOON");
	m_com_anim->SetAnimation(L"BUILD_S");


	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_Collision(m_vPos , m_rect , m_vertex , false) ) );	


	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_select_ui = new CUI_Select(L"Select72" , m_vPos , 10);
	m_select_ui->Initialize();

	m_energybar_ui = new CUI_Energy_bar(this , 62);
	m_energybar_ui->Initialize();

	m_fbuild_tick = float(m_unitinfo.maxhp)/m_unitinfo.fbuildtime;
	m_build_maxhp = m_unitinfo.maxhp;
}

void CSpore::Update(void)
{
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
		m_com_anim->SetTextureName(L"Z_COCOON");

		m_build_hp += m_fbuild_tick * GETTIME;		

		if(m_build_hp >= m_build_maxhp)
		{
			m_com_anim->SetAnimation(L"BIRTH_S");

			m_unitinfo.hp = m_unitinfo.maxhp;
			m_unitinfo.maxhp = m_unitinfo.maxhp;
			m_unitinfo.state = STATE_NONE;

			CIngame_UIMgr::GetInstance()->BuildTech_Update(Z_SPORE , 1);
		}
	}

	m_select_ui->Update();
	m_energybar_ui->Update();
	//CFontMgr::GetInstance()->Setnumber_combine_Font(L"%d" , 111 , 400 , 300);
}

void CSpore::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_select_ui->Render();

	m_com_anim->Render();

	m_energybar_ui->Render();
}

void CSpore::Inputkey_reaction(const int& nkey)
{

}

void CSpore::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}

void CSpore::Update_Cmdbtn(void)
{

}

void CSpore::Update_Wireframe(void)
{

}

void CSpore::Dead(void)
{

}

void CSpore::Release(void)
{
	CZerg_building::area_release();
}
