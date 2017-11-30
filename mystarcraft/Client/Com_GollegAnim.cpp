#include "StdAfx.h"
#include "Com_GollegAnim.h"

#include "TextureMgr.h"
#include "TimeMgr.h"
#include "Obj.h"

CCom_GollegAnim::CCom_GollegAnim(D3DXMATRIX& objmat)
:CCom_Animation(objmat)
{
	m_objname = L"GOLIATH_LEG";
	m_statkey = L"";
	//memset(m_statkey , 0 , sizeof(TCHAR)*64);
}

CCom_GollegAnim::~CCom_GollegAnim(void)
{
}

void CCom_GollegAnim::Initialize(CObj* pobj)
{
	m_battack_end = true;

	SetAnimation(L"IDLE");
	m_pobj = pobj;
	m_rotation_speed = 60;

	CCom_Animation::InitTexidx();
}
void CCom_GollegAnim::SetAnimation(const TCHAR* statekey)
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

			if(L"MOVE" == m_statkey)
			{
				m_frame.fframespeed *= 1.4f;
			}
		}
	}
}

void CCom_GollegAnim::Update(void)
{
	CCom_Animation::DirIdxCalculation();

	// ATTACK일경우 무기가 발사준비 완료일때 재생시킨다

	m_frame.fcurframe += GETTIME*m_frame.fframespeed;
	if( int(m_frame.fcurframe) >= m_frame.umax)
	{
		m_frame.fcurframe = 0;
	}

	const vector<TEXINFO*> vtemp = m_animtexture[m_texdiridx];

	if( (int)(m_frame.fcurframe) <= m_frame.umax)
		m_curtex = (vtemp)[int(m_frame.fcurframe)];
}

void CCom_GollegAnim::Render(void)
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

void CCom_GollegAnim::Release(void)
{

}

