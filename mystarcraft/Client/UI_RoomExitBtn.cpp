#include "StdAfx.h"
#include "UI_RoomExitBtn.h"

#include "FontMgr.h"
#include "MouseMgr.h"
#include "Session_Mgr.h"
CUI_RoomExitBtn::CUI_RoomExitBtn(void)
{
}

CUI_RoomExitBtn::~CUI_RoomExitBtn(void)
{
	Release();
}

void CUI_RoomExitBtn::Initialize(void)
{

}

void CUI_RoomExitBtn::Update(void)
{
	m_matworld._41 = m_vpos.x;
	m_matworld._42 = m_vpos.y;
	if(NULL != m_parentmat)
		m_matworld *= *m_parentmat;

	m_rect.left = m_matworld._41;
	m_rect.right = m_matworld._41 + 140;
	m_rect.top = m_matworld._42;
	m_rect.bottom = m_matworld._42 + 22;

	D3DCOLOR ecolor = D3DCOLOR_ARGB(255,0,255,0);
	if(MyPtInrect(CMouseMgr::GetInstance()->GetScreenMousePt() , &m_rect))
		ecolor = D3DCOLOR_ARGB(255,0,255,0);
	else
		ecolor = D3DCOLOR_ARGB(255,85,185,23);

	CFontMgr::GetInstance()->Setbatch_Font(L"Cancel" , m_matworld._41 + 72, m_matworld._42, 
		ecolor);
}
void CUI_RoomExitBtn::Render(void)
{

}

bool CUI_RoomExitBtn::UI_ptinrect(const D3DXVECTOR2 vpos)
{
	//m_rect.left = m_matworld._41;
	//m_rect.right = m_matworld._41 + 140;
	//m_rect.top = m_matworld._42;
	//m_rect.bottom = m_matworld._42 + 22;

	if(MyPtInrect(vpos , &m_rect))
	{
		UI_Reaction();
		return true;
	}

	return false;
}

void CUI_RoomExitBtn::UI_Reaction(void)
{
	//³ª°¡±â
	WRITE_TCP_PACKET(PT_ROOM_LEAVE , WriteBuffer , 
		WRITE_PT_ROOM_LEAVE(WriteBuffer ));
}

void CUI_RoomExitBtn::Release(void)
{

}
