#include "StdAfx.h"
#include "Com_SCVAnim.h"

#include "TimeMgr.h"
#include "TextureMgr.h"
#include "Obj.h"

CCom_SCVAnim::CCom_SCVAnim(D3DXMATRIX& objmat )
:CCom_Animation(objmat )
{
	m_objname = L"SCV";
	m_statkey = L"";
}

CCom_SCVAnim::~CCom_SCVAnim(void)
{
}

void CCom_SCVAnim::Initialize(CObj* pobj)
{
	m_battack_sync = false;
	SetAnimation(L"IDLE");

	m_pobj = pobj;
	CCom_Animation::InitTexidx();

	m_rotation_speed = 80.f;
}

void CCom_SCVAnim::Update(void)
{
	CCom_Animation::DirIdxCalculation();
	

	m_frame.fcurframe += GETTIME*m_frame.fframespeed;
	if(m_frame.fcurframe >= m_frame.umax)
	{
		if(L"ATTACK" == m_statkey)
		{
			m_frame.fcurframe = 1;
			m_battack_end = true;
		}
		else
			m_frame.fcurframe = 0;
	}
	else
	{
		if( L"ATTACK" == m_statkey)
			m_battack_end = false;
		else
			m_battack_end = true;
	}


	if(L"ATTACK" == m_statkey)
	{
		if( 1 == int(m_frame.fcurframe) )
		{
			m_battack_sync = true;
		}
		else
			m_battack_sync = false;
	}

	if( (int)(m_frame.fcurframe) <= m_frame.umax)
		m_curtex = m_animtexture[m_texdiridx][ int(m_frame.fcurframe) ];
}

void CCom_SCVAnim::Render(void)
{
	if(NULL == m_curtex)
		return;

	m_objshadow_mat = m_objmat;
	m_objshadow_mat._42 += 8;

	m_pSprite->SetTransform(&m_objshadow_mat);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3( 36.f , 36.f , 0)
		, NULL , D3DCOLOR_ARGB(150,0,0,0));


	m_pSprite->SetTransform(&m_objmat);
	if(TEAM_1 == m_pobj->GetTeamNumber())
	{
		m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3( 36.f , 36.f , 0)
			, NULL , D3DCOLOR_ARGB(255,255,0,0));
	}
	else
	{
		m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3( 36.f , 36.f , 0)
			, NULL , D3DCOLOR_ARGB(255,255,255,255));
	}
}


void CCom_SCVAnim::SetAnimation(const TCHAR* statekey)
{
	if(m_statkey != statekey)/*새로 들어오는 상태가 기존들어오는 상태와 다른상태여야 한다.*/
	{
		if(false == m_battack_end)
			return;

		m_statkey = statekey;
		m_frame.fcurframe = 0;


			/* [i]는 방향 , DRONE, MOVE의 사진집합 */
		m_animtexture = CTextureMgr::GetInstance()->GetTUnitTexture(m_objname , statekey );

		if(NULL != m_animtexture)
		{
			m_frame.umax = m_animtexture[0].size();
			m_frame.fframespeed = (float)m_frame.umax;

			if(L"ATTACK" == m_statkey)
				m_frame.fframespeed = (float)m_frame.umax*3.5f;

			m_curtex = m_animtexture[m_texdiridx][ int(m_frame.fcurframe) ];
		}
	}
}
void CCom_SCVAnim::Release(void)
{

}
