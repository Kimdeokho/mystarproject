#include "StdAfx.h"
#include "Com_LaserAnim.h"

#include "TextureMgr.h"
#include "TimeMgr.h"
#include "Obj.h"
CCom_LaserAnim::CCom_LaserAnim(D3DXMATRIX& objmat , const TCHAR* objname )
:CCom_Animation(objmat)
{
	m_objname = objname;
	m_statkey = L"";
}

CCom_LaserAnim::~CCom_LaserAnim(void)
{
}

void CCom_LaserAnim::Initialize(CObj* pobj)
{
	m_pobj = pobj;

	SetAnimation(L"IDLE");

	Dir_Initialize();
}

void CCom_LaserAnim::Update(void)
{
	m_frame.fcurframe += GETTIME*m_frame.fframespeed;

	if( int(m_frame.fcurframe) >= m_frame.umax)
	{
		m_frame.fcurframe = 0;
	}

	m_curtex = m_animtexture32[m_texdiridx][int(m_frame.fcurframe)];
}

void CCom_LaserAnim::Render(void)
{
	if(NULL == m_curtex)
		return;

	m_pSprite->SetTransform(&m_objmat);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0)
		, NULL , D3DCOLOR_ARGB(255,255,255,255));
}

void CCom_LaserAnim::Release(void)
{

}

void CCom_LaserAnim::SetAnimation(const TCHAR* statekey)
{
	if(m_statkey != statekey)
	{
		if(false == m_battack_end)
			return;

		m_statkey = statekey;
		m_frame.fcurframe = 0;			

		/* [i]는 방향 , DRONE, MOVE의 사진집합 */
		m_animtexture32 = CTextureMgr::GetInstance()->GetTUnitTexture(m_objname , m_statkey );

		m_frame.umax = m_animtexture32[0].size();
		m_frame.fframespeed = (float)m_frame.umax;
	}
}
