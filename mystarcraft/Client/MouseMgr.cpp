#include "StdAfx.h"
#include "MouseMgr.h"

#include "ScrollMgr.h"
#include "MyMath.h"
#include "TimeMgr.h"
#include "KeyMgr.h"

IMPLEMENT_SINGLETON(CMouseMgr)

CMouseMgr::CMouseMgr(void)
{
	RECT Clip;
	GetClientRect(g_hWnd , &Clip);
	ClientToScreen(g_hWnd ,(LPPOINT)&Clip);
	ClientToScreen(g_hWnd ,(LPPOINT)(&Clip.right));
	ClipCursor(&Clip);
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

void CMouseMgr::Update(void)
{

	if(NULL != GetFocus() )
	{
		if(false == CKeyMgr::GetInstance()->GetLbdraging())
		{
			RECT Clip;
			GetClientRect(g_hWnd , &Clip);
			ClientToScreen(g_hWnd ,(LPPOINT)&Clip);
			ClientToScreen(g_hWnd ,(LPPOINT)(&Clip.right));
			ClipCursor(&Clip);

			GetCursorPos(&m_mousept);
			ScreenToClient(g_hWnd , &m_mousept);

			float fspeed = 1500.f;
			if(m_mousept.x <= 5)
			{
				CScrollMgr::m_fScrollX -= GETTIME * fspeed;
				m_mousept.x = 5;
			}
			else if(m_mousept.x >= BACKBUFFER_SIZEX - 5)
			{
				CScrollMgr::m_fScrollX += GETTIME * fspeed;
				m_mousept.x = BACKBUFFER_SIZEX - 5;
			}

			if( m_mousept.y <= 5)
			{
				CScrollMgr::m_fScrollY -= GETTIME * fspeed;
				m_mousept.y = 5;
			}
			else if(m_mousept.y >= BACKBUFFER_SIZEY - 5)
			{
				CScrollMgr::m_fScrollY += GETTIME * fspeed;
				m_mousept.y = BACKBUFFER_SIZEY - 5;
			}
		}
		
	}
	else
	{
		RECT Clip;
		GetClientRect(NULL , &Clip);
		ClientToScreen(NULL ,(LPPOINT)&Clip);
		ClientToScreen(NULL ,(LPPOINT)(&Clip.right));
		ClipCursor(&Clip);
	}
}

//POINT CMouseMgr::m_mousept;
//D3DXVECTOR2 CMouseMgr::m_vmousept;
//POINT CMouseMgr::GetMousePt(void)
//{
//	GetCursorPos(&m_mousept);
//	ScreenToClient(g_hWnd , &m_mousept);
//
//	return m_mousept;
//}
