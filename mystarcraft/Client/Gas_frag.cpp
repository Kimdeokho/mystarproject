#include "StdAfx.h"
#include "Gas_frag.h"

#include "TextureMgr.h"
#include "ScrollMgr.h"
CGas_frag::CGas_frag(void)
{
}

CGas_frag::~CGas_frag(void)
{
}

void CGas_frag::Initialize(void)
{
	m_sortID = SORT_GROUND;
	m_vecTex = CTextureMgr::GetInstance()->GetGeneralTexture(L"T_GAS_FRAG");

	m_matWorld._11 = 1.2f;
	m_matWorld._22 = 1.2f;
}

void CGas_frag::Update(void)
{

}

void CGas_frag::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_pSprite->SetTransform(&m_matWorld);
	m_pSprite->Draw((*m_vecTex)[0]->pTexture , NULL , &D3DXVECTOR3(16,16,0) , NULL , D3DCOLOR_ARGB(255,255,255,255));
}

void CGas_frag::Release(void)
{

}
