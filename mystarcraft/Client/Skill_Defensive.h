#pragma once
#include "skill.h"

class CSkill_Defensive :
	public CSkill
{
private:
	const vector<TEXINFO*>*		m_generatex;
	const vector<TEXINFO*>*		m_generaback_tex;

	TEXINFO*		m_backtex;

	UNIT_SIZE		m_unitsize;
	float			m_defensive_time;

	int				m_shild_hp;
public:
	void	SetDamage(const int& shild);
	int		GetShild(void);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CSkill_Defensive(CObj* pobj , CObj* ptarget);
	~CSkill_Defensive(void);
};
