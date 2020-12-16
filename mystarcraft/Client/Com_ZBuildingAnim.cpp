#include "StdAfx.h"
#include "Com_ZBuildingAnim.h"

#include "TextureMgr.h"
#include "TimeMgr.h"
#include "Obj.h"
#include "ScrollMgr.h"

#include "TileManager.h"
#include "Session_Mgr.h"

#include "Zerg_building.h"

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


	m_bsighton = false;
	m_isescape = false;

	m_staticTex = NULL;
	m_updateTex = NULL;

	D3DXMatrixIdentity(&m_curMat);

	m_staticPos = m_pobj->GetPos();
}

void CCom_ZBuildingAnim::Update(void)
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


	m_frame.fcurframe += GETTIME*m_frame.fframespeed;

	if(m_frame.fcurframe >= m_frame.umax)
	{
		m_frame.fcurframe = 0.f;
		if(!wcsncmp(m_statkey , L"BIRTH" , 5 ))
		{
			SetTextureName(m_origin_name);
			SetAnimation(L"IDLE");
			m_pobj->SetState(IDLE);

			if(OBJ_Z_GAS != m_pobj->GetOBJNAME())
				((CZerg_building*)m_pobj)->Build_Complete();
		}
	}

	if( (int)(m_frame.fcurframe) <= m_frame.umax)
		m_updateTex = (*m_generaltex)[int(m_frame.fcurframe)];
}

void CCom_ZBuildingAnim::Render(void)
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


void CCom_ZBuildingAnim::SetOriginal_texname(const TCHAR* statekey)
{
	m_origin_name = statekey;
}
void CCom_ZBuildingAnim::Release(void)
{

}