#include "StdAfx.h"
#include "Drone.h"

#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "TimeMgr.h"
CDrone::CDrone(void)
{
}

CDrone::~CDrone(void)
{
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
}

void CDrone::Update(void)
{
	CObj::idx_update();
	CUnit::FogUpdate();
	CUnit::Dir_calculation();

	static int diridx = 0;
	static float ftime = 0.f;
	static int frame = 0;

	ftime += GETTIME;

	if(ftime >= 0.2f)
	{
		ftime = 0.f;
		diridx += 1;
		frame += 1;
	}

	if(frame > 5)
		frame = 0;
	if(diridx > 16)
		diridx = 0;

	//m_vPos.x += GETTIME*50;
	

	const vector<TEXINFO*>* vtemp = m_moveTex[m_diridx];
	m_curtex = ((*vtemp)[frame])->pTexture;

}

void CDrone::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;

	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY + 7;
	m_pSprite->SetTransform(&m_matWorld);
	m_pSprite->Draw(m_curtex , NULL , &m_vTextureCenter , NULL , D3DCOLOR_ARGB(255,0,0,0));

	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;
	m_pSprite->SetTransform(&m_matWorld);
	m_pSprite->Draw(m_curtex , NULL , &m_vTextureCenter , NULL , D3DCOLOR_ARGB(255,255,255,255));
}

void CDrone::Release(void)
{

}
