#include "StdAfx.h"
#include "Ultra.h"

#include "ScrollMgr.h"
#include "TimeMgr.h"
#include "MouseMgr.h"
#include "Area_Mgr.h"
#include "KeyMgr.h"
#include "UnitMgr.h"
#include "Ingame_UIMgr.h"
#include "MyMath.h"
#include "ObjMgr.h"
#include "LineMgr.h"
#include "Session_Mgr.h"

#include "Com_fog.h"
#include "Com_Pathfind.h"
#include "Com_Meleesearch.h"
#include "Com_UltraAnim.h"
#include "Com_Worksearch.h"
#include "Com_WUltra.h"
#include "Com_CC.h"
#include "Com_Collision.h"

#include "UI_Select.h"

#include "UI_Energy_bar.h"
#include "Building_Preview.h"
#include "GeneraEff.h"

#include "Input_Interface.h"

#include "MyCmd_Building.h"

#include "Corpse.h"

#include "Skill_Defensive.h"

#include "UI_Resource.h"
#include "UI_Cmd_info.h"
#include "FontMgr.h"
#include "UI_Wireframe.h"

CUltra::CUltra(void)
{
}

CUltra::~CUltra(void)
{
	Release();
}

void CUltra::Initialize(void)
{
	CObj::unit_area_Initialize();
	CUnit::Initialize();

	m_vcurdir = D3DXVECTOR2(1,1);

	m_eOBJ_NAME = OBJ_ULTRA;
	m_sortID = SORT_GROUND;
	m_ecategory = CATEGORY_UNIT;

	m_unitinfo.etribe = TRIBE_ZERG;
	m_unitinfo.eAttackType = ATTACK_ONLY_GROUND;
	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.esize = SIZE_LARGE;
	m_unitinfo.erace = OBJRACE_CREATURE;
	m_unitinfo.state = IDLE;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_LARGE;
	m_unitinfo.maxhp = 400;
	m_unitinfo.hp = 400;
	m_unitinfo.mp = 0;
	//m_unitinfo.fspeed = 93;
	m_unitinfo.fspeed = 133; //속업 133
	m_unitinfo.attack_range = 1*32;
	m_unitinfo.air_attack_range = 1*32;
	m_unitinfo.search_range = 8*32;
	m_unitinfo.fog_range = 32*10*2;
	m_unitinfo.armor = 3;

	m_vertex.left = 19.f;
	m_vertex.right = 19.f;
	m_vertex.top =  16.f;
	m_vertex.bottom = 16.f;

	m_rect.left = m_vPos.x - m_vertex.left; 
	m_rect.right = m_vPos.x + m_vertex.right;
	m_rect.top = m_vPos.y - m_vertex.top;
	m_rect.bottom = m_vPos.y + m_vertex.bottom;


	m_com_pathfind = new CCom_Pathfind(m_vPos , m_rect , 32 ,16);
	m_com_anim = new CCom_UltraAnim(m_matWorld);
	m_com_cc = new CCom_CC();
	m_com_targetsearch = new CCom_Meleesearch(SEARCH_ONLY_ENEMY);
	m_com_weapon = new CCom_WUltra;

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_CC ,  m_com_cc ));	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range)));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim));	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_Collision(m_vPos , m_rect , m_vertex)) ) ;	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_PATHFINDE ,  m_com_pathfind) );	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_WEAPON , m_com_weapon )) ;
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_TARGET_SEARCH , m_com_targetsearch ) );	


	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_select_ui = new CUI_Select(L"Select48" , m_vPos , 13);
	m_select_ui->Initialize();

	m_energybar_ui = new CUI_Energy_bar(this , 38 , 30);
	m_energybar_ui->Initialize();
}

void CUltra::Update(void)
{
	CObj::area_update();

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();

	if(IDLE == m_unitinfo.state)
	{
		m_com_anim->SetAnimation(L"IDLE");
	}
	else if(MOVE == m_unitinfo.state || COLLISION == m_unitinfo.state)
	{
		m_com_anim->SetAnimation(L"MOVE");
	}
	else if(ATTACK == m_unitinfo.state)
	{
		m_com_anim->SetAnimation(L"ATTACK");
	}

	m_select_ui->Update();
	m_energybar_ui->Update();
}

void CUltra::Render(void)
{
	if(false == m_unitinfo.is_active)
		return;

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;


	m_select_ui->Render();
	m_com_anim->Render();
	m_com_cc->Render();
	m_energybar_ui->Render();

	m_com_pathfind->Render();
	CLineMgr::GetInstance()->collisionbox_render(m_rect);
}

void CUltra::Inputkey_reaction(const int& nkey)
{
	if( false == m_unitinfo.is_active )
		return;

	if(VK_RBUTTON == nkey)
	{
		m_unitinfo.state = MOVE;
		m_unitinfo.order = ORDER_MOVE;

		CObj* ptarget = CArea_Mgr::GetInstance()->GetChoiceTarget();
		((CCom_Targetsearch*)m_com_targetsearch)->SetTarget(ptarget);

		if(NULL != m_com_pathfind)
		{
			D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();
			if(NULL != ptarget)
			{
				m_bmagicbox = false;
			}			

			((CCom_Pathfind*)m_com_pathfind)->SetGoalPos(goalpos , m_bmagicbox);
			((CCom_Pathfind*)m_com_pathfind)->SetFlowField();
			((CCom_Pathfind*)m_com_pathfind)->StartPathfinding();
			m_bmagicbox = false;
		}
	}
}

void CUltra::Inputkey_reaction(const int& firstkey , const int& secondkey)
{
	if( false == m_unitinfo.is_active )
		return;

	if('A' == firstkey && VK_LBUTTON == secondkey)
	{
		m_unitinfo.order = ORDER_MOVE_ATTACK;
		m_unitinfo.state = MOVE;
		((CCom_Targetsearch*)m_com_targetsearch)->SetTarget(CArea_Mgr::GetInstance()->GetChoiceTarget());

		if(NULL != m_com_pathfind)
		{
			D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();


			((CCom_Pathfind*)m_com_pathfind)->SetGoalPos(goalpos , m_bmagicbox);
			((CCom_Pathfind*)m_com_pathfind)->SetFlowField();
			((CCom_Pathfind*)m_com_pathfind)->StartPathfinding();
			m_bmagicbox = false;
		}
	}
}

bool CUltra::Input_cmd(const int& nkey , bool* waitkey)
{
	if('A' == nkey)
		waitkey[nkey] = true;

	return false;
}

bool CUltra::Input_cmd(const int& firstkey , const int& secondkey)
{
	return false;
}

void CUltra::Update_Cmdbtn(void)
{
	const CUI* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();

	((CUI_Cmd_info*)pui)->Create_Cmdbtn(0 , L"BTN_MOVE" , BTN_MOVE);
	((CUI_Cmd_info*)pui)->Create_Cmdbtn(1 , L"BTN_STOP" , BTN_STOP);
	((CUI_Cmd_info*)pui)->Create_Cmdbtn(2 , L"BTN_ATTACK" , BTN_ATTACK);
	((CUI_Cmd_info*)pui)->Create_Cmdbtn(3 , L"BTN_PATROL" , BTN_PATROL);
	((CUI_Cmd_info*)pui)->Create_Cmdbtn(4 , L"BTN_HOLD" , BTN_HOLD);
}

void CUltra::Update_Wireframe(void)
{
	D3DXVECTOR2 interface_pos = CIngame_UIMgr::GetInstance()->GetMainInterface_pos();

	if(true == CIngame_UIMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.state))
	{
		CUI* pui = NULL;
		pui = new CUI_Wireframe(L"WIRE_ULTRA" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));
		pui->Initialize();
		CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);

		CFontMgr::GetInstance()->SetInfomation_font(L"Zerg Ultra" ,interface_pos.x + 320 , interface_pos.y + 390 );
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

	WEAPON_INFO temp_info = (m_com_weapon)->GetWeapon_info();
	CFontMgr::GetInstance()->Setbatch_Font(L"공격력:%d + %d" ,temp_info.damage , m_upg_info[UPG_Z_MELEE_ATT].upg_cnt,
		interface_pos.x + 310 , interface_pos.y + 440 , D3DCOLOR_ARGB(255,255,255,255));
	CFontMgr::GetInstance()->Setbatch_Font(L"방어력:%d + %d",m_unitinfo.armor, m_upg_info[UPG_Z_GROUND_ARMOR].upg_cnt 
		,interface_pos.x + 310 , interface_pos.y + 458 , D3DCOLOR_ARGB(255,255,255,255));
}

void CUltra::SetDamage(const int& idamage , DAMAGE_TYPE edamagetype)
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
	{
		if(DAMAGE_MAGIC == edamagetype)
			tempdamage = (float)idamage;
		else
			tempdamage = (float)idamage - (m_unitinfo.armor + m_upg_info[UPG_Z_GROUND_ARMOR].upg_cnt);
	}

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

	if((int)tempdamage <= 0)
	{
		if( 0 == rand()%2)
			tempdamage = 1;
		else
			tempdamage = 0;
	}

	m_unitinfo.hp -= (int)(tempdamage + 0.5f);

	if(m_unitinfo.hp <= 0)
	{
		//킬수 + 1 하면 되는디..
		m_unitinfo.hp = 0;
		m_bdestroy = true;
		Dead();
	}
	else if(m_unitinfo.hp >= m_unitinfo.maxhp)
		m_unitinfo.hp = m_unitinfo.maxhp;
}

void CUltra::Dead(void)
{
	CObj* pobj = new CCorpse(L"ULTRADEAD" , L"ULTRAWRECKAGE");
	pobj->SetPos(m_vPos.x , m_vPos.y);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddCorpse(pobj);
}

void CUltra::Release(void)
{
	CObj::area_release();

	CIngame_UIMgr::GetInstance()->GetResource_UI()->SetPopvalue(-4 , m_eteamnumber);
}
