#include "StdAfx.h"
#include "Com_T_gasAnim.h"

#include "Obj.h"
#include "TimeMgr.h"
#include "TextureMgr.h"
#include "Session_Mgr.h"
#include "TileManager.h"
#include "ScrollMgr.h"

CCom_T_gasAnim::CCom_T_gasAnim(D3DXMATRIX& objmat , float fbuildtime)
:CCom_Animation(objmat)
{
	m_objname = L"T_GAS";

	m_statkey = L"";

	m_fbuildtime = fbuildtime;
}

CCom_T_gasAnim::~CCom_T_gasAnim(void)
{
}

void CCom_T_gasAnim::Initialize(CObj* pobj)
{
	m_pobj = pobj;

	SetAnimation(L"BUILD");
}

void CCom_T_gasAnim::Update(void)
{
	TEAM_NUMBER eteam = CSession_Mgr::GetInstance()->GetTeamNumber();
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

			m_staticTex = (*m_generaltex)[int(m_frame.fcurframe)];
			m_curMat = m_objmat;
			m_staticPos = m_pobj->GetPos();
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
	}
	//m_frame.fcurframe += GETTIME*m_frame.fframespeed;
	//if(m_frame.fcurframe >= m_frame.umax)
	//{
	//	m_frame.fcurframe = 0.f;

	//	if(L"BUILD" == m_statkey)
	//	{
	//		m_pobj->SetState(IDLE);
	//	}
	//}


	if( (int)(m_frame.fcurframe) <= m_frame.umax)
		m_updateTex = (*m_generaltex)[int(m_frame.fcurframe)];
}

void CCom_T_gasAnim::Render(void)
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

	m_objshadow_mat = m_objmat;
	m_objshadow_mat._41 -= 8;
	m_objshadow_mat._42 -= 8;

	m_pSprite->SetTransform(&m_objshadow_mat);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0) , NULL , D3DCOLOR_ARGB(100,0,0,0));

	m_pSprite->SetTransform(&m_objmat);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0)
		, NULL , D3DCOLOR_ARGB(255,255,255,255));
}

void CCom_T_gasAnim::Release(void)
{

}

void CCom_T_gasAnim::SetAnimation(const TCHAR* statekey)
{
	if(m_statkey != statekey)/*새로 들어오는 상태가 기존들어오는 상태와 다른상태여야 한다.*/
	{
		m_statkey = statekey;

		m_generaltex = CTextureMgr::GetInstance()->GetStateTexture_vecset( m_objname , m_statkey);

		m_frame.fcurframe = 0;

		m_frame.umax = m_generaltex->size();

	
		m_frame.fframespeed = (float)m_frame.umax;
	}
}
