#include "StdAfx.h"
#include "Marine.h"

#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "TimeMgr.h"
#include "MouseMgr.h"

#include "Com_fog.h"
#include "Com_Pathfind.h"
#include "Com_multitexture.h"
#include "Com_Targetsearch.h"
#include "Com_singletexture.h"
#include "Com_MarineAnim.h"
#include "Com_Wmarine.h"

#include "LineMgr.h"
#include "TileManager.h"
#include "Com_Collision.h"
CMarine::CMarine(void)
{
}

CMarine::~CMarine(void)
{
}

void CMarine::Initialize(void)
{
	CObj::Initialize();
	m_curtex = NULL;



	m_sortID = SORT_GROUND;
	m_eType = MOVE_GROUND;
	m_estate = IDLE;
	m_ekategorie = UNIT;
	m_eteamnumber = TEAM_0;
	m_eorder = ORDER_NONE;


	int icase = rand()%23 + 15;
	float fvtx = (float)(icase)/2;

	m_vertex.left = 8.5;
	m_vertex.right = 8.5;
	m_vertex.top =  10;
	m_vertex.bottom = 10;


	//m_rect.left = m_vPos.x - m_vertex.left; 
	//m_rect.right = m_vPos.x + m_vertex.right;
	//m_rect.top = m_vPos.y - m_vertex.top;
	//m_rect.bottom = m_vPos.y + m_vertex.bottom;



	//m_panimation = new CCom_DroneAnim(m_matWorld , m_curtex);

	m_com_pathfind = new CCom_Pathfind(m_vPos , m_rect);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , 512)));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , new CCom_MarineAnim(m_matWorld , m_curtex)));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_PATHFINDE ,  m_com_pathfind) );	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_TARGET_SEARCH , new CCom_Targetsearch(128 , 256 , SEARCH_ONLY_ENEMY) ) );	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_Collision(m_vPos , m_rect , m_vertex)) ) ;	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_WEAPON , new CCom_Wmarine()) ) ;	


	//m_fspeed = float(rand()%170 + 30);
	m_fspeed = 70;

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);
}

void CMarine::Update(void)
{
	CObj::idx_update();

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;
	m_matshadow = m_matWorld;
	m_matshadow._41 -= 4;
}

void CMarine::Render(void)
{
	const TEXINFO*	ptemp = NULL;
	ptemp = CTextureMgr::GetInstance()->GetSingleTexture(L"UI" , L"Select32");


	//m_pSprite->SetTransform(&m_matshadow);
	//m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0)
	//	, NULL , D3DCOLOR_ARGB(125,0,0,0));

	//if(true == m_bSelect)
	//{
	//	D3DXMATRIX matSelect;
	//	matSelect = m_matWorld;
	//	/*선택 이미지*/
	//	matSelect._42 += 13;
	//	m_pSprite->SetTransform(&matSelect);
	//	m_pSprite->Draw(ptemp->pTexture , NULL , &D3DXVECTOR3(20,20,0) , NULL , D3DCOLOR_ARGB(255,0,255,255));
	//}

	m_pSprite->SetTransform(&m_matWorld);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0)
		, NULL , D3DCOLOR_ARGB(255,255,255,255));

	//CLineMgr::GetInstance()->collisionbox_render(m_rect);
}

void CMarine::Inputkey_reaction(const int& nkey)
{
	if(VK_RBUTTON == nkey)
	{
		m_estate = MOVE;
		m_eorder = ORDER_MOVE;

		((CCom_Pathfind*)m_com_pathfind)->SetGoalPos(CTileManager::GetInstance()->GetFlowFiled_GoalPos());
		((CCom_Pathfind*)m_com_pathfind)->SetGoalidx(CTileManager::GetInstance()->GetFlowFiled_Goalidx());
		((CCom_Pathfind*)m_com_pathfind)->SetFlowField();
		((CCom_Pathfind*)m_com_pathfind)->StartPathfinding();

	}
	if('Q' == nkey)
	{
		//m_fspeed = 30;
		m_eteamnumber = TEAM_0;
	}
	if('W' == nkey)
	{
		//m_fspeed = 120;
		m_eteamnumber = TEAM_1;
	}
	if('A' == nkey)
	{		
	}
	if('Z' == nkey)
	{
	}
}

void CMarine::Inputkey_reaction(const int& firstkey , const int& secondkey)
{
	if('A' == firstkey && VK_LBUTTON == secondkey)
	{
		m_eorder = ORDER_MOVE_ATTACK;
		m_estate = MOVE;

		((CCom_Pathfind*)m_com_pathfind)->SetGoalPos(CTileManager::GetInstance()->GetFlowFiled_GoalPos());
		((CCom_Pathfind*)m_com_pathfind)->SetGoalidx(CTileManager::GetInstance()->GetFlowFiled_Goalidx());
		((CCom_Pathfind*)m_com_pathfind)->SetFlowField();
		((CCom_Pathfind*)m_com_pathfind)->StartPathfinding();
	}
}

void CMarine::Release(void)
{

}
