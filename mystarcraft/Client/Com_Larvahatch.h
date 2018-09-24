#pragma once
#include "component.h"

class CCom_Larvahatch :
	public CComponent
{
private:
	int				m_Init_larvacnt;
	float			m_hatchtimer;
	list<CObj*>		m_larvalist;
public:
	DWORD		GetLarvacount(void);
	void		Decrease_Larva(CObj* plarva);
	void		AddLarva(const int loopcnt);
	void		Select_Larva(void);
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Release(void);
public:
	CCom_Larvahatch(int larvacnt);
	~CCom_Larvahatch(void);
};
