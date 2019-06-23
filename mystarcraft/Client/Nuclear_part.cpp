#include "StdAfx.h"
#include "Nuclear_part.h"

#include "Component.h"
#include "Com_PartAnim.h"
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
#include "Comandcenter.h"

#include "UI_Wireframe.h"
#include "UI_Energy_bar.h"

#include "UI_Cmd_info.h"
#include "UI_form.h"
CNuclear_part::CNuclear_part(CObj* pobj)
{
	m_mainbuilding = pobj;
}

CNuclear_part::~CNuclear_part(void)
{
	Release();
}

void CNuclear_part::Initialize(void)
{
	m_com_pathfind = NULL;

	CTerran_building::building_area_Initialize(2 , 2);
	CTerran_building::building_pos_Initialize(2 , 2);

	m_vertex.left = 32.f;//원래 37이던데 이유는 흠...
	m_vertex.right = 32.f;
	m_vertex.top =  16.f;
	m_vertex.bottom = 26.f;

	m_ebuild_tech = T_NC_PART;

	m_sortID = SORT_GROUND;	
	m_ecategory = CATEGORY_BUILDING;
	m_eOBJ_NAME = OBJ_NC_PART;
	m_eteamnumber = m_mainbuilding->GetTeamNumber();

	m_unitinfo.etribe = TRIBE_TERRAN;
	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.state = BUILD;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_LARGE;
	m_unitinfo.maxhp = 600;
	m_unitinfo.hp = 1;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 0;
	m_unitinfo.search_range = 0;
	m_unitinfo.fog_range = 512;
	m_unitinfo.fbuildtime = 10.f;


	m_com_anim = new CCom_PartAnim(L"T_NC_PART" , L"NC_LINK", m_matWorld);

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

	m_nuclear_cnt = 0;
	m_upg_info = CIngame_UIMgr::GetInstance()->GetUpginfo();
}

void CNuclear_part::Update(void)
{
	D3DXVECTOR2 vpos;
	vpos = CMyMath::index_to_Pos(m_curidx32 , 128 , 32);
	vpos.x -= CScrollMgr::m_fScrollX;
	vpos.y -= CScrollMgr::m_fScrollY;
	CFontMgr::GetInstance()->Setbatch_Font(L"%d", m_upg_info[UPG_T_BCN0].upg_cnt, m_vPos.x - CScrollMgr::m_fScrollX, 
		m_vPos.y - CScrollMgr::m_fScrollY);


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

	if(!m_nuclear.empty())
	{
		m_nuclear[0].curtime += GETTIME;
		
		if(m_nuclear[0].curtime >= m_nuclear[0].maxtime)
		{
			m_upg_info[UPG_T_BCN0].upg_cnt += 1;
			m_nuclear.clear();
			m_nuclear_cnt += 1;

			m_unitinfo.state = IDLE;
		}
	}

	CTerran_building::fire_eff_update();

	m_select_ui->Update();
	m_energybar_ui->Update();
}

void CNuclear_part::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_select_ui->Render();
	m_com_anim->Render();
	m_energybar_ui->Render();

	CTerran_building::fire_eff_render();

	CLineMgr::GetInstance()->collisionbox_render(m_rect);
}

void CNuclear_part::Release(void)
{
	CTerran_building::area_release();
}

void CNuclear_part::Dead(void)
{
	CObj* pobj = new CGeneraEff(L"XLARGEBANG" , m_vPos , D3DXVECTOR2(1.f,1.f) , SORT_GROUND);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);

	pobj = new CCorpse(L"" , L"TBDSMALL_WRECKAGE");
	pobj->SetPos(m_vPos.x , m_vPos.y);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddCorpse(pobj);

	if(NULL != m_mainbuilding)
		((CTerran_building*)m_mainbuilding)->SetPartBuilding(NULL);
}
int CNuclear_part::GetNuclear_cnt(void)
{
	return m_nuclear_cnt;
}
void CNuclear_part::SetNuclear_cnt(const int& cnt)
{
	m_nuclear_cnt += cnt;
}
void CNuclear_part::Inputkey_reaction(const int& nkey)
{
	if('N' == nkey)
	{
		if(0 == m_nuclear_cnt)
		{
			m_unitinfo.state = DEVELOPING;
			PRODUCTION_INFO temp;
			temp.maxtime = 5;
			m_nuclear.push_back(temp);

		}
	}
}

void CNuclear_part::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}
bool CNuclear_part::Input_cmd(const int& nkey, bool* waitkey)
{
	if('N' == nkey)
		return true;

	return false;
}
void CNuclear_part::Setlink(bool blink , CObj* pobj)
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
void CNuclear_part::Update_Cmdbtn(void)
{
	const CUI* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();

	if(IDLE == m_unitinfo.state)
	{
		if(0 == m_nuclear_cnt)
			((CUI_Cmd_info*)pui)->Create_Cmdbtn(0 , L"BTN_NC_MISSILE" , BTN_NC_MISSILE);
	}
}

void CNuclear_part::Update_Wireframe(void)
{
	D3DXVECTOR2 interface_pos = CIngame_UIMgr::GetInstance()->GetMainInterface_pos();

	if(true == CIngame_UIMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.state))
	{		
		CUI* pui = NULL;
		pui = new CUI_Wireframe( L"WIRE_NC_PART" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));
		pui->Initialize();
		CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);

		CFontMgr::GetInstance()->SetInfomation_font(L"Terran Nuclear Silo" ,interface_pos.x + 320 , interface_pos.y + 390 );

		if(BUILD == m_unitinfo.state)
		{
			CFontMgr::GetInstance()->SetInfomation_font(L"Under construction" , interface_pos.x + 320 , interface_pos.y + 415);
		}
		else if(DEVELOPING == m_unitinfo.state)
		{
			CFontMgr::GetInstance()->SetInfomation_font(L"Upgrading" , interface_pos.x + 330 , interface_pos.y + 415);

			pui = new CUI_form(L"EDGE" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
			CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);


			if(!m_nuclear.empty())
			{
				pui = new CUI_form(L"BTN_NC_MISSILE" , D3DXVECTOR2(interface_pos.x + 258 , interface_pos.x + 410));
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

	if(!m_nuclear.empty())
	{
		CIngame_UIMgr::GetInstance()->SetProduction_info(D3DXVECTOR2(interface_pos.x + 293 , interface_pos.y + 435) , m_nuclear[0].curtime / m_nuclear[0].maxtime );
	}

}

