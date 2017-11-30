#pragma once
#include "component.h"


class CCom_Targetsearch :
	public CComponent
{
protected:
	CObj*			m_ptarget;
	CComponent*		m_com_pathfind;
	CComponent*		m_com_anim;
	CComponent*		m_com_weapon;

	TARGET_SEARCH_TYPE	m_search_type;

	//float			m_fsearchtime;
	int				m_target_objid;

	const int*				m_pattack_range;
	const int*				m_psearch_range;
	const int*				m_pair_range;

	bool			m_bforced_target;

	MYRECT<float> m_myrc;
	MYRECT<float> m_outrc;
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	void	SetTarget(CObj* ptarget);
public:
	CObj*	GetTarget(void);
	int		GetTargetObjid(void);
public:
	CCom_Targetsearch(void);
	virtual ~CCom_Targetsearch(void);
};


