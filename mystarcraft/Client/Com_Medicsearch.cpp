#include "StdAfx.h"
#include "Com_Medicsearch.h"

#include "Obj.h"
#include "ObjMgr.h"
#include "Area_Mgr.h"
#include "MyMath.h"

#include "Com_Weapon.h"
#include "Com_Pathfind.h"
#include "Bunker.h"

#include "TimeMgr.h"

#include "Dropship.h"
#include "Bunker.h"
CCom_Medicsearch::CCom_Medicsearch()
{
}

CCom_Medicsearch::~CCom_Medicsearch(void)
{
}

void CCom_Medicsearch::Initialize(CObj* pobj /*= NULL*/)
{
	m_ptarget = NULL;

	m_pobj = pobj;

	m_com_pathfind = (m_pobj->GetComponent(COM_PATHFINDE));

	m_com_anim = (m_pobj->GetComponent(COM_ANIMATION));
	m_com_weapon = (m_pobj->GetComponent(COM_WEAPON));

	m_target_objid = -1;

	
	m_search_type = SEARCH_ONLY_ALLY;

	m_psearch_range = &(m_pobj->GetUnitinfo().search_range);
	m_pattack_range =  &( m_pobj->GetUnitinfo().attack_range );

	m_search_time = 0.f;
	m_btarget_search = true;
}

void CCom_Medicsearch::Update(void)
{
	if(COLLISION == m_pobj->GetUnitinfo().estate ||
		TRANSFORMING == m_pobj->GetUnitinfo().estate ||
		BOARDING == m_pobj->GetUnitinfo().estate)
		return;

	m_ptarget = CObjMgr::GetInstance()->obj_alivecheck(m_target_objid);

	if( NULL == m_ptarget)
	{
		m_bforced_target = false;
		m_target_objid = 0;
	}
	else
	{
		if(BOARDING == m_ptarget->GetUnitinfo().estate)
		{
			m_ptarget = NULL;
			m_target_objid = 0;
		}
	}

	if(NULL != m_com_pathfind)
		((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(m_target_objid);

	if(true == m_bforced_target)
	{
		if( m_pobj->GetTeamNumber() == m_ptarget->GetTeamNumber())
		{
			if(ORDER_MOVE == m_pobj->GetUnitinfo().eorder)
			{
				if(OBJ_BUNKER == m_ptarget->GetOBJNAME())
				{
					m_myrc= m_pobj->GetMyRect();
					m_myrc.left -= 2;
					m_myrc.right += 2;
					m_myrc.top -= 2;
					m_myrc.bottom += 2;
					if(MyIntersectrect(&m_outrc , &m_myrc , &(m_ptarget->GetMyRect()) ) )
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
				else if(OBJ_DROPSHIP == m_ptarget->GetOBJNAME())
				{
					if(CMyMath::pos_distance( (m_ptarget)->GetPos() , m_pobj->GetPos()) < 32*32)
					{			
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

						m_target_objid = 0;
						m_ptarget = NULL;
						((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(m_target_objid);
						((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
					}
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
				m_ptarget = CArea_Mgr::GetInstance()->Explore_medic_target(m_pobj , *m_psearch_range );			
			}

			if(NULL != m_ptarget)
				m_target_objid = m_ptarget->GetObjNumber();
			else
			{
				m_btarget_search = true;
				m_target_objid = 0;
			}
		}
		else if(ORDER_MOVE == m_pobj->GetUnitinfo().eorder)
		{

		}
		else
		{
			m_target_objid = 0;
			m_ptarget = NULL;
			m_bforced_target = false;
			m_btarget_search = true;
		}

		if(NULL != m_ptarget)
		{
			//원거리
			if(CMyMath::pos_distance( (m_ptarget)->GetPos() , m_pobj->GetPos()) < (*m_pattack_range)*(*m_pattack_range))
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
			}
			else if(CMyMath::pos_distance( (m_ptarget)->GetPos() , m_pobj->GetPos()) > (*m_psearch_range)*(*m_psearch_range))
			{
				//추적범위 밖이면
				m_pobj->SetState(IDLE);
				m_ptarget = NULL;
				m_target_objid = 0;

				if(NULL != m_com_pathfind)
					((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
			}
			else
			{
				//공격범위 밖, 추적범위 안이면
				//공격 후딜이 다 끝나고 추적

				if(ORDER_BUNKER_BOARDING == m_pobj->GetUnitinfo().eorder)
				{
					m_ptarget = NULL;
					m_target_objid = 0;
				}

				if(m_ptarget->GetUnitinfo().maxhp <= m_ptarget->GetUnitinfo().hp)
				{
					m_ptarget = NULL;
					m_target_objid = 0;

					if(NULL != m_com_pathfind)
						((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
				}
				else
				{
					//공격할 준비가 되었으면
					if(NULL != m_com_pathfind)
						((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(false);
				}
			}
		}
	}
}

void CCom_Medicsearch::Render(void)
{

}

void CCom_Medicsearch::Release(void)
{

}
