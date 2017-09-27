#pragma once
#include "unit.h"

class CComponent;
class CSkill;
class CMarine :
	public CUnit
{
private:
	CComponent*		m_com_targetsearch;
	CComponent*		m_com_pathfind;
	CComponent*		m_com_weapon;

	CSkill*			m_skill_sp;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
	virtual void SetDestroy(bool bdestroy);
	virtual void Release(void);
public:
	CMarine(void);
	~CMarine(void);
};
