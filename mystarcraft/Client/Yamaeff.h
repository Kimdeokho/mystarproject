#pragma once
#include "skill.h"

class CYamaeff :
	public CSkill
{
private:
	const vector<TEXINFO*>*	m_yamaready_tex;
	const vector<TEXINFO*>*	m_yamafire_tex;

	bool	m_bready;
	bool	m_bfire;
	int		m_loopcnt;

	float	m_ready_time;
	float	m_fire_time;

	D3DXVECTOR3	m_yamaready_center;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CYamaeff(CObj* pobj , CObj* ptarget);
	~CYamaeff(void);
};