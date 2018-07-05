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
	/*ȭ�� ��ǥ�� ��ũ�Ѱ����� ���Ѱ� ���� 0~4096*/
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
	//Ŭ�� �� ������ ��ġ�� �����Ѵ�
	/*���� ����, N������ ������ �Լ��� �����ϴ� �������� ���콺�� ��ġ�� �ʿ�� �Ҷ�
	 ������ GetcursorPos�� ȣ���ϸ� �ȵȴ� ������ �񵿱� �����ε� ���� �ڼ��� �������ڸ�
	 1~10�� ������ �Լ����� ���������� �����ϴ� �������� ���콺�� ��ġ�� ���ݸ��̶� �����̸�
	 1�� ���ְ� 10�������� ���� �ٸ� ���콺�� ��ǥ�� ������ ��Ȳ�� ���� �� �ִ�.
	 ���� �� �Լ��� Ŭ���� ���� ��ǥ�� ������ ���� ���� ��ǥ�� �ٸ� ������Ʈ���� �ѷ��ֱ� �����̴�*/


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
