#pragma once
#include "component.h"

class CCom_info :
	public CComponent
{
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CCom_info(void);
	~CCom_info(void);
};
