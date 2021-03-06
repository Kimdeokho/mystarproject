#include "StdAfx.h"
#include "Com_WMutal.h"

#include "Obj.h"
#include "Com_Animation.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "GeneraEff.h"

#include "Mutal_Bullet.h"

CCom_WMutal::CCom_WMutal(void)
{
}

CCom_WMutal::~CCom_WMutal(void)
{
}
void CCom_WMutal::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	m_animation = (m_pobj->GetComponent(COM_ANIMATION));

	m_attack_delay = 1.25f;
	m_weapon_info.damage = 25;
	m_weapon_info.airdamage = 25;
	m_weapon_info.eDamageType = DAMAGE_NOMAL;
	m_weapon_info.eAirDamageType = DAMAGE_NOMAL;

	m_bfire = false;
}

void CCom_WMutal::Update(void)
{
	if(m_bfire)
	{
		m_attack_time += GETTIME;
		if(m_attack_time > m_attack_delay) 
		{
			m_attack_time = GETTIME;// * float(rand()%5);
			m_bfire = false;
		}
	}
}

void CCom_WMutal::Render(void)
{

}
void CCom_WMutal::fire(CObj* ptarget)
{
	if(false == m_bfire)
	{
		CSoundDevice::GetInstance()->PlayBattleSound(SND_B_MUTAL_FIRE , m_pobj->GetPos());

		m_pobj->SetState(ATTACK);
		m_bfire = true;

		m_attack_time = 0.f;

		D3DXVECTOR2 vpos = m_pobj->GetPos();

		//�Ѿ� ����
		CObj* pbullet = new CMutal_Bullet(ptarget->GetObjNumber() , ptarget->GetPos() , m_pobj);
		pbullet->SetPos(vpos);
		pbullet->Initialize();
		CObjMgr::GetInstance()->AddObject(pbullet , OBJ_BULLET);
	}
	else
	{
		if(MOVE == m_pobj->GetUnitinfo().state)
			m_pobj->SetState(IDLE);
	}
}

void CCom_WMutal::Release(void)
{

}

