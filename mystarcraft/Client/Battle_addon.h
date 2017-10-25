#pragma once
#include "terran_building.h"

class CBattle_addon :
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
	virtual void Setlink(bool blink);
public:
	CBattle_addon(CObj* pobj);
	~CBattle_addon(void);
};
