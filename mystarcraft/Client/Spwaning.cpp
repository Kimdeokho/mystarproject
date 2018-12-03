#include "StdAfx.h"
#include "Spwaning.h"

#include "MyMath.h"
#include "TimeMgr.h"
#include "Ingame_UIMgr.h"
#include "MyMath.h"
#include "ScrollMgr.h"
#include "Ingame_UIMgr.h"
#include "FontMgr.h"
#include "ObjMgr.h"
#include "UnitMgr.h"

#include "Com_fog.h"
#include "Com_Creep.h"
#include "Com_ZBuildingAnim.h"
#include "Com_Creep.h"
#include "Com_Collision.h"

#include "UI_Wireframe.h"
#include "UI_Select.h"
#include "UI_Energy_bar.h"
#include "UI_Cmd_info.h"

#include "Corpse.h"
#include "GeneraEff.h"

CSpwaning::CSpwaning(void)
{
}

CSpwaning::~CSpwaning(void)
{
	Release();
}

void CSpwaning::Initialize(void)
{
	m_vertex.left = 36.f;
	m_vertex.right = 41.f;
	m_vertex.top =  28.f;
	m_vertex.bottom = 19.f;

	CZerg_building::building_area_Initialize(2 , 3);
	CZerg_building::building_pos_Initialize(2 , 3);

	m_eOBJ_NAME = OBJ_SPAWNING;
	m_sortID = SORT_GROUND;
	m_ecategory = CATEGORY_BUILDING;

	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.state = BUILD;
	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_LARGE;	
	m_unitinfo.maxhp = 750;
	m_unitinfo.hp = 0;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 0;
	m_unitinfo.search_range = 0;
	m_unitinfo.fog_range = 32*8*2;
	m_unitinfo.fbuildtime = 10.f;
	m_unitinfo.armor = 1;

	m_com_anim = new CCom_ZBuildingAnim(L"Z_SPWANING_POOL" , m_matWorld);

	m_com_anim->SetTextureName(L"Z_COCOON");
	m_com_anim->SetAnimation(L"BUILD_S");


	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_unitinfo.fog_range) ));	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_Collision(m_vPos , m_rect , m_vertex , false) ) );	


	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_select_ui = new CUI_Select(L"Select110" , m_vPos , 10);
	m_select_ui->Initialize();

	m_energybar_ui = new CUI_Energy_bar(this , 116);
	m_energybar_ui->Initialize();

	m_fbuild_tick = float(m_unitinfo.maxhp)/m_unitinfo.fbuildtime;
	m_build_maxhp = m_unitinfo.maxhp;
}

void CSpwaning::Update(void)
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

			CIngame_UIMgr::GetInstance()->BuildTech_Update(Z_SPWANING_POOL , 1);
		}
	}

	m_select_ui->Update();
	m_energybar_ui->Update();
	//CFontMgr::GetInstance()->Setnumber_combine_Font(L"%d" , 111 , 400 , 300);
}

void CSpwaning::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_select_ui->Render();

	m_com_anim->Render();

	m_energybar_ui->Render();
}

void CSpwaning::Inputkey_reaction(const int& nkey)
{

}

void CSpwaning::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}

void CSpwaning::Update_Cmdbtn(void)
{

}

void CSpwaning::Update_Wireframe(void)
{

}

void CSpwaning::Dead(void)
{
	CObj* pobj = new CGeneraEff(L"BLOOD_BOOM" , m_vPos , D3DXVECTOR2(1.f,1.f) , SORT_GROUND);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddEffect(pobj);

	pobj = new CCorpse(L"" , L"ZBD_S_WRECKAGE");
	pobj->SetPos(m_vPos.x , m_vPos.y);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddCorpse(pobj);

	CUnitMgr::GetInstance()->clear_destroy_unitlist(this);
}

void CSpwaning::Release(void)
{
	CZerg_building::area_release();
}
