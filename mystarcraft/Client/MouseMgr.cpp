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
		if(false == CKeyMgr::GetInstance()->GetTurboKeyDown_Check(VK_LBUTTON))
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

void CMouseMgr::Init_clickpos(void)
{
	//클릭 한 순간의 위치를 저장한다
	/*만든 이유, N마리의 유닛의 함수를 실행하는 과정에서 마우스의 위치를 필요로 할때
	 일일히 GetcursorPos를 호출하면 안된다 이유는 비동기 때문인데 좀더 자세히 설명하자면
	 1~10번 유닛의 함수를을 순차적으로 실행하는 과정에서 마우스의 위치를 조금만이라도 움직이면
	 1번 유닛과 10번유닛이 서로 다른 마우스의 좌표를 가지는 상황이 나올 수 있다.
	 따라서 이 함수는 클릭한 순간 좌표를 저장한 다음 같은 좌표를 다른 오브젝트에게 뿌려주기 위함이다*/


	GetCursorPos(&m_mousept);
	ScreenToClient(g_hWnd , &m_mousept);

	m_click_pos.x = float(m_mousept.x) + CScrollMgr::m_fScrollX;
	m_click_pos.y = float(m_mousept.y) + CScrollMgr::m_fScrollY;
}
const D3DXVECTOR2& CMouseMgr::GetClick_Pos(void)
{
	return m_click_pos;
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
