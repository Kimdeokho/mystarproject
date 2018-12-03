#pragma once
#include "component.h"

class CCom_Detect :
	public CComponent
{
private:
	list<CObj*>*	m_area64;
	list<CObj*>		m_oldlist;
	list<CObj*>		m_curlist;

	float			m_detecttime;

	int				m_range;
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CCom_Detect(void);
	~CCom_Detect(void);
};
