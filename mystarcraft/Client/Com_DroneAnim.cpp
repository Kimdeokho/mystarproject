#include "StdAfx.h"
#include "Com_DroneAnim.h"

#include "TimeMgr.h"
#include "TextureMgr.h"
CCom_DroneAnim::CCom_DroneAnim(D3DXMATRIX& objmat )
: CCom_Animation(objmat ) 
{
	m_objname = L"DRONE";
	m_statkey = L"";
	//lstrcpy(m_objname ,L"DRONE");
	//memset(m_statkey , 0 , sizeof(TCHAR)*64);
}

CCom_DroneAnim::~CCom_DroneAnim(void)
{
}

void CCom_DroneAnim::Initialize(CObj* pobj)
{
	SetAnimation(L"IDLE");

	m_pobj = pobj;
	CCom_Animation::InitTexidx();
}

void CCom_DroneAnim::Update(void)
{
	CCom_Animation::DirIdxCalculation();

	if(m_frame.fcurframe >= m_frame.umax)
	{
		m_frame.fcurframe = 0;
	}
	m_frame.fcurframe += GETTIME*m_frame.fframespeed;	

	//const vector<TEXINFO*> vtemp = m_animtexture[m_texdiridx];

	if( (int)(m_frame.fcurframe) <= m_frame.umax)
		m_curtex = m_animtexture[m_texdiridx][int(m_frame.fcurframe)];

}

void CCom_DroneAnim::Render(void)
{

}

void CCom_DroneAnim::Release(void)
{

}

void CCom_DroneAnim::SetAnimation(const TCHAR* statekey)
{
	if(m_statkey != statekey)/*���� ������ ���°� ���������� ���¿� �ٸ����¿��� �Ѵ�.*/
	{
		m_statkey = statekey;

		m_frame.fcurframe = 0;
			/* [i]�� ���� , DRONE, MOVE�� �������� */
		m_animtexture = CTextureMgr::GetInstance()->GetZUnitTexture(m_objname , statekey );


		if(NULL != m_animtexture)
		{
			m_frame.umax = m_animtexture[0].size() - 1;
			m_frame.fframespeed = (float)m_frame.umax;
		}
	}
}
