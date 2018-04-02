#include "StdAfx.h"
#include "Goliath_part.h"

#include "Com_Distancesearch.h"
#include "Com_WGoliath.h"
#include "Com_GolarmAnim.h"

#include "ScrollMgr.h"
#include "MyMath.h"
#include "Goliath.h"
#include "Area_Mgr.h"
CGoliath_part::CGoliath_part(CObj* goliath_leg)
{
	m_goliath_leg = goliath_leg;
}

CGoliath_part::~CGoliath_part(void)
{
}

void CGoliath_part::Initialize(void)
{
	m_etarget_movetype = MOVE_GROUND;

	m_vcurdir = CMyMath::dgree_to_dir(2*22.5f);

	m_eteamnumber = TEAM_0;


	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.state = IDLE;
	m_ecategory = CATEGORY_UNIT;
	m_eOBJ_NAME = OBJ_GOLIATH;

	m_unitinfo.order = ORDER_NONE;
	m_unitinfo.eArmorType = ARMOR_LARGE;
	m_unitinfo.hp = 0;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 0;
	m_unitinfo.search_range = 255;
	m_unitinfo.attack_range = 6*32;
	m_unitinfo.air_attack_range = 5*32;

	m_unitinfo.fog_range = 512;

	m_com_targetsearch = new CCom_Distancesearch(SEARCH_ONLY_ENEMY , m_goliath_leg);
	m_com_anim = new CCom_GolarmAnim(m_matWorld );
	m_com_weapon = new CCom_WGoliath();

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_TARGET_SEARCH ,  m_com_targetsearch ) );	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_WEAPON ,  m_com_weapon )) ;

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

}

void CGoliath_part::Update(void)
{
	m_curidx32 = CMyMath::Pos_to_index(m_vPos ,32);
	m_curidx64 = CMyMath::Pos_to_index(m_vPos , 64);

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();


	if(IDLE == m_unitinfo.state)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
		m_goliath_leg->SetState(IDLE);
	}
	else if(ATTACK == m_unitinfo.state)
	{
		if(MOVE_AIR == m_etarget_movetype)
		{
			((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
		}
		else
		{
			((CCom_Animation*)m_com_anim)->SetAnimation(L"ATTACK");
		}		
		m_goliath_leg->SetState(IDLE);
	}
	else if(MOVE == m_unitinfo.state)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"MOVE");
		m_goliath_leg->SetState(MOVE);
	}


	if( false == m_upg_feedback[UPG_T_VFC3] && m_upg_info[UPG_T_VFC3].upg_cnt >= 1)
	{
		m_unitinfo.air_attack_range += 3*32;
		m_upg_feedback[UPG_T_VFC3] = true;
		((CCom_Distancesearch*)m_com_targetsearch)->Range_update();
	}
}

void CGoliath_part::Render(void)
{
	if( false == m_unitinfo.is_active )
		return;

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_com_anim->Render();
}

void CGoliath_part::Release(void)
{

}

void CGoliath_part::Inputkey_reaction(const int& nkey)
{
	if(VK_RBUTTON == nkey)
	{
		m_unitinfo.state = MOVE;
		m_unitinfo.order = ORDER_MOVE;

		CObj* ptarget = CArea_Mgr::GetInstance()->GetChoiceTarget();
		((CCom_Targetsearch*)m_com_targetsearch)->SetTarget(ptarget);
	}
}

void CGoliath_part::Inputkey_reaction(const int& firstkey , const int& secondkey)
{
	if('A' == firstkey && VK_LBUTTON == secondkey)
	{
		((CCom_Targetsearch*)m_com_targetsearch)->SetTarget(CArea_Mgr::GetInstance()->GetChoiceTarget());
	}
}

void CGoliath_part::Dead(void)
{

}

void CGoliath_part::settarget_movetype(MOVE_TYPE emovetype)
{
	m_etarget_movetype = emovetype;
}

void CGoliath_part::setarm_pos(const D3DXVECTOR2& vpos)
{
	m_vPos = vpos;
}
