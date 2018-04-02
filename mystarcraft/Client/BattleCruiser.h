#pragma once
#include "unit.h"

class CBattleCruiser :
	public CUnit
{
private:
	CSkill*		m_yamaeff;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
	virtual void SetDamage(const int& idamage , DAMAGE_TYPE edamagetype);
	virtual void Release(void);
	virtual void Dead(void);

	virtual void Update_Cmdbtn(void);
	virtual void Update_Wireframe(void);

public:
	CBattleCruiser(void);
	~CBattleCruiser(void);
};
