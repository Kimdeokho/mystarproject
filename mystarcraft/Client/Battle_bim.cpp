#include "StdAfx.h"
#include "Battle_bim.h"

#include "Com_LaserAnim.h"
#include "TimeMgr.h"

#include "ObjMgr.h"
#include "GeneraEff.h"
#include "MyMath.h"

#include "ScrollMgr.h"
CBattle_bim::CBattle_bim(const int& ptarget_id , const D3DXVECTOR2& vdesetpos)
{
	m_ptarget = NULL;
	m_target_id = ptarget_id;
	m_vdest_pos = vdesetpos;
}

CBattle_bim::~CBattle_bim(void)
{
}

void CBattle_bim::Initialize(void)
{
	m_sortID = SORT_AIR_EFF;

	m_vcurdir = m_vdest_pos - m_vPos;
	D3DXVec2Normalize(&m_vcurdir , &m_vcurdir);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , new CCom_LaserAnim(m_matWorld , L"BATTLE_BIM")));

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);
}

void CBattle_bim::Update(void)
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
	m_vPos += vdir*GETTIME*800;

	if(CMyMath::pos_distance(m_vPos , m_vdest_pos) < 8*8)
	{
		if(NULL != m_ptarget)
			m_ptarget->SetDamage(25, DAMAGE_NOMAL);

		m_bdestroy = true;
		Dead();
	}	
}

void CBattle_bim::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Render();
}

void CBattle_bim::Release(void)
{

}

void CBattle_bim::Dead(void)
{
	CObj* peff = NULL;
	if(NULL != m_ptarget)
		peff = new CGeneraEff(L"BIM_HIT" ,m_ptarget->GetPos()  , D3DXVECTOR2(1.1f , 1.1f), SORT_AIR_EFF , 2.f);
	else
		peff = new CGeneraEff(L"BIM_HIT" ,m_vdest_pos , D3DXVECTOR2(1.1f , 1.1f), SORT_AIR_EFF , 2.f);

	peff->Initialize();
	CObjMgr::GetInstance()->AddEffect(peff);
}
