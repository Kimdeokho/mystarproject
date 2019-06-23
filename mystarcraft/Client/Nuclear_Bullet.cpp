#include "StdAfx.h"
#include "Nuclear_Bullet.h"

#include "Com_dirBulletAnim.h"
#include "TimeMgr.h"

#include "ObjMgr.h"
#include "GeneraEff.h"
#include "MyMath.h"

#include "Area_Mgr.h"
#include "ScrollMgr.h"
CNuclear_Bullet::CNuclear_Bullet(const D3DXVECTOR2& vdesetpos)
{
	m_vdest_pos = vdesetpos;
	m_vPos = m_vdest_pos;
	m_vPos.y -= BACKBUFFER_SIZEY/2;
}

CNuclear_Bullet::~CNuclear_Bullet(void)
{
	Release();
}

void CNuclear_Bullet::Initialize(void)
{
	m_sortID = SORT_AIR_EFF;

	m_vcurdir = m_vdest_pos - m_vPos;
	D3DXVec2Normalize(&m_vcurdir , &m_vcurdir);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , new CCom_dirBulletAnim(m_matWorld , L"NUCLEAR_BULLET")));

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_splash_range[0] = 4*32;
	m_splash_range[1] = 6*32;
	m_splash_range[2] = 8*32;

	m_ftrail_time = 0.f;

	m_ftick_distance = GETTIME*100;
}

void CNuclear_Bullet::Update(void)
{
	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();


	m_ftrail_time += GETTIME;
	if(m_ftrail_time > 0.25f)
	{
		m_ftrail_time = 0.f;
		D3DXVECTOR2	vpos = m_vPos;
		vpos.y -= 15;
		CObj* peff = new CGeneraEff(L"TURRET_TRAIL" ,vpos , D3DXVECTOR2(1.3f , 1.3f), SORT_AIR_EFF);
		peff->Initialize();
		CObjMgr::GetInstance()->AddEffect(peff);
	}


	D3DXVECTOR2 vdir = D3DXVECTOR2(0,1);
	m_vPos += vdir*GETTIME*100;

	if(CMyMath::pos_distance(m_vPos , m_vdest_pos) < m_ftick_distance*m_ftick_distance)
	{
		CArea_Mgr::GetInstance()->Setsplash_damage(this , 500 , DAMAGE_BOOM , m_vdest_pos ,
			m_splash_range, true, m_injurelist);

		m_injurelist.clear();

		m_bdestroy = true;
		Dead();
	}

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;
}

void CNuclear_Bullet::Render(void)
{
	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Render();
}

void CNuclear_Bullet::Release(void)
{
	m_injurelist.clear();
}

void CNuclear_Bullet::Dead(void)
{
	m_vdest_pos.y -= 63;
	CObj* peff = new CGeneraEff(L"NUCLEAR_BOOM" ,m_vdest_pos , D3DXVECTOR2(1.1f , 1.1f), SORT_AIR_EFF ,0.65f);

	peff->Initialize();
	CObjMgr::GetInstance()->AddEffect(peff);
}
