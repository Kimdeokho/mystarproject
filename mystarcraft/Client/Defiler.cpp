#include "StdAfx.h"
#include "Defiler.h"

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
#include "Com_DefilerAnim.h"
#include "Com_CC.h"
#include "Com_Collision.h"
#include "Com_UsingSkill.h"

#include "UI_Select.h"

#include "UI_Energy_bar.h"
#include "Building_Preview.h"
#include "GeneraEff.h"

#include "Input_Interface.h"

#include "MyCmd_Building.h"

#include "UI_Cmd_info.h"
#include "UI_Resource.h"
#include "UI_Wireframe.h"
#include "FontMgr.h"

#include "Corpse.h"

#include "Skill_Defensive.h"

CDefiler::CDefiler(void)
{
}

CDefiler::~CDefiler(void)
{
	Release();
}

void CDefiler::Initialize(void)
{
	CObj::unit_area_Initialize();
	CUnit::Initialize();

	m_eOBJ_NAME = OBJ_DEFILER;
	m_sortID = SORT_GROUND;
	m_ecategory = CATEGORY_UNIT;

	m_unitinfo.etribe = TRIBE_ZERG;
	m_unitinfo.eAttackType = ATTACK_ONLY_GROUND;
	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.esize = SIZE_MEDIUM;
	m_unitinfo.erace = OBJRACE_CREATURE;
	m_unitinfo.state = IDLE;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_MEDIUM;
	m_unitinfo.maxhp = 80;
	m_unitinfo.hp = 80;
	m_unitinfo.mp = 0;
	//m_unitinfo.fspeed = 93;
	m_unitinfo.fspeed = 68; //속업 133
	m_unitinfo.attack_range = 1*32;
	m_unitinfo.air_attack_range = 1*32;
	m_unitinfo.search_range = 6*32;
	m_unitinfo.fog_range = 32*8*2;
	m_unitinfo.armor = 0;

	m_vertex.left = 13.5f;
	m_vertex.right = 13.5f;
	m_vertex.top =  12.5f;
	m_vertex.bottom = 12.5f;


	m_rect.left = m_vPos.x - m_vertex.left; 
	m_rect.right = m_vPos.x + m_vertex.right;
	m_rect.top = m_vPos.y - m_vertex.top;
	m_rect.bottom = m_vPos.y + m_vertex.bottom;


	m_com_pathfind = new CCom_Pathfind(m_vPos , m_rect , 32 ,16);
	m_com_anim = new CCom_DefilerAnim(m_matWorld);
	m_com_cc = new CCom_CC();
	m_com_usingskill = new CCom_UsingSkill();

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_CC ,  m_com_cc ));	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range)));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim));	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_Collision(m_vPos , m_rect , m_vertex)) ) ;	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_PATHFINDE ,  m_com_pathfind) );	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_USINGSKILL , m_com_usingskill));



	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_select_ui = new CUI_Select(L"Select32" , m_vPos , 13);
	m_select_ui->Initialize();

	m_energybar_ui = new CUI_Energy_bar(this , 32 , 30);
	m_energybar_ui->Initialize();
}

void CDefiler::Update(void)
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
		m_com_anim->SetAnimation(L"IDLE");
	}

	m_select_ui->Update();
	m_energybar_ui->Update();
}

void CDefiler::Render(void)
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
	//CLineMgr::GetInstance()->collisionbox_render(m_rect);
}

void CDefiler::Inputkey_reaction(const int& nkey)
{
	if( false == m_unitinfo.is_active )
		return;

	if(VK_RBUTTON == nkey)
	{
		m_unitinfo.state = MOVE;
		m_unitinfo.order = ORDER_MOVE;

		CObj* ptarget = CArea_Mgr::GetInstance()->GetChoiceTarget();

		if(NULL != m_com_pathfind)
		{
			D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();
			if(NULL != ptarget)
			{
				m_bmagicbox = false;
				m_com_pathfind->SetTargetObjID(ptarget->GetObjNumber());
			}
			m_com_pathfind->SetGoalPos(goalpos , m_bmagicbox);	

			m_com_pathfind->SetFlowField();
			m_com_pathfind->StartPathfinding();
			m_bmagicbox = false;
		}
	}
}

void CDefiler::Inputkey_reaction(const int& firstkey , const int& secondkey)
{
	if( false == m_unitinfo.is_active )
		return;

	if('A' == firstkey && VK_LBUTTON == secondkey)
	{
		m_unitinfo.order = ORDER_MOVE_ATTACK;
		m_unitinfo.state = MOVE;

		if(NULL != m_com_pathfind)
		{
			D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();


			m_com_pathfind->SetGoalPos(goalpos , m_bmagicbox);
			m_com_pathfind->SetFlowField();
			m_com_pathfind->StartPathfinding();
			m_bmagicbox = false;
		}
	}

	if('W' == firstkey && VK_LBUTTON == secondkey)
	{
		m_unitinfo.order = ORDER_USINGSKILL;
		m_unitinfo.state = MOVE;

		CObj* ptarget = CArea_Mgr::GetInstance()->GetChoiceTarget();

		D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();

		if( NULL != ptarget)
			m_com_pathfind->SetTargetObjID(ptarget->GetObjNumber());

		m_com_pathfind->SetGoalPos(goalpos , false);
		m_com_pathfind->SetFlowField();
		m_com_pathfind->StartPathfinding();

		if(NULL != ptarget)
			m_com_usingskill->SetUsingSkill(SO_DRAKSWARM , ptarget , ptarget->GetPos());
		else
			m_com_usingskill->SetUsingSkill(SO_DRAKSWARM , NULL , goalpos);
	}

	if('G' == firstkey && VK_LBUTTON == secondkey)
	{
		m_unitinfo.order = ORDER_USINGSKILL;
		m_unitinfo.state = MOVE;

		CObj* ptarget = CArea_Mgr::GetInstance()->GetChoiceTarget();

		D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();

		if( NULL != ptarget)
			m_com_pathfind->SetTargetObjID(ptarget->GetObjNumber());

		m_com_pathfind->SetGoalPos(goalpos , false);
		m_com_pathfind->SetFlowField();
		m_com_pathfind->StartPathfinding();

		if(NULL != ptarget)
			m_com_usingskill->SetUsingSkill(SO_PLAGUE , ptarget , ptarget->GetPos());
		else
			m_com_usingskill->SetUsingSkill(SO_PLAGUE , NULL , goalpos);

	}
}

bool CDefiler::Input_cmd(const int& nkey , bool* waitkey)
{
	if( 'W' == nkey )
		waitkey[nkey] = true;

	else if( 'G' == nkey )
		waitkey[nkey] = true;

	return false;
}

bool CDefiler::Input_cmd(const int& firstkey , const int& secondkey)
{
	return false;
}

void CDefiler::Update_Cmdbtn(void)
{
	CUI_Cmd_info* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();

	pui->Create_Cmdbtn(0 , L"BTN_MOVE" , BTN_MOVE);
	pui->Create_Cmdbtn(1 , L"BTN_STOP" , BTN_STOP);
	pui->Create_Cmdbtn(2 , L"BTN_ATTACK" , BTN_ATTACK);
	pui->Create_Cmdbtn(3 , L"BTN_PATROL" , BTN_PATROL);
	pui->Create_Cmdbtn(4 , L"BTN_HOLD" , BTN_HOLD);

	pui->Create_Cmdbtn(6 , L"BTN_DARKSWARM" , BTN_DARKSWARM);
	pui->Create_Cmdbtn(7 , L"BTN_Z_VF0" , BTN_Z_VF0);
}

void CDefiler::Update_Wireframe(void)
{
	D3DXVECTOR2 interface_pos = CIngame_UIMgr::GetInstance()->GetMainInterface_pos();

	if(true == CIngame_UIMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.state))
	{
		CUI* pui = NULL;
		pui = new CUI_Wireframe(L"WIRE_DEFILER" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));
		pui->Initialize();
		CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);

		CFontMgr::GetInstance()->SetInfomation_font(L"Zerg Defiler" ,interface_pos.x + 320 , interface_pos.y + 390 );
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

	CFontMgr::GetInstance()->Setbatch_Font(L"방어력:%d + %d",m_unitinfo.armor, m_upg_info[UPG_Z_GROUND_ARMOR].upg_cnt 
		,interface_pos.x + 310 , interface_pos.y + 458 , D3DCOLOR_ARGB(255,255,255,255));
}

void CDefiler::SetDamage(const int& idamage , DAMAGE_TYPE edamagetype)
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

void CDefiler::Dead(void)
{
	CSoundDevice::GetInstance()->PlayBattleSound(SND_B_DFDTH , m_vPos);

	CObj* pobj = new CCorpse(L"DEFILER_DEAD" , L"DEFILER_WRECKAGE");
	pobj->SetPos(m_vPos.x , m_vPos.y);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddCorpse(pobj);
}

void CDefiler::Release(void)
{
	CObj::area_release();

	CIngame_UIMgr::GetInstance()->GetResource_UI()->SetPopvalue(-1 , m_eteamnumber);
}
