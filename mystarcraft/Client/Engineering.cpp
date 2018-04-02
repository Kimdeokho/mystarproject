#include "StdAfx.h"
#include "Engineering.h"

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
#include "TimeMgr.h"
#include "ComanderMgr.h"

#include "Building_Preview.h"
#include "MouseMgr.h"

#include "UI_Wireframe.h"
#include "UI_Cmd_info.h"
#include "UI_form.h"

#include "UI_Energy_bar.h"
CEngineering::CEngineering(void)
{
}

CEngineering::~CEngineering(void)
{
	Release();
}

void CEngineering::Initialize(void)
{
	m_com_pathfind = NULL;

	m_vertex.left = 48.f;
	m_vertex.right = 49.f;
	m_vertex.top =  32.f;
	m_vertex.bottom = 35.f;

	CTerran_building::building_area_Initialize(3 , 4);
	CTerran_building::building_pos_Initialize(3 , 4);

	m_ebuild_tech = T_EB;

	m_sortID = SORT_GROUND;	
	m_ecategory = CATEGORY_BUILDING;
	m_eOBJ_NAME = OBJ_BARRACK;
	m_eteamnumber = TEAM_0;

	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.state = BUILD;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_LARGE;
	m_unitinfo.maxhp = 850;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 28;
	m_unitinfo.search_range = 0;
	m_unitinfo.fog_range = 512;
	m_unitinfo.fbuildtime = 10.f;

	m_com_anim = new CCom_TBuildingAnim(L"T_EB",m_matWorld );
	m_com_pathfind = new CCom_AirPathfind(m_vPos);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_Collision(m_vPos , m_rect , m_vertex , false) ) ) ;	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_select_ui = new CUI_Select(L"Select146" , m_vPos , 10);
	m_select_ui->Initialize();

	m_energybar_ui = new CUI_Energy_bar(this , 146);
	m_energybar_ui->Initialize();	

	m_is_take_off = false;

	m_fbuild_tick = float(m_unitinfo.maxhp)/m_unitinfo.fbuildtime;
	CTerran_building::fire_eff_initialize();

	m_upg_info = CComanderMgr::GetInstance()->GetUpginfo();
}

void CEngineering::Update(void)
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
	else if(TAKE_OFF == m_unitinfo.state)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"AIR");

		m_vPos.y -= GETTIME*20;

		if(m_vairpos.y - 48.f >= m_vPos.y)
		{
			//이륙 완료
			m_vPos.y = m_vairpos.y - 48.f;
			m_unitinfo.state = AIR_IDLE;
		}
	}
	else if(WORK == m_unitinfo.state)
	{

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
		vpos.x -= m_irow*32;
		vpos.y -= m_weight.y;
		((CBuilding_Preview*)m_sub_preview)->SetPos(vpos);
		CComanderMgr::GetInstance()->SetPreview(m_main_preview);
		CComanderMgr::GetInstance()->SetPreview(m_sub_preview);
	}

	CTerran_building::fire_eff_update();
	CTerran_building::upginfo_update(UPG_T_BIO_WEAPON);
	CTerran_building::upginfo_update(UPG_T_BIO_ARMOR);

	m_select_ui->Update();
	m_energybar_ui->Update();
}

void CEngineering::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_select_ui->Render();
	m_com_anim->Render();
	m_energybar_ui->Render();

	CTerran_building::fire_eff_render();

	CLineMgr::GetInstance()->collisionbox_render(m_rect);
}

void CEngineering::Release(void)
{
	CTerran_building::area_release();

	COMPONENT_PAIR::iterator iter = m_componentlist.find(COM_PATHFINDE);
	Safe_Delete(m_com_pathfind);
	if(iter != m_componentlist.end())
	{
		m_componentlist.erase(iter);
	}
}

void CEngineering::Dead(void)
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

	if( true == m_upg_info[UPG_T_BIO_WEAPON].proceeding &&
		m_obj_id == m_upg_info[UPG_T_BIO_WEAPON].obj_num)
	{
		m_upg_info[UPG_T_BIO_WEAPON].proceeding = false;
		m_upg_info[UPG_T_BIO_WEAPON].obj_num = 0;
		m_upg_info[UPG_T_BIO_WEAPON].curtime = 0;
	}

	if( true == m_upg_info[UPG_T_BIO_ARMOR].proceeding &&
		m_obj_id == m_upg_info[UPG_T_BIO_ARMOR].obj_num)
	{
		m_upg_info[UPG_T_BIO_ARMOR].proceeding = false;
		m_upg_info[UPG_T_BIO_ARMOR].obj_num = 0;
		m_upg_info[UPG_T_BIO_ARMOR].curtime = 0;
	}
}

void CEngineering::Inputkey_reaction(const int& nkey)
{
	m_is_preview = false;

	if(TAKE_OFF == m_unitinfo.state ||
		LANDING == m_unitinfo.state ||
		BUILD == m_unitinfo.state)
		return;

	if('L' == nkey)
	{
		if(false == m_is_take_off)
		{
			//이륙
			CTerran_building::TakeOff();
		}
		else
		{
			//착륙 프리뷰를 킨다
			m_is_preview = true;
			((CBuilding_Preview*)m_main_preview)->SetPreviewInfo(L"T_EB", T_EB , 3 , 4 , this , m_vertex);			
		}		
	}
	if(VK_LBUTTON == nkey)
	{
		if(true == ((CBuilding_Preview*)m_main_preview)->GetActive())
		{
			//착륙 분기
			if(true == ((CBuilding_Preview*)m_main_preview)->Install_check())
			{
				((CBuilding_Preview*)m_main_preview)->SetActive(false);
				((CBuilding_Preview*)m_sub_preview)->SetActive(false);

				m_unitinfo.order = ORDER_LANDING_MOVE;
				D3DXVECTOR2 vclickpos = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
				int idx32 = CMyMath::Pos_to_index(vclickpos , 32);
				vclickpos = CMyMath::index_to_Pos(idx32 , 128 , 32);
				CTerran_building::Landing_move(vclickpos);		

			}
			else
			{
				m_is_preview = true; //설치에 실패하면 프리뷰를 계속 본다.
			}
		}
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
	}
	if('W' == nkey)
	{
		//공업
		if( false == m_upg_info[UPG_T_BIO_WEAPON].proceeding &&
			m_upg_info[UPG_T_BIO_WEAPON].upg_cnt < 3)
		{
			if( 1 == m_upg_info[UPG_T_BIO_WEAPON].upg_cnt &&
				CComanderMgr::GetInstance()->Get_T_BuildTech(T_SIENCE) == 0)
			{
				CFontMgr::GetInstance()->SetNoticeFont(L"테크가 부족합니다 (필요건물 Sience facility)"
					, BACKBUFFER_SIZEX/2 , BACKBUFFER_SIZEY - BACKBUFFER_SIZEY/3.2f);
				return;
			}
			m_upg_info[UPG_T_BIO_WEAPON].proceeding = true;
			m_upg_info[UPG_T_BIO_WEAPON].obj_num = m_obj_id;
			m_unitinfo.state = DEVELOPING;
			
		}
	}
	if('A' == nkey)
	{
		//방업
		if( false == m_upg_info[UPG_T_BIO_ARMOR].proceeding &&
			m_upg_info[UPG_T_BIO_ARMOR].upg_cnt < 3)
		{
			if( 1 == m_upg_info[UPG_T_BIO_ARMOR].upg_cnt &&
				CComanderMgr::GetInstance()->Get_T_BuildTech(T_SIENCE) == 0)
			{
				CFontMgr::GetInstance()->SetNoticeFont(L"테크가 부족합니다 (필요건물 Sience facility)"
					, BACKBUFFER_SIZEX/2 , BACKBUFFER_SIZEY - BACKBUFFER_SIZEY/3.2f);
				return;
			}

			m_upg_info[UPG_T_BIO_ARMOR].proceeding = true;
			m_upg_info[UPG_T_BIO_ARMOR].obj_num = m_obj_id;
			m_unitinfo.state = DEVELOPING;
		}
	}
}

void CEngineering::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}
void CEngineering::Update_Cmdbtn(void)
{
	const CUI* pui = CComanderMgr::GetInstance()->GetCmd_info();

	if(IDLE == m_unitinfo.state)
	{
		if( false == m_upg_info[UPG_T_BIO_WEAPON].proceeding && m_upg_info[UPG_T_BIO_WEAPON].upg_cnt < 3)
		{
			if( 1 == m_upg_info[UPG_T_BIO_WEAPON].upg_cnt &&
				CComanderMgr::GetInstance()->Get_T_BuildTech(T_SIENCE) == 0)
				((CUI_Cmd_info*)pui)->Create_Cmdbtn(0 , L"BTN_T_BEW" , BTN_T_BEW , false);
			else
				((CUI_Cmd_info*)pui)->Create_Cmdbtn(0 , L"BTN_T_BEW" , BTN_T_BEW , true);
		}
		if( false == m_upg_info[UPG_T_BIO_ARMOR].proceeding && m_upg_info[UPG_T_BIO_ARMOR].upg_cnt < 3)
		{
			if( 1 == m_upg_info[UPG_T_BIO_ARMOR].upg_cnt &&
				CComanderMgr::GetInstance()->Get_T_BuildTech(T_SIENCE) == 0)
				((CUI_Cmd_info*)pui)->Create_Cmdbtn(1 , L"BTN_T_BEA" , BTN_T_BEA , false);
			else
				((CUI_Cmd_info*)pui)->Create_Cmdbtn(1 , L"BTN_T_BEA" , BTN_T_BEA , true);
		}

		((CUI_Cmd_info*)pui)->Create_Cmdbtn(8 , L"BTN_TAKE_OFF" , BTN_TAKE_OFF , true);
	}
	else if(DEVELOPING == m_unitinfo.state)
		((CUI_Cmd_info*)pui)->Create_Cmdbtn(8 , L"BTN_CANCLE" , BTN_CANCLE , true);
	else if(AIR_IDLE == m_unitinfo.state ||
		TAKE_OFF == m_unitinfo.state)
	{
		((CUI_Cmd_info*)pui)->Create_Cmdbtn(0 , L"BTN_MOVE" , BTN_MOVE , true);
		((CUI_Cmd_info*)pui)->Create_Cmdbtn(1 , L"BTN_STOP" , BTN_STOP , true);
		((CUI_Cmd_info*)pui)->Create_Cmdbtn(8 , L"BTN_LANDING" , BTN_LANDING , true);
	}
}

void CEngineering::Update_Wireframe(void)
{
	D3DXVECTOR2 interface_pos = CComanderMgr::GetInstance()->GetMainInterface_pos();

	if(true == CComanderMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.state))
	{		
		CUI* pui = NULL;
		pui = new CUI_Wireframe(L"WIRE_ENGINEERING" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));
		pui->Initialize();
		CComanderMgr::GetInstance()->add_wireframe_ui(pui);

		CFontMgr::GetInstance()->SetInfomation_font(L"Terran Engineering" ,interface_pos.x + 320 , interface_pos.y + 390 );

		if(BUILD == m_unitinfo.state)
		{
			CFontMgr::GetInstance()->SetInfomation_font(L"Under construction.." , interface_pos.x + 320 , interface_pos.y + 415);
		}
		else if(DEVELOPING == m_unitinfo.state)
		{
			CFontMgr::GetInstance()->SetInfomation_font(L"Upgrading" , interface_pos.x + 330 , interface_pos.y + 415);

			pui = new CUI_form(L"EDGE" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
			CComanderMgr::GetInstance()->add_wireframe_ui(pui);

			if(true == m_upg_info[UPG_T_BIO_WEAPON].proceeding && 
				m_upg_info[UPG_T_BIO_WEAPON].obj_num == m_obj_id)
			{
				pui = new CUI_form(L"BTN_T_BEW" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
				CComanderMgr::GetInstance()->add_wireframe_ui(pui);
			}
			else if(true == m_upg_info[UPG_T_BIO_ARMOR].proceeding && 
				m_upg_info[UPG_T_BIO_ARMOR].obj_num == m_obj_id)
			{
				pui = new CUI_form(L"BTN_T_BEA" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
				CComanderMgr::GetInstance()->add_wireframe_ui(pui);
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
		CComanderMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 260 , interface_pos.y + 435) , m_build_hp / (float)m_unitinfo.maxhp );


	if(true == m_upg_info[UPG_T_BIO_WEAPON].proceeding && m_upg_info[UPG_T_BIO_WEAPON].obj_num == m_obj_id)
		CComanderMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 293 , interface_pos.y + 435) , m_upg_info[UPG_T_BIO_WEAPON].curtime / m_upg_info[UPG_T_BIO_WEAPON].maxtime );
	else if(true == m_upg_info[UPG_T_BIO_ARMOR].proceeding && m_upg_info[UPG_T_BIO_ARMOR].obj_num == m_obj_id)
		CComanderMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 293 , interface_pos.y + 435) , m_upg_info[UPG_T_BIO_ARMOR].curtime / m_upg_info[UPG_T_BIO_ARMOR].maxtime );
	//-------------------------------------------
}