#include "StdAfx.h"
#include "Com_AirCollision.h"

#include "Obj.h"
#include "TimeMgr.h"
#include "Area_Mgr.h"
#include "TileManager.h"
#include "Com_Animation.h"
#include "ObjMgr.h"

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
	m_search_time = 0.f;

	m_collision_vnormal = D3DXVECTOR2(0,0);


	m_fspeed = &(m_pobj->GetUnitinfo().fspeed);

	m_air_vertex.left = m_vertex.left/2;
	m_air_vertex.right = m_vertex.right/2;
	m_air_vertex.top = m_vertex.top/2;
	m_air_vertex.bottom = m_vertex.bottom/2;

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

	//�浹�ǰ������� ,, �� �ش������� �����ǹ����� ���ջ��̳�!

	if(NULL == m_collision_target)
	{
		m_search_time += GETTIME;

		if(m_search_time >= 0.2f )
		{
			m_collision_target = CArea_Mgr::GetInstance()->Collision_check(m_pobj);
			if(NULL != m_collision_target)
			{
				//printf("�浹! %d \n" , m_obj_id);
				m_target_objid = m_collision_target->GetObjNumber();
				m_vtargetpos = m_collision_target->GetPos();
				m_collision_vnormal = m_vtargetpos - m_vPos;
				D3DXVec2Normalize(&m_collision_vnormal , &m_collision_vnormal);

				if( int(m_collision_vnormal.x) == 0 && int(m_collision_vnormal.y == 0) )
				{
					//�������϶� ������ ���⺤�͸� �ش�.
					//m_collision_vnormal = OFFSET_DIRVEC;
					m_collision_vnormal = D3DXVECTOR2( float(rand()%10 - 5) , float(rand()%10 - 5) );
					D3DXVec2Normalize(&m_collision_vnormal , &m_collision_vnormal);
				}
			}

			m_search_time = 0.f;
		}

	}
	else
	{
		m_collision_target = CObjMgr::GetInstance()->obj_alivecheck(m_target_objid);
		MYRECT<float> temp;
		if(NULL != m_collision_target &&
			MyIntersectrect(&temp , &m_rect , &m_collision_target->GetMyRect()) )
		{
			/*Ÿ�ٰ� �浹�� �Ͼ�� ���̶��*/

			m_vPos -= GETTIME* m_collision_vnormal * 5;

		}
		else
		{
			m_collision_target = NULL;
			//���� �ް��ִ� ��ɿ����� ���¸� ���ش�.
			m_pobj->SetState(IDLE);
		}

	}
}

void CCom_AirCollision::Release(void)
{

}
