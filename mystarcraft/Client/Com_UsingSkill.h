#pragma once
#include "component.h"

class CCom_UsingSkill :
	public CComponent
{
private:
	SKILL_ORDER				m_skill_order;
	const D3DXVECTOR2*		m_objpos;
	D3DXVECTOR2				m_using_pos;
	CObj*					m_ptarget;
	int						m_targetid;
public:
	void	Setskill_order(SKILL_ORDER so);
	void	SetUsingSkill(SKILL_ORDER so, CObj* pobj , const D3DXVECTOR2& vpos);
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CCom_UsingSkill(void);
	~CCom_UsingSkill(void);
};
