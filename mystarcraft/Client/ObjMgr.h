#pragma once

#include "Include.h"

const int MAX_OBJCNT = 1601;
class CObj;
class CUI;
class CEffect;
class CObjMgr
{
public:
	DECLARE_SINGLETON(CObjMgr)
private:
	list<CObj*>		m_ObjList[OBJ_END];
	list<CObj*>		m_ZBuilding_List[ZB_END];
	list<CObj*>		m_ZUnit_List[ZU_END];

	list<CObj*>		m_TBuilding_List[TB_END];
	list<CObj*>		m_TUnit_List[TU_END];
	multimap<float , CObj*> m_rendersort[SORT_END];


	list<CUI*>		m_SelectUI_List;
	list<CObj*>	m_Effect_List;
	CObj*			m_obj_alive[MAX_OBJCNT];
public:
	void Release();
public:
	bool obj_place(CObj* pobj);
	void obj_leave(const int& idx);
	CObj* obj_alivecheck(const int& idx);
public:
	void AddObject(CObj* pObj , OBJID eid);
	void AddObject(CObj* pObj , ZERG_BUILDING_ID eid);
	void AddObject(CObj* pObj , ZERG_UNIT_ID eid);

	void AddObject(CObj* pObj , TERRAN_BUILDING_ID eid);
	void AddObject(CObj* pObj , TERRAN_UNIT_ID eid);

	void AddSelect_UI(CUI* pui);
	void AddEffect(CObj* peff);

	void DestroyObj(TERRAN_UNIT_ID eid);
public:
	void LoadObj(HANDLE hFile);
public:
public:
	void Update(void);
	void Destroy_Update(void);
	void Render(void);
private:
	CObjMgr(void);
public:
	~CObjMgr(void);
};
