#include "StdAfx.h"
#include "Com_PartAnim.h"

#include "TextureMgr.h"
#include "TimeMgr.h"
#include "Obj.h"

#include "Terran_building.h"

#include "TileManager.h"
#include "Session_Mgr.h"
#include "ScrollMgr.h"

CCom_PartAnim::CCom_PartAnim(const TCHAR* objkey , const TCHAR* linktexkey , D3DXMATRIX& objmat )
:CCom_Animation(objmat)
{
	m_objname = objkey;
	m_link_texkey = linktexkey;

	m_statkey = L"";
	m_linktex = NULL;
}

CCom_PartAnim::~CCom_PartAnim(void)
{
}

void CCom_PartAnim::Initialize(CObj* pobj)
{
	m_pobj = pobj;

	m_subgeneral_tex = CTextureMgr::GetInstance()->GetGeneralTexture(m_link_texkey);
	m_subframe.fcurframe = 0;
	m_subframe.umax = m_subgeneral_tex->size();
	m_subframe.fframespeed = float(m_subframe.umax);
	m_sub_on = false;
	m_playdir = 1;
	m_fbuildtime = m_pobj->GetUnitinfo().fbuildtime;

	SetAnimation(L"BUILD");

	m_curtex = (*m_generaltex)[0];

	m_bsighton = false;
	m_isescape = false;

	m_staticTex = NULL;
	m_updateTex = NULL;

	D3DXMatrixIdentity(&m_curMat);
	m_staticPos = D3DXVECTOR2(0,0);
}

void CCom_PartAnim::Update(void)
{
	TEAM_NUMBER eteam = CSession_Mgr::GetInstance()->GetTeamNumber();

	if(CDebug_Mgr::m_dbglist[CDebug_Mgr::DBG_FOG])
		m_isescape = false;
	else
	{
		if(FOG_ALPHA == CTileManager::GetInstance()->GetFogLight( m_pobj->Getcuridx(32) , eteam))
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


				m_staticTex = (*m_generaltex)[int(m_frame.fcurframe)];
				m_curMat = m_objmat;
				m_staticPos = m_pobj->GetPos();
			}
		}
	}


	if(L"BUILD" == m_statkey)
	{
		const UNITINFO& unit_info = m_pobj->GetUnitinfo();
		float curframe = float(unit_info.hp) / float(unit_info.maxhp);
		m_frame.fcurframe = m_frame.umax * curframe;
	}
	else
		m_frame.fcurframe += GETTIME*m_frame.fframespeed;

	if(m_frame.fcurframe >= m_frame.umax)
	{
		m_frame.fcurframe = 0.f;
		if( L"BUILD" == m_statkey )
		{
			m_pobj->SetState(IDLE);
			m_sub_on = true;
			//((CTerran_building*)m_pobj)->Build_Complete();
		}
	}


	if( (int)(m_frame.fcurframe) <= m_frame.umax)
		m_updateTex = (*m_generaltex)[int(m_frame.fcurframe)];

	sub_update();
}
void CCom_PartAnim::sub_update(void)
{
	if(false == m_sub_on)
		return;


	m_subframe.fcurframe += GETTIME*m_subframe.fframespeed * m_playdir;
	if(m_subframe.fcurframe >= m_subframe.umax)
	{
		m_subframe.fcurframe =  float(m_subframe.umax - 1);
	}
	else if(m_subframe.fcurframe <= 0)
	{
		m_subframe.fcurframe = 0;
	}

	if( (int)(m_subframe.fcurframe) <= m_subframe.umax)
		m_linktex = (*m_subgeneral_tex)[int(m_subframe.fcurframe)];
}

void CCom_PartAnim::Render(void)
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

	if(NULL != m_linktex)
	{
		m_pSprite->Draw(m_linktex->pTexture , NULL , &D3DXVECTOR3(float(m_linktex->ImgInfo.Width/2) , float(m_linktex->ImgInfo.Height/2 ) , 0)
			, NULL , D3DCOLOR_ARGB(255,255,255,255));
	}


	m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0)
		, NULL , D3DCOLOR_ARGB(255,255,255,255));
}

void CCom_PartAnim::Release(void)
{

}

void CCom_PartAnim::SetAnimation(const TCHAR* statekey)
{
	if(m_statkey != statekey)/*새로 들어오는 상태가 기존들어오는 상태와 다른상태여야 한다.*/
	{
		m_statkey = statekey;

		m_generaltex = CTextureMgr::GetInstance()->GetStateTexture_vecset( m_objname , m_statkey);

		if(NULL != m_generaltex)
		{
			m_frame.fcurframe = 0;
			m_frame.umax = m_generaltex->size();

			if(L"BUILD" == m_statkey)
				m_frame.fframespeed = 4/m_fbuildtime;
			else
				m_frame.fframespeed = (float)m_frame.umax;
		}
	}
}


void CCom_PartAnim::play_direction(const int& dir)
{
	m_playdir = dir;
}
