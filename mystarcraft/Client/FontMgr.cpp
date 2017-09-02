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

	m_vecbatchfont.reserve(600);
		
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
	RECT rc = {0};

	if(!m_render_fontlist.empty())
	{
		list<FONT_INFO>::iterator iter = m_render_fontlist.begin();
		list<FONT_INFO>::iterator iter_end = m_render_fontlist.end();

		for( ; iter != iter_end; ++iter)
		{
			m_matfont._41 = (*iter).fX;
			m_matfont._42 = (*iter).fY;

			m_pSprite->SetTransform(&m_matfont);
			m_pFont->DrawTextW(m_pSprite, (*iter).font , lstrlen((*iter).font) , &rc , DT_NOCLIP , (*iter).font_color );


		}
		//m_render_fontlist.clear();
	}


	if(!m_keyrender.empty())
	{
		list<FONT_INFO>::iterator iter = m_keyrender.begin();
		list<FONT_INFO>::iterator iter_end = m_keyrender.end();

		for( ; iter != iter_end; ++iter)
		{
			m_matfont._41 = (*iter).fX;
			m_matfont._42 = (*iter).fY;

			m_pSprite->SetTransform(&m_matfont);
			m_pFont->DrawTextW(m_pSprite, (*iter).font , lstrlen((*iter).font) , &rc , DT_NOCLIP , (*iter).font_color );

		}
	}


	if(!m_vecbatchfont.empty())
	{
		size_t loopcnt = m_vecbatchfont.size();

		for(size_t i = 0; i < loopcnt; ++i)
		{
			
			m_matfont._41 = m_vecbatchfont[i].fX;
			m_matfont._42 = m_vecbatchfont[i].fY;

			m_pSprite->SetTransform(&m_matfont);
			m_pFont->DrawTextW(m_pSprite, m_vecbatchfont[i].font , lstrlen(m_vecbatchfont[i].font) , &rc , DT_NOCLIP , m_vecbatchfont[i].font_color );
		}
		m_vecbatchfont.clear();
	}
}
void CFontMgr::Setnumber_combine_Font(const TCHAR* szfont , const int& font_number , float posX , float posY , D3DCOLOR _color /*= D3DCOLOR_ARGB(255,0,255,0)*/)
{
	m_render_fontlist.clear();

	FONT_INFO ptemp;

	ptemp.fX = posX;
	ptemp.fY = posY;
	ptemp.font_color = _color;
	
	wsprintf(ptemp.font , szfont , font_number);

	m_render_fontlist.push_back(ptemp);
}
void CFontMgr::Set_KeyInput_Font(const TCHAR* szfont , D3DCOLOR _color /*= D3DCOLOR_ARGB(255,0,255,0)*/ )
{
	FONT_INFO ptemp;

	float startpos = 500;

	if(m_keyrender.size() >= 30)
	{
		m_keyrender.pop_front();
	}

	if(!m_keyrender.empty())
	{
		for(list<FONT_INFO>::iterator iter = m_keyrender.begin(); iter != m_keyrender.end(); ++iter)
			(*iter).fY -= 15;
	}

	lstrcpy(ptemp.font , szfont);
	ptemp.fX = 10;
	ptemp.fY = startpos;
	ptemp.font_color = _color;

	m_keyrender.push_back(ptemp);



}
void CFontMgr::Setbatch_Font(const TCHAR* szfont , const int& font_number , float posX , float posY , D3DCOLOR _color /*= D3DCOLOR_ARGB(255,0,255,0)*/)
{
	FONT_INFO ptemp;

	ptemp.fX = posX;
	ptemp.fY = posY;
	ptemp.font_color = _color;

	wsprintf(ptemp.font , szfont , font_number);

	m_vecbatchfont.push_back(ptemp);
}
void CFontMgr::Setbatch_Font(const TCHAR* szfont , float posX , float posY , D3DCOLOR _color /*= D3DCOLOR_ARGB(255,0,255,0)*/)
{
	FONT_INFO ptemp;

	ptemp.fX = posX;
	ptemp.fY = posY;
	ptemp.font_color = _color;
	lstrcpy(ptemp.font , szfont);
	m_vecbatchfont.push_back(ptemp);
}
void CFontMgr::SetFontInfo(const TCHAR* szfont , float posX , float posY , D3DCOLOR _color /*= D3DCOLOR_ARGB(255,0,255,0)*/)
{
	m_render_fontlist.clear();

	FONT_INFO ptemp;

	lstrcpy(ptemp.font , szfont);
	int len = lstrlen(ptemp.font);
	ptemp.fX = posX - len*10/2;
	ptemp.fY = posY;
	m_fontcolor	  = _color;

	m_render_fontlist.push_back(ptemp);
}

void CFontMgr::Release(void)
{
	m_render_fontlist.clear();
}
