#include "StdAfx.h"
#include "Com_WFirebat.h"

#include "Obj.h"
#include "Com_Animation.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "GeneraEff.h"
#include "MultiEff.h"
#include "Area_Mgr.h"
#include "FontMgr.h"
#include "ScrollMgr.h"
CCom_WFirebat::CCom_WFirebat(void)
{
}

CCom_WFirebat::~CCom_WFirebat(void)
{
}

void CCom_WFirebat::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	m_animation = (m_pobj->GetComponent(COM_ANIMATION));

	m_weapon_info.damage = 16;
	m_weapon_info.eDamageType = DAMAGE_VIBRATE;
	
	m_attack_delay = 0.9166f;

	m_bfire = false;

	for(int i = 0; i < 3; ++i)
		m_splash_range[i] = 48*0.625f;
}

void CCom_WFirebat::Update(void)
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
}

void CCom_WFirebat::Render(void)
{

}

void CCom_WFirebat::Release(void)
{

}

void CCom_WFirebat::fire(CObj* ptarget)
{
	if(false == m_bfire)
	{
		m_pobj->SetState(IDLE);

		//if( true == ((CCom_Animation*)m_animation)->GetRotationComplete())
		{
			CSoundDevice::GetInstance()->PlayBattleSound(SND_B_FIREBAT_ATT , m_pobj->GetPos());
			m_pobj->SetState(ATTACK);
			m_bfire = true;

			m_attack_time = 0.f;
			//ÃÑ¾Ë»ý¼º
			//Å¸°Ù¿¡°Ô µ¥¹ÌÁö ¤¡¤¡¤¡¤¡

			CObj* peff = NULL;

			peff = new CMultiEff(L"FIRESHOT" ,((CCom_Animation*)m_animation)->GetCurDirIdx() , 1.5f , 1 , SORT_GROUND_EFF);

			D3DXVECTOR2 vdir = m_pobj->GetcurDir();
			D3DXVECTOR2 vpos = m_pobj->GetPos();

			vpos += vdir*20;
			peff->SetPos(vpos);	
			peff->Initialize();
			CObjMgr::GetInstance()->AddEffect(peff);

			D3DXVECTOR2	vsplash_pos[5];
			vsplash_pos[0] = ptarget->GetPos();
			for(int i = 1; i < 5; ++i)
			{
				vsplash_pos[i] = vsplash_pos[i-1] + vdir * 20;
			}

			//m_pobj->GetUnitinfo().eAttackType;
			for(int i = 0; i < 5; ++i)
			{				
				//renderpos.x = vsplash_pos[i].x - CScrollMgr::m_fScrollX;
				//renderpos.y = vsplash_pos[i].y - CScrollMgr::m_fScrollY;
				//CFontMgr::GetInstance()->SetNoticeFont(L"@" , renderpos.x , renderpos.y );

				CArea_Mgr::GetInstance()->Setsplash_damage(m_pobj, 
					m_weapon_info.damage + m_upg_info[UPG_T_BIO_WEAPON].upg_cnt*2,
					m_weapon_info.eDamageType,
					vsplash_pos[i] , m_splash_range , false , m_injure_list);
			}
			m_injure_list.clear();
		}
	}
	else
	{
		if(MOVE == m_pobj->GetUnitinfo().state)
			m_pobj->SetState(IDLE);
	}
}
