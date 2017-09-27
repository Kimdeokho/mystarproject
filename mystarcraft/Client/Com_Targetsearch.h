#pragma once
#include "component.h"


class CCom_Targetsearch :
	public CComponent
{
private:
	CObj*			m_ptarget; 
	CObj*			m_oldtarget;
	CComponent*		m_com_pathfind;
	CComponent*		m_com_anim;
	CComponent*		m_com_weapon;

	TARGET_SEARCH_TYPE	m_search_type;

	bool			m_attacksw;
	bool			m_bsearch;
	float			m_fsearchtime;
	int				m_target_objid;

	const int*				m_pattack_range;
	const int*				m_psearch_range;
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	void Initialize(CObj* pobj , CObj*	pother_obj);
public:
	void	SetSearchtime(float time);
	void    SetSearch(bool bsearch);
public:
	CObj*	GetTarget(void);
	int		GetTargetObjid(void);
public:
	CCom_Targetsearch(void);
	CCom_Targetsearch(const int* attack_range , const int* search_range , TARGET_SEARCH_TYPE esearch_type);
	~CCom_Targetsearch(void);
};


