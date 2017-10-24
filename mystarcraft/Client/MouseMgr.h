#pragma once

#include "Include.h"
class CMouseMgr
{
	DECLARE_SINGLETON(CMouseMgr)
private:
	POINT m_mousept;
	D3DXVECTOR2 m_vmousept;
public:
	void Update(void);
public:
	 const POINT& GetMousePt(void);
	 const D3DXVECTOR2& GetAddScrollvMousePt(void);
	 const D3DXVECTOR2& GetScreenMousePt(void);
	 const int GetMousePt_to_idx(const int& tilesize);
public:
	CMouseMgr(void);
	~CMouseMgr(void);
};
