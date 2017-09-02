#pragma once
#include "component.h"

class CCom_singletexture :
	public CComponent
{
private:
	LPD3DXSPRITE			m_pSprite;
public:
	CCom_singletexture(void);
	~CCom_singletexture(void);
};
