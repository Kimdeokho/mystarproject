#include "StdAfx.h"
#include "Com_TurretheadAnim.h"

#include "TextureMgr.h"
#include "TimeMgr.h"
#include "Obj.h"

CCom_TurretheadAnim::CCom_TurretheadAnim(D3DXMATRIX& objmat)
:CCom_Animation(objmat)
{
	m_objname = L"T_TURRET_HEAD";
	m_statkey = L"";

}

CCom_TurretheadAnim::~CCom_TurretheadAnim(void)
{
}

void CCom_TurretheadAnim::Initialize(CObj* pobj)
{
	m_fdir = 0.f;
	m_rotation_speed = 40;
	m_pobj = pobj;

	SetAnimation(L"IDLE");
}

void CCom_TurretheadAnim::Update(void)
{
	if(L"IDLE" == m_statkey)
	{
		m_fdir += 30 * GETTIME;

		if(m_fdir >= 32.f)
			m_fdir = 0;

		m_texdiridx = int(m_fdir);
		if(m_texdiridx > 16)
		{

			m_texdiridx = 32 - m_texdiridx;
			m_objmat._11 = -1;
		}
		else
			m_objmat._11 = 1;
	}
	else
	{
		CCom_Animation::DirIdxCalculation();

		m_frame.fcurframe += GETTIME*m_frame.fframespeed;

		if( int(m_frame.fcurframe) >= m_frame.umax)
		{
			m_frame.fcurframe = 0;
		}

	}

	//const vector<TEXINFO*>* vtemp = m_animtexture32[m_texdiridx];
	m_curtex = m_animtexture[m_texdiridx][int(m_frame.fcurframe)];

}
void CCom_TurretheadAnim::Render(void)
{
	m_pSprite->SetTransform(&m_objmat);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0)
		, NULL , D3DCOLOR_ARGB(255,255,255,255));
}

void CCom_TurretheadAnim::Release(void)
{

}

void CCom_TurretheadAnim::SetAnimation(const TCHAR* statekey)
{
	if(m_statkey != statekey)
	{
		if(false == m_battack_end)
			return;

		m_statkey = statekey;
		m_frame.fcurframe = 0;			

		//for(int i = 0; i < DIR_CNT17; ++i)
		//{
			/* [i]는 방향 , DRONE, MOVE의 사진집합 */
			m_animtexture = CTextureMgr::GetInstance()->GetTUnitTexture(m_objname , m_statkey );
			//if(NULL == m_animtexture32[i])
				//break;
		//}

		//if(NULL != m_animtexture32[0])
		//{
			m_frame.umax = m_animtexture[0].size();
			m_frame.fframespeed = (float)m_frame.umax;
			m_curtex = m_animtexture[m_texdiridx][int(m_frame.fcurframe)];
		//}
	}
}
