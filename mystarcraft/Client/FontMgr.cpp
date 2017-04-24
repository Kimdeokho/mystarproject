#include "StdAfx.h"
#include "FontMgr.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CFontMgr)
CFontMgr::CFontMgr(void):m_fontpool(100)
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
void CFontMgr::FontRender(void)
{
	if(m_render_fontlist.empty())
		return;

	RECT rc = {0};
	list<FONT_INFO*>::iterator iter = m_render_fontlist.begin();
	list<FONT_INFO*>::iterator iter_end = m_render_fontlist.end();

	for( ; iter != iter_end; ++iter)
	{
		m_matfont._41 = (*iter)->fX;
		m_matfont._42 = (*iter)->fY;

		m_pSprite->SetTransform(&m_matfont);
		m_pFont->DrawTextW(m_pSprite, (*iter)->font , lstrlen((*iter)->font) , &rc , DT_NOCLIP , (*iter)->font_color );

		m_fontpool.destroy( (*iter) );
	}
	m_render_fontlist.clear();
}
void CFontMgr::Setnumber_combine_Font(const TCHAR* szfont , const int& font_number , float posX , float posY , D3DCOLOR _color /*= D3DCOLOR_ARGB(255,0,255,0)*/)
{
	FONT_INFO* ptemp = m_fontpool.construct();

	ptemp->fX = posX;
	ptemp->fY = posY;
	ptemp->font_color = _color;
	
	wsprintf(ptemp->font , szfont , font_number);

	m_render_fontlist.push_back(ptemp);
}
void CFontMgr::Setnumber_combine_Font(const TCHAR* szfont , const float& font_number , float posX , float posY , D3DCOLOR _color /*= D3DCOLOR_ARGB(255,0,255,0)*/)
{
	FONT_INFO* ptemp = m_fontpool.construct();

	ptemp->fX = posX;
	ptemp->fY = posY;
	ptemp->font_color = _color;

	_stprintf_s(ptemp->font , sizeof(TCHAR)*255 ,L"%f", font_number);

	m_render_fontlist.push_back(ptemp);
}
void CFontMgr::SetFontInfo(const TCHAR* szfont , float posX , float posY , D3DCOLOR _color /*= D3DCOLOR_ARGB(255,0,255,0)*/)
{
	FONT_INFO* ptemp = m_fontpool.construct();

	lstrcpy(ptemp->font , szfont);
	int len = lstrlen(ptemp->font);
	ptemp->fX = posX - len*10/2;
	ptemp->fY = posY;
	m_fontcolor	  = _color;

	m_render_fontlist.push_back(ptemp);
}

void CFontMgr::Release(void)
{
	m_render_fontlist.clear();
}
