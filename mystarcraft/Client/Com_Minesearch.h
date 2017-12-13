#pragma once
#include "com_targetsearch.h"

class CCom_Minesearch :
	public CCom_Targetsearch
{
private:
private:
	bool	m_bmelee_search;

	float	 m_meleerangeX;
	float	 m_meleerangeY;

	float	 m_search_time;
	bool	 m_btarget_search;
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CCom_Minesearch(void);
	~CCom_Minesearch(void);
};
