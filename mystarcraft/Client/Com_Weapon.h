#pragma once
#include "component.h"

class CCom_Animation;

class CCom_Weapon :
	public CComponent
{
protected:
	float	m_attack_delay;
	float	m_attack_time;
	bool	m_bfire;

	CCom_Animation*		m_animation;
public:
	virtual void fire(CObj*	ptarget) = 0;
public:
	virtual void Initialize(CObj* pobj = NULL) = 0;
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	bool GetFire(void);
public:
	CCom_Weapon(void);
	virtual ~CCom_Weapon(void);
};
