#pragma once
#include "obj.h"

class CMineral :
	public CObj
{
private:
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Release(void);
public:
	CMineral(void);
	~CMineral(void);
};
