#include "StdAfx.h"
#include "UI_Select.h"

#include "Device.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
CUI_Select::CUI_Select(const TCHAR* statekey , const D3DXMATRIX& matworld) :
m_objmat(matworld)
{
	m_statekey = statekey;
	m_bselect = false;
}

CUI_Select::~CUI_Select(void)
{
}

void CUI_Select::Initialize(void)
{
	m_texinfo = CTextureMgr::GetInstance()->GetSingleTexture(L"UI" , m_statekey);		
	m_vcenter.x = (float)(m_texinfo->ImgInfo.Width/2);
	m_vcenter.y = (float)(m_texinfo->ImgInfo.Height/2);
	m_vcenter.z = 0;

	m_color = D3DCOLOR_ARGB(255,0 , 255 , 255);
}

void CUI_Select::Update(void)
{
}

void CUI_Select::Render(void)
{
	if(true == m_bselect)
	{
		
		m_matworld = m_objmat;
		m_matworld._42 += 13;
		if(CScrollMgr::inside_camera(m_matworld._41 , m_matworld._42))
		{
			m_pSprite->SetTransform(&m_matworld);
			m_pSprite->Draw(m_texinfo->pTexture , NULL , &m_vcenter , NULL , m_color);
		}
	}
}

void CUI_Select::Release(void)
{

}
