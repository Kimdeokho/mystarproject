#include "StdAfx.h"
#include "Com_WSiege.h"

#include "Obj.h"
#include "TimeMgr.h"
#include "Com_Animation.h"
#include "TimeMgr.h"
#include "GeneraEff.h"
#include "MultiEff.h"
#include "ObjMgr.h"

CCom_WSiege::CCom_WSiege(const int& damage , DAMAGE_TYPE edamagetype)
{

}

CCom_WSiege::~CCom_WSiege(void)
{
}

void CCom_WSiege::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	m_animation = (m_pobj->GetComponent(COM_ANIMATION));
	m_attack_delay = 3.125f;

	m_bfire = false;

	m_damage = m_pobj->GetUnitinfo().damage;
}

void CCom_WSiege::fire(CObj*& ptarget)
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
			m_targetpos = ptarget->GetPos();
			//m_targetpos.x += float(rand()%20 - 10);
			//m_targetpos.y += float(rand()%20 - 10);

			CObj* peff = new CGeneraEff(L"SIEGEBOOM" ,m_targetpos , D3DXVECTOR2(1,1), SORT_GROUND_EFF , 2.5f);

			peff->Initialize();
			CObjMgr::GetInstance()->AddEffect(peff);

			peff = new CMultiEff(L"SIEGEFIRE" , ((CCom_Animation*)m_animation)->GetCurDirIdx() ,3);

			D3DXVECTOR2 vdir = m_pobj->GetcurDir();
			D3DXVECTOR2 vpos = m_pobj->GetPos();

			vpos += vdir*35;
			peff->SetPos(vpos);			
			peff->Initialize();
			CObjMgr::GetInstance()->AddEffect(peff);

			(ptarget)->SetDamage(m_damage , m_edamagetype);
		}

	}
}
void CCom_WSiege::Update(void)
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

void CCom_WSiege::Render(void)
{

}

void CCom_WSiege::Release(void)
{

}
