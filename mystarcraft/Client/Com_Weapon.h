#pragma once
#include "component.h"

class CCom_Animation;
class CCom_Weapon :
	public CComponent
{
protected:
	int			m_cur_target_num;
	int			m_cur_target_cntnum;

	UPG_INFO*	m_upg_info;
	WEAPON_INFO	m_weapon_info;

	float	m_attack_delay;
	float	m_air_attack_delay;

	float	m_attack_time;
	float	m_air_attack_time;

	bool	m_bfire;
	bool	m_bair_fire;

	CComponent*		m_animation;
	D3DXVECTOR2		m_targetpos;
public:
	void SetAttackDelay(float delay);
	const WEAPON_INFO& GetWeapon_info(void);
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
