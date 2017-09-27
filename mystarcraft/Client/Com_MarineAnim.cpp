#include "StdAfx.h"
#include "Com_MarineAnim.h"

#include "TextureMgr.h"
#include "TimeMgr.h"

CCom_MarineAnim::CCom_MarineAnim(D3DXMATRIX& objmat , TEXINFO*& curtex)
:CCom_Animation(objmat , curtex)
{
	m_objname = L"MARINE";
	m_statkey = L"";
	//memset(m_statkey , 0 , sizeof(TCHAR)*64);
	m_tempkey = L"";
	m_attackloop = 0;
}

CCom_MarineAnim::~CCom_MarineAnim(void)
{
}

void CCom_MarineAnim::Initialize(CObj* pobj)
{
	SetAnimation(L"IDLE");
	m_pobj = pobj;
	m_rotation_speed = 40;

	CCom_Animation::InitTexidx();
}

void CCom_MarineAnim::SetAnimation(const TCHAR* statekey)
{
	//0 총꺼내기 , 1~2공격 루프3회

	/*애니메이션 딜레이가 존재한다
	예를들어 
	*/


	if(m_statkey != statekey)
	{
		if(m_attackloop >= 3)
			m_attackloop = 0;

		m_statkey = statekey;
		m_frame.fcurframe = 0;
		//재생해도 된다		

		for(int i = 0; i < DIR_CNT; ++i)
		{
			/* [i]는 방향 , DRONE, MOVE의 사진집합 */
			m_animtexture[i] = CTextureMgr::GetInstance()->GetTUnitTexture(m_objname , m_statkey , i);
			if(NULL == m_animtexture[i])
				break;
		}

		if(NULL != m_animtexture[0])
		{
			m_frame.umax = m_animtexture[0]->size();
			m_frame.fframespeed = (float)m_frame.umax;
			if(L"ATTACK" == statekey)
				m_frame.fframespeed *= 8;
			if(L"MOVE" == statekey)
				m_frame.fframespeed *= 2;
		}
	}
	else
	{
		if( L"ATTACK" == m_statkey /*!lstrcmp(m_statkey , L"ATTACK")*/ )
		{
			if(m_attackloop >= 3)
			{
				m_attackloop = 0;
				m_frame.fcurframe = 0;
			}
		}
	}


	// ATTACK일경우 무기가 발사준비 완료일때 재생시킨다

	//if(무기 발사준비가 완료되면)
	/*
	 attack_loop = 0;
	*/
}

void CCom_MarineAnim::Update(void)
{
	CCom_Animation::DirIdxCalculation();

	// ATTACK일경우 무기가 발사준비 완료일때 재생시킨다

	if(m_attackloop >= 3 )
		return;

	m_frame.fcurframe += GETTIME*m_frame.fframespeed;
	if( int(m_frame.fcurframe) >= m_frame.umax)
	{

		if( L"ATTACK" == m_statkey)
			++m_attackloop;

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

void CCom_MarineAnim::Render(void)
{

}

void CCom_MarineAnim::Release(void)
{

}


