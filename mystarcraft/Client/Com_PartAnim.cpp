#include "StdAfx.h"
#include "Com_PartAnim.h"

#include "TextureMgr.h"
#include "TimeMgr.h"
#include "Obj.h"

CCom_PartAnim::CCom_PartAnim(const TCHAR* objkey , const TCHAR* linktexkey , D3DXMATRIX& objmat , TEXINFO*& curtex ,TEXINFO*& linktex)
:CCom_Animation(objmat , curtex) , m_linktex(linktex)
{
	m_objname = objkey;
	m_link_texkey = linktexkey;

	m_statkey = L"";
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
}

void CCom_PartAnim::Update(void)
{
	m_frame.fcurframe += GETTIME*m_frame.fframespeed;
	if(m_frame.fcurframe >= m_frame.umax)
	{
		m_frame.fcurframe = 0.f;
		if( L"BUILD" == m_statkey )
		{
			m_pobj->SetState(IDLE);
			m_sub_on = true;
		}
	}


	if( (int)(m_frame.fcurframe) <= m_frame.umax)
		m_curtex = (*m_generaltex)[int(m_frame.fcurframe)];

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
