#include "StdAfx.h"
#include "Com_Animation.h"

#include "Obj.h"
#include "TextureMgr.h"
#include "MyMath.h"
#include "TimeMgr.h"
#include "FontMgr.h"
CCom_Animation::CCom_Animation(D3DXMATRIX& objmat , TEXINFO*& curtex)
:m_objmat(objmat) , m_curtex(curtex)
{
	m_texdiridx = 0;
	m_curdiridx = 0;
	m_newdiridx = 0;
	m_olddiridx = 0;

	m_newdgree = 0.f;

	m_pobj = NULL;
	m_frotation_time = 0.f;
	m_rotation_dir = 1;

	m_vout = D3DXVECTOR3(0,0,0);
	m_olddirvec = D3DXVECTOR3(OFFSET_DIRVEC.x , OFFSET_DIRVEC.y , 0);
	m_newdirvec = D3DXVECTOR3(0,0,0);
}

CCom_Animation::~CCom_Animation(void)
{
}

void CCom_Animation::SetAnimation(const TCHAR* statekey)
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

void CCom_Animation::Update(void)
{

}

void CCom_Animation::DirIdxCalculation(void)
{
	float       fdot;

	D3DXVECTOR2	curdir = m_pobj->GetcurDir(); 
	fdot = D3DXVec2Dot(&curdir , &OFFSET_DIRVEC);

	m_newdgree = CMyMath::scala_to_dgree(fdot);

	if(m_pobj->GetcurDir().x < 0)
		m_newdgree = 360 - m_newdgree;

	m_newdiridx = (int)( (m_newdgree/22.5f) + 0.5f) % 16;

	m_newdirvec = D3DXVECTOR3(curdir.x , curdir.y , 0);


	if(m_newdiridx != m_olddiridx)
	{

		D3DXVec3Cross(&m_vout , &m_olddirvec , &m_newdirvec);

		//왼손기준 z가 양수면 시계방향

		if(m_vout.z > 0)
		{
			m_rotation_dir = 1;
			//printf("시계방향\n");
		}
		else
		{
			m_rotation_dir = -1;
			//printf("반시계방향\n");
		}

		m_olddiridx = m_newdiridx;
		m_olddirvec = m_newdirvec;
	}

	if((int)m_curdiridx != m_newdiridx)
	{
		float rotation_speed = GETTIME*m_rotation_dir*50;
		if(rotation_speed > 1.f)
			rotation_speed = 1;

		m_curdiridx += rotation_speed;

		if(m_curdiridx < 0)
			m_curdiridx = 15.99f;
		else if(m_curdiridx >= 16)
			m_curdiridx = 0;

	}

	m_texdiridx = (int)m_curdiridx;

	if(m_texdiridx > 8)
	{
		m_texdiridx = 16 - m_texdiridx;
		m_objmat._11 = -1;
	}
	else
		m_objmat._11 = 1;

	//printf(" m_curdiridx %f\n" , m_curdiridx);
	//printf(" m_newdiridx %d\n" , m_newdiridx);
}

void CCom_Animation::Render(void)
{

}
