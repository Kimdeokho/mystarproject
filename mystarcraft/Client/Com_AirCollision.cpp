#include "StdAfx.h"
#include "Com_AirCollision.h"

#include "Obj.h"
#include "TimeMgr.h"
#include "Area_Mgr.h"
#include "TileManager.h"
#include "ObjMgr.h"
#include "MyMath.h"

CCom_AirCollision::CCom_AirCollision(D3DXVECTOR2& vpos , MYRECT<float>& rc , const MYRECT<float>& vtx)
:m_vPos(vpos) , m_rect(rc) , m_vertex(vtx)
{
}

CCom_AirCollision::~CCom_AirCollision(void)
{
}

void CCom_AirCollision::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	m_target_objid = 0;
	m_obj_cnt = 0;
	m_search_time = 0.f;

	m_collision_vnormal = D3DXVECTOR2(0,0);


	m_fspeed = &(m_pobj->GetUnitinfo().fspeed);

	m_air_vertex.left = m_vertex.left;
	m_air_vertex.right = m_vertex.right;
	m_air_vertex.top = m_vertex.top;
	m_air_vertex.bottom = m_vertex.bottom;

	m_rect.left = m_vPos.x - m_air_vertex.left; 
	m_rect.right = m_vPos.x + m_air_vertex.right;
	m_rect.top = m_vPos.y - m_air_vertex.top;
	m_rect.bottom = m_vPos.y + m_air_vertex.bottom;

	m_collision_target = NULL;

}

void CCom_AirCollision::Update(void)
{
	m_rect.left = m_vPos.x - m_air_vertex.left; 
	m_rect.right = m_vPos.x + m_air_vertex.right;
	m_rect.top = m_vPos.y - m_air_vertex.top;
	m_rect.bottom = m_vPos.y + m_air_vertex.bottom;

	//충돌되고있을때 ,, 그 해당유닛이 삭제되버리면 힙손상이네!

	if(NULL == m_collision_target)
	{
		m_search_time += GETTIME;

		if(m_search_time >= 1.0f )
		{
			m_collision_target = CArea_Mgr::GetInstance()->Collision_check(m_pobj);
			if(NULL != m_collision_target)
			{
				m_obj_cnt = m_collision_target->GetObjCountNumber();
				m_target_objid = m_collision_target->GetObjNumber();
				m_vtargetpos = m_collision_target->GetPos();
				m_collision_vnormal = m_vtargetpos - m_vPos;
				D3DXVec2Normalize(&m_collision_vnormal , &m_collision_vnormal);

				if( CMyMath::FLT_compare(m_collision_vnormal.x , 0.f) && 
					CMyMath::FLT_compare(m_collision_vnormal.y , 0.f))
				{
					//영벡터일때 임의의 방향벡터를 준다.
					//m_collision_vnormal = OFFSET_DIRVEC;
					m_collision_vnormal = D3DXVECTOR2( float(rand()%10 - 5) , float(rand()%10 - 5) );

					if( CMyMath::FLT_compare(m_collision_vnormal.x , 0.f) && 
						CMyMath::FLT_compare(m_collision_vnormal.y , 0.f))
						m_collision_vnormal.x = 1.f;

					D3DXVec2Normalize(&m_collision_vnormal , &m_collision_vnormal);
				}
			}

			m_search_time = 0.f;
		}

	}
	else
	{
		m_collision_target = CObjMgr::GetInstance()->obj_alivecheck(m_target_objid);

		if(NULL != m_collision_target)
		{
			if(m_obj_cnt != m_collision_target->GetObjCountNumber())
			{
				m_collision_target = NULL;
				m_target_objid = 0;
			}
		}
		MYRECT<float> temp;
		if(NULL != m_collision_target &&
			MyIntersectrect(&temp , &m_rect , &m_collision_target->GetMyRect()) )
		{
			/*타겟과 충돌이 일어나는 중이라면*/

			m_vPos -= GETTIME * m_collision_vnormal * 10;

		}
		else
		{
			m_target_objid = 0;
			m_collision_target = NULL;
			//현재 받고있는 명령에따라 상태를 해준다.
			m_pobj->SetState(IDLE);
		}

	}
}

void CCom_AirCollision::Release(void)
{

}
