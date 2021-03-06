#pragma once
#include "Include.h"
#include "Debug_Mgr.h"

class CObj;
class CComponent
{
protected:
	CObj*		m_pobj;
public:
	virtual void Initialize(CObj* pobj = NULL) = 0;
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CComponent(void);
	virtual ~CComponent(void);
};
