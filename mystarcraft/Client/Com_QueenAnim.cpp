#include "StdAfx.h"
#include "Com_QueenAnim.h"

#include "TextureMgr.h"
#include "TimeMgr.h"

CCom_QueenAnim::CCom_QueenAnim(D3DXMATRIX& objmat)
:CCom_Animation(objmat)
{
	m_objname = L"QUEEN";
	m_statkey = L"";
}

CCom_QueenAnim::~CCom_QueenAnim(void)
{
}

void CCom_QueenAnim::Initialize(CObj* pobj)
{
	m_battack_end = true;

	SetAnimation(L"IDLE");
	m_pobj = pobj;
	m_rotation_speed = 80;

	CCom_Animation::InitTexidx();
}

void CCom_QueenAnim::Update(void)
{
	CCom_Animation::DirIdxCalculation();

	m_frame.fcurframe += GETTIME*m_frame.fframespeed;
	if( int(m_frame.fcurframe) >= m_frame.umax)
	{
		m_frame.fcurframe = 0;
	}

	if( (int)(m_frame.fcurframe) <= m_frame.umax)
		m_curtex = m_animtexture[m_texdiridx][ int(m_frame.fcurframe) ];
}

void CCom_QueenAnim::Render(void)
{
	m_objshadow_mat = m_objmat;
	m_objshadow_mat._42 += 38;

	m_pSprite->SetTransform(&m_objshadow_mat);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &m_vcenter
		, NULL , D3DCOLOR_ARGB(150,0,0,0));

	m_pSprite->SetTransform(&m_objmat);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &m_vcenter
		, NULL , D3DCOLOR_ARGB(255,255,255,255));
}

void CCom_QueenAnim::SetAnimation(const TCHAR* statekey)
{
	if(m_statkey != statekey)
	{
		if(false == m_battack_end)
			return;

		m_statkey = statekey;
		m_frame.fcurframe = 0;	

		/* [i]´Â ¹æÇâ */
		m_animtexture = CTextureMgr::GetInstance()->GetZUnitTexture(m_objname , m_statkey );

		if(NULL != m_animtexture)
		{
			m_frame.umax = m_animtexture[0].size();
			m_frame.fframespeed = (float)m_frame.umax * 2;

			m_curtex = m_animtexture[m_texdiridx][ int(m_frame.fcurframe) ];
			m_vcenter = D3DXVECTOR3( float(m_curtex->ImgInfo.Width / 2) , float(m_curtex->ImgInfo.Height / 2) , 0);
		}
	}
}

void CCom_QueenAnim::Release(void)
{

}
