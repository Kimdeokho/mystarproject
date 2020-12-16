#include "StdAfx.h"
#include "Com_WTank.h"

#include "Obj.h"
#include "Com_Animation.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "GeneraEff.h"
#include "MultiEff.h"
#include "Tankbarrel.h"

#include "Skill_DarkSwarm.h"

CCom_WTank::CCom_WTank()
{
}

CCom_WTank::~CCom_WTank(void)
{
}

void CCom_WTank::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	m_animation = (m_pobj->GetComponent(COM_ANIMATION));

	m_attack_delay = 1.5416f;
	m_weapon_info.damage = 30;

	m_weapon_info.eDamageType = DAMAGE_BOOM;

	m_bfire = false;

}
void CCom_WTank::Update(void)
{
	if(m_bfire)
	{
		m_attack_time += GETTIME;
		if(m_attack_time > m_attack_delay) //½ºÆÀ»¡¸é ÀÌ°Ô ÁÙ°ÚÁö
		{
			m_attack_time = GETTIME * float(rand()%5);
			m_bfire = false;
		}
	}
}

void CCom_WTank::fire(CObj* ptarget)
{
	if(false == m_bfire)
	{
		if( true == ((CCom_Animation*)m_animation)->GetRotationComplete())
		{
			CSoundDevice::GetInstance()->PlayBattleSound(SND_B_TANK_FIRE , m_pobj->GetPos());

			m_bfire = true;
			m_pobj->SetState(ATTACK);
			m_attack_time = 0.f;
			//ÃÑ¾Ë»ý¼º
			//Å¸°Ù¿¡°Ô µ¥¹ÌÁö ¤¡¤¡¤¡¤¡
			m_targetpos = ptarget->GetPos();
			m_targetpos -= m_pobj->GetcurDir()*(ptarget->GetVertex().right/2);

			CObj* peff = new CGeneraEff(L"ToongToong" ,m_targetpos , D3DXVECTOR2(1.3f , 1.3f), SORT_GROUND_EFF);
			peff->Initialize();
			CObjMgr::GetInstance()->AddEffect(peff);


			peff = new CMultiEff(L"TANKFIRE" , ((CCom_Animation*)m_animation)->GetCurDirIdx() , 7 , 1 , SORT_GROUND_EFF);
			peff->SetPos(((CTankbarrel*)m_pobj)->GetbarrelPos());
			peff->Initialize();
			CObjMgr::GetInstance()->AddEffect(peff);
			

			int idx = ptarget->Getcuridx(32);

			if(MOVE_GROUND == ptarget->GetUnitinfo().eMoveType && 
				CSkill_DarkSwarm::m_darkswarm_cnt[idx] == 0)
				ptarget->SetDamage( m_weapon_info.damage + m_upg_info[UPG_T_MECHANIC_WEAPON].upg_cnt*3,
				m_weapon_info.eDamageType );
		}
	}
	else
	{
		if(MOVE == m_pobj->GetUnitinfo().state)
			m_pobj->SetState(IDLE);
	}
}




void CCom_WTank::Render(void)
{

}

void CCom_WTank::Release(void)
{

}

