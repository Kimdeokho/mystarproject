#include "StdAfx.h"
#include "Com_ZBuildingAnim.h"

#include "TextureMgr.h"
#include "TimeMgr.h"
#include "Obj.h"
#include "ScrollMgr.h"
#include "Terran_building.h"

#include "TileManager.h"
#include "Session_Mgr.h"
CCom_ZBuildingAnim::CCom_ZBuildingAnim(const TCHAR* objname , D3DXMATRIX& objmat)
:CCom_Animation(objmat)
{
	m_objname = objname;
	m_origin_name = objname;
	m_statkey = L"";
}

CCom_ZBuildingAnim::~CCom_ZBuildingAnim(void)
{
	Release();
}

void CCom_ZBuildingAnim::Initialize(CObj* pobj)
{
	m_pobj = pobj;

	m_sighton = false;
	m_escape = false;
}

void CCom_ZBuildingAnim::Update(void)
{
	TEAM_NUMBER eteam = CSession_Mgr::GetInstance()->GetTeamNumber();
	if(CTileManager::GetInstance()->GetFogLight(m_pobj->Getcuridx(32) , eteam))
	{
		m_sighton = true;
		m_escape = false;
	}
	else
	{		
		if(m_sighton)
		{
			//켜졌다가 꺼진상태
			m_sighton = false;
			//마지막 상태 저장
			m_escape = true;
		}
	}
	if(m_escape)
		return;


	m_frame.fcurframe += GETTIME*m_frame.fframespeed;

	if(m_frame.fcurframe >= m_frame.umax)
	{
		m_frame.fcurframe = 0.f;
		if(!wcsncmp(m_statkey , L"BIRTH" , 5 ))
		{
			SetTextureName(m_origin_name);
			SetAnimation(L"IDLE");
			m_pobj->SetState(IDLE);
		}
	}

	if( (int)(m_frame.fcurframe) <= m_frame.umax)
		m_curtex = (*m_generaltex)[int(m_frame.fcurframe)];
}

void CCom_ZBuildingAnim::Render(void)
{
	m_pSprite->SetTransform(&m_objmat);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0)
		, NULL , D3DCOLOR_ARGB(255, 255, 255, 255));
}



void CCom_ZBuildingAnim::SetAnimation(const TCHAR* statekey)
{
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
			else
				m_frame.fframespeed = (float)m_frame.umax;
		}
	}
}
void CCom_ZBuildingAnim::Release(void)
{

}

void CCom_ZBuildingAnim::SetOriginal_texname(const TCHAR* statekey)
{
	m_origin_name = statekey;
}
