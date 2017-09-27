#include "StdAfx.h"
#include "Com_WTank.h"

#include "Obj.h"
#include "Com_Animation.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "GeneraEff.h"
#include "MultiEff.h"

CCom_WTank::CCom_WTank(const int& damage , DAMAGE_TYPE edamagetype)
{
	m_damage = damage;
	m_edamagetype = edamagetype;
}

CCom_WTank::~CCom_WTank(void)
{
}

void CCom_WTank::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	m_animation = (m_pobj->GetComponent(COM_ANIMATION));

	m_attack_delay = 1.5416f;

	m_bfire = false;

}

void CCom_WTank::fire(CObj*& ptarget)
{
	if(false == m_bfire)
	{
		if( true == ((CCom_Animation*)m_animation)->GetRotationComplete())
		{
			m_bfire = true;
			m_pobj->SetState(ATTACK);
			m_attack_time = 0.f;
			//총알생성
			//타겟에게 데미지 ㄱㄱㄱㄱ
			CObj* peff = new CGeneraEff(L"ToongToong" ,ptarget->GetPos() , D3DXVECTOR2(1.3f , 1.3f), SORT_GROUND_EFF);
			peff->Initialize();
			CObjMgr::GetInstance()->AddEffect(peff);

			peff = new CMultiEff(L"TANKFIRE" , ((CCom_Animation*)m_animation)->GetCurDirIdx() , 7);

			peff->SetPos(m_pobj->GetPos());
			peff->Initialize();
			CObjMgr::GetInstance()->AddEffect(peff);
			

			(ptarget)->SetDamage(m_damage , m_edamagetype);
		}
	}
	else
	{
		//if(L"MOVE" == ((CCom_Animation*)m_animation)->GetAnimation())
		{
			//((CCom_Animation*)m_animation)->SetAnimation(L"IDLE");
		}
	}
}


void CCom_WTank::Update(void)
{
	if(m_bfire)
	{
		m_attack_time += GETTIME;
		if(m_attack_time > m_attack_delay) //스팀빨면 이게 줄겠지
		{
			m_attack_time = 0.f;
			m_bfire = false;
		}
	}
}

void CCom_WTank::Render(void)
{

}

void CCom_WTank::Release(void)
{

}

