#include "StdAfx.h"
#include "Com_DroneAnim.h"

#include "TimeMgr.h"
CCom_DroneAnim::CCom_DroneAnim(D3DXMATRIX& objmat , TEXINFO*& curtex)
: CCom_Animation(objmat , curtex) 
{
	m_objname = L"DRONE";
	m_statkey = L"";
}

CCom_DroneAnim::~CCom_DroneAnim(void)
{
}

void CCom_DroneAnim::Initialize(CObj* pobj)
{
	SetAnimation(L"IDLE");

	m_pobj = pobj;
}

void CCom_DroneAnim::Update(void)
{
	CCom_Animation::DirIdxCalculation();

	if(m_frame.fcurframe >= m_frame.umax)
	{
		m_frame.fcurframe = 0;
	}
	m_frame.fcurframe += GETTIME*m_frame.fframespeed;	

	const vector<TEXINFO*>* vtemp = m_animtexture[m_texdiridx];

	if( (int)(m_frame.fcurframe) <= m_frame.umax)
		m_curtex = (*vtemp)[int(m_frame.fcurframe)];

}

void CCom_DroneAnim::Render(void)
{

}

void CCom_DroneAnim::Release(void)
{

}
