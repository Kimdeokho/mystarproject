#pragma once
#include "component.h"

class CCom_Transport :
	public CComponent
{
private:
	multimap<int , BOARDING_INFO , greater<int>>	m_boarding_unit;
	int												m_curcapacity;

	float											m_collocate_time;
public:
	void    boarding_wire_ui(void);
	bool	setunit(CObj* pobj);
	bool	unit_landing(void);
	void    collocate_unit(CObj* pobj);
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CCom_Transport(void);
	~CCom_Transport(void);
};
