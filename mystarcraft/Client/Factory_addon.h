#pragma once
#include "terran_building.h"

class CFactory_addon :
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
	virtual void Setlink(bool blink , CObj* pobj);

	virtual void Update_Cmdbtn(void);
	virtual void Update_Wireframe(void);
public:
	CFactory_addon(CObj* pobj);
	~CFactory_addon(void);
};
