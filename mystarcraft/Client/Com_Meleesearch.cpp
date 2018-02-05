#include "StdAfx.h"
#include "Com_Meleesearch.h"

#include "Obj.h"
#include "ObjMgr.h"
#include "Area_Mgr.h"
#include "MyMath.h"

#include "Com_Weapon.h"
#include "Com_Pathfind.h"
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

	m_meleerangeX = 4.5f;
	m_meleerangeY = 4.5f;

	m_psearch_range = &(m_pobj->GetUnitinfo().search_range);

	m_search_time = 0.f;
	m_meleesearch_time = 0.f;
	m_btarget_search = true;
}

void CCom_Meleesearch::Update(void)
{
	if(/*COLLISION == m_pobj->GetUnitinfo().estate ||*/
		TRANSFORMING == m_pobj->GetUnitinfo().estate)
		return;

	m_ptarget = CObjMgr::GetInstance()->obj_alivecheck(m_target_objid);

	if( NULL != m_ptarget)
	{
		//갑작스레 팀이 바뀔때 예외처리인데 사실 지워도 상관없음
		//if(m_pobj->GetTeamNumber() == m_ptarget->GetTeamNumber())
		//{
		//	m_ptarget = NULL; 
		//	m_target_objid = 0;
		//}
	}
	else
	{
		m_bforced_target = false;
		//m_bmelee_search = true;
		m_target_objid = 0;
	}

	if(NULL != m_com_pathfind)
		((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(m_target_objid);




	if(true == m_bforced_target)
	{
		m_myrc= m_pobj->GetMyRect();
		m_myrc.left -= m_meleerangeX;
		m_myrc.right += m_meleerangeX;
		m_myrc.top -= m_meleerangeY;
		m_myrc.bottom += m_meleerangeY;

		if(RESOURCE != m_ptarget->GetCategory())
		{
			if( ORDER_MOVE == m_pobj->GetUnitinfo().eorder &&
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
					else if(OBJ_DROPSHIP == m_ptarget->GetOBJNAME())
					{
						//if(CMyMath::pos_distance( (m_ptarget)->GetPos() , m_pobj->GetPos()) < 32*32)
						//{			
							//범위에 들어오면
							m_pobj->Setdir( (m_ptarget)->GetPos() - m_pobj->GetPos());

							if(true == ((CDropship*)m_ptarget)->setunit(m_pobj))
							{
								m_pobj->SetSelect(NONE_SELECT);
								m_pobj->area_release();
								m_pobj->SetState(BOARDING); 
							}
							else
							{
								m_pobj->SetState(IDLE);						
							}
							m_pobj->SetOrder(ORDER_NONE);
						//}
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
			else
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
					if(NULL != m_com_pathfind)
						((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(false);
				}
			}

		}
	}
	else
	{
		if(ORDER_MOVE != m_pobj->GetUnitinfo().eorder)
		{
			m_search_time += GETTIME;

			if(true == m_btarget_search &&
				m_search_time > 0.2f)
			{
				m_search_time = 0.f;
				m_ptarget = CArea_Mgr::GetInstance()->Auto_explore_target(m_pobj , *m_psearch_range , m_search_type);			
			}

			if(NULL != m_ptarget)
				m_target_objid = m_ptarget->GetObjNumber();
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
			if(ORDER_BUNKER_BOARDING == m_pobj->GetUnitinfo().eorder)
			{
				m_meleerangeX = 48;
				m_meleerangeY = 32;
			}
			else
			{
				m_meleerangeX = 6.f;
				m_meleerangeY = 6.f;
			}

			m_myrc = m_pobj->GetMyRect();
			m_myrc.left -= m_meleerangeX;
			m_myrc.right += m_meleerangeX;
			m_myrc.top -= m_meleerangeY;
			m_myrc.bottom += m_meleerangeY;

			//m_meleesearch_time += GETTIME;

			//if(m_meleesearch_time > 0.2f)
			//{
			//	m_meleesearch_time = 0.f;
			//	if( true == m_bmelee_search &&
			//		CArea_Mgr::GetInstance()->MeleeAttack_Search(m_pobj ,m_ptarget/*새타겟을 얻기위함*/ , m_myrc , m_search_type))
			//	{
			//		m_target_objid = m_ptarget->GetObjNumber();
			//		m_bmelee_search = false;
			//	}
			//}

			if( MyIntersectrect(&m_outrc , &m_myrc , &(m_ptarget->GetMyRect()) ) )
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
				//m_bmelee_search = false;
			}
			else
			{				
				if(CMyMath::pos_distance( (m_ptarget)->GetPos() , m_pobj->GetPos()) > (*m_psearch_range)*(*m_psearch_range))
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
					if(NULL != m_com_pathfind)
						((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(false);
				}
				//m_bmelee_search = true;
				m_btarget_search = true;
			}
		}
	}
}

void CCom_Meleesearch::Render(void)
{

}

void CCom_Meleesearch::Release(void)
{

}
