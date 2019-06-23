#include "StdAfx.h"
#include "Skill_SP.h"

#include "TimeMgr.h"
#include "Com_Weapon.h"
#include "Obj.h"
CSkill_SP::CSkill_SP(void)
{
}

CSkill_SP::CSkill_SP(CObj* pobj , CComponent* pweapon)
{
	m_pobj = pobj;
	m_com_weapon = pweapon;
	m_sptime = 0.f;
	m_bstimpack = false;
	m_bonce= false;
}

CSkill_SP::~CSkill_SP(void)
{
	m_com_weapon = NULL;
}

void CSkill_SP::Initialize(void)
{
}
void CSkill_SP::Update(void)
{
	if(true == m_bstimpack)
	{
		m_sptime += GETTIME;

		if(m_sptime >= 10.f)
		{
			m_sptime = 0.f;
			m_bstimpack = false;
		}
	}
	else
	{
		if(true == m_bonce)
		{
			m_bonce = false;
			((CCom_Weapon*)m_com_weapon)->SetAttackDelay(0.625f);
			m_pobj->SetSpeed(60);
		}
	}
}
void CSkill_SP::use_sp(void)
{
	if( m_pobj->GetUnitinfo().hp - 10 > 10)
	{
		m_pobj->SetHP(-10);
		m_bonce = true;
		m_bstimpack = true;
		((CCom_Weapon*)m_com_weapon)->SetAttackDelay(0.375f);
		m_pobj->SetSpeed(98);
	}
}


