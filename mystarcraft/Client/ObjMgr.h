#pragma once

#include "Include.h"

class CObj;
class CObjMgr
{
public:
	DECLARE_SINGLETON(CObjMgr)
private:
	list<CObj*>		m_ObjList[OBJ_END];
	list<CObj*>		m_ZBuilding_List[ZB_END];
	list<CObj*>		m_ZUnit_List[ZU_END];
	multimap<float , CObj*> m_rendersort[SORT_END];

public:
	void Release();
public:
	void AddObject(CObj* pObj , OBJID eid);
	void AddObject(CObj* pObj , ZERG_BUILDING_ID eid);
	void AddObject(CObj* pObj , ZERG_UNIT_ID eid);

	void DestroyObj(ZERG_BUILDING_ID eid);
public:
	void LoadObj(HANDLE hFile);
public:
public:
	void Update(void);
	void Render(void);
private:
	CObjMgr(void);
public:
	~CObjMgr(void);
};
