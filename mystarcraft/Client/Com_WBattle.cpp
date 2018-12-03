#include "StdAfx.h"
#include "Com_WBattle.h"

#include "Obj.h"
#include "Com_Animation.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "GeneraEff.h"
#include "MultiEff.h"

#include "Goliath_Bullet.h"
#include "Goliath_part.h"
#include "Battle_bim.h"

CCom_WBattle::CCom_WBattle(void)
{
}

CCom_WBattle::~CCom_WBattle(void)
{
}

void CCom_WBattle::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	m_animation = (m_pobj->GetComponent(COM_ANIMATION));

	m_attack_delay = 1.25f;
	m_weapon_info.damage = 25;
	m_weapon_info.airdamage = 25;
	m_weapon_info.eDamageType = DAMAGE_NOMAL;
	m_weapon_info.eAirDamageType = DAMAGE_NOMAL;

	m_bfire = false;
}
void CCom_WBattle::fire(CObj* ptarget)
{
	if(false == m_bfire)
	{
		m_pobj->SetState(ATTACK);
		m_bfire = true;

		m_attack_time = 0.f;


		CObj* peff = new CMultiEff(L"BATTLEFIRE" , ((CCom_Animation*)m_animation)->GetCurDirIdx() ,2.f , 1 , SORT_AIR_EFF);
		D3DXVECTOR2 vdir = m_pobj->GetcurDir();
		D3DXVECTOR2 vpos = m_pobj->GetPos();

		peff->SetPos(vpos);			
		peff->Initialize();
		CObjMgr::GetInstance()->AddEffect(peff);

		//총알 생성

		vpos = vpos + vdir*30;
		CObj* pbullet = new CBattle_bim(ptarget->GetObjNumber() , ptarget->GetPos() );
		pbullet->SetPos(vpos);
		pbullet->Initialize();
		CObjMgr::GetInstance()->AddObject(pbullet , OBJ_BULLET);
	}
	else
	{
		if(MOVE == m_pobj->GetUnitinfo().state)
			m_pobj->SetState(IDLE);
	}
}

void CCom_WBattle::Update(void)
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

void CCom_WBattle::Render(void)
{

}

void CCom_WBattle::Release(void)
{

}

