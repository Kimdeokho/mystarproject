#pragma once
#include "Include.h"

class CObj;
class CComponent
{
protected:
	CObj*		m_pobj;
public:
	virtual void Initialize(CObj* pobj) = 0;
	virtual void Update(void) = 0;
	virtual void Release(void){};
public:
	CComponent(void);
	virtual ~CComponent(void);
};
