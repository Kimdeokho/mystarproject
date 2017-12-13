#include "StdAfx.h"
#include "Com_BulletAnim.h"

#include "TextureMgr.h"
#include "TimeMgr.h"
CCom_BulletAnim::CCom_BulletAnim(D3DXMATRIX& objmat , const TCHAR* objname)
: CCom_Animation(objmat)
{
	m_objname = objname;
}

CCom_BulletAnim::~CCom_BulletAnim(void)
{
}

void CCom_BulletAnim::Initialize(CObj* pobj)
{
	m_pobj = pobj;

	m_generaltex = CTextureMgr::GetInstance()->GetGeneralTexture(m_objname);
	m_curtex = (*m_generaltex)[0];
	m_vcenter = D3DXVECTOR3( float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2) , 0 );

	m_frame.fcurframe = 0.f;
	m_frame.umax = m_generaltex->size();
	m_frame.fframespeed = float(m_frame.umax + 1);
}

void CCom_BulletAnim::Update(void)
{

	m_frame.fcurframe += GETTIME*m_frame.fframespeed*2;

	if(m_frame.fcurframe >= m_frame.umax)
	{
		m_frame.fcurframe = 0.f;
	}
	m_curtex = (*m_generaltex)[ int(m_frame.fcurframe) ];
}

void CCom_BulletAnim::Render(void)
{
	m_pSprite->SetTransform(&m_objmat);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &m_vcenter , NULL , D3DCOLOR_ARGB(255,255,255,255));
}

void CCom_BulletAnim::Release(void)
{

}

void CCom_BulletAnim::SetAnimation(const TCHAR* statekey)
{
}
