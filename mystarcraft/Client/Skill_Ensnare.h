#pragma once
#include "skill.h"

class CSkill_Ensnare :
	public CSkill
{
private:
	const vector<TEXINFO*>*		m_generatex;

	UNIT_SIZE		m_unitsize;
	float			m_ensnare_time;

	float			m_ori_speed;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CSkill_Ensnare(CObj* ptarget);
	~CSkill_Ensnare(void);
};
