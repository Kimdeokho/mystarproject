#include "StdAfx.h"
#include "UI_Connect.h"

#include "Device.h"
#include "FontMgr.h"
#include "MouseMgr.h"
#include "Login_UIMgr.h"


CUI_Connect::CUI_Connect(const D3DXVECTOR2& vpos)
{
	m_vpos = vpos;
}

CUI_Connect::~CUI_Connect(void)
{
}

void CUI_Connect::Initialize(void)
{
	D3DXMatrixIdentity(&m_matfont);
	lstrcpy(m_text , L"O K");
}

void CUI_Connect::Update(void)
{
	m_matworld._41 = m_vpos.x;
	m_matworld._42 = m_vpos.y;

	if(NULL != m_parentmat)
		m_matworld *= *m_parentmat;

	m_rect.left = m_matworld._41 - 60;
	m_rect.right = m_matworld._41 + 60;
	m_rect.top = m_matworld._42 - 12;
	m_rect.bottom = m_matworld._42 + 12;

	if(MyPtInrect(CMouseMgr::GetInstance()->GetScreenMousePt() , &m_rect))
	{
		CFontMgr::GetInstance()->Setbatch_Font(m_text , m_matworld._41, m_matworld._42
			,D3DCOLOR_ARGB(255,0,255,0));
	}
	else
	{
		CFontMgr::GetInstance()->Setbatch_Font(m_text , m_matworld._41, m_matworld._42
			,D3DCOLOR_ARGB(255,85,185,23));
	}
}

void CUI_Connect::Render(void)
{
}

bool CUI_Connect::UI_ptinrect(const D3DXVECTOR2 vpos)
{
	m_rect.left = m_matworld._41 - 60;
	m_rect.right = m_matworld._41 + 60;
	m_rect.top = m_matworld._42 - 12;
	m_rect.bottom = m_matworld._42 + 12;

	if(MyPtInrect(vpos , &m_rect))
	{
		UI_Reaction();
		return true;
	}

	return false;
}

void CUI_Connect::UI_Reaction(void)
{
	//¿¬°á	
	CLogin_UIMgr::GetInstance()->Connect_Server();
}

void CUI_Connect::Release(void)
{

}
