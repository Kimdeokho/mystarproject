#include "StdAfx.h"
#include "Com_WVulture.h"

#include "Obj.h"
#include "Com_Animation.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "GeneraEff.h"

#include "Vulture_Bullet.h"
CCom_WVulture::CCom_WVulture(void)
{
}

CCom_WVulture::~CCom_WVulture(void)
{
}

void CCom_WVulture::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	m_animation = (m_pobj->GetComponent(COM_ANIMATION));

	m_attack_delay = 1.25f;
	m_weapon_info.damage = 20;
	m_weapon_info.airdamage = 0;
	m_weapon_info.eDamageType = DAMAGE_VIBRATE;
	m_weapon_info.eAirDamageType = DAMAGE_NOMAL;

	m_bfire = false;
}

void CCom_WVulture::fire(CObj*& ptarget)
{
	if(false == m_bfire)
	{
		m_pobj->SetState(IDLE);

		//if( true == ((CCom_Animation*)m_animation)->GetRotationComplete())
		{
			m_pobj->SetState(ATTACK);
			m_bfire = true;

			m_attack_time = 0.f;
			//총알생성
			D3DXVECTOR2 vpos;
			vpos = m_pobj->GetPos();
			vpos = vpos + m_pobj->GetcurDir() * 30;
			CObj* peff = NULL;

			peff = new CGeneraEff(L"VULTURE_FIRE" ,vpos , D3DXVECTOR2(1,1), SORT_GROUND_EFF , 1.5f);
			peff->Initialize();
			CObjMgr::GetInstance()->AddEffect(peff);

			CObj* pbullet;
			pbullet = new CVulture_Bullet(ptarget->GetObjNumber() , ptarget->GetPos());
			pbullet->SetPos(vpos);
			pbullet->Initialize();
			CObjMgr::GetInstance()->AddObject(pbullet , OBJ_BULLET);
			
		}
	}
	else
	{
		if(MOVE == m_pobj->GetUnitinfo().state)
			m_pobj->SetState(IDLE);
	}
}

void CCom_WVulture::Update(void)
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

void CCom_WVulture::Render(void)
{

}

void CCom_WVulture::Release(void)
{

}


