#pragma once
#include "com_weapon.h"

class CCom_WZergling :
	public CCom_Weapon
{
private:
	bool	m_attack_on;
	int		m_target_id;
	int		m_target_cntnum;
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	virtual void fire(CObj*	ptarget );
public:
	CCom_WZergling(void);
	~CCom_WZergling(void);
};
