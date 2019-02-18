#include "StdAfx.h"
#include "Barrack.h"

#include "Component.h"
#include "Com_TBuildingAnim.h"
#include "Com_Collision.h"
#include "Com_fog.h"
#include "Com_AirPathfind.h"
#include "Com_Production_building.h"

#include "ScrollMgr.h"
#include "MyMath.h"
#include "Area_Mgr.h"
#include "UnitMgr.h"
#include "ObjMgr.h"
#include "Ingame_UIMgr.h"
#include "UI_Resource.h"

#include "GeneraEff.h"

#include "UI_Select.h"
#include "LineMgr.h"
#include "FontMgr.h"
#include "Corpse.h"
#include "TimeMgr.h"
#include "Ingame_UIMgr.h"

#include "Building_Preview.h"
#include "MouseMgr.h"

#include "UI_Cmd_info.h"
#include "UI_Wireframe.h"
#include "UI_Energy_bar.h"
#include "Session_Mgr.h"

#include "MyCmd_PartBuilding.h"
#include "KeyMgr.h"
#include "Input_Interface.h"
CBarrack::CBarrack(void)
{
}

CBarrack::~CBarrack(void)
{
	Release();
}

void CBarrack::Initialize(void)
{
	m_com_pathfind = NULL;

	m_vertex.left = 48.f;
	m_vertex.right = 57.f;
	m_vertex.top =  40.f;
	m_vertex.bottom = 33.f;

	CTerran_building::building_area_Initialize(3 , 4);
	CTerran_building::building_pos_Initialize(3 , 4);

	m_ebuild_tech = T_BARRACK;

	m_sortID = SORT_GROUND;	
	m_ecategory = CATEGORY_BUILDING;
	m_eOBJ_NAME = OBJ_BARRACK;

	m_unitinfo.etribe = TRIBE_TERRAN;
	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.state = BUILD;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_LARGE;

	m_unitinfo.maxhp = 1000;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 28;
	m_unitinfo.search_range = 0;
	m_unitinfo.fog_range = 512;
	m_unitinfo.fbuildtime = 2.f;

	m_com_anim = new CCom_TBuildingAnim(L"T_BARRACK",m_matWorld );
	m_com_pathfind = new CCom_AirPathfind(m_vPos);
	m_com_production = new CCom_Production_building(m_vPos , m_weight , m_irow , m_icol);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_PRODUCTION_BUILDING , m_com_production));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_Collision(m_vPos , m_rect , m_vertex , false) ) ) ;	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_select_ui = new CUI_Select(L"Select146" , m_vPos , 10);
	m_select_ui->Initialize();

	m_energybar_ui = new CUI_Energy_bar(this , 146 , m_vertex.bottom*1.8f);
	m_energybar_ui->Initialize();

	m_is_take_off = false;

	m_fbuild_tick = float(m_unitinfo.maxhp)/m_unitinfo.fbuildtime;
	CTerran_building::fire_eff_initialize();
}

void CBarrack::Update(void)
{
	//CFontMgr::GetInstance()->Setbatch_Font(L"%d" , 0 , m_vPos.x , m_vPos.y);
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
	else if(TAKE_OFF == m_unitinfo.state)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"AIR");

		m_vPos.y -= GETTIME*20;

		if(m_vairpos.y - 48.f >= m_vPos.y)
		{
			//ÀÌ·ú ¿Ï·á
			m_vPos.y = m_vairpos.y - 48.f;
			m_unitinfo.state = AIR_IDLE;
		}
	}
	else if(PRODUCTION == m_unitinfo.state)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"PRODUCTION");
	}
	else if(LANDING == m_unitinfo.state)
	{
		m_vPos.y += GETTIME*20;

		if(m_vgroundpos.y <= m_vPos.y)
		{
			m_is_take_off = false;
			m_vPos.y = m_vgroundpos.y;
			m_unitinfo.eMoveType = MOVE_GROUND;
			m_sortID = SORT_GROUND;
			m_unitinfo.order = ORDER_NONE;
			m_unitinfo.state = IDLE;

			CTerran_building::building_area_Initialize(3, 4);
			COMPONENT_PAIR::iterator iter = m_componentlist.find(COM_PATHFINDE);
			m_componentlist.erase(iter);
		}

	}


	if(ORDER_LANDING_MOVE == m_unitinfo.order)
	{
		if(true == ((CCom_AirPathfind*)m_com_pathfind)->Getarrive() ) 
		{
			if(true == ((CBuilding_Preview*)m_main_preview)->Install_check())
			{
				m_unitinfo.state = LANDING;
				m_unitinfo.order = ORDER_NONE;
			}
			else
			{
				m_unitinfo.state = AIR_IDLE;
				m_unitinfo.order = ORDER_NONE;
			}
		}
	}

	if(true == m_is_preview)
	{
		D3DXVECTOR2 vpos = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
		((CBuilding_Preview*)m_main_preview)->SetPos(vpos);
		CIngame_UIMgr::GetInstance()->SetPreview(m_main_preview);
	}

	CTerran_building::fire_eff_update();

	m_select_ui->Update();
	m_energybar_ui->Update();
}

void CBarrack::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_select_ui->Render();
	m_com_anim->Render();
	m_energybar_ui->Render();

	CTerran_building::fire_eff_render();

	CLineMgr::GetInstance()->collisionbox_render(m_rect);
}

void CBarrack::Inputkey_reaction(const int& nkey)
{
	if(TAKE_OFF == m_unitinfo.state ||
		LANDING == m_unitinfo.state)
		return;

	if('L' == nkey)
	{
		if(false == m_is_take_off)
		{
			//ÀÌ·ú
			CTerran_building::TakeOff();
		}	
	}

	if(nkey == 9998)
	{
		m_unitinfo.order = ORDER_LANDING_MOVE;
		D3DXVECTOR2 vclickpos = m_main_preview->GetPreviewInfo().vpos;
		int idx32 = CMyMath::Pos_to_index(vclickpos , 32);
		vclickpos = CMyMath::index_to_Pos(idx32 , 128 , 32);
		CTerran_building::Landing_move(vclickpos);
	}

	if(VK_RBUTTON == nkey)
	{
		((CBuilding_Preview*)m_main_preview)->SetActive(false);
		((CBuilding_Preview*)m_sub_preview)->SetActive(false);

		if(AIR_IDLE == m_unitinfo.state)
		{
			m_unitinfo.order = ORDER_MOVE;
			D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();
			((CCom_AirPathfind*)m_com_pathfind)->SetGoalPos(goalpos);

		}
		else
		{
			CObj* ptarget = CArea_Mgr::GetInstance()->GetChoiceTarget();

			D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();
			m_com_production->set_rallypoint(goalpos);

			if(this == ptarget)
			{
				m_com_production->set_is_rally(false);
				m_com_production->set_rallypoint(m_vPos);								
			}
			else
			{
				m_com_production->set_is_rally(true);
				m_com_production->rallypoint_pathfinding();

			}
		}
	}

	if(false == m_is_take_off)
	{
		CUI_Resource* pui = CIngame_UIMgr::GetInstance()->GetResource_UI();
		if('M' == nkey)
		{
			if(pui->is_excess_of_population(1 , m_eteamnumber) &&
				pui->SetResource(-50 , 0,  m_eteamnumber))
				m_com_production->add_production_info(1.f , 1.f ,OBJ_MARINE , L"BTN_MARINE");
		}
		if('F' == nkey)
		{
			if(pui->is_excess_of_population(1 , m_eteamnumber) &&
				pui->SetResource(-50 , -25,  m_eteamnumber))
				m_com_production->add_production_info(1.f , 1.f , OBJ_FIREBAT , L"BTN_FIREBAT");
		}
		if('G' == nkey)
		{
			if(pui->is_excess_of_population(1 , m_eteamnumber) &&
				pui->SetResource(-50 , -25,  m_eteamnumber))
				m_com_production->add_production_info(1.f , 1.f ,OBJ_GHOST , L"BTN_GHOST");
		}
		if('C' == nkey)
		{
			if(pui->is_excess_of_population(1 , m_eteamnumber) &&
				pui->SetResource(-50 , -25,  m_eteamnumber))
				m_com_production->add_production_info(1.f , 1.f ,OBJ_MEDIC , L"BTN_MEDIC");
		}
	}
}

void CBarrack::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}
bool CBarrack::Input_cmd(const int& nkey , bool* waitkey)
{
	if(VK_LBUTTON == nkey)
	{
		if(true == (m_main_preview)->GetActive())
		{
			//Âø·ú ºÐ±â
			if(true == (m_main_preview)->Install_check())
			{
				(m_main_preview)->SetActive(false);
				(m_sub_preview)->SetActive(false);

				PREVIEW_INFO maininfo , subinfo;
				maininfo = m_main_preview->GetPreviewInfo();
				subinfo = m_sub_preview->GetPreviewInfo();
				CKeyMgr::GetInstance()->GetInputDevice()->PushCommand(CMyCmd_PartBuilding::StaticCreate(maininfo , subinfo , 9998));	
				m_is_preview = false;
			}
			else
			{
				m_is_preview = true; //¼³Ä¡¿¡ ½ÇÆÐÇÏ¸é ÇÁ¸®ºä¸¦ °è¼Ó º»´Ù.
			}
		}
		else
		{
			const CUI* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();
			CMD_BTN ebtn = ((CUI_Cmd_info*)pui)->Get_clicked_btn();
			if(BTN_TAKE_OFF == ebtn ||	BTN_LANDING == ebtn)
				Input_cmd('L' , waitkey);
		}
	}
	if('L' == nkey)
	{
		//Âø·ú ÇÁ¸®ºä¸¦ Å²´Ù
		if(true == m_is_take_off)
		{
			m_is_preview = true;
			(m_main_preview)->SetPreviewInfo(L"T_BARRACK", T_BARRACK , 3 , 4 , m_vertex);			
		}
		else
			return true;
	}

	if(false == m_is_take_off)
	{
		if('M' == nkey)
			return true;
		if('F' == nkey)
			return true;
		if('G' == nkey)
			return true;
		if('C' == nkey)
			return true;
	}

	return false;
}
bool CBarrack::Input_cmd(const int& firstkey , const int& secondkey)
{
	return false;
}

void CBarrack::Update_Cmdbtn(void)
{
	CUI_Cmd_info* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();
	if(IDLE == m_unitinfo.state || PRODUCTION == m_unitinfo.state)
	{
		pui->Create_Cmdbtn(0 , L"BTN_MARINE" , BTN_MARINE , true);

		if(0 < CIngame_UIMgr::GetInstance()->Get_BuildTech(T_ACADEMY))
		{
			pui->Create_Cmdbtn(1 , L"BTN_FIREBAT" , BTN_FIREBAT , true);
			pui->Create_Cmdbtn(3 , L"BTN_MEDIC" , BTN_MEDIC , true);
		}
		else
		{
			pui->Create_Cmdbtn(1 , L"BTN_FIREBAT" , BTN_FIREBAT , false);
			pui->Create_Cmdbtn(3 , L"BTN_MEDIC" , BTN_MEDIC , false);
		}

		if(0 < CIngame_UIMgr::GetInstance()->Get_BuildTech(T_GHOST_ADDON))
			pui->Create_Cmdbtn(2 , L"BTN_GHOST" , BTN_GHOST , true);
		else
			pui->Create_Cmdbtn(2 , L"BTN_GHOST" , BTN_GHOST, false);

		if(IDLE == m_unitinfo.state)
			pui->Create_Cmdbtn(8 , L"BTN_TAKE_OFF" , BTN_TAKE_OFF , true);

	}
	else if(AIR_IDLE == m_unitinfo.state ||
		TAKE_OFF == m_unitinfo.state)
	{
		pui->Create_Cmdbtn(0 , L"BTN_MOVE" , BTN_MOVE , true);
		pui->Create_Cmdbtn(1 , L"BTN_STOP" , BTN_STOP , true);
		pui->Create_Cmdbtn(8 , L"BTN_LANDING" , BTN_LANDING , true);
	}

}

void CBarrack::Update_Wireframe(void)
{
	D3DXVECTOR2 interface_pos = CIngame_UIMgr::GetInstance()->GetMainInterface_pos();

	if(true == CIngame_UIMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.state))
	{
		CUI* pui = NULL;
		pui = new CUI_Wireframe(L"WIRE_BARRACK" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));
		pui->Initialize();
		CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);

		CFontMgr::GetInstance()->SetInfomation_font(L"Terran Barrack" ,interface_pos.x + 320 , interface_pos.y + 390 );

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

	m_com_production->show_production_state();
}

void CBarrack::Dead(void)
{
	CObj* pobj = new CGeneraEff(L"XLARGEBANG" , m_vPos , D3DXVECTOR2(1.f,1.f) , SORT_GROUND);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);

	pobj = new CCorpse(L"" , L"TBDSMALL_WRECKAGE");
	pobj->SetPos(m_vPos.x , m_vPos.y);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddCorpse(pobj);

	CUnitMgr::GetInstance()->clear_destroy_unitlist(this);

	if(NULL != m_partbuilding)
	{
		((CTerran_building*)m_partbuilding)->Setlink(false , NULL);
		m_partbuilding = NULL;
	}
}

void CBarrack::Release(void)
{
	CTerran_building::area_release();	

	COMPONENT_PAIR::iterator iter = m_componentlist.find(COM_PATHFINDE);
	Safe_Delete(m_com_pathfind);
	if(iter != m_componentlist.end())
	{
		m_componentlist.erase(iter);
	}

	CTerran_building::fire_eff_render();
}

