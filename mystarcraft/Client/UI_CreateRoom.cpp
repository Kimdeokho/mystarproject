#include "StdAfx.h"
#include "UI_CreateRoom.h"

#include "FontMgr.h"
#include "MouseMgr.h"
#include "Loby_UIMgr.h"
CUI_CreateRoom::CUI_CreateRoom(const D3DXVECTOR2& vpos)
{
	m_vpos = vpos;
}

CUI_CreateRoom::~CUI_CreateRoom(void)
{
	Release();
}

void CUI_CreateRoom::Initialize(void)
{
	D3DXMatrixIdentity(&m_matfont);
	lstrcpy(m_text , L"Create Game");
}

void CUI_CreateRoom::Update(void)
{
	m_matworld._41 = m_vpos.x;
	m_matworld._42 = m_vpos.y;

	if(NULL != m_parentmat)
		m_matworld *= *m_parentmat;

	m_matfont._41 = m_matworld._41 + 82;
	m_matfont._42 = m_matworld._42;

	m_rect.left = m_matworld._41 + 0;
	m_rect.right = m_matworld._41 + 170;
	m_rect.top = m_matworld._42 + 0;
	m_rect.bottom = m_matworld._42 + 20;

	if(MyPtInrect(CMouseMgr::GetInstance()->GetScreenMousePt() , &m_rect))
	{
		CFontMgr::GetInstance()->Setbatch_Font(m_text , m_matfont._41, m_matfont._42
			,D3DCOLOR_ARGB(255,0,255,0));
	}
	else
	{
		CFontMgr::GetInstance()->Setbatch_Font(m_text , m_matfont._41, m_matfont._42
			,D3DCOLOR_ARGB(255,85,185,23));
	}
}

void CUI_CreateRoom::Render(void)
{

}

bool CUI_CreateRoom::UI_ptinrect(const D3DXVECTOR2 vpos)
{
	m_rect.left = m_matworld._41 + 0;
	m_rect.right = m_matworld._41 + 170;
	m_rect.top = m_matworld._42 + 0;
	m_rect.bottom = m_matworld._42 + 20;

	if(MyPtInrect(vpos , &m_rect))
	{
		UI_Reaction();
		return true;
	}

	return false;
}

void CUI_CreateRoom::UI_Reaction(void)
{
	//방만들기
	//CLoby_UIMgr::GetInstance()->Exit();
	CLoby_UIMgr::GetInstance()->Room_Popup(true);
}

void CUI_CreateRoom::Release(void)
{

}
