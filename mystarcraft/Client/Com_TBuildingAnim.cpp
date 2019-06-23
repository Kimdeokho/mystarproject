#include "StdAfx.h"
#include "Com_TBuildingAnim.h"

#include "TextureMgr.h"
#include "TimeMgr.h"
#include "Obj.h"

#include "Terran_building.h"

#include "ScrollMgr.h"
#include "TileManager.h"
#include "Session_Mgr.h"
CCom_TBuildingAnim::CCom_TBuildingAnim(const TCHAR* objname ,D3DXMATRIX& objmat)
:CCom_Animation(objmat)
{
	m_objname = objname;

	m_statkey = L"";
}

CCom_TBuildingAnim::~CCom_TBuildingAnim(void)
{
}

void CCom_TBuildingAnim::Initialize(CObj* pobj)
{
	//완성 , 미완성에따라 build 또는 idle로 초기화
	m_pobj = pobj;

	m_fbuildtime = m_pobj->GetUnitinfo().fbuildtime;
	SetAnimation(L"BUILD");

	m_bsighton = false;
	m_isescape = false;

	m_staticTex = NULL;
	m_updateTex = NULL;

	D3DXMatrixIdentity(&m_curMat);

	m_staticidx = m_pobj->Getcuridx(32);
	m_staticPos = D3DXVECTOR2(0,0);
}

void CCom_TBuildingAnim::Update(void)
{

	TEAM_NUMBER eteam = CSession_Mgr::GetInstance()->GetTeamNumber();

	if(FOG_ALPHA == CTileManager::GetInstance()->GetFogLight( m_staticidx , eteam) ||
		FOG_ALPHA == CTileManager::GetInstance()->GetFogLight( m_pobj->Getcuridx(32) , eteam))
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
			m_staticidx = m_pobj->Getcuridx(32);
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
		if( L"AIR" == m_statkey)
			m_frame.fcurframe = (float)(m_frame.umax - 1);
	}


	if( (int)(m_frame.fcurframe) <= m_frame.umax)
		m_updateTex = (*m_generaltex)[int(m_frame.fcurframe)];


}

void CCom_TBuildingAnim::Render(void)
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
		m_staticidx = m_pobj->Getcuridx(32);
	}

	if(!CScrollMgr::inside_camera(m_curMat._41 + CScrollMgr::m_fScrollX , m_curMat._42 + CScrollMgr::m_fScrollY))
		return;
	if(NULL == m_curtex)
		return;

	m_objshadow_mat = m_curMat;
	if(AIR_IDLE == m_pobj->GetUnitinfo().state && !m_isescape)
	{
		m_objshadow_mat._42 += 48;
		m_pSprite->SetTransform(&m_objshadow_mat);
		m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0) , NULL , D3DCOLOR_ARGB(100,0,0,0));
	}
	else if(TAKE_OFF == m_pobj->GetUnitinfo().state && !m_isescape)
	{
		m_objshadow_mat._42 = m_vairpos.y - CScrollMgr::m_fScrollY;
		m_pSprite->SetTransform(&m_objshadow_mat);
		m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0) , NULL , D3DCOLOR_ARGB(100,0,0,0));
	}
	else if(LANDING == m_pobj->GetUnitinfo().state && !m_isescape)
	{
		m_objshadow_mat._42 = m_vgroundpos.y - CScrollMgr::m_fScrollY;
		m_pSprite->SetTransform(&m_objshadow_mat);
		m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0) , NULL , D3DCOLOR_ARGB(100,0,0,0));
	}
	else
	{
		m_objshadow_mat._41 -= 8;
		m_objshadow_mat._42 -= 8;

		m_pSprite->SetTransform(&m_objshadow_mat);
		m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0) , NULL , D3DCOLOR_ARGB(100,0,0,0));
	}


	D3DCOLOR ecolor;
	if(TEAM_1 == m_pobj->GetTeamNumber())
		ecolor = D3DCOLOR_ARGB(255, 255, 0, 0);
	else
		ecolor = D3DCOLOR_ARGB(255, 255, 255, 255);


	m_pSprite->SetTransform(&m_curMat);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0)
		, NULL , ecolor);
}

void CCom_TBuildingAnim::Release(void)
{

}

void CCom_TBuildingAnim::SetAnimation(const TCHAR* statekey)
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
				m_frame.fframespeed = float(m_frame.umax)/m_fbuildtime;
			else if(L"PRODUCTION" == m_statkey)
				m_frame.fframespeed = float(m_frame.umax) * 4;
			else
				m_frame.fframespeed = (float)m_frame.umax;
		}
	}
}

void CCom_TBuildingAnim::SetAirpos(const D3DXVECTOR2& vpos)
{
	m_vairpos = vpos;
}

void CCom_TBuildingAnim::SetGroundpos(const D3DXVECTOR2& vpos)
{
	m_vgroundpos = vpos;
}
