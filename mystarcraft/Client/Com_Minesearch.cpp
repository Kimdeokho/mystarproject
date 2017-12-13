#include "StdAfx.h"
#include "Com_Minesearch.h"

#include "Obj.h"
#include "ObjMgr.h"
#include "Area_Mgr.h"
#include "MyMath.h"

#include "Com_Weapon.h"
#include "Com_Pathfind.h"
#include "TimeMgr.h"

CCom_Minesearch::CCom_Minesearch(void)
{
}

CCom_Minesearch::~CCom_Minesearch(void)
{
}

void CCom_Minesearch::Initialize(CObj* pobj /*= NULL*/)
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
	m_btarget_search = true;
}

void CCom_Minesearch::Update(void)
{
	m_ptarget = CObjMgr::GetInstance()->obj_alivecheck(m_target_objid);

	if( NULL != m_ptarget)
	{
	}
	else
	{
		m_target_objid = 0;
	}

	if(NULL != m_com_pathfind)
		((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(m_target_objid);


	if(ORDER_MOVE != m_pobj->GetUnitinfo().eorder)
	{
		m_search_time += GETTIME;

		if(true == m_btarget_search &&
			m_search_time > 0.2f)
		{
			m_search_time = 0.f;
			m_ptarget = CArea_Mgr::GetInstance()->AutoSearch_target(m_pobj , *m_psearch_range , m_search_type);			
		}

		if(NULL != m_ptarget)
		{
			m_btarget_search = false;
			m_target_objid = m_ptarget->GetObjNumber();
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
		if( BURROW == m_pobj->GetUnitinfo().estate)
		{
			m_pobj->SetState(PULL);
			return;
		}
		else if( PULL == m_pobj->GetUnitinfo().estate)
			return;

		m_myrc = m_pobj->GetMyRect();
		m_myrc.left -= m_meleerangeX;
		m_myrc.right += m_meleerangeX;
		m_myrc.top -= m_meleerangeY;
		m_myrc.bottom += m_meleerangeY;

		if( MyIntersectrect(&m_outrc , &m_myrc , &(m_ptarget->GetMyRect()) ) )
		{
			m_pobj->Setdir( (m_ptarget)->GetPos() - m_pobj->GetPos());

			if(NULL != m_com_weapon)
				((CCom_Weapon*)m_com_weapon)->fire(m_ptarget);

			//if(NULL != m_com_pathfind)
			//{
			//	((CCom_Pathfind*)m_com_pathfind)->ClearPath();
			//	((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(true);
			//}
		}
		else
		{				
			if(CMyMath::pos_distance( (m_ptarget)->GetPos() , m_pobj->GetPos()) > (*m_psearch_range)*(*m_psearch_range))
			{
				//ÃßÀû¹üÀ§ ¹Û
				//´Ù½Ã ¶¥¿¡ ¹ÚÈù´Ù

				m_pobj->SetState(PLANT);
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
			m_btarget_search = true;
		}
	}
	else
	{
		if( IDLE == m_pobj->GetUnitinfo().estate ||
			MOVE == m_pobj->GetUnitinfo().estate)
		{
			//´Ù½Ã ¶¥¿¡ ¹ÚÈù´Ù

			m_pobj->SetState(PLANT);
		}
	}
}

void CCom_Minesearch::Render(void)
{

}

void CCom_Minesearch::Release(void)
{

}
