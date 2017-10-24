#pragma once
#include "component.h"


class CCom_Collision :
	public CComponent
{
private:
	CComponent*				m_com_pathfind;
private:
	float			m_search_time;
	const float*	m_fspeed;
	bool			m_bcollision;

	CObj*						m_collision_target;
	D3DXVECTOR2&				m_vPos;
	D3DXVECTOR2					m_collision_vnormal;
	D3DXVECTOR2					m_vtargetpos;

	MYRECT<float>&				m_rect;
	const MYRECT<float>&		m_vertex;

	int				m_target_objid;
	bool			m_bsearch_collision;
	
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Release(void);
public:
	void SetCollsearch(bool bcoll_search);
public:
	CCom_Collision(D3DXVECTOR2& vpos , MYRECT<float>&	rc , const MYRECT<float>& vtx , bool bsearch_collision = true);
	~CCom_Collision(void);
};
