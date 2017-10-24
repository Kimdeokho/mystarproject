#include "StdAfx.h"
#include "Com_SiegebodyAnim.h"
#include "TextureMgr.h"
#include "TimeMgr.h"

CCom_SiegebodyAnim::CCom_SiegebodyAnim(D3DXMATRIX& objmat , TEXINFO*& curtex)
: CCom_Animation(objmat , curtex)
{
	m_banim_end = false;
	m_bsiegemode = false;
	m_objname = L"SIEGEBODY";
	m_statkey = L"";
}

CCom_SiegebodyAnim::~CCom_SiegebodyAnim(void)
{
}

void CCom_SiegebodyAnim::Initialize(CObj* pobj)
{
	m_pobj = pobj;

	SetAnimation(L"SIEGEBODY_TRANS");

}

void CCom_SiegebodyAnim::Update(void)
{
	//회전할필요 없다

	if(true == m_btransforming)
	{
		//정변

		m_frame.fcurframe += GETTIME*m_frame.fframespeed;

		if(false == m_bsiegemode)
		{
			if(m_frame.fcurframe >= m_frame.umax)
			{
				m_frame.fcurframe = 0;
				m_banim_end = true;
				m_bsiegemode = true;
				SetAnimation(L"IDLE");
				return;
			}
			else
				m_banim_end = false;
			m_curtex = (*m_generaltex)[int(m_frame.fcurframe)];

		}
		else
		{
			if(m_frame.fcurframe >= m_frame.umax)
			{
				m_frame.fcurframe = 0;
			}

			if( NULL !=  m_animtexture[0] )
			{
				const vector<TEXINFO*>* vtemp = m_animtexture[0];

				if( (int)(m_frame.fcurframe) <= m_frame.umax)
					m_curtex = (*vtemp)[int(m_frame.fcurframe)];
			}
			else
				m_curtex = NULL;
		}

	}
	else
	{
		m_frame.fcurframe -= GETTIME*m_frame.fframespeed;

		if(true == m_bsiegemode)
		{
			if(m_frame.fcurframe < 0)
			{
				m_frame.fcurframe = 0;
				m_banim_end = true;
				SetAnimation(L"IDLE");
				return;
			}
			else
				m_banim_end = false;
			m_curtex = (*m_generaltex)[int(m_frame.fcurframe)];

		}
	}


}

void CCom_SiegebodyAnim::Render(void)
{

}

void CCom_SiegebodyAnim::SetAnimation(const TCHAR* statekey)
{
	if(m_statkey != statekey)
	{
		m_statkey = statekey;
		m_frame.fcurframe = 0.f;
		

		if(L"SIEGEBODY_TRANS" == statekey)
		{
			m_banim_end = false;
			m_generaltex = CTextureMgr::GetInstance()->GetGeneralTexture(L"SIEGEBODY_TRANS");

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
				m_animtexture[i] = CTextureMgr::GetInstance()->GetTUnitTexture(m_objname , statekey , i);
			}

			m_frame.umax = m_animtexture[0]->size();
			m_frame.fframespeed = (float)m_frame.umax;
		}
	}
}

void CCom_SiegebodyAnim::Release(void)
{

}

