#pragma once
#include "com_targetsearch.h"

class CCom_Meleesearch :
	public CCom_Targetsearch
{
private:
	bool m_bmelee_search;

	float	 m_meleerangeX;
	float	 m_meleerangeY;

	float	 m_search_time;
	float	 m_meleesearch_time;
	bool	 m_btarget_search;
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CCom_Meleesearch(TARGET_SEARCH_TYPE esearch_type);
	~CCom_Meleesearch(void);
};
