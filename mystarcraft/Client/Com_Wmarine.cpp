#include "StdAfx.h"
#include "Com_Wmarine.h"

#include "Obj.h"
#include "Com_Animation.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "GeneraEff.h"
#include "MultiEff.h"
CCom_Wmarine::CCom_Wmarine()
{
}

CCom_Wmarine::~CCom_Wmarine(void)
{
}

void CCom_Wmarine::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	m_animation = (m_pobj->GetComponent(COM_ANIMATION));

	m_attack_delay = 0.625f;
	m_weapon_info.damage = 6;
	m_weapon_info.airdamage = 6;
	m_weapon_info.eDamageType = DAMAGE_NOMAL;
	m_weapon_info.eAirDamageType = DAMAGE_NOMAL;

	m_bfire = false;
}
void CCom_Wmarine::fire(CObj*&	ptarget )
{
	if(false == m_bfire)
	{
		m_pobj->SetState(IDLE);
		
		if( true == ((CCom_Animation*)m_animation)->GetRotationComplete())
		{
			m_pobj->SetState(ATTACK);
			m_bfire = true;
			
			m_attack_time = 0.f;
			//�Ѿ˻���
			//Ÿ�ٿ��� ������ ��������
			m_targetpos = ptarget->GetPos();
			m_targetpos -= m_pobj->GetcurDir()*(ptarget->GetVertex().right/2);

			CObj* peff = NULL;

			if(ORDER_BUNKER_BOARDING == m_pobj->GetUnitinfo().order)
			{
				D3DXVECTOR2 vpos = m_pobj->GetPos() + m_pobj->GetcurDir()*17;
				vpos.y -= 15;
				peff = new CMultiEff(L"BUNKERFIRE" , ((CCom_Animation*)m_animation)->GetCurDirIdx() ,6.0f , 7 , SORT_GROUND_EFF);
				peff->SetPos(vpos);
				peff->Initialize();
				CObjMgr::GetInstance()->AddEffect(peff);
			}

			if(MOVE_GROUND == ptarget->GetUnitinfo().eMoveType)
				peff = new CGeneraEff(L"GaussGun" ,m_targetpos , D3DXVECTOR2(1,1), SORT_GROUND_EFF , 1.5);
			else
				peff = new CGeneraEff(L"GaussGun" ,m_targetpos , D3DXVECTOR2(1,1), SORT_AIR_EFF , 1.5);
			peff->Initialize();
			CObjMgr::GetInstance()->AddEffect(peff);

			(ptarget)->SetDamage(m_weapon_info.damage + m_upg_info[UPG_T_BIO_WEAPON].upg_cnt , m_weapon_info.eDamageType);
		}
	}
	else
	{
		if(MOVE == m_pobj->GetUnitinfo().state)
			m_pobj->SetState(IDLE);
	}
}
void CCom_Wmarine::Update(void)
{
	if(m_bfire)
	{
		m_attack_time += GETTIME;
		if(m_attack_time > m_attack_delay) //�������� �̰� �ٰ���
		{
			m_attack_time = 0.f;
			m_bfire = false;
		}
	}
}

void CCom_Wmarine::Render(void)
{

}

void CCom_Wmarine::Release(void)
{

}

