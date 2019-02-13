#include "StdAfx.h"
#include "GeneraEff.h"
#include "TextureMgr.h"
#include "TimeMgr.h"

#include "ScrollMgr.h"
CGeneraEff::CGeneraEff(void)
{
}

CGeneraEff::CGeneraEff(const TCHAR* texkey ,const D3DXVECTOR2& vpos ,const D3DXVECTOR2& vsize, SORT_ID esortid, const float ftimespeed
					   , D3DCOLOR	_color)
{
	m_texname = texkey;
	m_sortID = esortid;
	m_vPos = vpos;
	m_vsize = vsize;
	m_ftimespeed = ftimespeed;
	m_color = _color;
}

CGeneraEff::~CGeneraEff(void)
{
}

void CGeneraEff::Initialize(void)
{	

	m_generaltex = CTextureMgr::GetInstance()->GetGeneralTexture(m_texname);
	m_curtex = (*m_generaltex)[0];
	m_vcenter = D3DXVECTOR3( float(m_curtex->ImgInfo.Width)/2 , float(m_curtex->ImgInfo.Height)/2 , 0 );

	m_frame.fcurframe = 0.f;
	m_frame.umax = m_generaltex->size();
	m_frame.fframespeed = float(m_frame.umax + 1);
}

void CGeneraEff::Update(void)
{
	m_matWorld._11 = m_vsize.x;
	m_matWorld._22 = m_vsize.y;
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_frame.fcurframe += GETTIME*m_frame.fframespeed*m_ftimespeed;

	if(m_frame.fcurframe >= m_frame.umax)
	{
		m_frame.fcurframe = 0.f;
		m_bdestroy = true;
		return;
	}
	m_curtex = (*m_generaltex)[ int(m_frame.fcurframe) ];
}

void CGeneraEff::Render(void)
{
	m_pSprite->SetTransform(&m_matWorld);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &m_vcenter , NULL , m_color);
}

void CGeneraEff::Release(void)
{

}
