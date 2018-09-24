#pragma once
#include "obj.h"

class CCreep_collapse :
	public CObj
{
private:
	float			m_creepoff_timer;
	list<int>		m_creepoff_list;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CCreep_collapse(const list<int>& creeplist);
	~CCreep_collapse(void);
};
