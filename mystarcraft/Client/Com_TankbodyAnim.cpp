#include "StdAfx.h"
#include "Com_TankbodyAnim.h"

#include "TimeMgr.h"
#include "TextureMgr.h"
#include "Obj.h"
#include "MyMath.h"
CCom_TankbodyAnim::CCom_TankbodyAnim(D3DXMATRIX& objmat , TEXINFO*& curtex)
:CCom_Animation(objmat , curtex)
{
	m_objname = L"TANKBODY";
	m_statkey = L"";
}

CCom_TankbodyAnim::~CCom_TankbodyAnim(void)
{
}

void CCom_TankbodyAnim::Initialize(CObj* pobj)
{
	SetAnimation(L"IDLE");
	m_pobj = pobj;

	m_rotation_speed = 20;

	CCom_Animation::InitTexidx();
}

void CCom_TankbodyAnim::Update(void)
{
	CCom_Animation::DirIdxCalculation();

	m_frame.fcurframe += GETTIME*m_frame.fframespeed;
	if(m_frame.fcurframe >= m_frame.umax)
	{
		m_frame.fcurframe = 0;
	}
		

	if(NULL !=  m_animtexture[m_texdiridx] )
	{
		const vector<TEXINFO*>* vtemp = m_animtexture[m_texdiridx];

		if( (int)(m_frame.fcurframe) <= m_frame.umax)
			m_curtex = (*vtemp)[int(m_frame.fcurframe)];
	}
	else
		m_curtex = NULL;
}

void CCom_TankbodyAnim::Render(void)
{

}

void CCom_TankbodyAnim::Release(void)
{

}

void CCom_TankbodyAnim::SetAnimation(const TCHAR* statekey)
{
	if(m_statkey != statekey)/*새로 들어오는 상태가 기존들어오는 상태와 다른상태여야 한다.*/
	{
		//if(statekey == L"MOVE")
		//	return;

		m_statkey = statekey;

		m_frame.fcurframe = 0;

		for(int i = 0; i < DIR_CNT; ++i)
		{
			/* [i]는 방향 , DRONE, MOVE의 사진집합 */
			m_animtexture[i] = CTextureMgr::GetInstance()->GetTUnitTexture(m_objname , statekey , i);
			if(NULL == m_animtexture[i])
				break;
		}

		if(NULL != m_animtexture[0])
		{
			m_frame.umax = m_animtexture[0]->size();
			m_frame.fframespeed = (float)m_frame.umax*2;
		}
	}
}
