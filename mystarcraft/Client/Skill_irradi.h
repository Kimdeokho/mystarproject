#pragma once
#include "skill.h"

class CSkill_irradi :
	public CSkill
{
private:
	const vector<TEXINFO*>*		m_generatex;

	list <CObj*>				m_injure_list;

	float		m_fduration_time;
	float		m_damage_time;
	float		m_fsplash_range[3];
public:

	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CSkill_irradi(CObj* pobj, CObj* ptarget);
	~CSkill_irradi(void);
};
