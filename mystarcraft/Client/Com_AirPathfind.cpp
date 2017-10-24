#include "StdAfx.h"
#include "Com_AirPathfind.h"

#include "TimeMgr.h"
#include "MyMath.h"
#include "Obj.h"
#include "ObjMgr.h"
CCom_AirPathfind::CCom_AirPathfind(D3DXVECTOR2& vpos)
: m_vpos(vpos)
{
}

CCom_AirPathfind::~CCom_AirPathfind(void)
{
}

void CCom_AirPathfind::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	m_is_moveupdate = false;
	m_is_arrive = false;

	m_fchase_time = 0.f;
	m_target_oldidx = 0;
	m_target_objid = 0;

	m_fspeed = &(m_pobj->GetUnitinfo().fspeed);


	
}

void CCom_AirPathfind::Update(void)
{
	m_pTarget = CObjMgr::GetInstance()->obj_alivecheck(m_target_objid);

	TargetChase();
	Air_MovingUpdate();

}
void CCom_AirPathfind::TargetChase(void)
{
	if(NULL != m_pTarget)
	{
		m_fchase_time += GETTIME;

		if(m_fchase_time >= 0.2f)
		{
			m_fchase_time = 0.f;

			int target_curidx = CMyMath::Pos_to_index(m_pTarget->GetPos() , 16);
			if(target_curidx != m_target_oldidx)
			{
				m_target_oldidx = target_curidx;
				SetDestPos(m_pTarget->GetPos());
			}
		}
	}
	else
	{
		m_target_oldidx = -1;
		m_fchase_time = 0.f;
		if( ORDER_MOVE_ATTACK == m_pobj->GetUnitinfo().eorder)
		{
			//명령이 어택무브 중이었다면 다시 지형을 찾는다.
			SetDestPos(m_vgoalpos);
		}
		else if(ORDER_NONE == m_pobj->GetUnitinfo().eorder)
		{
			//m_pobj->SetState(IDLE);
		}
	}
}
void CCom_AirPathfind::Air_MovingUpdate(void)
{
	if(false == m_is_moveupdate)
		return;

	m_vcurdir = m_vcurdest_pos - m_vpos;
	D3DXVec2Normalize(&m_vcurdir , &m_vcurdir);
	m_pobj->Setdir(m_vcurdir);

	m_vpos += m_vcurdir*GETTIME*(*m_fspeed);
	if(CMyMath::pos_distance(m_vpos , m_vgoalpos) < 2*2)
	{
		m_vpos = m_vgoalpos;
		m_is_moveupdate = false;
		m_is_arrive = true;

		if(ORDER_MOVE == m_pobj->GetUnitinfo().eorder || 
			ORDER_MOVE_ATTACK == m_pobj->GetUnitinfo().eorder)
		{
			m_pobj->SetOrder(ORDER_NONE);
		}
		
	}
	else
	{
		m_is_arrive = false;
	}
}
void CCom_AirPathfind::Render(void)
{

}

void CCom_AirPathfind::Release(void)
{

}
void CCom_AirPathfind::SetAir_moveupdate(bool is_dupdate)
{
	m_is_moveupdate = is_dupdate;
}
void CCom_AirPathfind::SetDestPos(const D3DXVECTOR2& vdestpos)
{
	m_is_moveupdate = true;
	m_vcurdest_pos = vdestpos;
}
void CCom_AirPathfind::SetGoalPos(const D3DXVECTOR2& vgoalpos)
{
	m_is_moveupdate = true;
	m_vcurdest_pos = vgoalpos;
	m_vgoalpos = vgoalpos;
}

void CCom_AirPathfind::SetTargetObjID(const int& objid)
{
	m_target_objid = objid;
}

bool CCom_AirPathfind::Getarrive(void)
{
	return m_is_arrive;
}


