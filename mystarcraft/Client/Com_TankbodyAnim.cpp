#include "StdAfx.h"
#include "Com_TankbodyAnim.h"

#include "TimeMgr.h"
#include "TextureMgr.h"
#include "Obj.h"
#include "MyMath.h"
CCom_TankbodyAnim::CCom_TankbodyAnim(D3DXMATRIX& objmat)
:CCom_Animation(objmat)
{
	m_objname = L"TANKBODY";
	m_statkey = L"";
}

CCom_TankbodyAnim::~CCom_TankbodyAnim(void)
{
}

void CCom_TankbodyAnim::Initialize(CObj* pobj)
{
	m_statkey = L"";
	m_curtex = NULL;

	SetAnimation(L"IDLE");
	m_pobj = pobj;

	m_rotation_speed = 30;

	CCom_Animation::InitTexidx();
}

void CCom_TankbodyAnim::Update(void)
{
	//if(NULL == m_animtexture)
	//{
	//	m_curtex = NULL;
	//	return;
	//}

	CCom_Animation::DirIdxCalculation();

	m_frame.fcurframe += GETTIME*m_frame.fframespeed;

	if(m_frame.fcurframe >= m_frame.umax)
	{
		m_frame.fcurframe = 0;
	}

	if( (int)(m_frame.fcurframe) <= m_frame.umax)
		m_curtex = m_animtexture[m_texdiridx][int(m_frame.fcurframe)];
}

void CCom_TankbodyAnim::Render(void)
{
	if(NULL == m_curtex)
		return;

	m_pSprite->SetTransform(&m_objmat);
	if(TEAM_1 == m_pobj->GetTeamNumber())
	{
		m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0)
			, NULL , D3DCOLOR_ARGB(255,255,0,0));
	}
	else
	{
		m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0)
			, NULL , D3DCOLOR_ARGB(255,255,255,255));
	}
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

		//for(int i = 0; i < DIR_CNT; ++i)
		//{
			/* [i]는 방향 , DRONE, MOVE의 사진집합 */
			m_animtexture = CTextureMgr::GetInstance()->GetTUnitTexture(m_objname , statekey );
			//if(NULL == m_animtexture[i])
				//break;
		//}

		if(NULL != m_animtexture)
		{
			m_frame.umax = m_animtexture[0].size();
			m_frame.fframespeed = (float)m_frame.umax*2;
			m_curtex = m_animtexture[m_texdiridx][0];
		}
	}
}
