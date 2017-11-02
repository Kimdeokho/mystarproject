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

CCom_Pathfind::CCom_Pathfind(D3DXVECTOR2& vpos , MYRECT<float>& rc, const int& mainstep , const int& substep)
:m_vPos(vpos) , m_rect(rc)
{
	m_pTarget = NULL;
	m_mainstep = mainstep;
	m_substep = substep;
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

	m_com_animation = (m_pobj->GetComponent(COM_ANIMATION));
	m_com_collision = (m_pobj->GetComponent(COM_COLLISION));

	m_target_objid = 0;
	m_obj_vertex = m_pobj->GetVertex();

	m_fspeed = &(m_pobj->GetUnitinfo().fspeed);


	m_terrainpath.reserve(256);
	m_realpath.reserve(256);
	m_collisionmove_time = 0;
	m_vcurdir = D3DXVECTOR2(0.f , 0.f);
	m_vgap = D3DXVECTOR2(0.f, 0.f);
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
	m_fchase_time = 0.f;


	m_refindcnt = 0;

	m_bmagicbox = false;
}
void CCom_Pathfind::Target_chase(void)
{
	if(NULL != m_pTarget)
	{
		m_fchase_time += GETTIME;

		if(m_fchase_time >= 0.2f)
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
					m_Astar->UnitPath_calculation_Start(m_vPos , m_pTarget->GetPos() , m_substep  , true);//스텝사이즈 주의
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
			StartPathfinding(m_bmagicbox);
		}
		else if(ORDER_NONE == m_pobj->GetUnitinfo().eorder)
		{
			//if(NULL != m_com_collision)
			//{
			//if(false == ((CCom_Collision*)m_com_collision)->GetCollision())
			//{
			if(m_realpath.empty())
			{
				m_multithread = false;
				m_pobj->SetState(IDLE);
			}
			//}
			//}
		}
	}
}
void CCom_Pathfind::Update(void)
{
	if(COLLISION == m_pobj->GetUnitinfo().estate ||
		TRANSFORMING == m_pobj->GetUnitinfo().estate ||
		BOARDING == m_pobj->GetUnitinfo().estate ||
		ORDER_BUNKER_BOARDING == m_pobj->GetUnitinfo().eorder)
		return;

	m_pTarget = CObjMgr::GetInstance()->obj_alivecheck(m_target_objid);

	if(NULL == m_pTarget)
		m_target_objid = 0;


	Target_chase();
	if(true == m_multithread)
	{
		m_Astar->UnitPath_calculation_Update(m_obj_vertex , m_realpath , m_pTarget);

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
					if(ORDER_MOVE_BUILD != m_pobj->GetUnitinfo().eorder &&
						ORDER_BUILDING != m_pobj->GetUnitinfo().eorder)
					{
						m_pobj->SetOrder(ORDER_NONE);
						m_pobj->SetState(IDLE);
					}

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
				m_Astar->UnitPath_calculation_Start(m_vPos , m_terrainpath[m_curterrain_pathidx] , m_mainstep  , weightG);
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
				m_Astar->UnitPath_calculation_Start(m_vPos , m_terrainpath[m_curterrain_pathidx] , m_substep  , false);
			}

			m_multithread = true;
			return;
		}
		else
		{
			if(m_curterrain_pathidx != m_terrainpath.size() - 1 &&
				CMyMath::pos_distance(m_vPos , m_terrainpath[m_curterrain_pathidx]) < 100*100)
			{
				m_realpath.clear();
				++m_curterrain_pathidx;
				m_Astar->UnitPath_calculation_Start(m_vPos , m_terrainpath[m_curterrain_pathidx] , m_mainstep , true);
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
				m_Astar->UnitPath_calculation_Start(m_vPos , m_pTarget->GetPos() , m_substep , true);
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
		m_collisionmove_time = 0.f;
		//m_collisionmove_time -= GETTIME;
		//if(m_collisionmove_time <= 0)
		//	m_collisionmove_time = 0;

		if(true == m_collision_move)
		{			
			m_stoptime += GETTIME;

			if(m_stoptime >=  0.15f)
			{
				m_stoptime = 0.f;
				m_collision_move = false;
			}
			m_pobj->SetState(IDLE);
		}
		else
		{
			m_stoptime = 0.f;
			//어택땅 할때마다 여기가 걸리네..

			if(true == ((CCom_Animation*)m_com_animation)->GetAttack_end())
			{
				m_vPos = m_vprepos;
				m_rect = m_prerect;
				m_pobj->SetState(MOVE);
			}
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
				m_Astar->UnitPath_calculation_Start(m_vPos , m_terrainpath[m_curterrain_pathidx] , m_mainstep , false);
		}
		else
		{
			m_terrainpath.clear();
			m_Astar->UnitPath_calculation_Start(m_vPos , m_pTarget->GetPos() , m_substep , false);
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

	m_bmagicbox = bmagicbox;
	m_refindcnt = 0;

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
	const int	istep = 6;
	D3DXVECTOR2 temppos;
	int	loopcnt = 0;

	int	gapidx = 0;
	int	oriidx = 0;
	BYTE byfloor = 0;
	while(true)
	{
		//일일히 담는것이아니라 n스텝당 한번씩 담자

		loopcnt = 0;

		temppos = CMyMath::index_to_Pos( m_flowfieldpath[tempidx]  , SQ_TILECNTX , SQ_TILESIZEX);

		
		if(MOVE_NONE == CTileManager::GetInstance()->GetTileOption(tempidx))
		{
			// 그지역이 장애물이라면
			temppos = CMyMath::index_to_Pos( preidx , SQ_TILECNTX , SQ_TILESIZEX);

			gapidx = CMyMath::Pos_to_index( temppos + m_vgap , 32);
			oriidx = CMyMath::Pos_to_index( temppos  , 32);

			if(CTileManager::GetInstance()->GetTileFloor(oriidx) != 
				CTileManager::GetInstance()->GetTileFloor(gapidx))
			{
				m_terrainpath.push_back( temppos );
			}
			else
			{
				if(MOVE_NONE == CTileManager::GetInstance()->GetTileOption(gapidx))
				{
					m_terrainpath.push_back( temppos );
				}
				else
				{
					m_terrainpath.push_back( temppos + m_vgap);
				}
			}
			break;
		}

		if(tempidx == m_igoalidx)
		{
			//최종지점 도착
			gapidx = CMyMath::Pos_to_index( m_goalpos + m_vgap , 32);
			oriidx = CMyMath::Pos_to_index( temppos  , 32);

			if(CTileManager::GetInstance()->GetTileFloor(oriidx) != 
				CTileManager::GetInstance()->GetTileFloor(gapidx))
			{
				m_terrainpath.push_back( m_goalpos );
			}
			else
			{
				if(MOVE_NONE == CTileManager::GetInstance()->GetTileOption(gapidx))
					m_terrainpath.push_back( m_goalpos );
				else
					m_terrainpath.push_back( m_goalpos + m_vgap);
			}
			break;
		}

		if( 0 != icurstepcnt &&
			0 == icurstepcnt % istep )
		{
			//temppos가 다른유닛과 충돌하는지 본다.
			gapidx = CMyMath::Pos_to_index( temppos + m_vgap , 32);
			oriidx = CMyMath::Pos_to_index( temppos  , 32);

			if(CTileManager::GetInstance()->GetTileFloor(oriidx) != 
				CTileManager::GetInstance()->GetTileFloor(gapidx))
			{
				m_terrainpath.push_back( temppos );
			}
			else
			{
				if(MOVE_NONE == CTileManager::GetInstance()->GetTileOption(gapidx))
					m_terrainpath.push_back( temppos );
				else
					m_terrainpath.push_back( temppos + m_vgap);
			}
		}
		++icurstepcnt;

		preidx = tempidx;
		tempidx = m_flowfieldpath[tempidx]; //다음 경로로 가는 인덱스를 준다

	}

	m_curterrain_pathidx = 0;

	m_Astar->UnitPath_calculation_Start(m_vPos , m_terrainpath[m_curterrain_pathidx] , m_mainstep , true);
	m_multithread = true;

}

void CCom_Pathfind::SetPathfindPause(bool bpause)
{
	m_pathfind_pause = bpause;
}

void CCom_Pathfind::SetGoalPos(const D3DXVECTOR2& vgoalpos)
{
	m_goalpos = vgoalpos;
	m_igoalidx = CMyMath::Pos_to_index(m_goalpos , 32);
	gap_initialize();
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
	//const TEXINFO* ptex = CTextureMgr::GetInstance()->GetSingleTexture(L"DebugTile" , L"White");

	//D3DXMATRIX	tempmat;
	//D3DXMatrixIdentity(&tempmat);

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



	//ptex = CTextureMgr::GetInstance()->GetSingleTexture(L"DebugTile" , L"tile8x8");


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
void CCom_Pathfind::gap_initialize(void)
{
	m_vgap = m_pobj->GetPos() - CUnitMgr::GetInstance()->GetUnitCentterPt();
}

