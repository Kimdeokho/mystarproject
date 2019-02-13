#include "StdAfx.h"
#include "MouseMgr.h"

#include "ScrollMgr.h"
#include "MyMath.h"
#include "TimeMgr.h"
#include "KeyMgr.h"
#include "TextureMgr.h"
#include "Area_Mgr.h"
#include "ScrollMgr.h"
#include "FontMgr.h"
#include "Device.h"
#include "Session_Mgr.h"

#include "Obj.h"
IMPLEMENT_SINGLETON(CMouseMgr)

CMouseMgr::CMouseMgr(void)
{
	/*
	RECT Clip;
	GetClientRect(g_hWnd , &Clip);
	ClientToScreen(g_hWnd ,(LPPOINT)&Clip);
	ClientToScreen(g_hWnd ,(LPPOINT)(&Clip.right));
	ClipCursor(&Clip);*/
}

CMouseMgr::~CMouseMgr(void)
{
}

void CMouseMgr::Initialize(void)
{
	m_mouse_texkey = L"";
	m_pSprite = CDevice::GetInstance()->GetSprite();

	m_tex_set = CTextureMgr::GetInstance()->GetStateTexture_vecset(L"MOUSE" , L"CLICK");
	m_clicktex = (*m_tex_set)[0];
	m_vclick_center = D3DXVECTOR3(m_clicktex->ImgInfo.Width/2.f , m_clicktex->ImgInfo.Height/2.f , 0.f);

	SetMouseTexture(L"IDLE");

	D3DXMatrixIdentity(&m_mouse_mat);
	D3DXMatrixIdentity(&m_click_mat);

	m_fsize_sign = 1.f;
	m_fclick_size = 0.7f;
	m_cur_ms = MS_NONE;
	m_is_click_mark = false;

	m_mouse_color = D3DCOLOR_ARGB(255,255,255,255);

	ShowCursor( FALSE );
}

void CMouseMgr::Lobby_Update(void)
{
	m_frame.fcurframe += GETTIME*m_frame.fframespeed;

	if(m_frame.fcurframe >= m_frame.umax)
		m_frame.fcurframe = 0.f;

	m_curtex = (*m_tex_set)[(int)m_frame.fcurframe];

	D3DXVECTOR2 vmouse_pos = GetScreenMousePt();
	m_mouse_mat._41 = vmouse_pos.x;
	m_mouse_mat._42 = vmouse_pos.y;

}

void CMouseMgr::Update(void)
{
	if(GetFocus() == g_hWnd)
	{		
		if(false == CKeyMgr::GetInstance()->GetTurboKeyDown_Check(VK_LBUTTON))
		{
			RECT Clip;
			GetClientRect(g_hWnd , &Clip);
			ClientToScreen(g_hWnd ,(LPPOINT)&Clip); //left , top을 screen(윈도우)좌표로 바꾸기
			ClientToScreen(g_hWnd ,(LPPOINT)(&Clip.right)); //right , bottom을 screen(윈도우)좌표로 바꾸기
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

		m_frame.fcurframe += GETTIME*m_frame.fframespeed;

		if(m_frame.fcurframe >= m_frame.umax)
			m_frame.fcurframe = 0.f;

		m_curtex = (*m_tex_set)[(int)m_frame.fcurframe];

		D3DXVECTOR2 vmouse_pos = GetScreenMousePt();
		m_mouse_mat._41 = vmouse_pos.x;
		m_mouse_mat._42 = vmouse_pos.y;

		if(m_is_click_mark)
		{
			m_fclick_size += GETTIME * m_fsize_sign * 1.5f;
			if(m_fclick_size > 1.f)
			{
				m_fsize_sign *= -1;
				m_fclick_size = 1.f;
			}
			else if(m_fclick_size < 0.7f)
			{
				m_fclick_size = 0.7f;
				m_is_click_mark = false;
			}
			m_click_mat._11 = m_fclick_size;
			m_click_mat._22 = m_fclick_size;
			m_click_mat._41 = m_click_pos.x - CScrollMgr::m_fScrollX;
			m_click_mat._42 = m_click_pos.y - CScrollMgr::m_fScrollY;
		}


	}

	if(Unitdetect())
	{
		SetMouseState(MS_DETECT);
	}
	else
	{
		if(MS_AIM != m_cur_ms)
			SetMouseState(MS_IDLE);
	}
}
void CMouseMgr::Render(void)
{
	if(m_is_click_mark)
	{
		m_pSprite->SetTransform(&m_click_mat);
		m_pSprite->Draw(m_clicktex->pTexture , NULL , &m_vclick_center
			, NULL , D3DCOLOR_ARGB(255,255,255,255));
	}


	m_pSprite->SetTransform(&m_mouse_mat);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &m_vcenter
		, NULL , m_mouse_color);
}

void CMouseMgr::SetMouseState(const MOUSE_STATE estate)
{
	if(MS_CLICK_MARK == estate)
	{
		m_click_pos = GetAddScrollvMousePt();

		m_is_click_mark = true;
		m_fclick_size = 0.7f;
		m_fsize_sign = 1.f;

		SetMouseTexture(L"IDLE");
		//mark on
	}
	else if(MS_AIM == estate)
	{
		SetMouseTexture(L"AIM");
	}
	else if(MS_IDLE == estate)
	{
		SetMouseTexture(L"IDLE");
	}
	else if(MS_DETECT == estate)
	{
		SetMouseTexture(L"ALLY_POINT");
	}
	m_cur_ms = estate;
}
void CMouseMgr::SetMouseTexture(const TCHAR* texkey)
{
	if(m_mouse_texkey != texkey)
		m_mouse_texkey = texkey;
	else
		return;

	m_tex_set = CTextureMgr::GetInstance()->GetStateTexture_vecset(L"MOUSE" , m_mouse_texkey);
	m_curtex = (*m_tex_set)[0];
	m_vcenter = D3DXVECTOR3(m_curtex->ImgInfo.Width/2.f , m_curtex->ImgInfo.Height/2.f , 0.f);

	m_frame.fcurframe = 0.f;
	m_frame.umax = m_tex_set->size();
	m_frame.fframespeed = (float)m_frame.umax;
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

	if(m_vmousept.x <= 1)
		m_vmousept.x = 1;
	else if(m_vmousept.x >= 4095)
		m_vmousept.x = 4095;

	if(m_vmousept.y <= 1)
		m_vmousept.y = 1;
	else if(m_vmousept.y >= 4095)
		m_vmousept.y = 4095;

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

bool CMouseMgr::Unitdetect(void)
{
	if(MS_AIM == m_cur_ms)
		return false;

	list<CObj*>* arealist = CArea_Mgr::GetInstance()->GetArea64();

	D3DXVECTOR2 vmouse_pos = GetAddScrollvMousePt();

	int idxarr[ASTAR_DIR_END] = {0};
	int idx = 0;
	
	CMyMath::Calculator_eight_idx(idxarr , vmouse_pos , 64);

	//CFontMgr::GetInstance()->Setbatch_Font(L"%d, %d" , int(vmouse_pos.x) , int(vmouse_pos.y),
	//	vmouse_pos.x - CScrollMgr::m_fScrollX, vmouse_pos.y - CScrollMgr::m_fScrollY
	//	,D3DCOLOR_ARGB(255,0,255,0));


	TEAM_NUMBER eteam = CSession_Mgr::GetInstance()->GetTeamNumber();
	for(int i = 0; i < ASTAR_DIR_END; ++i)
	{
		idx = idxarr[i];
		if(idx < 0 || arealist[idx].empty())
			continue;

		list<CObj*>::iterator iter		= arealist[idx].begin();
		list<CObj*>::iterator iter_end	= arealist[idx].end();

		for( ; iter != iter_end; ++iter)
		{
			if(OBJ_NONE == (*iter)->GetOBJNAME())
				continue;

			if(MyPtInrect( vmouse_pos ,  &((*iter)->GetMyRect()) ))
			{
				//색변경
				//if( (*iter)->GetTeamNumber() == eteam)
				//	m_mouse_color = D3DCOLOR_ARGB(255,255,255,255);
				//else
				//	m_mouse_color = D3DCOLOR_ARGB(255,255,0,0);

				return true;
			}
		}
	}

	//m_mouse_color = D3DCOLOR_ARGB(255,255,255,255);

	return false;
}