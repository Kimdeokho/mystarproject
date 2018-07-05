#include "StdAfx.h"
#include "UI_RCreateOK.h"

#include "FontMgr.h"
#include "Loby_UIMgr.h"
#include "MouseMgr.h"

#include "Loby_UIMgr.h"
CUI_RCreateOK::CUI_RCreateOK(void)
{
}

CUI_RCreateOK::~CUI_RCreateOK(void)
{
	Release();
}

void CUI_RCreateOK::Initialize(void)
{
	lstrcpy(m_text , L"O K");
	
	m_matworld._41 = m_vpos.x;
	m_matworld._42 = m_vpos.y;
}

void CUI_RCreateOK::Update(void)
{
	m_matworld._41 = m_vpos.x;
	m_matworld._42 = m_vpos.y;

	if(NULL != m_parentmat)
		m_matworld *= *m_parentmat;

	m_rect.left = m_matworld._41;
	m_rect.right = m_matworld._41 + 156;
	m_rect.top = m_matworld._42;
	m_rect.bottom = m_matworld._42 + 28;

	if(MyPtInrect(CMouseMgr::GetInstance()->GetScreenMousePt() , &m_rect))
	{
		CFontMgr::GetInstance()->Setbatch_Font(m_text , m_matworld._41 + 78, m_matworld._42 + 5
			,D3DCOLOR_ARGB(255,0,255,0));
	}
	else
	{
		CFontMgr::GetInstance()->Setbatch_Font(m_text , m_matworld._41 + 78, m_matworld._42 + 5
			,D3DCOLOR_ARGB(255,85,185,23));
	}
}

void CUI_RCreateOK::Render(void)
{

}

bool CUI_RCreateOK::UI_ptinrect(const D3DXVECTOR2 vpos)
{
	m_rect.left = m_matworld._41;
	m_rect.right = m_matworld._41 + 156;
	m_rect.top = m_matworld._42;
	m_rect.bottom = m_matworld._42 + 28;

	if(MyPtInrect(vpos , &m_rect))
	{
		UI_Reaction();
		return true;
	}

	return false;
}

void CUI_RCreateOK::UI_Reaction()
{
	CLoby_UIMgr::GetInstance()->Create_Room();
}

void CUI_RCreateOK::Init_State(void)
{

}

void CUI_RCreateOK::Release(void)
{

}
