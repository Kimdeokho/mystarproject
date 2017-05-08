#include "StdAfx.h"
#include "ObjPoolMgr.h"



#include "Obj.h"
#include "ObjMgr.h"
#include "Drone.h"
#include "Hatchery.h"

IMPLEMENT_SINGLETON(CObjPoolMgr)
CObjPoolMgr::CObjPoolMgr(void)
{
}

CObjPoolMgr::~CObjPoolMgr(void)
{
	Release();
}

//void CObjPoolMgr::Initialize(void)
//{
//	for(int i = 0; i < ZU_END; ++i)
//	{
//		m_ZUnitpool[i] = NULL;
//	}
//
//	for(int i = 0; i < ZB_END; ++i)
//	{
//		m_ZBuildingpool[i] = NULL;
//	}
//
//	CObj* pobj = NULL;
//
//	m_ZUnitpool[ZU_DRONE] = new boost::object_pool<CDrone>(MAX_OBJECTCNT);
//	pobj = m_ZUnitpool[ZU_DRONE]->construct();
//
//	m_ZBuildingpool[ZB_HACHERY] = new boost::object_pool<CHatchery>(MAX_OBJECTCNT);
//	pobj = m_ZBuildingpool[ZB_HACHERY]->construct();
//}
//CObj* CObjPoolMgr::construct(ZERG_UNIT_ID eId)
//{
//	m_Obj_Cnt += 1;
//
//	return m_ZUnitpool[eId]->construct();
//}
//CObj* CObjPoolMgr::construct(ZERG_BUILDING_ID eId)
//{
//	m_Obj_Cnt += 1;
//
//	return m_ZBuildingpool[eId]->construct();
//}
//void CObjPoolMgr::destroy(CObj* pobj , ZERG_UNIT_ID eId)
//{
//	m_Obj_Cnt -= 1;
//
//	if(ZU_DRONE == eId)
//	{
//		m_ZUnitpool[eId]->destroy((CDrone*)pobj);
//	}
//}
//void CObjPoolMgr::destroy(CObj* pobj , ZERG_BUILDING_ID eId)
//{
//	m_Obj_Cnt -= 1;
//
//	if(ZB_HACHERY == eId)
//	{
//		m_ZBuildingpool[eId]->destroy((CHatchery*)pobj);
//	}
//}
void CObjPoolMgr::Release(void)
{
	for(int i = 0; i < ZU_END; ++i)
	{
		//if( NULL != m_ZUnitpool[i])
			Safe_Delete(m_ZUnitpool[i]);
	}

	for(int i = 0; i < ZB_END; ++i)
	{
		Safe_Delete(m_ZBuildingpool[i]);
	}
}

int CObjPoolMgr::m_Obj_Cnt(0);