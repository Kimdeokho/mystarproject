#include "StdAfx.h"
#include "Com_multitexture.h"

#include "Device.h"
#include "Animation.h"
#include "Obj.h"
#include "MyMath.h"
//CCom_multitexture::CCom_multitexture(void)
//{
//}

CCom_multitexture::CCom_multitexture(const D3DXVECTOR2& vpos , D3DXMATRIX& matworld , CAnimation* panim)
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
	m_anim->Setcurtex_adress(&m_curtex);
	m_pSprite = CDevice::GetInstance()->GetSprite();
}

void CCom_multitexture::Update(void)
{
	float       fdot , fdgree;
	int			diridx;
	D3DXVECTOR2	curdir = m_pobj->GetcurDir(); 

	fdot = D3DXVec2Dot(&curdir , &OFFSET_DIRVEC);

	fdgree = CMyMath::scala_to_dgree(fdot);

	if(m_pobj->GetcurDir().x < 0)
		fdgree = 360 - fdgree;

	diridx = (int)( (fdgree/22.5f) + 0.5f);


	if(diridx > 8)
	{
		diridx = 16 - diridx;
		m_objmat._11 = -1;
	}
	else
		m_objmat._11 = 1;

	m_anim->Setdir(diridx);
}

void CCom_multitexture::Render(void)
{
	m_pSprite->SetTransform(&m_objmat);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3( float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2) , 0) , NULL , D3DCOLOR_ARGB(255,255,255,255));
}

void CCom_multitexture::Release(void)
{

}


