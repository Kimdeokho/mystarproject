#include "StdAfx.h"
#include "Goliath.h"

#include "Tankbody.h"
#include "Tankbarrel.h"

#include "Siegebody.h"
#include "Siegebarrel.h"

#include "Com_fog.h"
#include "Com_Collision.h"
#include "Com_Pathfind.h"
#include "Com_GollegAnim.h"
#include "Com_CC.h"

#include "TileManager.h"

#include "ScrollMgr.h"
#include "LineMgr.h"
#include "FontMgr.h"
#include "MyMath.h"
#include "UnitMgr.h"
#include "GeneraEff.h"
#include "Area_Mgr.h"
#include "ObjMgr.h"
#include "UI_Select.h"

#include "Goliath_part.h"

#include "Ingame_UIMgr.h"
#include "UI_Cmd_info.h"
#include "UI_Wireframe.h"
#include "UI_Energy_bar.h"
#include "UI_Resource.h"
#include "Skill_Defensive.h"
CGoliath::CGoliath(void)
{
}

CGoliath::~CGoliath(void)
{
	Release();
}

void CGoliath::Initialize(void)
{
	m_vcurdir = CMyMath::dgree_to_dir(2*22.5f);

	CObj::unit_area_Initialize();
	CUnit::Initialize();

	m_sortID = SORT_GROUND;	
	m_ecategory = CATEGORY_UNIT;
	m_eOBJ_NAME = OBJ_GOLIATH;

	m_unitinfo.etribe = TRIBE_TERRAN;
	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.state = IDLE;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.esize = SIZE_SMALL;
	m_unitinfo.erace = OBJRACE_MECHANIC;
	m_unitinfo.eArmorType = ARMOR_LARGE;
	m_unitinfo.hp = 125;
	m_unitinfo.maxhp = m_unitinfo.hp;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 77;
	m_unitinfo.fog_range = 512;

	m_vertex.left = 16;
	m_vertex.right = 16;
	m_vertex.top =  16;
	m_vertex.bottom = 16;

	m_pgoliath_arm = new CGoliath_part(this);
	m_pgoliath_arm->SetPos(D3DXVECTOR2(-100.f , -100.f));
	m_pgoliath_arm->SetTeamNumber(m_eteamnumber);
	m_pgoliath_arm->SetObjID(m_obj_id);

	m_com_pathfind = new CCom_Pathfind(m_vPos , m_rect , 32 , 16);
	m_com_collision = new CCom_Collision(m_vPos , m_rect , m_vertex , true);
	m_com_anim = new CCom_GollegAnim(m_matWorld);
	m_com_cc = new CCom_CC();

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_CC ,  m_com_cc )) ;	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , m_com_collision ) ) ;	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_PATHFINDE ,  m_com_pathfind) );
	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_pgoliath_arm->Initialize();

	m_select_ui = new CUI_Select(L"Select48" , m_vPos , 8);
	m_select_ui->Initialize();

	m_energybar_ui = new CUI_Energy_bar(this , 48);
	m_energybar_ui->Initialize();

}

void CGoliath::Update(void)
{
	CObj::area_update();

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();


	if(IDLE == m_unitinfo.state)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");

		if(ATTACK != m_pgoliath_arm->GetUnitinfo().state)
			m_pgoliath_arm->SetState(IDLE);
	}
	else if(MOVE == m_unitinfo.state || COLLISION == m_unitinfo.state)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"MOVE");
		m_pgoliath_arm->Setdir(m_vcurdir);
		m_pgoliath_arm->SetState(MOVE);
	}

	if(ORDER_NONE == m_unitinfo.order)
	{
		m_pgoliath_arm->SetOrder(ORDER_NONE);
	}

	sync_arm();
	m_pgoliath_arm->SetActive(m_unitinfo.is_active);
	m_pgoliath_arm->Update();

	m_select_ui->Update();
	m_energybar_ui->Update();
}

void CGoliath::Render(void)
{
	if( false == m_unitinfo.is_active )
		return;

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;


	m_select_ui->Render();
	m_com_anim->Render();
	m_pgoliath_arm->Render();
	m_com_cc->Render();
	m_energybar_ui->Render();

	if(NULL != m_com_pathfind)
		m_com_pathfind->Render();

	CLineMgr::GetInstance()->collisionbox_render(m_rect);
}


void CGoliath::Inputkey_reaction(const int& nkey)
{
	if( 'W' == nkey)
	{
		m_eteamnumber = TEAM_1;
		m_pgoliath_arm->SetTeamNumber(TEAM_1);
	}
	if(VK_RBUTTON == nkey)
	{
			if(TRANSFORMING != m_unitinfo.state)
			{
				m_unitinfo.state = MOVE;
				m_unitinfo.order = ORDER_MOVE;
				m_pgoliath_arm->SetOrder(ORDER_MOVE);
				m_pgoliath_arm->SetState(MOVE);

				if(NULL != m_com_pathfind)
				{
					D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();

					CObj* ptarget = CArea_Mgr::GetInstance()->GetChoiceTarget();
					if(NULL != ptarget)
						m_bmagicbox = false;

					m_com_pathfind->SetGoalPos(goalpos , m_bmagicbox);
					m_com_pathfind->SetFlowField();
					m_com_pathfind->StartPathfinding();
					m_bmagicbox = false;
				}
			}
	}

	m_pgoliath_arm->Inputkey_reaction(nkey);
}

void CGoliath::Inputkey_reaction(const int& firstkey , const int& secondkey)
{
	if('A' == firstkey && VK_LBUTTON == secondkey)
	{

		m_unitinfo.order = ORDER_MOVE_ATTACK;
		m_unitinfo.state = MOVE;
		m_pgoliath_arm->SetOrder(ORDER_MOVE_ATTACK);
		m_pgoliath_arm->SetState(MOVE);				

		if(NULL != m_com_pathfind)
		{
			D3DXVECTOR2 goalpos = CUnitMgr::GetInstance()->GetUnitGoalPos();

			m_com_pathfind->SetGoalPos(goalpos , m_bmagicbox);
			m_com_pathfind->SetFlowField();
			m_com_pathfind->StartPathfinding();
			m_bmagicbox = false;
		}
	}

	m_pgoliath_arm->Inputkey_reaction(firstkey , secondkey);
}
void CGoliath::sync_arm(void)
{
	((CGoliath_part*)m_pgoliath_arm)->setarm_pos(m_vPos);
}
void CGoliath::SetDamage(const int& idamage , DAMAGE_TYPE edamagetype)
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
		tempdamage = (float)idamage - (m_unitinfo.armor + m_upg_info[UPG_T_MECHANIC_ARMOR].upg_cnt);

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
void CGoliath::Update_Cmdbtn(void)
{
	const CUI* pui = CIngame_UIMgr::GetInstance()->GetCmd_info();

	((CUI_Cmd_info*)pui)->Create_Cmdbtn(0 , L"BTN_MOVE" , BTN_MOVE);
	((CUI_Cmd_info*)pui)->Create_Cmdbtn(1 , L"BTN_STOP" , BTN_STOP);
	((CUI_Cmd_info*)pui)->Create_Cmdbtn(2 , L"BTN_ATTACK" , BTN_ATTACK);
	((CUI_Cmd_info*)pui)->Create_Cmdbtn(3 , L"BTN_PATROL" , BTN_PATROL);
	((CUI_Cmd_info*)pui)->Create_Cmdbtn(4 , L"BTN_HOLD" , BTN_HOLD);

}
void CGoliath::Update_Wireframe(void)
{
	D3DXVECTOR2 interface_pos = CIngame_UIMgr::GetInstance()->GetMainInterface_pos();

	if(true == CIngame_UIMgr::GetInstance()->renewal_wireframe_ui(this , m_unitinfo.state))
	{
		CUI* pui = NULL;
		pui = new CUI_Wireframe(L"WIRE_GOLIATH" , D3DXVECTOR2(interface_pos.x + 165, interface_pos.y + 390 ));
		pui->Initialize();
		CIngame_UIMgr::GetInstance()->add_wireframe_ui(pui);

		CFontMgr::GetInstance()->SetInfomation_font(L"Terran Goliath" ,interface_pos.x + 320 , interface_pos.y + 390 );
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


	CFontMgr::GetInstance()->Setbatch_Font(L"공중 공격력:%d + %d" , 20 , m_upg_info[UPG_T_MECHANIC_WEAPON].upg_cnt*4,
		interface_pos.x + 310 , interface_pos.y + 422 , D3DCOLOR_ARGB(255,255,255,255));

	CFontMgr::GetInstance()->Setbatch_Font(L"지상 공격력:%d + %d" , 12 , m_upg_info[UPG_T_MECHANIC_WEAPON].upg_cnt*1,
		interface_pos.x + 310 , interface_pos.y + 440 , D3DCOLOR_ARGB(255,255,255,255));

	CFontMgr::GetInstance()->Setbatch_Font(L"방어력:%d + %d",m_unitinfo.armor, m_upg_info[UPG_T_MECHANIC_ARMOR].upg_cnt 
		,interface_pos.x + 310 , interface_pos.y + 458 , D3DCOLOR_ARGB(255,255,255,255));

}
void CGoliath::Dead(void)
{
	CObj* pobj = new CGeneraEff(L"SMALLBANG" , m_vPos , D3DXVECTOR2(0.85f,0.85f) , SORT_GROUND );
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);
}
void CGoliath::Release(void)
{
	CObj::area_release();

	CUnitMgr::GetInstance()->clear_destroy_unitlist(this);

	Safe_Delete(m_pgoliath_arm);

	CIngame_UIMgr::GetInstance()->GetResource_UI()->SetPopvalue(-2 , m_eteamnumber);
}