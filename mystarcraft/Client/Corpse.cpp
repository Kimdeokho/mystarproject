#include "StdAfx.h"
#include "Corpse.h"

#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "TimeMgr.h"
#include "Device.h"
#include "MyMath.h"
CCorpse::CCorpse(const TCHAR* texname , const TCHAR* wreckagename)
{
	m_texname = texname;
	m_wreckage_name = wreckagename;
	m_pSprite = CDevice::GetInstance()->GetSprite();
	m_bwreckage = false;
}

CCorpse::~CCorpse(void)
{
}

void CCorpse::Initialize(void)
{
	if(L"" != m_texname)
	{
		m_generaltex = CTextureMgr::GetInstance()->GetGeneralTexture(m_texname);
		m_curtex = (*m_generaltex)[0];
		m_frame.umax = m_generaltex->size();
		m_frame.fframespeed = float(m_frame.umax + 1);
		m_vcenter = D3DXVECTOR3( float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2) , 0 );
	}
	else
	{
		m_frame.fframespeed = 1;
		m_frame.umax = 0;
	}

	m_frame.fcurframe = 0.f;

	m_sortID = SORT_CORPSE;
	
}

void CCorpse::Update(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	//m_duration += GETTIME;
	//if(m_duration >= 20.f)
	//{
	//	m_bdestroy = true;
	//	return;
	//}

	

	m_frame.fcurframe += GETTIME*m_frame.fframespeed;

	if(m_frame.fcurframe >= m_frame.umax )
	{
		if(false == m_bwreckage)
		{
			m_bwreckage = true;
			m_generaltex = CTextureMgr::GetInstance()->GetGeneralTexture(m_wreckage_name);
			m_curtex = (*m_generaltex)[0];

			m_frame.umax = m_generaltex->size();
			m_frame.fframespeed = 0.08f;	
			m_vcenter = D3DXVECTOR3( float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2) , 0 );
		}
		else
		{
			m_bdestroy = true;
		}

		m_frame.fcurframe = 0;
		//return;
	}
	//else
	//{
	//	m_frame.fcurframe += GETTIME*m_frame.fframespeed;
	//}
	m_curtex = (*m_generaltex)[ int(m_frame.fcurframe) ];
}

void CCorpse::Render(void)
{
	//if(CScrollMgr::inside_camera(m_matWorld._41 , m_matWorld._42))
	{
		m_pSprite->SetTransform(&m_matWorld);
		m_pSprite->Draw(m_curtex->pTexture , NULL , &m_vcenter , NULL , D3DCOLOR_ARGB(255,255,255,255));
	}
}

void CCorpse::Release(void)
{

}
