#include "StdAfx.h"
#include "Wraith.h"

#include "ScrollMgr.h"
#include "TimeMgr.h"
#include "MouseMgr.h"
#include "ComanderMgr.h"

#include "Com_fog.h"
#include "Com_Airsearch.h"
#include "Com_WraithAnim.h"
#include "Com_AirPathfind.h"
#include "Com_WWraith.h"
#include "Com_AirCollision.h"
#include "Com_CC.h"

#include "ObjMgr.h"
#include "LineMgr.h"
#include "FontMgr.h"
#include "UI_Select.h"

#include "UnitMgr.h"
#include "Area_Mgr.h"

#include "GeneraEff.h"
#include "MyMath.h"

#include "UI_Cmd_info.h"
#include "UI_Wireframe.h"
#include "UI_Energy_bar.h"
#include "Skill_Defensive.h"
CWraith::CWraith(void)
{
}

CWraith::~CWraith(void)
{
	Release();
}

void CWraith::Initialize(void)
{
	CObj::unit_area_Initialize();
	CUnit::Initialize(); //맵 디스플레이

	m_sortID = SORT_AIR;	
	m_ecategory = CATEGORY_UNIT;
	m_eteamnumber = TEAM_0;
	m_eOBJ_NAME = OBJ_WRAITH;

	m_unitinfo.eMoveType = MOVE_AIR;
	m_unitinfo.state = IDLE;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.esize = SIZE_MEDIUM;
	m_unitinfo.erace = OBJRACE_MECHANIC;
	m_unitinfo.eArmorType = ARMOR_LARGE;
	m_unitinfo.hp = 120;
	m_unitinfo.maxhp = 120;
	m_unitinfo.mp = 50;
	m_unitinfo.maxmp = 200;
	m_unitinfo.fspeed = 113;
	m_unitinfo.attack_range = 5*32;
	m_unitinfo.air_attack_range = 5*32;
	m_unitinfo.search_range = 6*32;
	m_unitinfo.fog_range = 512;

	m_vertex.left = 19;
	m_vertex.right = 19;
	m_vertex.top =  15;
	m_vertex.bottom = 15;

	m_com_targetsearch = new CCom_Airsearch();
	m_com_anim = new CCom_WraithAnim(m_matWorld);
	m_com_pathfind = new CCom_AirPathfind(m_vPos);
	m_com_cc = new CCom_CC();

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_CC ,  m_com_cc )) ;	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_TARGET_SEARCH ,  m_com_targetsearch ) );
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_AIR_PATHFIND , m_com_pathfind));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_WEAPON , new CCom_WWraith()));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_AirCollision(m_vPos , m_rect , m_vertex)));

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_select_ui = new CUI_Select(L"Select32" , m_vPos , 13);
	m_select_ui->Initialize();

	m_energybar_ui = new CUI_Energy_bar(this , 32 , m_vertex.bottom*2);
	m_energybar_ui->Initialize();

	m_upg_info = CComanderMgr::GetInstance()->GetUpginfo();
}

void CWraith::Update(void)
{
	CObj::area_update();	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();

	m_select_ui->Update();
	m_energybar_ui->Update();
}

void CWraith::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	
	m_select_ui->Render();
	m_com_anim->Render();	
	m_com_cc->Render();
	m_energybar_ui->Render();

	CLineMgr::GetInstance()->collisionbox_render(m_rect);
}

void CWraith::Inputkey_reaction(const int& nkey)
{
	if( false == m_unitinfo.is_active )
		return;

	if(VK_RBUTTON == nkey)
	{
		m_unitinfo.state = MOVE;
		m_unitinfo.order = ORDER_MOVE;

		CObj* ptarget = CArea_Mgr::GetInstance()->GetChoiceTarget();
		((CCom_Targetsearch*)m_com_targetsearch)->SetTarget(ptarget);

		D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();
		((CCom_AirPathfind*)m_com_pathfind)->SetGoalPos(goalpos , m_bmagicbox);
	}
	if('W' == nkey)
	{
		m_eteamnumber = TEAM_1;
		//m_skill_sp->use();
	}
	if('A' == nkey)
	{		
	}
	if('Z' == nkey)
	{
	}
}

void CWraith::Inputkey_reaction(const int& firstkey , const int& secondkey)
{
	if('A' == firstkey && VK_LBUTTON == secondkey)
	{
		m_unitinfo.order = ORDER_MOVE_ATTACK;
		m_unitinfo.state = MOVE;
		((CCom_Targetsearch*)m_com_targetsearch)->SetTarget(CArea_Mgr::GetInstance()->GetChoiceTarget());

		if(NULL != m_com_pathfind)
		{
			D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();
			((CCom_AirPathfind*)m_com_pathfind)->SetGoalPos(goalpos , m_bmagicbox);

			m_bmagicbox = false;
		}
	}
}
void CWraith::SetDamage(const int& idamage , DAMAGE_TYPE edamagetype)
{
	CSkill* pskill = ((CCom_CC*)m_com_cc)->GetDefensive();

	float tempdamage = 0.f;

	if(NULL != pskill)
	{
		int shild = ((CSkill_Defensive*)pskill)->GetShild();
		((CSkill_Defensive*)pskill)->SetDamage(idamage);

		if( shild - idamage > 0)
			return;
		else
			tempdamage = float(idamage - shild); 
	}
	else	
		tempdamage = (float)idamage - (m_unitinfo.armor + m_upg_info[UPG_T_AIR_ARMOR].upg_cnt);

	if( ARMOR_SMALL == m_unitinfo.eArmorType)
	{
		if(DAMAGE_BOOM == edamagetype)
			tempdamage *= 0.5f;
	}
	else if( ARMOR_MEDIUM == m_unitinfo.eArmorType)
	{
		if(DAMAGE_VIBRATE == edamagetype)
			tempdamage *= 0.5f;
		else if(DAMAGE_BOOM == edamagetype)
			tempdamage *= 0.75f;
	}
	else if( ARMOR_LARGE == m_unitinfo.eArmorType)
	{
		if(DAMAGE_VIBRATE == edamagetype)
			tempdamage *= 0.25f;
	}

	m_unitinfo.hp -= (int)(tempdamage + 0.5f);

	if(m_unitinfo.hp <= 0)
	{
		//킬수 + 1 하면 되는디..
		m_unitinfo.hp = 0;
		m_bdestroy = true;
		Dead();
	}
	if(m_unitinfo.hp >= m_unitinfo.maxhp)
		m_unitinfo.hp = m_unitinfo.maxhp;
}
void CWraith::Update_Cmdbtn(void)
{
	const CUI* pui = CComanderMgr::GetInstance()->GetCmd_info();

	((CUI_Cmd_info*)pui)->Create_Cmdbtn(0 , L"BTN_MOVE" , BTN_MOVE);
	((CUI_Cmd_info*)pui)->Create_Cmdbtn(1 , L"BTN_STOP" , BTN_STOP);
	((CUI_Cmd_info*)pui)->Create_Cmdbtn(2 , L"BTN_ATTACK" , BTN_ATTACK);
	((CUI_Cmd_info*)pui)->Create_Cmdbtn(3 , L"BTN_PATROL" , BTN_PATROL);
	((CUI_Cmd_info*)pui)->Create_Cmdbtn(4 , L"BTN_HOLD" , BTN_HOLD);
}
void CWraith::Update_Wireframe(void)
{
	D3DXVECTOR2 interface_pos = CComanderMgr::GetInstance()->GetMainInterface_pos();

	if(true == CComanderMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.state))
	{
		CUI* pui = NULL;
		pui = new CUI_Wireframe(L"WIRE_WRAITH" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));
		pui->Initialize();
		CComanderMgr::GetInstance()->add_wireframe_ui(pui);

		CFontMgr::GetInstance()->SetInfomation_font(L"Terran Wraith" ,interface_pos.x + 320 , interface_pos.y + 390 );
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
		interface_pos.x + 195 , interface_pos.y + 450 , font_color);
	CFontMgr::GetInstance()->Setbatch_Font(L"%d/%d" , m_unitinfo.mp , m_unitinfo.maxmp,
		interface_pos.x + 200 , interface_pos.y + 465 , D3DCOLOR_ARGB(255,255,255,255));


	CFontMgr::GetInstance()->Setbatch_Font(L"공중 공격력:%d + %d" , 20 , m_upg_info[UPG_T_AIR_WEAPON].upg_cnt*2,
		interface_pos.x + 310 , interface_pos.y + 422 , D3DCOLOR_ARGB(255,255,255,255));

	CFontMgr::GetInstance()->Setbatch_Font(L"지상 공격력:%d + %d" , 6 , m_upg_info[UPG_T_AIR_WEAPON].upg_cnt*1,
		interface_pos.x + 310 , interface_pos.y + 440 , D3DCOLOR_ARGB(255,255,255,255));

	CFontMgr::GetInstance()->Setbatch_Font(L"방어력:%d + %d",m_unitinfo.armor, m_upg_info[UPG_T_AIR_ARMOR].upg_cnt 
		,interface_pos.x + 310 , interface_pos.y + 458 , D3DCOLOR_ARGB(255,255,255,255));
}
void CWraith::Release(void)
{
	CObj::area_release();

	CUnitMgr::GetInstance()->clear_destroy_unitlist(this);
}

void CWraith::Dead(void)
{
	CObj* pobj = new CGeneraEff(L"SMALLBANG" , m_vPos , D3DXVECTOR2(1.0f ,1.0f) , SORT_AIR );
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);
}
