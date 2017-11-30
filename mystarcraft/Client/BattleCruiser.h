#pragma once
#include "unit.h"

class CBattleCruiser :
	public CUnit
{
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
	virtual void Release(void);
	virtual void Dead(void);
public:
	CBattleCruiser(void);
	~CBattleCruiser(void);
};
