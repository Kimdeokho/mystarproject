#include "StdAfx.h"
#include "Battle_bim.h"

#include "Com_LaserAnim.h"
#include "TimeMgr.h"

#include "ObjMgr.h"
#include "GeneraEff.h"
#include "MyMath.h"

#include "ScrollMgr.h"
#include "Skill_DarkSwarm.h"
CBattle_bim::CBattle_bim(const int& ptarget_id , const D3DXVECTOR2& vdesetpos)
{
	m_ptarget = NULL;
	m_target_id = ptarget_id;
	m_vdest_pos = vdesetpos;
}

CBattle_bim::~CBattle_bim(void)
{
}

void CBattle_bim::Initialize(void)
{
	m_sortID = SORT_AIR_EFF;

	m_vcurdir = m_vdest_pos - m_vPos;

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , new CCom_LaserAnim(m_matWorld , L"BATTLE_BIM" , 2)));

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_ftick_distance = GETTIME*1000;
}

void CBattle_bim::Update(void)
{
	m_ptarget = CObjMgr::GetInstance()->obj_alivecheck(m_target_id);

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();

	if(NULL != m_ptarget)
	{
		m_vdest_pos = m_ptarget->GetPos();
		m_old_targetpos = m_vdest_pos;
	}
	else
	{
		m_target_id = 0;
		m_vdest_pos = m_old_targetpos;
	}

	D3DXVECTOR2 vdir = m_vdest_pos - m_vPos;	
	D3DXVec2Normalize(&vdir , &vdir);
	m_vPos += vdir*m_ftick_distance;

	if(CMyMath::pos_distance(m_vPos , m_vdest_pos) < m_ftick_distance*m_ftick_distance)
	{
		if(NULL != m_ptarget)
		{
			int idx = m_ptarget->Getcuridx(32);
			if(MOVE_GROUND == m_ptarget->GetUnitinfo().eMoveType)
			{
				if(CSkill_DarkSwarm::m_darkswarm_cnt[idx] == 0)
					m_ptarget->SetDamage(25 + m_upg_info[UPG_T_AIR_WEAPON].upg_cnt * 3, DAMAGE_NOMAL);
			}
			else
				m_ptarget->SetDamage(25 + m_upg_info[UPG_T_AIR_WEAPON].upg_cnt * 3, DAMAGE_NOMAL);
		}

		m_bdestroy = true;
		Dead();
	}	
}

void CBattle_bim::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Render();
}

void CBattle_bim::Release(void)
{

}

void CBattle_bim::Dead(void)
{
	SORT_ID esort_id;

	if(NULL != m_ptarget)
	{
		if(MOVE_GROUND == m_ptarget->GetUnitinfo().eMoveType)
			esort_id = SORT_GROUND;
		else
			esort_id = SORT_AIR_EFF;
	}
	else
		esort_id = SORT_GROUND;

	CObj* peff = NULL;
	if(NULL != m_ptarget)
		peff = new CGeneraEff(L"BIM_HIT" ,m_ptarget->GetPos()  , D3DXVECTOR2(1.1f , 1.1f), esort_id , 2.f);
	else
		peff = new CGeneraEff(L"BIM_HIT" ,m_vdest_pos , D3DXVECTOR2(1.1f , 1.1f), esort_id , 2.f);

	peff->Initialize();
	CObjMgr::GetInstance()->AddEffect(peff);
}
