#include "StdAfx.h"
#include "FontMgr.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CFontMgr)
CFontMgr::CFontMgr(void)
{
}

CFontMgr::~CFontMgr(void)
{
}

void CFontMgr::Initialize(void)
{
	lstrcpy(m_szfont , L"");
	D3DXMatrixIdentity(&m_matfont);
	m_pDevice = CDevice::GetInstance();
	m_pSprite = CDevice::GetInstance()->GetSprite();
	m_pFont   = CDevice::GetInstance()->GetFont();
}

void CFontMgr::FontRender(const TCHAR*	szfont , float posX , float posY , D3DCOLOR _color)
{
	lstrcpy(m_szfont , szfont);
	int len = lstrlen(m_szfont);
	m_matfont._41 = posX - len*10/2;
	m_matfont._42 = posY;
	m_fontcolor	  = _color;

	RECT rc = {0};

	m_pSprite->SetTransform(&m_matfont);
	m_pFont->DrawTextW(m_pSprite , m_szfont, len,
		&rc ,  DT_NOCLIP , m_fontcolor);
}