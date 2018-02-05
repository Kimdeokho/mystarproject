#include "StdAfx.h"
#include "Cmd_btn.h"

#include "TextureMgr.h"
CCmd_btn::CCmd_btn(const TCHAR* texkey, CMD_BTN ebtn , const D3DXVECTOR2& vpos ,bool is_active)
{
	m_texkey = texkey;
	m_btn_id = ebtn;
	m_vpos = vpos;
	m_color = D3DCOLOR_ARGB(255,255,255,255);
	m_is_active = is_active;

	m_texinfo = NULL;

	m_rect.left = m_vpos.x - 18;
	m_rect.right = m_vpos.x + 18;
	m_rect.top = m_vpos.y - 17;
	m_rect.bottom = m_vpos.y + 17;
}

CCmd_btn::~CCmd_btn(void)
{
}

void CCmd_btn::Initialize(void)
{
}

void CCmd_btn::Update(void)
{

}

void CCmd_btn::Render(void)
{
	if(NULL == m_texinfo)
		return;

	if(true == m_is_active)
		m_color = D3DCOLOR_ARGB(255,255,255,255);
	else
		m_color = D3DCOLOR_ARGB(255,255,0,0);

	m_pSprite->SetTransform(&m_matworld);
	m_pSprite->Draw(m_texinfo->pTexture , NULL , &m_vcenter , NULL , m_color);
}

void CCmd_btn::Release(void)
{

}

CMD_BTN CCmd_btn::GetCmdbtn_id(void)
{
	return m_btn_id;
}

void CCmd_btn::Init_btn(const TCHAR* texkey , CMD_BTN ebtn ,const D3DXVECTOR2& vpos, bool is_ative)
{
	m_texkey = texkey;
	m_vpos = vpos;
	m_matworld._41 = m_vpos.x;
	m_matworld._42 = m_vpos.y;

	m_texinfo = CTextureMgr::GetInstance()->GetSingleTexture(L"UI" , m_texkey);
	m_vcenter = D3DXVECTOR3( 18 , 17 , 0);
	m_btn_id = ebtn;
	m_is_active = is_ative;

}
