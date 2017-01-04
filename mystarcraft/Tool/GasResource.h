#pragma once
#include "obj.h"

class CGasResource :
	public CObj
{
public:
	virtual void Initialize(void);
	virtual void Render(void);
	virtual void MinmapRender(const float fratioX , const float fratioY);
	virtual void Restore_TIleOption(void);
public:
	void InitRect(void);
public:
	CGasResource(void);
	~CGasResource(void);
};
