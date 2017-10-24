#pragma once
#include "com_targetsearch.h"

class CCom_Meleesearch :
	public CCom_Targetsearch
{
private:
	bool m_bmelee_search;

public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CCom_Meleesearch(const int* attack_range , const int* search_range , TARGET_SEARCH_TYPE esearch_type );
	~CCom_Meleesearch(void);
};
