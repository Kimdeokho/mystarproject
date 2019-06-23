#include "StdAfx.h"
#include "Lurker_egg.h"

#include "ScrollMgr.h"
#include "TimeMgr.h"
#include "MouseMgr.h"
#include "Area_Mgr.h"
#include "KeyMgr.h"
#include "UnitMgr.h"
#include "Ingame_UIMgr.h"
#include "MyMath.h"
#include "ObjMgr.h"
#include "LineMgr.h"
#include "ObjMgr.h"
#include "FontMgr.h"
#include "Session_Mgr.h"

#include "Com_fog.h"
#include "Com_LurkerEggAnim.h"
#include "Com_CC.h"
#include "Com_Collision.h"
#include "Com_Larvahatch.h"
#include "Com_AirPathfind.h"
#include "Com_Pathfind.h"

#include "UI_Select.h"
#include "UI_Energy_bar.h"
#include "Building_Preview.h"
#include "GeneraEff.h"

#include "Input_Interface.h"

#include "MyCmd_Building.h"
#include "UI_Cmd_info.h"
#include "UI_Wireframe.h"
#include "Corpse.h"

#include "Lurker.h"

CLurker_egg::CLurker_egg(void)
{
}

CLurker_egg::~CLurker_egg(void)
{
	Release();
}

void CLurker_egg::Initialize(void)
{
	CObj::unit_area_Initialize();

	m_eOBJ_NAME = OBJ_LARVA_EGG;
	m_sortID = SORT_GROUND;
	m_ecategory = CATEGORY_UNIT;

	m_unitinfo.eAttackType = ATTACK_ONLY_GROUND;
	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.esize = SIZE_SMALL;
	m_unitinfo.erace = OBJRACE_CREATURE;
	m_unitinfo.state = MUTATE;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_SMALL;
	m_unitinfo.maxhp = 100;
	m_unitinfo.hp = 100;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 22;
	m_unitinfo.attack_range = 0*32;
	m_unitinfo.air_attack_range = 0*32;
	m_unitinfo.search_range = 0*32;
	m_unitinfo.fog_range = 32*4*2;
	m_unitinfo.armor = 10;

	m_vertex.left = 16.f;
	m_vertex.right = 16.f;
	m_vertex.top =  16.f;
	m_vertex.bottom = 16.f;


	m_rect.left = m_vPos.x - m_vertex.left; 
	m_rect.right = m_vPos.x + m_vertex.right;
	m_rect.top = m_vPos.y - m_vertex.top;
	m_rect.bottom = m_vPos.y + m_vertex.bottom;


	m_com_anim = new CCom_LurkerEggAnim(m_matWorld );
	m_com_cc = new CCom_CC();

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_CC ,  m_com_cc ));	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range)));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim));	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_Collision(m_vPos , m_rect , m_vertex, false)) ) ;	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_select_ui = new CUI_Select(L"Select22" , m_vPos , 13);
	m_select_ui->Initialize();

	m_energybar_ui = new CUI_Energy_bar(this , 22 , 32);
	m_energybar_ui->Initialize();

	m_create_cnt = 1;
	m_build_hp = 0;
	m_build_maxhp = 10;


	if(m_eteamnumber == CSession_Mgr::GetInstance()->GetTeamNumber())
	{
		CUnitMgr::GetInstance()->SetUnit(this);
		SetSelect(GENERAL_SELECT );
	}
}

void CLurker_egg::Update(void)
{
	CObj::area_update();

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();

	m_build_hp += GETTIME;

	if(IDLE == m_unitinfo.state)
	{
		//m_com_anim->SetAnimation(L"IDLE");			

		if(m_build_hp >= m_build_maxhp)
		{
			m_com_anim->SetAnimation(L"LURKER_BIRTH");
			m_unitinfo.state = STATE_NONE;
			//생산 아이디
		}
	}
	else if(MUTATE == m_unitinfo.state)
	{
		m_com_anim->SetAnimation(L"MUTATE");
	}

	m_select_ui->Update();
	m_energybar_ui->Update();
}

void CLurker_egg::Render(void)
{
	if(false == m_unitinfo.is_active)
		return;

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;


	m_select_ui->Render();
	m_com_anim->Render();
	m_com_cc->Render();
	m_energybar_ui->Render();

	//m_com_pathfind->Render();
	CLineMgr::GetInstance()->collisionbox_render(m_rect);
}

void CLurker_egg::Inputkey_reaction(const int& nkey)
{

}

void CLurker_egg::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}

bool CLurker_egg::Input_cmd(const int& nkey , bool* waitkey)
{
	return false;
}

bool CLurker_egg::Input_cmd(const int& firstkey , const int& secondkey)
{
	return false;
}

void CLurker_egg::Update_Cmdbtn(void)
{

}

void CLurker_egg::Update_Wireframe(void)
{
	D3DXVECTOR2 interface_pos = CIngame_UIMgr::GetInstance()->GetMainInterface_pos();

	if(true == CIngame_UIMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.state))
	{
		CUI* pui = NULL;
		pui = new CUI_Wireframe(L"WIRE_LURKEGG" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));
		pui->Initialize();
		CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);

		CFontMgr::GetInstance()->SetInfomation_font(L"Zerg LurkerEgg" ,interface_pos.x + 320 , interface_pos.y + 390 );
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

	CFontMgr::GetInstance()->Setbatch_Font(L"방어력:%d + %d",m_unitinfo.armor, m_upg_info[UPG_Z_GROUND_ARMOR].upg_cnt 
		,interface_pos.x + 310 , interface_pos.y + 458 , D3DCOLOR_ARGB(255,255,255,255));
}

void CLurker_egg::Dead(void)
{

}

void CLurker_egg::Release(void)
{
	CObj* pobj = NULL;

	for(int i = 0; i < m_create_cnt; ++i)
	{
		pobj = new CLurker;

		CObjMgr::GetInstance()->AddObject(pobj , OBJ_LURKER);

		pobj->SetPos(m_vPos);
		pobj->SetTeamNumber(m_eteamnumber);
		pobj->Initialize();

		if(CUnitMgr::GetInstance()->is_unit(this))
		{			
			CUnitMgr::GetInstance()->SetUnit(pobj);
			pobj->SetSelect(GENERAL_SELECT);
		}
	}

	CUnitMgr::GetInstance()->clear_destroy_unitlist(this);

	CObj::area_release();
}
