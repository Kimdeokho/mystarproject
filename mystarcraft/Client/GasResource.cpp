#include "StdAfx.h"
#include "GasResource.h"

#include "TextureMgr.h"
#include "Device.h"
#include "ScrollMgr.h"
CGasResource::CGasResource(void)
{
}

CGasResource::~CGasResource(void)
{
}

void CGasResource::Initialize(void)
{
	m_rect.left		= m_vPos.x - 64;
	m_rect.right	= m_vPos.x + 64;
	m_rect.top		= m_vPos.y - 32;
	m_rect.bottom	= m_vPos.y + 32;

	m_pSprite = CDevice::GetInstance()->GetSprite();
	m_pGeneralTexture = CTextureMgr::GetInstance()->GetGeneralTexture(L"Gas");

	m_vTextureCenter = D3DXVECTOR3(64, 32, 0);
}
void CGasResource::Update(void)
{

}
void CGasResource::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;
	m_pSprite->SetTransform(&m_matWorld);
	m_pSprite->Draw( m_pGeneralTexture[0]->pTexture , NULL, &m_vTextureCenter , NULL , D3DCOLOR_ARGB(255,255,255,255));
}
void CGasResource::Release(void)
{

}
