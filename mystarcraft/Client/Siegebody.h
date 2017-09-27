#pragma once
#include "unit.h"

class CSiegebody :
	public CUnit
{
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
public:
	CSiegebody(void);
	~CSiegebody(void);
};
