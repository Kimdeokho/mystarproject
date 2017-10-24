#pragma once
#include "Include.h"

class CDevice;
class CFontMgr
{
public:
	DECLARE_SINGLETON(CFontMgr)
private:
	D3DXMATRIX			m_matfont;
	CDevice*			m_pDevice;
	LPD3DXSPRITE		m_pSprite;
	LPD3DXFONT			m_pFont;
	D3DCOLOR			m_fontcolor;
	TCHAR				m_szfont[MAX_PATH];

	list<FONT_INFO>		m_render_fontlist;
	list<FONT_INFO>		m_keyrender;
	vector<FONT_INFO>	m_vecbatchfont;
	list<FONT_INFO>		m_list_noticefont;
	//boost::object_pool<FONT_INFO>	m_fontpool;
public:
	void	Initialize(void);
	void    Setnumber_combine_Font(const TCHAR* szfont , const int& font_number , float posX , float posY , D3DCOLOR _color = D3DCOLOR_ARGB(255,0,255,0));
	void	Setnumber_combine_Font(const TCHAR* szfont , const float& font_number , float posX , float posY , D3DCOLOR _color /*= D3DCOLOR_ARGB(255,0,255,0)*/);
	void	Setbatch_Font(const TCHAR* szfont , const int& font_number , float posX , float posY , D3DCOLOR _color = D3DCOLOR_ARGB(255,0,255,0));
	void	Setbatch_Font(const TCHAR* szfont , float posX , float posY , D3DCOLOR _color = D3DCOLOR_ARGB(255,0,255,0));
	void	Set_KeyInput_Font(const TCHAR* szfont , D3DCOLOR _color = D3DCOLOR_ARGB(255,0,255,0));
	void	SetFontInfo(const TCHAR* szfont , float posX , float posY , D3DCOLOR _color = D3DCOLOR_ARGB(255,0,255,0));

	void	SetNoticeFont(const TCHAR* szfont , float posX , float posY , D3DCOLOR _color = D3DCOLOR_ARGB(255,255,255,255));
	void	SetNoticeFont(const TCHAR* szfont , const int& fontnumber, float posX , float posY , D3DCOLOR _color = D3DCOLOR_ARGB(255,255,255,255));
	void	FontRender(const TCHAR*	szfont , float posX , float posY , D3DCOLOR _color);
	void	FontRender(void);
	void    Release(void);
private:
	CFontMgr(void);
public:
	~CFontMgr(void);
};
