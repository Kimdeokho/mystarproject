#include "StdAfx.h"
#include "Com_Pathfind.h"

#include "Obj.h"
#include "Astar.h"
#include "TileManager.h"
#include "MyMath.h"
#include "TimeMgr.h"
#include "Com_Targetsearch.h"
#include "Com_Animation.h"
#include "Com_Collision.h"

#include "Area_Mgr.h"

#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "Device.h"
#include "FontMgr.h"
#include "ObjMgr.h"
#include "UnitMgr.h"
CCom_Pathfind::CCom_Pathfind(D3DXVECTOR2& vpos , MYRECT<float>&	rc)
:m_vPos(vpos) , m_rect(rc)
{
	m_pTarget = NULL;
}

CCom_Pathfind::~CCom_Pathfind(void)
{
	Release();
}
void CCom_Pathfind::Initialize(CObj* pobj)
{
	m_pobj = pobj;

	m_Astar = new CAstar;
	m_Astar->Initialize(m_pobj);



	m_com_target = (m_pobj->GetComponent(COM_TARGET_SEARCH));
	m_com_animation = (m_pobj->GetComponent(COM_ANIMATION));
	m_com_collision = (m_pobj->GetComponent(COM_COLLISION));

	m_target_objid = 0;
	m_obj_vertex = m_pobj->GetVertex();
	//m_fspeed = m_pobj->GetSpeed();
	m_fspeed = &(m_pobj->GetUnitinfo().fspeed);

	m_stepsize = 16;

	
	m_terrainpath.reserve(256);
	m_realpath.reserve(256);
	m_collisionmove_time = 0;
	m_vcurdir = D3DXVECTOR2(0.f , 0.f);
	m_arrivalrange = 256;
	m_vprepos = m_vPos;
	m_curterrain_pathidx = 0;
	m_target_oldidx = -1;
	m_realpathidx = 0;

	m_pathfind_pause = false;
	m_multithread = false;
	m_collision_move = false;
	m_stoptime = 0.f;

	m_igoalidx = 0;
	m_fchase_time = 0.3f;

	m_stoptime_offset = float(rand()%3 + 1)/5;

	m_refindcnt = 0;

	m_bmagicbox = false;
}
void CCom_Pathfind::Target_chase(void)
{

	//m_pTarget = ptarget;
	if(NULL != m_pTarget)
	{
		m_fchase_time += GETTIME;

		if(m_fchase_time > 0.3f)
		{
			m_fchase_time = 0.f;
			if(false == m_multithread && false == m_pathfind_pause)
			{
				//몇초마다 한번씩 길찾기 하기로 하자
				//적을 추적한다
				int target_curidx = CMyMath::Pos_to_index(m_pTarget->GetPos() , 16);
				if(target_curidx != m_target_oldidx)
				{
					m_target_oldidx = target_curidx;
					ClearPath();
					//공격 대상도 탐색에 제외
					m_Astar->UnitPath_calculation_Start(m_vPos , m_pTarget->GetPos() , 32 ,  m_pTarget , true);//스텝사이즈 주의
					m_multithread = true;
				}
			}
		}
	}
	else
	{
		m_target_oldidx = -1;
		m_fchase_time = 0.f;
		m_pathfind_pause = true;
		if( ORDER_MOVE_ATTACK == m_pobj->GetUnitinfo().eorder && 
			m_terrainpath.empty() )
		{
			//명령이 어택무브 중이었다면 다시 지형을 찾는다.
			StartPathfinding(false);
		}
		else if(ORDER_NONE == m_pobj->GetUnitinfo().eorder)
		{
			if(NULL != m_com_collision)
			{
				if(false == ((CCom_Collision*)m_com_collision)->GetCollision())
				{
					if(m_realpath.empty())
					{
						m_multithread = false;
						//((CCom_Animation*)m_com_animation)->SetAnimation(L"IDLE");
						m_pobj->SetState(IDLE);
					}
				}
			}
		}
	}
}
void CCom_Pathfind::Update(void)
{
	if(COLLISION == m_pobj->GetUnitinfo().estate ||
		TRANSFORMING == m_pobj->GetUnitinfo().estate)
		return;

	m_pTarget = CObjMgr::GetInstance()->obj_alivecheck(m_target_objid);

	if(NULL == m_pTarget)
		m_target_objid = 0;

	Target_chase();

	if(true == m_multithread)
	{
		m_Astar->UnitPath_calculation_Update(m_obj_vertex , m_realpath);

		if(!m_realpath.empty())
		{
			m_multithread = false;
			m_realpathidx = m_realpath.size() - 1;
		}
	}

	UnitMoving_update();
}


void CCom_Pathfind::UnitMoving_update()
{

	if( m_realpath.empty() )
		return;

	if(!m_terrainpath.empty())
	{
		if(0 == m_realpathidx)
		{
			m_realpath.clear();

			if(CMyMath::pos_distance(m_vPos , m_terrainpath[m_curterrain_pathidx]) < m_arrivalrange*m_arrivalrange)
			{		
				++m_curterrain_pathidx;

				if((unsigned int)m_curterrain_pathidx >= m_terrainpath.size())
				{
					//도착점에 도착 했을때
					m_pobj->SetOrder(ORDER_NONE);
					m_pobj->SetState(IDLE);
					//((CCom_Animation*)m_com_animation)->SetAnimation(L"IDLE");

					m_terrainpath.clear();
					m_multithread = false;
					return;
				}

				bool weightG = false;
				if(m_terrainpath.size() - 1 != m_curterrain_pathidx) //terrainidx가 끝이 아니라면
					weightG = true;
				else
					weightG = false;

				m_refindcnt = 0;
				m_Astar->UnitPath_calculation_Start(m_vPos , m_terrainpath[m_curterrain_pathidx] , m_stepsize  , m_pTarget, weightG);
			}
			else
			{
				//일정횟수 연속으로 여기에 걸리면 인덱스를 다음으로 넘기는방법도 괜찮을듯

				//++m_refindcnt;

				//if(m_refindcnt > 5)
				//{
				//	if(m_curterrain_pathidx < int(m_terrainpath.size() - 1) )
				//		++m_curterrain_pathidx;

				//	m_refindcnt = 0;
				//	printf("ㅋㅌㅊㅍ\n");
				//}
				m_Astar->UnitPath_calculation_Start(m_vPos , m_terrainpath[m_curterrain_pathidx] , 32  , m_pTarget, false);
			}

			m_multithread = true;
			return;
		}
		else
		{
			if(m_curterrain_pathidx != m_terrainpath.size() - 1 &&
				CMyMath::pos_distance(m_vPos , m_terrainpath[m_curterrain_pathidx]) < 32*32)
			{
				m_realpath.clear();
				++m_curterrain_pathidx;
				m_Astar->UnitPath_calculation_Start(m_vPos , m_terrainpath[m_curterrain_pathidx] , m_stepsize ,m_pTarget, true);
				m_refindcnt = 0;
				m_multithread = true;
				return;
			}
		}
	}
	else
	{
		if(0 == m_realpathidx)
		{
			m_pobj->SetState(IDLE);
			m_realpath.clear();

			if(NULL != m_pTarget)
			{			
				m_terrainpath.clear();
				m_Astar->UnitPath_calculation_Start(m_vPos , m_pTarget->GetPos() , 32 , m_pTarget, true);
				m_multithread = true;
			}
			return;
		}
	}



	m_vcurdir = m_realpath[m_realpathidx - 1] - m_vPos;

	D3DXVec2Normalize(&m_vcurdir , &m_vcurdir);
	m_pobj->Setdir(m_vcurdir);

	m_vprepos = m_vPos + m_vcurdir*GETTIME*(*m_fspeed);

	m_prerect.left = m_vprepos.x - m_obj_vertex.left;
	m_prerect.right = m_vprepos.x + m_obj_vertex.right;
	m_prerect.top = m_vprepos.y - m_obj_vertex.top;
	m_prerect.bottom = m_vprepos.y + m_obj_vertex.bottom;



	if(CArea_Mgr::GetInstance()->overlap_prevention(m_prerect ,m_rect, m_vprepos, m_vPos , m_pobj ))
	{
		m_collisionmove_time += GETTIME;

		m_pobj->SetState(IDLE);

		m_collision_move = true;
	}
	else
	{
		if(true == m_collision_move)
		{			
			m_stoptime += GETTIME;

			if(m_stoptime >=  0.25f)
			{
				//m_stopcnt += 1;
				m_stoptime = 0.f;
				m_collision_move = false;
			}
			m_pobj->SetState(IDLE);
		}
		else
		{
			m_collisionmove_time -= GETTIME;
			if(m_collisionmove_time <= 0)
			{
				m_collisionmove_time = 0.f;
			}

			m_stoptime = 0.f;
			//어택땅 할때마다 여기가 걸리네..
			m_vPos = m_vprepos;
			m_rect = m_prerect;
			m_pobj->SetState(MOVE);
		}
	}



	if(m_collisionmove_time >= 0.4f)
	{
		m_collision_move = false;
		m_collisionmove_time = 0.f;
		m_realpath.clear();

		if(NULL == m_pTarget)
		{
			if(!m_terrainpath.empty())
				m_Astar->UnitPath_calculation_Start(m_vPos , m_terrainpath[m_curterrain_pathidx] , m_stepsize ,m_pTarget, false);
		}
		else
		{
			m_terrainpath.clear();
			m_Astar->UnitPath_calculation_Start(m_vPos , m_pTarget->GetPos() , 32 ,m_pTarget, false);
		}
		m_refindcnt = 0;

		m_multithread = true;
		return;
	}


	if( int(CMyMath::pos_distance( m_vPos ,m_realpath[m_realpathidx - 1]) <= 1) )
	{
		m_vPos = m_realpath[m_realpathidx - 1];
		--m_realpathidx;
	}
}

void CCom_Pathfind::ClearPath(void)
{
	if(!m_terrainpath.empty())
		m_terrainpath.clear();
	if(!m_realpath.empty())
		m_realpath.clear();
}

void CCom_Pathfind::StartPathfinding(bool bmagicbox)
{
	//printf("make_flowfiledpath 호출\n");

	m_refindcnt = 0;

	if(NULL != m_com_target)
	{
		((CCom_Targetsearch*)m_com_target)->SetSearch(true);	
		((CCom_Targetsearch*)m_com_target)->SetSearchtime(0.f);
	}

	m_fchase_time = 0.f;
	m_pathfind_pause = true;
	m_realpathidx = 0;
	m_curterrain_pathidx = 0;

	if( !m_terrainpath.empty() )
		m_terrainpath.clear();

	if( !m_realpath.empty() )
		m_realpath.clear();


	int tempidx = m_pobj->Getcuridx(32);
	int preidx = m_pobj->Getcuridx(32);

	m_arrivalrange = 192;
	int icurstepcnt = 0;
	const int	istep = 9;
	D3DXVECTOR2 temppos;
	int	loopcnt = 0;

	D3DXVECTOR2 vgap;
	if(true == bmagicbox)
	{
		vgap = m_pobj->GetPos() - CUnitMgr::GetInstance()->GetUnitCentterPt();
	}
	else
	{
		vgap = D3DXVECTOR2(0 , 0);
	}

	int	gapidx = 0;
	while(true)
	{
		//일일히 담는것이아니라 n스텝당 한번씩 담자

		loopcnt = 0;

		temppos = CMyMath::index_to_Pos( m_flowfieldpath[tempidx]  , SQ_TILECNTX , SQ_TILESIZEX);

		//장애물 발견하면 중단
		if(MOVE_NONE == CTileManager::GetInstance()->GetTileOption(tempidx))
		{
			m_goalpos = CMyMath::index_to_Pos( preidx , SQ_TILECNTX , SQ_TILESIZEX);

			gapidx = CMyMath::Pos_to_index( m_goalpos + vgap , 32);
			if(MOVE_NONE == CTileManager::GetInstance()->GetTileOption(gapidx))
				m_terrainpath.push_back( m_goalpos );
			else
				m_terrainpath.push_back( m_goalpos + vgap);


			break;
		}

		if(tempidx == m_igoalidx)
		{
			gapidx = CMyMath::Pos_to_index( m_goalpos + vgap , 32);
			if(MOVE_NONE == CTileManager::GetInstance()->GetTileOption(gapidx))
				m_terrainpath.push_back( m_goalpos );
			else
				m_terrainpath.push_back( m_goalpos + vgap);

			break;
		}

		if( 0 != icurstepcnt &&
			0 == icurstepcnt % istep )
		{
			//temppos가 다른유닛과 충돌하는지 본다.
			gapidx = CMyMath::Pos_to_index( temppos + vgap , 32);
			if(MOVE_NONE == CTileManager::GetInstance()->GetTileOption(gapidx))
				m_terrainpath.push_back( temppos );
			else
				m_terrainpath.push_back( temppos + vgap);

			

		}
		++icurstepcnt;

		preidx = tempidx;
		tempidx = m_flowfieldpath[tempidx];

	}

	m_curterrain_pathidx = 0;

	m_Astar->UnitPath_calculation_Start(m_vPos , m_terrainpath[m_curterrain_pathidx] , m_stepsize ,m_pTarget, true);
	m_multithread = true;

}

void CCom_Pathfind::SetPathfindPause(bool bpause)
{
	m_pathfind_pause = bpause;
}

void CCom_Pathfind::SetGoalPos(const D3DXVECTOR2& vgoalpos)
{
	m_goalpos = vgoalpos;
}

void CCom_Pathfind::SetGoalidx(const int& idx)
{
	m_igoalidx = idx;
}
void CCom_Pathfind::Release(void)
{
	m_pobj = NULL;
	Safe_Delete(m_Astar);
	m_terrainpath.clear();
	m_realpath.clear();
}

void CCom_Pathfind::Render(void)
{
	const TEXINFO* ptex = CTextureMgr::GetInstance()->GetSingleTexture(L"DebugTile" , L"White");

	D3DXMATRIX	tempmat;
	D3DXMatrixIdentity(&tempmat);

	if(!m_terrainpath.empty())
	{
		for(size_t i = 0; i < m_terrainpath.size(); ++i)
		{
			tempmat._41 = m_terrainpath[i].x - CScrollMgr::m_fScrollX;
			tempmat._42 = m_terrainpath[i].y - CScrollMgr::m_fScrollY;

			CDevice::GetInstance()->GetSprite()->SetTransform(&tempmat);

			if(0 == i)
				CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(16,16,0) , NULL , D3DCOLOR_ARGB(255,255,0,255));
			else if( i == m_terrainpath.size() - 1)
				CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(16,16,0) , NULL , D3DCOLOR_ARGB(255,0,0,255));
			else
				CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(16,16,0) , NULL , D3DCOLOR_ARGB(255,0,255,0));
		}
	}



	ptex = CTextureMgr::GetInstance()->GetSingleTexture(L"DebugTile" , L"tile8x8");


	//if(!m_realpath.empty())
	//{
	//	for(size_t i = 0; i < m_realpath.size(); ++i)
	//	{
	//		tempmat._41 = m_realpath[i].x - CScrollMgr::m_fScrollX;
	//		tempmat._42 = m_realpath[i].y - CScrollMgr::m_fScrollY;

	//		CDevice::GetInstance()->GetSprite()->SetTransform(&tempmat);

	//		if(0 == i)
	//			CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(4,4,0) , NULL , D3DCOLOR_ARGB(255,255,0,255));
	//		else if( i == m_realpath.size() - 1)
	//			CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(4,4,0) , NULL , D3DCOLOR_ARGB(255,0,0,255));
	//		else
	//			CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(4,4,0) , NULL , D3DCOLOR_ARGB(255,0,255,0));
	//	}

	//	//tempmat._41 = m_terrain_path[0].x - CScrollMgr::m_fScrollX;
	//	//tempmat._42 = m_terrain_path[0].y - CScrollMgr::m_fScrollY;
	//	//CDevice::GetInstance()->GetSprite()->SetTransform(&tempmat);
	//	//CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(16,16,0) , NULL , D3DCOLOR_ARGB(255,0,255,0));
	//}
}

void CCom_Pathfind::SetFlowField(void)
{
	memcpy(m_flowfieldpath , CTileManager::GetInstance()->Get_flowfield_node() , sizeof(short)*16384);
}

void CCom_Pathfind::SetTargetObjID(const int& objid)
{
	m_target_objid = objid;
}
