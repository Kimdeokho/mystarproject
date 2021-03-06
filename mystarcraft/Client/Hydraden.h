#pragma once
#include "zerg_building.h"

class CHydraden :
	public CZerg_building
{
private:
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
	virtual bool Input_cmd(const int& nkey , bool* waitkey);
	virtual void Update_Cmdbtn(void);
	virtual void Update_Wireframe(void);

	virtual void Dead(void);
	virtual void Release(void);
public:
	CHydraden(void);
	~CHydraden(void);
};
