#pragma once
#include "com_weapon.h"

class CCom_WMine :
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
	CCom_WMine(void);
	~CCom_WMine(void);
};
