#include "StdAfx.h"
#include "Unit.h"

#include "TextureMgr.h"
#include "TileManager.h"
#include "TimeMgr.h"
#include "MyMath.h"
#include "MouseMgr.h"
#include "FontMgr.h"
#include "ScrollMgr.h"

#include "Astar.h"
#include "Area_Mgr.h"

#include "Device.h"
CUnit::CUnit(void)
{
	//m_fogtime = 0.f;
	//m_sightoffsw = false;
	//m_InitSight = false;
	//m_iSightrange = 0;
	//memset(m_fogsearch , 0 , sizeof(m_fogsearch));
	//m_diridx = 0;

	m_collision_target = NULL;
	m_attack_target = NULL;

	m_diridx = 0;
	m_search_time = 0.f;
	m_movetime = 0.f;

	m_igoalidx = 0;

	


	m_Astar = new CAstar;
	m_Astar->Initialize(this);


	m_multithread = false;

	m_terrainpath.reserve(256);
	m_dummypath.reserve(256);
	m_goalpath.reserve(256);
	m_realpath.reserve(256);
}

CUnit::~CUnit(void)
{
	Release();
}

void CUnit::Initialize(void)
{

}

void CUnit::Update(void)
{

}

void CUnit::Render(void)
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
}

void CUnit::Release(void)
{
	Safe_Delete(m_Astar);
	m_terrainpath.clear();
	FogRelease();
}
void CUnit::FogUpdate(void)
{
	//m_curidx32 = CMyMath::Pos_to_index(m_vPos.x , m_vPos.y);

	//
	//if(true == m_sightoffsw)
	//	m_fogtime += GETTIME;

	//if(m_fogtime >3.0f)
	//{
	//	if(!m_Sightoff_List.empty() )
	//	{
	//		list<int>::iterator iter = m_Sightoff_List.begin();
	//		list<int>::iterator iter_end = m_Sightoff_List.end();

	//		for( ; iter != iter_end; ++iter)
	//		{
	//			m_fogsearch[(*iter)] = false;
	//			CTileManager::GetInstance()->SightOffRender(*iter);
	//		}

	//		m_Sightoff_List.clear();
	//	}
	//	m_fogtime = 0.f;
	//	m_sightoffsw = false;
	//	CTileManager::GetInstance()->SightOnRender(m_curidx32 , m_iSightrange , m_Sightoff_List , m_fogsearch , m_eType);
	//}
	//
	//if(m_curidx32 != m_oldidx32)
	//{
	//	m_sightoffsw = true;		
	//	CTileManager::GetInstance()->SightOnRender(m_curidx32 , m_iSightrange , m_Sightoff_List , m_fogsearch , m_eType);
	//	m_oldidx32 = m_curidx32;
	//}	
}
void CUnit::FogRelease(void)
{
	//if(!m_Sightoff_List.empty())
	//{
	//	list<int>::iterator iter = m_Sightoff_List.begin();
	//	list<int>::iterator iter_end = m_Sightoff_List.end();

	//	for( ; iter != iter_end; ++iter)
	//		CTileManager::GetInstance()->SightOffRender(*iter);

	//	m_Sightoff_List.clear();
	//}
}
void CUnit::Collision_update(void)
{
	m_rect.left = m_vPos.x - m_vertex.left; 
	m_rect.right = m_vPos.x + m_vertex.right;
	m_rect.top = m_vPos.y - m_vertex.top;
	m_rect.bottom = m_vPos.y + m_vertex.bottom;


	//충돌여부만 검사하고 충돌한 타겟을 가져온다
	//타겟을 가져오면 그 타겟과 나자신을 밀어낸다
	//밀어낼동안엔 충돌여부를 검사하지 않는다
	//전부 다 밀어내면 충돌 타겟을 NULL로 만든다.
	//만약 일꾼이 자원을캐는 상태라면 이 검사를 하지 않는다.


	if(NULL == m_collision_target)
	{
		m_search_time += GETTIME;

		if(m_search_time > 0.2f )
		{
			if(CArea_Mgr::GetInstance()->Collision_check(m_curidx64 , this , m_collision_target))
			{
				/*길찾기 이동을 중지하라는 신호를 보내야할듯*/

				vtargetpos = m_collision_target->GetPos();
				vnormal = vtargetpos - m_vPos;

				if(vnormal.x == 0 && vnormal.y == 0)
				{
					//영벡터일때 임의의 방향벡터를 준다.
					vnormal = OFFSET_DIRVEC;
				}

				D3DXVec2Normalize(&vnormal , &vnormal);

				float       fdot , fdgree;			
				fdot = D3DXVec2Dot(&(-vnormal) , &OFFSET_DIRVEC);
				fdgree = CMyMath::scala_to_dgree(fdot);

				if(-vnormal.x < 0)
					fdgree = 360 - fdgree;

				m_diridx = int(fdgree/22.5);

				if(m_diridx > 8)
				{
					m_diridx = 16 - m_diridx;
					m_matWorld._11 = -1;
				}
				else
					m_matWorld._11 = 1;
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
			if(MOVE_OK == ptilelist[m_curidx32]->byOption)
				m_vPos -= GETTIME*32*vnormal;
		}
		else
			m_collision_target = NULL;

	}
}
void CUnit::Dir_calculation(void)
{
	D3DXVECTOR2 vmouse;
	D3DXVECTOR2 vnormal;
	float       fdot , fdgree;
	int			diridx = 0;
	vmouse = CMouseMgr::GetvMousePt();

	D3DXVec2Normalize(&vnormal , &(vmouse - m_vPos));
	fdot = D3DXVec2Dot(&vnormal , &OFFSET_DIRVEC);

	fdgree = CMyMath::scala_to_dgree(fdot);

	if(vnormal.x < 0)
		fdgree = 360 - fdgree;

	//if(vmouse.x <= m_vPos.x)
	//fdgree = 360 - fdgree;

	diridx = int(fdgree/22.5);

	if(diridx > 8)
	{
		diridx = 16 - diridx;
		m_matWorld._11 = -1;
	}
	else
		m_matWorld._11 = 1;

	//CFontMgr::GetInstance()->Setnumber_combine_Font(L"PosX %d" , (int)m_vPos.x , 100,120);
	//CFontMgr::GetInstance()->Setnumber_combine_Font(L"PosY %d" , (int)m_vPos.y , 100,140);
	//CFontMgr::GetInstance()->Setnumber_combine_Font(L"MouseX %d" , (int)vmouse.x , 100,160);
	//CFontMgr::GetInstance()->Setnumber_combine_Font(L"MouseY %d" , (int)vmouse.y , 100,180);
	//CFontMgr::GetInstance()->Setnumber_combine_Font(L"Dot %f" , fdot , 100,200);
	//CFontMgr::GetInstance()->Setnumber_combine_Font(L"Dgree %d" , (int)fdgree , 100,220);
	//CFontMgr::GetInstance()->Setnumber_combine_Font(L"DIR %d" , (int)diridx , 100,240);
}
void CUnit::PathFinder_Update(void)
{
}
void CUnit::Pathfind_start(void)
{
	/*지형 경로 구하기*/


	/*유닛 경로 구하기*/
	//m_vwaypointPos = (*m_terrain_path)[0];
	//m_Astar->Path_calculation(m_vPos , m_vwaypointPos , m_rect);
}

void CUnit::Setstate(const TCHAR* statekey)
{
	if(m_statename != statekey)/*새로 들어오는 상태가 기존들어오는 상태와 다른상태여야 한다.*/
	{
		m_statename = statekey;
		m_frame.fcurframe = 0;

		for(int i = 0; i < DIR_CNT; ++i)
		{
			/* [i]는 방향 , DRONE, MOVE의 사진집합 */
			m_unittexture[i] = CTextureMgr::GetInstance()->GetZUnitTexture(m_objname , m_statename , i);
		}
		m_frame.umax = m_unittexture[0]->size() - 1;
		m_frame.fframespeed = (float)m_frame.umax;
	}
}
void CUnit::Animation_update(void)
{
	if(m_frame.fcurframe >= m_frame.umax)
	{
		m_frame.fcurframe = 0;
	}
	m_frame.fcurframe += GETTIME*m_frame.fframespeed;	

	const vector<TEXINFO*>* vtemp = m_unittexture[m_diridx];
	//m_curtex = ((*vtemp)[int(m_frame.fcurframe)])->pTexture;

	if( (unsigned int)m_frame.fcurframe < m_frame.umax)
		m_curtex = ((*vtemp)[int(m_frame.fcurframe)])->pTexture;
}

void CUnit::Search_Update(void)
{
	if(m_attack_target == NULL)
		CArea_Mgr::GetInstance()->AutoSearch_target(this , m_attack_target , m_curidx512 , 800 , 512);


	//if(m_attack_target != NULL)
	//	CFontMgr::GetInstance()->SetFontInfo(L"타겟찾음" , m_matWorld._41 , m_matWorld._42);
}

short* CUnit::getflowfield(void)
{
	return m_flowfieldpath;
}

void CUnit::testmove(void)
{
	//	m_flowfieldpath[m_curidx32]

	//그냥 임시로 테스트하는거다


	

	if(true == m_multithread)
	{
		if(!m_terrainpath.empty())
		{
			if(CMyMath::Pos_to_index(m_curpos , 16) !=
				CMyMath::Pos_to_index(m_destpos , 16))
			{
				m_Astar->UnitPath_calculation_Update(m_curpos , m_destpos , m_rect , m_obj_id , m_goalpath ,false);
			}
		}

		if(!m_goalpath.empty())
			m_multithread = false;
	}


	UnitMove_update();

	//if( !m_terrainpath.empty() )
	//{
	//	if(m_punitpath->empty())
	//	{
	//		//길찾기를 한다
	//		
	//		m_Astar->UnitPath_calculation_Update(m_vPos , m_terrainpath[m_curterrain_pathidx] , m_rect , m_obj_id , false);

	//		m_unitpathidx = m_punitpath->size() - 1;
	//	}
	//	else
	//	{
	//		UnitMove_update();
	//	}
	//}
}
void CUnit::UnitMove_update(void)
{
	//유닛 이동..

	//끝에 도달하면 전부 비운다. m_punitpath->clear();

	if(m_realpath.empty())
		return;



	if(0 == m_realpathidx)
	{
		m_realpath.clear();

		if(CMyMath::Pos_to_index(m_vPos , 16) ==
			CMyMath::Pos_to_index( m_terrainpath[m_curterrain_pathidx] , 16) )
		{
			m_vPos = m_terrainpath[m_curterrain_pathidx];
			++m_curterrain_pathidx;
			if((unsigned int)m_curterrain_pathidx >= m_terrainpath.size())
			{
				m_terrainpath.clear();
				m_multithread = false;
				return;
			}
		}

		//////@@@@@@@@@@@@@@@@@@@@@@@@
		size_t maxloop;
		if(m_goalpath.empty())
		{
			//목적지를 구하지못했으면 다시 더미패스를 구한다.
			m_multithread = true;
			m_Astar->UnitPath_calculation_Start(m_vPos , m_terrainpath[m_curterrain_pathidx]);
			m_Astar->UnitPath_calculation_Update(m_vPos , m_terrainpath[m_curterrain_pathidx] , m_rect , m_obj_id ,m_dummypath, true);

			maxloop = m_dummypath.size();
			for(size_t i = 0; i < maxloop; ++i)
			{
				m_realpath.push_back(m_dummypath[i]);
			}
			
			m_curpos = m_dummypath[0];
			m_destpos = m_terrainpath[m_curterrain_pathidx];

			m_realpathidx = maxloop - 1;

			m_dummypath.clear();
			
			m_Astar->UnitPath_calculation_Start(m_curpos , m_destpos);
		}
		else
		{
			m_multithread = false;
			maxloop = m_goalpath.size();
			for(size_t i = 0; i < maxloop; ++i)
			{
				m_realpath.push_back(m_goalpath[i]);
			}
			

			m_realpathidx = maxloop - 1;

			m_goalpath.clear();
		}
		//////@@@@@@@@@@@@@@@@@@@@@@@@
	}

	D3DXVECTOR2 vstart = m_realpath[m_realpathidx];
	D3DXVECTOR2 vdest = m_realpath[m_realpathidx - 1];
	D3DXVECTOR2 vnormal = vdest - vstart;

	D3DXVec2Normalize(&vnormal , &vnormal);


	if(GETTIME >= 0.03f)
		return;

	m_vPos += vnormal*GETTIME*120;


	if(int(CMyMath::pos_distance( m_vPos , vdest)) <= 1)
	{
		m_vPos = vdest;
		--m_realpathidx;
	}
}
void CUnit::make_flowfieldpath(const int& goalidx)
{

	int tempidx = m_curidx32;
	int preidx = m_curidx32;
	m_igoalidx = goalidx;

	if( !m_terrainpath.empty() )
		m_terrainpath.clear();

	if( !m_realpath.empty() )
		m_realpath.clear();

	int icurstepcnt = 1;
	const int	istep = 25;
	D3DXVECTOR2 temppos;

	while(true)
	{
		//일일히 담는것이아니라 n스텝당 한번씩 담자

		//유닛 길찾다가 부딪친 다른유닛이 웨이포인트에 있을때 그냥 도착한셈 치고 다른 웨이포인트로 길찾기시도

		temppos = CMyMath::index_to_Pos( m_flowfieldpath[tempidx]  , SQ_TILECNTX , SQ_TILESIZEX);


		if(tempidx == m_igoalidx)
		{
			temppos = CMouseMgr::GetvMousePt();
			m_terrainpath.push_back( temppos );
			break;
		}

		//장애물 발견하면 중단
		if(MOVE_NONE == CTileManager::GetInstance()->GetTileOption(tempidx))
		{
			temppos = CMyMath::index_to_Pos( preidx , SQ_TILECNTX , SQ_TILESIZEX);
			m_terrainpath.push_back(temppos);
			break;
		}

		if( 0 == icurstepcnt % istep )
			m_terrainpath.push_back( temppos );

		++icurstepcnt;

		preidx = tempidx;
		tempidx = m_flowfieldpath[tempidx];

	}

	m_curterrain_pathidx = 0;

	m_Astar->UnitPath_calculation_Start(m_vPos , m_terrainpath[m_curterrain_pathidx]);
	m_Astar->UnitPath_calculation_Update(m_vPos , m_terrainpath[m_curterrain_pathidx] , m_rect , m_obj_id ,m_dummypath, true);

	size_t maxloop = m_dummypath.size();
	for(size_t i = 0; i < maxloop; ++i)
	{
		m_realpath.push_back(m_dummypath[i]);
	}
	

	m_curpos = m_dummypath[0];
	m_destpos = m_terrainpath[m_curterrain_pathidx];

	m_realpathidx = maxloop - 1;

	m_dummypath.clear();

	m_multithread = true;

	m_Astar->UnitPath_calculation_Start(m_curpos , m_destpos);
}