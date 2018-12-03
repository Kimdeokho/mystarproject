#include "StdAfx.h"
#include "Larva_egg.h"

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
#include "Com_LarvaEggAnim.h"
#include "Com_CC.h"
#include "Com_Collision.h"
#include "Com_Larvahatch.h"
#include "Com_AirPathfind.h"
#include "Com_Pathfind.h"

#include "UI_Select.h"
#include "UI_Energy_bar.h"
#include "Building_Preview.h"
#include "GeneraEff.h"

#include "Input_Interface.h"

#include "MyCmd_Building.h"
#include "UI_Cmd_info.h"
#include "Corpse.h"

#include "Zerg_building.h"
#include "Drone.h"
#include "Zergling.h"
#include "Hydra.h"
#include "Ultra.h"
#include "Overload.h"
#include "Mutal.h"
#include "Scourge.h"
#include "Queen.h"
#include "Brude.h"
#include "Defiler.h"

CLarva_egg::CLarva_egg(const float maxtime ,const OBJID id, const int hatch_num, const int createcnt , const TCHAR* birthkey)
{
	m_production_info.curtime = 0;
	m_production_info.maxtime = maxtime;
	m_production_info.eid = id;
	m_hatch_num = hatch_num;
	m_create_cnt = createcnt;

	m_birthkey = birthkey;
	//if(OBJ_DRONE == m_production_info.eid)
}

CLarva_egg::~CLarva_egg(void)
{
	Release();
}

void CLarva_egg::Initialize(void)
{
	CObj::unit_area_Initialize();

	m_eOBJ_NAME = OBJ_LARVA_EGG;
	m_sortID = SORT_GROUND;
	m_ecategory = CATEGORY_UNIT;

	m_unitinfo.eAttackType = ATTACK_ONLY_GROUND;
	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.esize = SIZE_SMALL;
	m_unitinfo.erace = OBJRACE_CREATURE;
	m_unitinfo.state = MUTATE;
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

	m_vertex.left = 16.f;
	m_vertex.right = 16.f;
	m_vertex.top =  16.f;
	m_vertex.bottom = 16.f;


	m_rect.left = m_vPos.x - m_vertex.left; 
	m_rect.right = m_vPos.x + m_vertex.right;
	m_rect.top = m_vPos.y - m_vertex.top;
	m_rect.bottom = m_vPos.y + m_vertex.bottom;


	m_com_anim = new CCom_LarvaEggAnim(m_matWorld , m_birthkey);
	m_com_cc = new CCom_CC();

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_CC ,  m_com_cc ));	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range)));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim));	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_Collision(m_vPos , m_rect , m_vertex, false)) ) ;	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_select_ui = new CUI_Select(L"Select22" , m_vPos , 13);
	m_select_ui->Initialize();

	m_energybar_ui = new CUI_Energy_bar(this , 22 , 32);
	m_energybar_ui->Initialize();

	m_build_hp = 0;
}

void CLarva_egg::Update(void)
{
	CObj::area_update();

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();

	if(IDLE == m_unitinfo.state)
	{
		//m_com_anim->SetAnimation(L"IDLE");

		m_production_info.curtime += GETTIME;	

		if(m_production_info.curtime >= m_production_info.maxtime)
		{
			m_com_anim->SetAnimation(L"READY");
			m_unitinfo.state = STATE_NONE;
			//생산 아이디
		}
	}
	else if(MUTATE == m_unitinfo.state)
	{
		m_com_anim->SetAnimation(L"MUTATE");
	}

	m_select_ui->Update();
	m_energybar_ui->Update();
}

void CLarva_egg::Render(void)
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
	CLineMgr::GetInstance()->collisionbox_render(m_rect);
}

void CLarva_egg::Inputkey_reaction(const int& nkey)
{

}

void CLarva_egg::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}

bool CLarva_egg::Input_cmd(const int& nkey , bool* waitkey)
{
	return false;
}

bool CLarva_egg::Input_cmd(const int& firstkey , const int& secondkey)
{
	return false;
}

void CLarva_egg::Update_Cmdbtn(void)
{

}

void CLarva_egg::Update_Wireframe(void)
{

}

void CLarva_egg::Dead(void)
{

}

void CLarva_egg::Release(void)
{
	CObj* pobj = NULL;

	CObj* phatch = CObjMgr::GetInstance()->GetObj(m_hatch_num);

	for(int i = 0; i < m_create_cnt; ++i)
	{
		if(OBJ_ZERGLING == m_production_info.eid)
			pobj = new CZergling;
		else if(OBJ_DRONE == m_production_info.eid)
			pobj = new CDrone;
		else if(OBJ_HYDRA == m_production_info.eid)
			pobj = new CHydra;
		else if(OBJ_ULTRA == m_production_info.eid)
			pobj = new CUltra;
		else if(OBJ_OVERLOAD == m_production_info.eid)
			pobj = new COverload;
		else if(OBJ_MUTAL == m_production_info.eid)
			pobj = new CMutal;
		else if(OBJ_SCOURGE == m_production_info.eid)
			pobj = new CScourge;
		else if(OBJ_QUEEN == m_production_info.eid)
			pobj = new CQueen;
		else if(OBJ_BRUDE == m_production_info.eid)
			pobj = new CBrude;
		else if(OBJ_DEFILER == m_production_info.eid)
			pobj = new CDefiler;

		CObjMgr::GetInstance()->AddObject(pobj , m_production_info.eid);

		pobj->Setdir(D3DXVECTOR2(1,1));
		pobj->SetTeamNumber(m_eteamnumber);
		pobj->SetPos(m_vPos);
		pobj->Initialize();

		if(MOVE_GROUND == pobj->GetUnitinfo().eMoveType)
			pobj->SetPos(m_vPos);
		else
			pobj->SetPos( D3DXVECTOR2(m_vPos.x , m_vPos.y - 40.f));		


		if(NULL != phatch)
		{
			if(OBJ_HATCERY == phatch->GetOBJNAME())
			{
				bool				isrally = ((CZerg_building*)phatch)->Get_Is_rally();
				D3DXVECTOR2			rallypoint = ((CZerg_building*)phatch)->GetRallyPoint();
				vector<D3DXVECTOR2> rallypath = ((CZerg_building*)phatch)->GetRally_Path();

				CComponent* pcom = NULL;
				if(true == isrally)
				{
					pobj->SetOrder(ORDER_MOVE);
					pcom = pobj->GetComponent(COM_PATHFINDE);

					if(MOVE_GROUND == pobj->GetUnitinfo().eMoveType)
					{
						((CCom_Pathfind*)pcom)->SetGoalPos(rallypoint , false);
						((CCom_Pathfind*)pcom)->Setrally_path(rallypath);
					}
					else
					{					
						((CCom_AirPathfind*)pcom)->SetGoalPos(rallypoint , false);
					}
				}
			}
		}

	}


	CObj::area_release();
}
