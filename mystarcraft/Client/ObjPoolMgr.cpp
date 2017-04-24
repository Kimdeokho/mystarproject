#include "StdAfx.h"
#include "ObjPoolMgr.h"



#include "Obj.h"
#include "ObjMgr.h"
#include "Drone.h"

IMPLEMENT_SINGLETON(CObjPoolMgr)
CObjPoolMgr::CObjPoolMgr(void)
{
}

CObjPoolMgr::~CObjPoolMgr(void)
{
	Release();
}

void CObjPoolMgr::Initialize(void)
{
	m_ZUnitpool[ZU_DRONE] = new boost::object_pool<CDrone>(MAX_OBJECTCNT);
	CObj* pobj = m_ZUnitpool[ZU_DRONE]->construct();
}
CObj* CObjPoolMgr::construct(ZERG_UNIT_ID eId)
{
	m_Obj_Cnt += 1;

	return m_ZUnitpool[eId]->construct();
}
void CObjPoolMgr::destroy(CObj* pobj , ZERG_UNIT_ID eId)
{
	m_Obj_Cnt -= 1;

	if(ZU_DRONE == eId)
	{
		m_ZUnitpool[eId]->destroy((CDrone*)pobj);
	}
}
void CObjPoolMgr::Release(void)
{
	for(int i = 0; i < ZU_END; ++i)
		delete m_ZUnitpool[i];
}

int CObjPoolMgr::m_Obj_Cnt(0);