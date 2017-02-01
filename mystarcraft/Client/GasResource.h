#pragma once
#include "obj.h"

class CGasResource :
	public CObj
{
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Release(void);
public:
	CGasResource(void);
	~CGasResource(void);
};
