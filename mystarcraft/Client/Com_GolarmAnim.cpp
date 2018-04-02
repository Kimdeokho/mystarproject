#include "StdAfx.h"
#include "Com_GolarmAnim.h"

#include "TextureMgr.h"
#include "TimeMgr.h"
#include "Obj.h"

CCom_GolarmAnim::CCom_GolarmAnim(D3DXMATRIX& objmat)
:CCom_Animation(objmat)
{
	m_objname = L"GOLIATH_ARM";
	m_statkey = L"";
	m_attackloop = 0;
}

CCom_GolarmAnim::~CCom_GolarmAnim(void)
{
}

void CCom_GolarmAnim::Initialize(CObj* pobj)
{
	m_battack_end = true;

	SetAnimation(L"IDLE");
	m_pobj = pobj;
	m_rotation_speed = 60;

	CCom_Animation::InitTexidx();
}
void CCom_GolarmAnim::SetAnimation(const TCHAR* statekey)
{
	if(m_statkey != statekey)
	{
		if(false == m_battack_end)
			return;

		m_statkey = statekey;
		m_frame.fcurframe = 0;	

		/* [i]는 방향 , DRONE, MOVE의 사진집합 */
		m_animtexture = CTextureMgr::GetInstance()->GetTUnitTexture(m_objname , m_statkey );


		if(NULL != m_animtexture)
		{
			m_frame.umax = m_animtexture[0].size();
			m_frame.fframespeed = (float)m_frame.umax;

			if(L"ATTACK" == m_statkey)
				m_frame.fframespeed *= 8;
			if(L"MOVE" == m_statkey)
				m_frame.fframespeed *= 2;

			m_curtex = m_animtexture[m_texdiridx][ int(m_frame.fcurframe) ];
		}
	}
}
void CCom_GolarmAnim::Update(void)
{
	CCom_Animation::DirIdxCalculation();

	// ATTACK일경우 무기가 발사준비 완료일때 재생시킨다

	m_frame.fcurframe += GETTIME*m_frame.fframespeed;
	if( int(m_frame.fcurframe) >= m_frame.umax)
	{

		if( L"ATTACK" == m_statkey)
		{
			++m_attackloop;

			if(m_attackloop >= 3)
			{
				m_battack_end = true;
				m_attackloop = 0;
				m_pobj->SetState(IDLE);
			}
		}
		m_frame.fcurframe = 0;
	}
	else
	{
		if( L"ATTACK" == m_statkey)
			m_battack_end = false;
		else
			m_battack_end = true;
	}

	if( (int)(m_frame.fcurframe) <= m_frame.umax)
		m_curtex = m_animtexture[m_texdiridx][ int(m_frame.fcurframe) ];
}

void CCom_GolarmAnim::Render(void)
{
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

void CCom_GolarmAnim::Release(void)
{

}


