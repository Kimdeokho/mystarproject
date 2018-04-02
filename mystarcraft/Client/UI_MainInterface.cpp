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
	m_vcenter.x = 0.f;
	m_vcenter.y = 0.f;
	m_vcenter.z = 0.f;

	m_color = D3DCOLOR_ARGB(255,255 , 255 , 255);

	//m_vpos.x = BACKBUFFER_SIZEX/2;
	//m_vpos.y = BACKBUFFER_SIZEY/2 + (BACKBUFFER_SIZEY/2 - m_vcenter.y); //원래 위치 //600일땐 360 // 480일땐 240

	m_vpos.x = BACKBUFFER_SIZEX/2 - 320; //320은 사진 절반크기
	m_vpos.y = BACKBUFFER_SIZEY - 480; //480은 사진크기
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
