#include "StdAfx.h"
#include "Com_Medicsearch.h"

#include "Obj.h"
#include "ObjMgr.h"
#include "Area_Mgr.h"
#include "MyMath.h"

#include "Com_Weapon.h"
#include "Com_Pathfind.h"
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
}

void CCom_Medicsearch::Update(void)
{
	if(/*COLLISION == m_pobj->GetUnitinfo().estate ||*/
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

	if(ORDER_MOVE != m_pobj->GetUnitinfo().eorder)
	{
		if(NULL == m_ptarget)
		{
			m_ptarget = CArea_Mgr::GetInstance()->AutoSearch_target(m_pobj , *m_psearch_range , m_search_type);			
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

void CCom_Medicsearch::Render(void)
{

}

void CCom_Medicsearch::Release(void)
{

}
