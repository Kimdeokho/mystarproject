#pragma once
#include "ui.h"

class CUI_Connect :
	public CUI
{
private:
	D3DXMATRIX	m_matfont;
	TCHAR		m_text[32];
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual bool UI_ptinrect(const D3DXVECTOR2 vpos);
	virtual void UI_Reaction(void);
	virtual void Release(void);
public:
	CUI_Connect(const D3DXVECTOR2& vpos);
	~CUI_Connect(void);
};
