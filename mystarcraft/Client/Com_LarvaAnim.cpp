#include "StdAfx.h"
#include "Com_LarvaAnim.h"

#include "TimeMgr.h"
#include "TextureMgr.h"
CCom_LarvaAnim::CCom_LarvaAnim(D3DXMATRIX& objmat)
: CCom_Animation(objmat ) 
{
	m_objname = L"LARVA";
	m_statkey = L"";
}

CCom_LarvaAnim::~CCom_LarvaAnim(void)
{
}
void CCom_LarvaAnim::Initialize(CObj* pobj)
{
	m_battack_sync = false;
	SetAnimation(L"IDLE");

	m_pobj = pobj;
	CCom_Animation::InitTexidx();

	m_rotation_speed = 80.f;	
}

void CCom_LarvaAnim::Update(void)
{
	CCom_Animation::DirIdxCalculation();

	m_frame.fcurframe += GETTIME*m_frame.fframespeed;	
	if(m_frame.fcurframe >= m_frame.umax)
	{
		m_frame.fcurframe = 0;
	}	

	if( (int)(m_frame.fcurframe) <= m_frame.umax)
		m_curtex = m_animtexture[m_texdiridx][int(m_frame.fcurframe)];

}

void CCom_LarvaAnim::Render(void)
{
	if(NULL == m_curtex)
		return;

	m_pSprite->SetTransform(&m_objmat);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &m_vcenter
		, NULL , D3DCOLOR_ARGB(255,255,255,255));
}
void CCom_LarvaAnim::SetAnimation(const TCHAR* statekey)
{
	if(m_statkey != statekey)/*���� ������ ���°� ���������� ���¿� �ٸ����¿��� �Ѵ�.*/
	{
		m_statkey = statekey;
		m_frame.fcurframe = 0;

		/* [i]�� ���� , DRONE, MOVE�� �������� */
		m_animtexture = CTextureMgr::GetInstance()->GetZUnitTexture(m_objname , statekey );

		if(NULL != m_animtexture)
		{
			m_frame.umax = m_animtexture[0].size();

			m_frame.fframespeed = (float)m_frame.umax*1.3f;

			m_curtex = m_animtexture[m_texdiridx][ int(m_frame.fcurframe) ];

			m_vcenter = D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2), 0.f);
		}
	}
}

void CCom_LarvaAnim::Release(void)
{

}
