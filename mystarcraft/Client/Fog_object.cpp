#include "StdAfx.h"
#include "Fog_object.h"

#include "TimeMgr.h"
#include "Com_fog.h"
#include "MyMath.h"
CFog_object::CFog_object(const int& irange , const float& fogtime , const D3DXVECTOR2& vpos)
{
	m_irange = irange;
	m_fogtime = fogtime;
	m_vPos = vpos;
}

CFog_object::~CFog_object(void)
{
	Release();
}

void CFog_object::Initialize(void)
{
	

	m_sortID = SORT_GROUND;
	m_eOBJ_NAME = OBJ_FOG;
	m_eteamnumber = TEAM_NONE;

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_FOG , new CCom_fog(m_curidx32 , &m_irange) ));
	m_curtime = 0.f;

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);
}

void CFog_object::Update(void)
{
	m_curidx32 = CMyMath::Pos_to_index(m_vPos ,32);

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();

	m_curtime += GETTIME;

	if(m_curtime > m_fogtime )
		m_bdestroy = true;
}

void CFog_object::Render(void)
{

}

void CFog_object::Release(void)
{

}

void CFog_object::Dead(void)
{

}
