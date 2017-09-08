#include "StdAfx.h"
#include "Com_Collision.h"

#include "Obj.h"
#include "TimeMgr.h"
#include "Area_Mgr.h"
#include "TileManager.h"
#include "Com_Animation.h"
CCom_Collision::CCom_Collision(D3DXVECTOR2& vpos , MYRECT<float>& rc , const MYRECT<float>& vtx)
:m_vPos(vpos) , m_rect(rc) , m_vertex(vtx)
{
}

CCom_Collision::~CCom_Collision(void)
{
}

void CCom_Collision::Initialize(CObj* pobj)
{
	m_pobj = pobj;

	m_search_time = 0.f;

	m_collision_vnormal = D3DXVECTOR2(0,0);
	

	m_fspeed = m_pobj->GetSpeed();

	m_rect.left = m_vPos.x - m_vertex.left; 
	m_rect.right = m_vPos.x + m_vertex.right;
	m_rect.top = m_vPos.y - m_vertex.top;
	m_rect.bottom = m_vPos.y + m_vertex.bottom;

	m_collision_target = NULL;

	m_animation = (CCom_Animation*)(m_pobj->GetComponent(COM_ANIMATION));
}

void CCom_Collision::Update(void)
{
	m_rect.left = m_vPos.x - m_vertex.left; 
	m_rect.right = m_vPos.x + m_vertex.right;
	m_rect.top = m_vPos.y - m_vertex.top;
	m_rect.bottom = m_vPos.y + m_vertex.bottom;


	////충돌여부만 검사하고 충돌한 타겟을 가져온다
	////타겟을 가져오면 그 타겟과 나자신을 밀어낸다
	////밀어낼동안엔 충돌여부를 검사하지 않는다
	////전부 다 밀어내면 충돌 타겟을 NULL로 만든다.
	////만약 일꾼이 자원을캐는 상태라면 이 검사를 하지 않는다.


	if(NULL == m_collision_target)
	{
		m_search_time += GETTIME;

		if(m_search_time >= 0.2f )
		{
			if(CArea_Mgr::GetInstance()->Collision_check(m_pobj->Getcuridx(64) , m_pobj , m_collision_target))
			{
				//printf("충돌! %d \n" , m_obj_id);
				/*길찾기 이동을 중지하라는 신호를 보내야할듯*/

				m_vtargetpos = m_collision_target->GetPos();
				m_collision_vnormal = m_vtargetpos - m_vPos;

				D3DXVec2Normalize(&m_collision_vnormal , &m_collision_vnormal);
				if( int(m_collision_vnormal.x) == 0 && int(m_collision_vnormal.y == 0) )
				{
					//영벡터일때 임의의 방향벡터를 준다.
					m_collision_vnormal = OFFSET_DIRVEC;
				}
			}

			m_search_time = 0.f;
		}

	}
	else
	{
		MYRECT<float> temp;
		if( MyIntersectrect(&temp , &m_rect , &m_collision_target->GetMyRect()) )
		{
			//임의
			TILE** ptilelist = CTileManager::GetInstance()->GetSqTile();


			/*타겟과 충돌이 일어나는 중이라면*/
			if(MOVE_OK == ptilelist[m_pobj->Getcuridx(32)]->byOption)
			{
				//Dir_calculation(-m_collision_vnormal);
				m_vPos -= GETTIME* (*m_fspeed) *m_collision_vnormal;

				//m_estate = MOVE;
				m_pobj->Setdir(-m_collision_vnormal);
				m_pobj->SetState(MOVE);
				m_animation->SetAnimation(L"MOVE");
			}
		}
		else
		{
			m_collision_target = NULL;
			//현재 받고있는 명령에따라 상태를 해준다.
			//m_estate = IDLE;
			m_pobj->SetState(IDLE);
			m_animation->SetAnimation(L"IDLE");
		}

	}
}

void CCom_Collision::Release(void)
{

}
