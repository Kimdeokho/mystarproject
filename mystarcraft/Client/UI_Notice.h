#pragma once
#include "ui.h"

class CUI_Notice :
	public CUI
{
private:
	TCHAR		m_message[256];
public:
	void	set_message(const TCHAR* sz);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual bool UI_ptinrect(const D3DXVECTOR2 vpos);
	virtual void UI_Reaction(void);
	virtual void Release(void);
public:
	CUI_Notice(void);
	~CUI_Notice(void);
};
