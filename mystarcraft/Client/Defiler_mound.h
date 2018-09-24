#pragma once
#include "zerg_building.h"

class CDefiler_mound :
	public CZerg_building
{
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
	virtual void Update_Cmdbtn(void);
	virtual void Update_Wireframe(void);

	virtual void Dead(void);
	virtual void Release(void);
public:
	CDefiler_mound(void);
	~CDefiler_mound(void);
};
