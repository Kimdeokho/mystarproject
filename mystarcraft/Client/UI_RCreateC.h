#pragma once
#include "ui.h"

class CUI_IME;
class CUI_RCreateC :
	public CUI
{
private:
	TCHAR	m_text[32];
	CUI_IME*	m_titleime;
public:
	void	SetTitleIME(CUI_IME* pui);
public:
	void Initialize(void);
	void Update(void);
	void Render(void);
	bool UI_ptinrect(const D3DXVECTOR2 vpos);
	void UI_Reaction();
	void Init_State(void);
	void Release(void);
public:
public:
	CUI_RCreateC(void);
	~CUI_RCreateC(void);
};
