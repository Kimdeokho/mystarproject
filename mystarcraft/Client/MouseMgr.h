#pragma once

#include "Include.h"
class CMouseMgr
{
private:
	static POINT m_mousept;
	static D3DXVECTOR2 m_vmousept;
public:
	static const POINT& GetMousePt(void)
	{
		GetCursorPos(&m_mousept);
		ScreenToClient(g_hWnd , &m_mousept);

		return m_mousept;
	}

	static D3DXVECTOR2 GetvMousePt(void)
	{
		GetCursorPos(&m_mousept);
		ScreenToClient(g_hWnd , &m_mousept);

		m_vmousept.x = float(m_mousept.x);
		m_vmousept.y = float(m_mousept.y);
		return m_vmousept;
	}
public:
	CMouseMgr(void);
	~CMouseMgr(void);
};
