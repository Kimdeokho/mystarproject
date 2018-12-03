#include "StdAfx.h"
#include "Skill_Defensive.h"

#include "Obj.h"
#include "Device.h"
#include "TimeMgr.h"
#include "TextureMgr.h"
#include "Area_Mgr.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"

CSkill_Defensive::CSkill_Defensive(CObj* pobj , CObj* ptarget)
{
	m_pobj = pobj;
	m_ptarget = ptarget;
}

CSkill_Defensive::~CSkill_Defensive(void)
{
}

void CSkill_Defensive::Initialize(void)
{
	m_skillname = SN_DEFENSIVE;
	m_vPos = m_ptarget->GetPos();
	m_targetid = m_ptarget->GetObjNumber();
	m_unitsize = m_ptarget->GetUnitinfo().esize;

	if(SIZE_SMALL == m_unitsize)
	{
		m_generatex = CTextureMgr::GetInstance()->GetGeneralTexture(L"DEFEN_S");
		m_generaback_tex = CTextureMgr::GetInstance()->GetGeneralTexture(L"DEFEN_BS");
	}
	else if(SIZE_MEDIUM == m_unitsize)
	{
		m_generatex = CTextureMgr::GetInstance()->GetGeneralTexture(L"DEFEN_M");
		m_generaback_tex = CTextureMgr::GetInstance()->GetGeneralTexture(L"DEFEN_BM");
	}
	else if(SIZE_LARGE == m_unitsize)
	{
		m_generatex = CTextureMgr::GetInstance()->GetGeneralTexture(L"DEFEN_L");
		m_generaback_tex = CTextureMgr::GetInstance()->GetGeneralTexture(L"DEFEN_BL");
	}
	

	m_frame.fcurframe = 0;
	m_frame.umax = m_generatex->size();
	m_frame.fframespeed = float(m_frame.umax);

	m_curtex = ((*m_generatex)[0]);
	m_backtex = ((*m_generaback_tex)[0]);

	m_matWorld._11 = 1.f;
	m_matWorld._22 = 1.f;

	m_shild_hp = 255;

	m_defensive_time = 0.f;
}

void CSkill_Defensive::Update(void)
{
	m_ptarget = CObjMgr::GetInstance()->obj_alivecheck(m_targetid);

	if(NULL == m_ptarget || m_shild_hp <= 0)
	{
		m_bdestroy = true;
		m_targetid = 0;
		return;
	}
	else
		m_vPos = m_ptarget->GetPos();

	m_defensive_time += GETTIME;
	m_frame.fcurframe += GETTIME*m_frame.fframespeed*3.5f;

	if(m_defensive_time > 60.f)
	{
		m_bdestroy = true;
		return;
	}

	if(m_frame.fcurframe > m_frame.umax )
		m_frame.fcurframe = 0.f;

	m_curtex = ((*m_generatex)[ (int)m_frame.fcurframe ]);
	m_backtex = ((*m_generaback_tex)[ (int)m_frame.fcurframe ]);
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;
}

void CSkill_Defensive::Render(void)
{
	CDevice::GetInstance()->GetSprite()->SetTransform(&m_matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3( float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2) , 0) , 
		NULL , D3DCOLOR_ARGB(255,255,255,255));
	CDevice::GetInstance()->GetSprite()->Draw(m_backtex->pTexture , NULL , &D3DXVECTOR3( float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2) , 0) , 
		NULL , D3DCOLOR_ARGB(255,255,255,255));
}

void CSkill_Defensive::SetDamage(const int& shild)
{
	m_shild_hp -= shild;
}
int CSkill_Defensive::GetShild(void)
{
	return m_shild_hp;
}

void CSkill_Defensive::Release(void)
{

}
