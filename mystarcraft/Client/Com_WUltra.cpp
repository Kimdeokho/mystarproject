#include "StdAfx.h"
#include "Com_WUltra.h"

#include "Obj.h"

#include "TimeMgr.h"
#include "ObjMgr.h"

#include "Com_Animation.h"

CCom_WUltra::CCom_WUltra(void)
{
}

CCom_WUltra::~CCom_WUltra(void)
{
}

void CCom_WUltra::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	m_animation = (m_pobj->GetComponent(COM_ANIMATION));

	m_attack_delay = 0.625f;

	m_bfire = true;

	m_weapon_info.damage = 20;
	m_weapon_info.eDamageType = DAMAGE_NOMAL;

	m_attack_time = m_attack_delay;
	m_attack_on = false;
	m_target_cntnum = 0;
	m_target_id = 0;
}

void CCom_WUltra::Update(void)
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

void CCom_WUltra::fire(CObj* ptarget)
{
	if( false == m_bfire)
	{
		m_attack_on = true;
		m_pobj->SetState(IDLE);
		m_target_cntnum = ptarget->GetObjCountNumber();
		m_target_id = ptarget->GetObjNumber();
	}

	if(m_attack_on)
	{
		CObj* ptarget = CObjMgr::GetInstance()->obj_alivecheck(m_target_id);

		if(NULL != ptarget && m_target_cntnum == ptarget->GetObjCountNumber())
		{
			if(true == ((CCom_Animation*)m_animation)->GetRotationComplete())
			{				
				CSoundDevice::GetInstance()->PlayBattleSound(SND_B_UTATT , m_pobj->GetPos());
				m_pobj->SetState(ATTACK);
				m_attack_time = 0.f;			

				//if(true == ((CCom_Animation*)m_animation)->GetAttackSync())
				{
					m_bfire = true;				
					ptarget->SetDamage( m_weapon_info.damage , m_weapon_info.eDamageType );
					m_attack_on = false;
				}
			}
		}
		else
		{
			m_attack_time = m_attack_delay;
			m_attack_on = false;
			m_target_cntnum = 0;
			m_target_id = 0;
		}
	}
}

void CCom_WUltra::Render(void)
{

}

void CCom_WUltra::Release(void)
{

}
