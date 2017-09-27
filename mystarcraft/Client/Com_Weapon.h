#pragma once
#include "component.h"


class CCom_Weapon :
	public CComponent
{
protected:
	float	m_attack_delay;
	float	m_attack_time;
	bool	m_bfire;
	int		m_damage;
	DAMAGE_TYPE	m_edamagetype;

	CComponent*		m_animation;
	D3DXVECTOR2		m_targetpos;
public:
	void SetAttackDelay(float delay);
public:
	virtual void fire(CObj*&	ptarget) = 0;
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
