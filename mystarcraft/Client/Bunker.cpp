#include "StdAfx.h"
#include "Bunker.h"

#include "Component.h"
#include "Com_TBuildingAnim.h"
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
#include "ComanderMgr.h"
#include "TimeMgr.h"
CBunker::CBunker(void)
{
}

CBunker::~CBunker(void)
{
	Release();
}

void CBunker::Initialize(void)
{
	m_com_pathfind = NULL;

	m_vertex.left = 32.f;
	m_vertex.right = 33.f;
	m_vertex.top =  24.f;
	m_vertex.bottom = 17.f;

	CTerran_building::building_area_Initialize(2 , 3);
	CTerran_building::building_pos_Initialize(2 , 3);

	m_ebuild_tech = T_BUNKER;

	m_sortID = SORT_GROUND;	
	m_ecategory = BUILDING;
	m_eOBJ_NAME = OBJ_ARMOURY;
	m_eteamnumber = TEAM_0;

	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.estate = BUILD;
	m_unitinfo.eorder = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_LARGE;

	m_unitinfo.hp = 0;
	m_unitinfo.maxhp = 350;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 28;
	m_unitinfo.search_range = 0;
	m_unitinfo.fog_range = 512;
	m_unitinfo.fbuildtime = 10.f;

	m_com_anim = new CCom_TBuildingAnim(L"T_BUNKER" , m_matWorld);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_Collision(m_vPos , m_rect , m_vertex , false) ) ) ;	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_select_ui = new CUI_Select(L"Select94" , m_vPos , 7);
	m_select_ui->Initialize();
	CObjMgr::GetInstance()->AddSelect_UI(m_select_ui , MOVE_GROUND);

	m_fbuild_tick = float(m_unitinfo.maxhp)/m_unitinfo.fbuildtime;
	CTerran_building::fire_eff_initialize();
}

void CBunker::Update(void)
{
	CTerran_building::building_area_update();	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();

	m_select_ui->Update();


	if(IDLE == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
	}
	else if(BUILD == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"BUILD");

		m_build_hp += m_fbuild_tick * GETTIME;
		m_unitinfo.hp = (int)m_build_hp;

		if(m_unitinfo.hp >= m_unitinfo.maxhp )
		{
			m_unitinfo.hp = m_unitinfo.maxhp;
			m_unitinfo.estate = IDLE;
			CTerran_building::Build_Complete();
		}
	}

	CTerran_building::fire_eff_update();


	//D3DXVECTOR2 vpos;
	//vpos = CMyMath::index_to_Pos(m_curidx32 , 128 , 32);
	//vpos.x -= CScrollMgr::m_fScrollX;
	//vpos.y -= CScrollMgr::m_fScrollY;
	//CFontMgr::GetInstance()->Setbatch_Font(L"@" , m_vPos.x - CScrollMgr::m_fScrollX, 
	//	m_vPos.y - CScrollMgr::m_fScrollY);

}

void CBunker::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_com_anim->Render();

	CLineMgr::GetInstance()->collisionbox_render(m_rect);

	CTerran_building::fire_eff_render();
}

void CBunker::Release(void)
{
	CTerran_building::area_release();
	CUnitMgr::GetInstance()->clear_destroy_unitlist(this);
}

void CBunker::Dead(void)
{
	CObj* pobj = new CGeneraEff(L"XLARGEBANG" , m_vPos , D3DXVECTOR2(1.f,1.f) , SORT_GROUND);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);


	pobj = new CCorpse(L"" , L"TBDSMALL_WRECKAGE");
	pobj->SetPos(m_vPos.x , m_vPos.y);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddCorpse(pobj);

	if(!m_unitlist.empty())
	{
		list<CObj*>::iterator iter = m_unitlist.begin();
		list<CObj*>::iterator iter_end = m_unitlist.end();

		int icnt = 0;
		for( ; iter != iter_end; ++iter)
		{
			(*iter)->unit_area_Initialize();
			(*iter)->SetState(IDLE);
			(*iter)->SetOrder(ORDER_NONE);

			if(0 == icnt)
				(*iter)->SetPos(m_vPos.x - 16 , m_vPos.y - 16);
			else if(1 == icnt)
				(*iter)->SetPos(m_vPos.x + 16 , m_vPos.y - 16);
			else if(2 == icnt)
				(*iter)->SetPos(m_vPos.x - 16 , m_vPos.y + 16);
			else if(3 == icnt)
				(*iter)->SetPos(m_vPos.x + 16 , m_vPos.y + 16);

			++icnt;
		}
		m_unitlist.clear();
	}
}

void CBunker::Inputkey_reaction(const int& nkey)
{

}

void CBunker::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}
bool CBunker::UnitEnter_Bunker(CObj* pobj)
{
	if(OBJ_FIREBAT == pobj->GetOBJNAME() ||
		OBJ_MARINE == pobj->GetOBJNAME() ||
		OBJ_MEDIC == pobj->GetOBJNAME() ||
		OBJ_GHOST == pobj->GetOBJNAME())
	{
		if(m_unitlist.size() < 4)
		{
			pobj->SetPos(m_vPos);
			m_unitlist.push_back(pobj);
			return true;
		}
		else
			return false;
	}
	else
		return false;
}
