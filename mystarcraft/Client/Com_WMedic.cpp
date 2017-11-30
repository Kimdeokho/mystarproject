#include "StdAfx.h"
#include "Com_WMedic.h"

#include "Obj.h"
#include "Com_Animation.h"
#include "TimeMgr.h"
#include "ObjMgr.h"

CCom_WMedic::CCom_WMedic()
{
}

CCom_WMedic::~CCom_WMedic(void)
{
}

void CCom_WMedic::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	m_animation = (m_pobj->GetComponent(COM_ANIMATION));

	m_attack_delay = 0.051f;
	m_weapon_info.damage = 1;
	m_weapon_info.eDamageType = DAMAGE_NOMAL;

	m_bfire = false;
}
void CCom_WMedic::fire(CObj*& ptarget)
{

	if(ptarget->GetUnitinfo().maxhp == ptarget->GetUnitinfo().hp)
	{
		m_pobj->SetState(IDLE);
	}
	else
	{
		if(false == m_bfire)
		{
			m_pobj->SetState(HEALING);
			m_bfire = true;
			m_attack_time = 0.f;

			m_pobj->SetMP(-1);

			if(m_pobj->GetUnitinfo().mp > 0)
				(ptarget)->SetDamage( -m_weapon_info.damage , m_weapon_info.eDamageType);
		}
	}
}
void CCom_WMedic::Update(void)
{
	if(m_bfire)
	{
		m_attack_time += GETTIME;
		if(m_attack_time > m_attack_delay) //스팀빨면 이게 줄겠지
		{
			m_attack_time = 0.f;
			m_bfire = false;
		}
	}
}

void CCom_WMedic::Render(void)
{

}

void CCom_WMedic::Release(void)
{

}


