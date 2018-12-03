#include "StdAfx.h"
#include "Com_LurkerAnim.h"

#include "TimeMgr.h"
#include "TextureMgr.h"

#include "Lurker.h"
#include "Obj.h"
CCom_LurkerAnim::CCom_LurkerAnim(D3DXMATRIX& objmat)
: CCom_Animation(objmat )
{
	m_objname = L"LURKER";
	m_statkey = L"";
}

CCom_LurkerAnim::~CCom_LurkerAnim(void)
{
}

void CCom_LurkerAnim::Initialize(CObj* pobj)
{
	m_battack_sync = false;
	SetAnimation(L"IDLE");

	m_pobj = pobj;
	CCom_Animation::InitTexidx();

	m_rotation_speed = 80.f;	
}

void CCom_LurkerAnim::Update(void)
{
	CCom_Animation::DirIdxCalculation();	

	m_frame.fcurframe += GETTIME*m_frame.fframespeed;
	if(m_frame.fcurframe >= m_frame.umax)
	{
		if(m_statkey == L"DIG")
		{
			((CLurker*)m_pobj)->Burrow_Complete();
			SetAnimation(L"BURROW");
		}
		else if(m_statkey == L"UNBURROW")
		{
			m_pobj->SetState(IDLE);
			SetAnimation(L"IDLE");
		}
		m_frame.fcurframe = 0;
	}

	if( (int)(m_frame.fcurframe) <= m_frame.umax)
		m_curtex = m_animtexture[m_texdiridx][ int(m_frame.fcurframe) ];
}

void CCom_LurkerAnim::Render(void)
{
	if(NULL == m_curtex)
		return;

	if(L"BURROW" != m_statkey)
	{
		m_objshadow_mat = m_objmat;
		m_objshadow_mat._42 += 8;

		m_pSprite->SetTransform(&m_objshadow_mat);
		m_pSprite->Draw(m_curtex->pTexture , NULL , &m_vcenter
			, NULL , D3DCOLOR_ARGB(150,0,0,0));
	}


	m_pSprite->SetTransform(&m_objmat);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &m_vcenter
		, NULL , D3DCOLOR_ARGB(255,255,255,255));
}

void CCom_LurkerAnim::SetAnimation(const TCHAR* statekey)
{
	if(m_statkey != statekey)/*새로 들어오는 상태가 기존들어오는 상태와 다른상태여야 한다.*/
	{
		m_statkey = statekey;
		m_frame.fcurframe = 0;

		/* [i]는 방향 , DRONE, MOVE의 사진집합 */
		m_animtexture = CTextureMgr::GetInstance()->GetZUnitTexture(m_objname , statekey );

		if(NULL != m_animtexture)
		{
			m_frame.umax = m_animtexture[0].size();

			if(L"MOVE" == m_statkey)
				m_frame.fframespeed = (float)m_frame.umax*2.3f;
			else if(L"IDG" == m_statkey)
				m_frame.fframespeed = (float)m_frame.umax*3.3f;
			else if(L"UNBURROW" == m_statkey)
				m_frame.fframespeed = (float)m_frame.umax*4.3f;
			else
				m_frame.fframespeed = (float)m_frame.umax*1.3f;

			m_curtex = m_animtexture[m_texdiridx][ int(m_frame.fcurframe) ];
			m_vcenter = D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2), 0.f);
		}
	}
}

void CCom_LurkerAnim::Release(void)
{

}
