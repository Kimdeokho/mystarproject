#pragma once
#include "zerg_building.h"

class CSpore :
	public CZerg_building
{
private:
public:
	virtual void Build_Complete(void);
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
public:
	CSpore(void);
	~CSpore(void);
};
