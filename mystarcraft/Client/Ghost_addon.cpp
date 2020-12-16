#include "StdAfx.h"
#include "Ghost_addon.h"

#include "Component.h"
#include "Com_PartAnim.h"
#include "Com_Collision.h"
#include "Com_fog.h"

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
#include "Comandcenter.h"

#include "UI_Cmd_info.h"
#include "UI_Wireframe.h"
#include "UI_form.h"

#include "UI_Energy_bar.h"
CGhost_addon::CGhost_addon(CObj* pobj)
{
	m_mainbuilding = pobj;
}

CGhost_addon::~CGhost_addon(void)
{
	Release();
}

void CGhost_addon::Initialize(void)
{
	m_com_pathfind = NULL;

	CTerran_building::building_area_Initialize(2 , 2);
	CTerran_building::building_pos_Initialize(2 , 2);

	m_vertex.left = 32.f;//원래 37이던데 이유는 흠...
	m_vertex.right = 29.f;
	m_vertex.top =  24.f;
	m_vertex.bottom = 23.f;

	m_ebuild_tech = T_GHOST_ADDON;

	m_sortID = SORT_GROUND;	
	m_ecategory = CATEGORY_BUILDING;
	m_eOBJ_NAME = OBJ_GHOST_ADDON;
	m_eteamnumber = m_mainbuilding->GetTeamNumber();

	m_unitinfo.etribe = TRIBE_TERRAN;
	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.state = BUILD;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_LARGE;
	m_unitinfo.maxhp = 750;
	m_unitinfo.hp = 1;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 0;
	m_unitinfo.search_range = 0;
	m_unitinfo.fog_range = 512;
	m_unitinfo.fbuildtime = 10.f;


	m_com_anim = new CCom_PartAnim(L"T_GHOST_ADDON", L"GHOST_LINK" , m_matWorld);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_Collision(m_vPos , m_rect , m_vertex , false) ) ) ;	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_select_ui = new CUI_Select(L"Select94" , m_vPos , 13);
	m_select_ui->Initialize();

	m_energybar_ui = new CUI_Energy_bar(this , 94 , m_vertex.bottom*1.8f);
	m_energybar_ui->Initialize();

	m_fbuild_tick = float(m_unitinfo.maxhp)/m_unitinfo.fbuildtime;
	CTerran_building::fire_eff_initialize();

	m_upg_info = CIngame_UIMgr::GetInstance()->GetUpginfo();
}

void CGhost_addon::Update(void)
{
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
			m_mainbuilding->SetState(IDLE);
		}
	}

	CTerran_building::fire_eff_update();

	CTerran_building::upginfo_update(UPG_T_VIC0);
	CTerran_building::upginfo_update(UPG_T_VIC1);
	CTerran_building::upginfo_update(UPG_T_VIC3);
	CTerran_building::upginfo_update(UPG_T_VIC4);

	m_select_ui->Update();
	m_energybar_ui->Update();
}

void CGhost_addon::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_select_ui->Render();
	m_com_anim->Render();
	m_energybar_ui->Render();

	CTerran_building::fire_eff_render();

	CLineMgr::GetInstance()->collisionbox_render(m_rect);
}

void CGhost_addon::Inputkey_reaction(const int& nkey)
{
	if(DEVELOPING == m_unitinfo.state)
		return;

	if( VK_LBUTTON == nkey )
	{
		const CUI* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();
		CMD_BTN eclicked_btn = ((CUI_Cmd_info*)pui)->Get_clicked_btn();

		if(BTN_T_VIC0 == eclicked_btn)
			Inputkey_reaction('Q');
		else if(BTN_T_VIC0 == eclicked_btn)
			Inputkey_reaction('W');
		else if(BTN_T_VIC3 == eclicked_btn)
			Inputkey_reaction('A');
		else if(BTN_T_VIC4 == eclicked_btn)
			Inputkey_reaction('S');
	}

	if('Q' == nkey)
	{
		if( false == m_upg_info[UPG_T_VIC0].proceeding &&
			m_upg_info[UPG_T_VIC0].upg_cnt == 0)
		{
			m_upg_info[UPG_T_VIC0].proceeding = true;
			m_upg_info[UPG_T_VIC0].obj_num = m_obj_id;
			m_unitinfo.state = DEVELOPING;
		}
	}
	if('W' == nkey)
	{
		if( false == m_upg_info[UPG_T_VIC1].proceeding &&
			m_upg_info[UPG_T_VIC1].upg_cnt == 0)
		{
			m_upg_info[UPG_T_VIC1].proceeding = true;
			m_upg_info[UPG_T_VIC1].obj_num = m_obj_id;
			m_unitinfo.state = DEVELOPING;
		}
	}
	if('A' == nkey)
	{
		if( false == m_upg_info[UPG_T_VIC3].proceeding &&
			m_upg_info[UPG_T_VIC3].upg_cnt == 0)
		{
			m_upg_info[UPG_T_VIC3].proceeding = true;
			m_upg_info[UPG_T_VIC3].obj_num = m_obj_id;
			m_unitinfo.state = DEVELOPING;
		}
	}
	if('S' == nkey)
	{
		if( false == m_upg_info[UPG_T_VIC4].proceeding &&
			m_upg_info[UPG_T_VIC4].upg_cnt == 0)
		{
			m_upg_info[UPG_T_VIC4].proceeding = true;
			m_upg_info[UPG_T_VIC4].obj_num = m_obj_id;
			m_unitinfo.state = DEVELOPING;
		}
	}
}

void CGhost_addon::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}
bool CGhost_addon::Input_cmd(const int& nkey, bool* waitkey)
{
	if( 'Q' == nkey )
		return true;
	if( 'W' == nkey )
		return true;
	if( 'A' == nkey )
		return true;
	if( 'S' == nkey )
		return true;

	return false;
}
void CGhost_addon::Setlink(bool blink , CObj* pobj)
{
	if(true == blink)
	{
		m_mainbuilding = pobj;
		((CCom_PartAnim*)m_com_anim)->play_direction(1);
	}
	else
	{
		m_mainbuilding = NULL;
		((CCom_PartAnim*)m_com_anim)->play_direction(-1);
	}
}
void CGhost_addon::Update_Cmdbtn(void)
{
	CUI_Cmd_info* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();
	if(IDLE == m_unitinfo.state )
	{			
		if(NULL != m_mainbuilding)
		{
			if( false == m_upg_info[UPG_T_VIC0].proceeding && m_upg_info[UPG_T_VIC0].upg_cnt < 1)
				pui->Create_Cmdbtn(0 , L"BTN_T_VIC0" , BTN_T_VIC0 , true , L"Q");
			if( false == m_upg_info[UPG_T_VIC1].proceeding && m_upg_info[UPG_T_VIC1].upg_cnt < 1)
				pui->Create_Cmdbtn(1 , L"BTN_T_VIC1" , BTN_T_VIC1 , true , L"W");
			if( false == m_upg_info[UPG_T_VIC3].proceeding && m_upg_info[UPG_T_VIC3].upg_cnt < 1)
				pui->Create_Cmdbtn(3 , L"BTN_T_VIC3" , BTN_T_VIC3 , true , L"A");
			if( false == m_upg_info[UPG_T_VIC4].proceeding && m_upg_info[UPG_T_VIC4].upg_cnt < 1)
				pui->Create_Cmdbtn(4 , L"BTN_T_VIC4" , BTN_T_VIC4 , true , L"S");
		}

	}
	else if(DEVELOPING == m_unitinfo.state)
		((CUI_Cmd_info*)pui)->Create_Cmdbtn(8 , L"BTN_CANCLE" , BTN_CANCLE , true);
}

void CGhost_addon::Update_Wireframe(void)
{
	D3DXVECTOR2 interface_pos = CIngame_UIMgr::GetInstance()->GetMainInterface_pos();

	if(true == CIngame_UIMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.state))
	{		
		CUI* pui = NULL;
		pui = new CUI_Wireframe( L"WIRE_GHOST_ADDON" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));
		pui->Initialize();
		CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);

		CFontMgr::GetInstance()->SetInfomation_font(L"Terran Covert Ops" ,interface_pos.x + 320 , interface_pos.y + 390 );

		if(BUILD == m_unitinfo.state)
		{
			CFontMgr::GetInstance()->SetInfomation_font(L"Under construction" , interface_pos.x + 320 , interface_pos.y + 415);
		}
		else if(DEVELOPING == m_unitinfo.state)
		{
			CFontMgr::GetInstance()->SetInfomation_font(L"Upgrading" , interface_pos.x + 330 , interface_pos.y + 415);

			pui = new CUI_form(L"EDGE" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
			CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);

			if(true == m_upg_info[UPG_T_VIC0].proceeding && 
				m_upg_info[UPG_T_VIC0].obj_num == m_obj_id)
			{
				pui = new CUI_form(L"BTN_T_VIC0" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
				CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);
			}
			else if(true == m_upg_info[UPG_T_VIC1].proceeding && 
				m_upg_info[UPG_T_VIC1].obj_num == m_obj_id)
			{
				pui = new CUI_form(L"BTN_T_VIC1" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
				CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);
			}
			else if(true == m_upg_info[UPG_T_VIC3].proceeding && 
				m_upg_info[UPG_T_VIC3].obj_num == m_obj_id)
			{
				pui = new CUI_form(L"BTN_T_VIC3" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
				CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);
			}
			else if(true == m_upg_info[UPG_T_VIC4].proceeding && 
				m_upg_info[UPG_T_VIC4].obj_num == m_obj_id)
			{
				pui = new CUI_form(L"BTN_T_VIC4" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
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

	if(true == m_upg_info[UPG_T_VIC0].proceeding && m_upg_info[UPG_T_VIC0].obj_num == m_obj_id)
		CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 293 , interface_pos.y + 435) , m_upg_info[UPG_T_VIC0].curtime / m_upg_info[UPG_T_VIC0].maxtime );
	else if(true == m_upg_info[UPG_T_VIC1].proceeding && m_upg_info[UPG_T_VIC1].obj_num == m_obj_id)
		CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 293 , interface_pos.y + 435) , m_upg_info[UPG_T_VIC1].curtime / m_upg_info[UPG_T_VIC1].maxtime );
	else if(true == m_upg_info[UPG_T_VIC3].proceeding && m_upg_info[UPG_T_VIC3].obj_num == m_obj_id)
		CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 293 , interface_pos.y + 435) , m_upg_info[UPG_T_VIC3].curtime / m_upg_info[UPG_T_VIC3].maxtime );
	else if(true == m_upg_info[UPG_T_VIC4].proceeding && m_upg_info[UPG_T_VIC4].obj_num == m_obj_id)
		CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 293 , interface_pos.y + 435) , m_upg_info[UPG_T_VIC4].curtime / m_upg_info[UPG_T_VIC4].maxtime );
}
void CGhost_addon::Dead(void)
{
	CSoundDevice::GetInstance()->PlayBattleSound(SND_B_TBMIDDLE_BOOM , m_vPos);

	CObj* pobj = new CGeneraEff(L"XLARGEBANG" , m_vPos , D3DXVECTOR2(1.f,1.f) , SORT_GROUND);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);

	pobj = new CCorpse(L"" , L"TBDSMALL_WRECKAGE");
	pobj->SetPos(m_vPos.x , m_vPos.y);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddCorpse(pobj);


	if(NULL != m_mainbuilding)
		((CTerran_building*)m_mainbuilding)->SetPartBuilding(NULL);

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
void CGhost_addon::Release(void)
{
	CTerran_building::area_release();
}
