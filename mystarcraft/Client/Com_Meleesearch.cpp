#include "StdAfx.h"
#include "Com_Meleesearch.h"

#include "Obj.h"
#include "ObjMgr.h"
#include "Area_Mgr.h"
#include "MyMath.h"

#include "Com_Weapon.h"
#include "Com_Pathfind.h"
#include "Com_Transport.h"

#include "TimeMgr.h"
#include "Bunker.h"
#include "Dropship.h"
CCom_Meleesearch::CCom_Meleesearch(TARGET_SEARCH_TYPE esearch_type)
{
	m_search_type = esearch_type;
}

CCom_Meleesearch::~CCom_Meleesearch(void)
{
}

void CCom_Meleesearch::Initialize(CObj* pobj /*= NULL*/)
{
	m_ptarget = NULL;

	m_pobj = pobj;

	m_com_pathfind = (m_pobj->GetComponent(COM_PATHFINDE));
	m_com_anim = (m_pobj->GetComponent(COM_ANIMATION));
	m_com_weapon = (m_pobj->GetComponent(COM_WEAPON));

	m_target_objid = -1;
	//m_bmelee_search = true;

	m_meleerangeX = m_pobj->GetUnitinfo().fspeed * 0.2f;
	m_meleerangeY = m_meleerangeX;

	//m_meleerangeX = GETTIME * m_pobj->GetUnitinfo().fspeed * 3.f;
	//m_meleerangeY = GETTIME * m_pobj->GetUnitinfo().fspeed * 3.f;

	m_psearch_range = &(m_pobj->GetUnitinfo().search_range);

	m_search_time = 0.f;
	m_search_around_time = 0.f;
	m_btarget_search = true;
}

void CCom_Meleesearch::Update(void)
{
	if(/*COLLISION == m_pobj->GetUnitinfo().state ||*/
		TRANSFORMING == m_pobj->GetUnitinfo().state)
		return;

	m_ptarget = CObjMgr::GetInstance()->obj_alivecheck(m_target_objid);

	if( NULL == m_ptarget )
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
		m_myrc= m_pobj->GetMyRect();
		m_myrc.left -= m_meleerangeX;
		m_myrc.right += m_meleerangeX;
		m_myrc.top -= m_meleerangeY;
		m_myrc.bottom += m_meleerangeY;

		if(CATEGORY_RESOURCE != m_ptarget->GetCategory())
		{
			if( ORDER_MOVE == m_pobj->GetUnitinfo().order &&
				m_pobj->GetTeamNumber() == m_ptarget->GetTeamNumber())
			{
				if(MyIntersectrect(&m_outrc , &m_myrc , &(m_ptarget->GetMyRect()) ) )
				{
					m_pobj->Setdir( (m_ptarget)->GetPos() - m_pobj->GetPos());

					if(OBJ_BUNKER == m_ptarget->GetOBJNAME() )
					{
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
					}
					else if(NULL != m_com_transport)
					{
						//범위에 들어오면
						m_pobj->Setdir( (m_ptarget)->GetPos() - m_pobj->GetPos());

						if(true == m_com_transport->setunit(m_pobj))
						{
							m_pobj->SetSelect(NONE_SELECT);
							m_pobj->area_release();
							m_pobj->SetActive(false); 
							m_com_transport = NULL;
						}
						else
						{
							m_pobj->SetState(IDLE);						
						}
						m_pobj->SetOrder(ORDER_NONE);
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
			else if(ORDER_MOVE_ATTACK == m_pobj->GetUnitinfo().order)
			{
				if(MyIntersectrect(&m_outrc , &m_myrc , &(m_ptarget->GetMyRect()) ) )
				{
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
					if(NULL != m_com_pathfind)
					{
						//if(ATTACK == m_pobj->GetUnitinfo().state)
							//m_pobj->SetState(MOVE);

						((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(false);
					}
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
				m_search_time > 0.65f)
			{
				int path_size = ((CCom_Pathfind*)m_com_pathfind)->GetTargetPath_size();
				m_search_time = 0.f;
				m_ptarget = CArea_Mgr::GetInstance()->Melee_explore_target(m_pobj , *m_psearch_range , m_search_type , path_size);							
			}

			if(NULL != m_ptarget)
			{
				//m_target_objid = m_ptarget->GetObjNumber();
			}
			else
			{
				m_bforced_target = false;
				m_btarget_search = true;
				m_target_objid = 0;

			}
		}
		else
		{
			m_btarget_search = true;
			m_bforced_target = false;
			m_target_objid = 0;
			m_ptarget = NULL;
		}

		if(NULL != m_ptarget)
		{
			if(ORDER_BUNKER_BOARDING == m_pobj->GetUnitinfo().order)
			{
				m_meleerangeX = 48;
				m_meleerangeY = 32;
			}
			else
			{
				//m_meleerangeX = GETTIME * m_pobj->GetUnitinfo().fspeed * 3.f;
				//m_meleerangeX = m_pobj->GetUnitinfo().fspeed * GETTIME * 3;
				m_meleerangeX = m_pobj->GetUnitinfo().fspeed * 0.2f;
				m_meleerangeY = m_meleerangeX;
			}

			m_myrc = m_pobj->GetMyRect();
			m_myrc.left -= m_meleerangeX;
			m_myrc.right += m_meleerangeX;
			m_myrc.top -= m_meleerangeY;
			m_myrc.bottom += m_meleerangeY;

			m_search_around_time += GETTIME;
			if(m_btarget_search && m_search_around_time > 0.2f)
			{
				m_search_around_time = 0.f;
				CArea_Mgr::GetInstance()->Collision_check(m_pobj , m_ptarget , m_myrc);
			}

			m_target_objid = m_ptarget->GetObjNumber();
			m_obj_cnt = m_ptarget->GetObjCountNumber();

			if( MyIntersectrect( &m_myrc , &(m_ptarget->GetMyRect()) ) )
			{
				m_btarget_search = false;
				m_pobj->Setdir( (m_ptarget)->GetPos() - m_pobj->GetPos());

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
				if(CMyMath::pos_distance( m_ptarget->GetPos() , m_pobj->GetPos()) > (*m_psearch_range)*(*m_psearch_range))
				{
					//추적범위 밖
					m_pobj->SetState(IDLE);
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
					if(ATTACK == m_pobj->GetUnitinfo().state)
						m_pobj->SetState(MOVE);

					if(NULL != m_com_pathfind)
						((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(false);
				}
				m_btarget_search = true;
			}
		}
		else
		{
			m_btarget_search = true;

			m_search_time = 1.4f;
			m_search_around_time = 1.2f;
		}
	}
}

void CCom_Meleesearch::Render(void)
{

}

void CCom_Meleesearch::Release(void)
{

}
