#include "StdAfx.h"
#include "Com_Distancesearch.h"

#include "Obj.h"
#include "ObjMgr.h"
#include "Area_Mgr.h"
#include "MyMath.h"

#include "Com_Weapon.h"
#include "Com_Pathfind.h"
CCom_Distancesearch::CCom_Distancesearch(const int* attack_range , const int* search_range , TARGET_SEARCH_TYPE esearch_type , CObj* otherobj)
{
	m_pattack_range = attack_range;
	m_psearch_range = search_range;
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

	m_target_objid = -1;
}

void CCom_Distancesearch::Update(void)
{
	if(/*COLLISION == m_pobj->GetUnitinfo().estate ||*/
		TRANSFORMING == m_pobj->GetUnitinfo().estate)
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
		//����Ÿ���� ���� ���
		if(RESOURCE != m_ptarget->GetCategory())
		{
			if(CMyMath::pos_distance( (m_ptarget)->GetPos() , m_pobj->GetPos()) < (*m_pattack_range)*(*m_pattack_range))
			{			
				//���� ������ ������
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
			//���Ÿ�
			if(CMyMath::pos_distance( (m_ptarget)->GetPos() , m_pobj->GetPos()) < (*m_pattack_range)*(*m_pattack_range))
			{			
				//���� ������ ������
				m_pobj->Setdir( (m_ptarget)->GetPos() - m_pobj->GetPos());

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
				//�������� ���̸�
				m_pobj->SetState(IDLE);
				m_ptarget = NULL;
				m_target_objid = 0;

				if(NULL != m_com_pathfind)
					((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
			}
			else
			{
				//���ݹ��� ��, �������� ���̸�
				//���� �ĵ��� �� ������ ����
				//m_target_objid = 0;

				//m_pobj->SetState(MOVE);
				//if( NULL != m_com_weapon && 
				//	false == ((CCom_Weapon*)m_com_weapon)->GetFire())
				{
					//������ �غ� �Ǿ�����
					if(NULL != m_com_pathfind)
						((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(false);
				}
			}
		}
	}

}

void CCom_Distancesearch::Render(void)
{

}

void CCom_Distancesearch::Release(void)
{

}
