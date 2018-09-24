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
	list<CObj*>		m_CorpseList;
	//list<CObj*>		m_ZBuilding_List[ZB_END];
	//list<CObj*>		m_ZUnit_List[ZU_END];

	//list<CObj*>		m_TBuilding_List[TB_END];
	//list<CObj*>		m_TUnit_List[TU_END];
	multimap<float , CObj*> m_rendersort[SORT_END];
	list<CObj*>				m_air_rendersort;
	list<CObj*>				m_groundeff_renderlist;
	list<CObj*>				m_aireff_renderlist;


	//list<CUI*>		m_SelectUI_List;
	//list<CUI*>		m_AirSelectUI_List;
	//list<CUI*>		m_energybar_list;
	//list<CUI*>		m_air_energybar_list;
	list<CObj*>		m_Effect_List;
	CObj*			m_obj_alive[MAX_OBJCNT];
public:
	void Release();
public:
	bool obj_place(CObj* pobj);
	void obj_leave(const int& idx);
	CObj* obj_alivecheck(const int& idx);
public:
	void AddObject(CObj* pObj , OBJID eid);
	void AddCorpse(CObj* pObj );

	void AddEffect(CObj* peff);

	CObj* GetCoreBuilding(CObj* pself);
	void  UpdateSelect_UI(void);

	void  Place_Terran(D3DXVECTOR2 vpos , TEAM_NUMBER eteam);
	void  Place_Zerg(D3DXVECTOR2 vpos , TEAM_NUMBER eteam);
	void  Nuclear_launch(void);

	CObj*	GetObj(const USHORT objnum);
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
