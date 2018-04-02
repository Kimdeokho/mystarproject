#pragma once
#include "com_weapon.h"

class CCom_WGhost :
	public CCom_Weapon
{
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	virtual void fire(CObj*&	ptarget );
public:
	CCom_WGhost(void);
	~CCom_WGhost(void);
};
