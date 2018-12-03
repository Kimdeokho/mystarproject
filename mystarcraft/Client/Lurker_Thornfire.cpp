#include "StdAfx.h"
#include "Lurker_Thornfire.h"

#include "ObjMgr.h"

#include "Thorn_Bullet.h"
#include "TimeMgr.h"
#include "Area_Mgr.h"

#include "Ingame_UIMgr.h"
CLurker_Thornfire::CLurker_Thornfire(const D3DXVECTOR2& vstart , const D3DXVECTOR2& vend,const int range , const WEAPON_INFO& tinfo ,CObj* pobj)
{
	m_vstart = vstart;
	m_vend = vend;
	D3DXVec2Normalize(&m_vcurdir , &(m_vend - m_vstart));
	m_range = range / 32;
	m_weapon_info = tinfo;
	m_pobj = pobj;
}

CLurker_Thornfire::~CLurker_Thornfire(void)
{
}

void CLurker_Thornfire::Initialize(void)
{
	//유닛 사거리 + 1

	//가시의 반지름은 32*0.625
	//간격은 32*1.25

	m_sortID = SORT_CORPSE;
	m_thorn_delay = 0.f;
	m_curcnt = 0;

	for(int i = 0; i < 3; ++i)
		m_splash_range[i] = 32*0.625f;

	m_upg_info = CIngame_UIMgr::GetInstance()->GetUpginfo();
}

void CLurker_Thornfire::Update(void)
{
	m_thorn_delay += GETTIME;

	if(m_thorn_delay > 0.08f)
	{
		++m_curcnt;
		m_thorn_delay = 0.f;

		CObj* pbullet = NULL;

		D3DXVECTOR2 vpos;
		vpos = m_vstart + m_vcurdir * (m_curcnt * 32 * 1.f); //1.25에서 바꿈

		pbullet = new CThorn_Bullet;

		CObjMgr::GetInstance()->AddObject(pbullet , OBJ_BULLET);
		pbullet->SetPos(vpos);
		pbullet->Initialize();


		for(int i = 0; i < 3; ++i)
		{
			vpos = m_vstart + m_vcurdir * ((m_curcnt + i) * 32 * 1.f); //1.25에서 바꿈

			CArea_Mgr::GetInstance()->Setsplash_damage(m_pobj, 
				m_weapon_info.damage + m_upg_info[UPG_Z_MISSILE_ATT].upg_cnt*2,
				m_weapon_info.eDamageType , vpos,  
				m_splash_range , false , m_injure_list);			
		}

		if(m_curcnt >= m_range)
		{			
			SetDestroy(true);
			m_injure_list.clear();
			m_curcnt = 0;
		}
	}
}

void CLurker_Thornfire::Render(void)
{

}

void CLurker_Thornfire::Release(void)
{

}
