#include "StdAfx.h"
#include "UI_Energy_bar.h"

#include "Obj.h"
#include "ScrollMgr.h"
#include "TextureMgr.h"

CUI_Energy_bar::CUI_Energy_bar(CObj* pobj , float fwidth_size )
{
	m_pobj = pobj;
	m_fwidth_size = fwidth_size;
	m_fheight = m_pobj->GetVertex().bottom * 2 + 3 ;
}
CUI_Energy_bar::CUI_Energy_bar(CObj* pobj , float fwidth_size , float fheight)
{
	m_pobj = pobj;
	m_fwidth_size = fwidth_size;
	m_fheight = fheight;
}

CUI_Energy_bar::~CUI_Energy_bar(void)
{
}

void CUI_Energy_bar::Initialize(void)
{
	m_texinfo = CTextureMgr::GetInstance()->GetSingleTexture(L"UI" , L"ENERGY_BAR");		
	//m_vcenter.x = (float)(m_texinfo->ImgInfo.Width/2);
	//m_vcenter.y = (float)(m_texinfo->ImgInfo.Height/2);

	m_vcenter.x = 0;
	m_vcenter.y = 0;
	m_vcenter.z = 0;

	//m_fheight = m_pobj->GetVertex().bottom * 2 + 3 ;
	m_color = D3DCOLOR_ARGB(255,0,180,0);
	m_uipos = m_pobj->GetPosAdress();

	D3DXMatrixIdentity(&m_matback);
	D3DXMatrixIdentity(&m_matmp);

	m_active = false;
}

void CUI_Energy_bar::Update(void)
{
	if(!m_active)
		return;

	m_fmaxhp = float(m_pobj->GetUnitinfo().maxhp);
	m_fhp = float(m_pobj->GetUnitinfo().hp);
	float fratio = (m_fhp / m_fmaxhp);

	if( 0.33f < fratio && fratio <= 0.66f)
		m_color = D3DCOLOR_ARGB(255,255,255,0);
	else if(fratio <= 0.33f)
		m_color = D3DCOLOR_ARGB(255,200,0,0);
	else if( 0.66f < fratio)
		m_color = D3DCOLOR_ARGB(255,0,170,0);

	m_matworld._11 = (m_fhp / m_fmaxhp) * m_fwidth_size;
	m_matworld._41 = m_uipos->x - CScrollMgr::m_fScrollX - m_fwidth_size/2; // - ((fhp / fmaxhp) * m_fwidth_size) / 2;
	m_matworld._42 = m_uipos->y - CScrollMgr::m_fScrollY;
	m_matworld._42 += m_fheight;

	m_matback._11 = m_fwidth_size;
	m_matback._41 = m_matworld._41;
	m_matback._42 = m_matworld._42;

	m_fmaxmp = float(m_pobj->GetUnitinfo().maxmp);
	m_fmp = float(m_pobj->GetUnitinfo().mp);
	
	if( 0 < m_fmaxmp)
	{
		m_matmp._11 = (m_fmp / m_fmaxmp) * m_fwidth_size;
		m_matmp._41 = m_matworld._41;
		m_matmp._42 = m_matworld._42;
		m_matmp._42 += 6.f;
	}
}

void CUI_Energy_bar::Render(void)
{
	if(!m_active)
		return;

	//if(CScrollMgr::inside_camera(m_matworld._41 ,m_matworld._42))
	{
		m_pSprite->SetTransform(&m_matback);
		m_pSprite->Draw(m_texinfo->pTexture , NULL , &m_vcenter , NULL , D3DCOLOR_ARGB(255,200,200,200));

		m_pSprite->SetTransform(&m_matworld);
		m_pSprite->Draw(m_texinfo->pTexture , NULL , &m_vcenter , NULL , m_color);

		if( 0 < m_fmaxmp )
		{
			m_matback._42 = m_matmp._42;
			m_pSprite->SetTransform(&m_matback);
			m_pSprite->Draw(m_texinfo->pTexture , NULL , &m_vcenter , NULL , D3DCOLOR_ARGB(255,200,200,200));

			m_pSprite->SetTransform(&m_matmp);
			m_pSprite->Draw(m_texinfo->pTexture , NULL , &m_vcenter , NULL , D3DCOLOR_ARGB(255,200,21,200));
		}
	}
}

void CUI_Energy_bar::Release(void)
{

}
