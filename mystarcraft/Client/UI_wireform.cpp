#include "StdAfx.h"
#include "UI_wireform.h"

#include "TextureMgr.h"
CUI_wireform::CUI_wireform(void)
{
	m_is_active = true;
	m_texinfo = NULL;
}

CUI_wireform::~CUI_wireform(void)
{
}

void CUI_wireform::Initialize(void)
{

}

void CUI_wireform::Update(void)
{

}

void CUI_wireform::Render(void)
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

void CUI_wireform::Release(void)
{

}
void CUI_wireform::set_texturekey(const TCHAR* texkey)
{
	m_texinfo = CTextureMgr::GetInstance()->GetSingleTexture(L"UI" , texkey);
}
void CUI_wireform::set_objaddress(CObj* const pobj)
{
	m_objaddress = pobj;
}


