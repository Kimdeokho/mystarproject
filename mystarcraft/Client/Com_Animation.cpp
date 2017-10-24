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

	m_rotation_speed = 10.f;
	m_brotationcomplete = false;
	m_banim_end = false;

	m_generaltex = NULL;
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

	m_newdiridx = (int)( (m_newdgree/22.5f) + 0.5f) % 16;
	m_curdiridx = (float)m_newdiridx;
}
void CCom_Animation::SetAnimation(const TCHAR* statekey)
{
	//if(m_statkey != statekey)/*새로 들어오는 상태가 기존들어오는 상태와 다른상태여야 한다.*/
	//{
	//	m_statkey = statekey;
	//	//lstrcpy(m_statkey , statekey);

	//	m_frame.fcurframe = 0;

	//	for(int i = 0; i < DIR_CNT; ++i)
	//	{
	//		/* [i]는 방향 , DRONE, MOVE의 사진집합 */
	//		m_animtexture[i] = CTextureMgr::GetInstance()->GetZUnitTexture(m_objname , statekey , i);
	//		if(NULL == m_animtexture[i])
	//			break;
	//	}

	//	if(NULL != m_animtexture[0])
	//	{
	//		m_frame.umax = m_animtexture[0]->size() - 1;
	//		m_frame.fframespeed = (float)m_frame.umax;
	//	}
	//}
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

	if(curdir.x < 0)
		m_newdgree = 360 - m_newdgree;

	m_newdiridx = (int)( (m_newdgree/22.5f) + 0.5f) % 16;


	if(m_newdiridx != (int)m_curdiridx)
	{
		float cur_radian = CMyMath::dgree_to_radian(m_curdiridx*22.5f);
		float new_radian = CMyMath::dgree_to_radian(m_newdiridx*22.5f);

		m_olddirvec.x = cosf(cur_radian);
		m_olddirvec.y = sinf(cur_radian);

		m_newdirvec.x = cosf(new_radian);
		m_newdirvec.y = sinf(new_radian);

		D3DXVec3Cross(&m_vout , &m_olddirvec , &m_newdirvec);
		//왼손기준 z가 양수면 시계방향

		if(m_vout.z > 0)
		{
			m_rotation_dir = 1;
		}
		else
		{
			m_rotation_dir = -1;
		}

		//m_bsw = true;
	}

	if((int)m_curdiridx != m_newdiridx)
	{
		float rotation_speed = GETTIME*m_rotation_dir*m_rotation_speed;
		if(rotation_speed > 1.f)
			rotation_speed = 1;

		m_curdiridx += rotation_speed;

		if(m_curdiridx < 0)
			m_curdiridx = 15.99f;
		else if(m_curdiridx >= 16)
			m_curdiridx = 0;

		m_brotationcomplete = false;
	}
	else
	{
		m_brotationcomplete = true;
		//방향 전부 돌림
		//if(true == m_bsw)
		//{
		//	m_bsw = false;
		//	m_brotationcomplete = true;
		//}
	}

	m_texdiridx = (int)m_curdiridx;

	if(m_texdiridx > 8)
	{
		m_texdiridx = 16 - m_texdiridx;
		m_objmat._11 = -1;
	}
	else
		m_objmat._11 = 1;


	m_pobj->Setdiridx( m_texdiridx );

}
void CCom_Animation::AIR_DirIdxCalculation(void)
{
	//float       fdot;


	//D3DXVECTOR2	curdir = m_pobj->GetcurDir(); 
	//fdot = D3DXVec2Dot(&curdir , &OFFSET_DIRVEC);

	//m_newdgree = CMyMath::scala_to_dgree(fdot);

	//if(m_pobj->GetcurDir().x < 0)
	//	m_newdgree = 360 - m_newdgree;

	//m_newdiridx = (int)( (m_newdgree/11.25f) + 0.5f) % 32;


	//if(m_newdiridx != (int)m_curdiridx)
	//{
	//	float cur_radian = CMyMath::dgree_to_radian(m_curdiridx*11.25f);
	//	float new_radian = CMyMath::dgree_to_radian(m_newdiridx*11.25f);

	//	m_olddirvec.x = cosf(cur_radian);
	//	m_olddirvec.y = sinf(cur_radian);

	//	m_newdirvec.x = cosf(new_radian);
	//	m_newdirvec.y = sinf(new_radian);

	//	D3DXVec3Cross(&m_vout , &m_olddirvec , &m_newdirvec);
	//	//왼손기준 z가 양수면 시계방향

	//	if(m_vout.z > 0)
	//	{
	//		m_rotation_dir = 1;
	//		//printf("시계방향\n");
	//	}
	//	else
	//	{
	//		m_rotation_dir = -1;
	//		//printf("반시계방향\n");
	//	}

	//	m_bsw = true;
	//}

	//if((int)m_curdiridx != m_newdiridx)
	//{
	//	float rotation_speed = GETTIME*m_rotation_dir*m_rotation_speed;
	//	if(rotation_speed > 1.f)
	//		rotation_speed = 1;

	//	m_curdiridx += rotation_speed;

	//	if(m_curdiridx < 0)
	//		m_curdiridx = 31.99f;
	//	else if(m_curdiridx >= 32)
	//		m_curdiridx = 0;

	//	m_brotationcomplete = false;
	//}
	//else
	//{
	//	//방향 전부 돌림
	//	if(m_bsw)
	//	{
	//		m_bsw = false;
	//		m_brotationcomplete = true;
	//	}
	//}

	//m_texdiridx = (int)m_curdiridx;

	//if(m_texdiridx > 16)
	//{
	//	m_texdiridx = 32 - m_texdiridx;
	//	m_objmat._11 = -1;
	//}
	//else
	//	m_objmat._11 = 1;


	//m_pobj->Setdiridx( m_texdiridx );

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
	int newdiridx = (int)( (newdgree/22.5f) + 0.5f) % 16;

	if(newdiridx != (int)m_curdiridx)
		return false;
	else
		return true;

	//공중일땐 도 바껴야겠지..

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
