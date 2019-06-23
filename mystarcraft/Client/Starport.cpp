#include "StdAfx.h"
#include "Starport.h"

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
#include "GeneraEff.h"

#include "UI_Select.h"
#include "LineMgr.h"
#include "FontMgr.h"
#include "Corpse.h"
#include "TimeMgr.h"
#include "Ingame_UIMgr.h"

#include "Building_Preview.h"
#include "MouseMgr.h"

#include "Star_addon.h"
#include "UI_Cmd_info.h"
#include "UI_Wireframe.h"
#include "UI_form.h"
#include "UI_Resource.h"

#include "UI_Energy_bar.h"
#include "KeyMgr.h"
#include "Input_Interface.h"
#include "MyCmd_PartBuilding.h"
CStarport::CStarport(void)
{
}

CStarport::~CStarport(void)
{
	Release();
}

void CStarport::Initialize(void)
{
	m_com_pathfind = NULL;

	m_vertex.left = 48.f;
	m_vertex.right = 49.f;
	m_vertex.top =  40.f;
	m_vertex.bottom = 39.f;

	CTerran_building::building_area_Initialize(3 , 4);
	CTerran_building::building_pos_Initialize(3 , 4);

	m_ebuild_tech = T_STARPORT;

	m_sortID = SORT_GROUND;	
	m_ecategory = CATEGORY_BUILDING;
	m_eOBJ_NAME = OBJ_STARPORT;

	m_unitinfo.etribe = TRIBE_TERRAN;
	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.state = BUILD;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_LARGE;
	m_unitinfo.maxhp = 1300;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 28;
	m_unitinfo.search_range = 0;
	m_unitinfo.fog_range = 512;
	m_unitinfo.fbuildtime = 10.f;

	m_com_anim = new CCom_TBuildingAnim(L"T_STARPORT",m_matWorld );
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

	m_energybar_ui = new CUI_Energy_bar(this , 146 ,  m_vertex.bottom * 1.6f);
	m_energybar_ui->Initialize();

	m_is_take_off = false;

	m_fbuild_tick = float(m_unitinfo.maxhp)/m_unitinfo.fbuildtime;
	CTerran_building::fire_eff_initialize();
}

void CStarport::Update(void)
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

			if(true == m_is_autoinstall)
			{
				m_is_autoinstall = false;
				m_is_partinstall = true;

				Landing_move( m_sub_preview->GetPreviewInfo().vpos );
			}
		}
	}
	else if(LANDING == m_unitinfo.state)
	{
		m_vPos.y += GETTIME*20;

		if(m_vgroundpos.y <= m_vPos.y)
		{
			//설치해야 한다면..
			if(true == m_is_partinstall)
			{
				CObj* pobj = NULL;
				m_is_partinstall = false;
				if(T_STAR_ADDON == ((CBuilding_Preview*)m_main_preview)->GetPreviewInfo().ebuild)
				{
					pobj = new CStar_addon(this/*아니면 오브젝트 아이디*/);
					pobj->SetPos(((CBuilding_Preview*)m_main_preview)->GetPreviewInfo().vpos);
					pobj->Initialize();				
					CObjMgr::GetInstance()->AddObject(pobj , OBJ_STAR_ADDON);
				}
				m_partbuilding = pobj;
				((CTerran_building*)m_partbuilding)->Setlink(true , this);
			}
			else
			{
				int partidx = m_curidx32 + 3 + SQ_TILECNTX;
				m_partbuilding = CArea_Mgr::GetInstance()->Search_Partbuilding(m_curidx64 , partidx , OBJ_STAR_ADDON);
				if(NULL != m_partbuilding)
					((CTerran_building*)m_partbuilding)->Setlink(true , this);
			}

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
			if(true == m_is_partinstall)
			{
				if(true == ((CBuilding_Preview*)m_main_preview)->Install_check() &&
					true == ((CBuilding_Preview*)m_sub_preview)->Install_check())
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
			else
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
	}



	if(true == m_is_preview)
	{
		D3DXVECTOR2 vpos = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
		((CBuilding_Preview*)m_main_preview)->SetPos(vpos);
		vpos.x -= m_irow*32;
		vpos.y -= m_weight.y;
		((CBuilding_Preview*)m_sub_preview)->SetPos(vpos);
		CIngame_UIMgr::GetInstance()->SetPreview(m_main_preview);
		CIngame_UIMgr::GetInstance()->SetPreview(m_sub_preview);
	}

	CTerran_building::fire_eff_update();

	m_select_ui->Update();
	m_energybar_ui->Update();
}

void CStarport::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_select_ui->Render();
	m_com_anim->Render();
	m_energybar_ui->Render();

	CTerran_building::fire_eff_render();

	CLineMgr::GetInstance()->collisionbox_render(m_rect);
	m_com_production->Render();
}


void CStarport::Inputkey_reaction(const int& nkey)
{
	if(TAKE_OFF == m_unitinfo.state ||
		LANDING == m_unitinfo.state)
		return;


	if('L' == nkey)
	{
		if(false == m_is_take_off)
		{
			//이륙
			CTerran_building::TakeOff();
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
		else
		{
			CObj* ptarget = CArea_Mgr::GetInstance()->GetChoiceTarget();

			D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();
			((CCom_Production_building*)m_com_production)->set_rallypoint(goalpos);

			if(this == ptarget)
			{
				((CCom_Production_building*)m_com_production)->set_is_rally(false);
				((CCom_Production_building*)m_com_production)->set_rallypoint(m_vPos);								
			}
			else
			{
				((CCom_Production_building*)m_com_production)->set_is_rally(true);
				((CCom_Production_building*)m_com_production)->rallypoint_pathfinding();

			}
		}
	}

	if(9998 == nkey)
	{
		m_unitinfo.order = ORDER_LANDING_MOVE;
		D3DXVECTOR2 vclickpos = m_main_preview->GetPreviewInfo().vpos;
		int idx32 = CMyMath::Pos_to_index(vclickpos , 32);
		vclickpos = CMyMath::index_to_Pos(idx32 , 128 , 32);
		CTerran_building::Landing_move(vclickpos);
	}
	if(9999 == nkey)
	{
		if(true == m_is_take_off)
		{
			//공중에서 부속품 설치
			m_is_partinstall = true;
			CTerran_building::Landing_move( (m_sub_preview)->GetPreviewInfo().vpos);
		}
		else
		{

			D3DXVECTOR2 vpt = m_sub_preview->GetPreviewInfo().vcenter_pos;
			int idx = CMyMath::Pos_to_index(vpt , 32);

			if(idx != m_curidx32)
			{
				m_is_autoinstall = true;
				m_is_partinstall = true;

				CTerran_building::TakeOff();
			}
			else
			{
				//옆에 바로 설치
				CObj* pobj = NULL;
				TERRAN_BUILD_TECH ebuild = (m_main_preview)->GetPreviewInfo().ebuild;
				if(T_STAR_ADDON == ebuild)
				{
					pobj = new CStar_addon(this/*아니면 오브젝트 아이디*/);
					pobj->SetPos((m_main_preview)->GetPreviewInfo().vpos);
					pobj->Initialize();				
					CObjMgr::GetInstance()->AddObject(pobj , OBJ_STARPORT);

				}
				m_partbuilding = pobj;
				((CTerran_building*)m_partbuilding)->Setlink(true , this);
			}
		}
	}

	if(false == m_is_take_off)
	{
		CUI_Resource* pui = CIngame_UIMgr::GetInstance()->GetResource_UI();

		if('W' == nkey)
		{
			if(pui->is_excess_of_population(2 , m_eteamnumber) && pui->SetResource(-150 , -100, m_eteamnumber))
				((CCom_Production_building*)m_com_production)->add_production_info(1.f , 2, OBJ_WRAITH , L"BTN_WRAITH");
		}
		if('D' == nkey)
		{
			if(pui->is_excess_of_population(2 , m_eteamnumber) && pui->SetResource(-100 , -100, m_eteamnumber))
			((CCom_Production_building*)m_com_production)->add_production_info(1.f , 2, OBJ_DROPSHIP , L"BTN_DROPSHIP");
		}
		if('V' == nkey)
		{
			if(pui->is_excess_of_population(2 , m_eteamnumber) && pui->SetResource(-100 , -225, m_eteamnumber))
			((CCom_Production_building*)m_com_production)->add_production_info(1.f , 2, OBJ_VESSEL , L"BTN_VESSEL");
		}
		if('B' == nkey)
		{
			if(pui->is_excess_of_population(6 , m_eteamnumber) && pui->SetResource(-400 , -300, m_eteamnumber))
			((CCom_Production_building*)m_com_production)->add_production_info(1.f , 6, OBJ_BATTLE , L"BTN_BATTLE");
		}
	}
}

void CStarport::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}
bool CStarport::Input_cmd(const int& nkey, bool* waitkey)
{
	if(TAKE_OFF == m_unitinfo.state ||
		LANDING == m_unitinfo.state)
		return false;

	MYRECT<float> temprc;
	if('C' == nkey)
	{
		m_is_preview = true;
		((CBuilding_Preview*)m_main_preview)->SetPreviewInfo(L"T_STAR_ADDON", T_STAR_ADDON , 2 , 2 ,  temprc);
		((CBuilding_Preview*)m_sub_preview)->SetPreviewInfo(L"T_STARPORT", T_STARPORT , 3 , 4 ,  m_vertex);
	}
	if('L' == nkey)
	{
		if(true == m_is_take_off)
		{
			//이륙
			m_is_preview = true;
			(m_main_preview)->SetPreviewInfo(L"T_STARPORT", T_FACTORY , 3 , 4 ,  m_vertex);			
		}
		else
			return true;
	}

	if(false == m_is_take_off)
	{
		if('W' == nkey)
			return true;
		if('D' == nkey)
			return true;
		if('V' == nkey)
			return true;
		if('B' == nkey)
			return true;
	}

	if(VK_LBUTTON == nkey)
	{
		const CUI* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();
		CMD_BTN eclicked_btn = ((CUI_Cmd_info*)pui)->Get_clicked_btn();

		if(BTN_TAKE_OFF == eclicked_btn)
		{
			Inputkey_reaction('L');
			return false;
		}
		if(BTN_LANDING == eclicked_btn)
		{
			Inputkey_reaction('L');
			return false;
		}

		if(true == (m_main_preview)->GetActive() &&
			true == (m_sub_preview)->GetActive())
		{
			if(true == (m_main_preview)->Install_check() &&
				true == (m_sub_preview)->Install_check())
			{
				m_is_preview = false;
				(m_main_preview)->SetActive(false);
				(m_sub_preview)->SetActive(false);

				PREVIEW_INFO maininfo , subinfo;
				maininfo = m_main_preview->GetPreviewInfo();
				subinfo = m_sub_preview->GetPreviewInfo();
				CKeyMgr::GetInstance()->GetInputDevice()->PushCommand(CMyCmd_PartBuilding::StaticCreate(maininfo , subinfo , 9999));
			}
			else
			{
				m_is_preview = true; //설치에 실패하면 프리뷰를 계속 본다.
			}
		}
		else if(true == (m_main_preview)->GetActive())
		{
			//착륙 분기
			if(true == (m_main_preview)->Install_check())
			{
				m_is_preview = false;
				(m_main_preview)->SetActive(false);
				(m_sub_preview)->SetActive(false);

				PREVIEW_INFO maininfo , subinfo;
				maininfo = m_main_preview->GetPreviewInfo();
				subinfo = m_sub_preview->GetPreviewInfo();
				CKeyMgr::GetInstance()->GetInputDevice()->PushCommand(CMyCmd_PartBuilding::StaticCreate(maininfo , subinfo , 9998));
			}
			else
			{
				m_is_preview = true; //설치에 실패하면 프리뷰를 계속 본다.
			}
		}
	}

	return false;
}
bool CStarport::Input_cmd(const int& firstkey , const int& secondkey)
{
	return false;
}
void CStarport::Update_Cmdbtn(void)
{
	const CUI* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();

	if(IDLE == m_unitinfo.state || PRODUCTION == m_unitinfo.state)
	{
		((CUI_Cmd_info*)pui)->Create_Cmdbtn(0 , L"BTN_WRAITH" , BTN_WRAITH , true);

		if(0 < CIngame_UIMgr::GetInstance()->Get_BuildTech(T_STAR_ADDON ,m_eteamnumber))
		{
			((CUI_Cmd_info*)pui)->Create_Cmdbtn(1 , L"BTN_DROPSHIP" , BTN_DROPSHIP , true);

			if(0 < CIngame_UIMgr::GetInstance()->Get_BuildTech(T_SIENCE ,m_eteamnumber))
				((CUI_Cmd_info*)pui)->Create_Cmdbtn(3 , L"BTN_VESSEL" , BTN_VESSEL , true);
			else
				((CUI_Cmd_info*)pui)->Create_Cmdbtn(3 , L"BTN_VESSEL" , BTN_VESSEL , false);

			if(0 < CIngame_UIMgr::GetInstance()->Get_BuildTech(T_BATTLE_ADDON ,m_eteamnumber))
				((CUI_Cmd_info*)pui)->Create_Cmdbtn(4 , L"BTN_BATTLE" , BTN_BATTLE , true);
			else
				((CUI_Cmd_info*)pui)->Create_Cmdbtn(4 , L"BTN_BATTLE" , BTN_BATTLE , false);
		}
		else
		{
			((CUI_Cmd_info*)pui)->Create_Cmdbtn(1 , L"BTN_DROPSHIP" , BTN_DROPSHIP , false);
			((CUI_Cmd_info*)pui)->Create_Cmdbtn(3 , L"BTN_VESSEL" , BTN_VESSEL , false);
			((CUI_Cmd_info*)pui)->Create_Cmdbtn(4 , L"BTN_BATTLE" , BTN_BATTLE , false);
		}

		if(IDLE == m_unitinfo.state)
		{
			((CUI_Cmd_info*)pui)->Create_Cmdbtn(8 , L"BTN_TAKE_OFF" , BTN_TAKE_OFF , true);

			if( NULL == m_partbuilding )
				((CUI_Cmd_info*)pui)->Create_Cmdbtn(6 , L"BTN_STAR_ADDON" , BTN_FAC_ADDON , true);
			else
			{
				if(BUILD == m_partbuilding->GetUnitinfo().state)
					((CUI_Cmd_info*)pui)->clear_btn();
			}
		}
	}
	else if(AIR_IDLE == m_unitinfo.state ||
		TAKE_OFF == m_unitinfo.state)
	{
		((CUI_Cmd_info*)pui)->Create_Cmdbtn(0 , L"BTN_MOVE" , BTN_MOVE , true);
		((CUI_Cmd_info*)pui)->Create_Cmdbtn(1 , L"BTN_STOP" , BTN_STOP , true);
		((CUI_Cmd_info*)pui)->Create_Cmdbtn(8 , L"BTN_LANDING" , BTN_LANDING , true);
	}
}
void CStarport::Update_Wireframe(void)
{
	D3DXVECTOR2 interface_pos = CIngame_UIMgr::GetInstance()->GetMainInterface_pos();

	if(true == CIngame_UIMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.state))
	{		
		CUI* pui = NULL;
		pui = new CUI_Wireframe(L"WIRE_STARPORT" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));
		pui->Initialize();
		CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);

		CFontMgr::GetInstance()->SetInfomation_font(L"Terran Starport" ,interface_pos.x + 320 , interface_pos.y + 390 );

		if(BUILD == m_unitinfo.state)
		{
			CFontMgr::GetInstance()->SetInfomation_font(L"Under construction.." , interface_pos.x + 320 , interface_pos.y + 415);
		}

		if( NULL != m_partbuilding )
		{
			if(BUILD == m_partbuilding->GetUnitinfo().state)
			{
				CFontMgr::GetInstance()->SetInfomation_font(L"Adding on.." , interface_pos.x + 330 , interface_pos.y + 415);

				pui = new CUI_form(L"EDGE" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
				CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);
				
				pui = new CUI_form(L"BTN_STAR_ADDON" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
				CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);
			}
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

	((CCom_Production_building*)m_com_production)->show_production_state();

	if( NULL != m_partbuilding )
	{
		if(BUILD == m_partbuilding->GetUnitinfo().state)
		{
			m_unitinfo.state = ADDING_ON;

			UNITINFO temp = m_partbuilding->GetUnitinfo();
			CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 293 , interface_pos.y + 435) , temp.hp / (float)temp.maxhp );
		}
	}
}
void CStarport::Dead(void)
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
void CStarport::Release(void)
{
	CTerran_building::area_release();

	COMPONENT_PAIR::iterator iter = m_componentlist.find(COM_PATHFINDE);
	Safe_Delete(m_com_pathfind);
	if(iter != m_componentlist.end())
	{
		m_componentlist.erase(iter);
	}
}