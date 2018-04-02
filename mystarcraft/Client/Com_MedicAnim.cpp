#include "StdAfx.h"
#include "Com_MedicAnim.h"

#include "TextureMgr.h"
#include "TimeMgr.h"
#include "Obj.h"
CCom_MedicAnim::CCom_MedicAnim(D3DXMATRIX& objmat)
:CCom_Animation(objmat)
{
	m_objname = L"MEDIC";
	m_statkey = L"";
}

CCom_MedicAnim::~CCom_MedicAnim(void)
{
}

void CCom_MedicAnim::Initialize(CObj* pobj)
{
	m_battack_end = true;

	SetAnimation(L"IDLE");
	m_pobj = pobj;
	m_rotation_speed = 90;

	CCom_Animation::InitTexidx();
}
void CCom_MedicAnim::SetAnimation(const TCHAR* statekey)
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
				m_frame.fframespeed *= 2;
			else if(L"HEAL" == m_statkey)
				m_frame.fframespeed *= 7;

			m_curtex = m_animtexture[m_texdiridx][ int(m_frame.fcurframe) ];
		}
	}
}

void CCom_MedicAnim::Update(void)
{
	CCom_Animation::DirIdxCalculation();

	// ATTACK일경우 무기가 발사준비 완료일때 재생시킨다


	m_frame.fcurframe += GETTIME*m_frame.fframespeed;
	if( int(m_frame.fcurframe) >= m_frame.umax)
	{
		if(L"HEAL" == m_statkey)
			m_battack_end = true;

		m_frame.fcurframe = 0;
	}
	else
	{
		if(L"HEAL" == m_statkey)
			m_battack_end = false;
	}

	//const vector<TEXINFO*> vtemp = m_animtexture[m_texdiridx];

	//if( (int)(m_frame.fcurframe) <= m_frame.umax)
	//	m_curtex = (vtemp)[int(m_frame.fcurframe)];

	if( (int)(m_frame.fcurframe) <= m_frame.umax)
		m_curtex = m_animtexture[m_texdiridx][ int(m_frame.fcurframe) ];
}

void CCom_MedicAnim::Render(void)
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

void CCom_MedicAnim::Release(void)
{

}

