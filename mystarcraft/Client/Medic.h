#pragma once
#include "unit.h"

class CMedic :
	public CUnit
{
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);

	virtual void SetDamage(const int& idamage , DAMAGE_TYPE edamagetype);
	virtual void Update_Cmdbtn(void);
	virtual void Update_Wireframe(void);
	virtual void Release(void);
	virtual void Dead(void);
public:
	CMedic(void);
	~CMedic(void);
};
