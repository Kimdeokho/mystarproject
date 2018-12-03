#include "StdAfx.h"
#include "Com_HydraAnim.h"

#include "TimeMgr.h"
#include "TextureMgr.h"

CCom_HydraAnim::CCom_HydraAnim(D3DXMATRIX& objmat )
: CCom_Animation(objmat )
{
	m_objname = L"HYDRA";
	m_statkey = L"";
}

CCom_HydraAnim::~CCom_HydraAnim(void)
{
}

void CCom_HydraAnim::Initialize(CObj* pobj)
{
	m_battack_sync = false;
	SetAnimation(L"IDLE");

	m_pobj = pobj;
	CCom_Animation::InitTexidx();

	m_rotation_speed = 80.f;	
}

void CCom_HydraAnim::Update(void)
{
	CCom_Animation::DirIdxCalculation();	

	m_frame.fcurframe += GETTIME*m_frame.fframespeed;
	if(m_frame.fcurframe >= m_frame.umax)
	{
		if(L"ATTACK" == m_statkey)
		{
			m_frame.fframespeed = 4.f;
			m_frame.fcurframe = 2;
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
		if( 3 == int(m_frame.fcurframe) )
		{
			m_battack_sync = true;
		}
		else
			m_battack_sync = false;
	}

	if( (int)(m_frame.fcurframe) <= m_frame.umax)
		m_curtex = m_animtexture[m_texdiridx][ int(m_frame.fcurframe) ];
}

void CCom_HydraAnim::Render(void)
{
	if(NULL == m_curtex)
		return;

	m_objshadow_mat = m_objmat;
	m_objshadow_mat._42 += 8;

	m_pSprite->SetTransform(&m_objshadow_mat);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &m_vcenter
		, NULL , D3DCOLOR_ARGB(150,0,0,0));


	m_pSprite->SetTransform(&m_objmat);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &m_vcenter
		, NULL , D3DCOLOR_ARGB(255,255,255,255));
}
void CCom_HydraAnim::SetAnimation(const TCHAR* statekey)
{
	if(m_statkey != statekey)/*새로 들어오는 상태가 기존들어오는 상태와 다른상태여야 한다.*/
	{
		m_statkey = statekey;
		m_frame.fcurframe = 0;

		/* [i]는 방향 , DRONE, MOVE의 사진집합 */
		m_animtexture = CTextureMgr::GetInstance()->GetZUnitTexture(m_objname , statekey );

		if(NULL != m_animtexture)
		{
			m_frame.umax = m_animtexture[0].size();

			if(L"MOVE" == m_statkey)
				m_frame.fframespeed = (float)m_frame.umax*2.3f;
			else if(L"ATTACK" == m_statkey)
				m_frame.fframespeed = (float)m_frame.umax*3.3f;
			else
				m_frame.fframespeed = (float)m_frame.umax*1.3f;

			m_curtex = m_animtexture[m_texdiridx][ int(m_frame.fcurframe) ];
			m_vcenter = D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2), 0.f);
		}
	}
}

void CCom_HydraAnim::Release(void)
{

}