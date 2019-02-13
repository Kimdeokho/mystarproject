#include "StdAfx.h"
#include "Com_Airsearch.h"

#include "Obj.h"
#include "ObjMgr.h"
#include "Area_Mgr.h"
#include "MyMath.h"

#include "Com_Weapon.h"
#include "Com_AirPathfind.h"
CCom_Airsearch::CCom_Airsearch(void)
{
}

CCom_Airsearch::~CCom_Airsearch(void)
{
}

void CCom_Airsearch::Initialize(CObj* pobj /*= NULL*/)
{
	m_search_type = SEARCH_ONLY_ENEMY;
	m_ptarget = NULL;

	m_pobj = pobj;


	m_com_pathfind = (m_pobj->GetComponent(COM_AIR_PATHFIND));

	m_com_anim = (m_pobj->GetComponent(COM_ANIMATION));
	m_com_weapon = (m_pobj->GetComponent(COM_WEAPON));

	m_target_objid = -1;

	m_pattack_range = &( m_pobj->GetUnitinfo().attack_range );
	m_pair_range = &( m_pobj->GetUnitinfo().air_attack_range );
	m_psearch_range = &(m_pobj->GetUnitinfo().search_range);
}

void CCom_Airsearch::Update(void)
{
	if(/*COLLISION == m_pobj->GetUnitinfo().state ||*/
		TRANSFORMING == m_pobj->GetUnitinfo().state)
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


	if(NULL != m_com_pathfind)
		((CCom_AirPathfind*)m_com_pathfind)->SetTargetObjID(m_target_objid);


	if(true == m_bforced_target)
	{
		//강제타겟이 있을 경우
		if(NULL == m_ptarget)
			return;

		if(CATEGORY_RESOURCE != m_ptarget->GetCategory())
		{
			if( m_pobj->GetTeamNumber() == m_ptarget->GetTeamNumber() &&
				ORDER_MOVE == m_pobj->GetUnitinfo().order)
			{
				m_myrc= m_pobj->GetMyRect();
				m_myrc.left -= 2;
				m_myrc.right += 2;
				m_myrc.top -= 2;
				m_myrc.bottom += 2;

				if(NULL != m_com_pathfind)
					((CCom_AirPathfind*)m_com_pathfind)->SetAir_moveupdate(true);
				if(MyIntersectrect( &m_myrc , &(m_ptarget->GetMyRect()) ) )
				{
					m_pobj->SetState(IDLE);
					m_pobj->SetOrder(ORDER_NONE);
					m_target_objid = 0;
					m_ptarget = NULL;
					((CCom_AirPathfind*)m_com_pathfind)->SetTargetObjID(m_target_objid);
					((CCom_AirPathfind*)m_com_pathfind)->SetAir_moveupdate(false);
				}
			}
			else
			{
				int frange = 0;
				if( MOVE_AIR == m_ptarget->GetUnitinfo().eMoveType)
					frange = *m_pair_range;
				else 
					frange = *m_pattack_range;

				if(CMyMath::pos_distance( (m_ptarget)->GetPos() , m_pobj->GetPos()) < frange*frange)
				{		
					if(ORDER_MOVE == m_pobj->GetUnitinfo().order)
					{
						m_bforced_target = false;
						m_target_objid = 0;
						return;
					}
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


					if(NULL != m_com_pathfind)
					{
						//길찾기 중지
						((CCom_AirPathfind*)m_com_pathfind)->SetAir_moveupdate(false);
					}
				}
				else
				{
					if(NULL != m_com_pathfind)
					{
						//타겟을 쫓아가자
						((CCom_AirPathfind*)m_com_pathfind)->SetAir_moveupdate(true);
					}
				}
			}

		}

	}
	else
	{
		if(ORDER_MOVE != m_pobj->GetUnitinfo().order)
		{
			if(NULL == m_ptarget)
			{
				int att_range = max(*m_pair_range , *m_pattack_range);
				m_ptarget = CArea_Mgr::GetInstance()->Auto_explore_target(m_pobj, *m_psearch_range, att_range, m_search_type);			
			}

			if(NULL != m_ptarget)
				m_target_objid = m_ptarget->GetObjNumber();
			else
				m_target_objid = 0;
		}
		else
		{
			m_target_objid = 0;
			m_ptarget = NULL;
			m_bforced_target = false;
		}

		if(NULL != m_ptarget)
		{
			int frange = 0;
			if( MOVE_AIR == m_ptarget->GetUnitinfo().eMoveType)
				frange = *m_pair_range;
			else 
				frange = *m_pattack_range;
			//원거리
			if(CMyMath::pos_distance( (m_ptarget)->GetPos() , m_pobj->GetPos()) < frange * frange)
			{			
				//공격 범위에 들어오면
				m_pobj->Setdir( (m_ptarget)->GetPos() - m_pobj->GetPos());

				if(NULL != m_com_weapon)
					((CCom_Weapon*)m_com_weapon)->fire(m_ptarget);


				if(NULL != m_com_pathfind)
				{
					//길찾기 중지
					((CCom_AirPathfind*)m_com_pathfind)->SetAir_moveupdate(false);
				}
			}
			else if(CMyMath::pos_distance( (m_ptarget)->GetPos() , m_pobj->GetPos()) > (*m_psearch_range)*(*m_psearch_range))
			{
				//추적범위 밖이면
				m_pobj->SetState(IDLE);
				m_ptarget = NULL;
				m_target_objid = 0;

				if(NULL != m_com_pathfind)
				{
					((CCom_AirPathfind*)m_com_pathfind)->SetAir_moveupdate(false);
				}
			}
			else
			{
				//공격범위 밖, 추적범위 안이면
				//공격 후딜이 다 끝나고 추적
				//m_target_objid = 0;

				//m_pobj->SetState(MOVE);
				//if( NULL != m_com_weapon && 
				//	false == ((CCom_Weapon*)m_com_weapon)->GetFire())
				{
					//공격할 준비가 되었으면
					if(NULL != m_com_pathfind)
					{
						//타겟을 쫓아가자
						((CCom_AirPathfind*)m_com_pathfind)->SetAir_moveupdate(true);
					}
				}
			}
		}
	}
}

void CCom_Airsearch::Render(void)
{

}

void CCom_Airsearch::Release(void)
{

}
