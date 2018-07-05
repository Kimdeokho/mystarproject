#pragma once
#include "ui.h"

class CUI_MiniUnitDisplay :
	public CUI
{
	const D3DXVECTOR2& m_vminipos;
	D3DXVECTOR2			m_vweight;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CUI_MiniUnitDisplay(const D3DXVECTOR2& vobjpos);
	~CUI_MiniUnitDisplay(void);
};
