#pragma once
#include "com_weapon.h"

class CCom_WSiege :
	public CCom_Weapon
{
public:
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	virtual void fire(CObj*& ptarget);
public:
	CCom_WSiege(const int& damage , DAMAGE_TYPE edamagetype);
	~CCom_WSiege(void);
};
