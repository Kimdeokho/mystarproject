#include "StdAfx.h"
#include "Com_ThornAnim.h"

#include "TextureMgr.h"
#include "TimeMgr.h"

#include "Obj.h"
CCom_ThornAnim::CCom_ThornAnim(D3DXMATRIX& objmat , const TCHAR* objname)
:CCom_Animation(objmat)
{
	m_objname = objname;
}

CCom_ThornAnim::~CCom_ThornAnim(void)
{
}

void CCom_ThornAnim::Initialize(CObj* pobj)
{
	m_pobj = pobj;

	m_generaltex = CTextureMgr::GetInstance()->GetGeneralTexture(m_objname);
	m_curtex = (*m_generaltex)[0];
	m_vcenter = D3DXVECTOR3( float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2) , 0 );

	m_frame.fcurframe = 0.f;
	m_frame.umax = m_generaltex->size();
	m_frame.fframespeed = float(m_frame.umax );
}

void CCom_ThornAnim::Update(void)
{
	m_frame.fcurframe += GETTIME*m_frame.fframespeed*1.4f;

	if(m_frame.fcurframe >= m_frame.umax)
	{
		m_frame.fcurframe = 0.f;
		m_pobj->SetDestroy(true);
	}
	m_curtex = (*m_generaltex)[ int(m_frame.fcurframe) ];
}

void CCom_ThornAnim::Render(void)
{
	m_pSprite->SetTransform(&m_objmat);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &m_vcenter , NULL , D3DCOLOR_ARGB(255,255,255,255));
}

void CCom_ThornAnim::Release(void)
{

}
