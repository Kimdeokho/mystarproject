#pragma once
#include "com_targetsearch.h"

class CCom_Airsearch :
	public CCom_Targetsearch
{
private:
	float		m_search_time;
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CCom_Airsearch(void);
	~CCom_Airsearch(void);
};
