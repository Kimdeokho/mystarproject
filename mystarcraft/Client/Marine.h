#pragma once
#include "unit.h"

class CComponent;
class CSkill;
class CMarine :
	public CUnit
{
private:
	CComponent*		m_com_weapon;

	CSkill*			m_skill_sp;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
	
	virtual void Release(void);
	virtual void Dead(void);

	virtual void Update_Cmdbtn(void);
	virtual void Update_Wireframe(void);
public:
	CMarine(void);
	~CMarine(void);
};
