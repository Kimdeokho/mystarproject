#include "StdAfx.h"
#include "Com_Animation.h"

#include "Obj.h"
#include "TextureMgr.h"
#include "MyMath.h"
#include "TimeMgr.h"
#include "FontMgr.h"
#include "Device.h"
CCom_Animation::CCom_Animation(D3DXMATRIX& objmat)
:m_objmat(objmat) 
{
	D3DXMatrixIdentity(&m_objshadow_mat);
	m_pSprite = CDevice::GetInstance()->GetSprite();

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

	m_rotation_speed = 80.f;
//	m_brotationcomplete = false;
	m_banim_end = false;

	m_animtexture = NULL;
	m_generaltex = NULL;
	m_curtex = NULL;
	m_statkey = L"";
	m_battack_end = true;
}

CCom_Animation::~CCom_Animation(void)
{
}
void CCom_Animation::InitTexidx(void)
{
	D3DXVECTOR2	curdir = m_pobj->GetcurDir();
	float fdot = D3DXVec2Dot(&curdir , &OFFSET_DIRVEC);

	m_newdgree = CMyMath::scala_to_dgree(fdot);

	if(curdir.x < 0)
		m_newdgree = 360 - m_newdgree;

	m_newdiridx = (int)( (m_newdgree/11.25f) + 0.5f) % 32;
	m_curdiridx = (float)m_newdiridx;

	//m_texdiridx = (int)m_newdiridx;
}
void CCom_Animation::SetAnimation(const TCHAR* statekey)
{
}

void CCom_Animation::Update(void)
{

}
void CCom_Animation::Dir_Initialize(void)
{
	float       fdot;

	D3DXVECTOR2	curdir = m_pobj->GetcurDir(); 
	fdot = D3DXVec2Dot(&curdir , &OFFSET_DIRVEC);

	m_newdgree = CMyMath::scala_to_dgree(fdot);

	if(m_pobj->GetcurDir().x < 0)
		m_newdgree = 360 - m_newdgree;

	m_newdiridx = (int)( (m_newdgree/11.25f) + 0.5f) % 32;

	m_texdiridx = (int)m_newdiridx;

	if(m_texdiridx > 16)
	{
		m_texdiridx = 32 - m_texdiridx;
		m_objmat._11 = -1;
	}
	else
		m_objmat._11 = 1;
}
void CCom_Animation::DirIdxCalculation(void)
{	
	float       fdot;

	D3DXVECTOR2	curdir = m_pobj->GetcurDir(); 
	fdot = D3DXVec2Dot(&curdir , &OFFSET_DIRVEC);
	
	m_newdgree = CMyMath::scala_to_dgree(fdot);

	if(m_pobj->GetcurDir().x < 0)
		m_newdgree = 360 - m_newdgree;

	m_newdiridx = (int)( (m_newdgree/11.25f) + 0.5f) % 32;

	if(m_newdiridx != (int)m_curdiridx)
	{
		float cur_radian = CMyMath::dgree_to_radian(m_curdiridx*11.25f);
		float new_radian = CMyMath::dgree_to_radian(m_newdiridx*11.25f);

		m_olddirvec.x = cosf(cur_radian);
		m_olddirvec.y = sinf(cur_radian);

		m_newdirvec.x = cosf(new_radian);
		m_newdirvec.y = sinf(new_radian);

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
	}

	m_rotation_tick = GETTIME*m_rotation_dir*m_rotation_speed;

	if( abs(m_newdiridx - (int)m_curdiridx) > abs( (int)m_rotation_tick) )
	{		

		m_curdiridx += m_rotation_tick;

		if(m_curdiridx < 0)
			m_curdiridx = 31.99f;
		else if(m_curdiridx >= 32)
			m_curdiridx = 0;

	}
	else
	{
		m_curdiridx = (float)m_newdiridx;
	}

	m_texdiridx = (int)m_curdiridx;
	if(m_texdiridx > 16)
	{
		m_texdiridx = 32 - m_texdiridx;
		m_objmat._11 = -1;
	}
	else
		m_objmat._11 = 1;

}

void CCom_Animation::Render(void)
{

}

const TCHAR* CCom_Animation::GetAnimation(void)
{
	return m_statkey;
}

bool CCom_Animation::GetRotationComplete(void)
{
	D3DXVECTOR2	curdir = m_pobj->GetcurDir(); 
	float fdot = D3DXVec2Dot(&curdir , &OFFSET_DIRVEC);

	float newdgree = CMyMath::scala_to_dgree(fdot);

	if(m_pobj->GetcurDir().x < 0)
		newdgree = 360 - newdgree;

	int newdiridx = (int)( (newdgree/11.25f) + 0.5f) % 32;

	if(newdiridx != (int)m_curdiridx)
		return false;
	else
		return true;

}
int CCom_Animation::GetCurDirIdx(void)
{
	return (int)m_curdiridx;
}
bool CCom_Animation::GetAnimation_end(void)
{
	return m_banim_end;
}

bool CCom_Animation::GetAttackSync(void)
{
	return m_battack_sync;
}

bool CCom_Animation::GetAttack_end(void)
{
	return m_battack_end;
}

void CCom_Animation::SetTextureName(const TCHAR* name)
{
	m_objname = name;
}

