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
#include "Debug_Mgr.h"
#include "LineMgr.h"

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

	m_com_animation = ((CCom_Animation*)(m_pobj->GetComponent(COM_ANIMATION)));
	m_com_collision = (m_pobj->GetComponent(COM_COLLISION));

	m_target_objid = 0;
	m_obj_vertex = m_pobj->GetVertex();

	m_fspeed = &(m_pobj->GetUnitinfo().fspeed);


	m_terrainpath.reserve(256);
	m_realpath.reserve( 256 );

	m_collisionMove_time = 0;
	m_vcurdir = D3DXVECTOR2(0.f , 0.f);
	m_vgap = D3DXVECTOR2(0.f, 0.f);
	m_arrivalrange = 64;
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
	m_fchase_time = 1.0f;

	m_terrain_step = 8;

	m_bmagicbox = false;

	m_refind_time = 0.f;

	m_timeoffset = 1.5f;

	m_curobj_idx = &(m_pobj->Getcuridx(32));
	m_oldobj_idx = -1;
	m_oldcase = -1;

	m_stopTimeOffset = m_pobj->GetVertex().left * 0.0075f;	
}
void CCom_Pathfind::Target_chase(void)
{
	if(NULL != m_pTarget)
	{
		m_refind_time = 0.4f - GETTIME*4;
		m_fchase_time += GETTIME;

		if(m_fchase_time >= 0.8f )
		{
			m_fchase_time = 0.f;
			if( true == ((CCom_Animation*)m_com_animation)->GetAttack_end())
			{
				if(false == m_multithread && false == m_pathfind_pause )
				{
					if(ATTACK != m_pobj->GetUnitinfo().state)
					{
						int target_curidx = CMyMath::Pos_to_index(m_pTarget->GetPos() , m_substep);
						m_target_oldidx = target_curidx;

						if(!m_terrainpath.empty())
							m_terrainpath.clear();

						//���� ��� Ž���� ����
						D3DXVECTOR2 target_pos = m_pTarget->GetPos();

						m_Astar->UnitPath_calculation_Start(m_vPos , target_pos , m_substep, m_terrainpath , m_curterrain_pathidx);//���ܻ����� ����

						m_multithread = true;
					}
				}
			}
		}
	}
	else
	{
		m_target_oldidx = -1;
		m_fchase_time = 1.2f;
		m_pathfind_pause = true;
		if( ORDER_MOVE_ATTACK == m_pobj->GetUnitinfo().order && 
			m_terrainpath.empty() )
		{
			m_multithread = false;
			m_pobj->SetState(IDLE);			
			m_refind_time += GETTIME;

			//����� ���ù��� ���̾��ٸ� �ٽ� ������ ã�´�.
			if(m_refind_time >= 0.4f)
			{
				if(!m_realpath.empty())
					m_realpath.clear();

				m_refind_time = 0.f;
				if(true == ((CCom_Animation*)m_com_animation)->GetAttack_end())
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


	if( COLLISION == m_pobj->GetUnitinfo().state ||
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
		vector<D3DXVECTOR2> vtemp;
		m_Astar->UnitPath_calculation_Update(vtemp , m_pTarget);

		if(!vtemp.empty())
		{
			m_multithread = false;
			m_realpathidx = vtemp.size() - 1;

			float fdist = CMyMath::pos_distance(vtemp[m_realpathidx] , m_vPos);
			bool distCheck = false;
			for(int i = m_realpathidx; i > 0; --i)
			{

				fdist += CMyMath::pos_distance(vtemp[i-1] , vtemp[i]);

				if(fdist > 4*4)
				{
					distCheck = true;
					break;
				}
			}

			if(!distCheck)
			{
				m_pobj->SetState(IDLE);
				m_realpath.clear();
				m_multithread = true;

				if(!m_terrainpath.empty())
				{
					if((unsigned int)m_curterrain_pathidx >= m_terrainpath.size() - 1 &&
						CMyMath::pos_distance(m_vPos , m_terrainpath[m_curterrain_pathidx]) < m_arrivalrange*m_arrivalrange)
					{
						m_terrainpath.clear();
						if(ORDER_MOVE_BUILD != m_pobj->GetUnitinfo().order)
						{
							m_pobj->SetOrder(ORDER_NONE);
							m_pobj->SetState(IDLE);
						}

						m_multithread = false;
						m_Astar->Release_unit_closelist();
						m_Astar->Release_unit_openlist();
					}
					else
						m_Astar->UnitPath_calculation_Start(m_vPos , m_terrainpath[m_curterrain_pathidx] , m_mainstep, m_terrainpath , m_curterrain_pathidx);
				}
				else if( NULL != m_pTarget )
					m_Astar->UnitPath_calculation_Start(m_vPos , m_pTarget->GetPos() , m_substep, m_terrainpath , m_curterrain_pathidx);

			}
			else
			{
				if(!m_realpath.empty())
				{
					D3DXVECTOR2 vtemp1 , vtemp2;
					for(int i = m_realpathidx; i > 0; --i)
					{
						vtemp1 = vtemp[i] - m_vPos;
						vtemp2 = vtemp[i - 1] - vtemp[i];

						if(0.0f <= D3DXVec2Dot(&vtemp1 , &vtemp2)) //90������ �۴�
						{
							m_realpathidx = i + 1;
							break;
						}
					}
				}
				vtemp.swap(m_realpath);
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
			m_pobj->SetState(IDLE);
			m_realpath.clear();

			if( CMyMath::pos_distance(m_vPos , m_terrainpath[m_curterrain_pathidx]) < m_arrivalrange*m_arrivalrange &&
				!CTileManager::GetInstance()->Bresenham_Tilecheck(m_vPos , m_terrainpath[m_curterrain_pathidx]))
			{		
				m_arrivalrange = 64;
				++m_curterrain_pathidx;

				if((unsigned int)m_curterrain_pathidx >= m_terrainpath.size())
				{
					//�������� ���� ������

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
			else //�̵���θ� �� ���������� �������� �������� ��������
			{				
				if(m_curterrain_pathidx >= (int)m_terrainpath.size() - 1)
				{
					m_arrivalrange += 64;
					if(m_arrivalrange >= 270)
						m_arrivalrange = 270;
				}

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
				float frange = (float)m_terrain_step/1.5f*(float)SQ_TILESIZEX;

				if( m_curterrain_pathidx < (int)m_terrainpath.size() - 1 &&
					(CMyMath::pos_distance(m_vPos , m_terrainpath[m_curterrain_pathidx])) < frange*frange)
				{
					if(!CTileManager::GetInstance()->Bresenham_Tilecheck(m_vPos , m_terrainpath[m_curterrain_pathidx]))
					{
						m_arrivalrange = 64;
						//m_realpath.clear(); ����
						++m_curterrain_pathidx;

						m_Astar->UnitPath_calculation_Start(m_vPos , m_terrainpath[m_curterrain_pathidx] , m_mainstep, m_terrainpath , m_curterrain_pathidx);
						m_multithread = true;
						return;
					}
				}
			}
		}
	}
	else //������� ����
	{
		if(0 == m_realpathidx) 
		{
			m_pobj->SetState(IDLE);
			m_realpath.clear();

			if(NULL != m_pTarget)
			{			
				m_arrivalrange = 64;

				D3DXVECTOR2 target_pos = m_pTarget->GetPos();

				m_Astar->UnitPath_calculation_Start(m_vPos , target_pos , m_substep, m_terrainpath , m_curterrain_pathidx);
				m_multithread = true;
			}		
			return;
		}
	}

	float onestep = GETTIME*(*m_fspeed);	
	if( (CMyMath::pos_distance( m_vprepos ,m_realpath[m_realpathidx - 1]) <= onestep*onestep) )
	{
		--m_realpathidx;
		if( 0 == m_realpathidx)
			m_pobj->SetState(IDLE);

		m_vprepos = m_realpath[m_realpathidx];

		m_prerect.left = m_vprepos.x - m_obj_vertex.left;
		m_prerect.right = m_vprepos.x + m_obj_vertex.right;
		m_prerect.top = m_vprepos.y - m_obj_vertex.top;
		m_prerect.bottom = m_vprepos.y + m_obj_vertex.bottom;
	}
	else
	{
		m_vcurdir = m_realpath[m_realpathidx - 1] - m_vPos;
		D3DXVec2Normalize(&m_vcurdir , &m_vcurdir);
		m_pobj->Setdir(m_vcurdir);

		m_vprepos = m_vPos + m_vcurdir*onestep;

		m_prerect.left = m_vprepos.x - m_obj_vertex.left;
		m_prerect.right = m_vprepos.x + m_obj_vertex.right;
		m_prerect.top = m_vprepos.y - m_obj_vertex.top;
		m_prerect.bottom = m_vprepos.y + m_obj_vertex.bottom;
	}

	int icase = PATHFIND_OPTION::NONE;
	icase = CArea_Mgr::GetInstance()->overlap_prevention(m_prerect ,m_rect, m_vprepos, m_vPos , m_pobj , m_pTarget);

	if(icase != m_oldcase)
	{
		m_oldcase = icase;
		m_collisionMove_time -= GETTIME / 2;

		if(m_collisionMove_time < 0 )
			m_collisionMove_time = 0.f;
	}
	if( PATHFIND_OPTION::WAIT == icase)
	{		
		m_collisionMove_time += GETTIME;
		m_pobj->SetState(IDLE);
		m_is_stop = true;

		if(NULL != m_pTarget)
			m_timeoffset = 0.f;
		else
			m_timeoffset = 0.37f;
	}
	else if( PATHFIND_OPTION::IMMEDIATE_PATHFIND == icase)
	{
		//��ٷ� ��ã��
		m_collisionMove_time += GETTIME;
		m_pobj->SetState(IDLE);
		m_timeoffset = 0.2f;
	}
	else if( PATHFIND_OPTION::NONE == icase)
	{
		m_collisionMove_time = 0.f;

		if(true == m_is_stop)
		{			
			m_stoptime += GETTIME;

			if(m_stoptime >=  m_stopTimeOffset)
			{
				m_stoptime = 0.f;
				m_is_stop = false;
			}
			m_pobj->SetState(IDLE);
		}
		else
		{
			if(m_com_animation->GetAttack_end())
			{
				m_stoptime = 0.f;			
				m_vPos = m_vprepos;
				m_pobj->SetState(MOVE);								
			}
			return;
		}
	}

	if(m_collisionMove_time >= m_timeoffset)
	{
		m_is_stop = false;
		m_collisionMove_time = 0.f;
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
		//������ ��°��̾ƴ϶� n���ܴ� �ѹ��� ����

		loopcnt = 0;

		temppos = CMyMath::index_to_Pos( m_flowfieldpath[tempidx]  , SQ_TILECNTX , SQ_TILESIZEX);


		if(MOVE_NONE == CTileManager::GetInstance()->GetTileOption(tempidx))
		{
			// �������� ��ֹ��̶��
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
					m_terrainpath.push_back( temppos );
				else
					m_terrainpath.push_back( temppos + m_vgap);
			}
			break;
		}

		if(tempidx == m_igoalidx)
		{
			//�������� ����
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
		tempidx = m_flowfieldpath[tempidx]; //���� ��η� ���� �ε����� �ش�

	}

	m_curterrain_pathidx = 0;

	m_Astar->UnitPath_calculation_Start(m_vPos , m_terrainpath[m_curterrain_pathidx] , m_mainstep, m_terrainpath , m_curterrain_pathidx);
	m_multithread = true;
	m_arrivalrange = 64;
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
	if(!CDebug_Mgr::m_dbglist[CDebug_Mgr::DBG_PATHFIND])
		return;

	const TEXINFO* ptex = CTextureMgr::GetInstance()->GetSingleTexture(L"DebugTile" , L"White");

	D3DXMATRIX	tempmat;
	D3DXMatrixIdentity(&tempmat);


	ptex = CTextureMgr::GetInstance()->GetSingleTexture(L"DebugTile" , L"tile8x8");


	if(!m_realpath.empty())
	{
		for(size_t i = 0; i < m_realpath.size(); ++i)
		{
			tempmat._41 = m_realpath[i].x - CScrollMgr::m_fScrollX;
			tempmat._42 = m_realpath[i].y - CScrollMgr::m_fScrollY;

			CDevice::GetInstance()->GetSprite()->SetTransform(&tempmat);

			if( i < m_realpath.size() - 1 )
				CLineMgr::GetInstance()->PathLineRender(m_realpath[i] , m_realpath[i + 1] , 1.0f);
		}
	}

	if(!m_terrainpath.empty())
	{
		for(size_t i = 0; i < m_terrainpath.size(); ++i)
		{
			tempmat._41 = m_terrainpath[i].x - CScrollMgr::m_fScrollX;
			tempmat._42 = m_terrainpath[i].y - CScrollMgr::m_fScrollY;

			CDevice::GetInstance()->GetSprite()->SetTransform(&tempmat);

			CDevice::GetInstance()->GetSprite()->Draw( ptex->pTexture , NULL , &D3DXVECTOR3(16,16,0) , NULL , D3DCOLOR_ARGB(255,255,0,255));
		}
	}

	//m_Astar->Path_Render();
}

void CCom_Pathfind::gap_initialize(bool bmagicbox)
{
	if(true == bmagicbox)
		m_vgap = m_pobj->GetPos() - CUnitMgr::GetInstance()->GetUnitCentterPt();
	else
	{
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


const D3DXVECTOR2 CCom_Pathfind::GetGoalPos(void)
{
	return m_goalpos + m_vgap;
}


int CCom_Pathfind::GetTargetPath_size(void)
{
	if(NULL == m_pTarget)
		return 0;

	if(!m_realpath.empty())
	{
		return m_realpathidx;
	}
	else
		return 0;
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