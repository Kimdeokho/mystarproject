#include "StdAfx.h"
#include "Mineral_frag.h"

#include "TextureMgr.h"
#include "ScrollMgr.h"
CMineral_frag::CMineral_frag(CObj* pobj)
{
	m_pobj = pobj;
}

CMineral_frag::~CMineral_frag(void)
{
}

void CMineral_frag::Initialize(void)
{
	m_sortID = SORT_GROUND;
	m_vecTex = CTextureMgr::GetInstance()->GetGeneralTexture(L"MINERAL_FRAG");

	m_matWorld._11 = 1.2f;
	m_matWorld._22 = 1.2f;

}

void CMineral_frag::Update(void)
{
	m_vPos = m_pobj->GetPos() + m_pobj->GetcurDir()*20;
}

void CMineral_frag::Render(void)
{
	
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_pSprite->SetTransform(&m_matWorld);
	m_pSprite->Draw((*m_vecTex)[0]->pTexture , NULL , &D3DXVECTOR3(16,16,0) , NULL , D3DCOLOR_ARGB(255,255,255,255));
}

void CMineral_frag::Release(void)
{

}
