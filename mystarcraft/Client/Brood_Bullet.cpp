#include "StdAfx.h"
#include "Brood_Bullet.h"

#include "Com_dirBulletAnim.h"
#include "TimeMgr.h"

#include "ObjMgr.h"
#include "GeneraEff.h"
#include "MyMath.h"

#include "ScrollMgr.h"

#include "Brude.h"

CBrood_Bullet::CBrood_Bullet(TEAM_NUMBER eteam , const int& ptarget_id , const D3DXVECTOR2& vdesetpos)
{
	m_ptarget = NULL;
	m_target_id = ptarget_id;
	m_vdest_pos = vdesetpos;
	m_eteam = eteam;
}

CBrood_Bullet::~CBrood_Bullet(void)
{
	Release();
}
void CBrood_Bullet::Initialize(void)
{
	m_sortID = SORT_AIR_EFF;

	m_vcurdir = m_vdest_pos - m_vPos;
	D3DXVec2Normalize(&m_vcurdir , &m_vcurdir);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , new CCom_dirBulletAnim(m_matWorld , L"PARASITE_BULLET")));

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_ftick_distance = GETTIME*450;
}

void CBrood_Bullet::Update(void)
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

	if(CMyMath::pos_distance(m_vPos , m_vdest_pos) < m_ftick_distance*m_ftick_distance)
	{
		if( NULL != m_ptarget)
			m_ptarget->SetDamage(m_ptarget->GetUnitinfo().maxhp, DAMAGE_NOMAL);

		m_bdestroy = true;
		Dead();
	}

	m_vPos += vdir*m_ftick_distance;

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;
}

void CBrood_Bullet::Render(void)
{

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Render();
}


void CBrood_Bullet::Dead(void)
{
	CObj* pobj = NULL;

	for(int i = 0; i < 2; ++i)
	{
		pobj = new CBrude;
		CObjMgr::GetInstance()->AddObject(pobj , OBJ_BRUDE);

		pobj->SetTeamNumber(m_eteam);
		pobj->SetPos(m_vPos.x + i*10.f , m_vPos.y);
		pobj->Initialize();
	}
}

void CBrood_Bullet::Release(void)
{

}