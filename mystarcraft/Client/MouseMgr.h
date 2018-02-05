#pragma once

#include "Include.h"
class CMouseMgr
{
	DECLARE_SINGLETON(CMouseMgr)
private:
	POINT m_mousept;
	D3DXVECTOR2 m_vmousept;
	D3DXVECTOR2 m_click_pos;
public:
	void Update(void);
	void Init_clickpos(void);
public:
	 const POINT& GetMousePt(void);
	 const D3DXVECTOR2& GetAddScrollvMousePt(void);
	 const D3DXVECTOR2& GetScreenMousePt(void);
	 const D3DXVECTOR2& GetClick_Pos(void);
	 const int GetMousePt_to_idx(const int& tilesize);
public:
	CMouseMgr(void);
	~CMouseMgr(void);
};
