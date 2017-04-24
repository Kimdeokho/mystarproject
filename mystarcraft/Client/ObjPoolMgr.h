#pragma once

#include "Include.h"

class CObj;
class CDrone;
class CObjPoolMgr
{
	DECLARE_SINGLETON(CObjPoolMgr)
private:
	boost::object_pool<CDrone>*	m_ZUnitpool[ZU_END];

public:
	static 	int					m_Obj_Cnt;
public:
	void Initialize(void);
	void Release(void);
public:
	CObj* construct(ZERG_UNIT_ID eId);
	void destroy(CObj* pobj ,ZERG_UNIT_ID eId);
private:
	CObjPoolMgr(void);
public:
	~CObjPoolMgr(void);
};
