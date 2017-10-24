#pragma once
#include "gasbuilding.h"

class CT_gas :
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
public:
	CT_gas(void);
	CT_gas(CObj* resource_gas);
	~CT_gas(void);
};
