#include "StdAfx.h"
#include "LoopEff.h"

#include "TextureMgr.h"
#include "TimeMgr.h"

#include "ScrollMgr.h"
CLoopEff::CLoopEff(const TCHAR* texkey ,const D3DXVECTOR2& vpos ,const D3DXVECTOR2& vsize, SORT_ID esortid, const float& ftimespeed)
{
	m_texname = texkey;
	m_sortID = esortid;
	m_vPos = vpos;
	m_vsize = vsize;
	m_ftimespeed = ftimespeed;
}

CLoopEff::~CLoopEff(void)
{
}

void CLoopEff::Initialize(void)
{
	m_generaltex = CTextureMgr::GetInstance()->GetGeneralTexture(m_texname);
	m_curtex = (*m_generaltex)[0];
	m_vcenter = D3DXVECTOR3( float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2) , 0 );

	m_frame.fcurframe = 0.f;
	m_frame.umax = m_generaltex->size();
	m_frame.fframespeed = float(m_frame.umax + 1);

	m_matWorld._11 = m_vsize.x;
	m_matWorld._22 = m_vsize.y;
}

void CLoopEff::Update(void)
{
	m_frame.fcurframe += GETTIME*m_frame.fframespeed*m_ftimespeed;

	if(m_frame.fcurframe >= m_frame.umax)
	{
		m_frame.fcurframe = 0.f;
	}
	m_curtex = (*m_generaltex)[ int(m_frame.fcurframe) ];
}

void CLoopEff::Render(void)
{

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	//if(CScrollMgr::inside_camera(m_matWorld._41 , m_matWorld._42))
	{
		m_pSprite->SetTransform(&m_matWorld);
		m_pSprite->Draw(m_curtex->pTexture , NULL , &m_vcenter , NULL , D3DCOLOR_ARGB(255,255,255,255));
	}
}

void CLoopEff::Release(void)
{

}

void CLoopEff::SetTexname(const TCHAR* texkey)
{
	if(m_texname != texkey)
	{
		m_texname = texkey;

		m_generaltex = CTextureMgr::GetInstance()->GetGeneralTexture(texkey);
		m_curtex = (*m_generaltex)[0];
		m_vcenter = D3DXVECTOR3( float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2) , 0 );

		m_frame.fcurframe = 0.f;
		m_frame.umax = m_generaltex->size();
		m_frame.fframespeed = float(m_frame.umax + 1);
	}
}
