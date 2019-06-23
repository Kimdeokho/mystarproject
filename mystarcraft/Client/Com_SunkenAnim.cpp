#include "StdAfx.h"
#include "Com_SunkenAnim.h"

#include "TextureMgr.h"
#include "TimeMgr.h"
#include "Obj.h"
#include "ScrollMgr.h"
#include "Terran_building.h"

#include "TileManager.h"
#include "Session_Mgr.h"
#include "ScrollMgr.h"

#include "Sunken.h"

CCom_SunkenAnim::CCom_SunkenAnim(const TCHAR* objname , D3DXMATRIX& objmat)
:CCom_Animation(objmat )
{
	m_objname = objname;
	m_origin_name = objname;
	m_statkey = L"";
}

CCom_SunkenAnim::~CCom_SunkenAnim(void)
{
	Release();
}

void CCom_SunkenAnim::Initialize(CObj* pobj)
{
	m_pobj = pobj;

	m_sighton = false;
	m_escape = false;

	m_sign = 1.f;

	m_bsighton = false;
	m_isescape = false;

	m_staticTex = NULL;
	m_updateTex = NULL;

	D3DXMatrixIdentity(&m_curMat);
	m_staticPos = D3DXVECTOR2(0,0);
}

void CCom_SunkenAnim::Update(void)
{
	TEAM_NUMBER eteam = CSession_Mgr::GetInstance()->GetTeamNumber();

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

	m_frame.fcurframe += GETTIME*m_frame.fframespeed * m_sign;

	if(m_frame.fcurframe >= m_frame.umax)
	{		
		if(!wcsncmp(m_statkey , L"BIRTH" , 5 ))
		{
			SetTextureName(m_origin_name);
			SetAnimation(L"IDLE");
			((CZerg_building*)m_pobj)->Build_Complete();
		}

		if(StrStrW(m_statkey , L"ATTACK"))
		{
			m_sign *= -1;
			m_frame.fcurframe = float(m_frame.umax - 1);			
		}
		else
			m_frame.fcurframe = 0.f;
	}
	else if(m_frame.fcurframe <= 0)
	{
		m_battack_end = true;
		m_frame.fcurframe = 0.f;
		m_sign *= -1;

		if(ATTACK == m_pobj->GetUnitinfo().state)
		{
			SetAnimation(L"IDLE");
			m_pobj->SetState(IDLE);
		}
	}
	else
	{
		if(StrStrW(m_statkey , L"ATTACK"))
			m_battack_end = false;
		else
			m_battack_end = true;
	}

	if( (int)(m_frame.fcurframe) <= m_frame.umax)
		m_updateTex = (*m_generaltex)[int(m_frame.fcurframe)];
}

void CCom_SunkenAnim::Render(void)
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

	if(!CScrollMgr::inside_camera(m_staticPos.x , m_staticPos.y))
		return;
	if(NULL == m_curtex)
		return;

	m_pSprite->SetTransform(&m_curMat);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0)
		, NULL , D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CCom_SunkenAnim::SetAnimation(const TCHAR* statekey)
{
	if(!m_battack_end)
		return;
	if(m_statkey != statekey)/*새로 들어오는 상태가 기존들어오는 상태와 다른상태여야 한다.*/
	{
		m_statkey = statekey;

		m_generaltex = CTextureMgr::GetInstance()->GetStateTexture_vecset( m_objname , m_statkey);

		if(NULL != m_generaltex)
		{
			m_frame.fcurframe = 0;

			m_frame.umax = m_generaltex->size();

			if(!wcsncmp(m_statkey , L"BUILD" , 5 ))
				m_frame.fframespeed = float(m_frame.umax) * 1.4f;
			else if(!wcsncmp(m_statkey , L"BIRTH" , 5 ))
				m_frame.fframespeed = float(m_frame.umax) * 5.f;
			else if(StrStrW(m_statkey , L"ATTACK"))
				m_frame.fframespeed = float(m_frame.umax) * 1.7f;
			else
				m_frame.fframespeed = (float)m_frame.umax;
		}
	}
}
void CCom_SunkenAnim::SetOriginal_texname(const TCHAR* statekey)
{
	m_origin_name = statekey;
}

void CCom_SunkenAnim::Release(void)
{

}
