#include "StdAfx.h"
#include "Com_WSiege.h"

#include "Obj.h"
#include "TimeMgr.h"
#include "Com_Animation.h"
#include "TimeMgr.h"
#include "GeneraEff.h"
#include "MultiEff.h"
#include "ObjMgr.h"
#include "Tankbarrel.h"
#include "Area_Mgr.h"

#include "Skill_DarkSwarm.h"

CCom_WSiege::CCom_WSiege()
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
	m_weapon_info.damage = 70;
	m_weapon_info.eDamageType = DAMAGE_BOOM;

	m_bfire = false;


	m_splash_range[0] = 10;
	m_splash_range[1] = 25;
	m_splash_range[2] = 40;
}

void CCom_WSiege::fire(CObj* ptarget)
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

			CObj* peff = new CGeneraEff(L"SIEGEBOOM" ,m_targetpos , D3DXVECTOR2(1,1), SORT_GROUND_EFF , 1.2f);

			peff->Initialize();
			CObjMgr::GetInstance()->AddEffect(peff);

			peff = new CMultiEff(L"SIEGEFIRE" , ((CCom_Animation*)m_animation)->GetCurDirIdx() ,2.0f , 1 , SORT_GROUND_EFF);

			D3DXVECTOR2 vdir = m_pobj->GetcurDir();
			D3DXVECTOR2 vpos = ((CTankbarrel*)m_pobj)->GetbarrelPos();

			vpos += vdir*30;
			peff->SetPos(vpos);			
			peff->Initialize();
			CObjMgr::GetInstance()->AddEffect(peff);

			int idx = ptarget->Getcuridx(32);
			if(MOVE_GROUND == ptarget->GetUnitinfo().eMoveType && 
				CSkill_DarkSwarm::m_darkswarm_cnt[idx] > 0)
			{
				D3DXVECTOR2 vdir;
				vdir = ptarget->GetPos() - m_pobj->GetPos();
				D3DXVec2Normalize(&vdir , &vdir);
				m_targetpos -= vdir*30;
			}

			CArea_Mgr::GetInstance()->Setsplash_damage(m_pobj, 
				m_weapon_info.damage + m_upg_info[UPG_T_MECHANIC_WEAPON].upg_cnt*5,
				m_weapon_info.eDamageType, m_targetpos,  
				m_splash_range , true , m_injure_list);

			m_injure_list.clear();
		}

	}
	else
	{
		if(MOVE == m_pobj->GetUnitinfo().state)
			m_pobj->SetState(IDLE);
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
