#include "StdAfx.h"
#include "Com_WLurker.h"

#include "Obj.h"
#include "Com_Animation.h"
#include "TimeMgr.h"
#include "ObjMgr.h"

#include "Lurker_Thornfire.h"

CCom_WLurker::CCom_WLurker(void)
{
}

CCom_WLurker::~CCom_WLurker(void)
{
}
void CCom_WLurker::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	m_animation = (m_pobj->GetComponent(COM_ANIMATION));

	m_attack_delay = 1.5416f;
	m_weapon_info.damage = 20;
	m_weapon_info.airdamage = 0;
	m_weapon_info.eDamageType = DAMAGE_NOMAL;
	m_weapon_info.eAirDamageType = DAMAGE_NONE;

	m_bfire = false;
}
void CCom_WLurker::fire(CObj* ptarget)
{
	if(false == m_bfire)
	{
		m_pobj->SetState(BURROW);
		m_bfire = true;

		m_attack_time = 0.f;

		CObj* pbullet = new CLurker_Thornfire(m_pobj->GetPos() , ptarget->GetPos() , m_pobj->GetUnitinfo().attack_range , m_weapon_info , m_pobj );
		CObjMgr::GetInstance()->AddObject(pbullet , OBJ_BULLET);

		pbullet->SetPos(m_pobj->GetPos());
		pbullet->Initialize();

		//(ptarget)->SetDamage(m_weapon_info.damage + m_upg_info[UPG_Z_MISSILE_ATT].upg_cnt , m_weapon_info.eDamageType);
	}
	else
	{
		if(MOVE == m_pobj->GetUnitinfo().state)
			m_pobj->SetState(IDLE);
	}
}
void CCom_WLurker::Update(void)
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

void CCom_WLurker::Render(void)
{

}



void CCom_WLurker::Release(void)
{

}
