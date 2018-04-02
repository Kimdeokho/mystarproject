#pragma once
#include "obj.h"

class CNuclear_aim :
	public CObj
{
private:
	const TEXINFO*	m_curtex;
	const vector<TEXINFO*>*	m_aimtex;

	CObj*			m_pobj;
	FRAME			m_frame;	

	bool	m_bactive;
	bool	m_bfire;

	float	m_aim_time;

	D3DXVECTOR3	m_vcenter;

	int		m_caster_id;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
public:
	CNuclear_aim(CObj* pobj);
	~CNuclear_aim(void);
};
