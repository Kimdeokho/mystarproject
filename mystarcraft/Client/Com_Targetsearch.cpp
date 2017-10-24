#include "StdAfx.h"
#include "Com_Targetsearch.h"

#include "Obj.h"

#include "Com_Pathfind.h"
#include "Com_Animation.h"
#include "Com_Weapon.h"
#include "Area_Mgr.h"
#include "MyMath.h"
#include "FontMgr.h"
#include "Timemgr.h"

#include "ObjMgr.h"
CCom_Targetsearch::CCom_Targetsearch(void)
{
	m_search_type = SEARCH_ANYTHING;
	m_pattack_range = NULL;
	m_psearch_range = NULL;
	m_bforced_target = false;
	m_ptarget = NULL;
}
CCom_Targetsearch::~CCom_Targetsearch(void)
{
	Release();
}

void CCom_Targetsearch::Initialize(CObj* pobj)
{
	m_ptarget = NULL;

	m_pobj = pobj;

	m_com_pathfind = (m_pobj->GetComponent(COM_PATHFINDE));
	m_com_anim = (m_pobj->GetComponent(COM_ANIMATION));
	m_com_weapon = (m_pobj->GetComponent(COM_WEAPON));

	//m_fsearchtime = 0.f;
	m_target_objid = -1;
}
void CCom_Targetsearch::Update(void)
{
	//if(/*COLLISION == m_pobj->GetUnitinfo().estate ||*/
	//	TRANSFORMING == m_pobj->GetUnitinfo().estate)
	//	return;

	////if(NULL != m_com_weapon && true == ((CCom_Weapon*)m_com_weapon)->GetFire())
	//	//return;

	//m_ptarget = CObjMgr::GetInstance()->obj_alivecheck(m_target_objid);



	//if( NULL != m_ptarget)
	//{
	//	//갑작스레 팀이 바뀔때 예외처리인데 사실 지워도 상관없음
	//	if(m_pobj->GetTeamNumber() == m_ptarget->GetTeamNumber())
	//	{
	//		m_ptarget = NULL; 
	//		m_target_objid = 0;
	//	}
	//}
	//else
	//{
	//	m_bmelee_search = true;
	//}

	//if(ORDER_MOVE != m_pobj->GetUnitinfo().eorder)
	//{
	//	if(NULL == m_ptarget)
	//	{
	//		m_ptarget = CArea_Mgr::GetInstance()->AutoSearch_target(m_pobj , *m_psearch_range , *m_pattack_range , m_search_type);			
	//	}

	//	if(NULL != m_ptarget)
	//		m_target_objid = m_ptarget->GetObjID();
	//	else
	//	{
	//		m_bmelee_search = true;
	//		m_target_objid = 0;
	//	}
	//}
	//else
	//{
	//	m_bmelee_search = true;
	//	m_target_objid = 0;
	//	m_ptarget = NULL;
	//}

	//if(NULL != m_com_pathfind)
	//	((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(m_target_objid);

	//if(NULL != m_ptarget)
	//{
	//	//근접 , 원거리
	//	//근접은 뛰어가다 부딪힌놈 때리면 되는건가

	//	if( *m_pattack_range <= 19)
	//	{
	//		MYRECT<float> myrc = m_pobj->GetMyRect();
	//		MYRECT<float> temp;

	//		myrc.left -= 5;
	//		myrc.right += 5;
	//		myrc.top -= 5;
	//		myrc.bottom += 5;

	//		if(true == m_bmelee_search &&
	//			CArea_Mgr::GetInstance()->MeleeAttack_Search(m_pobj ,m_ptarget , myrc , m_search_type))
	//		{
	//			m_target_objid = m_ptarget->GetObjID();
	//			m_bmelee_search = false;
	//		}
	//			
	//		if(MyIntersectrect(&temp , &myrc , &(m_ptarget->GetMyRect()) ) )
	//		{
	//			m_pobj->Setdir( (m_ptarget)->GetPos() - m_pobj->GetPos());

	//			if(NULL != m_com_weapon)
	//				((CCom_Weapon*)m_com_weapon)->fire(m_ptarget);

	//			if(NULL != m_com_pathfind)
	//			{
	//				((CCom_Pathfind*)m_com_pathfind)->ClearPath();
	//				((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
	//			}
	//			m_bmelee_search = false;
	//		}
	//		else
	//		{				
	//			if(CMyMath::pos_distance( (m_ptarget)->GetPos() , m_pobj->GetPos()) > (*m_psearch_range)*(*m_psearch_range))
	//			{
	//				//추적범위 밖
	//				//m_pobj->SetState(IDLE);
	//				m_ptarget = NULL;
	//				m_target_objid = 0;

	//				if(NULL != m_com_pathfind)
	//					((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
	//			}
	//			else
	//			{
	//				if(NULL != m_com_pathfind)
	//					((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(false);
	//			}
	//			m_bmelee_search = true;
	//		}

	//	}
	//	else
	//	{
	//		//원거리
	//		if(CMyMath::pos_distance( (m_ptarget)->GetPos() , m_pobj->GetPos()) < (*m_pattack_range)*(*m_pattack_range))
	//		{			
	//			//공격 범위에 들어오면

	//			m_pobj->Setdir( (m_ptarget)->GetPos() - m_pobj->GetPos());

	//			if(NULL != m_com_weapon)
	//				((CCom_Weapon*)m_com_weapon)->fire(m_ptarget);


	//			if(NULL != m_com_pathfind)
	//			{
	//				((CCom_Pathfind*)m_com_pathfind)->ClearPath();
	//				((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
	//			}
	//		}
	//		else if(CMyMath::pos_distance( (m_ptarget)->GetPos() , m_pobj->GetPos()) > (*m_psearch_range)*(*m_psearch_range))
	//		{
	//			//추적범위 밖이면
	//			m_pobj->SetState(IDLE);
	//			m_ptarget = NULL;
	//			m_target_objid = 0;

	//			if(NULL != m_com_pathfind)
	//				((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
	//		}
	//		else
	//		{
	//			//공격범위 밖, 추적범위 안이면
	//			//공격 후딜이 다 끝나고 추적
	//			if( NULL != m_com_weapon && 
	//				false == ((CCom_Weapon*)m_com_weapon)->GetFire())
	//			{
	//				if(NULL != m_com_pathfind)
	//					((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(false);
	//			}
	//		}
	//	}


	//}
}

void CCom_Targetsearch::Render(void)
{
}

void CCom_Targetsearch::Release(void)
{
	//m_ptarget = NULL;

	m_pobj = NULL;
	m_com_pathfind = NULL;
	m_com_anim = NULL;
	m_com_weapon = NULL;

}
//void CCom_Targetsearch::SetSearchtime(float time)
//{
//	//m_ptarget = NULL;
//	//m_fsearchtime = time;
//}
CObj* CCom_Targetsearch::GetTarget(void)
{
	return m_ptarget;
}


int CCom_Targetsearch::GetTargetObjid(void)
{
	return m_target_objid;
}

void CCom_Targetsearch::SetTarget(CObj* ptarget)
{
	if(NULL != ptarget)
	{
		m_bforced_target = true;
		m_target_objid = ptarget->GetObjNumber();
	}
	else
	{
		m_target_objid = 0;
		m_bforced_target = false;
	}

	m_ptarget = ptarget;
}
