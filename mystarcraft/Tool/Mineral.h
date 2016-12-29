#pragma once
#include "obj.h"

class CMineral :
	public CObj
{
public:
	virtual void Render(void);
	virtual void InitRect(void);
public:
	virtual void Restore_TIleOption(void);
public:
	CMineral(void);
	~CMineral(void);
};
