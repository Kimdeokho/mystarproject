#pragma once
#include "ui.h"

class CUI_StartBtn :
	public CUI
{
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual bool UI_ptinrect(const D3DXVECTOR2 vpos);
	virtual void UI_Reaction(void);
	virtual void Release(void);
public:
	CUI_StartBtn(void);
	~CUI_StartBtn(void);
};
