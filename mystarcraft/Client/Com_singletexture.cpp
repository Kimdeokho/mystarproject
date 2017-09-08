#include "StdAfx.h"
#include "Com_singletexture.h"

#include "Device.h"
#include "TextureMgr.h"
CCom_singletexture::CCom_singletexture(const D3DXMATRIX& matobj, const TCHAR* kategoriekey , const TCHAR* texturekey)
:m_matobj(matobj)
{
	m_kategorie_key = kategoriekey;
	m_texture_key = texturekey;
}

CCom_singletexture::~CCom_singletexture(void)
{
}

void CCom_singletexture::Initialize(CObj* pobj)
{
	m_pobj = pobj;
	m_pSprite = CDevice::GetInstance()->GetSprite();

	m_curtex = CTextureMgr::GetInstance()->GetSingleTexture( L"UI" , L"Select32");
}

void CCom_singletexture::Update(void)
{

}

void CCom_singletexture::Render(void)
{
		m_pSprite->SetTransform(&m_matobj);
		m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3( (float)(m_curtex->ImgInfo.Width/2),(float)(m_curtex->ImgInfo.Height/2) , 0) , 
			NULL , D3DCOLOR_ARGB(255,0,255,255));
}

void CCom_singletexture::Release(void)
{

}
