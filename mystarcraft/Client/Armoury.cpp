#include "StdAfx.h"
#include "Armoury.h"

#include "Component.h"
#include "Com_TBuildingAnim.h"
#include "Com_Collision.h"
#include "Com_fog.h"
#include "Com_AirPathfind.h"

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
#include "Ingame_UIMgr.h"
#include "TimeMgr.h"

#include "UI_Wireframe.h"
#include "UI_Cmd_info.h"
#include "UI_form.h"

#include "UI_Energy_bar.h"
CArmoury::CArmoury(void)
{
}

CArmoury::~CArmoury(void)
{
	Release();
}

void CArmoury::Initialize(void)
{
	m_com_pathfind = NULL;

	m_vertex.left = 48.f;
	m_vertex.right = 48.f;
	m_vertex.top =  32.f;
	m_vertex.bottom = 23.f;

	CTerran_building::building_area_Initialize(2 , 3);
	CTerran_building::building_pos_Initialize(2 , 3);

	m_ebuild_tech = T_ARMOURY;

	m_sortID = SORT_GROUND;	
	m_ecategory = CATEGORY_BUILDING;
	m_eOBJ_NAME = OBJ_ARMOURY;
	m_eteamnumber = TEAM_0;

	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.state = BUILD;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_LARGE;
	m_unitinfo.maxhp = 750;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 28;
	m_unitinfo.search_range = 0;
	m_unitinfo.fog_range = 512;
	m_unitinfo.fbuildtime = 1.f;

	m_com_anim = new CCom_TBuildingAnim(L"T_ARMOURY" , m_matWorld);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_Collision(m_vPos , m_rect , m_vertex , false) ) ) ;	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_select_ui = new CUI_Select(L"Select110" , m_vPos , 10);
	m_select_ui->Initialize();

	m_energybar_ui = new CUI_Energy_bar(this , 110);
	m_energybar_ui->Initialize();

	m_fbuild_tick = float(m_unitinfo.maxhp)/m_unitinfo.fbuildtime;
	CTerran_building::fire_eff_initialize();

	m_upg_info =  CIngame_UIMgr::GetInstance()->GetUpginfo();
}

void CArmoury::Update(void)
{
	CTerran_building::building_area_update();	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();

	if(IDLE == m_unitinfo.state)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
	}
	else if(BUILD == m_unitinfo.state)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"BUILD");

		m_build_hp += m_fbuild_tick * GETTIME;
		m_unitinfo.hp = (int)m_build_hp;

		if(m_unitinfo.hp >= m_unitinfo.maxhp )
		{
			m_unitinfo.hp = m_unitinfo.maxhp;
			m_unitinfo.state = IDLE;
			CTerran_building::Build_Complete();
		}
	}

	CTerran_building::fire_eff_update();
	CTerran_building::upginfo_update(UPG_T_MECHANIC_WEAPON);
	CTerran_building::upginfo_update(UPG_T_MECHANIC_ARMOR);
	CTerran_building::upginfo_update(UPG_T_AIR_WEAPON);
	CTerran_building::upginfo_update(UPG_T_AIR_ARMOR);

	m_select_ui->Update();
	m_energybar_ui->Update();
}

void CArmoury::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_select_ui->Render();
	m_com_anim->Render();
	m_energybar_ui->Render();

	CLineMgr::GetInstance()->collisionbox_render(m_rect);

	CTerran_building::fire_eff_render();
}

void CArmoury::Release(void)
{
	CTerran_building::area_release();
}

void CArmoury::Dead(void)
{
	CObj* pobj = new CGeneraEff(L"XLARGEBANG" , m_vPos , D3DXVECTOR2(1.f,1.f) , SORT_GROUND);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);


	pobj = new CCorpse(L"" , L"TBDSMALL_WRECKAGE");
	pobj->SetPos(m_vPos.x , m_vPos.y);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddCorpse(pobj);

	CUnitMgr::GetInstance()->clear_destroy_unitlist(this);

	if( true == m_upg_info[UPG_T_MECHANIC_WEAPON].proceeding &&
		m_obj_id == m_upg_info[UPG_T_MECHANIC_WEAPON].obj_num)
	{
		m_upg_info[UPG_T_MECHANIC_WEAPON].proceeding = false;
		m_upg_info[UPG_T_MECHANIC_WEAPON].obj_num = 0;
		m_upg_info[UPG_T_MECHANIC_WEAPON].curtime = 0;
	}

	if( true == m_upg_info[UPG_T_MECHANIC_ARMOR].proceeding &&
		m_obj_id == m_upg_info[UPG_T_MECHANIC_ARMOR].obj_num)
	{
		m_upg_info[UPG_T_MECHANIC_ARMOR].proceeding = false;
		m_upg_info[UPG_T_MECHANIC_ARMOR].obj_num = 0;
		m_upg_info[UPG_T_MECHANIC_ARMOR].curtime = 0;
	}

	if( true == m_upg_info[UPG_T_AIR_WEAPON].proceeding &&
		m_obj_id == m_upg_info[UPG_T_AIR_WEAPON].obj_num)
	{
		m_upg_info[UPG_T_AIR_WEAPON].proceeding = false;
		m_upg_info[UPG_T_AIR_WEAPON].obj_num = 0;
		m_upg_info[UPG_T_AIR_WEAPON].curtime = 0;
	}

	if( true == m_upg_info[UPG_T_AIR_ARMOR].proceeding &&
		m_obj_id == m_upg_info[UPG_T_AIR_ARMOR].obj_num)
	{
		m_upg_info[UPG_T_AIR_ARMOR].proceeding = false;
		m_upg_info[UPG_T_AIR_ARMOR].obj_num = 0;
		m_upg_info[UPG_T_AIR_ARMOR].curtime = 0;
	}

}

void CArmoury::Inputkey_reaction(const int& nkey)
{
	if('W' == nkey)
	{
		//공업
		if( false == m_upg_info[UPG_T_MECHANIC_WEAPON].proceeding &&
			m_upg_info[UPG_T_MECHANIC_WEAPON].upg_cnt < 3)
		{
			if( 1 == m_upg_info[UPG_T_MECHANIC_WEAPON].upg_cnt &&
				CIngame_UIMgr::GetInstance()->Get_T_BuildTech(T_SIENCE) == 0)
			{
				CFontMgr::GetInstance()->SetNoticeFont(L"테크가 부족합니다 (필요건물 Sience facility)"
					, BACKBUFFER_SIZEX/2 , BACKBUFFER_SIZEY - BACKBUFFER_SIZEY/3.2f);
				return;
			}
			m_upg_info[UPG_T_MECHANIC_WEAPON].proceeding = true;
			m_upg_info[UPG_T_MECHANIC_WEAPON].obj_num = m_obj_id;
			m_unitinfo.state = DEVELOPING;

		}
	}
	if('P' == nkey)
	{
		//방업
		if( false == m_upg_info[UPG_T_MECHANIC_ARMOR].proceeding &&
			m_upg_info[UPG_T_MECHANIC_ARMOR].upg_cnt < 3)
		{
			if( 1 == m_upg_info[UPG_T_MECHANIC_ARMOR].upg_cnt &&
				CIngame_UIMgr::GetInstance()->Get_T_BuildTech(T_SIENCE) == 0)
			{
				CFontMgr::GetInstance()->SetNoticeFont(L"테크가 부족합니다 (필요건물 Sience facility)"
					, BACKBUFFER_SIZEX/2 , BACKBUFFER_SIZEY - BACKBUFFER_SIZEY/3.2f);
				return;
			}

			m_upg_info[UPG_T_MECHANIC_ARMOR].proceeding = true;
			m_upg_info[UPG_T_MECHANIC_ARMOR].obj_num = m_obj_id;
			m_unitinfo.state = DEVELOPING;
		}
	}

	if('S' == nkey)
	{
		//공업
		if( false == m_upg_info[UPG_T_AIR_WEAPON].proceeding &&
			m_upg_info[UPG_T_AIR_WEAPON].upg_cnt < 3)
		{
			if( 1 == m_upg_info[UPG_T_AIR_WEAPON].upg_cnt &&
				CIngame_UIMgr::GetInstance()->Get_T_BuildTech(T_SIENCE) == 0)
			{
				CFontMgr::GetInstance()->SetNoticeFont(L"테크가 부족합니다 (필요건물 Sience facility)"
					, BACKBUFFER_SIZEX/2 , BACKBUFFER_SIZEY - BACKBUFFER_SIZEY/3.2f);
				return;
			}
			m_upg_info[UPG_T_AIR_WEAPON].proceeding = true;
			m_upg_info[UPG_T_AIR_WEAPON].obj_num = m_obj_id;
			m_unitinfo.state = DEVELOPING;

		}
	}
	if('H' == nkey)
	{
		//방업
		if( false == m_upg_info[UPG_T_AIR_ARMOR].proceeding &&
			m_upg_info[UPG_T_AIR_ARMOR].upg_cnt < 3)
		{
			if( 1 == m_upg_info[UPG_T_AIR_ARMOR].upg_cnt &&
				CIngame_UIMgr::GetInstance()->Get_T_BuildTech(T_SIENCE) == 0)
			{
				CFontMgr::GetInstance()->SetNoticeFont(L"테크가 부족합니다 (필요건물 Sience facility)"
					, BACKBUFFER_SIZEX/2 , BACKBUFFER_SIZEY - BACKBUFFER_SIZEY/3.2f);
				return;
			}

			m_upg_info[UPG_T_AIR_ARMOR].proceeding = true;
			m_upg_info[UPG_T_AIR_ARMOR].obj_num = m_obj_id;
			m_unitinfo.state = DEVELOPING;
		}
	}
}

void CArmoury::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}
void CArmoury::Update_Cmdbtn(void)
{
	const CUI* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();

	if(IDLE == m_unitinfo.state)
	{
		if( false == m_upg_info[UPG_T_MECHANIC_WEAPON].proceeding && m_upg_info[UPG_T_MECHANIC_WEAPON].upg_cnt < 3)
		{
			if( 1 == m_upg_info[UPG_T_MECHANIC_WEAPON].upg_cnt &&
				CIngame_UIMgr::GetInstance()->Get_T_BuildTech(T_SIENCE) == 0)
				((CUI_Cmd_info*)pui)->Create_Cmdbtn(0 , L"BTN_T_VAW" , BTN_T_VAW , false);
			else
				((CUI_Cmd_info*)pui)->Create_Cmdbtn(0 , L"BTN_T_VAW" , BTN_T_VAW , true);
		}
		if( false == m_upg_info[UPG_T_MECHANIC_ARMOR].proceeding && m_upg_info[UPG_T_MECHANIC_ARMOR].upg_cnt < 3)
		{
			if( 1 == m_upg_info[UPG_T_MECHANIC_ARMOR].upg_cnt &&
				CIngame_UIMgr::GetInstance()->Get_T_BuildTech(T_SIENCE) == 0)
				((CUI_Cmd_info*)pui)->Create_Cmdbtn(3 , L"BTN_T_VAP" , BTN_T_VAP , false);
			else
				((CUI_Cmd_info*)pui)->Create_Cmdbtn(3 , L"BTN_T_VAP" , BTN_T_VAP , true);
		}

		if( false == m_upg_info[UPG_T_AIR_WEAPON].proceeding && m_upg_info[UPG_T_AIR_WEAPON].upg_cnt < 3)
		{
			if( 1 == m_upg_info[UPG_T_AIR_WEAPON].upg_cnt &&
				CIngame_UIMgr::GetInstance()->Get_T_BuildTech(T_SIENCE) == 0)
				((CUI_Cmd_info*)pui)->Create_Cmdbtn(1 , L"BTN_T_VAS" , BTN_T_VAS , false);
			else
				((CUI_Cmd_info*)pui)->Create_Cmdbtn(1 , L"BTN_T_VAS" , BTN_T_VAS , true);
		}
		if( false == m_upg_info[UPG_T_AIR_ARMOR].proceeding && m_upg_info[UPG_T_AIR_ARMOR].upg_cnt < 3)
		{
			if( 1 == m_upg_info[UPG_T_AIR_ARMOR].upg_cnt &&
				CIngame_UIMgr::GetInstance()->Get_T_BuildTech(T_SIENCE) == 0)
				((CUI_Cmd_info*)pui)->Create_Cmdbtn(4 , L"BTN_T_VAH" , BTN_T_VAH , false);
			else
				((CUI_Cmd_info*)pui)->Create_Cmdbtn(4 , L"BTN_T_VAH" , BTN_T_VAH , true);
		}
	}
	else if(DEVELOPING == m_unitinfo.state)
		((CUI_Cmd_info*)pui)->Create_Cmdbtn(8 , L"BTN_CANCLE" , BTN_CANCLE , true);
}

void CArmoury::Update_Wireframe(void)
{
	D3DXVECTOR2 interface_pos = CIngame_UIMgr::GetInstance()->GetMainInterface_pos();

	if(true == CIngame_UIMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.state))
	{		
		CUI* pui = NULL;
		pui = new CUI_Wireframe(L"WIRE_ARMOURY" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));
		pui->Initialize();
		CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);

		CFontMgr::GetInstance()->SetInfomation_font(L"Terran Armoury" ,interface_pos.x + 320 , interface_pos.y + 390 );

		if(BUILD == m_unitinfo.state)
		{
			CFontMgr::GetInstance()->SetInfomation_font(L"Under construction.." , interface_pos.x + 320 , interface_pos.y + 415);
		}
		else if(DEVELOPING == m_unitinfo.state)
		{
			CFontMgr::GetInstance()->SetInfomation_font(L"Upgrading" , interface_pos.x + 330 , interface_pos.y + 415);

			pui = new CUI_form(L"EDGE" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
			CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);

			if(true == m_upg_info[UPG_T_MECHANIC_WEAPON].proceeding && 
				m_upg_info[UPG_T_MECHANIC_WEAPON].obj_num == m_obj_id)
			{
				pui = new CUI_form(L"BTN_T_VAW" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
				CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);
			}
			else if(true == m_upg_info[UPG_T_MECHANIC_ARMOR].proceeding && 
				m_upg_info[UPG_T_MECHANIC_ARMOR].obj_num == m_obj_id)
			{
				pui = new CUI_form(L"BTN_T_VAP" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
				CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);
			}
			else if(true == m_upg_info[UPG_T_AIR_WEAPON].proceeding && 
				m_upg_info[UPG_T_AIR_WEAPON].obj_num == m_obj_id)
			{
				pui = new CUI_form(L"BTN_T_VAS" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
				CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);
			}
			else if(true == m_upg_info[UPG_T_AIR_ARMOR].proceeding && 
				m_upg_info[UPG_T_AIR_ARMOR].obj_num == m_obj_id)
			{
				pui = new CUI_form(L"BTN_T_VAH" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
				CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);
			}
		}
	}

	//--------------------계속해서 갱신받는 부분

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
	//-------------------------------------------

	if(true == m_upg_info[UPG_T_MECHANIC_WEAPON].proceeding && m_upg_info[UPG_T_MECHANIC_WEAPON].obj_num == m_obj_id)
		CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 293 , interface_pos.y + 435) , m_upg_info[UPG_T_MECHANIC_WEAPON].curtime / m_upg_info[UPG_T_MECHANIC_WEAPON].maxtime );
	else if(true == m_upg_info[UPG_T_MECHANIC_ARMOR].proceeding && m_upg_info[UPG_T_MECHANIC_ARMOR].obj_num == m_obj_id)
		CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 293 , interface_pos.y + 435) , m_upg_info[UPG_T_MECHANIC_ARMOR].curtime / m_upg_info[UPG_T_MECHANIC_ARMOR].maxtime );
	else if(true == m_upg_info[UPG_T_AIR_WEAPON].proceeding && m_upg_info[UPG_T_AIR_WEAPON].obj_num == m_obj_id)
		CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 293 , interface_pos.y + 435) , m_upg_info[UPG_T_AIR_WEAPON].curtime / m_upg_info[UPG_T_AIR_WEAPON].maxtime );
	else if(true == m_upg_info[UPG_T_AIR_ARMOR].proceeding && m_upg_info[UPG_T_AIR_ARMOR].obj_num == m_obj_id)
		CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 293 , interface_pos.y + 435) , m_upg_info[UPG_T_AIR_ARMOR].curtime / m_upg_info[UPG_T_AIR_ARMOR].maxtime );
}