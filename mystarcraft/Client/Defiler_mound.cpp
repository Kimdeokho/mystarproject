#include "StdAfx.h"
#include "Defiler_mound.h"

#include "MyMath.h"
#include "TimeMgr.h"
#include "Ingame_UIMgr.h"
#include "MyMath.h"
#include "ScrollMgr.h"
#include "Ingame_UIMgr.h"
#include "FontMgr.h"
#include "ObjMgr.h"
#include "UnitMgr.h"
#include "LineMgr.h"

#include "Com_fog.h"
#include "Com_Creep.h"
#include "Com_ZBuildingAnim.h"
#include "Com_Creep.h"
#include "Com_Collision.h"

#include "UI_Wireframe.h"
#include "UI_Select.h"
#include "UI_Energy_bar.h"
#include "UI_Cmd_info.h"
#include "UI_form.h"

#include "Corpse.h"
#include "GeneraEff.h"
CDefiler_mound::CDefiler_mound(void)
{
}

CDefiler_mound::~CDefiler_mound(void)
{
}
void CDefiler_mound::Initialize(void)
{
	m_vertex.left = 64-16;
	m_vertex.right = 64-15;
	m_vertex.top =  32;
	m_vertex.bottom = 32-27;

	CZerg_building::building_area_Initialize(2 , 4);
	CZerg_building::building_pos_Initialize(2 , 4);

	m_eOBJ_NAME = OBJ_DEFILER_MOUND;
	m_sortID = SORT_GROUND;
	m_ecategory = CATEGORY_BUILDING;

	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.state = BUILD;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_LARGE;	
	m_unitinfo.maxhp = 850;
	m_unitinfo.hp = 0;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 0;
	m_unitinfo.search_range = 0;
	m_unitinfo.fog_range = 32*8*2;
	m_unitinfo.fbuildtime = 10.f;
	m_unitinfo.armor = 1;

	m_com_anim = new CCom_ZBuildingAnim(L"Z_DEFILER_MOUND" , m_matWorld);

	m_com_anim->SetTextureName(L"Z_COCOON");
	m_com_anim->SetAnimation(L"BUILD_S");


	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_Collision(m_vPos , m_rect , m_vertex , false) ) );	


	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_select_ui = new CUI_Select(L"Select122" , m_vPos , 10);
	m_select_ui->Initialize();

	m_energybar_ui = new CUI_Energy_bar(this , 110 , m_vertex.top*1.7f);
	m_energybar_ui->Initialize();

	m_fbuild_tick = float(m_unitinfo.maxhp)/m_unitinfo.fbuildtime;
	m_build_maxhp = m_unitinfo.maxhp;
}

void CDefiler_mound::Update(void)
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
		m_unitinfo.hp = (int)m_build_hp;

		if( 2 > (m_build_maxhp / m_build_hp) )
			m_com_anim->SetAnimation(L"BUILD_M");
		else
			m_com_anim->SetAnimation(L"BUILD_S");

		if(m_build_hp >= m_build_maxhp)
		{
			if(NULL ==GetComponent(COM_CREEP))
			{
				m_com_creep = new CCom_Creep(false);
				m_componentlist.insert(COMPONENT_PAIR::value_type(COM_CREEP , m_com_creep ));
				m_com_creep->Initialize(this);
			}

			m_com_anim->SetAnimation(L"BIRTH_M");

			m_unitinfo.hp = m_unitinfo.maxhp;
			m_unitinfo.maxhp = m_unitinfo.maxhp;
			m_unitinfo.state = STATE_NONE;

			CIngame_UIMgr::GetInstance()->BuildTech_Update(Z_DEFILER_MOUND , 1 , m_eteamnumber);
		}
	}

	m_select_ui->Update();
	m_energybar_ui->Update();
	//CFontMgr::GetInstance()->Setnumber_combine_Font(L"%d" , 111 , 400 , 300);

	for(int i = UPG_Z_VF0; i <= UPG_Z_VF0; ++i)
		CZerg_building::upginfo_update((UPGRADE)i);
}

void CDefiler_mound::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_select_ui->Render();

	m_com_anim->Render();

	m_energybar_ui->Render();

	CLineMgr::GetInstance()->collisionbox_render(m_rect);
}
void CDefiler_mound::Inputkey_reaction(const int& nkey)
{
	if('Q' == nkey)
	{
		if( false == m_upg_info[UPG_Z_VF0].proceeding &&
			m_upg_info[UPG_Z_VF0].upg_cnt < 1)
		{
			m_upg_info[UPG_Z_VF0].proceeding = true;
			m_upg_info[UPG_Z_VF0].obj_num = m_obj_id;
			m_unitinfo.state = DEVELOPING;

		}
	}
}
void CDefiler_mound::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}
bool CDefiler_mound::Input_cmd(const int& nkey , bool* waitkey)
{
	if('Q' == nkey)
		return true;

	return false;
}
void CDefiler_mound::Update_Cmdbtn(void)
{
	CUI_Cmd_info* pcmd = CIngame_UIMgr::GetInstance()->GetCmd_info();
	if(IDLE == m_unitinfo.state )
	{
		if( !m_upg_info[UPG_Z_VF0].proceeding && m_upg_info[UPG_Z_VF0].upg_cnt == 0)
				pcmd->Create_Cmdbtn(0 , L"BTN_Z_VF0" , BTN_Z_VF0 , true , L"Q");
	}
	else if(DEVELOPING == m_unitinfo.state)
		pcmd->Create_Cmdbtn(8 , L"BTN_CANCLE" , BTN_CANCLE , true);
}

void CDefiler_mound::Update_Wireframe(void)
{
	D3DXVECTOR2 interface_pos = CIngame_UIMgr::GetInstance()->GetMainInterface_pos();

	if(true == CIngame_UIMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.state))
	{		
		CUI* pui = NULL;	

		pui = new CUI_Wireframe(L"WIRE_DEFILER_MOUND" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));
		CFontMgr::GetInstance()->SetInfomation_font(L"Zerg DefilerMound" ,interface_pos.x + 320 , interface_pos.y + 390 );

		pui->Initialize();
		CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);

		if(BUILD == m_unitinfo.state)
		{
			CFontMgr::GetInstance()->SetInfomation_font(L"Under construction" , interface_pos.x + 320 , interface_pos.y + 415);
		}
		else if(DEVELOPING == m_unitinfo.state)
		{
			CFontMgr::GetInstance()->SetInfomation_font(L"Upgrading" , interface_pos.x + 330 , interface_pos.y + 415);

			pui = new CUI_form(L"EDGE" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
			CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);

			if(true == m_upg_info[UPG_Z_VF0].proceeding && 
				m_upg_info[UPG_Z_VF0].obj_num == m_obj_id)
			{
				pui = new CUI_form(L"BTN_Z_VF0" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
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
		CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 260 , interface_pos.y + 435) , m_build_hp / (float)m_build_maxhp );

	for(int i = BTN_Z_VF0; i <= BTN_Z_VF0; ++i)
	{
		if(true == m_upg_info[i].proceeding && m_upg_info[i].obj_num == m_obj_id)
		{
			CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 293 , interface_pos.y + 435) , m_upg_info[i].curtime / m_upg_info[i].maxtime );
			break;
		}
	}
}

void CDefiler_mound::Dead(void)
{
	CObj* pobj = new CGeneraEff(L"BLOOD_BOOM" , m_vPos , D3DXVECTOR2(1.f,1.f) , SORT_GROUND);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);

	pobj = new CCorpse(L"" , L"ZBD_S_WRECKAGE");
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

void CDefiler_mound::Release(void)
{
	CZerg_building::area_release();
}
