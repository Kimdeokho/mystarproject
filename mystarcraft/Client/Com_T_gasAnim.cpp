#include "StdAfx.h"
#include "Com_T_gasAnim.h"

#include "Obj.h"
#include "TimeMgr.h"
#include "TextureMgr.h"
CCom_T_gasAnim::CCom_T_gasAnim(D3DXMATRIX& objmat , TEXINFO*& curtex , float fbuildtime)
:CCom_Animation(objmat , curtex)
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
	m_frame.fcurframe += GETTIME*m_frame.fframespeed;
	if(m_frame.fcurframe >= m_frame.umax)
	{
		m_frame.fcurframe = 0.f;

		if(L"BUILD" == m_statkey)
		{
			m_pobj->SetState(IDLE);
		}
	}


	if( (int)(m_frame.fcurframe) <= m_frame.umax)
		m_curtex = (*m_generaltex)[int(m_frame.fcurframe)];
}

void CCom_T_gasAnim::Render(void)
{

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

		if(L"BUILD" == m_statkey)
			m_frame.fframespeed = 4/m_fbuildtime;
		else
			m_frame.fframespeed = (float)m_frame.umax;
	}
}
