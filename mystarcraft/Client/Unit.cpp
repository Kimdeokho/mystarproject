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

CUnit::CUnit(void)
{
	m_fogtime = 0.f;
	m_sightoffsw = false;
	m_InitSight = false;
	m_iSightrange = 0;
	memset(m_fogsearch , 0 , sizeof(m_fogsearch));
	m_diridx = 0;

	m_fAstarTick = 0;
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

}

void CUnit::Release(void)
{
	FogRelease();
	delete m_Astar;
}
void CUnit::FogInitialize(void)
{
	m_InitSight = true;

	
}
void CUnit::FogUpdate(void)
{
	m_curidx32 = CMyMath::Pos_to_index(m_vPos.x , m_vPos.y);

	
	if(true == m_sightoffsw)
		m_fogtime += GETTIME;

	if(m_fogtime >3.0f)
	{
		if(!m_Sightoff_List.empty() )
		{
			list<int>::iterator iter = m_Sightoff_List.begin();
			list<int>::iterator iter_end = m_Sightoff_List.end();

			for( ; iter != iter_end; ++iter)
			{
				m_fogsearch[(*iter)] = false;
				CTileManager::GetInstance()->SightOffRender(*iter);
			}

			m_Sightoff_List.clear();
		}
		m_fogtime = 0.f;
		m_sightoffsw = false;
		CTileManager::GetInstance()->SightOnRender(m_curidx32 , m_iSightrange , m_Sightoff_List , m_fogsearch , m_eType);
	}
	
	if(m_curidx32 != m_oldidx32)
	{
		m_sightoffsw = true;		
		CTileManager::GetInstance()->SightOnRender(m_curidx32 , m_iSightrange , m_Sightoff_List , m_fogsearch , m_eType);
		m_oldidx32 = m_curidx32;
	}
	
}
void CUnit::FogRelease(void)
{
	if(!m_Sightoff_List.empty())
	{
		list<int>::iterator iter = m_Sightoff_List.begin();
		list<int>::iterator iter_end = m_Sightoff_List.end();

		for( ; iter != iter_end; ++iter)
			CTileManager::GetInstance()->SightOffRender(*iter);

		m_Sightoff_List.clear();
	}
}

void CUnit::Dir_calculation(void)
{
	D3DXVECTOR2 vmouse;
	D3DXVECTOR2 vnormal;
	float       fdot , fdgree;
	vmouse = CMouseMgr::GetvMousePt();

	//vmouse.x += CScrollMgr::m_fScrollX;
	//vmouse.y += CScrollMgr::m_fScrollY;

	D3DXVec2Normalize(&vnormal , &(vmouse - m_vPos));
	fdot = D3DXVec2Dot(&vnormal , &OFFSET_DIRVEC);

	fdgree = CMyMath::scala_to_dgree(fdot);

	
	if(vmouse.x <= m_vPos.x)
		fdgree = 360 - fdgree;

	m_diridx = int(fdgree/22.5);

	if(m_diridx > 8)
	{
		m_diridx = 16 - m_diridx;
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
	//CFontMgr::GetInstance()->Setnumber_combine_Font(L"DIR %d" , (int)m_diridx , 100,240);
}
void CUnit::PathFinder_Initialize(void)
{
	m_Astar = m_Astar = new CAstar;
	m_Astar->Initialize(this);
	m_terrain_path = m_Astar->GetTerrain_Path();
	m_dummy_path = m_Astar->Getdummy_Path();
	m_unit_path = m_Astar->GetUnit_Path();

}

void CUnit::PathFinder_Update(void)
{
	/*���������� �׻� �ֺ��� ���ְ� �浹�˻縦 �ϰ��־���Ѵ�.
	�ڽ��� �̵���ο� �����δ��� �����̳� ����ġ ���� ���ְ� �浹�ϰ� �ɰ��
	����Ƚ�� ���� ��ٸ��� ����Ƚ���� �Ѿ�ٸ�
	���� ���ã�⸦ �ٽ��Ѵ�./


	/*���� ��θ� ��������
	������ ��θ� ���ؾ��Ѵ�
	������ ��δ� �ſ� �����ؾ��ϱ⶧���� ª����θ� ���� �̵��Ѵ�.
	������ �������� �ٸ� ������ ���ִٸ� ���� ��θ� ã�´�.
	���� �δ��� ������ ��ֹ�������� �ʴ´�.*/


	/*��ã���� ���� ������ ���ؼ� 
	���� ��θ� �̸� ���صΰ� �׵ڿ� ��¥ ��θ� ���غ���.*/

	m_Astar->Path_calculation_Update(m_vGoalPos);
	m_Astar->MoveUpdate();


		//if(!m_dummy_path->empty())
		//{
		//	static size_t curidx = m_dummy_path->size() - 1;

		//	if(0 != curidx)
		//	{
		//		D3DXVECTOR2 curpos;
		//		D3DXVECTOR2 nextpos;
		//		D3DXVECTOR2 vnormal;

		//		curpos = (*m_dummy_path)[curidx];
		//		nextpos = (*m_dummy_path)[curidx-1];

		//		D3DXVec2Normalize(&vnormal , &(nextpos - curpos));

		//		m_vPos += vnormal*GETTIME*100;

		//		if( abs((int)m_vPos.x - (int)nextpos.x) <= 2 &&
		//			abs((int)m_vPos.y - (int)nextpos.y) <= 2)
		//		{
		//			--curidx;
		//		}
		//	}

		//}

	
	//if(!m_dummy_path->empty())
	//{
	//	m_fAstarTick += GETTIME;
	//	if(m_fAstarTick > 1.5f)
	//	{
	//		m_fAstarTick = 0.f;					
	//		m_Astar->Path_calculation_Start(m_vPos , m_vGoalPos , false);
	//		
	//		m_dummy_path->clear();
	//	}		
	//}
}
void CUnit::Pathfind_start(void)
{
	/*���� ��� ���ϱ�*/
	m_vGoalPos = CMouseMgr::GetvMousePt();
	m_vwaypointPos = CMouseMgr::GetvMousePt();

	m_Astar->Path_calculation_Start(m_vPos , m_vGoalPos , true);

	/*���� ��� ���ϱ�*/
	//m_vwaypointPos = (*m_terrain_path)[0];
	//m_Astar->Path_calculation(m_vPos , m_vwaypointPos , m_rect);
}

