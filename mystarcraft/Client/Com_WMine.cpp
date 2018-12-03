#include "StdAfx.h"
#include "Com_WMine.h"

#include "Obj.h"
#include "Com_Animation.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "GeneraEff.h"
#include "MultiEff.h"
#include "Area_Mgr.h"
CCom_WMine::CCom_WMine(void)
{
}

CCom_WMine::~CCom_WMine(void)
{
}

void CCom_WMine::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	m_animation = (m_pobj->GetComponent(COM_ANIMATION));

	m_weapon_info.damage = 125;
	m_weapon_info.eDamageType = DAMAGE_BOOM;

	m_bfire = false;

	m_splash_range[0] = 50;
	m_splash_range[1] = 75;
	m_splash_range[2] = 100;
}

void CCom_WMine::Update(void)
{
}

void CCom_WMine::Render(void)
{

}

void CCom_WMine::Release(void)
{

}

void CCom_WMine::fire(CObj* ptarget)
{
	if(false == m_bfire)
	{
		m_bfire = true;

		m_attack_time = 0.f;

		CArea_Mgr::GetInstance()->Setsplash_damage(m_pobj, 
			m_weapon_info.damage , m_weapon_info.eDamageType,
			m_pobj->GetPos(), 
			m_splash_range , true , m_injure_list);

		m_injure_list.clear();

		m_pobj->SetDestroy(true);
		m_pobj->Dead();
	}
}
