#include "StdAfx.h"
#include "Vulture_Bullet.h"

#include "Com_BulletAnim.h"
#include "TimeMgr.h"

#include "ObjMgr.h"
#include "GeneraEff.h"
#include "MyMath.h"

#include "ScrollMgr.h"
#include "Skill_DarkSwarm.h"

CVulture_Bullet::CVulture_Bullet(const int target_id ,const D3DXVECTOR2& vdesetpos)
{
	m_ptarget = NULL;
	m_target_id = target_id;
	m_vdest_pos = vdesetpos;
}

CVulture_Bullet::~CVulture_Bullet(void)
{
}

void CVulture_Bullet::Initialize(void)
{
	m_sortID = SORT_GROUND_EFF;

	m_vcurdir = m_vdest_pos - m_vPos;
	D3DXVec2Normalize(&m_vcurdir , &m_vcurdir);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , new CCom_BulletAnim(m_matWorld , L"VULTURE_BULLET")));

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_ftick_distance = GETTIME*450;
}

void CVulture_Bullet::Update(void)
{
	m_ptarget = CObjMgr::GetInstance()->obj_alivecheck(m_target_id);

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

	D3DXVECTOR2 vdir = m_vdest_pos - m_vPos;	
	D3DXVec2Normalize(&vdir , &vdir);
	m_vPos += vdir*m_ftick_distance;

	if(CMyMath::pos_distance(m_vPos , m_vdest_pos) < m_ftick_distance*m_ftick_distance)
	{
		if(NULL != m_ptarget)
		{
			int idx = m_ptarget->Getcuridx(32);
			if(MOVE_GROUND == m_ptarget->GetUnitinfo().eMoveType && 
				CSkill_DarkSwarm::m_darkswarm_cnt[idx] == 0)
				m_ptarget->SetDamage(20, DAMAGE_VIBRATE);
		}

		m_bdestroy = true;
		Dead();
	}	

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;
}

void CVulture_Bullet::Render(void)
{

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Render();
}

void CVulture_Bullet::Release(void)
{

}

void CVulture_Bullet::Dead(void)
{
	CObj* peff = NULL;
	if(NULL != m_ptarget)
		peff = new CGeneraEff(L"VULTURE_HIT" ,m_ptarget->GetPos()  , D3DXVECTOR2(1.1f , 1.1f), SORT_GROUND_EFF );
	else
		peff = new CGeneraEff(L"VULTURE_HIT" ,m_vdest_pos , D3DXVECTOR2(1.1f , 1.1f), SORT_GROUND_EFF );

	peff->Initialize();
	CObjMgr::GetInstance()->AddEffect(peff);
}
