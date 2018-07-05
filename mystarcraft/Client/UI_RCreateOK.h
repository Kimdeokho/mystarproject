#pragma once
#include "ui.h"

class CUI_RCreateOK :
	public CUI
{
private:
	TCHAR	m_text[32];
public:
	void Initialize(void);
	void Update(void);
	void Render(void);
	bool UI_ptinrect(const D3DXVECTOR2 vpos);
	void UI_Reaction();
	void Init_State(void);
	void Release(void);
public:
	CUI_RCreateOK(void);
	~CUI_RCreateOK(void);
};
