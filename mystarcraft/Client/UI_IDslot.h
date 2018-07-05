#pragma once
#include "ui.h"

class CUI_IDslot :
	public CUI
{
private:
	TCHAR		m_text[32];
public:
	void	SetUser(const WCHAR* szid);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual bool UI_ptinrect(const D3DXVECTOR2 vpos);
	virtual void UI_Reaction(void);
	virtual void Release(void);
public:
	CUI_IDslot(void);
	~CUI_IDslot(void);
};
