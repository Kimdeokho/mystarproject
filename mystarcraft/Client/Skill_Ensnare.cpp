#include "StdAfx.h"
#include "Skill_Ensnare.h"

#include "Obj.h"
#include "Device.h"
#include "TimeMgr.h"
#include "TextureMgr.h"
#include "Area_Mgr.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"

CSkill_Ensnare::CSkill_Ensnare(CObj* ptarget)
{
	m_ptarget = ptarget;
}

CSkill_Ensnare::~CSkill_Ensnare(void)
{
	Release();
}

void CSkill_Ensnare::Initialize(void)
{
	m_skillname = SN_ENSNARE;
	m_vPos = m_ptarget->GetPos();
	m_targetid = m_ptarget->GetObjNumber();
	m_unitsize = m_ptarget->GetUnitinfo().esize;

	if(SIZE_SMALL == m_unitsize)
		m_generatex = CTextureMgr::GetInstance()->GetGeneralTexture(L"ENSNARE_S");
	else if(SIZE_MEDIUM == m_unitsize)
		m_generatex = CTextureMgr::GetInstance()->GetGeneralTexture(L"ENSNARE_M");
	else if(SIZE_LARGE == m_unitsize)
		m_generatex = CTextureMgr::GetInstance()->GetGeneralTexture(L"ENSNARE_L");

	m_frame.fcurframe = 0;
	m_frame.umax = m_generatex->size();
	m_frame.fframespeed = float(m_frame.umax);

	m_curtex = ((*m_generatex)[0]);

	m_matWorld._11 = 1.f;
	m_matWorld._22 = 1.f;

	m_ensnare_time = 0.f;

	m_ori_speed = m_ptarget->GetUnitinfo().fspeed;
	m_ptarget->Get_ref_Unitinfo().fspeed = m_ori_speed / 2.5f;

	for(int i = 0; i < TEAM_END; ++i)
	{
		if(i != m_ptarget->GetTeamNumber())
			m_ptarget->Get_ref_Unitinfo().detect[i] += 1;
	}
}

void CSkill_Ensnare::Update(void)
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

	m_ensnare_time += GETTIME;
	m_frame.fcurframe += GETTIME*m_frame.fframespeed*2.5f;

	if(m_ensnare_time > 30.f)
	{
		m_ptarget->Get_ref_Unitinfo().fspeed = m_ori_speed;
		m_bdestroy = true;
		return;
	}

	if(m_frame.fcurframe > m_frame.umax )
		m_frame.fcurframe = 0.f;

	m_curtex = ((*m_generatex)[ (int)m_frame.fcurframe ]);
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;
}

void CSkill_Ensnare::Render(void)
{
	CDevice::GetInstance()->GetSprite()->SetTransform(&m_matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3( float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2) , 0) , 
		NULL , D3DCOLOR_ARGB(255,255,255,255));
}

void CSkill_Ensnare::Release(void)
{
	for(int i = 0; i < TEAM_END; ++i)
	{
		if(i != m_ptarget->GetTeamNumber())
			m_ptarget->Get_ref_Unitinfo().detect[i] -= 1;
	}
}
