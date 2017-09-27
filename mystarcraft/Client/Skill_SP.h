#pragma once
#include "skill.h"

class CComponent;
class CSkill_SP :
	public CSkill
{
private:
	CComponent*	m_com_weapon;
	float			m_sptime;
	bool			m_bstimpack;
	bool			m_bonce;
public:
	virtual void Initialize(void);
public:
	virtual void Update(void);
	virtual void use(void);
public:
	CSkill_SP(void);
	CSkill_SP(CObj*	pobj , CComponent* pweapon);
	~CSkill_SP(void);
};
