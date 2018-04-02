#include "StdAfx.h"
#include "Com_VessleAnim.h"

#include "TextureMgr.h"
#include "TimeMgr.h"
#include "Obj.h"
CCom_VessleAnim::CCom_VessleAnim(D3DXMATRIX& objmat)
:CCom_Animation(objmat)
{
	m_objname = L"VESSLE";
	m_statkey = L"";
}

CCom_VessleAnim::~CCom_VessleAnim(void)
{
}

void CCom_VessleAnim::Initialize(CObj* pobj)
{
	m_pobj = pobj;
	tex_list = CTextureMgr::GetInstance()->GetGeneralTexture(L"VESSLE");

	m_curtex = ((*tex_list)[0]);
}

void CCom_VessleAnim::Update(void)
{

}

void CCom_VessleAnim::Render(void)
{
	m_objshadow_mat = m_objmat;
	m_objshadow_mat._42 += 38;

	m_pSprite->SetTransform(&m_objshadow_mat);
	m_pSprite->Draw(m_curtex->pTexture , NULL ,  &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0)
		, NULL , D3DCOLOR_ARGB(150,0,0,0));

	m_pSprite->SetTransform(&m_objmat);
	if(TEAM_1 == m_pobj->GetTeamNumber())
	{
		m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0)
			, NULL , D3DCOLOR_ARGB(255,255,0,0));
	}
	else
	{
		m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0)
			, NULL , D3DCOLOR_ARGB(255,255,255,255));
	}
}

void CCom_VessleAnim::Release(void)
{

}

void CCom_VessleAnim::SetAnimation(const TCHAR* statekey)
{

}
