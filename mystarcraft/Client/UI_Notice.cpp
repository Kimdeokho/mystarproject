#include "StdAfx.h"
#include "UI_Notice.h"

#include "TimeMgr.h"
#include "FontMgr.h"
#include "TextureMgr.h"

CUI_Notice::CUI_Notice(void)
{
}

CUI_Notice::~CUI_Notice(void)
{
}

void CUI_Notice::Initialize(void)
{
	m_active = false;
	lstrcpy(m_message , L"");
	m_texinfo = CTextureMgr::GetInstance()->GetSingleTexture(L"UI" , L"POPOPUP");

	m_matworld._41 = m_vpos.x;
	m_matworld._42 = m_vpos.y;

	m_color = D3DCOLOR_ARGB(100,255,255,255);

	m_vpos.x = BACKBUFFER_SIZEX / 2 - m_texinfo->ImgInfo.Width / 2.f;
	m_vpos.y = BACKBUFFER_SIZEY / 2 - m_texinfo->ImgInfo.Height / 2.f;
}

void CUI_Notice::Update(void)
{
	if(!m_active)
		return;

	m_matworld._41 = m_vpos.x;
	m_matworld._42 = m_vpos.y;

	CFontMgr::GetInstance()->Setbatch_Font(m_message , m_vpos.x + 160 , m_vpos.y + 70);

	CFontMgr::GetInstance()->Setbatch_Font(L"O K" , m_vpos.x + 180 , m_vpos.y + 165);
}

void CUI_Notice::Render(void)
{
	if(!m_active)
		return;

	m_pSprite->SetTransform(&m_matworld);
	m_pSprite->Draw(m_texinfo->pTexture , NULL , &D3DXVECTOR3(0.f,0.f,0.f) , NULL , 
		m_color);
}

bool CUI_Notice::UI_ptinrect(const D3DXVECTOR2 vpos)
{
	if(!m_active)
		return false;

	m_rect.left = m_vpos.x + 105.f;
	m_rect.right = m_vpos.x + 260.f;
	m_rect.top = m_vpos.y + 160.f;
	m_rect.bottom = m_vpos.y + 190.f;

	//m_rect.left = m_matworld._41;
	//m_rect.right = m_matworld._41 + m_texinfo->ImgInfo.Width;
	//m_rect.top = m_matworld._42;
	//m_rect.bottom = m_matworld._42 + m_texinfo->ImgInfo.Height;

	if(MyPtInrect(vpos , &m_rect))
		return true;
	else
		return false;
}

void CUI_Notice::UI_Reaction(void)
{
	if(!m_active)
		return;

	m_active = false;
}

void CUI_Notice::set_message(const TCHAR* sz)
{
	lstrcpy(m_message , sz);
}

void CUI_Notice::Release(void)
{

}
