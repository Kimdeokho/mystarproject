#include "StdAfx.h"
#include "Tankbody.h"

#include "Com_fog.h"
#include "Com_Collision.h"
#include "Com_Pathfind.h"
#include "Com_TankbodyAnim.h"

#include "TileManager.h"

#include "ScrollMgr.h"
#include "LineMgr.h"
#include "FontMgr.h"
CTankbody::CTankbody(void)
{
}

CTankbody::~CTankbody(void)
{
}

void CTankbody::Initialize(void)
{
	CObj::Initialize();

	m_ecategory = UNIT;
	m_eteamnumber = TEAM_0;

	m_unitinfo.eMoveType = MOVE_GROUND;
	m_unitinfo.estate = IDLE;
	m_unitinfo.eorder = ORDER_NONE;
	m_unitinfo.eDamageType = DAMAGE_NOMAL;
	m_unitinfo.eArmorType = ARMOR_LARGE;
	m_unitinfo.damage = 0;
	m_unitinfo.hp = 150;
	m_unitinfo.mp = 0;
	m_unitinfo.fspeed = 68;


	m_vertex.left = 16;
	m_vertex.right = 16;
	m_vertex.top =  16;
	m_vertex.bottom = 16;

	m_com_pathfind = new CCom_Pathfind(m_vPos , m_rect);
	m_com_anim = new CCom_TankbodyAnim(m_matWorld , m_curtex);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , 512) ));
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_Collision(m_vPos , m_rect , m_vertex) ) ) ;	
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , m_com_anim ));		
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_PATHFINDE ,  m_com_pathfind) );

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);
}

void CTankbody::Update(void)
{
	CObj::idx_update();

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;


	if(IDLE == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
	}
	else if(ATTACK == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"IDLE");
	}
	else if(MOVE == m_unitinfo.estate)
	{
		((CCom_Animation*)m_com_anim)->SetAnimation(L"MOVE");
	}

	CFontMgr::GetInstance()->Setbatch_Font(L"%d",m_unitinfo.hp , m_matWorld._41 , m_matWorld._42);
}

void CTankbody::Render(void)
{
	if(NULL == m_curtex)
		return;

	m_pSprite->SetTransform(&m_matWorld);
	if(TEAM_1 == m_eteamnumber)
	{
		m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0)
			, NULL , D3DCOLOR_ARGB(255,255,0,0));
	}
	else
	{
		m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0)
			, NULL , D3DCOLOR_ARGB(255,255,255,255));
	}

	CLineMgr::GetInstance()->collisionbox_render(m_rect);
}

void CTankbody::Inputkey_reaction(const int& nkey)
{
	if(VK_RBUTTON == nkey)
	{
		m_unitinfo.estate = MOVE;
		m_unitinfo.eorder = ORDER_MOVE;

		if(NULL != m_com_pathfind)
		{
			((CCom_Pathfind*)m_com_pathfind)->SetGoalPos(CTileManager::GetInstance()->GetFlowFiled_GoalPos());
			((CCom_Pathfind*)m_com_pathfind)->SetGoalidx(CTileManager::GetInstance()->GetFlowFiled_Goalidx());
			((CCom_Pathfind*)m_com_pathfind)->SetFlowField();
			((CCom_Pathfind*)m_com_pathfind)->StartPathfinding();
		}
	}
}

void CTankbody::Inputkey_reaction(const int& firstkey , const int& secondkey)
{
	if('A' == firstkey && VK_LBUTTON == secondkey)
	{
		m_unitinfo.eorder = ORDER_MOVE_ATTACK;
		m_unitinfo.estate = MOVE;

		if(NULL != m_com_pathfind)
		{
			((CCom_Pathfind*)m_com_pathfind)->SetGoalPos(CTileManager::GetInstance()->GetFlowFiled_GoalPos());
			((CCom_Pathfind*)m_com_pathfind)->SetGoalidx(CTileManager::GetInstance()->GetFlowFiled_Goalidx());
			((CCom_Pathfind*)m_com_pathfind)->SetFlowField();
			((CCom_Pathfind*)m_com_pathfind)->StartPathfinding();
		}


	}
}
