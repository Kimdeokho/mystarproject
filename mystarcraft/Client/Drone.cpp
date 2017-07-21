#include "stdafx.h"
#include "Drone.h"

#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "TimeMgr.h"
#include "Astar.h"
#include "MouseMgr.h"

#include "Com_fog.h"
#include "Com_Pathfind.h"

#include "LineMgr.h"

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
	m_objname = L"DRONE";
	m_statename = L"";

	
	
	m_vTextureCenter = D3DXVECTOR3(64,64,0);
	m_sortID = SORT_GROUND;
	m_eType = TYPE_GROUND;

	//m_iSightrange = 19;

	//m_vertex.left = 11.5f;
	//m_vertex.right = 11.5f;
	//m_vertex.top =  11.5f;
	//m_vertex.bottom = 11.5f;

	m_vertex.left = 60;
	m_vertex.right = 59;
	m_vertex.top =  41;
	m_vertex.bottom = 42;


	m_rect.left = m_vPos.x - m_vertex.left; 
	m_rect.right = m_vPos.x + m_vertex.right;
	m_rect.top = m_vPos.y - m_vertex.top;
	m_rect.bottom = m_vPos.y + m_vertex.bottom;

	m_comfog = new CCom_fog(m_curidx32 , 480 );
	m_comfog->Initialize(this);

	m_compathfind = new Com_Pathfind;
	m_compathfind->Initialize(this);

	CUnit::Setstate(L"MOVE");
	CUnit::Dir_calculation();
}

void CDrone::Update(void)
{
	CObj::idx_update();
	CUnit::Collision_update();
	//CUnit::FogUpdate();
	

	CUnit::Animation_update();
	//CUnit::Dir_calculation();
	CUnit::PathFinder_Update();

	CUnit::testmove();
	CUnit::Search_Update();
	

	m_comfog->Update();

	short* g = m_flowfieldpath;
	int a = 0;
}

void CDrone::Render(void)
{
	const TEXINFO*	ptemp = NULL;
	ptemp = CTextureMgr::GetInstance()->GetSingleTexture(L"UI" , L"Select32");

	CUnit::Render();

	m_Astar->Path_Render();

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;

	/*그림자 이미지*/
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY + 7;
	m_pSprite->SetTransform(&m_matWorld);
	m_pSprite->Draw(m_curtex , NULL , &m_vTextureCenter , NULL , D3DCOLOR_ARGB(170,0,0,0));

	if(true == m_bSelect)
	{
		/*선택 이미지*/
		m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY + 9;
		m_pSprite->SetTransform(&m_matWorld);
		m_pSprite->Draw(ptemp->pTexture , NULL , &D3DXVECTOR3(20,20,0) , NULL , D3DCOLOR_ARGB(255,0,255,255));
	}


	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;
	m_pSprite->SetTransform(&m_matWorld);
	m_pSprite->Draw(m_curtex , NULL , &m_vTextureCenter , NULL , D3DCOLOR_ARGB(255,255,255,255));



	CLineMgr::GetInstance()->collisionbox_render(m_rect);


	
	

}
void CDrone::Inputkey_reaction(const int& nkey)
{
	if(VK_RBUTTON == nkey)
	{
		//((Com_Pathfind*)m_compathfind)->StartPathfinding();
	}
}

void CDrone::Release(void)
{
	Safe_Delete(m_comfog);
	Safe_Delete(m_compathfind);
}