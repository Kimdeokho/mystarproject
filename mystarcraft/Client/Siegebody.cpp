#include "StdAfx.h"
#include "Siegebody.h"

#include "Com_fog.h"
#include "Com_Collision.h"
#include "Com_Pathfind.h"

#include "ScrollMgr.h"
CSiegebody::CSiegebody(void)
{
}

CSiegebody::~CSiegebody(void)
{
}

void CSiegebody::Initialize(void)
{

}

void CSiegebody::Update(void)
{
	CObj::idx_update();
}

void CSiegebody::Render(void)
{
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
}
void CSiegebody::Inputkey_reaction(const int& nkey)
{

}

void CSiegebody::Inputkey_reaction(const int& firstkey , const int& secondkey)
{

}

void CSiegebody::Release(void)
{

}

