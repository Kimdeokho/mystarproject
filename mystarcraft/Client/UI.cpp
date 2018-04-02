#include "StdAfx.h"
#include "UI.h"

#include "Device.h"
CUI::CUI(void)
{
	m_pSprite = CDevice::GetInstance()->GetSprite();
	D3DXMatrixIdentity(&m_matworld);
	m_color = D3DCOLOR_ARGB(255,255,255,255);
	m_bdestroy = false;
}


CUI::~CUI(void)
{
}

void CUI::Initialize(void)
{

}

void CUI::Update(void)
{

}

void CUI::Render(void)
{

}

void CUI::Release(void)
{

}
void CUI::SetDestroy(bool bdestroy )
{
	m_bdestroy = bdestroy;
}

bool CUI::GetDestroy(void)
{
	return m_bdestroy;
}

const MYRECT<float>& CUI::GetMyRect(void)
{
	return m_rect;
}

void CUI::SetPos(const D3DXVECTOR2& vpos)
{
	m_vpos = vpos;
}

const D3DXVECTOR2& CUI::GetPos(void)
{
	return m_vpos;
}

void CUI::SetActive(bool _active)
{
	m_active = _active;
}
