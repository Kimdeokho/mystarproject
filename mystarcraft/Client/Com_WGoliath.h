#pragma once
#include "com_weapon.h"

class CCom_WGoliath :
	public CCom_Weapon
{
private:
	int			m_air_damage;
	DAMAGE_TYPE	m_eair_damagetype;
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	virtual void fire(CObj*&	ptarget );
public:
	CCom_WGoliath(void);
	~CCom_WGoliath(void);
};
