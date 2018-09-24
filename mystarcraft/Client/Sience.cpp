#include "StdAfx.h"
#include "Sience.h"

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
#include "Ingame_UIMgr.h"

#include "Building_Preview.h"
#include "MouseMgr.h"

#include "Ghost_addon.h"
#include "Battle_addon.h"

#include "UI_Cmd_info.h"
#include "UI_Wireframe.h"
#include "UI_form.h"

#include "UI_Energy_bar.h"
#include "MyCmd_PartBuilding.h"
#include "KeyMgr.h"
#include "Input_Interface.h"
CSience::CSience(void)
{
}

CSience::~CSience(void)
{
	Release();
}

void CSience::Initialize(void)
{
	m_com_pathfind = NULL;

	m_vertex.left = 48.f;
	m_vertex.right = 49.f;
	m_vertex.top =  38.f;
	m_vertex.bottom = 39.f;

	CTerran_building::building_area_Initialize(3 , 4);
	CTerran_building::building_pos_Initialize(3 , 4);

	m_ebuild_tech = T_SIENCE;

	m_sortID = SORT_GROUND;	
	m_ecategory = CATEGORY_BUILDING;
	m_eOBJ_NAME = OBJ_SIENCE;

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

	m_com_anim = new CCom_TBuildingAnim(L"T_SIENCE",m_matWorld );
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

	m_energybar_ui = new CUI_Energy_bar(this , 146 , m_vertex.bottom*1.6f);
	m_energybar_ui->Initialize();

	m_is_take_off = false;

	m_fbuild_tick = float(m_unitinfo.maxhp)/m_unitinfo.fbuildtime;
	CTerran_building::fire_eff_initialize();

	m_upg_info = CIngame_UIMgr::GetInstance()->GetUpginfo();
}

void CSience::Update(void)
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

				Landing_move( ((CBuilding_Preview*)m_sub_preview)->GetPreviewInfo().vpos );
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
				if(T_GHOST_ADDON == ((CBuilding_Preview*)m_main_preview)->GetPreviewInfo().ebuild)
				{
					pobj = new CGhost_addon(this/*아니면 오브젝트 아이디*/);
					pobj->SetPos(((CBuilding_Preview*)m_main_preview)->GetPreviewInfo().vpos);
					pobj->Initialize();				
					CObjMgr::GetInstance()->AddObject(pobj , OBJ_GHOST_ADDON);
				}
				else if(T_BATTLE_ADDON == ((CBuilding_Preview*)m_main_preview)->GetPreviewInfo().ebuild)
				{
					pobj = new CBattle_addon(this);
					pobj->SetPos(((CBuilding_Preview*)m_main_preview)->GetPreviewInfo().vpos);
					pobj->Initialize();				
					CObjMgr::GetInstance()->AddObject(pobj , OBJ_BATTLE_ADDON);
				}
				m_partbuilding = pobj;
				((CTerran_building*)m_partbuilding)->Setlink(true , this);
			}
			else
			{
				int partidx = m_curidx32 + 3 + SQ_TILECNTX;
				m_partbuilding = CArea_Mgr::GetInstance()->Search_Partbuilding(m_curidx64 , partidx , OBJ_GHOST_ADDON);
				if(NULL != m_partbuilding)
					((CTerran_building*)m_partbuilding)->Setlink(true , this);
				else
				{
					m_partbuilding = CArea_Mgr::GetInstance()->Search_Partbuilding(m_curidx64 , partidx , OBJ_BATTLE_ADDON);
					if(NULL != m_partbuilding)
						((CTerran_building*)m_partbuilding)->Setlink(true , this);
				}
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

	//vpos = CMyMath::index_to_Pos(m_curidx32 , 128 , 32);
	//vpos.x -= CScrollMgr::m_fScrollX;
	//vpos.y -= CScrollMgr::m_fScrollY;
	//CFontMgr::GetInstance()->Setbatch_Font(L"@" , m_vPos.x - CScrollMgr::m_fScrollX, 
	//	m_vPos.y - CScrollMgr::m_fScrollY);

	CTerran_building::fire_eff_update();

	CTerran_building::upginfo_update(UPG_T_VI0);
	CTerran_building::upginfo_update(UPG_T_VI1);
	CTerran_building::upginfo_update(UPG_T_VI2);

	m_select_ui->Update();
	m_energybar_ui->Update();
}

void CSience::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_select_ui->Render();
	m_com_anim->Render();
	m_energybar_ui->Render();

	CTerran_building::fire_eff_render();

	CLineMgr::GetInstance()->collisionbox_render(m_rect);
}



void CSience::Inputkey_reaction(const int& nkey)
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
	if('Q' == nkey)
	{
		if( false == m_upg_info[UPG_T_VI0].proceeding &&
			m_upg_info[UPG_T_VI0].upg_cnt == 0)
		{
			m_upg_info[UPG_T_VI0].proceeding = true;
			m_upg_info[UPG_T_VI0].obj_num = m_obj_id;
			m_unitinfo.state = DEVELOPING;
		}
	}
	if('W' == nkey)
	{
		if( false == m_upg_info[UPG_T_VI1].proceeding &&
			m_upg_info[UPG_T_VI1].upg_cnt == 0)
		{
			m_upg_info[UPG_T_VI1].proceeding = true;
			m_upg_info[UPG_T_VI1].obj_num = m_obj_id;
			m_unitinfo.state = DEVELOPING;
		}
	}
	if('E' == nkey)
	{
		if( false == m_upg_info[UPG_T_VI2].proceeding &&
			m_upg_info[UPG_T_VI2].upg_cnt == 0)
		{
			m_upg_info[UPG_T_VI2].proceeding = true;
			m_upg_info[UPG_T_VI2].obj_num = m_obj_id;
			m_unitinfo.state = DEVELOPING;
		}
	}

	if(9998 == nkey)
	{
		m_unitinfo.order = ORDER_LANDING_MOVE;
		//D3DXVECTOR2 vclickpos = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
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
				CObj* pobj = NULL;
				if(T_GHOST_ADDON == ((CBuilding_Preview*)m_main_preview)->GetPreviewInfo().ebuild)
				{
					pobj = new CGhost_addon(this);
					pobj->SetPos(((CBuilding_Preview*)m_main_preview)->GetPreviewInfo().vpos);
					pobj->Initialize();				
					CObjMgr::GetInstance()->AddObject(pobj , OBJ_GHOST_ADDON);
				}
				else if(T_BATTLE_ADDON == ((CBuilding_Preview*)m_main_preview)->GetPreviewInfo().ebuild)
				{
					pobj = new CBattle_addon(this);
					pobj->SetPos(((CBuilding_Preview*)m_main_preview)->GetPreviewInfo().vpos);
					pobj->Initialize();				
					CObjMgr::GetInstance()->AddObject(pobj , OBJ_BATTLE_ADDON);
				}
				m_partbuilding = pobj;
				((CTerran_building*)m_partbuilding)->Setlink(true , this);
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
}

void CSience::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}
void CSience::Input_cmd(const int& nkey, bool* waitkey)
{
	if(TAKE_OFF == m_unitinfo.state ||
		LANDING == m_unitinfo.state)
		return;

	if(VK_LBUTTON == nkey)
	{
		const CUI* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();
		CMD_BTN eclicked_btn = ((CUI_Cmd_info*)pui)->Get_clicked_btn();

		if(BTN_TAKE_OFF == eclicked_btn)
		{
			Inputkey_reaction('L');
			return;
		}
		if(BTN_LANDING == eclicked_btn)
		{
			Inputkey_reaction('L');
			return;
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

	MYRECT<float> tempvtx;
	if('C' == nkey)
	{
		m_is_preview = true;
		((CBuilding_Preview*)m_main_preview)->SetPreviewInfo(L"T_GHOST_ADDON", T_GHOST_ADDON , 2 , 2 ,  tempvtx);
		((CBuilding_Preview*)m_sub_preview)->SetPreviewInfo(L"T_SIENCE", T_SIENCE , 3 , 4 ,  m_vertex);
		waitkey[nkey] = false;
	}
	if('P' == nkey)
	{
		m_is_preview = true;
		((CBuilding_Preview*)m_main_preview)->SetPreviewInfo(L"T_BATTLE_ADDON", T_BATTLE_ADDON , 2 , 2 ,  tempvtx);
		((CBuilding_Preview*)m_sub_preview)->SetPreviewInfo(L"T_SIENCE", T_SIENCE , 3 , 4 ,  m_vertex);
		waitkey[nkey] = false;
	}

	if('L' == nkey)
	{
		if(true == m_is_take_off)
		{
			//이륙
			m_is_preview = true;
			(m_main_preview)->SetPreviewInfo(L"T_SIENCE", T_SIENCE , 3 , 4 ,  m_vertex);			
		}
	}
}
void CSience::Input_cmd(const int& firstkey , const int& secondkey)
{

}
void CSience::Update_Cmdbtn(void)
{
	const CUI* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();
	
	if(IDLE == m_unitinfo.state)
	{
		if( NULL == m_partbuilding )
		{
			((CUI_Cmd_info*)pui)->Create_Cmdbtn(6 , L"BTN_GHOST_ADDON" , BTN_GHOST_ADDON , true);
			((CUI_Cmd_info*)pui)->Create_Cmdbtn(7 , L"BTN_BATTLE_ADDON" , BTN_BATTLE_ADDON , true);
		}
	
		if( false == m_upg_info[UPG_T_VI0].proceeding && m_upg_info[UPG_T_VI0].upg_cnt < 1)
		{
			((CUI_Cmd_info*)pui)->Create_Cmdbtn(0 , L"BTN_T_VI0" , BTN_T_VI0 , true);
		}
		if( false == m_upg_info[UPG_T_VI1].proceeding && m_upg_info[UPG_T_VI1].upg_cnt < 1)
		{
			((CUI_Cmd_info*)pui)->Create_Cmdbtn(1 , L"BTN_T_VI1" , BTN_T_VI1 , true);
		}
		if( false == m_upg_info[UPG_T_VI2].proceeding && m_upg_info[UPG_T_VI2].upg_cnt < 1)
		{
			((CUI_Cmd_info*)pui)->Create_Cmdbtn(2 , L"BTN_T_VI2" , BTN_T_VI2 , true);
		}

		((CUI_Cmd_info*)pui)->Create_Cmdbtn(8 , L"BTN_TAKE_OFF" , BTN_TAKE_OFF , true);
	}
	else if(AIR_IDLE == m_unitinfo.state ||
		TAKE_OFF == m_unitinfo.state)
	{
		((CUI_Cmd_info*)pui)->Create_Cmdbtn(0 , L"BTN_MOVE" , BTN_MOVE , true);
		((CUI_Cmd_info*)pui)->Create_Cmdbtn(1 , L"BTN_STOP" , BTN_STOP , true);
		((CUI_Cmd_info*)pui)->Create_Cmdbtn(8 , L"BTN_LANDING" , BTN_LANDING , true);
	}
	else if(DEVELOPING == m_unitinfo.state)
		((CUI_Cmd_info*)pui)->Create_Cmdbtn(8 , L"BTN_CANCLE" , BTN_CANCLE , true);
}
void CSience::Update_Wireframe(void)
{
	D3DXVECTOR2 interface_pos = CIngame_UIMgr::GetInstance()->GetMainInterface_pos();

	if(true == CIngame_UIMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.state))
	{		
		CUI* pui = NULL;
		pui = new CUI_Wireframe(L"WIRE_SIENCE" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));
		pui->Initialize();
		CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);

		CFontMgr::GetInstance()->SetInfomation_font(L"Terran Sience" ,interface_pos.x + 320 , interface_pos.y + 390 );

		if(BUILD == m_unitinfo.state)
		{
			CFontMgr::GetInstance()->SetInfomation_font(L"Under construction.." , interface_pos.x + 320 , interface_pos.y + 415);
		}
		else if(DEVELOPING == m_unitinfo.state)
		{
			CFontMgr::GetInstance()->SetInfomation_font(L"Upgrading" , interface_pos.x + 330 , interface_pos.y + 415);

			pui = new CUI_form(L"EDGE" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
			CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);

			if(true == m_upg_info[UPG_T_VI0].proceeding && 
				m_upg_info[UPG_T_VI0].obj_num == m_obj_id)
			{
				pui = new CUI_form(L"BTN_T_VI0" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
				CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);
			}
			else if(true == m_upg_info[UPG_T_VI1].proceeding && 
				m_upg_info[UPG_T_VI1].obj_num == m_obj_id)
			{
				pui = new CUI_form(L"BTN_T_VI1" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
				CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);
			}
			else if(true == m_upg_info[UPG_T_VI2].proceeding && 
				m_upg_info[UPG_T_VI2].obj_num == m_obj_id)
			{
				pui = new CUI_form(L"BTN_T_VI2" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
				CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);
			}
		}

		if( NULL != m_partbuilding )
		{
			if(BUILD == m_partbuilding->GetUnitinfo().state)
			{
				CFontMgr::GetInstance()->SetInfomation_font(L"Adding on.." , interface_pos.x + 330 , interface_pos.y + 415);

				pui = new CUI_form(L"EDGE" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
				CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);
				if(OBJ_BATTLE_ADDON == m_partbuilding->GetOBJNAME())
				{
					pui = new CUI_form(L"BTN_BATTLE_ADDON" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
					CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);
				}
				else if(OBJ_GHOST_ADDON == m_partbuilding->GetOBJNAME())
				{
					pui = new CUI_form(L"BTN_GHOST_ADDON" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
					CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);
				}
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

	if(true == m_upg_info[UPG_T_VI0].proceeding && m_upg_info[UPG_T_VI0].obj_num == m_obj_id)
		CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 293 , interface_pos.y + 435) , m_upg_info[UPG_T_VI0].curtime / m_upg_info[UPG_T_VI0].maxtime );
	else if(true == m_upg_info[UPG_T_VI1].proceeding && m_upg_info[UPG_T_VI1].obj_num == m_obj_id)
		CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 293 , interface_pos.y + 435) , m_upg_info[UPG_T_VI1].curtime / m_upg_info[UPG_T_VI1].maxtime );
	else if(true == m_upg_info[UPG_T_VI2].proceeding && m_upg_info[UPG_T_VI2].obj_num == m_obj_id)
		CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 293 , interface_pos.y + 435) , m_upg_info[UPG_T_VI2].curtime / m_upg_info[UPG_T_VI2].maxtime );

	if( NULL != m_partbuilding )
	{
		if(BUILD == m_partbuilding->GetUnitinfo().state)
		{
			m_unitinfo.state = ADDING_ON;

			UNITINFO temp = m_partbuilding->GetUnitinfo();
			CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 293 , interface_pos.y + 435) , temp.hp / (float)temp.maxhp );
		}
	}
	//-------------------------------------------
}
void CSience::Release(void)
{
	CTerran_building::area_release();

	COMPONENT_PAIR::iterator iter = m_componentlist.find(COM_PATHFINDE);
	Safe_Delete(m_com_pathfind);
	if(iter != m_componentlist.end())
	{
		m_componentlist.erase(iter);
	}
}

void CSience::Dead(void)
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