#include "StdAfx.h"
#include "Mineral.h"

#include "TextureMgr.h"
#include "Device.h"
#include "ScrollMgr.h"
CMineral::CMineral(void)
{
}

CMineral::~CMineral(void)
{
}

void CMineral::Initialize(void)
{
	m_rect.left		= m_vPos.x - 32;
	m_rect.right	= m_vPos.x + 32;
	m_rect.top		= m_vPos.y - 16;
	m_rect.bottom	= m_vPos.y + 16;

	m_pSprite = CDevice::GetInstance()->GetSprite();
	m_pGeneralTexture = CTextureMgr::GetInstance()->GetGeneralTexture(L"Mineral");
 
	m_vTextureCenter = D3DXVECTOR3(32, 16, 0);
}
void CMineral::Update(void)
{

}
void CMineral::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;
	m_pSprite->SetTransform(&m_matWorld);
	m_pSprite->Draw( (*m_pGeneralTexture)[0]->pTexture , NULL, &m_vTextureCenter , NULL , D3DCOLOR_ARGB(255,255,255,255));
}
void CMineral::Release(void)
{

}