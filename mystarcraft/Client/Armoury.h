#pragma once
#include "terran_building.h"

class CArmoury :
	public CTerran_building
{
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Release(void);
	virtual void Dead(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
public:
	CArmoury(void);
	~CArmoury(void);
};
