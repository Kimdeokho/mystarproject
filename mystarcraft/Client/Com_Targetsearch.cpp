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

		CComponent* pcom = ptarget->GetComponent(COM_TRANSPORT);
		if(NULL != pcom)
			m_com_transport = ((CCom_Transport*)pcom);
	}
	else
	{
		m_target_objid = 0;
		m_bforced_target = false;
	}

	m_ptarget = ptarget;
}
