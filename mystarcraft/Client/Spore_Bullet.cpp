#include "StdAfx.h"
#include "Spore_Bullet.h"

#include "Com_BulletAnim.h"
#include "TimeMgr.h"

#include "ObjMgr.h"
#include "GeneraEff.h"
#include "MyMath.h"

#include "ScrollMgr.h"

CSpore_Bullet::CSpore_Bullet(const int target_id ,const D3DXVECTOR2& vdesetpos)
{
	m_ptarget = NULL;
	m_target_id = target_id;
	m_vdest_pos = vdesetpos;
}

CSpore_Bullet::~CSpore_Bullet(void)
{
}

void CSpore_Bullet::Initialize(void)
{
	m_sortID = SORT_GROUND_EFF;

	m_vcurdir = m_vdest_pos - m_vPos;
	D3DXVec2Normalize(&m_vcurdir , &m_vcurdir);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , new CCom_BulletAnim(m_matWorld , L"SPORE_BULLET")));

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_ftick_distance = GETTIME*450;
}

void CSpore_Bullet::Update(void)
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
			m_ptarget->SetDamage(15, DAMAGE_NOMAL);

		m_bdestroy = true;
		Dead();
	}	

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;
}

void CSpore_Bullet::Render(void)
{

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Render();
}
void CSpore_Bullet::Dead(void)
{
	CObj* peff = NULL;
	if(NULL != m_ptarget)
		peff = new CGeneraEff(L"MUTAL_HIT" ,m_ptarget->GetPos()  , D3DXVECTOR2(1.1f , 1.1f), SORT_AIR_EFF );
	else
		peff = new CGeneraEff(L"MUTAL_HIT" ,m_vdest_pos , D3DXVECTOR2(1.1f , 1.1f), SORT_AIR_EFF );

	peff->Initialize();
	CObjMgr::GetInstance()->AddEffect(peff);
}
void CSpore_Bullet::Release(void)
{

}