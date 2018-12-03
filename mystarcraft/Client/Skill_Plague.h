#pragma once
#include "skill.h"

class CSkill_Plague :
	public CSkill
{
private:
	const vector<TEXINFO*>*		m_generatex;

	UNIT_SIZE		m_unitsize;
	float			m_plague_time;
	float			m_damage_tick;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CSkill_Plague(CObj* ptarget);
	~CSkill_Plague(void);
};
