#include "StdAfx.h"
#include "Turret_bullet.h"

#include "Com_dirBulletAnim.h"
#include "TimeMgr.h"

#include "ObjMgr.h"
#include "GeneraEff.h"
#include "MyMath.h"

#include "ScrollMgr.h"

CTurret_bullet::CTurret_bullet(const int& target_id ,const D3DXVECTOR2& vdesetpos)
{
	m_ptarget = NULL;
	m_target_id = target_id;
	m_vdest_pos = vdesetpos;
}

CTurret_bullet::~CTurret_bullet(void)
{
	Release();
}

void CTurret_bullet::Initialize(void)
{
	m_sortID = SORT_AIR_EFF;

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , new CCom_dirBulletAnim( m_matWorld , L"TURRET_BULLET" )));

	m_vcurdir = m_vdest_pos - m_vPos;
	m_trail_time = 0.15f;
	m_accel = 10.f;
	m_accel2 = 0.1f;

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);


}

void CTurret_bullet::Update(void)
{
	m_accel2 += GETTIME*6;
	m_accel += GETTIME*500;
	if(m_accel2 > 1)
	{
		m_accel2 = 1.f;
	}
	if(m_accel > 500)
	{
		m_accel = 500;
	}

	m_trail_time += GETTIME;
	if(m_trail_time > 0.15f)
	{
		m_trail_time = 0.f;
		CObj* peff = new CGeneraEff(L"TURRET_TRAIL" ,m_vPos , D3DXVECTOR2(1.3f , 1.3f), SORT_AIR_EFF);
		peff->Initialize();
		CObjMgr::GetInstance()->AddEffect(peff);
	}

	m_ptarget = CObjMgr::GetInstance()->obj_alivecheck(m_target_id);

	//타겟을 쫓지말고 OBJID로 검사해야할듯...
	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();


	if(NULL != m_ptarget)
	{
		m_vdest_pos = m_ptarget->GetPos();
		m_old_targetpos = m_vdest_pos;
	}
	else
	{
		m_target_id = 0;
		m_vdest_pos = m_old_targetpos;
	}

	m_vcurdir = m_vdest_pos - m_vPos;
	D3DXVec2Normalize(&m_vcurdir , &m_vcurdir);
	m_ftick_distance = GETTIME*m_accel*m_accel2;

	if(CMyMath::pos_distance(m_vPos , m_vdest_pos) < m_ftick_distance*m_ftick_distance)
	{
		if(NULL != m_ptarget)
			m_ptarget->SetDamage(20, DAMAGE_BOOM);

		m_bdestroy = true;
		Dead();
	}

	m_vPos += m_vcurdir*m_ftick_distance;
}

void CTurret_bullet::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Render();
}

void CTurret_bullet::Release(void)
{
}

void CTurret_bullet::Dead(void)
{
	CObj* peff = NULL;
	if(NULL != m_ptarget)
		peff = new CGeneraEff(L"ToongToong" ,m_ptarget->GetPos()  , D3DXVECTOR2(1.3f , 1.3f), SORT_AIR_EFF);
	else
		peff = new CGeneraEff(L"ToongToong" ,m_vdest_pos , D3DXVECTOR2(1.3f , 1.3f), SORT_AIR_EFF);

	peff->Initialize();
	CObjMgr::GetInstance()->AddEffect(peff);

}
