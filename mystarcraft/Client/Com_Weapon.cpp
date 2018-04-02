#include "StdAfx.h"
#include "Com_Weapon.h"

#include "ComanderMgr.h"
CCom_Weapon::CCom_Weapon(void)
{
	m_attack_delay = 0.f;
	m_attack_time = 0.f;
	m_air_attack_delay = 0.f;
	m_air_attack_time = 0.f;
	m_bfire = false;
	m_bair_fire = false;

	m_upg_info = CComanderMgr::GetInstance()->GetUpginfo();
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

void CCom_Weapon::SetAttackDelay(float delay)
{
	m_attack_delay = delay;
}

const WEAPON_INFO& CCom_Weapon::GetWeapon_info(void)
{
	return m_weapon_info;
}
