#pragma once
#include "terran_building.h"

class CEngineering :
	public CTerran_building
{
private:
public:
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Release(void);
	virtual void Dead(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);

	virtual void Update_Cmdbtn(void);
	virtual void Update_Wireframe(void);
public:
	CEngineering(void);
	~CEngineering(void);
};
