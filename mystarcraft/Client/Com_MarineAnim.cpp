#include "StdAfx.h"
#include "Com_MarineAnim.h"

#include "TextureMgr.h"
#include "TimeMgr.h"
#include "Obj.h"

CCom_MarineAnim::CCom_MarineAnim(D3DXMATRIX& objmat)
:CCom_Animation(objmat)
{
	m_objname = L"MARINE";
	m_statkey = L"";
	//memset(m_statkey , 0 , sizeof(TCHAR)*64);
	m_tempkey = L"";
	m_attackloop = 0;
}

CCom_MarineAnim::~CCom_MarineAnim(void)
{
}

void CCom_MarineAnim::Initialize(CObj* pobj)
{
	m_battack_end = true;

	SetAnimation(L"IDLE");
	m_pobj = pobj;
	m_rotation_speed = 40;

	CCom_Animation::InitTexidx();

}

void CCom_MarineAnim::SetAnimation(const TCHAR* statekey)
{
	if(m_statkey != statekey)
	{
		if(false == m_battack_end)
			return;

		m_statkey = statekey;
		m_frame.fcurframe = 0;			

		for(int i = 0; i < DIR_CNT; ++i)
		{
			/* [i]는 방향 , DRONE, MOVE의 사진집합 */
			m_animtexture[i] = CTextureMgr::GetInstance()->GetTUnitTexture(m_objname , m_statkey , i);
			if(NULL == m_animtexture[i])
				break;
		}

		if(NULL != m_animtexture[0])
		{
			m_frame.umax = m_animtexture[0]->size();
			m_frame.fframespeed = (float)m_frame.umax;
			if(L"ATTACK" == m_statkey)
				m_frame.fframespeed *= 8;
			if(L"MOVE" == m_statkey)
				m_frame.fframespeed *= 2;
		}
	}
}

void CCom_MarineAnim::Update(void)
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


	if(NULL !=  m_animtexture[m_texdiridx] )
	{
		const vector<TEXINFO*>* vtemp = m_animtexture[m_texdiridx];

		if( (int)(m_frame.fcurframe) <= m_frame.umax)
			m_curtex = (*vtemp)[int(m_frame.fcurframe)];
	}
	else
		m_curtex = NULL;

}

void CCom_MarineAnim::Render(void)
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

void CCom_MarineAnim::Release(void)
{

}


