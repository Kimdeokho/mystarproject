#include "StdAfx.h"
#include "MultiEff.h"

#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "TimeMgr.h"
CMultiEff::CMultiEff(const TCHAR* texkey , const int& curdiridx ,const float& framespped ,const int& loopcnt)
{
	m_texkey = texkey;
	m_curdiridx = curdiridx;
	m_ftimespeed = framespped;
	m_end_loopcnt = loopcnt;
}

CMultiEff::~CMultiEff(void)
{
}

void CMultiEff::Initialize(void)
{
	m_cur_loopcnt = 0;
	m_curtex = NULL;
	m_sortID = SORT_GROUND_EFF;
	//for(int i = 0; i < DIR_CNT; ++i)
	//{
	//	m_multiefftex[i] = CTextureMgr::GetInstance()->GetMultiEffTexture(m_texkey , i);
	//}

	if(m_curdiridx > 8)
	{
		m_curdiridx = 16 - m_curdiridx;
		m_matWorld._11 = -1;
	}
	else
	{
		m_matWorld._11 = 1;
	}
	m_multiefftex = CTextureMgr::GetInstance()->GetMultiEffTexture(m_texkey , m_curdiridx);

	m_curtex = (*m_multiefftex)[0];

	m_vcenter = D3DXVECTOR3( float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2) , 0);

	m_frame.fcurframe = 0.f;
	m_frame.umax = m_multiefftex->size();
	m_frame.fframespeed = float(m_frame.umax + 1) * m_ftimespeed;
}

void CMultiEff::Update(void)
{
	m_frame.fcurframe += GETTIME*m_frame.fframespeed;

	if(m_frame.fcurframe >= m_frame.umax)
	{
		m_frame.fcurframe = 0.f;
		++m_cur_loopcnt;

		if(m_cur_loopcnt >= m_end_loopcnt)
		{
			m_bdestroy = true;
			return;
		}
	}
	m_curtex = (*m_multiefftex)[ int(m_frame.fcurframe) ];
}

void CMultiEff::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_pSprite->SetTransform(&m_matWorld);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &m_vcenter , NULL , D3DCOLOR_ARGB(255,255,255,255));

}

void CMultiEff::Release(void)
{

}
