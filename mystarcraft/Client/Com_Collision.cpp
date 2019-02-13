#include "StdAfx.h"
#include "Com_Collision.h"

#include "Obj.h"
#include "TimeMgr.h"
#include "Area_Mgr.h"
#include "TileManager.h"
#include "Com_Animation.h"
#include "ObjMgr.h"

#include "MyMath.h"

CCom_Collision::CCom_Collision(D3DXVECTOR2& vpos , MYRECT<float>& rc , const MYRECT<float>& vtx , bool bsearch_collision)
:m_vPos(vpos) , m_rect(rc) , m_vertex(vtx) , m_bsearch_collision(bsearch_collision)
{
}

CCom_Collision::~CCom_Collision(void)
{
}

void CCom_Collision::Initialize(CObj* pobj)
{
	m_pobj = pobj;

	m_target_objid = 0;
	m_search_time = 0.6f;

	m_collision_vnormal = D3DXVECTOR2(0,0);
	

	m_fspeed = &(m_pobj->GetUnitinfo().fspeed);

	m_rect.left = m_vPos.x - m_vertex.left; 
	m_rect.right = m_vPos.x + m_vertex.right;
	m_rect.top = m_vPos.y - m_vertex.top;
	m_rect.bottom = m_vPos.y + m_vertex.bottom;

	m_collision_target = NULL;

	m_nuckback_time = 0.f;

	m_ptilelist = CTileManager::GetInstance()->GetSqTile();
}

void CCom_Collision::Update(void)
{
	m_rect.left = m_vPos.x - m_vertex.left; 
	m_rect.right = m_vPos.x + m_vertex.right;
	m_rect.top = m_vPos.y - m_vertex.top;
	m_rect.bottom = m_vPos.y + m_vertex.bottom;

	if(false == m_bsearch_collision)
		return;

	if( false == m_pobj->GetUnitinfo().is_active ||
		ORDER_BUNKER_BOARDING == m_pobj->GetUnitinfo().order)
		return;
	//충돌되고있을때 ,, 그 해당유닛이 삭제되버리면 힙손상이네!


	if(NULL == m_collision_target)
	{
		m_search_time += GETTIME;

		if(m_search_time >= 0.2f )
		{
			m_collision_target = CArea_Mgr::GetInstance()->Collision_check(m_pobj);
			if(NULL != m_collision_target)
			{
				//printf("충돌! %d \n" , m_obj_id);
				m_target_objid = m_collision_target->GetObjNumber();
				m_vtargetpos = m_collision_target->GetPos();
				m_collision_vnormal = m_vtargetpos - m_vPos;
				D3DXVec2Normalize(&m_collision_vnormal , &m_collision_vnormal);

				if( int(m_collision_vnormal.x) == 0 && int(m_collision_vnormal.y == 0) )
				{
					//영벡터일때 임의의 방향벡터를 준다.
					//m_collision_vnormal = OFFSET_DIRVEC;

					m_collision_vnormal = D3DXVECTOR2( float(rand()%10 - 5) + 1 , float(rand()%10 - 5) + 1 );
					D3DXVec2Normalize(&m_collision_vnormal , &m_collision_vnormal);
				}
			}

			m_search_time = 0.f;
		}

	}
	else
	{
		m_nuckback_time += GETTIME;
		//m_collision_target = CObjMgr::GetInstance()->obj_alivecheck(m_target_objid);
		//MYRECT<float> temp;
		if(/*NULL != m_collision_target &&*/
			/*MyIntersectrect(&temp , &m_rect , &m_collision_target->GetMyRect())*/ 
			m_nuckback_time <= 0.4f)
		{			
			//임의
			int idx32 = 0;
			m_vprepos = m_vPos - GETTIME * (*m_fspeed) * m_collision_vnormal;
			idx32 = CMyMath::Pos_to_index(m_vprepos , 32);

			/*타겟과 충돌이 일어나는 중이라면*/
			BYTE byop = m_ptilelist[idx32]->byOption;

			if(MOVE_OK == byop ||
				RESOURCE_MINERAL == byop ||
				RESOURCE_GAS == byop)
			{
				m_vPos = m_vprepos;
				m_rect.left = m_vPos.x - m_vertex.left; 
				m_rect.right = m_vPos.x + m_vertex.right;
				m_rect.top = m_vPos.y - m_vertex.top;
				m_rect.bottom = m_vPos.y + m_vertex.bottom;

				m_pobj->Setdir(-m_collision_vnormal);
				m_pobj->SetState(COLLISION);

				//충돌중일때 길찾기 멈춰야 할듯..
			}
			else if(MOVE_NONE == byop)
			{
				D3DXVECTOR2 temppos = CMyMath::index_to_Pos(idx32 ,SQ_TILECNTX, 32);

				D3DXVECTOR2 vtemp_dir =  temppos - m_vPos;
				D3DXVec2Normalize(&vtemp_dir , &vtemp_dir);

				m_vPos -= GETTIME * (*m_fspeed) * vtemp_dir;

				m_rect.left = m_vPos.x - m_vertex.left; 
				m_rect.right = m_vPos.x + m_vertex.right;
				m_rect.top = m_vPos.y - m_vertex.top;
				m_rect.bottom = m_vPos.y + m_vertex.bottom;

				m_pobj->Setdir(-vtemp_dir);
				m_pobj->SetState(COLLISION);

				m_collision_vnormal = vtemp_dir;

				
				//m_collision_target = NULL;
				//m_nuckback_time = 0.f;
			}
		}
		else
		{
			m_nuckback_time = 0.f;
			m_collision_target = NULL;
			//현재 받고있는 명령에따라 상태를 해준다.
			//if(TRANSFORMING != m_pobj->GetUnitinfo().state)
			m_pobj->SetState(IDLE);
		}

	}
}

void CCom_Collision::Release(void)
{

}

void CCom_Collision::SetCollsearch(bool bcoll_search)
{
	m_bsearch_collision = bcoll_search;
}
