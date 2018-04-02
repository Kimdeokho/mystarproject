#include "StdAfx.h"
#include "UI_Wireframe.h"

#include "TextureMgr.h"
CUI_Wireframe::CUI_Wireframe(const TCHAR* texkey, const D3DXVECTOR2& vpos)
{
	m_texkey = texkey;
	m_vpos = vpos;
}

CUI_Wireframe::~CUI_Wireframe(void)
{
}

void CUI_Wireframe::Initialize(void)
{
	m_texinfo = CTextureMgr::GetInstance()->GetSingleTexture(L"UI" , m_texkey);
	m_matworld._41 = m_vpos.x;
	m_matworld._42 = m_vpos.y;
	m_vcenter = D3DXVECTOR3( 0, 0, 0);
}

void CUI_Wireframe::Update(void)
{

}

void CUI_Wireframe::Render(void)
{
	if(NULL == m_texinfo)
		return;

	m_pSprite->SetTransform(&m_matworld);
	m_pSprite->Draw(m_texinfo->pTexture , NULL , &m_vcenter , NULL , m_color);
}

void CUI_Wireframe::Release(void)
{

}