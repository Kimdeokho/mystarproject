#pragma once
#include "obj.h"

class CGasResource :
	public CObj
{
public:
	virtual void Render(void);
	virtual void InitRect(void);
	virtual void Restore_TIleOption(void);
public:
	CGasResource(void);
	~CGasResource(void);
};
