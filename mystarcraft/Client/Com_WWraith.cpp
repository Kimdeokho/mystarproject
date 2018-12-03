#include "StdAfx.h"
#include "Com_WWraith.h"

#include "Obj.h"
#include "Com_Animation.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "GeneraEff.h"

#include "Wraith_Bullet.h"
#include "Wraith_Bim.h"
CCom_WWraith::CCom_WWraith(void)
{
}

CCom_WWraith::~CCom_WWraith(void)
{
}

void CCom_WWraith::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	m_animation = (m_pobj->GetComponent(COM_ANIMATION));

	m_attack_delay = 1.25f;
	m_air_attack_delay = 0.9166f;
	m_weapon_info.damage = 8;
	m_weapon_info.airdamage = 20;
	m_weapon_info.eDamageType = DAMAGE_NOMAL;
	m_weapon_info.eAirDamageType = DAMAGE_BOOM;

	m_bfire = false;
}
void CCom_WWraith::fire(CObj* ptarget)
{
	if(MOVE_GROUND == ptarget->GetUnitinfo().eMoveType)
	{
		if(false == m_bfire)
		{
			m_attack_time = 0.f;

			m_pobj->SetState(ATTACK);
			m_bfire = true;

			//ÃÑ¾Ë »ý¼º

			CObj* pbullet = new CWraith_Bim(ptarget->GetObjNumber() , ptarget->GetPos() );
			D3DXVECTOR2 vpos = m_pobj->GetPos();
			vpos = vpos + m_pobj->GetcurDir()*10;
			
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
	else
	{
		if(false == m_bair_fire)
		{
			m_attack_time = 0.f;

			m_pobj->SetState(ATTACK);
			m_bair_fire = true;

			CObj* pbullet = new CWraith_Bullet(ptarget->GetObjNumber() , ptarget->GetPos() );
			pbullet->SetPos(m_pobj->GetPos());
			pbullet->Initialize();
			CObjMgr::GetInstance()->AddObject(pbullet , OBJ_BULLET);
		}
		else
		{
			if(MOVE == m_pobj->GetUnitinfo().state)
				m_pobj->SetState(IDLE);
		}
	}
}
void CCom_WWraith::Update(void)
{
	if(m_bfire)
	{
		m_attack_time += GETTIME;
		if(m_attack_time > m_attack_delay) //½ºÆÀ»¡¸é ÀÌ°Ô ÁÙ°ÚÁö
		{
			m_attack_time = 0.f;
			m_bfire = false;
		}
	}

	if(m_bair_fire)
	{
		m_air_attack_time += GETTIME;
		if(m_air_attack_time > m_air_attack_delay) //½ºÆÀ»¡¸é ÀÌ°Ô ÁÙ°ÚÁö
		{
			m_air_attack_time = 0.f;
			m_bair_fire = false;
		}
	}
}

void CCom_WWraith::Render(void)
{

}

void CCom_WWraith::Release(void)
{

}


