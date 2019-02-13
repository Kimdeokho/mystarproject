#pragma once
#include "obj.h"

class CComset_Scan :
	public CObj
{
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CComset_Scan(void);
	~CComset_Scan(void);
};
