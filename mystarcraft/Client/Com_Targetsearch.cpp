#include "StdAfx.h"
#include "Com_Targetsearch.h"

#include "Obj.h"

#include "Com_Pathfind.h"
#include "Com_Animation.h"
#include "Area_Mgr.h"
#include "MyMath.h"

CCom_Targetsearch::CCom_Targetsearch(void)
{
	m_search_type = SEARCH_ANYTHING;
	m_iattack_range = 0;
	m_isearch_range = 0;
}

CCom_Targetsearch::CCom_Targetsearch(const int& attack_range , const int& search_range , TARGET_SEARCH_TYPE esearch_type)
{
	m_iattack_range = attack_range;
	m_isearch_range = search_range;
	m_search_type = esearch_type;
}

CCom_Targetsearch::~CCom_Targetsearch(void)
{
}

void CCom_Targetsearch::Initialize(CObj* pobj)
{
	m_ptarget = NULL;
	m_pobj = pobj;

	m_com_pathfind = ((CCom_Pathfind*)m_pobj->GetComponent(COM_PATHFINDE));
	m_com_anim = ((CCom_Animation*)m_pobj->GetComponent(COM_ANIMATION));

}

void CCom_Targetsearch::Update(void)
{
	if(ORDER_MOVE != m_pobj->GetOrder())
		m_ptarget = CArea_Mgr::GetInstance()->AutoSearch_target(m_pobj , m_pobj->Getcuridx(256) , m_isearch_range , 256, m_search_type);
	else
		m_ptarget = NULL;

	if(NULL != m_ptarget)
	{
		if(CMyMath::pos_distance(m_ptarget->GetPos() , m_pobj->GetPos()) < m_iattack_range*m_iattack_range)
		{
			//공격

			//m_estate = ATTACK;
			m_pobj->SetState(ATTACK);
			m_com_anim->SetAnimation(L"ATTACK");
			m_com_pathfind->ClearPath();
			m_com_pathfind->SetPathfindPause(true);
		}
		else
		{
			//공격 후딜이 다 끝나고 추적
			m_com_pathfind->SetPathfindPause(false);
			m_pobj->SetState(MOVE);
			m_com_anim->SetAnimation(L"MOVE");
			//m_com_pathfind->SetGoalPos(m_ptarget->GetPos());
			//m_com_pathfind->SetGoalidx(CMyMath::Pos_to_index(m_ptarget->GetPos() , 32));
		}
	}
}

void CCom_Targetsearch::Render(void)
{

}

void CCom_Targetsearch::Release(void)
{

}

CObj* CCom_Targetsearch::GetTarget(void)
{
	return m_ptarget;
}
