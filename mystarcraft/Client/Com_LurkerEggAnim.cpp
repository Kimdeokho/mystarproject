#include "StdAfx.h"
#include "Com_LurkerEggAnim.h"

#include "TimeMgr.h"
#include "TextureMgr.h"

#include "Obj.h"

CCom_LurkerEggAnim::CCom_LurkerEggAnim(D3DXMATRIX& objmat )
: CCom_Animation(objmat) 
{
	m_objname = L"Z_LURKER_EGG";
	m_statkey = L"";
}

CCom_LurkerEggAnim::~CCom_LurkerEggAnim(void)
{
}

void CCom_LurkerEggAnim::Initialize(CObj* pobj)
{
	m_pobj = pobj;
	m_battack_sync = false;
	SetAnimation(L"MUTATE");	

	m_pobj->SetState(MUTATE);
	m_rotation_speed = 80.f;
}

void CCom_LurkerEggAnim::Update(void)
{
	m_frame.fcurframe += GETTIME*m_frame.fframespeed;

	if(m_frame.fcurframe >= m_frame.umax)
	{
		m_frame.fcurframe = 0.f;
		if(L"MUTATE" == m_statkey)
		{
			m_pobj->SetState(IDLE);
			SetAnimation(L"IDLE");
		}
		else if(StrStrW(m_statkey , L"BIRTH"))
		{
			m_pobj->SetDestroy(true);
		}		
	}

	if( (int)(m_frame.fcurframe) <= m_frame.umax)
		m_curtex = (*m_generaltex)[int(m_frame.fcurframe)];
}

void CCom_LurkerEggAnim::Render(void)
{
	m_pSprite->SetTransform(&m_objmat);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &m_vcenter
		, NULL , D3DCOLOR_ARGB(255, 255, 255, 255));
}
void CCom_LurkerEggAnim::SetAnimation(const TCHAR* statekey)
{
	if(m_statkey != statekey)/*새로 들어오는 상태가 기존들어오는 상태와 다른상태여야 한다.*/
	{
		m_statkey = statekey;

		m_generaltex = CTextureMgr::GetInstance()->GetStateTexture_vecset( m_objname , m_statkey);

		if(NULL != m_generaltex)
		{
			m_frame.fcurframe = 0;

			m_frame.umax = m_generaltex->size();

			if(L"MUTATE" == m_statkey)
				m_frame.fframespeed = float(m_frame.umax) * 2.5f;
			//else if(L"READY" == m_statkey)
			//	m_frame.fframespeed = float(m_frame.umax) * 3.f;
			else if(StrStrW(m_statkey , L"BIRTH"))
				m_frame.fframespeed = float(m_frame.umax) * 2.f;
			else
				m_frame.fframespeed = (float)m_frame.umax;

			m_curtex = (*m_generaltex)[0];
			m_vcenter = D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2), 0.f);
		}
	}
}

void CCom_LurkerEggAnim::Release(void)
{

}

