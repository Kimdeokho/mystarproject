#include "StdAfx.h"
#include "MouseMgr.h"


CMouseMgr::CMouseMgr(void)
{
}

CMouseMgr::~CMouseMgr(void)
{
}
POINT CMouseMgr::m_mousept;
D3DXVECTOR2 CMouseMgr::m_vmousept;
//POINT CMouseMgr::GetMousePt(void)
//{
//	GetCursorPos(&m_mousept);
//	ScreenToClient(g_hWnd , &m_mousept);
//
//	return m_mousept;
//}
