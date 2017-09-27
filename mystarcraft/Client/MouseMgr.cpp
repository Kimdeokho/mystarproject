#include "StdAfx.h"
#include "MouseMgr.h"

#include "ScrollMgr.h"
#include "MyMath.h"

CMouseMgr::CMouseMgr(void)
{
}

CMouseMgr::~CMouseMgr(void)
{
}
const int CMouseMgr::GetMousePt_to_idx(const int& tilesize)
{
	return CMyMath::Pos_to_index(m_vmousept , tilesize);
}
const D3DXVECTOR2& CMouseMgr::GetAddScrollvMousePt(void)
{
	/*화면 좌표에 스크롤값까지 더한거 범위 0~4096*/
	GetCursorPos(&m_mousept);
	ScreenToClient(g_hWnd , &m_mousept);

	m_vmousept.x = float(m_mousept.x) + CScrollMgr::m_fScrollX;
	m_vmousept.y = float(m_mousept.y) + CScrollMgr::m_fScrollY;
	return m_vmousept;
}
const POINT& CMouseMgr::GetMousePt(void)
{
	GetCursorPos(&m_mousept);
	ScreenToClient(g_hWnd , &m_mousept);

	return m_mousept;
}

const D3DXVECTOR2& CMouseMgr::GetScreenMousePt(void)
{
	GetCursorPos(&m_mousept);
	ScreenToClient(g_hWnd , &m_mousept);

	m_vmousept.x = float(m_mousept.x);
	m_vmousept.y = float(m_mousept.y);

	return m_vmousept;
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
