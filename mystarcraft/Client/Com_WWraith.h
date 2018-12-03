#pragma once
#include "com_weapon.h"

class CCom_WWraith :
	public CCom_Weapon
{
private:
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	virtual void fire(CObj*	ptarget );
public:
	CCom_WWraith(void);
	~CCom_WWraith(void);
};
