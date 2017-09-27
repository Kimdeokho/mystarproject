#include "StdAfx.h"
#include "Com_SiegebarrelAnim.h"

#include "TextureMgr.h"
#include "TimeMgr.h"
#include "MyMath.h"
#include "Obj.h"
CCom_SiegebarrelAnim::CCom_SiegebarrelAnim(D3DXMATRIX& objmat , TEXINFO*& curtex)
:CCom_Animation(objmat , curtex)
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
	m_pobj = pobj;
	m_rotation_speed = 20;
	m_curdiridx = 14;

	SetAnimation(L"SIEGEBARREL_TRANS");

}

void CCom_SiegebarrelAnim::Update(void)
{	

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


				float fdgree = 10*22.5f;
				D3DXVECTOR2	vdir;
				vdir.x = cosf(CMyMath::dgree_to_radian(fdgree));
				vdir.y = sinf(CMyMath::dgree_to_radian(fdgree));
				m_pobj->Setdir(vdir);

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


			if(NULL !=  m_animtexture[m_texdiridx] )
			{
				const vector<TEXINFO*>* vtemp = m_animtexture[m_texdiridx];

				if( (int)(m_frame.fcurframe) <= m_frame.umax)
					m_curtex = (*vtemp)[int(m_frame.fcurframe)];
			}
			else
				m_curtex = NULL;
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
			m_pobj->SetState(TRANSFORMING);
			m_pobj->Setdir(CMyMath::dgree_to_dir(2*22.5f));
		}
	}
}

void CCom_SiegebarrelAnim::Render(void)
{

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
		}
		else
		{
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
}
void CCom_SiegebarrelAnim::Release(void)
{

}

