#include "StdAfx.h"
#include "UI_RCreateC.h"

#include "FontMgr.h"
#include "Loby_UIMgr.h"
#include "MouseMgr.h"
#include "UI_IME.h"
CUI_RCreateC::CUI_RCreateC(void)
{
}

CUI_RCreateC::~CUI_RCreateC(void)
{
	Release();
}

void CUI_RCreateC::Initialize(void)
{
	lstrcpy(m_text , L"Cancel");

	m_matworld._41 = m_vpos.x;
	m_matworld._42 = m_vpos.y;
}

void CUI_RCreateC::Update(void)
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

void CUI_RCreateC::Render(void)
{

}

bool CUI_RCreateC::UI_ptinrect(const D3DXVECTOR2 vpos)
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

void CUI_RCreateC::UI_Reaction()
{
	CLoby_UIMgr::GetInstance()->Room_Popup(false);
	m_titleime->TextClear();
}

void CUI_RCreateC::Init_State(void)
{

}

void CUI_RCreateC::Release(void)
{

}

void CUI_RCreateC::SetTitleIME(CUI_IME* pui)
{
	m_titleime = pui;	
}
