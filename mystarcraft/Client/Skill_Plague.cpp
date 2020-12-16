#include "StdAfx.h"
#include "Skill_Plague.h"

#include "Obj.h"
#include "Device.h"
#include "TimeMgr.h"
#include "TextureMgr.h"
#include "Area_Mgr.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"

CSkill_Plague::CSkill_Plague(CObj* ptarget)
{
	m_ptarget = ptarget;
}

CSkill_Plague::~CSkill_Plague(void)
{
}

void CSkill_Plague::Initialize(void)
{
	m_skillname = SN_PLAGUE;
	m_vPos = m_ptarget->GetPos();
	m_targetid = m_ptarget->GetObjNumber();
	m_unitsize = m_ptarget->GetUnitinfo().esize;

	if(SIZE_SMALL == m_unitsize)
		m_generatex = CTextureMgr::GetInstance()->GetGeneralTexture(L"PLAGUE_S");
	else if(SIZE_MEDIUM == m_unitsize)
		m_generatex = CTextureMgr::GetInstance()->GetGeneralTexture(L"PLAGUE_M");
	else if(SIZE_LARGE == m_unitsize)
		m_generatex = CTextureMgr::GetInstance()->GetGeneralTexture(L"PLAGUE_L");

	m_frame.fcurframe = 0;
	m_frame.umax = m_generatex->size();
	m_frame.fframespeed = float(m_frame.umax);

	m_curtex = ((*m_generatex)[0]);

	m_matWorld._11 = 1.f;
	m_matWorld._22 = 1.f;

	m_plague_time = 0.f;
	m_damage_tick = 1.f;


	for(int i = 0; i < TEAM_END; ++i)
	{
		if(i != m_ptarget->GetTeamNumber())
			m_ptarget->Get_ref_Unitinfo().detect[i] += 1;
	}
}

void CSkill_Plague::Update(void)
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

	m_plague_time += GETTIME;
	m_damage_tick += GETTIME;
	m_frame.fcurframe += GETTIME*m_frame.fframespeed*2.5f;

	if(m_plague_time > 30.f)
	{
		m_bdestroy = true;
		return;
	}

	//몇초에 한번씩
	if(m_damage_tick > 0.3f)
	{
		m_ptarget->SetHP(-3);

		if(m_ptarget->GetUnitinfo().hp <= 1)
			m_ptarget->Get_ref_Unitinfo().hp = 1;
			

		m_damage_tick = 0.f;		
	}
	

	if(m_frame.fcurframe > m_frame.umax )
		m_frame.fcurframe = 0.f;

	m_curtex = ((*m_generatex)[ (int)m_frame.fcurframe ]);
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;
}

void CSkill_Plague::Render(void)
{
	CDevice::GetInstance()->GetSprite()->SetTransform(&m_matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3( float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2) , 0) , 
		NULL , D3DCOLOR_ARGB(255,255,255,255));
}

void CSkill_Plague::Release(void)
{
	for(int i = 0; i < TEAM_END; ++i)
	{
		if(i != m_ptarget->GetTeamNumber())
			m_ptarget->Get_ref_Unitinfo().detect[i] -= 1;
	}
}
