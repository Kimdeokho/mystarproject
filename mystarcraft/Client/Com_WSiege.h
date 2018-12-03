#pragma once
#include "com_weapon.h"

class CCom_WSiege :
	public CCom_Weapon
{
private:
	list<CObj*>		m_injure_list;
	float			m_splash_range[3];
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	virtual void fire(CObj* ptarget);
public:
	CCom_WSiege();
	~CCom_WSiege(void);
};
