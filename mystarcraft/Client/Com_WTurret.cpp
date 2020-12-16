#include "StdAfx.h"
#include "Com_WTurret.h"

#include "Obj.h"
#include "Com_Animation.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "GeneraEff.h"
#include "Turret_bullet.h"

CCom_WTurret::CCom_WTurret(void)
{

}

CCom_WTurret::~CCom_WTurret(void)
{
}

void CCom_WTurret::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	m_animation = (m_pobj->GetComponent(COM_ANIMATION));

	m_attack_delay = 0.625f;
	m_weapon_info.damage = 20;
	m_weapon_info.airdamage = 6;
	m_weapon_info.eDamageType = DAMAGE_BOOM;
	m_weapon_info.eAirDamageType = DAMAGE_BOOM;

	m_bfire = false;

}

void CCom_WTurret::Update(void)
{
	if(m_bfire)
	{
		m_attack_time += GETTIME;
		if(m_attack_time > m_attack_delay)
		{
			m_attack_time = 0.f;
			m_bfire = false;
		}
	}
}
void CCom_WTurret::fire(CObj* ptarget)
{
	if(false == m_bfire)
	{
		m_pobj->SetState(ATTACK);

		//if( true == ((CCom_Animation*)m_animation)->GetRotationComplete())
		{			
			CSoundDevice::GetInstance()->PlayBattleSound(SND_B_TURRET_FIRE , m_pobj->GetPos());
			m_bfire = true;

			m_attack_time = 0.f;
			//총알생성
			//타겟에게 데미지 ㄱㄱㄱㄱ
			CObj* pbullet = new CTurret_bullet(ptarget->GetObjNumber() , ptarget->GetPos());
			pbullet->SetPos(m_pobj->GetPos().x , m_pobj->GetPos().y - 15);
			pbullet->Initialize();

			CObjMgr::GetInstance()->AddObject(pbullet , OBJ_BULLET);

		}
	}
}

void CCom_WTurret::Render(void)
{

}

void CCom_WTurret::Release(void)
{

}

