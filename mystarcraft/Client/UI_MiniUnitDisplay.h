#pragma once
#include "ui.h"

class CUI_MiniUnitDisplay :
	public CUI
{
	const D3DXVECTOR2& m_vminipos;
	D3DXVECTOR2			m_vweight;
	TEAM_NUMBER*		m_eteam;
	TEAM_NUMBER			m_myteam;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual const D3DXVECTOR2& GetPos(void) {return m_vminipos;}
	virtual void Render(void);
	virtual void Release(void);
public:
	CUI_MiniUnitDisplay(const D3DXVECTOR2& vobjpos , TEAM_NUMBER* eteam);
	~CUI_MiniUnitDisplay(void);
};
