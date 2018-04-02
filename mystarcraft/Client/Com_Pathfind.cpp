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

#include "MouseMgr.h"
#include "Spidermine.h"
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
	m_realpath.reserve( 256 );

	m_collisionmove_time = 0;
	m_vcurdir = D3DXVECTOR2(0.f , 0.f);
	m_vgap = D3DXVECTOR2(0.f, 0.f);
	m_arrivalrange = 350;
	m_vprepos = m_vPos;
	m_curterrain_pathidx = 0;
	m_target_oldidx = -1;
	m_realpathidx = 0;

	m_pathfind_pause = false;
	m_multithread = false;
	m_is_stop = false;
	m_is_end = false;
	m_stoptime = 0.f;

	m_igoalidx = 0;
	m_fchase_time = 0.f;

	m_terrain_step = 12;

	m_bmagicbox = false;

	m_refind_time = 0.f;

	m_timeoffset = 1.5f;

	m_curobj_idx = &(m_pobj->Getcuridx(32));
	m_oldobj_idx = -1;
	m_oldcase = -1;
}
void CCom_Pathfind::Target_chase(void)
{
	if(NULL != m_pTarget)
	{
		m_fchase_time += GETTIME;

		if(m_fchase_time >= 0.15f )
		{
			m_fchase_time = 0.f;
			if( true == ((CCom_Animation*)m_com_animation)->GetAttack_end())
			{
				if(false == m_multithread && false == m_pathfind_pause )
				{
					//몇초마다 한번씩 길찾기 하기로 하자
					//적을 추적한다

					if(ATTACK != m_pobj->GetUnitinfo().state)
					{
						int target_curidx = CMyMath::Pos_to_index(m_pTarget->GetPos() , 32);
						if(target_curidx != m_target_oldidx)
						{
							m_target_oldidx = target_curidx;
							ClearPath();
							//공격 대상도 탐색에 제외
							m_Astar->UnitPath_calculation_Start(m_vPos , m_pTarget->GetPos() , m_mainstep, m_terrainpath , m_curterrain_pathidx);//스텝사이즈 주의
							m_multithread = true;
						}
					}
				}
			}
		}
	}
	else
	{
		m_target_oldidx = -1;
		m_fchase_time = 0.f;
		m_pathfind_pause = true;
		if( ORDER_MOVE_ATTACK == m_pobj->GetUnitinfo().order && 
			m_terrainpath.empty() )
		{
			m_pobj->SetState(IDLE);
			m_multithread = false;
			m_refind_time += GETTIME;
			if(!m_realpath.empty())
				m_realpath.clear();

			//명령이 어택무브 중이었다면 다시 지형을 찾는다.
			if(m_refind_time >= 0.6f)
			{
				m_refind_time = 0.f;
				//if(true == ((CCom_Animation*)m_com_animation)->GetAttack_end())
				{
					m_vgap = D3DXVECTOR2(0,0);
					StartPathfinding();
				}
			}
		}
		else if(ORDER_NONE == m_pobj->GetUnitinfo().order)
		{
			if(m_realpath.empty())
			{
				m_multithread = false;
				if(BURROW != m_pobj->GetUnitinfo().state)
					m_pobj->SetState(IDLE);
			}
		}
	}
}
void CCom_Pathfind::Update(void)
{
	//D3DXVECTOR2 fontvpos = m_pobj->GetPos();
	//fontvpos.x -= CScrollMgr::m_fScrollX;
	//fontvpos.y -= CScrollMgr::m_fScrollY;
	//CFontMgr::GetInstance()->Setbatch_Font(L"%d" , m_arrivalrange , fontvpos.x , fontvpos.y);


	if(COLLISION == m_pobj->GetUnitinfo().state ||
		TRANSFORMING == m_pobj->GetUnitinfo().state ||
		false == m_pobj->GetUnitinfo().is_active ||
		ORDER_BUNKER_BOARDING == m_pobj->GetUnitinfo().order)
		return;

	m_pTarget = CObjMgr::GetInstance()->obj_alivecheck(m_target_objid);

	if(NULL == m_pTarget)
		m_target_objid = 0;


	Target_chase();

	if(true == m_multithread)
	{
		m_Astar->UnitPath_calculation_Update(m_realpath , m_pTarget);

		if(!m_realpath.empty())
		{
			m_multithread = false;
			m_realpathidx = m_realpath.size() - 1;
			if(m_realpathidx <= 0)
			{
				//m_realpath.clear();
				m_arrivalrange = 64;
			}
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

			if( CMyMath::pos_distance(m_vPos , m_terrainpath[m_curterrain_pathidx]) < m_arrivalrange*m_arrivalrange)
			{		
				m_arrivalrange = 128;
				++m_curterrain_pathidx;

				if((unsigned int)m_curterrain_pathidx >= m_terrainpath.size())
				{
					//도착점에 도착 했을때

					if(ORDER_MOVE_BUILD != m_pobj->GetUnitinfo().order)
					{
						m_pobj->SetOrder(ORDER_NONE);
						m_pobj->SetState(IDLE);
					}

					m_terrainpath.clear();
					m_multithread = false;
					m_Astar->Release_unit_closelist();
					m_Astar->Release_unit_openlist();
					return;
				}

				m_Astar->UnitPath_calculation_Start(m_vPos , m_terrainpath[m_curterrain_pathidx] , m_mainstep, m_terrainpath , m_curterrain_pathidx);
			}
			else
			{
				m_arrivalrange += 350;
				if(m_arrivalrange >= 350)
					m_arrivalrange = 350;

				//스텝을 다이동했지만 지정웨이포인트에 도착하지 못했을때
				m_Astar->UnitPath_calculation_Start(m_vPos , m_terrainpath[m_curterrain_pathidx] , m_mainstep, m_terrainpath , m_curterrain_pathidx);
			}

			m_multithread = true;
			return;
		}
		else
		{
			if(	*m_curobj_idx != m_oldobj_idx )
			{
				m_oldobj_idx = *m_curobj_idx;
				if( m_curterrain_pathidx < (int)m_terrainpath.size() - 1 &&
					CMyMath::pos_distance(m_vPos , m_terrainpath[m_curterrain_pathidx]) < 340*340)
				{
					if(CTileManager::GetInstance()->Bresenham_Tilecheck(m_vPos , m_terrainpath[m_curterrain_pathidx]))
					{
						m_realpath.clear();
						++m_curterrain_pathidx;

						m_Astar->UnitPath_calculation_Start(m_vPos , m_terrainpath[m_curterrain_pathidx] , m_mainstep, m_terrainpath , m_curterrain_pathidx);
						//m_refindcnt = 0;
						m_multithread = true;
						return;
					}
				}
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
				m_Astar->UnitPath_calculation_Start(m_vPos , m_pTarget->GetPos() , m_substep, m_terrainpath , m_curterrain_pathidx);
				m_multithread = true;
			}		
			return;
		}
	}

	m_vcurdir = m_realpath[m_realpathidx - 1] - m_vPos;

	D3DXVec2Normalize(&m_vcurdir , &m_vcurdir);
	m_pobj->Setdir(m_vcurdir);

	if( int(CMyMath::pos_distance( m_vprepos ,m_realpath[m_realpathidx - 1]) <= 2) )
	{		
		--m_realpathidx;
		m_vprepos = m_realpath[m_realpathidx];

		m_prerect.left = m_vprepos.x - m_obj_vertex.left;
		m_prerect.right = m_vprepos.x + m_obj_vertex.right;
		m_prerect.top = m_vprepos.y - m_obj_vertex.top;
		m_prerect.bottom = m_vprepos.y + m_obj_vertex.bottom;

	}
	else
	{
		m_vprepos = m_vPos + m_vcurdir*GETTIME*(*m_fspeed);

		m_prerect.left = m_vprepos.x - m_obj_vertex.left;
		m_prerect.right = m_vprepos.x + m_obj_vertex.right;
		m_prerect.top = m_vprepos.y - m_obj_vertex.top;
		m_prerect.bottom = m_vprepos.y + m_obj_vertex.bottom;
	}

	int icase = 0;
	icase = CArea_Mgr::GetInstance()->overlap_prevention(m_prerect ,m_rect, m_vprepos, m_vPos , m_pobj );

	if(icase != m_oldcase)
	{
		m_oldcase = icase;
		m_collisionmove_time = 0;
	}
	if( 1 == icase)
	{		
		m_collisionmove_time += GETTIME;

		m_pobj->SetState(IDLE);
		m_pobj->SetWait(false);

		m_is_stop = true;

		m_timeoffset = 2.5f;
	}
	else if( 2 == icase)
	{
		m_pobj->SetState(IDLE);
		m_pobj->SetWait(false);
		m_timeoffset = 0.f;
	}
	else if( 3 == icase)
	{
		m_collisionmove_time += GETTIME;
		m_pobj->SetState(IDLE);
		m_timeoffset = 5.f;
		m_is_stop = true;
	}
	else if( 4 == icase)
	{
		m_collisionmove_time += GETTIME;
		m_pobj->SetState(IDLE);
		m_timeoffset = 0.3f;
		m_is_stop = true;
	}
	else if( 0 == icase)
	{
		m_pobj->SetWait(false);
		m_collisionmove_time = 0.f;

		if(true == m_is_stop)
		{			
			m_stoptime += GETTIME;

			if(m_stoptime >=  0.1f)
			{
				m_stoptime = 0.f;
				m_is_stop = false;
			}
			m_pobj->SetState(IDLE);
		}
		else
		{
			m_stoptime = 0.f;
			
			m_vPos = m_vprepos;
			//m_rect = m_prerect;
			m_pobj->SetState(MOVE);								
			return;
		}
	}

	if(m_collisionmove_time >= m_timeoffset)
	{
		m_pobj->SetWait(false);
		m_is_stop = false;
		m_collisionmove_time = 0.f;
		m_stoptime = 0.f;
		m_realpath.clear();

		if(NULL == m_pTarget)
		{
			if(!m_terrainpath.empty())
			{
				m_Astar->UnitPath_calculation_Start(m_vPos , m_terrainpath[m_curterrain_pathidx] , m_mainstep, m_terrainpath , m_curterrain_pathidx);
				m_multithread = true;
			}
		}
		else
		{
			m_terrainpath.clear();
			m_Astar->UnitPath_calculation_Start(m_vPos , m_pTarget->GetPos() , m_substep, m_terrainpath , m_curterrain_pathidx);
			m_multithread = true;
		}
	}

}

void CCom_Pathfind::StartPathfinding(void)
{
	//m_refindcnt = 0;

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

	int icurstepcnt = 0;
	//const int	istep = 15;
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
			0 == icurstepcnt % m_terrain_step )
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

	if(m_curterrain_pathidx == m_terrainpath.size() - 1)
		m_is_end = true;
	else
		m_is_end = false;

	m_Astar->UnitPath_calculation_Start(m_vPos , m_terrainpath[m_curterrain_pathidx] , m_mainstep, m_terrainpath , m_curterrain_pathidx);
	m_multithread = true;
}
bool CCom_Pathfind::arrive_check(const int& terrain_step)
{
	int tempidx = m_pobj->Getcuridx(32);

	int	loopcnt = 0;

	while(true)
	{
		++loopcnt;

		if(tempidx == m_igoalidx)
			break;
		tempidx = m_flowfieldpath[tempidx]; //다음 경로로 가는 인덱스를 준다

	}

	int stepidx = (int(m_terrainpath.size()) - (loopcnt / terrain_step) );

	if(stepidx < 0)
		stepidx = 0;

	if( stepidx < m_curterrain_pathidx )
	{
		return false;
	}
	else
		return true;
}

void CCom_Pathfind::Setrally_path(const vector<D3DXVECTOR2>& vrally_path)
{
	if(!m_terrainpath.empty())
		m_terrainpath.clear();

	m_terrainpath.assign(vrally_path.begin() , vrally_path.end());
	m_curterrain_pathidx = 0;
	m_Astar->UnitPath_calculation_Start(m_vPos , m_terrainpath[m_curterrain_pathidx] , m_mainstep, m_terrainpath , m_curterrain_pathidx);
	m_multithread = true;
}
void CCom_Pathfind::SetPathfindPause(bool bpause)
{
	m_pathfind_pause = bpause;
}
void CCom_Pathfind::SetMultithread(bool bmulti)
{
	m_multithread = bmulti;
}

void CCom_Pathfind::SetGoalPos(const D3DXVECTOR2& vgoalpos , bool bmagicbox)
{
	//m_bmagicbox = bmagicbox;
	m_goalpos = vgoalpos;
	m_igoalidx = CMyMath::Pos_to_index(m_goalpos , 32);
	gap_initialize( bmagicbox );
}
void CCom_Pathfind::SetFlowField(void)
{
	memcpy(m_flowfieldpath , CTileManager::GetInstance()->Get_flowfield_node() , sizeof(short)*16384);
}

void CCom_Pathfind::SetTargetObjID(const int& objid)
{
	m_target_objid = objid;
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

	//m_Astar->Path_Render();
}

void CCom_Pathfind::gap_initialize(bool bmagicbox)
{
	if(true == bmagicbox)
		m_vgap = m_pobj->GetPos() - CUnitMgr::GetInstance()->GetUnitCentterPt();
	else
	{
		//m_vgap = (m_pobj->GetPos() - CUnitMgr::GetInstance()->GetUnitCentterPt() );//D3DXVECTOR2(0.f, 0.f);
		m_vgap = D3DXVECTOR2(0.f, 0.f);
	}
}

void CCom_Pathfind::ClearPath(void)
{
	if(!m_terrainpath.empty())
		m_terrainpath.clear();
	if(!m_realpath.empty())
		m_realpath.clear();
}
void CCom_Pathfind::Release(void)
{
	m_pobj = NULL;
	Safe_Delete(m_Astar);
	m_terrainpath.clear();
	m_realpath.clear();

	vector<D3DXVECTOR2>().swap( m_realpath );
	vector<D3DXVECTOR2>().swap( m_terrainpath );
}

const D3DXVECTOR2 CCom_Pathfind::GetGoalPos(void)
{
	//return m_goalpos + m_pobj->GetPos() - CUnitMgr::GetInstance()->GetUnitCentterPt();
	return m_goalpos + m_vgap;
}



