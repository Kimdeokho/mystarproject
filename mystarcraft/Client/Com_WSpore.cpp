#include "StdAfx.h"
#include "Com_WSpore.h"

#include "Obj.h"
#include "Com_Animation.h"
#include "TimeMgr.h"
#include "ObjMgr.h"

#include "Spore_Bullet.h"

CCom_WSpore::CCom_WSpore(void)
{
}

CCom_WSpore::~CCom_WSpore(void)
{
}

void CCom_WSpore::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	m_attack_delay = 0.625f;
	m_weapon_info.damage = 15;
	m_weapon_info.airdamage = 0;
	m_weapon_info.eDamageType = DAMAGE_BOOM;
	m_weapon_info.eAirDamageType = DAMAGE_BOOM;
}
void CCom_WSpore::fire(CObj* ptarget)
{
	if(false == m_bfire)
	{
		m_pobj->SetState(ATTACK);
		m_bfire = true;

		m_attack_time = 0.f;
		//총알생성
		//타겟에게 데미지 ㄱㄱㄱㄱ
		m_targetpos = ptarget->GetPos();

		CObj* pbullet = new CSpore_Bullet(ptarget->GetObjNumber() , m_targetpos);

		D3DXVECTOR2 vbullet_pos = m_pobj->GetPos();
		vbullet_pos.x -= 20;
		vbullet_pos.y -= 33;

		CObjMgr::GetInstance()->AddObject(pbullet , OBJ_BULLET);
		pbullet->SetPos(vbullet_pos);
		pbullet->Initialize();
		
	}
	else
	{
		if(MOVE == m_pobj->GetUnitinfo().state)
			m_pobj->SetState(IDLE);
	}
}

void CCom_WSpore::Update(void)
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

void CCom_WSpore::Render(void)
{

}

void CCom_WSpore::Release(void)
{

}

