#include "StdAfx.h"
#include "Academy.h"

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

#include "UI_Cmd_info.h"
#include "UI_Wireframe.h"
#include "UI_form.h"
#include "UI_Energy_bar.h"
CAcademy::CAcademy(void)
{
}

CAcademy::~CAcademy(void)
{
	Release();
}

void CAcademy::Initialize(void)
{
	m_com_pathfind = NULL;

	m_vertex.left = 40.f;
	m_vertex.right = 43.f;
	m_vertex.top =  32.f;
	m_vertex.bottom = 25.f;

	CTerran_building::building_area_Initialize(2 , 3);
	CTerran_building::building_pos_Initialize(2 , 3);

	m_ebuild_tech = T_ACADEMY;

	m_sortID = SORT_GROUND;	
	m_ecategory = CATEGORY_BUILDING;
	m_eOBJ_NAME = OBJ_ACADEMY;

	m_unitinfo.etribe = TRIBE_TERRAN;
	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.state = BUILD;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_LARGE;
	m_unitinfo.maxhp = 600;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 28;
	m_unitinfo.search_range = 0;
	m_unitinfo.fog_range = 512;
	m_unitinfo.fbuildtime = 10.f;

	m_com_anim = new CCom_TBuildingAnim(L"T_ACADEMY" , m_matWorld);

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

	m_upg_info = CIngame_UIMgr::GetInstance()->GetUpginfo();
}

void CAcademy::Update(void)
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

	CTerran_building::upginfo_update(UPG_T_STEAMPACK);
	CTerran_building::upginfo_update(UPG_T_BA0);
	CTerran_building::upginfo_update(UPG_T_BA3);
	CTerran_building::upginfo_update(UPG_T_BA4);
	CTerran_building::upginfo_update(UPG_T_BA5);

	m_select_ui->Update();
	m_energybar_ui->Update();
}

void CAcademy::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_select_ui->Render();
	m_com_anim->Render();
	m_energybar_ui->Render();

	CTerran_building::fire_eff_render();

	CLineMgr::GetInstance()->collisionbox_render(m_rect);
}

void CAcademy::Inputkey_reaction(const int& nkey)
{
	if(DEVELOPING == m_unitinfo.state)
		return;

	if( VK_LBUTTON == nkey )
	{
		const CUI* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();
		CMD_BTN eclicked_btn = ((CUI_Cmd_info*)pui)->Get_clicked_btn();

		if(BTN_STEAMPACK == eclicked_btn)
			Inputkey_reaction('W');
		else if(BTN_T_BA0 == eclicked_btn)
			Inputkey_reaction('Q');
		else if(BTN_T_BA3 == eclicked_btn)
			Inputkey_reaction('A');
		else if(BTN_T_BA4 == eclicked_btn)
			Inputkey_reaction('S');
		else if(BTN_T_BA5 == eclicked_btn)
			Inputkey_reaction('D');
	}

	if( 'Q' == nkey )
	{
		if( false == m_upg_info[UPG_T_BA0].proceeding &&
			m_upg_info[UPG_T_BA0].upg_cnt == 0)
		{
			m_upg_info[UPG_T_BA0].proceeding = true;
			m_upg_info[UPG_T_BA0].obj_num = m_obj_id;
			m_unitinfo.state = DEVELOPING;
		}
	}
	if( 'W' == nkey )
	{
		if( false == m_upg_info[UPG_T_STEAMPACK].proceeding &&
			m_upg_info[UPG_T_STEAMPACK].upg_cnt == 0)
		{
			m_upg_info[UPG_T_STEAMPACK].proceeding = true;
			m_upg_info[UPG_T_STEAMPACK].obj_num = m_obj_id;
			m_unitinfo.state = DEVELOPING;
		}
	}
	if( 'A' == nkey )
	{
		if( false == m_upg_info[UPG_T_BA3].proceeding &&
			m_upg_info[UPG_T_BA3].upg_cnt == 0)
		{
			m_upg_info[UPG_T_BA3].proceeding = true;
			m_upg_info[UPG_T_BA3].obj_num = m_obj_id;
			m_unitinfo.state = DEVELOPING;
		}
	}
	if( 'S' == nkey )
	{
		if( false == m_upg_info[UPG_T_BA4].proceeding &&
			m_upg_info[UPG_T_BA4].upg_cnt == 0)
		{
			m_upg_info[UPG_T_BA4].proceeding = true;
			m_upg_info[UPG_T_BA4].obj_num = m_obj_id;
			m_unitinfo.state = DEVELOPING;
		}
	}
	if( 'D' == nkey )
	{
		if( false == m_upg_info[UPG_T_BA5].proceeding &&
			m_upg_info[UPG_T_BA5].upg_cnt == 0)
		{
			m_upg_info[UPG_T_BA5].proceeding = true;
			m_upg_info[UPG_T_BA5].obj_num = m_obj_id;
			m_unitinfo.state = DEVELOPING;
		}
	}
}

void CAcademy::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}
bool CAcademy::Input_cmd(const int& nkey, bool* waitkey)
{
	if( 'Q' == nkey )
		return true;
	if( 'W' == nkey )
		return true;
	if( 'A' == nkey )
		return true;
	if( 'S' == nkey )
		return true;
	if( 'D' == nkey )
		return true;

	return false;
}

bool CAcademy::Input_cmd(const int& firstkey , const int& secondkey)
{
	return false;
}
void CAcademy::Update_Cmdbtn(void)
{
	CUI_Cmd_info* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();

	if(IDLE == m_unitinfo.state)
	{
		if( false == m_upg_info[UPG_T_BA0].proceeding && m_upg_info[UPG_T_BA0].upg_cnt < 1)
			pui->Create_Cmdbtn(0 , L"BTN_T_BA0" , BTN_T_BA0 , true , L"Q");
		if( false == m_upg_info[UPG_T_STEAMPACK].proceeding && m_upg_info[UPG_T_STEAMPACK].upg_cnt < 1)
			pui->Create_Cmdbtn(1 , L"BTN_STEAMPACK" , BTN_STEAMPACK , true , L"W");
		if( false == m_upg_info[UPG_T_BA3].proceeding && m_upg_info[UPG_T_BA3].upg_cnt < 1)
			pui->Create_Cmdbtn(3 , L"BTN_T_BA3" , BTN_T_BA3 , true , L"E");
		if( false == m_upg_info[UPG_T_BA4].proceeding && m_upg_info[UPG_T_BA4].upg_cnt < 1)
			pui->Create_Cmdbtn(4 , L"BTN_T_BA4" , BTN_T_BA4 , true , L"A");
		if( false == m_upg_info[UPG_T_BA5].proceeding && m_upg_info[UPG_T_BA5].upg_cnt < 1)
			pui->Create_Cmdbtn(5 , L"BTN_T_BA5" , BTN_T_BA5 , true , L"S");
	}
	else if(DEVELOPING == m_unitinfo.state)
		pui->Create_Cmdbtn(8 , L"BTN_CANCLE" , BTN_CANCLE , true);
	else if(AIR_IDLE == m_unitinfo.state ||
		TAKE_OFF == m_unitinfo.state)
	{
		pui->Create_Cmdbtn(0 , L"BTN_MOVE" , BTN_MOVE , true);
		pui->Create_Cmdbtn(1 , L"BTN_STOP" , BTN_STOP , true);
		pui->Create_Cmdbtn(8 , L"BTN_LANDING" , BTN_LANDING , true);
	}

}

void CAcademy::Update_Wireframe(void)
{
	D3DXVECTOR2 interface_pos = CIngame_UIMgr::GetInstance()->GetMainInterface_pos();

	if(true == CIngame_UIMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.state))
	{		
		CUI* pui = NULL;
		pui = new CUI_Wireframe(L"WIRE_ACADEMY" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));
		pui->Initialize();
		CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);

		CFontMgr::GetInstance()->SetInfomation_font(L"Terran Academy" ,interface_pos.x + 320 , interface_pos.y + 390 );

		if(BUILD == m_unitinfo.state)
		{
			CFontMgr::GetInstance()->SetInfomation_font(L"Under construction.." , interface_pos.x + 320 , interface_pos.y + 415);
		}
		else if(DEVELOPING == m_unitinfo.state)
		{
			CFontMgr::GetInstance()->SetInfomation_font(L"Upgrading" , interface_pos.x + 330 , interface_pos.y + 415);

			pui = new CUI_form(L"EDGE" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
			CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);

			if(true == m_upg_info[UPG_T_BA0].proceeding && 
				m_upg_info[UPG_T_BA0].obj_num == m_obj_id)
			{
				pui = new CUI_form(L"BTN_T_BA0" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
				CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);
			}
			else if(true == m_upg_info[UPG_T_STEAMPACK].proceeding && 
				m_upg_info[UPG_T_STEAMPACK].obj_num == m_obj_id)
			{
				pui = new CUI_form(L"BTN_STEAMPACK" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
				CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);
			}
			else if(true == m_upg_info[UPG_T_BA3].proceeding && 
				m_upg_info[UPG_T_BA3].obj_num == m_obj_id)
			{
				pui = new CUI_form(L"BTN_T_BA3" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
				CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);
			}
			else if(true == m_upg_info[UPG_T_BA4].proceeding && 
				m_upg_info[UPG_T_BA4].obj_num == m_obj_id)
			{
				pui = new CUI_form(L"BTN_T_BA4" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
				CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);
			}
			else if(true == m_upg_info[UPG_T_BA5].proceeding && 
				m_upg_info[UPG_T_BA5].obj_num == m_obj_id)
			{
				pui = new CUI_form(L"BTN_T_BA5" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
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

	if(true == m_upg_info[UPG_T_STEAMPACK].proceeding && m_upg_info[UPG_T_STEAMPACK].obj_num == m_obj_id)
		CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 293 , interface_pos.y + 435) , m_upg_info[UPG_T_STEAMPACK].curtime / m_upg_info[UPG_T_STEAMPACK].maxtime );
	else if(true == m_upg_info[UPG_T_BA0].proceeding && m_upg_info[UPG_T_BA0].obj_num == m_obj_id)
		CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 293 , interface_pos.y + 435) , m_upg_info[UPG_T_BA0].curtime / m_upg_info[UPG_T_BA0].maxtime );
	else if(true == m_upg_info[UPG_T_BA3].proceeding && m_upg_info[UPG_T_BA3].obj_num == m_obj_id)
		CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 293 , interface_pos.y + 435) , m_upg_info[UPG_T_BA3].curtime / m_upg_info[UPG_T_BA3].maxtime );
	else if(true == m_upg_info[UPG_T_BA4].proceeding && m_upg_info[UPG_T_BA4].obj_num == m_obj_id)
		CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 293 , interface_pos.y + 435) , m_upg_info[UPG_T_BA4].curtime / m_upg_info[UPG_T_BA4].maxtime );
	else if(true == m_upg_info[UPG_T_BA5].proceeding && m_upg_info[UPG_T_BA5].obj_num == m_obj_id)
		CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 293 , interface_pos.y + 435) , m_upg_info[UPG_T_BA5].curtime / m_upg_info[UPG_T_BA5].maxtime );
	//-------------------------------------------
}
void CAcademy::Dead(void)
{
	CObj* pobj = new CGeneraEff(L"XLARGEBANG" , m_vPos , D3DXVECTOR2(1.f,1.f) , SORT_GROUND);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);


	pobj = new CCorpse(L"" , L"TBDSMALL_WRECKAGE");
	pobj->SetPos(m_vPos.x , m_vPos.y);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddCorpse(pobj);


	for(int i = 0; i < UPG_END; ++i)
	{
		if( true == m_upg_info[i].proceeding &&
			m_obj_id == m_upg_info[i].obj_num)
		{
			m_upg_info[i].proceeding = false;
			m_upg_info[i].obj_num = 0;
			m_upg_info[i].curtime = 0;
			break;
		}
	}
}
void CAcademy::Release(void)
{
	CTerran_building::area_release();
}