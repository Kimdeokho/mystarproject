#pragma once

#include "Include.h"
class CMouseMgr
{
private:
	static POINT m_mousept;
	static D3DXVECTOR2 m_vmousept;
public:
	static const POINT& GetMousePt(void);
	static const D3DXVECTOR2& GetvMousePt(void);
	static const int GetMousePt_to_idx(const int& tilesize);
public:
	CMouseMgr(void);
	~CMouseMgr(void);
};
