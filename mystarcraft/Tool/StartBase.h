#pragma once
#include "obj.h"

class CStartBase :
	public CObj
{
public:
	virtual void Initialize(void);
	virtual void Render(void);
	virtual void Restore_TIleOption(void);
	virtual void MinmapRender(const float fratioX , const float fratioY);
public:
	void InitRect(void);
public:
	CStartBase(void);
	~CStartBase(void);
};
