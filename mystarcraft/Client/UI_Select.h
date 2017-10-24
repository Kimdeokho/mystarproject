#pragma once
#include "ui.h"

class CUI_Select :
	public CUI
{
private:
	//const D3DXMATRIX&	m_objmat;
	const D3DXVECTOR2&	m_objpos;
	float				m_fweightY;
private:
	bool				m_brender;
	SELECT_FLAG			m_eflag;


	float				m_blinktime;
	float				m_blinkend_time;
	float				m_blink_interval;
	float				m_interval_time;
	bool				m_blink_on;
	int					m_blinkcnt;
public:
	void SetSelect(SELECT_FLAG eflag = GENERAL_SELECT,D3DCOLOR ecolor = D3DCOLOR_ARGB(255,0,255,255));
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CUI_Select(const TCHAR* statekey ,const D3DXVECTOR2& objpos , float fweightY);
	~CUI_Select(void);
};
