#include "StdAfx.h"
#include "Com_AirPathfind.h"

#include "TimeMgr.h"
#include "MyMath.h"
#include "Obj.h"
#include "ObjMgr.h"
#include "UnitMgr.h"

#include "Com_Transport.h"
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

	m_target_objcnt = 0;
}

void CCom_AirPathfind::Update(void)
{
	if(TRANSFORMING == m_pobj->GetUnitinfo().state)
		return;

	m_pTarget = CObjMgr::GetInstance()->obj_alivecheck(m_target_objid);

	if(NULL != m_pTarget)
	{
		if(m_pTarget->GetObjCountNumber() != m_target_objcnt)
		{
			m_pTarget = NULL;
			m_target_objid = 0;
		}
	}
	else
	{
		m_target_objid = 0;
	}

	TargetChase();
	Air_MovingUpdate();

}
void CCom_AirPathfind::TargetChase(void)
{
	if(NULL != m_pTarget)
	{
		m_fchase_time += GETTIME;

		if(m_fchase_time >= 0.8f)
		{
			m_fchase_time = 0.f;

			int target_curidx = CMyMath::Pos_to_index(m_pTarget->GetPos() , 32);
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
		m_fchase_time = 0.8f;
		if( ORDER_MOVE_ATTACK == m_pobj->GetUnitinfo().order)
		{
			//����� ���ù��� ���̾��ٸ� �ٽ� ������ ã�´�.
			SetGoalPos(m_vgoalpos);
		}
		else if(ORDER_NONE == m_pobj->GetUnitinfo().order)
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

	float onestep = GETTIME*(*m_fspeed);
	

	D3DXVECTOR2 vtemp = m_vcurdir * onestep;

	if(m_vpos.x + vtemp.x <= 5)
		m_vpos.x = 5.f;
	else if(m_vpos.x + vtemp.x >= 4090)
		m_vpos.x = 4090;
	else
		m_vpos.x += vtemp.x;

	if(m_vpos.y + vtemp.y <= 5)
		m_vpos.y = 5.f;
	else if(m_vpos.y + vtemp.y >= 4090)
		m_vpos.y = 4090;
	else
		m_vpos.y += vtemp.y;

	//m_vpos += m_vcurdir * onestep ;


	if(CMyMath::pos_distance(m_vpos , m_vcurdest_pos) <= onestep*onestep * 4)
	{		
		m_is_arrive = true;
		m_is_moveupdate = false;

		//m_vpos = m_vgoalpos;

		if(ORDER_MOVE == m_pobj->GetUnitinfo().order || 
			ORDER_MOVE_ATTACK == m_pobj->GetUnitinfo().order)
		{
			if(AIR_IDLE != m_pobj->GetUnitinfo().state)
			{
				m_pobj->SetOrder(ORDER_NONE);
				m_pobj->SetState(IDLE);
			}
		}
		else if(ORDER_GET_OFF == m_pobj->GetUnitinfo().order)
		{
			CComponent* pcom;
			pcom = m_pobj->GetComponent(COM_TRANSPORT);
			((CCom_Transport*)pcom)->set_landing(true);
			m_pobj->SetState(IDLE);
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
void CCom_AirPathfind::SetAir_moveupdate(bool is_update)
{
	m_is_moveupdate = is_update;
}
void CCom_AirPathfind::SetDestPos(const D3DXVECTOR2& vdestpos)
{
	//Ÿ�� ��ġ�� �������ش�
	m_is_moveupdate = true;
	m_vcurdest_pos = vdestpos;
}
void CCom_AirPathfind::SetGoalPos(const D3DXVECTOR2& vgoalpos , bool is_magicbox)
{
	m_is_moveupdate = true;
	

	gap_initialize(is_magicbox);
	m_vgoalpos = vgoalpos + m_vgap;
	m_vcurdest_pos = vgoalpos + m_vgap; 
}

void CCom_AirPathfind::SetTargetObjID(const int objid)
{
	m_target_objid = objid;

	m_pTarget = CObjMgr::GetInstance()->obj_alivecheck(m_target_objid);

	if(NULL != m_pTarget)
	{
		m_target_objcnt = m_pTarget->GetObjCountNumber();
	}
}

bool CCom_AirPathfind::Getarrive(void)
{
	return m_is_arrive;
}
void CCom_AirPathfind::gap_initialize(bool is_magicbox)
{
	if(true == is_magicbox)
		m_vgap = m_pobj->GetPos() - CUnitMgr::GetInstance()->GetUnitCentterPt();
	else
		m_vgap = D3DXVECTOR2(0.f, 0.f);
}

