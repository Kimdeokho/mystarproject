#pragma once

#include "Include.h"

class CObj;
class CDrone;
class CHatchery;
class CObjPoolMgr
{
	DECLARE_SINGLETON(CObjPoolMgr)
private:
private:
	boost::object_pool<CDrone>*	m_ZUnitpool[ZU_END];
	boost::object_pool<CHatchery>*	m_ZBuildingpool[ZB_END];

public:
	static 	int					m_Obj_Cnt;
public:
	//void Initialize(void);
	void Release(void);
public:
	//CObj* construct(ZERG_UNIT_ID eId);
	//CObj* construct(ZERG_BUILDING_ID eId);

	//void destroy(CObj* pobj ,ZERG_UNIT_ID eId);
	//void destroy(CObj* pobj ,ZERG_BUILDING_ID eId);
private:
	CObjPoolMgr(void);
public:
	~CObjPoolMgr(void);
};
