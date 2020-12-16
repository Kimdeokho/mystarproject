#include "StdAfx.h"
#include "Com_MineAnim.h"

#include "TextureMgr.h"
#include "TimeMgr.h"
#include "Obj.h"
#include "Spidermine.h"

CCom_MineAnim::CCom_MineAnim(D3DXMATRIX& objmat)
:CCom_Animation(objmat)
{
	m_objname = L"MINE";
	m_statkey = L"";
}

CCom_MineAnim::~CCom_MineAnim(void)
{
}

void CCom_MineAnim::Initialize(CObj* pobj)
{
	SetAnimation(L"IDLE");
	m_pobj = pobj;
}

void CCom_MineAnim::Update(void)
{

	m_frame.fcurframe += GETTIME*m_frame.fframespeed;
	if( int(m_frame.fcurframe) >= m_frame.umax)
	{
		m_frame.fcurframe = 0;

		if(L"PLANT" == m_statkey)
		{
			SetAnimation(L"BURROW");
			m_pobj->SetState(BURROW);
			((CSpidermine*)m_pobj)->BurrowInit();
		}
		else if(L"PULL" == m_statkey)
		{
			SetAnimation(L"IDLE");
			m_pobj->SetState(IDLE);
		}
	}

	const vector<TEXINFO*> vtemp = *m_statetexture;

	if( (int)(m_frame.fcurframe) <= m_frame.umax)
		m_curtex = (vtemp)[int(m_frame.fcurframe)];
}

void CCom_MineAnim::Render(void)
{
	if(NULL == m_curtex)
		return;

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

void CCom_MineAnim::Release(void)
{

}

void CCom_MineAnim::SetAnimation(const TCHAR* statekey)
{
	if(m_statkey != statekey)
	{
		if(false == m_battack_end)
			return;

		m_statkey = statekey;
		m_frame.fcurframe = 0;	

		/* [i]는 방향 , DRONE, MOVE의 사진집합 */
		m_statetexture = CTextureMgr::GetInstance()->GetStateTexture_vecset(m_objname , m_statkey );

		if(NULL != m_statetexture)
		{
			m_frame.umax = m_statetexture->size();
			m_frame.fframespeed = (float)m_frame.umax;

			if(L"PLANT" == m_statkey || 
				L"PULL" == m_statkey || 
				L"MOVE" == m_statkey)
				m_frame.fframespeed *= 4;			
		}
	}
}
