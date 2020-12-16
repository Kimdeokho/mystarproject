#include "StdAfx.h"
#include "Com_TurretheadAnim.h"

#include "TextureMgr.h"
#include "TimeMgr.h"
#include "Session_Mgr.h"
#include "TileManager.h"
#include "ScrollMgr.h"
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

	m_bsighton = false;
	m_isescape = false;

	m_staticTex = NULL;
	m_updateTex = NULL;

	D3DXMatrixIdentity(&m_curMat);
	m_staticPos = m_pobj->GetPos();
}

void CCom_TurretheadAnim::Update(void)
{

	TEAM_NUMBER eteam = CSession_Mgr::GetInstance()->GetTeamNumber();

	if(CDebug_Mgr::m_dbglist[CDebug_Mgr::DBG_FOG])
		m_isescape = false;
	else
	{
		if(FOG_ALPHA == CTileManager::GetInstance()->GetFogLight(m_pobj->Getcuridx(32) , eteam))
		{
			m_bsighton = true;
			m_isescape = false;
		}
		else
		{		
			m_isescape = true;
			if(m_bsighton)
			{
				//켜졌다가 꺼진상태
				m_bsighton = false;
				//마지막 상태 저장


				m_staticTex = m_animtexture[m_texdiridx][int(m_frame.fcurframe)];
				m_curMat = m_objmat;
				m_staticPos = m_pobj->GetPos();
			}
		}
	}


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
	m_updateTex = m_animtexture[m_texdiridx][int(m_frame.fcurframe)];

}
void CCom_TurretheadAnim::Render(void)
{
	if(m_isescape)
	{
		m_curtex = m_staticTex;
		m_curMat._41 = m_staticPos.x - CScrollMgr::m_fScrollX;
		m_curMat._42 = m_staticPos.y - CScrollMgr::m_fScrollY;
	}
	else
	{
		m_curtex = m_updateTex;
		m_curMat = m_objmat;
	}
	if(!CScrollMgr::inside_camera(m_curMat._41 + CScrollMgr::m_fScrollX , m_curMat._42 + CScrollMgr::m_fScrollY))
		return;

	if(NULL == m_curtex)
		return;

	m_pSprite->SetTransform(&m_curMat);
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
