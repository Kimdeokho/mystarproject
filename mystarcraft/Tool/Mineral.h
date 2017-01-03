#pragma once
#include "obj.h"

class CMineral :
	public CObj
{
public:
	virtual void Initialize();
	virtual void Render(void);
	void InitRect(void);
public:
	virtual void Restore_TIleOption(void);
public:
	CMineral(void);
	~CMineral(void);
};
