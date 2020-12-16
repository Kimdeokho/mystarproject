#include "StdAfx.h"
#include "Com_Distancesearch.h"

#include "Obj.h"
#include "ObjMgr.h"
#include "TimeMgr.h"
#include "Area_Mgr.h"
#include "MyMath.h"

#include "Com_Weapon.h"
#include "Com_Animation.h"
#include "Com_Pathfind.h"
#include "Com_Transport.h"

#include "Bunker.h"
#include "Dropship.h"

CCom_Distancesearch::CCom_Distancesearch(TARGET_SEARCH_TYPE esearch_type , CObj* otherobj)
{
	m_search_type = esearch_type;
	m_otherobj = otherobj;
}

CCom_Distancesearch::~CCom_Distancesearch(void)
{
}

void CCom_Distancesearch::Initialize(CObj* pobj /*= NULL*/)
{
	m_ptarget = NULL;

	m_pobj = pobj;

	if(NULL == m_otherobj)
		m_com_pathfind = (m_pobj->GetComponent(COM_PATHFINDE));
	else
		m_com_pathfind = (m_otherobj->GetComponent(COM_PATHFINDE));

	m_com_anim = (m_pobj->GetComponent(COM_ANIMATION));
	m_com_weapon = (m_pobj->GetComponent(COM_WEAPON));



	m_pattack_range = &( m_pobj->GetUnitinfo().attack_range );
	m_pair_range = &( m_pobj->GetUnitinfo().air_attack_range );
	m_psearch_range = &(m_pobj->GetUnitinfo().search_range);	

	m_target_objid = 0;
	m_search_time = 0.4f;
	m_btarget_search = true;

	m_irange = 0;
	Range_update();
}

void CCom_Distancesearch::Update(void)
{
	if(COLLISION == m_pobj->GetUnitinfo().state ||
		TRANSFORMING == m_pobj->GetUnitinfo().state ||
		false == m_pobj->GetUnitinfo().is_active)
		return;

	m_ptarget = CObjMgr::GetInstance()->obj_alivecheck(m_target_objid);

	if( NULL == m_ptarget)
	{
		if(0 != m_target_objid)
		{
			//공격하고 있던 유닛이 죽은거
			m_search_time = 0.0f;
			float atttime = 0.f;
			((CCom_Weapon*)m_com_weapon)->SetAttackTime(atttime);
		}
		if(m_bforced_target)
		{
			m_pobj->SetOrder(ORDER_NONE);
			m_pobj->SetState(IDLE);
		}
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
			m_bforced_target = false;
		}

		if(NULL != m_ptarget && m_obj_cnt != m_ptarget->GetObjCountNumber())
		{
			m_ptarget = NULL;
			m_target_objid = 0;
			m_bforced_target = false;
		}
	}


	if(NULL != m_com_pathfind)
		((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(m_target_objid);

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

				if(OBJ_BUNKER == m_ptarget->GetOBJNAME())
				{
					m_myrc= m_pobj->GetMyRect();
					m_myrc.left -= 4;
					m_myrc.right += 4;
					m_myrc.top -= 4;
					m_myrc.bottom += 4;
					if(MyIntersectrect(&m_myrc , &(m_ptarget->GetMyRect()) ) )
					{
						m_bforced_target = false;

						m_pobj->Setdir( (m_ptarget)->GetPos() - m_pobj->GetPos());

						if( true == ((CBunker*)m_ptarget)->UnitEnter_Bunker(m_pobj) )
						{
							m_pobj->SetSelect(NONE_SELECT);
							m_pobj->area_release();
							m_pobj->SetState(IDLE); 
							m_pobj->SetOrder(ORDER_BUNKER_BOARDING);

						}
						else
						{
							m_pobj->SetState(IDLE);
							m_pobj->SetOrder(ORDER_NONE);
						}
						m_ptarget = NULL;
						m_target_objid = 0;

						if(NULL != m_com_pathfind)
						{
							((CCom_Pathfind*)m_com_pathfind)->ClearPath();
							((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
						}
					}
					else
					{
						if(NULL != m_com_pathfind)
							((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(false);
					}
				}
				else if(NULL != m_com_transport)
				{
					if(CMyMath::pos_distance( m_ptarget->GetPos() , m_pobj->GetPos()) < 32*32)
					{			
						//범위에 들어오면

						m_pobj->Setdir( m_ptarget->GetPos() - m_pobj->GetPos());

						CObj* pobj = NULL;
						if(NULL != m_otherobj)
							pobj = m_otherobj;
						else
							pobj = m_pobj;

						if(true == m_com_transport->setunit(pobj))
						{
							m_pobj->SetSelect(NONE_SELECT);
							m_pobj->area_release();
							m_pobj->SetActive(false);
							m_com_transport = NULL;

							if(NULL != m_otherobj)
							{
								m_otherobj->SetSelect(NONE_SELECT);
								m_otherobj->area_release();
								m_otherobj->SetActive(false); 
							}
						}
						else
						{
							m_pobj->SetState(IDLE);
							if(NULL != m_otherobj)
								m_otherobj->SetState(IDLE);
						}

						m_pobj->SetOrder(ORDER_NONE);
						if(NULL != m_otherobj)
							m_otherobj->SetOrder(ORDER_NONE);

						m_target_objid = 0;
						m_ptarget = NULL;
						((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(m_target_objid);
						((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
					}
				}
				else
				{
					m_myrc= m_pobj->GetMyRect();
					m_myrc.left -= 6;
					m_myrc.right += 6;
					m_myrc.top -= 6;
					m_myrc.bottom += 6;

					if(NULL != m_com_pathfind)
						((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(false);
					if(MyIntersectrect( &m_myrc , &(m_ptarget->GetMyRect()) ) )
					{
						m_pobj->SetState(IDLE);
						m_pobj->SetOrder(ORDER_NONE);
						m_target_objid = 0;
						m_ptarget = NULL;
						((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(m_target_objid);
						((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
					}
				}
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

					if(NULL != m_com_pathfind)
					{
						((CCom_Pathfind*)m_com_pathfind)->ClearPath();
						((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
					}
				}
				else
				{
					//공격범위 밖, 추적범위 안이면
					if(ATTACK == m_pobj->GetUnitinfo().state)
						m_pobj->SetState(MOVE);
					if(NULL != m_com_pathfind)
						((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(false);
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
				m_search_time > 0.55f)
			{				
				m_search_time = 0.f;
				int iattrange = max(*m_pattack_range , *m_pair_range);
				m_ptarget = CArea_Mgr::GetInstance()->Auto_explore_target(m_pobj , m_irange ,iattrange , m_search_type);			
			}

			if(NULL != m_ptarget)
			{
				m_target_objid = m_ptarget->GetObjNumber();
				m_obj_cnt = m_ptarget->GetObjCountNumber();
			}
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


				if(NULL != m_com_pathfind)
				{
					((CCom_Pathfind*)m_com_pathfind)->ClearPath();
					((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
				}

				m_btarget_search = false;
			}
			else if(CMyMath::pos_distance( (m_ptarget)->GetPos() , m_pobj->GetPos()) > (*m_psearch_range)*(*m_psearch_range))
			{
				//추적범위 밖이면				
				m_ptarget = NULL;
				m_target_objid = 0;

				if(NULL != m_com_pathfind)
				{
					m_pobj->SetState(IDLE);
					((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
				}
			}
			else
			{
				//공격범위 밖, 추적범위 안이면
				//공격 후딜이 다 끝나고 추적

				if(ORDER_BUNKER_BOARDING == m_pobj->GetUnitinfo().order)
				{
					m_ptarget = NULL;
					m_target_objid = 0;
				}

				//공격할 준비가 되었으면
				if(NULL != m_com_pathfind)
				{
					if(ATTACK == m_pobj->GetUnitinfo().state)
						m_pobj->SetState(MOVE);

					((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(false);
				}					
			}
		}
		else
		{
			if(OBJ_TURRET == m_pobj->GetOBJNAME())
				m_pobj->SetState(IDLE);

			m_btarget_search = true;
			//m_search_time = 1.4f;
		}
	}
}
void CCom_Distancesearch::Range_update(void)
{
	if(NULL != m_pattack_range && NULL != m_pair_range)
	{
		m_irange = max(*m_pattack_range , *m_psearch_range);
		m_irange = max(m_irange , *m_pair_range);
	}
}
void CCom_Distancesearch::Set_ComPathfind(CComponent* pcom)
{
	m_com_pathfind = pcom;
}
void CCom_Distancesearch::Render(void)
{

}


void CCom_Distancesearch::Release(void)
{

}

