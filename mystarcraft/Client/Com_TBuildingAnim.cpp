#include "StdAfx.h"
#include "Com_TBuildingAnim.h"

#include "TextureMgr.h"
#include "TimeMgr.h"
#include "Obj.h"

CCom_TBuildingAnim::CCom_TBuildingAnim(const TCHAR* objname ,D3DXMATRIX& objmat , TEXINFO*& curtex)
:CCom_Animation(objmat , curtex)
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
	m_frame.fcurframe += GETTIME*m_frame.fframespeed;
	if(m_frame.fcurframe >= m_frame.umax)
	{
		m_frame.fcurframe = 0.f;
		if( L"BUILD" == m_statkey )
		{
			m_pobj->SetState(IDLE);			
		}
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
			else
				m_frame.fframespeed = (float)m_frame.umax;
		}
	}
}
