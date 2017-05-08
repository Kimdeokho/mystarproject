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
public:
	CMouseMgr(void);
	~CMouseMgr(void);
};
