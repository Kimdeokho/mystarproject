#include "StdAfx.h"
#include "Com_WScourge.h"

#include "Obj.h"
#include "Com_Animation.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "GeneraEff.h"
#include "MultiEff.h"

CCom_WScourge::CCom_WScourge(void)
{
}

CCom_WScourge::~CCom_WScourge(void)
{
}

void CCom_WScourge::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	m_animation = (m_pobj->GetComponent(COM_ANIMATION));

	m_attack_delay = 0.f;
	m_weapon_info.damage = 0;
	m_weapon_info.airdamage = 110;
	m_weapon_info.eDamageType = DAMAGE_NOMAL;
	m_weapon_info.eAirDamageType = DAMAGE_NOMAL;
}

void CCom_WScourge::Update(void)
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

void CCom_WScourge::fire(CObj* ptarget)
{
	if(false == m_bfire)
	{
		CSoundDevice::GetInstance()->PlayBattleSound(SND_B_SCATT , m_pobj->GetPos());

		m_pobj->SetState(ATTACK);
		m_bfire = true;

		m_attack_time = 0.f;

		D3DXVECTOR2 vpos = m_pobj->GetPos();

		//ÃÑ¾Ë »ý¼º
		ptarget->SetDamage(m_weapon_info.airdamage , m_weapon_info.eAirDamageType);

		CObj* peff = new CGeneraEff(L"SCOURGE_BOOM", ptarget->GetPos() , D3DXVECTOR2(1,1), SORT_AIR_EFF , 1.7f);
		peff->SetPos(vpos);
		peff->Initialize();
		CObjMgr::GetInstance()->AddEffect(peff);

		m_pobj->SetDestroy(true);
	}
	else
	{
		if(MOVE == m_pobj->GetUnitinfo().state)
			m_pobj->SetState(IDLE);
	}
}

void CCom_WScourge::Render(void)
{

}

void CCom_WScourge::Release(void)
{

}
