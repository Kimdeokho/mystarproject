#pragma once
#include "com_targetsearch.h"

class CCom_Guardsearch :
	public CCom_Targetsearch
{
private:
	float			m_search_time;
	bool			m_btarget_search;

	int				m_irange;
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	void	Range_update(void);
public:
	CCom_Guardsearch(const TARGET_SEARCH_TYPE esearch_type );
	~CCom_Guardsearch(void);
};
