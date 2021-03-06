#include "StdAfx.h"
#include "Com_Weapon.h"

#include "Ingame_UIMgr.h"
CCom_Weapon::CCom_Weapon(void)
{
	m_attack_delay = 0.f;
	m_attack_time = 0.f;
	m_air_attack_delay = 0.f;
	m_air_attack_time = 0.f;
	m_bfire = false;
	m_bair_fire = false;

	m_upg_info = CIngame_UIMgr::GetInstance()->GetUpginfo();

	m_cur_target_num = -1;
	m_cur_target_cntnum = -1;
}

CCom_Weapon::~CCom_Weapon(void)
{
}

void CCom_Weapon::Update(void)
{

}

void CCom_Weapon::Render(void)
{

}

void CCom_Weapon::Release(void)
{

}

bool CCom_Weapon::GetFire(void)
{
	return m_bfire;
}

void CCom_Weapon::SetAttackTime(const float atttime)
{
	if(m_attack_time > m_attack_delay - 0.5f + atttime)
		m_attack_time = m_attack_delay - 0.5f + atttime;

	if(m_air_attack_time > m_air_attack_delay - 0.5f + atttime)
		m_air_attack_time = m_air_attack_delay - 0.5f + atttime;

}
void CCom_Weapon::SetAttackDelay(const float delay)
{
	m_attack_delay = delay;
}

const WEAPON_INFO& CCom_Weapon::GetWeapon_info(void)
{
	return m_weapon_info;
}
