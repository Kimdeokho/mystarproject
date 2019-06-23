#pragma once
#include "unit.h"

class CSkill_SP;
class CMarine :
	public CUnit
{
private:
	CComponent*		m_com_weapon;

	CSkill_SP*			m_skill_sp;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
	virtual bool Input_cmd(const int& nkey , bool* waitkey);

	virtual void Release(void);
	virtual void Dead(void);

	virtual void SetDamage(const int& idamage , DAMAGE_TYPE edamagetype);
	virtual void Update_Cmdbtn(void);
	virtual void Update_Wireframe(void);
public:
	CMarine(void);
	~CMarine(void);
};
