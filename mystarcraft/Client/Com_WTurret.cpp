#include "StdAfx.h"
#include "Com_WTurret.h"

#include "Obj.h"
#include "Com_Animation.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "GeneraEff.h"
#include "MultiEff.h"
CCom_WTurret::CCom_WTurret(const int& damage , DAMAGE_TYPE edamagetype)
{

}

CCom_WTurret::~CCom_WTurret(void)
{
}

void CCom_WTurret::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	m_animation = (m_pobj->GetComponent(COM_ANIMATION));

	m_attack_delay = 0.625f;

	m_bfire = false;
}

void CCom_WTurret::Update(void)
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
void CCom_WTurret::fire(CObj*& ptarget)
{
	if(false == m_bfire)
	{
		m_pobj->SetState(ATTACK);

		if( true == ((CCom_Animation*)m_animation)->GetRotationComplete())
		{			
			m_bfire = true;

			m_attack_time = 0.f;
			//총알생성
			//타겟에게 데미지 ㄱㄱㄱㄱ
			CObj* peff = new CGeneraEff(L"ToongToong" ,ptarget->GetPos() , D3DXVECTOR2(1.3f , 1.3f), SORT_GROUND_EFF);
			peff->Initialize();
			CObjMgr::GetInstance()->AddEffect(peff);

			(ptarget)->SetDamage(m_damage , m_edamagetype);
		}
	}
}

void CCom_WTurret::Render(void)
{

}

void CCom_WTurret::Release(void)
{

}

