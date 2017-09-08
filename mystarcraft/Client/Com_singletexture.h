#pragma once
#include "component.h"

class CCom_singletexture :
	public CComponent
{
private:
	LPD3DXSPRITE			m_pSprite;
	const TEXINFO*			m_curtex;

	const TCHAR*			m_kategorie_key;
	const TCHAR*			m_texture_key;

	const D3DXMATRIX&		m_matobj;		
private:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CCom_singletexture(const D3DXMATRIX& matobj, const TCHAR* kategoriekey , const TCHAR* texturekey);
	~CCom_singletexture(void);
};
