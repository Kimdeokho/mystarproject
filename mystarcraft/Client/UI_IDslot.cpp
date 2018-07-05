#include "StdAfx.h"
#include "UI_IDslot.h"

#include "TextureMgr.h"
#include "FontMgr.h"
CUI_IDslot::CUI_IDslot(void)
{
}

CUI_IDslot::~CUI_IDslot(void)
{
	Release();
}

void CUI_IDslot::Initialize(void)
{
	m_texinfo = CTextureMgr::GetInstance()->GetSingleTexture(L"UI" , L"ID_SLOT");
	lstrcpy(m_text , L"");
}

void CUI_IDslot::Update(void)
{
	m_matworld._41 = m_vpos.x;
	m_matworld._42 = m_vpos.y;
	if(NULL != m_parentmat)
		m_matworld *= *m_parentmat;

	CFontMgr::GetInstance()->Setbatch_Font(m_text , m_matworld._41 + 5 , m_matworld._42 , 
		D3DCOLOR_ARGB(255,0,255,0)
		, true);
}

void CUI_IDslot::Render(void)
{
	m_pSprite->SetTransform(&m_matworld);
	m_pSprite->Draw(m_texinfo->pTexture , NULL , &D3DXVECTOR3(0.f,0.f,0.f) , NULL , 
		m_color);
}

bool CUI_IDslot::UI_ptinrect(const D3DXVECTOR2 vpos)
{
	m_rect.left = m_matworld._41;
	m_rect.right = m_matworld._41 + m_texinfo->ImgInfo.Width;
	m_rect.top = m_matworld._42;
	m_rect.bottom = m_matworld._42 + m_texinfo->ImgInfo.Height;

	if(MyPtInrect(vpos , &m_rect))
		return true;

	return false;
}

void CUI_IDslot::UI_Reaction(void)
{

}

void CUI_IDslot::Release(void)
{

}

void CUI_IDslot::SetUser(const WCHAR* szid)
{
	lstrcpy(m_text , szid);
}
