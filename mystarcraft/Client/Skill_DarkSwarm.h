#pragma once
#include "obj.h"

class CSkill_DarkSwarm :
	public CObj
{
public:
	static int		m_darkswarm_cnt[SQ_TILECNTX*SQ_TILECNTY];
private:
	const TEXINFO*				m_curtex;
	const vector<TEXINFO*>*		m_generatex;
	FRAME						m_frame;

	float		m_dwtime;

	vector<int>					m_idxfont;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CSkill_DarkSwarm(const D3DXVECTOR2 vpos);
	~CSkill_DarkSwarm(void);
};
