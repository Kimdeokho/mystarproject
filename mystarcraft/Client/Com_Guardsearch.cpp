#include "StdAfx.h"
#include "Com_Guardsearch.h"

#include "Obj.h"
#include "ObjMgr.h"
#include "TimeMgr.h"
#include "Area_Mgr.h"
#include "MyMath.h"

#include "Com_Weapon.h"
#include "Com_Animation.h"
#include "Com_Pathfind.h"
#include "Bunker.h"
#include "Dropship.h"

CCom_Guardsearch::CCom_Guardsearch(const TARGET_SEARCH_TYPE esearch_type )
{
	m_search_type = esearch_type;
}

CCom_Guardsearch::~CCom_Guardsearch(void)
{
}
void CCom_Guardsearch::Initialize(CObj* pobj /*= NULL*/)
{
	m_ptarget = NULL;

	m_pobj = pobj;

	m_com_anim = (m_pobj->GetComponent(COM_ANIMATION));
	m_com_weapon = (m_pobj->GetComponent(COM_WEAPON));

	m_pattack_range = &( m_pobj->GetUnitinfo().attack_range );
	m_pair_range = &( m_pobj->GetUnitinfo().air_attack_range );
	m_psearch_range = &(m_pobj->GetUnitinfo().search_range);	

	m_target_objid = -1;
	m_search_time = 0.f;
	m_btarget_search = true;

	m_irange = 0;
	Range_update();
}

void CCom_Guardsearch::Update(void)
{
	if(COLLISION == m_pobj->GetUnitinfo().state ||
		TRANSFORMING == m_pobj->GetUnitinfo().state ||
		false == m_pobj->GetUnitinfo().is_active)
		return;

	m_ptarget = CObjMgr::GetInstance()->obj_alivecheck(m_target_objid);

	if( NULL == m_ptarget)
	{
		m_bforced_target = false;
		m_target_objid = 0;
	}
	else
	{
		TEAM_NUMBER eteam = m_pobj->GetTeamNumber();

		if(false == m_ptarget->GetUnitinfo().is_active || 
			m_ptarget->GetUnitinfo().detect[eteam] < 1)
		{
			m_ptarget = NULL;
			m_target_objid = 0;
		}
	}

	if(ORDER_USINGSKILL == m_pobj->GetUnitinfo().order)
		return;

	if(true == m_bforced_target)
	{
		//강제타겟이 있을 경우
		if(CATEGORY_RESOURCE != m_ptarget->GetCategory())
		{
			if( m_pobj->GetTeamNumber() == m_ptarget->GetTeamNumber() &&
				ORDER_MOVE == m_pobj->GetUnitinfo().order)
			{
			}
			else if(ORDER_MOVE_ATTACK == m_pobj->GetUnitinfo().order)
			{
				if(CMyMath::pos_distance( (m_ptarget)->GetPos() , m_pobj->GetPos()) < 
					(*m_pattack_range)*(*m_pattack_range))
				{			
					//공격 범위에 들어오면
					m_pobj->Setdir( (m_ptarget)->GetPos() - m_pobj->GetPos());

					ATTACK_SEARCH_TYPE emy_attacktype = m_pobj->GetUnitinfo().eAttackType;
					MOVE_TYPE	etarget_movetype = m_ptarget->GetUnitinfo().eMoveType;

					if( ATTACK_ONLY_AIR == emy_attacktype)
					{
						if(MOVE_GROUND == etarget_movetype)
							return;
					}
					else if( ATTACK_ONLY_GROUND == emy_attacktype)
					{
						if(MOVE_AIR == etarget_movetype)
							return;
					}

					if(NULL != m_com_weapon)
						((CCom_Weapon*)m_com_weapon)->fire(m_ptarget);
				}
			}
		}

	}
	else
	{
		if(ORDER_MOVE != m_pobj->GetUnitinfo().order)
		{
			m_search_time += GETTIME;

			if(true == m_btarget_search &&
				m_search_time > 0.2f)
			{				
				m_search_time = 0.f;
				int att_range = max(*m_pattack_range , *m_pair_range);
				m_ptarget = CArea_Mgr::GetInstance()->Auto_explore_target(m_pobj , m_irange , att_range , m_search_type);			
			}

			if(NULL != m_ptarget)
				m_target_objid = m_ptarget->GetObjNumber();
			else
			{
				m_target_objid = 0;
				m_btarget_search = true;
			}
		}
		else
		{
			m_target_objid = 0;
			m_ptarget = NULL;
			m_bforced_target = false;
		}

		if(NULL != m_ptarget)
		{
			int range = 0;
			if( MOVE_GROUND == m_ptarget->GetUnitinfo().eMoveType)
				range = *m_pattack_range;
			else
				range = *m_pair_range;

			//원거리
			if(CMyMath::pos_distance( (m_ptarget)->GetPos() , m_pobj->GetPos()) < range * range)
			{			
				//공격 범위에 들어오면
				m_pobj->Setdir( m_ptarget->GetPos() - m_pobj->GetPos());

				if(NULL != m_com_weapon)
					((CCom_Weapon*)m_com_weapon)->fire(m_ptarget);

				m_btarget_search = false;
			}
			else if(CMyMath::pos_distance( (m_ptarget)->GetPos() , m_pobj->GetPos()) > (*m_psearch_range)*(*m_psearch_range))
			{
				//추적범위 밖이면
				//m_pobj->SetState(IDLE);
				m_ptarget = NULL;
				m_target_objid = 0;

			}
		}
		else
		{
			//if(OBJ_TURRET == m_pobj->GetOBJNAME())
			if(true == ((CCom_Animation*)m_com_anim)->GetAttack_end())
				m_pobj->SetState(IDLE);
		}
	}
}
void CCom_Guardsearch::Range_update(void)
{
	if(NULL != m_pattack_range && NULL != m_pair_range)
	{
		m_irange = max(*m_pattack_range , *m_psearch_range);
		m_irange = max(m_irange , *m_pair_range);
	}
}
void CCom_Guardsearch::Render(void)
{

}

void CCom_Guardsearch::Release(void)
{

}
