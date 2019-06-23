#include "StdAfx.h"
#include "Skill_irradi.h"

#include "Obj.h"
#include "Device.h"
#include "TimeMgr.h"
#include "TextureMgr.h"
#include "Area_Mgr.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
CSkill_irradi::CSkill_irradi(CObj* pobj , CObj* ptarget)
{
	m_pobj = pobj;
	m_ptarget = ptarget;
}

CSkill_irradi::~CSkill_irradi(void)
{
}
void CSkill_irradi::Initialize(void)
{
	m_skillname = SN_IRRADIATE;
	m_generatex = CTextureMgr::GetInstance()->GetGeneralTexture(L"IRRADIATE");

	m_damage_time = 0.f;
	m_fduration_time = 0.f;

	m_frame.fcurframe = 0;
	m_frame.umax = m_generatex->size();
	m_frame.fframespeed = float(m_frame.umax);

	m_targetid = m_ptarget->GetObjNumber();
	m_vPos = m_ptarget->GetPos();
	
	m_fsplash_range[0] = 2*32;
	m_fsplash_range[1] = 2*32;
	m_fsplash_range[2] = 2*32;

	m_curtex = (*m_generatex)[0];
}

void CSkill_irradi::Update(void)
{
	m_ptarget = CObjMgr::GetInstance()->obj_alivecheck(m_targetid);

	if(NULL == m_ptarget)
	{
		m_bdestroy = true;
		m_targetid = 0;
		return;
	}
	else
		m_vPos = m_ptarget->GetPos();

	m_fduration_time += GETTIME;

	if(m_fduration_time >= 25) //노말시간에 0.625를 곱한다
	{
		m_fduration_time = 0;
		m_bdestroy = true;
		return;
	}

	m_damage_time += GETTIME;
	if(m_damage_time > 0.3f)
	{
		m_damage_time = 0.f;

		//뎀지주기
		CArea_Mgr::GetInstance()->Setirradi_damage(m_pobj , 3 , DAMAGE_MAGIC , m_vPos , m_fsplash_range
			, true , m_injure_list);

		m_injure_list.clear();
	}



	m_frame.fcurframe += GETTIME * m_frame.fframespeed * 1.7f;
	if(m_frame.fcurframe > m_frame.umax )
	{
		m_frame.fcurframe = 0.f;
	}
	m_curtex = (*m_generatex)[ int(m_frame.fcurframe) ];
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;
}

void CSkill_irradi::Render(void)
{
	CDevice::GetInstance()->GetSprite()->SetTransform(&m_matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3( float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2) , 0) , 
		NULL , D3DCOLOR_ARGB(255,255,255,255));
}

void CSkill_irradi::Release(void)
{

}
