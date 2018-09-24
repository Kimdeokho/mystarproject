#include "StdAfx.h"
#include "Com_WGoliath.h"

#include "Obj.h"
#include "Com_Animation.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "GeneraEff.h"
#include "MultiEff.h"

#include "Goliath_Bullet.h"
#include "Goliath_part.h"
CCom_WGoliath::CCom_WGoliath(void)
{
}

CCom_WGoliath::~CCom_WGoliath(void)
{
}

void CCom_WGoliath::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	m_animation = (m_pobj->GetComponent(COM_ANIMATION));

	m_attack_delay = 0.9166f;
	m_weapon_info.damage = 12;
	m_weapon_info.airdamage = 20;
	m_weapon_info.eDamageType = DAMAGE_NOMAL;
	m_weapon_info.eAirDamageType = DAMAGE_BOOM;

	m_bfire = false;

}
void CCom_WGoliath::fire(CObj*& ptarget)
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
			//타겟에게 데미지 ㄱㄱㄱㄱ

			if(MOVE_GROUND == ptarget->GetUnitinfo().eMoveType)
			{
				CObj* peff = NULL;

				peff = new CGeneraEff(L"GaussGun" ,ptarget->GetPos() , D3DXVECTOR2(1.2f,1.2f), SORT_GROUND_EFF , 1.4f);
				peff->Initialize();
				CObjMgr::GetInstance()->AddEffect(peff);
				(ptarget)->SetDamage(m_weapon_info.damage + m_upg_info[UPG_T_MECHANIC_WEAPON].upg_cnt
					, m_weapon_info.eDamageType);

				((CGoliath_part*)m_pobj)->settarget_movetype(MOVE_GROUND);
			}
			else
			{
				//총알 생성
				D3DXVECTOR2 vdir = D3DXVECTOR2(ptarget->GetPos() - m_pobj->GetPos());
				D3DXVec2Normalize(&vdir , &vdir);

				D3DXVECTOR2 subdir = D3DXVECTOR2(vdir.y, -vdir.x);
				D3DXVECTOR2 subdir2 = D3DXVECTOR2(-vdir.y, vdir.x);


				CObj* pbullet = new CGoliath_Bullet(ptarget->GetObjNumber() , ptarget->GetPos() ,LEFT);
				pbullet->SetPos(m_pobj->GetPos().x , m_pobj->GetPos().y );
				pbullet->Initialize();
				CObjMgr::GetInstance()->AddObject(pbullet , OBJ_BULLET);

				pbullet = new CGoliath_Bullet(ptarget->GetObjNumber() , ptarget->GetPos() , RIGHT);
				pbullet->SetPos(m_pobj->GetPos().x , m_pobj->GetPos().y );
				pbullet->Initialize();
				CObjMgr::GetInstance()->AddObject(pbullet , OBJ_BULLET);

				((CGoliath_part*)m_pobj)->settarget_movetype(MOVE_AIR);
			}
		}
	}
	else
	{
		if(MOVE == m_pobj->GetUnitinfo().state)
			m_pobj->SetState(IDLE);
	}
}

void CCom_WGoliath::Update(void)
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

void CCom_WGoliath::Render(void)
{

}

void CCom_WGoliath::Release(void)
{

}

