#include "StdAfx.h"
#include "Larva.h"

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
#include "ObjMgr.h"

#include "Com_fog.h"
#include "Com_LarvaAnim.h"
#include "Com_CC.h"
#include "Com_Collision.h"
#include "Com_Larvahatch.h"

#include "UI_Select.h"
#include "UI_Energy_bar.h"
#include "Building_Preview.h"
#include "GeneraEff.h"

#include "Input_Interface.h"

#include "MyCmd_Building.h"
#include "UI_Cmd_info.h"
#include "Corpse.h"
#include "Larva_egg.h"


CLarva::CLarva(int hatch_number)
{
	m_hatch_num = hatch_number;
}

CLarva::~CLarva(void)
{
	Release();
}

void CLarva::Initialize(void)
{
	CObj::unit_area_Initialize();

	m_eOBJ_NAME = OBJ_LARVA;
	m_sortID = SORT_CORPSE;
	m_ecategory = CATEGORY_UNIT;

	m_unitinfo.eAttackType = ATTACK_ONLY_GROUND;
	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.esize = SIZE_SMALL;
	m_unitinfo.erace = OBJRACE_CREATURE;
	m_unitinfo.state = IDLE;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_SMALL;
	m_unitinfo.maxhp = 40;
	m_unitinfo.hp = 40;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 22;
	m_unitinfo.attack_range = 0*32;
	m_unitinfo.air_attack_range = 0*32;
	m_unitinfo.search_range = 0*32;
	m_unitinfo.fog_range = 32*4*2;
	m_unitinfo.armor = 10;

	m_vertex.left = 0.f;
	m_vertex.right = 0.f;
	m_vertex.top =  0.f;
	m_vertex.bottom = 0.f;


	m_rect.left = m_vPos.x - m_vertex.left; 
	m_rect.right = m_vPos.x + m_vertex.right;
	m_rect.top = m_vPos.y - m_vertex.top;
	m_rect.bottom = m_vPos.y + m_vertex.bottom;


	m_com_anim = new CCom_LarvaAnim(m_matWorld);
	m_com_cc = new CCom_CC();

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_CC ,  m_com_cc ));	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range)));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim));	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_Collision(m_vPos , m_rect , m_vertex , false)) ) ;	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_select_ui = new CUI_Select(L"Select22" , m_vPos , 13);
	m_select_ui->Initialize();

	m_energybar_ui = new CUI_Energy_bar(this , 22 , 30);
	m_energybar_ui->Initialize();

	m_waypoint[0].x = m_vPos.x - 16;
	m_waypoint[0].y = m_vPos.y + 4;

	m_waypoint[1].x = m_vPos.x + 16;
	m_waypoint[1].y = m_vPos.y + 4;

	m_waypoint[2].x = m_vPos.x + 16;
	m_waypoint[2].y = m_vPos.y + 16;

	m_waypoint[3].x = m_vPos.x - 16;
	m_waypoint[3].y = m_vPos.y + 16;

	m_moveswitch = false;
	m_idletime = 0.f;
	m_standard = rand()%3 + 1;
	m_wayidx = rand()%4;
}

void CLarva::Update(void)
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
	else if(MOVE == m_unitinfo.state)
	{
		m_com_anim->SetAnimation(L"MOVE");
	}


	/*
	움직이고, 몇초간 멈추고
	*/

	if(!m_moveswitch)
	{
		m_unitinfo.state = IDLE;
		m_idletime += GETTIME;
		if(m_standard < (DWORD)m_idletime)
		{
			m_wayidx += 1;
			m_wayidx %= 4;

			D3DXVec2Normalize(&m_vcurdir , &(m_waypoint[m_wayidx] - m_vPos));
			m_moveswitch = true;
			m_idletime = 0.f;
		}
	}
	else
	{
		m_unitinfo.state = MOVE;

		float onestep = GETTIME * m_unitinfo.fspeed;		
		m_vPos += m_vcurdir * onestep;

		if(CMyMath::pos_distance(m_vPos , m_waypoint[m_wayidx]) < onestep*onestep*2)
		{
			m_standard = rand()%2 + 1;
			m_moveswitch = false;
		}
	}

	m_select_ui->Update();
	m_energybar_ui->Update();
}

void CLarva::Render(void)
{
	if(false == m_unitinfo.is_active)
		return;

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;


	m_select_ui->Render();
	m_com_anim->Render();
	m_com_cc->Render();
	m_energybar_ui->Render();

	//m_com_pathfind->Render();
	//CLineMgr::GetInstance()->collisionbox_render(m_rect);
}

void CLarva::Inputkey_reaction(const int& nkey)
{
	CObj* pobj = NULL;
	if('D' == nkey)
	{
		SetDestroy(true);

		pobj = new CLarva_egg(1.f , OBJ_DRONE , m_hatch_num , 1 , L"DRONE_BIRTH");
		CObjMgr::GetInstance()->AddObject(pobj , OBJ_LARVA_EGG);	
	}

	if('Z' == nkey)
	{
		SetDestroy(true);

		pobj = new CLarva_egg(1.f , OBJ_ZERGLING , m_hatch_num , 2 , L"ZERGLING_BIRTH");
		CObjMgr::GetInstance()->AddObject(pobj , OBJ_LARVA_EGG);
	}

	if( NULL != pobj)
	{
		pobj->SetPos(m_vPos);
		pobj->SetTeamNumber(m_eteamnumber);
		pobj->Initialize();		
	}
}

void CLarva::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}

void CLarva::Input_cmd(const int& nkey , bool* waitkey)
{

}

void CLarva::Input_cmd(const int& firstkey , const int& secondkey)
{

}

void CLarva::Update_Cmdbtn(void)
{

}

void CLarva::Dead(void)
{

}

void CLarva::Release(void)
{
	CObj::area_release();

	CObj* pobj = CObjMgr::GetInstance()->obj_alivecheck(m_hatch_num);
	if(NULL != pobj)
	{
		CCom_Larvahatch* pcom = ((CCom_Larvahatch*)(pobj->GetComponent(COM_LARVAHATCH)));

		if(NULL != pcom)
		{
			pcom->Decrease_Larva(this);
		}
	}
}
