#pragma once
#include "unit.h"

class CSiegebarrel :
	public CUnit
{
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
public:
	CSiegebarrel(void);
	~CSiegebarrel(void);
};
