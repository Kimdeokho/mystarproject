#include "StdAfx.h"
#include "DroneAnim.h"

#include "TimeMgr.h"
CDroneAnim::CDroneAnim(void)
{
}

CDroneAnim::~CDroneAnim(void)
{
}

void CDroneAnim::Initialize(void)
{

}

void CDroneAnim::Update(void)
{

	if(m_frame.fcurframe >= m_frame.umax)
	{
		m_frame.fcurframe = 0;
	}
	m_frame.fcurframe += GETTIME*m_frame.fframespeed;	

	const vector<TEXINFO*>* vtemp = m_animtexture[m_diridx];

	if( (unsigned int)m_frame.fcurframe <= m_frame.umax)
		*m_curtex = ((*vtemp)[int(m_frame.fcurframe)]);

}

void CDroneAnim::Render(void)
{

}

void CDroneAnim::Release(void)
{

}
