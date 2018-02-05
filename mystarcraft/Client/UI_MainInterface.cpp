#include "StdAfx.h"
#include "UI_MainInterface.h"
#include "TextureMgr.h"
CUI_MainInterface::CUI_MainInterface(const TCHAR* statekey)
{
	m_statekey = statekey;
}

CUI_MainInterface::~CUI_MainInterface(void)
{
}

void CUI_MainInterface::Initialize(void)
{
	m_texinfo = CTextureMgr::GetInstance()->GetSingleTexture(L"UI" , m_statekey);
	m_vcenter.x = (float)(m_texinfo->ImgInfo.Width/2);
	m_vcenter.y = (float)(m_texinfo->ImgInfo.Height/2);
	m_vcenter.z = 0;

	m_color = D3DCOLOR_ARGB(255,255 , 255 , 255);

	m_vpos.x = BACKBUFFER_SIZEX/2;
	m_vpos.y = BACKBUFFER_SIZEY/2 + (BACKBUFFER_SIZEY/2 - m_vcenter.y); //¿ø·¡ À§Ä¡ //600ÀÏ¶© 360 // 480ÀÏ¶© 240
	m_matworld._41 = m_vpos.x;
	m_matworld._42 = m_vpos.y;
}

void CUI_MainInterface::Update(void)
{

}

void CUI_MainInterface::Render(void)
{
	//m_matworld._11 = 1.25f;
	//m_matworld._22 = 1.25f;
	m_pSprite->SetTransform(&m_matworld);
	m_pSprite->Draw(m_texinfo->pTexture , NULL , &m_vcenter , NULL , m_color);
}

void CUI_MainInterface::Release(void)
{

}
