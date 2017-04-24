#include "StdAfx.h"
#include "Unit.h"

#include "TextureMgr.h"
#include "TileManager.h"
#include "TimeMgr.h"
#include "MyMath.h"
#include "MouseMgr.h"
#include "FontMgr.h"
#include "ScrollMgr.h"

CUnit::CUnit(void)
{
	m_fogtime = 0.f;
	m_sightoffsw = false;
	m_InitSight = false;
	m_iSightrange = 0;
	memset(m_fogsearch , 0 , sizeof(m_fogsearch));
	m_diridx = 0;
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
}
void CUnit::FogInitialize(void)
{
	m_InitSight = true;
}
void CUnit::FogUpdate(void)
{
	//FogInitialize();

	//m_vcurPos.x = CMouseMgr::GetMousePt().x /*À¯´ÖÀÇÁÂÇ¥*/ + CScrollMgr::m_fScrollX;
	//m_vcurPos.y = CMouseMgr::GetMousePt().y + CScrollMgr::m_fScrollY;


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

	vmouse.x += CScrollMgr::m_fScrollX;
	vmouse.y += CScrollMgr::m_fScrollY;

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

	//CFontMgr::GetInstance()->Setnumber_combine_Font(L"MouseX %d" , (int)vmouse.x , 100,160);
	//CFontMgr::GetInstance()->Setnumber_combine_Font(L"MouseY %d" , (int)vmouse.y , 100,180);
	////CFontMgr::GetInstance()->Setnumber_combine_Font(L"Dot %f" , fdot , 100,200);
	//CFontMgr::GetInstance()->Setnumber_combine_Font(L"Dgree %d" , (int)fdgree , 100,220);
	//CFontMgr::GetInstance()->Setnumber_combine_Font(L"DIR %d" , (int)m_diridx , 100,240);
}
