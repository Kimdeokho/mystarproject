#include "StdAfx.h"
#include "Animation.h"

#include "Obj.h"
#include "TextureMgr.h"

CAnimation::CAnimation(void)
{
	m_diridx = 0;
	m_pobj = NULL;
}

CAnimation::~CAnimation(void)
{
}

void CAnimation::Setcurtex_adress(TEXINFO** curtex)
{
	m_curtex = curtex;
}

void CAnimation::SetAnimation(const TCHAR* statekey)
{
	if(m_statkey != statekey)/*새로 들어오는 상태가 기존들어오는 상태와 다른상태여야 한다.*/
	{
		m_statkey = statekey;

		//m_statename = statekey;
		m_frame.fcurframe = 0;

		for(int i = 0; i < DIR_CNT; ++i)
		{
			/* [i]는 방향 , DRONE, MOVE의 사진집합 */
			m_animtexture[i] = CTextureMgr::GetInstance()->GetZUnitTexture(m_objname , statekey , i);
			if(NULL == m_animtexture[i])
				break;
		}

		if(NULL != m_animtexture[0])
		{
			m_frame.umax = m_animtexture[0]->size() - 1;
			m_frame.fframespeed = (float)m_frame.umax;
		}
	}
}

void CAnimation::Update(void)
{

}

void CAnimation::Setdir(const int& idx)
{
	m_diridx = idx;
}
