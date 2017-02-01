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
public:
	void	Initialize(void);
	void	FontRender(const TCHAR*	szfont , float posX , float posY , D3DCOLOR _color);
private:
	CFontMgr(void);
public:
	~CFontMgr(void);
};
