#pragma once

#include "Component.h"

class CCom_generaltexture 
	: public CComponent
{
private:
	LPD3DXSPRITE			m_pSprite;
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CCom_generaltexture(void);
	~CCom_generaltexture(void);
};
