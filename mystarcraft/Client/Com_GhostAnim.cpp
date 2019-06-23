#include "StdAfx.h"
#include "Com_GhostAnim.h"

#include "TextureMgr.h"
#include "TimeMgr.h"
#include "Obj.h"
CCom_GhostAnim::CCom_GhostAnim(D3DXMATRIX& objmat)
:CCom_Animation(objmat)
{
	m_objname = L"GHOST";
	m_statkey = L"";
}

CCom_GhostAnim::~CCom_GhostAnim(void)
{
}

void CCom_GhostAnim::Initialize(CObj* pobj)
{
	m_battack_end = true;

	SetAnimation(L"IDLE");
	m_pobj = pobj;
	m_rotation_speed = 90;

	CCom_Animation::InitTexidx();

	m_isclocikng = false;
	m_discolor_time = 0.f;
}

void CCom_GhostAnim::SetAnimation(const TCHAR* statekey)
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
				m_frame.fframespeed *= 1.5f;
			else if(L"ATTACK" == m_statkey)
			{				
				m_frame.fframespeed *= 8.f;
			}

			m_curtex = m_animtexture[m_texdiridx][ int(m_frame.fcurframe) ];
		}
	}
}

void CCom_GhostAnim::Update(void)
{
	CCom_Animation::DirIdxCalculation();

	m_frame.fcurframe += GETTIME*m_frame.fframespeed;
	if( int(m_frame.fcurframe) >= m_frame.umax)
	{
		m_frame.fcurframe = 0;
		if(L"ATTACK" == m_statkey)
			m_battack_end = true;
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



	if(m_isclocikng)
	{
		m_discolor_time += GETTIME;
		if(m_discolor_time > 0.07f)
		{
			m_discolor_time = 0.f;
			m_alpha -= 8;

			if(m_alpha <= 80)
			{
				m_alpha = 80;
			}
		}
		m_color = D3DCOLOR_ARGB(m_alpha , 255,255,255);
	}
	else
	{
		if(m_alpha < 255)
		{
			m_discolor_time += GETTIME;
			if(m_discolor_time > 0.07f)
			{
				m_discolor_time = 0.f;
				m_alpha += 8;

				if(m_alpha >= 255)
					m_alpha = 255;
			}

			m_color = D3DCOLOR_ARGB(m_alpha , 255,255,255);
		}
	}
}

void CCom_GhostAnim::Render(void)
{
	m_pSprite->SetTransform(&m_objmat);
	if(TEAM_1 == m_pobj->GetTeamNumber())
	{
		m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0)
			, NULL , m_color);
	}
	else
	{
		m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0)
			, NULL , m_color);
	}
}

void CCom_GhostAnim::Release(void)
{

}
void CCom_GhostAnim::Clocking_on(void)
{
	m_isclocikng = true;
}

void CCom_GhostAnim::Clocking_off(void)
{
	m_isclocikng = false;
}

