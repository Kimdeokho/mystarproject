#pragma once
#include "terran_building.h"

class CStar_addon :
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
	virtual bool Input_cmd(const int& nkey, bool* waitkey);
	virtual void Setlink(bool blink , CObj* pobj);

	virtual void Update_Cmdbtn(void);
	virtual void Update_Wireframe(void);
public:
	CStar_addon(CObj* pobj);
	~CStar_addon(void);
};
