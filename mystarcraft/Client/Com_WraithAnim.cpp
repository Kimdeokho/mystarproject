#include "StdAfx.h"
#include "Com_WraithAnim.h"

#include "TextureMgr.h"
#include "TimeMgr.h"
#include "Obj.h"
CCom_WraithAnim::CCom_WraithAnim(D3DXMATRIX& objmat)
:CCom_Animation(objmat)
{
	m_objname = L"WRAITH";
	m_statkey = L"";
}

CCom_WraithAnim::~CCom_WraithAnim(void)
{
}

void CCom_WraithAnim::Initialize(CObj* pobj)
{
	m_battack_end = true;

	SetAnimation(L"IDLE");
	m_pobj = pobj;
	m_rotation_speed = 90;

	CCom_Animation::InitTexidx();
}
void CCom_WraithAnim::SetAnimation(const TCHAR* statekey)
{
	if(m_statkey != statekey)
	{
		if(false == m_battack_end)
			return;

		m_statkey = statekey;
		m_frame.fcurframe = 0;	

		/* [i]�� ���� , DRONE, MOVE�� �������� */
		m_animtexture = CTextureMgr::GetInstance()->GetTUnitTexture(m_objname , m_statkey );

		if(NULL != m_animtexture)
		{
			m_frame.umax = m_animtexture[0].size();
			m_frame.fframespeed = (float)m_frame.umax;
		}
	}
}

void CCom_WraithAnim::Update(void)
{
	CCom_Animation::DirIdxCalculation();

	m_frame.fcurframe += GETTIME*m_frame.fframespeed;
	if( int(m_frame.fcurframe) >= m_frame.umax)
	{
		m_frame.fcurframe = 0;
	}

	const vector<TEXINFO*> vtemp = m_animtexture[m_texdiridx];

	if( (int)(m_frame.fcurframe) <= m_frame.umax)
		m_curtex = (vtemp)[int(m_frame.fcurframe)];
}

void CCom_WraithAnim::Render(void)
{
	m_objshadow_mat = m_objmat;
	m_objshadow_mat._42 += 38;

	m_pSprite->SetTransform(&m_objshadow_mat);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3( 32.f , 32.f , 0)
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

void CCom_WraithAnim::Release(void)
{

}

