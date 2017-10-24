#include "StdAfx.h"
#include "UI_MiniUnitDisplay.h"

#include "TextureMgr.h"
CUI_MiniUnitDisplay::CUI_MiniUnitDisplay(const D3DXVECTOR2& vobjpos)
:m_vpos(vobjpos)
{
	m_statekey = L"DebugTile";
}

CUI_MiniUnitDisplay::~CUI_MiniUnitDisplay(void)
{
}

void CUI_MiniUnitDisplay::Initialize(void)
{
	m_texinfo = CTextureMgr::GetInstance()->GetSingleTexture(L"DebugTile" , L"tile2x2");		
	m_vcenter.x = (float)(m_texinfo->ImgInfo.Width/2);
	m_vcenter.y = (float)(m_texinfo->ImgInfo.Height/2);
	m_vcenter.z = 0;

	m_color = D3DCOLOR_ARGB(255,0 , 255 , 0);

	m_matworld._11 = 1.f;
	m_matworld._22 = 1.f;

	m_vweight.x = (BACKBUFFER_SIZEX - 640)/2 + 5;
	m_vweight.y = BACKBUFFER_SIZEY - 130;
}

void CUI_MiniUnitDisplay::Update(void)
{	

	m_matworld._41 = m_vpos.x * 0.03125f + m_vweight.x;
	m_matworld._42 = m_vpos.y * 0.03125f+ m_vweight.y;
}

void CUI_MiniUnitDisplay::Render(void)
{
	m_pSprite->SetTransform(&m_matworld);
	m_pSprite->Draw(m_texinfo->pTexture , NULL , &m_vcenter , NULL , m_color);
}

void CUI_MiniUnitDisplay::Release(void)
{

}
