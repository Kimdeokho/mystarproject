#pragma once

#include "Component.h"

class CCom_generaltexture 
	: public CComponent
{
private:
	LPD3DXSPRITE			m_pSprite;
public:
	CCom_generaltexture(void);
	~CCom_generaltexture(void);
};
