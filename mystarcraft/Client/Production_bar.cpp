#include "StdAfx.h"
#include "Production_bar.h"

#include "TextureMgr.h"
CProduction_bar::CProduction_bar(void)
{
}

CProduction_bar::~CProduction_bar(void)
{
}

void CProduction_bar::Initialize(void)
{
	//진행막대
	m_texinfo = CTextureMgr::GetInstance()->GetSingleTexture(L"UI" , L"PRODUCTION_BAR0");

	//백그라운드 막대
	m_backbar = CTextureMgr::GetInstance()->GetSingleTexture(L"UI" , L"PRODUCTION_BAR1");

	m_fratioX = 0.f;
	m_is_active = false;
}

void CProduction_bar::Update(void)
{

}

void CProduction_bar::Render(void)
{
	//97은 생산바 텍스쳐 막대크키

	//백그라운드 막대
	if(true == m_is_active)
	{
		//m_matworld._11 = 1;
		//m_matworld._41 = m_vpos.x;
		//m_matworld._42 = m_vpos.y;
		//m_pSprite->SetTransform(&m_matworld);
		//m_pSprite->Draw(m_backbar->pTexture , NULL , &D3DXVECTOR3(50.5 , 4.5 , 0) , NULL , D3DCOLOR_ARGB(255,255,255,255));

		//m_matworld._11 = 1 - m_fratioX;
		//m_matworld._41 = m_vpos.x + (97 * m_fratioX)/2;
		//m_matworld._42 = m_vpos.y;
		//m_pSprite->SetTransform(&m_matworld);
		//m_pSprite->Draw(m_texinfo->pTexture , NULL , &D3DXVECTOR3(48.5 , 2.5 , 0) , NULL , D3DCOLOR_ARGB(255,255,255,255));

		m_matworld._11 = 1;
		m_matworld._41 = m_vpos.x;
		m_matworld._42 = m_vpos.y;
		m_pSprite->SetTransform(&m_matworld);
		m_pSprite->Draw(m_backbar->pTexture , NULL , &D3DXVECTOR3(0 , 0 , 0) , NULL , D3DCOLOR_ARGB(255,255,255,255));

		m_matworld._11 = 1 - m_fratioX;
		m_matworld._41 = m_vpos.x + 1.5f + (97 * m_fratioX);
		m_matworld._42 = m_vpos.y + 2.f;
		m_pSprite->SetTransform(&m_matworld);
		m_pSprite->Draw(m_texinfo->pTexture , NULL , &D3DXVECTOR3(0 , 0 , 0) , NULL , D3DCOLOR_ARGB(255,255,255,255));
	}
}
void CProduction_bar::SetProduction_info(const D3DXVECTOR2& vpos , const float& ratioX)
{
	if(false == m_is_active)
		m_is_active =  true;

	m_fratioX = ratioX;
	m_vpos = vpos;

	if(m_fratioX >= 1.f)
		m_fratioX = 1.f;
}
void CProduction_bar::SetActive(bool is_active)
{
	m_is_active = is_active;
}
void CProduction_bar::Release(void)
{

}
