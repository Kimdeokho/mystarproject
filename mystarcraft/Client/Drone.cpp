#include "stdafx.h"
#include "Drone.h"

#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "TimeMgr.h"
#include "Astar.h"
#include "MouseMgr.h"

CDrone::CDrone(void)
{
}

CDrone::~CDrone(void)
{
	Release();
}

void CDrone::Initialize(void)
{
	CObj::Initialize();
	m_curtex = NULL;

	for(int i = 0; i < DIR_CNT; ++i)
	{
		m_moveTex[i] = CTextureMgr::GetInstance()->GetZUnitTexture(L"DRONE" , L"MOVE" , i);
	}

	m_vTextureCenter = D3DXVECTOR3(64,64,0);
	m_sortID = SORT_GROUND;
	m_eType = TYPE_GROUND;
	m_iSightrange = 19;

	m_rect.left = m_vPos.x - 11.5f;
	m_rect.right = m_vPos.x + 11.5f;
	m_rect.top = m_vPos.y - 11.5f;
	m_rect.bottom = m_vPos.y + 11.5f;

	PathFinder_Initialize();
}

void CDrone::Update(void)
{
	CObj::idx_update();
	//CUnit::FogUpdate();
	CUnit::Dir_calculation();
	CUnit::PathFinder_Update();

	static float ftime = 0.f;
	static int frame = 0;

	ftime += GETTIME;

	if(ftime >= 0.2f)
	{
		ftime = 0.f;
		frame += 1;
	}

	if(frame > 5)
		frame = 0;

	//m_vPos.x += GETTIME*50;
	

	const vector<TEXINFO*>* vtemp = m_moveTex[m_diridx];
	m_curtex = ((*vtemp)[frame])->pTexture;

}

void CDrone::Render(void)
{
	m_Astar->Path_Render();

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;

	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY + 7;
	m_pSprite->SetTransform(&m_matWorld);
	m_pSprite->Draw(m_curtex , NULL , &m_vTextureCenter , NULL , D3DCOLOR_ARGB(255,0,0,0));

	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;
	m_pSprite->SetTransform(&m_matWorld);
	m_pSprite->Draw(m_curtex , NULL , &m_vTextureCenter , NULL , D3DCOLOR_ARGB(255,255,255,255));


}
void CDrone::Inputkey_reaction(const int& nkey)
{
	if(VK_RBUTTON == nkey)
	{
		Pathfind_start();
	}
}

void CDrone::Release(void)
{
	
}