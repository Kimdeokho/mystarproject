#pragma once
#include "ui.h"

class CUI_RoomInfo :
	public CUI
{
private:
	TCHAR	m_szTitle[32];
	TCHAR	m_szSize[32];
public:
	void	SetTitle(const TCHAR* sztitle);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual bool UI_ptinrect(const D3DXVECTOR2 vpos);
	virtual void UI_Reaction(void);
	virtual void Release(void);
public:
	CUI_RoomInfo(void);
	~CUI_RoomInfo(void);
};
