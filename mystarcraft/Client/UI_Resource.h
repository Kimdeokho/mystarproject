#pragma once
#include "ui.h"

class CUI_Resource :
	public CUI
{
private:
	int		m_mineral_amount[TEAM_END];
	int		m_gas_amount[TEAM_END];

	int		m_cur_mineral[TEAM_END];
	int		m_cur_gas[TEAM_END];

	float		m_population_val[TEAM_END];
	float		m_population_maxval[TEAM_END];

	float			m_tick;

	const TEXINFO*	m_tex_mineral;
	const TEXINFO*	m_tex_gas;
	const TEXINFO*	m_tex_population;

	D3DXVECTOR2		m_mineral_pos;
	D3DXVECTOR2		m_gas_pos;
	D3DXVECTOR2		m_population_pos;

	TEAM_NUMBER		m_myteam;
	
public:
	bool SetResource(const int _mineral ,const int _gas , const TEAM_NUMBER eteam);
	void SetMaxPopvalue(const float _popvalue, const TEAM_NUMBER eteam);
	bool SetPopvalue(const float _popvalue, const TEAM_NUMBER eteam);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CUI_Resource(void);
	~CUI_Resource(void);
};
