#pragma once

#include "Include.h"
class CMouseMgr
{
private:
	static POINT m_mousept;
public:
	static POINT GetMousePt(void)
	{
		GetCursorPos(&m_mousept);
		ScreenToClient(g_hWnd , &m_mousept);

		return m_mousept;
	}
public:
	CMouseMgr(void);
	~CMouseMgr(void);
};
