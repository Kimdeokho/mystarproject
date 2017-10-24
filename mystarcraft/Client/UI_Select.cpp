#include "StdAfx.h"
#include "UI_Select.h"

#include "Device.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "TimeMgr.h"

CUI_Select::CUI_Select(const TCHAR* statekey , const D3DXVECTOR2& objpos , float fweightY) :
m_objpos(objpos)
{
	m_statekey = statekey;
	m_brender = false;
	m_fweightY = fweightY;
}

CUI_Select::~CUI_Select(void)
{
}

void CUI_Select::Initialize(void)
{
	m_eflag = NONE_SELECT;
	m_texinfo = CTextureMgr::GetInstance()->GetSingleTexture(L"UI" , m_statekey);		
	m_vcenter.x = (float)(m_texinfo->ImgInfo.Width/2);
	m_vcenter.y = (float)(m_texinfo->ImgInfo.Height/2);
	m_vcenter.z = 0;

	m_color = D3DCOLOR_ARGB(255,0 , 255 , 255); //이게 초록이던디..

	m_blinkcnt = 3;
	m_blinktime = 0.f;
	m_blinkend_time = 2.f;

	m_blink_interval = m_blinkend_time/(m_blinkcnt*2);
	m_blink_on = true;
	m_interval_time = 0.f;
}

void CUI_Select::Update(void)
{
	if(GENERAL_SELECT == m_eflag)
	{
		m_brender = true;
	}
	else if(NONE_SELECT == m_eflag)
	{
		m_brender = false;
	}
	else if(BLINK_SELECT == m_eflag)
	{
		m_blinktime += GETTIME;

		if( m_blinktime > m_blinkend_time)
		{
			m_eflag = NONE_SELECT;
			m_blinktime = 0.f;
			m_interval_time = 0.f;
			m_blink_on = true;
		}
		else
		{
			m_interval_time += GETTIME;
			if(m_interval_time > m_blink_interval)
			{
				if(true == m_blink_on)
					m_blink_on = false;
				else
					m_blink_on = true;

				m_interval_time = 0.f;
			}

			if(true == m_blink_on)
				m_brender = true;
			else
				m_brender = false;
		}
	}

	m_matworld._41 = m_objpos.x - CScrollMgr::m_fScrollX;
	m_matworld._42 = m_objpos.y - CScrollMgr::m_fScrollY;
}

void CUI_Select::Render(void)
{
	if(true == m_brender)
	{		
		m_matworld._42 += m_fweightY;
		
		if(CScrollMgr::inside_camera(m_matworld._41 ,m_matworld._42))
		{
			m_pSprite->SetTransform(&m_matworld);
			m_pSprite->Draw(m_texinfo->pTexture , NULL , &m_vcenter , NULL , m_color);
		}
	}
}
void CUI_Select::SetSelect( SELECT_FLAG eflag ,D3DCOLOR ecolor)
{
	m_eflag = eflag;
	m_color = ecolor;
}
void CUI_Select::Release(void)
{

}
