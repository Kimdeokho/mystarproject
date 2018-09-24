#include "StdAfx.h"
#include "Com_WZergling.h"

#include "Obj.h"

#include "TimeMgr.h"
#include "ObjMgr.h"

#include "Com_Animation.h"

CCom_WZergling::CCom_WZergling(void)
{
}

CCom_WZergling::~CCom_WZergling(void)
{
}

void CCom_WZergling::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	m_animation = (m_pobj->GetComponent(COM_ANIMATION));

	m_attack_delay = 0.33333f;

	m_bfire = true;

	m_weapon_info.damage = 5;
	m_weapon_info.eDamageType = DAMAGE_NOMAL;
}

void CCom_WZergling::Update(void)
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

void CCom_WZergling::Render(void)
{

}

void CCom_WZergling::fire(CObj*& ptarget)
{
	if( false == m_bfire)
	{		
		m_pobj->SetState(IDLE);
		if(true == ((CCom_Animation*)m_animation)->GetRotationComplete())
		{				
			m_pobj->SetState(ATTACK);
			m_attack_time = 0.f;			

			m_targetpos = ptarget->GetPos();

			if(true == ((CCom_Animation*)m_animation)->GetAttackSync())
			{
				m_bfire = true;
				(ptarget)->SetDamage( m_weapon_info.damage , m_weapon_info.eDamageType );
			}
		}
	}
}
void CCom_WZergling::Release(void)
{

}