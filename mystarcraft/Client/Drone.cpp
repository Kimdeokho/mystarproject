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
#include "FontMgr.h"
#include "TileManager.h"
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
	m_eType = MOVE_GROUND;
	m_estate = IDLE;
	m_ekategorie = UNIT;
	m_ediscrimination = ENEMY;
	m_eorder = ORDER_NONE;

	//m_iSightrange = 19;

	m_vertex.left = 11.5f;
	m_vertex.right = 11.5f;
	m_vertex.top =  11.5f;
	m_vertex.bottom = 11.5f;


	m_rect.left = m_vPos.x - m_vertex.left; 
	m_rect.right = m_vPos.x + m_vertex.right;
	m_rect.top = m_vPos.y - m_vertex.top;
	m_rect.bottom = m_vPos.y + m_vertex.bottom;

	m_comfog = new CCom_fog(m_curidx32 , 512 );
	m_comfog->Initialize(this);

	//m_compathfind = new Com_Pathfind;
	//m_compathfind->Initialize(this);

	CUnit::Set_texture_statename(L"IDLE");

	m_stepsize = 32;

	m_fspeed = float(rand()%170 + 30);
}

void CDrone::Update(void)
{
	CObj::idx_update();

	CUnit::Collision_update();	

	CUnit::Animation_update();
	////CUnit::Dir_calculation();

	CUnit::PathFinder_Update();

	
	CUnit::Search_Update();
	//

	m_comfog->Update();

	//int x , y;
	//x = (int)m_vPos.x;
	//y = (int)m_vPos.y;

	//CFontMgr::GetInstance()->Setbatch_Font(L"%d ," , x , m_vPos.x - 30- CScrollMgr::m_fScrollX  , m_vPos.y - CScrollMgr::m_fScrollY);
	//CFontMgr::GetInstance()->Setbatch_Font(L"%d" , y , m_vPos.x + 10 - CScrollMgr::m_fScrollX , m_vPos.y - CScrollMgr::m_fScrollY);

	//CFontMgr::GetInstance()->Setbatch_Font(L"%d" , m_obj_id , m_vPos.x - CScrollMgr::m_fScrollX , m_vPos.y - CScrollMgr::m_fScrollY - 10);

	//if(IDLE == m_eorder)
	//	CFontMgr::GetInstance()->Setbatch_Font(L"IDLE" , m_vPos.x - CScrollMgr::m_fScrollX - 20, m_vPos.y - CScrollMgr::m_fScrollY);
	//else if(MOVE == m_eorder)
	//	CFontMgr::GetInstance()->Setbatch_Font(L"MOVE" , m_vPos.x - CScrollMgr::m_fScrollX - 20, m_vPos.y - CScrollMgr::m_fScrollY);
	//else if(ATTACK == m_eorder)
	//	CFontMgr::GetInstance()->Setbatch_Font(L"ATTACK" , m_vPos.x - CScrollMgr::m_fScrollX - 20, m_vPos.y - CScrollMgr::m_fScrollY);

	//if(m_notfind_pathcnt >= 5)
	//	CFontMgr::GetInstance()->Setbatch_Font(L"찾기중단" , m_vPos.x - CScrollMgr::m_fScrollX - 20, m_vPos.y - CScrollMgr::m_fScrollY);
}

void CDrone::Render(void)
{
	const TEXINFO*	ptemp = NULL;
	ptemp = CTextureMgr::GetInstance()->GetSingleTexture(L"UI" , L"Select32");

	

	//m_Astar->Path_Render();

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

	CUnit::Render();

}
void CDrone::Inputkey_reaction(const int& nkey)
{
	if(VK_RBUTTON == nkey)
	{
		//m_eorder;
		//CUnit::Set_texture_statename(L"MOVE");
		m_estate = MOVE;
		m_eorder = ORDER_MOVE;

		m_goalpos = CMouseMgr::GetvMousePt();
		m_igoalidx = CMouseMgr::GetMousePt_to_idx(32);
		m_goalpos = CMouseMgr::GetvMousePt();

		memcpy(m_flowfieldpath , CTileManager::GetInstance()->Get_flowfield_node() , sizeof(short)*16384);
		CUnit::make_flowfieldpath();

	}
	if('Q' == nkey)
	{
		m_stepsize = 32;
		//m_fspeed = 30;
	}
	if('W' == nkey)
	{
		m_stepsize = 16;
		//m_fspeed = 120;
	}
	if('A' == nkey)
	{		
	}
	if('Z' == nkey)
	{
	}
}
void CDrone::Inputkey_reaction(const int& firstkey , const int& secondkey)
{
	if('A' == firstkey && VK_LBUTTON == secondkey)
	{
		m_eorder = ORDER_MOVE_ATTACK;
		m_estate = MOVE;

		m_goalpos = CMouseMgr::GetvMousePt();
		m_igoalidx = CMouseMgr::GetMousePt_to_idx(32);
		memcpy(m_flowfieldpath , CTileManager::GetInstance()->Get_flowfield_node() , sizeof(short)*16384);
		CUnit::make_flowfieldpath();
	}
}
void CDrone::Release(void)
{
	Safe_Delete(m_comfog);
	//Safe_Delete(m_compathfind);
}