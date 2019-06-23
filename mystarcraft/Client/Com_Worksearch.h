#pragma once
#include "com_targetsearch.h"

class CCom_Worksearch :
	public CCom_Targetsearch
{
private:
	CObj*	m_pmineral;
	CObj*	m_core_center; //해처리, 커맨드센터

	int		m_mineral_number;
	int		m_core_number;

	float	m_fsearch_time;

	float	m_collRange;
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CCom_Worksearch();
	~CCom_Worksearch(void);
};
