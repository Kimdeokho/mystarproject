#pragma once
#include "component.h"

class CCom_Pathfind;
class CCom_Animation;

class CCom_Targetsearch :
	public CComponent
{
private:
	CObj*			m_ptarget;
	CCom_Pathfind*	m_com_pathfind;
	CCom_Animation*	m_com_anim;

	TARGET_SEARCH_TYPE	m_search_type;
	int				m_iattack_range;
	int				m_isearch_range;
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CObj*	GetTarget(void);
public:
	CCom_Targetsearch(void);
	CCom_Targetsearch(const int& attack_range , const int& search_range , TARGET_SEARCH_TYPE esearch_type);
	~CCom_Targetsearch(void);
};


