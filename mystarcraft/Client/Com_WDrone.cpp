#include "StdAfx.h"
#include "Com_WDrone.h"

#include "Obj.h"

#include "TimeMgr.h"
#include "ObjMgr.h"
#include "MultiEff.h"
#include "GeneraEff.h"
#include "Mineral.h"
#include "Mineral_frag.h"
#include "Workman.h"

#include "Com_Animation.h"
CCom_WDrone::CCom_WDrone(void)
{
}

CCom_WDrone::~CCom_WDrone(void)
{
}

void CCom_WDrone::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	m_animation = (m_pobj->GetComponent(COM_ANIMATION));

	m_attack_delay = 0.625f;

	m_bfire = true;
	m_bmineral_gather = true;

	m_gathertime = 0.f;
	m_gatherdelay = 3.f;

	m_weapon_info.damage = 5;
	m_weapon_info.eDamageType = DAMAGE_NOMAL;
}

void CCom_WDrone::Update(void)
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

	if(m_bmineral_gather)
	{
	}
}

void CCom_WDrone::Render(void)
{

}
void CCom_WDrone::fire(CObj* ptarget)
{
	if( OBJ_MINERAL == ptarget->GetOBJNAME() )
	{
		//½Ã°£À» Àé´Ù..
		CObj* pfragment = NULL;
		if(true == ((CCom_Animation*)m_animation)->GetRotationComplete())
		{
			m_pobj->SetState(GATHER);
			m_targetpos = ptarget->GetPos();

			m_gathertime += GETTIME;

			if(false == m_bfire &&
				true == ((CCom_Animation*)m_animation)->GetAttackSync())
			{
				//¿©±â¼­ ¹Ì³×¶ö Á¡À¯ÀÏ²Û¼ö¸¦ += 1 ÇÑ´Ù
				//((CMineral*)ptarget)->Setworkman(m_pobj);
				m_bfire = true;
			}

			if(m_gathertime > m_gatherdelay)
			{
				//ÀÏÁ¤½Ã°£ ³Ñ¾î°¡¸é ÀÚ¿øÈ¹µæ, ³²Àº ³×¶ö¾çÀ» ±ð¾Æ³»°í
				/* ¹Ì³×¶ö Á¡À¯ÀÏ²Û¼ö¸¦ -= 1 ÇÑ´Ù
				¹Ì³×¶ö µ¢¾î¸®¸¦ ÀÏ²Û¼Õ¿¡ Áã¾îÁØ´Ù
				*/

				pfragment = new CMineral_frag(m_pobj);
				CObjMgr::GetInstance()->AddObject(pfragment , OBJ_FRAGMENT);
				pfragment->SetTeamNumber(m_pobj->GetTeamNumber());
				pfragment->Initialize();

				((CWorkman*)m_pobj)->setmineral_fragment(pfragment);
				((CWorkman*)m_pobj)->SetMineral_mark(NULL);
				m_pobj->SetState(IDLE);

				((CMineral*)ptarget)->SetMineral_amount(-8);
				((CMineral*)ptarget)->decrease_workman(m_pobj);
				m_gathertime = 0.f;
				m_pobj->SetOrder(ORDER_RETURN_CARGO);
			}
			else
				((CWorkman*)m_pobj)->SetMineral_mark(ptarget);
		}
		else
		{
			if(MOVE == m_pobj->GetUnitinfo().state)
				m_pobj->SetState(IDLE);
		}
	}
	else if( OBJ_GAS == ptarget->GetOBJNAME() )
	{
	}
	else
	{
		if( false == m_bfire)
		{		
			m_pobj->SetState(IDLE);
			if(true == ((CCom_Animation*)m_animation)->GetRotationComplete())
			{				
				m_pobj->SetState(ATTACK);
				m_attack_time = 0.f;			

				//m_targetpos = ptarget->GetPos();

				if(true == ((CCom_Animation*)m_animation)->GetAttackSync())
				{
					D3DXVECTOR2 vpos = m_pobj->GetPos();
					D3DXVECTOR2 vdir = m_pobj->GetcurDir();
					vpos = vpos + vdir * 12;

					m_bfire = true;
					CObj* peff = new CMultiEff(L"ACIDFIRE" , ((CCom_Animation*)m_animation)->GetCurDirIdx() , 1.3f , 1 , SORT_GROUND_EFF);
					peff->SetPos(vpos);
					peff->Initialize();
					CObjMgr::GetInstance()->AddEffect(peff);

					peff = new CGeneraEff(L"ACID_BOOM" ,  ptarget->GetPos() , D3DXVECTOR2(1.f , 1.f), SORT_GROUND_EFF , 1.3f);
					peff->Initialize();
					CObjMgr::GetInstance()->AddEffect(peff);

					(ptarget)->SetDamage( m_weapon_info.damage , m_weapon_info.eDamageType );
				}
			}
		}
	}
}
void CCom_WDrone::Release(void)
{

}