#include "StdAfx.h"
#include "Com_dirBulletAnim.h"

#include "TextureMgr.h"
#include "TimeMgr.h"
#include "Obj.h"
CCom_dirBulletAnim::CCom_dirBulletAnim(D3DXMATRIX& objmat , const TCHAR* objname)
:CCom_Animation(objmat)
{
	m_curtex = NULL;
	m_objname = objname;
	m_statkey = L"";
}

CCom_dirBulletAnim::~CCom_dirBulletAnim(void)
{
}

void CCom_dirBulletAnim::Initialize(CObj* pobj)
{
	m_rotation_speed = 120;
	m_pobj = pobj;
	SetAnimation(L"IDLE");
}

void CCom_dirBulletAnim::Update(void)
{
	CCom_Animation::DirIdxCalculation();

	m_frame.fcurframe += GETTIME*m_frame.fframespeed;

	if( int(m_frame.fcurframe) >= m_frame.umax)
	{
		m_frame.fcurframe = 0;
	}

	m_curtex = m_animtexture[m_texdiridx][int(m_frame.fcurframe)];
}

void CCom_dirBulletAnim::Render(void)
{
	if(NULL == m_curtex)
		return;

	m_pSprite->SetTransform(&m_objmat);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0)
		, NULL , D3DCOLOR_ARGB(255,255,255,255));
}

void CCom_dirBulletAnim::Release(void)
{

}

void CCom_dirBulletAnim::SetAnimation(const TCHAR* statekey)
{
	if(m_statkey != statekey)
	{
		if(false == m_battack_end)
			return;

		m_statkey = statekey;
		m_frame.fcurframe = 0;			

		/* [i]는 방향 , DRONE, MOVE의 사진집합 */
		m_animtexture = CTextureMgr::GetInstance()->GetTUnitTexture(m_objname , m_statkey );

		m_frame.umax = m_animtexture[0].size();
		m_frame.fframespeed = (float)m_frame.umax;
	}
}
