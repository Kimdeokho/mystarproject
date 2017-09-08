#include "StdAfx.h"
#include "Com_multitexture.h"

#include "Device.h"
#include "Com_Animation.h"
#include "Obj.h"
#include "MyMath.h"
#include "ScrollMgr.h"
//CCom_multitexture::CCom_multitexture(void)
//{
//}

CCom_multitexture::CCom_multitexture(const D3DXVECTOR2& vpos , D3DXMATRIX& matworld , CComponent* panim )
:m_vobjpos(vpos) , m_objmat(matworld) 
{
	m_anim = panim;
	m_pSprite = NULL;

}

CCom_multitexture::~CCom_multitexture(void)
{
}

void CCom_multitexture::Initialize(CObj* pobj)
{
	m_pobj = pobj;
	m_pSprite = CDevice::GetInstance()->GetSprite();

	m_anim->Initialize(m_pobj);

}

void CCom_multitexture::Update(void)
{

	m_anim->Update();
}

void CCom_multitexture::Render(void)
{
	m_pSprite->SetTransform(&m_objmat);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3( float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2) , 0) , NULL , D3DCOLOR_ARGB(255,255,255,255));
}

void CCom_multitexture::Release(void)
{

}

void CCom_multitexture::SetAnimation(const TCHAR* statekey)
{
	//m_anim->SetAnimation(statekey);
}


