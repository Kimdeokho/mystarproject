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

#include "UI_MiniUnitDisplay.h"
#include "ComanderMgr.h"

CUnit::CUnit(void)
{
	//m_fogtime = 0.f;
	//m_sightoffsw = false;
	//m_InitSight = false;
	//m_iSightrange = 0;
	//memset(m_fogsearch , 0 , sizeof(m_fogsearch));
	//m_diridx = 0;



	


	//m_Astar = new CAstar;
	//m_Astar->Initialize(this);

	//m_igoalidx = 0;	
	//m_multithread = false;

	//m_terrainpath.reserve(256);
	//m_realpath.reserve(256);

	//m_collisionmove_time = 0;

	//m_vcurdir = D3DXVECTOR2(0.f , 0.f);

	//m_arrivalrange = 256;
	//m_vprepos = m_vPos;

	//m_curterrain_pathidx = 0;

	//m_target_oldidx = -1;

	//m_pathfind_pause = false;

	//m_com_multitex		= NULL;
	//m_com_generaltex	= NULL;
	//m_com_singletex		= NULL;

	m_select_ui = NULL;
	m_miniunit_display = NULL;
}

CUnit::~CUnit(void)
{
	Release();

}

void CUnit::Initialize(void)
{
	m_miniunit_display = new CUI_MiniUnitDisplay(m_vPos);
	m_miniunit_display->Initialize();
	CComanderMgr::GetInstance()->SetMiniUnit_display(m_miniunit_display);
}

void CUnit::Update(void)
{

}

void CUnit::Render(void)
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

void CUnit::Release(void)
{
	//Safe_Delete(m_Astar);

	if(NULL != m_miniunit_display)
		m_miniunit_display->SetDestroy(true);

	if(NULL != m_select_ui)
		m_select_ui->SetDestroy(true);
}
void CUnit::Collision_update(void)
{
	//m_rect.left = m_vPos.x - m_vertex.left; 
	//m_rect.right = m_vPos.x + m_vertex.right;
	//m_rect.top = m_vPos.y - m_vertex.top;
	//m_rect.bottom = m_vPos.y + m_vertex.bottom;


	//////충돌여부만 검사하고 충돌한 타겟을 가져온다
	//////타겟을 가져오면 그 타겟과 나자신을 밀어낸다
	//////밀어낼동안엔 충돌여부를 검사하지 않는다
	//////전부 다 밀어내면 충돌 타겟을 NULL로 만든다.
	//////만약 일꾼이 자원을캐는 상태라면 이 검사를 하지 않는다.


	//if(NULL == m_collision_target)
	//{
	//	m_search_time += GETTIME;

	//	if(m_search_time >= 0.2f )
	//	{
	//		if(CArea_Mgr::GetInstance()->Collision_check(m_curidx64 , this , m_collision_target))
	//		{
	//			printf("충돌! %d \n" , m_obj_id);
	//			/*길찾기 이동을 중지하라는 신호를 보내야할듯*/

	//			vtargetpos = m_collision_target->GetPos();
	//			m_collision_vnormal = vtargetpos - m_vPos;

	//			D3DXVec2Normalize(&m_collision_vnormal , &m_collision_vnormal);
	//			if( int(m_collision_vnormal.x) == 0 && int(m_collision_vnormal.y == 0) )
	//			{
	//				//영벡터일때 임의의 방향벡터를 준다.
	//				m_collision_vnormal = OFFSET_DIRVEC;
	//			}
	//		}

	//		m_search_time = 0.f;
	//	}

	//}
	//else
	//{
	//	MYRECT<float> temp;
	//	if( MyIntersectrect(&temp , &m_rect , &m_collision_target->GetMyRect()) )
	//	{
	//		//임의
	//		TILE** ptilelist = CTileManager::GetInstance()->GetSqTile();


	//		/*타겟과 충돌이 일어나는 중이라면*/
	//		if(MOVE_OK == ptilelist[m_curidx32]->byOption)
	//		{
	//			//Dir_calculation(-m_collision_vnormal);
	//			m_vPos -= GETTIME*m_fspeed*m_collision_vnormal;

	//			m_estate = MOVE;
	//			Set_texture_statename(L"MOVE");
	//		}
	//	}
	//	else
	//	{
	//		m_collision_target = NULL;
	//		//현재 받고있는 명령에따라 상태를 해준다.
	//		m_estate = IDLE;
	//		//Set_texture_statename(L"IDLE");
	//	}

	//}
}
void CUnit::Dir_calculation(D3DXVECTOR2& vnomal)
{
	//float       fdot , fdgree;			

	//D3DXVec2Normalize(&vnomal , &vnomal);
	//fdot = D3DXVec2Dot(&vnomal , &OFFSET_DIRVEC);

	//fdgree = CMyMath::scala_to_dgree(fdot);

	//if(vnomal.x < 0)
	//	fdgree = 360 - fdgree;

	//m_diridx = (int)( (fdgree/22.5f) + 0.5f);


	//if(m_diridx > 8)
	//{
	//	m_diridx = 16 - m_diridx;
	//	m_matWorld._11 = -1;
	//}
	//else
	//	m_matWorld._11 = 1;


	//CFontMgr::GetInstance()->Setbatch_Font(L"%d" , m_diridx , m_matWorld._41 , m_matWorld._42 );

	//CFontMgr::GetInstance()->Setnumber_combine_Font(L"PosX %d" , (int)m_vPos.x , 100,120);
	//CFontMgr::GetInstance()->Setnumber_combine_Font(L"PosY %d" , (int)m_vPos.y , 100,140);
	//CFontMgr::GetInstance()->Setnumber_combine_Font(L"MouseX %d" , (int)vmouse.x , 100,160);
	//CFontMgr::GetInstance()->Setnumber_combine_Font(L"MouseY %d" , (int)vmouse.y , 100,180);
	//CFontMgr::GetInstance()->Setnumber_combine_Font(L"Dot %f" , fdot , 100,200);
	//CFontMgr::GetInstance()->Setnumber_combine_Font(L"Dgree %d" , (int)fdgree , 100,220);
	//CFontMgr::GetInstance()->Setnumber_combine_Font(L"DIR %d" , (int)diridx , 100,240);
}
void CUnit::Pathfind_start(void)
{
	/*지형 경로 구하기*/


	/*유닛 경로 구하기*/
	//m_vwaypointPos = (*m_terrain_path)[0];
	//m_Astar->Path_calculation(m_vPos , m_vwaypointPos , m_rect);
}

void CUnit::Set_texture_statename(const TCHAR* statename)
{
	//if(m_statename != statename)/*새로 들어오는 상태가 기존들어오는 상태와 다른상태여야 한다.*/
	//{
	//	m_statename = statename;

	//	//m_statename = statekey;
	//	m_frame.fcurframe = 0;

	//	for(int i = 0; i < DIR_CNT; ++i)
	//	{
	//		/* [i]는 방향 , DRONE, MOVE의 사진집합 */
	//		m_unittexture[i] = CTextureMgr::GetInstance()->GetZUnitTexture(m_objname , m_statename , i);
	//		if(NULL == m_unittexture[i])
	//			break;
	//	}

	//	if(NULL != m_unittexture[0])
	//	{
	//		m_frame.umax = m_unittexture[0]->size() - 1;
	//		m_frame.fframespeed = (float)m_frame.umax;
	//	}
	//}
}
void CUnit::Animation_update(void)
{
	//if(m_frame.fcurframe >= m_frame.umax)
	//{
	//	m_frame.fcurframe = 0;
	//}
	//m_frame.fcurframe += GETTIME*m_frame.fframespeed;	

	//const vector<TEXINFO*>* vtemp = m_unittexture[m_diridx];

	//if( (unsigned int)m_frame.fcurframe <= m_frame.umax)
	//	m_curtex = ((*vtemp)[int(m_frame.fcurframe)])->pTexture;
}

void CUnit::Search_Update(void)
{
	//원거리 유닛 AI로 하자	

	//if(ORDER_MOVE != m_eorder)
	//	m_attack_target = CArea_Mgr::GetInstance()->AutoSearch_target(this , m_curidx256 , 250 , 256, SEARCH_ONLY_ENEMY);
	//else
	//	m_attack_target = NULL;

	//if(NULL != m_attack_target)
	//{
	//	if(CMyMath::pos_distance(m_attack_target->GetPos() , m_vPos) < 32*32)
	//	{
	//		//공격
	//		if(!m_terrainpath.empty())
	//			m_terrainpath.clear();
	//		if(!m_realpath.empty())
	//			m_realpath.clear();

	//		m_estate = ATTACK;
	//		m_pathfind_pause = true;
	//	}
	//	else
	//	{
	//		//공격 후딜이 다 끝나고 추적
	//		m_pathfind_pause = false;
	//	}
	//}
}
void CUnit::PathFinder_Update(void)
{

	//if(NULL != m_attack_target)
	//{
	//	if(false == m_multithread && false == m_pathfind_pause)
	//	{
	//		int curidx16 = CMyMath::Pos_to_index(m_attack_target->GetPos() , 16);
	//		if(m_target_oldidx !=  curidx16)
	//		{
	//			printf("타겟 길찾기하자 %d\n" , m_obj_id);
	//			m_target_oldidx = curidx16;

	//			m_terrainpath.clear();
	//			m_realpath.clear();
	//			//공격 대상도 탐색에 제외
	//			m_Astar->UnitPath_calculation_Start(m_vPos , m_attack_target->GetPos() , m_stepsize , m_obj_id , true);
	//			m_multithread = true;
	//		}
	//	}
	//}
	//else
	//{
	//	if( ORDER_MOVE_ATTACK == m_eorder && 
	//		m_terrainpath.empty() )
	//	{
	//		//명령이 어택무브 중이었다면 다시 지형을 찾는다.
	//		make_flowfieldpath();
	//	}
	//}

	//if(true == m_multithread)
	//{
	//	m_Astar->UnitPath_calculation_Update(m_vertex , m_realpath);

	//	if(!m_realpath.empty())
	//	{
	//		//m_estate = MOVE;
	//		m_multithread = false;
	//		m_realpathidx = m_realpath.size() - 1;
	//	}
	//}
	//
	//UnitMoving_update();
}
void CUnit::UnitMoving_update(void)
{
	//유닛 이동..

	//if( m_realpath.empty() )
	//	return;

	//if(!m_terrainpath.empty())
	//{
	//	if(0 == m_realpathidx)
	//	{
	//		m_realpath.clear();
	//		
	//		if(CMyMath::pos_distance(m_vPos , m_terrainpath[m_curterrain_pathidx]) < m_arrivalrange*m_arrivalrange)
	//		{		
	//			//m_stepsize = 32;
	//			++m_curterrain_pathidx;
	//			
	//			if((unsigned int)m_curterrain_pathidx >= m_terrainpath.size())
	//			{
	//				//도착점에 도착 했을때
	//				m_eorder = ORDER_NONE;
	//				m_estate = IDLE;
	//				CUnit::Set_texture_statename(L"IDLE");
	//				m_terrainpath.clear();
	//				m_multithread = false;
	//				return;
	//			}
	//		}

	//		bool weightG = false;
	//		if(m_terrainpath.size() - 1 != m_curterrain_pathidx) //terrainidx가 끝이 아니라면
	//			weightG = true;
	//		else
	//			weightG = false;

	//		m_Astar->UnitPath_calculation_Start(m_vPos , m_terrainpath[m_curterrain_pathidx] , m_stepsize , m_obj_id , weightG);

	//		m_multithread = true;		
	//	}
	//	else
	//	{
	//		if(m_curterrain_pathidx != m_terrainpath.size() - 1 &&
	//			CMyMath::pos_distance(m_vPos , m_terrainpath[m_curterrain_pathidx]) < m_arrivalrange*m_arrivalrange)
	//		{
	//			m_realpath.clear();
	//			++m_curterrain_pathidx;
	//			m_Astar->UnitPath_calculation_Start(m_vPos , m_terrainpath[m_curterrain_pathidx] , m_stepsize , m_obj_id , true);
	//			m_multithread = true;
	//			return;
	//		}
	//	}
	//}

	//if(0 == m_realpathidx)
	//	return;

	//m_vcurdir = m_realpath[m_realpathidx - 1] - m_vPos;

	////m_pobj->setdir(m_vcurdir);
	//Dir_calculation(m_vcurdir);

	//m_vprepos = m_vPos + m_vcurdir*GETTIME*m_fspeed;

	//m_prerect.left = m_vprepos.x - m_vertex.left;
	//m_prerect.right = m_vprepos.x + m_vertex.right;
	//m_prerect.top = m_vprepos.y - m_vertex.top;
	//m_prerect.bottom = m_vprepos.y + m_vertex.bottom;



	//if(CArea_Mgr::GetInstance()->overlap_prevention(m_prerect ,m_rect, m_vprepos, m_vPos, m_vcurdir , m_obj_id ))
	//{
	//	m_collisionmove_time += GETTIME;
	//	m_estate = IDLE;
	//}
	//else
	//{
	//	m_collisionmove_time = 0.f;
	//	m_vPos = m_vprepos;
	//	m_rect = m_prerect;
	//	CUnit::Set_texture_statename(L"MOVE");
	//	m_estate = MOVE;
	//}



	//if(m_collisionmove_time >= 0.1f)
	//{
	//	m_collisionmove_time = 0.f;
	//	m_realpath.clear();

	//	if(NULL == m_attack_target)
	//		m_Astar->UnitPath_calculation_Start(m_vPos , m_terrainpath[m_curterrain_pathidx] , m_stepsize , m_obj_id , false);
	//	else
	//		m_Astar->UnitPath_calculation_Start(m_vPos , m_attack_target->GetPos() , m_stepsize , m_obj_id , false);

	//	m_multithread = true;
	//	return;
	//}


	//if( int(CMyMath::pos_distance( m_vPos ,m_realpath[m_realpathidx - 1]) <= 1) )
	//{
	//	m_vPos = m_realpath[m_realpathidx - 1];
	//	--m_realpathidx;
	//}

}
