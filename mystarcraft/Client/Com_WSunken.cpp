#include "StdAfx.h"
#include "Com_WSunken.h"

#include "Obj.h"
#include "Com_Animation.h"
#include "TimeMgr.h"
#include "ObjMgr.h"

#include "GeneraEff.h"

#include "Skill_DarkSwarm.h"

CCom_WSunken::CCom_WSunken(void)
{
}

CCom_WSunken::~CCom_WSunken(void)
{
}

void CCom_WSunken::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	m_animation = (m_pobj->GetComponent(COM_ANIMATION));

	m_attack_delay = 1.3333f;
	m_weapon_info.damage = 40;
	m_weapon_info.airdamage = 0;
	m_weapon_info.eDamageType = DAMAGE_BOOM;
	m_weapon_info.eAirDamageType = DAMAGE_BOOM;
}
void CCom_WSunken::fire(CObj* ptarget)
{
	if(false == m_bfire)
	{
		CSoundDevice::GetInstance()->PlayBattleSound(SND_B_SUNKEN_FIRE , m_pobj->GetPos());

		m_pobj->SetState(ATTACK);
		m_bfire = true;

		m_attack_time = 0.f;
		//총알생성
		//타겟에게 데미지 ㄱㄱㄱㄱ
		m_targetpos = ptarget->GetPos();
		m_targetpos -= m_pobj->GetcurDir()*(ptarget->GetVertex().right/2);

		D3DXVECTOR2 vpos = m_pobj->GetPos();
		D3DXVECTOR2 vdir = m_pobj->GetcurDir();
		vpos = vpos + vdir * 12;

		CObj* peff = NULL;

		peff = new CGeneraEff(L"SUNKEN_BULLET" ,m_targetpos , D3DXVECTOR2(1.2f, 1.2f), SORT_GROUND_EFF , 1.4f);
		peff->Initialize();
		CObjMgr::GetInstance()->AddEffect(peff);

		int idx = ptarget->Getcuridx(32);
		if(MOVE_GROUND == ptarget->GetUnitinfo().eMoveType &&
			CSkill_DarkSwarm::m_darkswarm_cnt[idx] == 0)
			ptarget->SetDamage(m_weapon_info.damage , m_weapon_info.eDamageType);
	}
	else
	{
	}
}
void CCom_WSunken::Update(void)
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


void CCom_WSunken::Render(void)
{

}

void CCom_WSunken::Release(void)
{

}

