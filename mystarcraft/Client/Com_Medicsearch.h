#pragma once
#include "com_targetsearch.h"

class CCom_Medicsearch :
	public CCom_Targetsearch
{
private:
	float	 m_search_time;
	bool	 m_btarget_search;
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CCom_Medicsearch();
	~CCom_Medicsearch(void);
};
