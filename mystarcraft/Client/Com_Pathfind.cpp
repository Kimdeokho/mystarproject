#include "StdAfx.h"
#include "Com_Pathfind.h"

#include "Obj.h"
#include "Astar.h"
#include "TileManager.h"
#include "MyMath.h"
#include "TimeMgr.h"
#include "Com_Targetsearch.h"
#include "Com_Animation.h"
#include "Area_Mgr.h"

#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "Device.h"

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



	m_com_target = (CCom_Targetsearch*)(m_pobj->GetComponent(COM_TARGET_SEARCH));
	m_com_animation = (CCom_Animation*)(m_pobj->GetComponent(COM_ANIMATION));

	m_objid = m_pobj->GetObjID();
	m_obj_vertex = m_pobj->GetVertex();
	m_fspeed = m_pobj->GetSpeed();

	m_stepsize = 32;

	
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
}

void CCom_Pathfind::Update(void)
{

	m_pTarget = m_com_target->GetTarget();
	if(NULL != m_pTarget)
	{
		if(false == m_multithread && false == m_pathfind_pause)
		{
			int curidx16 = CMyMath::Pos_to_index(m_pTarget->GetPos() , 16);
			if(m_target_oldidx !=  curidx16)
			{
				//적을 추적한다

				m_target_oldidx = curidx16;
				m_terrainpath.clear();
				m_realpath.clear();
				//공격 대상도 탐색에 제외
				m_Astar->UnitPath_calculation_Start(m_vPos , m_pTarget->GetPos() , m_stepsize , m_pobj->GetObjID() , true);
				m_multithread = true;
			}
		}
	}
	else
	{
		if( ORDER_MOVE_ATTACK == m_pobj->GetOrder() && 
			m_terrainpath.empty() )
		{
			//명령이 어택무브 중이었다면 다시 지형을 찾는다.
			StartPathfinding();
		}
	}

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

					m_com_animation->SetAnimation(L"IDLE");

					m_terrainpath.clear();
					m_multithread = false;
					return;
				}
			}

			bool weightG = false;
			if(m_terrainpath.size() - 1 != m_curterrain_pathidx) //terrainidx가 끝이 아니라면
				weightG = true;
			else
				weightG = false;

			m_Astar->UnitPath_calculation_Start(m_vPos , m_terrainpath[m_curterrain_pathidx] , m_stepsize , m_objid , weightG);

			m_multithread = true;		
		}
		else
		{
			if(m_curterrain_pathidx != m_terrainpath.size() - 1 &&
				CMyMath::pos_distance(m_vPos , m_terrainpath[m_curterrain_pathidx]) < m_arrivalrange*m_arrivalrange)
			{
				m_realpath.clear();
				++m_curterrain_pathidx;
				m_Astar->UnitPath_calculation_Start(m_vPos , m_terrainpath[m_curterrain_pathidx] , m_stepsize , m_objid , true);
				m_multithread = true;
				return;
			}
		}
	}

	if(0 == m_realpathidx)
		return;

	m_vcurdir = m_realpath[m_realpathidx - 1] - m_vPos;

	D3DXVec2Normalize(&m_vcurdir , &m_vcurdir);
	m_pobj->Setdir(m_vcurdir);

	m_vprepos = m_vPos + m_vcurdir*GETTIME*(*m_fspeed);

	m_prerect.left = m_vprepos.x - m_obj_vertex.left;
	m_prerect.right = m_vprepos.x + m_obj_vertex.right;
	m_prerect.top = m_vprepos.y - m_obj_vertex.top;
	m_prerect.bottom = m_vprepos.y + m_obj_vertex.bottom;



	if(CArea_Mgr::GetInstance()->overlap_prevention(m_prerect ,m_rect, m_vprepos, m_vPos, m_vcurdir , m_objid ))
	{
		m_collisionmove_time += GETTIME;

		m_com_animation->SetAnimation(L"IDLE");
		m_pobj->SetState(IDLE);

		m_collision_move = true;
	}
	else
	{
		if(true == m_collision_move)
		{
			m_stoptime += GETTIME;

			if(m_stoptime >= 0.15f)
			{
				m_stoptime = 0.f;
				m_collision_move = false;
			}
		}
		else
		{
			m_collisionmove_time = 0.f;
			m_vPos = m_vprepos;
			m_rect = m_prerect;
			m_com_animation->SetAnimation(L"MOVE");
			m_pobj->SetState(MOVE);
		}
	}



	if(m_collisionmove_time >= 0.1f)
	{
		m_collisionmove_time = 0.f;
		m_realpath.clear();

		if(NULL == m_pTarget)
			m_Astar->UnitPath_calculation_Start(m_vPos , m_terrainpath[m_curterrain_pathidx] , m_stepsize , m_objid , false);
		else
			m_Astar->UnitPath_calculation_Start(m_vPos , m_pTarget->GetPos() , m_stepsize , m_objid , false);

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

void CCom_Pathfind::StartPathfinding(void)
{
	printf("make_flowfiledpath 호출\n");

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
	while(true)
	{
		//일일히 담는것이아니라 n스텝당 한번씩 담자

		loopcnt = 0;

		temppos = CMyMath::index_to_Pos( m_flowfieldpath[tempidx]  , SQ_TILECNTX , SQ_TILESIZEX);

		//장애물 발견하면 중단
		if(MOVE_NONE == CTileManager::GetInstance()->GetTileOption(tempidx))
		{
			m_goalpos = CMyMath::index_to_Pos( preidx , SQ_TILECNTX , SQ_TILESIZEX);
			m_terrainpath.push_back(m_goalpos);

			break;
		}

		if(tempidx == m_igoalidx)
		{
			m_terrainpath.push_back( m_goalpos );
			break;
		}



		if( 0 != icurstepcnt &&
			0 == icurstepcnt % istep )
		{
			//temppos가 다른유닛과 충돌하는지 본다.
			m_terrainpath.push_back( temppos );

		}
		++icurstepcnt;

		preidx = tempidx;
		tempidx = m_flowfieldpath[tempidx];

	}

	m_curterrain_pathidx = 0;

	m_Astar->UnitPath_calculation_Start(m_vPos , m_terrainpath[m_curterrain_pathidx] , m_stepsize , m_objid , true);
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
}

void CCom_Pathfind::Render(void)
{
	const TEXINFO* ptex = CTextureMgr::GetInstance()->GetSingleTexture(L"DebugTile" , L"White");

	D3DXMATRIX	tempmat;
	D3DXMatrixIdentity(&tempmat);

	//if(!m_terrainpath.empty())
	//{
	//	for(size_t i = 0; i < m_terrainpath.size(); ++i)
	//	{
	//		tempmat._41 = m_terrainpath[i].x - CScrollMgr::m_fScrollX;
	//		tempmat._42 = m_terrainpath[i].y - CScrollMgr::m_fScrollY;

	//		CDevice::GetInstance()->GetSprite()->SetTransform(&tempmat);

	//		if(0 == i)
	//			CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(16,16,0) , NULL , D3DCOLOR_ARGB(255,255,0,255));
	//		else if( i == m_terrainpath.size() - 1)
	//			CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(16,16,0) , NULL , D3DCOLOR_ARGB(255,0,0,255));
	//		else
	//			CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(16,16,0) , NULL , D3DCOLOR_ARGB(255,0,255,0));
	//	}
	//}



	ptex = CTextureMgr::GetInstance()->GetSingleTexture(L"DebugTile" , L"tile8x8");


	if(!m_realpath.empty())
	{
		for(size_t i = 0; i < m_realpath.size(); ++i)
		{
			tempmat._41 = m_realpath[i].x - CScrollMgr::m_fScrollX;
			tempmat._42 = m_realpath[i].y - CScrollMgr::m_fScrollY;

			CDevice::GetInstance()->GetSprite()->SetTransform(&tempmat);

			if(0 == i)
				CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(4,4,0) , NULL , D3DCOLOR_ARGB(255,255,0,255));
			else if( i == m_realpath.size() - 1)
				CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(4,4,0) , NULL , D3DCOLOR_ARGB(255,0,0,255));
			else
				CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(4,4,0) , NULL , D3DCOLOR_ARGB(255,0,255,0));
		}

		//tempmat._41 = m_terrain_path[0].x - CScrollMgr::m_fScrollX;
		//tempmat._42 = m_terrain_path[0].y - CScrollMgr::m_fScrollY;
		//CDevice::GetInstance()->GetSprite()->SetTransform(&tempmat);
		//CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(16,16,0) , NULL , D3DCOLOR_ARGB(255,0,255,0));
	}
}

void CCom_Pathfind::SetFlowField(void)
{
	memcpy(m_flowfieldpath , CTileManager::GetInstance()->Get_flowfield_node() , sizeof(short)*16384);
}
