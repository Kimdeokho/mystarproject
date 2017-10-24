#include "StdAfx.h"
#include "Com_Meleesearch.h"

#include "Obj.h"
#include "ObjMgr.h"
#include "Area_Mgr.h"
#include "MyMath.h"

#include "Com_Weapon.h"
#include "Com_Pathfind.h"
CCom_Meleesearch::CCom_Meleesearch(const int* attack_range , const int* search_range , TARGET_SEARCH_TYPE esearch_type )
{
	m_pattack_range = attack_range;
	m_psearch_range = search_range;
	m_search_type = esearch_type;
}

CCom_Meleesearch::~CCom_Meleesearch(void)
{
}

void CCom_Meleesearch::Initialize(CObj* pobj /*= NULL*/)
{
	m_ptarget = NULL;
	m_bmelee_search = false;

	m_pobj = pobj;

	m_com_pathfind = (m_pobj->GetComponent(COM_PATHFINDE));
	m_com_anim = (m_pobj->GetComponent(COM_ANIMATION));
	m_com_weapon = (m_pobj->GetComponent(COM_WEAPON));

	m_target_objid = -1;
	m_bmelee_search = true;
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
		m_bmelee_search = true;
		m_target_objid = 0;
	}

	if(NULL != m_com_pathfind)
		((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(m_target_objid);




	if(true == m_bforced_target)
	{
		m_myrc= m_pobj->GetMyRect();
		m_myrc.left -= 5;
		m_myrc.right += 5;
		m_myrc.top -= 5;
		m_myrc.bottom += 5;
		if(RESOURCE != m_ptarget->GetCategory())
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
	else
	{
		if(ORDER_MOVE != m_pobj->GetUnitinfo().eorder)
		{
			if(NULL == m_ptarget)
			{
				m_ptarget = CArea_Mgr::GetInstance()->AutoSearch_target(m_pobj , *m_psearch_range , *m_pattack_range , m_search_type);			
			}

			if(NULL != m_ptarget)
				m_target_objid = m_ptarget->GetObjNumber();
			else
			{
				m_bforced_target = false;
				m_bmelee_search = true;
				m_target_objid = 0;

			}
		}
		else
		{
			m_bforced_target = false;
			m_bmelee_search = false;
			m_target_objid = 0;
			m_ptarget = NULL;
		}



		if(NULL != m_ptarget)
		{
			m_myrc= m_pobj->GetMyRect();
			m_myrc.left -= 5;
			m_myrc.right += 5;
			m_myrc.top -= 5;
			m_myrc.bottom += 5;


			if( true == m_bmelee_search &&
				CArea_Mgr::GetInstance()->MeleeAttack_Search(m_pobj ,m_ptarget/*새타겟을 얻기위함*/ , m_myrc , m_search_type))
			{
				m_target_objid = m_ptarget->GetObjNumber();
				m_bmelee_search = false;
			}

			if(MyIntersectrect(&m_outrc , &m_myrc , &(m_ptarget->GetMyRect()) ) )
			{
				m_pobj->Setdir( (m_ptarget)->GetPos() - m_pobj->GetPos());

				if(NULL != m_com_weapon)
					((CCom_Weapon*)m_com_weapon)->fire(m_ptarget);

				if(NULL != m_com_pathfind)
				{
					((CCom_Pathfind*)m_com_pathfind)->ClearPath();
					((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
				}
				m_bmelee_search = false;
			}
			else
			{				
				if( false == m_bforced_target &&
					CMyMath::pos_distance( (m_ptarget)->GetPos() , m_pobj->GetPos()) > (*m_psearch_range)*(*m_psearch_range))
				{
					//추적범위 밖
					//m_pobj->SetState(IDLE);
					m_ptarget = NULL;
					m_target_objid = 0;

					if(NULL != m_com_pathfind)
						((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
				}
				else
				{
					if(NULL != m_com_pathfind)
						((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(false);
				}
				m_bmelee_search = true;
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
