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


	////�浹���θ� �˻��ϰ� �浹�� Ÿ���� �����´�
	////Ÿ���� �������� �� Ÿ�ٰ� ���ڽ��� �о��
	////�о���ȿ� �浹���θ� �˻����� �ʴ´�
	////���� �� �о�� �浹 Ÿ���� NULL�� �����.
	////���� �ϲ��� �ڿ���ĳ�� ���¶�� �� �˻縦 ���� �ʴ´�.


	if(NULL == m_collision_target)
	{
		m_search_time += GETTIME;

		if(m_search_time >= 0.2f )
		{
			if(CArea_Mgr::GetInstance()->Collision_check(m_pobj->Getcuridx(64) , m_pobj , m_collision_target))
			{
				//printf("�浹! %d \n" , m_obj_id);
				/*��ã�� �̵��� �����϶�� ��ȣ�� �������ҵ�*/

				m_vtargetpos = m_collision_target->GetPos();
				m_collision_vnormal = m_vtargetpos - m_vPos;

				D3DXVec2Normalize(&m_collision_vnormal , &m_collision_vnormal);
				if( int(m_collision_vnormal.x) == 0 && int(m_collision_vnormal.y == 0) )
				{
					//�������϶� ������ ���⺤�͸� �ش�.
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
			//����
			TILE** ptilelist = CTileManager::GetInstance()->GetSqTile();


			/*Ÿ�ٰ� �浹�� �Ͼ�� ���̶��*/
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
			//���� �ް��ִ� ��ɿ����� ���¸� ���ش�.
			//m_estate = IDLE;
			m_pobj->SetState(IDLE);
			m_animation->SetAnimation(L"IDLE");
		}

	}
}

void CCom_Collision::Release(void)
{

}
