#include "StdAfx.h"
#include "Yama_bim.h"

#include "GeneraEff.h"
#include "MyMath.h"
#include "TimeMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"

#include "Com_LaserAnim.h"
CYama_bim::CYama_bim(const int& target_id ,const D3DXVECTOR2& vdesetpos)
{
	m_ptarget = NULL;
	m_target_id = target_id;
	m_vdest_pos = vdesetpos;
}

CYama_bim::~CYama_bim(void)
{
	Release();
}

void CYama_bim::Initialize(void)
{
	m_hit_sort = SORT_END;
	m_sortID = SORT_AIR_EFF;
	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , new CCom_LaserAnim( m_matWorld , L"YAMA_BIM" , 1.f)));

	m_vcurdir = m_vdest_pos - m_vPos;
	D3DXVec2Normalize(&m_vcurdir , &m_vcurdir);
	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_ftick_distance = GETTIME*1000;
}

void CYama_bim::Update(void)
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

		if( SORT_END == m_hit_sort)
		{
			if( MOVE_GROUND == m_ptarget->GetUnitinfo().eMoveType)
				m_hit_sort = SORT_GROUND_EFF;
			else if( MOVE_AIR == m_ptarget->GetUnitinfo().eMoveType)
				m_hit_sort = SORT_AIR_EFF;
		}
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
			CSoundDevice::GetInstance()->PlayBattleSound(SND_B_YAMA_HIT , m_ptarget->GetPos());
			m_ptarget->SetDamage(260, DAMAGE_BOOM);
		}

		m_bdestroy = true;
		Dead();
	}	


}

void CYama_bim::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Render();
}

void CYama_bim::Release(void)
{

}

void CYama_bim::Dead(void)
{
	CObj* peff = NULL;
	if(NULL != m_ptarget)
	{
		peff = new CGeneraEff(L"YAMA_HIT" ,m_ptarget->GetPos()  , D3DXVECTOR2(1.1f , 1.1f), m_hit_sort );
	}
	else
		peff = new CGeneraEff(L"YAMA_HIT" ,m_vdest_pos , D3DXVECTOR2(1.1f , 1.1f), m_hit_sort );

	peff->Initialize();
	CObjMgr::GetInstance()->AddEffect(peff);
}
