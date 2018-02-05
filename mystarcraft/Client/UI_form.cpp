#include "StdAfx.h"
#include "UI_form.h"

#include "TextureMgr.h"
CUI_form::CUI_form(void)
{
	m_is_active = true;
	m_texinfo = NULL;
}

CUI_form::CUI_form(const D3DXVECTOR2& vsize)
{
	m_matworld._11 = vsize.x;
	m_matworld._22 = vsize.y;
}

CUI_form::~CUI_form(void)
{
}

void CUI_form::Initialize(void)
{

}

void CUI_form::Update(void)
{

}

void CUI_form::Render(void)
{
	if(!m_is_active)
		return;

	if(NULL == m_texinfo)
		return;

	m_matworld._41 = m_vpos.x;
	m_matworld._42 = m_vpos.y;

	m_pSprite->SetTransform(&m_matworld);
	m_pSprite->Draw(m_texinfo->pTexture , NULL , &D3DXVECTOR3(0,0,0) , NULL , D3DCOLOR_ARGB(255,255,255,255));
}

void CUI_form::Release(void)
{

}

void CUI_form::set_texture(const TCHAR* texkey , const D3DXVECTOR2& vpos)
{
	m_vpos = vpos;

	m_texinfo = CTextureMgr::GetInstance()->GetSingleTexture(L"UI" , texkey);
}
void CUI_form::set_texture(const TCHAR* texkey )
{
	m_texinfo = CTextureMgr::GetInstance()->GetSingleTexture(L"UI" , texkey);

}

void CUI_form::set_active(bool is_active)
{
	m_is_active = is_active;
}
