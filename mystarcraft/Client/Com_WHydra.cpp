#include "StdAfx.h"
#include "Com_WHydra.h"

#include "Obj.h"
#include "Com_Animation.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "GeneraEff.h"
#include "MultiEff.h"
#include "Skill_DarkSwarm.h"

CCom_WHydra::CCom_WHydra(void)
{
}

CCom_WHydra::~CCom_WHydra(void)
{
}

void CCom_WHydra::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	m_animation = (m_pobj->GetComponent(COM_ANIMATION));

	m_attack_delay = 0.625f;
	m_weapon_info.damage = 10;
	m_weapon_info.airdamage = 10;
	m_weapon_info.eDamageType = DAMAGE_BOOM;
	m_weapon_info.eAirDamageType = DAMAGE_BOOM;

	m_bfire = false;

	m_attack_time = m_attack_delay;
}
void CCom_WHydra::fire(CObj* ptarget)
{
	if(false == m_bfire)
	{
		m_pobj->SetState(IDLE);

		if( true == ((CCom_Animation*)m_animation)->GetRotationComplete())
		{
			m_pobj->SetState(ATTACK);
			m_bfire = true;

			m_attack_time = 0.f;
			//총알생성
			//타겟에게 데미지 ㄱㄱㄱㄱ
			m_targetpos = ptarget->GetPos();
			m_targetpos -= m_pobj->GetcurDir()*(ptarget->GetVertex().right/2);

			CObj* peff = NULL;

			if(MOVE_GROUND == ptarget->GetUnitinfo().eMoveType)
				peff = new CGeneraEff(L"ACID_BOOM" ,m_targetpos , D3DXVECTOR2(1.2f, 1.2f), SORT_GROUND_EFF , 1.4f);
			else
				peff = new CGeneraEff(L"ACID_BOOM" ,m_targetpos , D3DXVECTOR2(1.2f , 1.2f), SORT_AIR_EFF , 1.4f);
			peff->Initialize();
			CObjMgr::GetInstance()->AddEffect(peff);

			D3DXVECTOR2 vpos = m_pobj->GetPos();
			D3DXVECTOR2 vdir = m_pobj->GetcurDir();
			vpos = vpos + vdir * 12;

			peff = new CMultiEff(L"ACIDFIRE" , ((CCom_Animation*)m_animation)->GetCurDirIdx() , 1.3f , 1 , SORT_GROUND_EFF);
			peff->SetPos(vpos);
			peff->Initialize();
			CObjMgr::GetInstance()->AddEffect(peff);

			int idx = ptarget->Getcuridx(32);
			if(MOVE_GROUND == ptarget->GetUnitinfo().eMoveType)
			{
				if(CSkill_DarkSwarm::m_darkswarm_cnt[idx] == 0)
					ptarget->SetDamage(m_weapon_info.damage + m_upg_info[UPG_Z_MISSILE_ATT].upg_cnt , m_weapon_info.eDamageType);
			}
			else
				ptarget->SetDamage(m_weapon_info.damage + m_upg_info[UPG_Z_MISSILE_ATT].upg_cnt , m_weapon_info.eDamageType);
		}
	}
	else
	{
		if(MOVE == m_pobj->GetUnitinfo().state)
			m_pobj->SetState(IDLE);
	}
}
void CCom_WHydra::Update(void)
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

void CCom_WHydra::Render(void)
{

}



void CCom_WHydra::Release(void)
{

}
