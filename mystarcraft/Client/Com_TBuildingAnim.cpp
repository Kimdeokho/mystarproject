#include "StdAfx.h"
#include "Com_TBuildingAnim.h"

#include "TextureMgr.h"
#include "TimeMgr.h"
#include "Obj.h"
#include "ScrollMgr.h"
#include "Terran_building.h"
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
}

void CCom_TBuildingAnim::Update(void)
{
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
		{
			m_frame.fcurframe = (float)(m_frame.umax - 1);
		}
	}


	if( (int)(m_frame.fcurframe) <= m_frame.umax)
		m_curtex = (*m_generaltex)[int(m_frame.fcurframe)];
}

void CCom_TBuildingAnim::Render(void)
{
	D3DCOLOR ecolor;
	if(TEAM_1 == m_pobj->GetTeamNumber())
		ecolor = D3DCOLOR_ARGB(255, 255, 0, 0);
	else
		ecolor = D3DCOLOR_ARGB(255, 255, 255, 255);

	m_objshadow_mat = m_objmat;
	if(AIR_IDLE == m_pobj->GetUnitinfo().state)
	{
		m_objshadow_mat._42 += 48;
		m_pSprite->SetTransform(&m_objshadow_mat);
		m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0) , NULL , D3DCOLOR_ARGB(100,0,0,0));
	}
	else if(TAKE_OFF == m_pobj->GetUnitinfo().state)
	{
		m_objshadow_mat._42 = m_vairpos.y - CScrollMgr::m_fScrollY;
		m_pSprite->SetTransform(&m_objshadow_mat);
		m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0) , NULL , D3DCOLOR_ARGB(100,0,0,0));
	}
	else if(LANDING == m_pobj->GetUnitinfo().state)
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

	m_pSprite->SetTransform(&m_objmat);
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
