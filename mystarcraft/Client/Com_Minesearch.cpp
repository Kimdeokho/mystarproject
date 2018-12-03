#include "StdAfx.h"
#include "Com_Minesearch.h"

#include "Obj.h"
#include "ObjMgr.h"
#include "Area_Mgr.h"
#include "MyMath.h"

#include "Com_Weapon.h"
#include "Com_Pathfind.h"
#include "TimeMgr.h"

CCom_Minesearch::CCom_Minesearch(TARGET_SEARCH_TYPE esearch_type)
{
	m_search_type = esearch_type;
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

	m_meleerangeX = 2.5f;
	m_meleerangeY = 2.5f;

	m_psearch_range = &(m_pobj->GetUnitinfo().search_range);

	m_search_time = 0.f;
	m_fdelay = 0.f;
	m_btarget_search = true;
	m_bwait = false;
}

void CCom_Minesearch::Update(void)
{
	m_ptarget = CObjMgr::GetInstance()->obj_alivecheck(m_target_objid);

	if( NULL != m_ptarget)
	{
	}
	else
		m_target_objid = 0;

	if(NULL != m_com_pathfind)
		((CCom_Pathfind*)m_com_pathfind)->SetTargetObjID(m_target_objid);


	if(ORDER_MOVE != m_pobj->GetUnitinfo().order)
	{
		m_search_time += GETTIME;

		if(	NULL == m_ptarget &&
			m_search_time > 0.2f)
		{
			m_search_time = 0.f;
			m_ptarget = CArea_Mgr::GetInstance()->Auto_explore_target(m_pobj , *m_psearch_range , m_search_type);			
		}

		if(NULL != m_ptarget)
		{
			m_target_objid = m_ptarget->GetObjNumber();
		}
		else
		{
			m_target_objid = 0;

		}
	}

	if(m_bwait)
	{
		m_fdelay += GETTIME;

		if(m_fdelay > 0.5f)
		{
			m_bwait = false;
			m_fdelay = 0.f;
		}
		else
			return;

	}
	if(NULL != m_ptarget)
	{
		if( BURROW == m_pobj->GetUnitinfo().state)
		{
			m_pobj->SetState(PULL);
			m_bwait = true;
			return;
		}

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

		}
		else
		{
			if(NULL != m_com_pathfind)
				((CCom_Pathfind*)m_com_pathfind)->SetPathfindPause(false);

		}
	}
	else
	{
		if( IDLE == m_pobj->GetUnitinfo().state ||
			MOVE == m_pobj->GetUnitinfo().state)
		{
			//´Ù½Ã ¶¥¿¡ ¹ÚÈù´Ù
			m_bwait = false;
			m_fdelay = 0.f;

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
