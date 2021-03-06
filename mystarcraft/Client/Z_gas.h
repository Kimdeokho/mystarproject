#pragma once
#include "gasbuilding.h"

class CZ_gas :
	public CGasBuilding
{
private:
	//CObj*	m_resource_gas;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Release(void);
	virtual void Dead(void);

	virtual void Update_Wireframe(void);
public:
	CZ_gas(CObj* resource_gas);
	~CZ_gas(void);
};
