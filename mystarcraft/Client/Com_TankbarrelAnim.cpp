#include "StdAfx.h"
#include "Com_TankbarrelAnim.h"

#include "TimeMgr.h"
#include "TextureMgr.h"
#include "FontMgr.h"
#include "Obj.h"

CCom_TankbarrelAnim::CCom_TankbarrelAnim(D3DXMATRIX& objmat )
:CCom_Animation(objmat )
{
	m_objname = L"TANKBARREL";
	m_statkey = L"";
}

CCom_TankbarrelAnim::~CCom_TankbarrelAnim(void)
{
}

void CCom_TankbarrelAnim::Initialize(CObj* pobj)
{
	SetAnimation(L"IDLE");
	m_pobj = pobj;
	m_rotation_speed = 20;
	m_curdiridx = 6;

	CCom_Animation::InitTexidx();
}

void CCom_TankbarrelAnim::Update(void)
{
	CCom_Animation::DirIdxCalculation();

	m_frame.fcurframe += GETTIME*m_frame.fframespeed;
	if(m_frame.fcurframe >= m_frame.umax)
		m_frame.fcurframe = 0;


	if(NULL !=  m_animtexture[m_texdiridx] )
	{
		const vector<TEXINFO*>* vtemp = m_animtexture[m_texdiridx];

		if( (int)(m_frame.fcurframe) <= m_frame.umax)
			m_curtex = (*vtemp)[int(m_frame.fcurframe)];
	}
	else
		m_curtex = NULL;

	//CFontMgr::GetInstance()->Setbatch_Font(L"%d",m_texdiridx , m_pobj->GetMat()._41 , m_pobj->GetMat()._42);
}
void CCom_TankbarrelAnim::Render(void)
{
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

void CCom_TankbarrelAnim::Release(void)
{

}

void CCom_TankbarrelAnim::SetAnimation(const TCHAR* statekey)
{
	if(m_statkey != statekey)/*새로 들어오는 상태가 기존들어오는 상태와 다른상태여야 한다.*/
	{
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
			m_frame.fframespeed = (float)m_frame.umax;
		}
	}
}
