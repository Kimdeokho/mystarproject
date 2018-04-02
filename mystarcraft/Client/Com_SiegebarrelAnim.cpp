#include "StdAfx.h"
#include "Com_SiegebarrelAnim.h"

#include "TextureMgr.h"
#include "TimeMgr.h"
#include "MyMath.h"
#include "Obj.h"
CCom_SiegebarrelAnim::CCom_SiegebarrelAnim(D3DXMATRIX& objmat)
:CCom_Animation(objmat)
{
	m_banim_end = false;
	m_btransforming = false;
	m_bsiegemode = false;
	m_objname = L"SIEGEBARREL";
	m_statkey = L"";
}

CCom_SiegebarrelAnim::~CCom_SiegebarrelAnim(void)
{
}

void CCom_SiegebarrelAnim::Initialize(CObj* pobj)
{
	m_banim_end = false;
	m_btransforming = false;
	m_bsiegemode = false;
	m_statkey = L"";

	m_pobj = pobj;
	m_rotation_speed = 40;
	m_curdiridx = 0;

	SetAnimation(L"SIEGEBARREL_TRANS");

	CCom_Animation::InitTexidx();

}

void CCom_SiegebarrelAnim::Update(void)
{	

	//if(NULL == m_animtexture)
	//{
	//	m_curtex = NULL;
	//	return;
	//}

	if(true == m_btransforming)
	{	//정변
		m_frame.fcurframe += GETTIME*m_frame.fframespeed;

		if(false == m_bsiegemode)
		{
			if(m_frame.fcurframe >= m_frame.umax)
			{
				m_frame.fcurframe = 0;
				m_banim_end = true;
				m_bsiegemode = true;
				SetAnimation(L"IDLE");
				//m_pobj->SetState(IDLE);

				//여기서 0은 수학좌표계 +X축에 해당한다
				// 16은 -X축

				m_pobj->Setdir(CMyMath::dgree_to_dir(20*11.25f));
				m_curdiridx = 28;
				return;
			}
			else
				m_banim_end = false;

			m_curtex = (*m_generaltex)[int(m_frame.fcurframe)];
		}
		else
		{
			CCom_Animation::DirIdxCalculation();

			if(m_frame.fcurframe >= m_frame.umax)
			{
				m_frame.fcurframe = 0;
			}

			if( (int)(m_frame.fcurframe) <= m_frame.umax)
				m_curtex = m_animtexture[m_texdiridx][int(m_frame.fcurframe)];
		}
	}
	else
	{
		//역변
		if(true == m_bsiegemode)
		{
			m_frame.fcurframe -= GETTIME*m_frame.fframespeed;
			m_objmat._11 = 1;
			m_objmat._22 = 1;

			if(m_frame.fcurframe < 0)
			{
				m_frame.fcurframe = float(m_frame.umax - 1);
				m_banim_end = true;
				m_bsiegemode = false;
				SetAnimation(L"IDLE");

				return;
			}
			else
				m_banim_end = false;

			m_curtex = (*m_generaltex)[int(m_frame.fcurframe)];

		}
		else
		{
			//여기서 0은 수학좌표계 +X축에 해당한다
			// 16은 -X축

			m_pobj->SetState(TRANSFORMING);
			m_pobj->Setdir(CMyMath::dgree_to_dir(4*11.25f));
		}
	}
}

void CCom_SiegebarrelAnim::Render(void)
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

void CCom_SiegebarrelAnim::SetAnimation(const TCHAR* statekey)
{
	if(m_statkey != statekey)/*새로 들어오는 상태가 기존들어오는 상태와 다른상태여야 한다.*/
	{
		m_statkey = statekey;
		m_frame.fcurframe = 0;

		if(L"SIEGEBARREL_TRANS" == statekey)
		{
			m_banim_end = false;
			m_generaltex = CTextureMgr::GetInstance()->GetGeneralTexture(L"SIEGEBARREL_TRANS");

			if(false == m_bsiegemode)
			{
				m_frame.umax = m_generaltex->size();
				m_frame.fframespeed = float(m_frame.umax);
				m_btransforming = true;
			}
			else
			{
				m_frame.umax = m_generaltex->size();
				m_frame.fframespeed = float(m_frame.umax);
				m_frame.fcurframe = float(m_frame.umax - 1);
				m_btransforming = false;
			}
			m_curtex = (*m_generaltex)[ (int)(m_frame.fcurframe) ];
		}
		else
		{
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
				m_frame.fframespeed = (float)m_frame.umax;
			}
		}

	}
}
void CCom_SiegebarrelAnim::Release(void)
{

}

