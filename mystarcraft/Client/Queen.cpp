#include "StdAfx.h"
#include "Queen.h"

#include "ScrollMgr.h"
#include "TimeMgr.h"
#include "MouseMgr.h"

#include "Com_fog.h"
#include "Com_QueenAnim.h"
#include "Com_AirPathfind.h"
#include "Com_AirCollision.h"
#include "Com_Transport.h"
#include "Com_UsingSkill.h"
#include "Com_CC.h"

#include "ObjMgr.h"
#include "LineMgr.h"
#include "FontMgr.h"
#include "Ingame_UIMgr.h"
#include "Area_Mgr.h"
#include "UnitMgr.h"
#include "Session_Mgr.h"

#include "UI_Select.h"

#include "GeneraEff.h"
#include "MyMath.h"

#include "UI_Resource.h"
#include "UI_Wireframe.h"
#include "UI_Cmd_info.h"
#include "UI_Energy_bar.h"
#include "Skill_Defensive.h"

CQueen::CQueen(void)
{
}

CQueen::~CQueen(void)
{
	Release();
}

void CQueen::Initialize(void)
{
	CObj::unit_area_Initialize();
	CUnit::Initialize(); //맵 디스플레이

	m_sortID = SORT_AIR;	
	m_ecategory = CATEGORY_UNIT;
	m_eOBJ_NAME = OBJ_QUEEN;

	m_unitinfo.etribe = TRIBE_ZERG;
	m_unitinfo.eMoveType = MOVE_AIR;
	m_unitinfo.state = IDLE;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.esize = SIZE_MEDIUM;
	m_unitinfo.erace = OBJRACE_CREATURE;
	m_unitinfo.eArmorType = ARMOR_LARGE;
	m_unitinfo.maxhp = 120;
	m_unitinfo.hp = 120;	
	m_unitinfo.mp = 0;
	m_unitinfo.maxmp = 0;
	m_unitinfo.fspeed = 113;
	m_unitinfo.attack_range = 4*32;
	m_unitinfo.air_attack_range = 4*32;
	m_unitinfo.search_range = 255;
	m_unitinfo.fog_range = 32*18;

	m_vertex.left = 24.f;
	m_vertex.right = 24.f;
	m_vertex.top =  24.f;
	m_vertex.bottom = 24.f;

	m_com_anim = new CCom_QueenAnim(m_matWorld);
	m_com_air_pathfind = new CCom_AirPathfind(m_vPos);
	m_com_usingskill = new CCom_UsingSkill();
	m_com_cc = new CCom_CC();

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_CC ,  m_com_cc )) ;	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_AirCollision(m_vPos , m_rect , m_vertex)));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_AIR_PATHFIND , m_com_air_pathfind));	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_USINGSKILL , m_com_usingskill));


	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_select_ui = new CUI_Select(L"Select48" , m_vPos , 13);
	m_select_ui->Initialize();

	m_energybar_ui = new CUI_Energy_bar(this , 50 , m_vertex.bottom*2);
	m_energybar_ui->Initialize();

	m_upg_info = CIngame_UIMgr::GetInstance()->GetUpginfo();
}

void CQueen::Update(void)
{
	CObj::area_update();	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();

	if(IDLE == m_unitinfo.state)
		m_com_anim->SetAnimation(L"IDLE");
	else if(MOVE == m_unitinfo.state)
		m_com_anim->SetAnimation(L"IDLE");

	m_select_ui->Update();
	m_energybar_ui->Update();
}

void CQueen::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;


	m_select_ui->Render();
	m_com_anim->Render();	
	m_com_cc->Render();
	m_energybar_ui->Render();

	CLineMgr::GetInstance()->collisionbox_render(m_rect);
}

void CQueen::Inputkey_reaction(const int& nkey)
{
	if( false == m_unitinfo.is_active )
		return;

	if(VK_RBUTTON == nkey)
	{
		m_unitinfo.state = MOVE;
		m_unitinfo.order = ORDER_MOVE;

		D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();
		m_com_air_pathfind->SetGoalPos(goalpos , m_bmagicbox);
		m_bmagicbox = true;
	}
}

void CQueen::Inputkey_reaction(const int& firstkey , const int& secondkey)
{
	if('A' == firstkey && VK_LBUTTON == secondkey)
	{
		m_unitinfo.order = ORDER_MOVE_ATTACK;
		m_unitinfo.state = MOVE;

		if(NULL != m_com_air_pathfind)
		{
			D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();
			m_com_air_pathfind->SetGoalPos(goalpos , m_bmagicbox);

			m_bmagicbox = false;
		}
	}

	if('B' == firstkey && VK_LBUTTON == secondkey)
	{
		m_unitinfo.order = ORDER_USINGSKILL;
		m_unitinfo.state = MOVE;

		CObj* ptarget = CArea_Mgr::GetInstance()->GetChoiceTarget();

		if( ptarget != NULL && CATEGORY_UNIT == ptarget->GetCategory())
		{
			(m_com_air_pathfind)->SetTargetObjID(ptarget->GetObjNumber());
			(m_com_air_pathfind)->SetGoalPos(ptarget->GetPos() , false);
			(m_com_usingskill)->SetUsingSkill(SO_BROODLING , ptarget , ptarget->GetPos());
		}
	}

	if('E' == firstkey && VK_LBUTTON == secondkey)
	{
		m_unitinfo.order = ORDER_USINGSKILL;
		m_unitinfo.state = MOVE;

		CObj* ptarget = CArea_Mgr::GetInstance()->GetChoiceTarget();

		if(ptarget != NULL)
		{
			m_com_air_pathfind->SetTargetObjID(ptarget->GetObjNumber());
			m_com_air_pathfind->SetGoalPos(ptarget->GetPos() , false);
			(m_com_usingskill)->SetUsingSkill(SO_ENSNARE , ptarget , ptarget->GetPos());
		}
		else
		{
			D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();
			m_com_air_pathfind->SetGoalPos(goalpos , false);
			(m_com_usingskill)->SetUsingSkill(SO_ENSNARE , NULL , goalpos);
		}
	}
}

bool CQueen::Input_cmd(const int& nkey , bool* waitkey)
{
	if('B' == nkey)
		waitkey[nkey] = true;
	else if('R' == nkey)
		waitkey[nkey] = true;
	else if('E' == nkey)
		waitkey[nkey] = true;

	return false;
}

bool CQueen::Input_cmd(const int& firstkey , const int& secondkey)
{
	return false;
}

void CQueen::Update_Cmdbtn(void)
{
	CUI_Cmd_info* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();

	pui->Create_Cmdbtn(0 , L"BTN_MOVE" , BTN_MOVE);
	pui->Create_Cmdbtn(1 , L"BTN_STOP" , BTN_STOP);
	pui->Create_Cmdbtn(2 , L"BTN_ATTACK" , BTN_ATTACK);
	pui->Create_Cmdbtn(3 , L"BTN_PATROL" , BTN_PATROL);
	pui->Create_Cmdbtn(4 , L"BTN_HOLD" , BTN_HOLD);

	pui->Create_Cmdbtn(6 , L"BTN_Z_VQ0" , BTN_Z_VQ0);
	pui->Create_Cmdbtn(7 , L"BTN_Z_VQ1" , BTN_Z_VQ1);
}

void CQueen::Update_Wireframe(void)
{
	D3DXVECTOR2 interface_pos = CIngame_UIMgr::GetInstance()->GetMainInterface_pos();

	if(true == CIngame_UIMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.state))
	{
		CUI* pui = NULL;
		pui = new CUI_Wireframe(L"WIRE_QUEEN" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));
		pui->Initialize();
		CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);

		CFontMgr::GetInstance()->SetInfomation_font(L"Zerg Queen" ,interface_pos.x + 320 , interface_pos.y + 390 );
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

	CFontMgr::GetInstance()->Setbatch_Font(L"방어력:%d + %d",m_unitinfo.armor, m_upg_info[UPG_T_AIR_ARMOR].upg_cnt 
		,interface_pos.x + 310 , interface_pos.y + 458 , D3DCOLOR_ARGB(255,255,255,255));
}

void CQueen::SetDamage(const int& idamage , DAMAGE_TYPE edamagetype)
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
			tempdamage = (float)idamage - (m_unitinfo.armor + m_upg_info[UPG_T_AIR_ARMOR].upg_cnt);
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
	if(m_unitinfo.hp >= m_unitinfo.maxhp)
		m_unitinfo.hp = m_unitinfo.maxhp;
}

void CQueen::Dead(void)
{
	CSoundDevice::GetInstance()->PlayBattleSound(SND_B_QUDTH , m_vPos);

	CObj* pobj = new CGeneraEff(L"ZFLY_S_DEAD" , m_vPos , D3DXVECTOR2(1.0f ,1.0f) , SORT_AIR );
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);
}

void CQueen::Release(void)
{
	CObj::area_release();

	CIngame_UIMgr::GetInstance()->GetResource_UI()->SetPopvalue(-1 , m_eteamnumber);
}
