#include "StdAfx.h"
#include "ObjMgr.h"

#include "Obj.h"
#include "Mineral.h"
#include "GasResource.h"

#include "Zerg_Building.h"
#include "ObjPoolMgr.h"
#include "Area_Mgr.h"
#include "UI.h"
#include "UnitMgr.h"
#include "Effect.h"

IMPLEMENT_SINGLETON(CObjMgr)
CObjMgr::CObjMgr(void)
{
	memset( m_obj_alive , 0 , sizeof(bool)*MAX_OBJCNT );
}

CObjMgr::~CObjMgr(void)
{
	Release();
}
void CObjMgr::Update(void)
{

	float fy = 0.f;
	SORT_ID sortid = SORT_END;

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iter_end;

	for(int i = 0; i < OBJ_END; ++i)
	{
		iter = m_ObjList[i].begin();
		iter_end = m_ObjList[i].end();

		for( ; iter != iter_end; ++iter)
		{
			(*iter)->Update();
				
		}
	}


	for(int i = 0; i < ZB_END; ++i)
	{
		iter = m_ZBuilding_List[i].begin();
		iter_end = m_ZBuilding_List[i].end();

		for( ; iter != iter_end; ++iter) 
		{
			(*iter)->Update();
		}
	}

	for(int i = 0; i < ZU_END; ++i)
	{
		iter = m_ZUnit_List[i].begin();
		iter_end = m_ZUnit_List[i].end();

		for( ; iter != iter_end; ++iter)
		{
			(*iter)->Update();
		}
	}

	for(int i = 0; i < TB_END; ++i)
	{
		iter = m_TBuilding_List[i].begin();
		iter_end = m_TBuilding_List[i].end();

		for( ; iter != iter_end; ++iter)
		{

			(*iter)->Update();
		}
	}

	for(int i = 0; i < TU_END; ++i)
	{
		iter = m_TUnit_List[i].begin();
		iter_end = m_TUnit_List[i].end();

		for( ; iter != iter_end; ++iter)
		{
			(*iter)->Update();
		}
	}

	if(!m_Effect_List.empty())
	{
		list<CObj*>::iterator iter_eff = m_Effect_List.begin();
		list<CObj*>::iterator iter_eff_end = m_Effect_List.end();

		for( ; iter_eff != iter_eff_end; ++iter_eff)
		{
			(*iter_eff)->Update();
		}
	}

	Destroy_Update();
}
void CObjMgr::Destroy_Update(void)
{


	float fy = 0.f;
	SORT_ID sortid = SORT_END;

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iter_end;

	for(int i = 0; i < OBJ_END; ++i)
	{
		iter = m_ObjList[i].begin();
		iter_end = m_ObjList[i].end();

		for( ; iter != iter_end; )
		{
			if(true == (*iter)->GetDestroy() )
			{
				if( i != OBJ_CORPSE)
					obj_leave( (*iter)->GetObjID() );

				Safe_Delete(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
			{
				fy = (*iter)->GetY();
				sortid = (*iter)->GetsortID();
				if(true == (*iter)->Be_in_camera()  )
					m_rendersort[ sortid ].insert( make_pair( fy , (*iter)) );

				++iter;
			}
		}
	}


	for(int i = 0; i < ZB_END; ++i)
	{
		iter = m_ZBuilding_List[i].begin();
		iter_end = m_ZBuilding_List[i].end();

		for( ; iter != iter_end;)
		{
			if(true == (*iter)->GetDestroy() )
			{
				obj_leave( (*iter)->GetObjID() );
				Safe_Delete(*iter);
				iter = m_ZBuilding_List[i].erase(iter);
				continue;
			}
			else
			{
				fy = (*iter)->GetY();
				sortid = (*iter)->GetsortID();
				if(true == (*iter)->Be_in_camera()  )
					m_rendersort[ sortid ].insert( make_pair( fy , (*iter)) );

				++iter;
			}
		}
	}

	for(int i = 0; i < ZU_END; ++i)
	{
		iter = m_ZUnit_List[i].begin();
		iter_end = m_ZUnit_List[i].end();

		for( ; iter != iter_end; )
		{		

			if(true == (*iter)->GetDestroy() )
			{
				obj_leave( (*iter)->GetObjID() );
				Safe_Delete(*iter);
				iter = m_ZUnit_List[i].erase(iter);
				continue;
			}
			else
			{
				fy = (*iter)->GetY();
				sortid = (*iter)->GetsortID();
				if(true == (*iter)->Be_in_camera()  )
					m_rendersort[ sortid ].insert( make_pair( fy , (*iter)) );

				++iter;
			}
		}
	}

	for(int i = 0; i < TB_END; ++i)
	{
		iter = m_TBuilding_List[i].begin();
		iter_end = m_TBuilding_List[i].end();

		for( ; iter != iter_end;)
		{			

			if(true == (*iter)->GetDestroy() )
			{
				obj_leave( (*iter)->GetObjID() );
				Safe_Delete(*iter);
				iter = m_TBuilding_List[i].erase(iter);
				continue;
			}
			else
			{
				fy = (*iter)->GetY();
				sortid = (*iter)->GetsortID();
				if(true == (*iter)->Be_in_camera()  )
					m_rendersort[ sortid ].insert( make_pair( fy , (*iter)) );

				++iter;
			}
		}
	}

	for(int i = 0; i < TU_END; ++i)
	{
		iter = m_TUnit_List[i].begin();
		iter_end = m_TUnit_List[i].end();

		for( ; iter != iter_end; )
		{
			if(true == (*iter)->GetDestroy() )
			{
				obj_leave( (*iter)->GetObjID() );
				Safe_Delete(*iter);
				iter = m_TUnit_List[i].erase(iter);
				continue;
			}
			else
			{
				fy = (*iter)->GetY();
				sortid = (*iter)->GetsortID();
				if(true == (*iter)->Be_in_camera()  )
					m_rendersort[ sortid ].insert( make_pair( fy , (*iter)) );

				++iter;
			}
		}
	}

	if(!m_Effect_List.empty())
	{
		list<CObj*>::iterator iter = m_Effect_List.begin();
		list<CObj*>::iterator iter_end = m_Effect_List.end();

		for( ; iter != iter_end; )
		{
			if(true == (*iter)->GetDestroy() )
			{
				Safe_Delete(*iter);
				iter = m_Effect_List.erase(iter);
				continue;
			}
			else
			{
				fy = (*iter)->GetY();
				sortid = (*iter)->GetsortID();
				if(true == (*iter)->Be_in_camera()  )
					m_rendersort[ sortid ].insert( make_pair( fy , (*iter)) );

				++iter;
			}
		}
	}


	if(!m_SelectUI_List.empty())
	{
		list<CUI*>::iterator iter = m_SelectUI_List.begin();
		list<CUI*>::iterator iter_end = m_SelectUI_List.end();

		for( ; iter != iter_end; )
		{
			if(true == (*iter)->GetDestroy() )
			{
				Safe_Delete(*iter);
				iter = m_SelectUI_List.erase(iter);
				continue;
			}
			else
			{
				++iter;
			}
		}
	}
}
void CObjMgr::Render(void)
{
	if(!m_rendersort[SORT_CORPSE].empty())
	{
		multimap<float,CObj*>::iterator iter = m_rendersort[SORT_CORPSE].begin();
		multimap<float,CObj*>::iterator iter_end = m_rendersort[SORT_CORPSE].end();

		for( ; iter != iter_end; ++iter)
			iter->second->Render();

		m_rendersort[SORT_CORPSE].clear();
	}


	if(!m_SelectUI_List.empty())
	{
		list<CUI*>::iterator iter = m_SelectUI_List.begin();
		list<CUI*>::iterator iter_end = m_SelectUI_List.end();

		for( ; iter != iter_end; ++iter)
		{
			(*iter)->Render();
		}
	}

	for(int i = SORT_GROUND; i < SORT_END; ++i)
	{
		if(true == m_rendersort[i].empty())
			continue;

		multimap<float,CObj*>::iterator iter = m_rendersort[i].begin();
		multimap<float,CObj*>::iterator iter_end = m_rendersort[i].end();

		for( ; iter != iter_end; ++iter)
			iter->second->Render();

		m_rendersort[i].clear();
	}
}
void CObjMgr::LoadObj(HANDLE hFile)
{
	DWORD dwbyte;
	D3DXVECTOR2 vPos;
	WCHAR		objname[MIN_STR] = L"";
	CObj*		pObj = NULL;

	while(TRUE)
	{
		ReadFile(hFile , &vPos , sizeof(D3DXVECTOR2) , &dwbyte , NULL);
		ReadFile(hFile , objname , sizeof(WCHAR)*MIN_STR , &dwbyte , NULL);
		if(0 == dwbyte)
			break;

		if(!wcscmp(objname ,L"Startbase"))
			continue;

		if(!wcscmp(objname ,L"Mineral"))
		{
			pObj = new CMineral;
			AddObject(pObj , OBJ_MINERAL);
		}
		else if(!wcscmp(objname ,L"Gas"))
		{
			pObj = new CGasResource;
			AddObject(pObj , OBJ_GAS);
		}
		//else if(!wcscmp(objname ,L"Startbase"))
		//{
		//	pObj = new CStartBase;
		//	AddObject(pObj , OBJ_STARTBASE);
		//}
		pObj->SetPos(vPos.x , vPos.y);
		pObj->Initialize();
	}
}
bool CObjMgr::obj_place(CObj* pobj)
{
	int i = 1;
	for( ; i < MAX_OBJCNT; ++i)
	{
		if(false == m_obj_alive[i])
		{
			pobj->SetObjID(i);
			m_obj_alive[i] = pobj;
			return true;
		}
	}

	return false;
	//return true;
}
void CObjMgr::obj_leave(const int& idx)
{
	m_obj_alive[idx]= NULL;
}
CObj* CObjMgr::obj_alivecheck(const int& idx)
{
	//true는 자리가 차 있다는것
	if(idx <= 0)
		return NULL;

	return m_obj_alive[idx];
}
void CObjMgr::AddObject(CObj* pObj , OBJID eid)
{
	if(OBJ_CORPSE == eid )
		m_ObjList[eid].push_back(pObj);
	else
	{
		if( true == obj_place(pObj) )
			m_ObjList[eid].push_back(pObj);
	}
}
void CObjMgr::AddObject(CObj* pObj , ZERG_BUILDING_ID eid)
{
	if(true == obj_place(pObj))
	m_ZBuilding_List[eid].push_back(pObj);
}
void CObjMgr::AddObject(CObj* pObj , ZERG_UNIT_ID eid)
{
	if(true == obj_place(pObj))
	m_ZUnit_List[eid].push_back(pObj);
}

void CObjMgr::AddObject(CObj* pObj , TERRAN_BUILDING_ID eid)
{
	if(true == obj_place(pObj))
	m_TBuilding_List[eid].push_back(pObj);
}

void CObjMgr::AddObject(CObj* pObj , TERRAN_UNIT_ID eid)
{
	if(true == obj_place(pObj))
	m_TUnit_List[eid].push_back(pObj);
}
void CObjMgr::AddSelect_UI(CUI* pui)
{
	m_SelectUI_List.push_back(pui);
}
void CObjMgr::AddEffect(CObj* peff)
{
	m_Effect_List.push_back(peff);
}

void CObjMgr::DestroyObj(TERRAN_UNIT_ID eid)
{
	CObj* pobj = m_TUnit_List[eid].back();
	pobj->SetDestroy(true);
	//Safe_Delete(pobj);
	//m_TUnit_List[eid].pop_back();
}
void CObjMgr::Release()
{	
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iter_end;
	for(int i = 0; i < OBJ_END; ++i)
	{
		if(m_ObjList[i].empty())
			continue;

		iter = m_ObjList[i].begin();
		iter_end = m_ObjList[i].end();

		for( ; iter != iter_end; ++iter)
			Safe_Delete(*iter);

		m_ObjList[i].clear();
	}



	for(int i = 0; i < ZB_END; ++i)
	{
		if(m_ZBuilding_List[i].empty())
			continue;
		iter = m_ZBuilding_List[i].begin();
		iter_end = m_ZBuilding_List[i].end();

		for( ; iter != iter_end; ++iter)
		{
			Safe_Delete(*iter);
		}
		m_ZBuilding_List[i].clear();
	}

	for(int i = 0; i < ZU_END; ++i)
	{
		if(m_ZUnit_List[i].empty())
			continue;
		iter = m_ZUnit_List[i].begin();
		iter_end = m_ZUnit_List[i].end();

		for( ; iter != iter_end; ++iter)
		{
			Safe_Delete(*iter);
		}
		m_ZUnit_List[i].clear();
	}


	for(int i = 0; i < TU_END; ++i)
	{
		if(m_TUnit_List[i].empty())
			continue;
		iter = m_TUnit_List[i].begin();
		iter_end = m_TUnit_List[i].end();

		for( ; iter != iter_end; ++iter)
		{
			Safe_Delete(*iter);
		}
		m_TUnit_List[i].clear();
	}

	for(int i = 0; i < TB_END; ++i)
	{
		if(m_TBuilding_List[i].empty())
			continue;
		iter = m_TBuilding_List[i].begin();
		iter_end = m_TBuilding_List[i].end();

		for( ; iter != iter_end; ++iter)
		{
			Safe_Delete(*iter);
		}
		m_TBuilding_List[i].clear();
	}


	for(int i = 0; i < SORT_END; ++i)
	{
		if(true == m_rendersort[i].empty())
			continue;

		m_rendersort[i].clear();
	}

	list<CUI*>::iterator iter_UI = m_SelectUI_List.begin();
	list<CUI*>::iterator iterUI_end = m_SelectUI_List.end();

	for( ; iter_UI != iterUI_end; ++iter_UI)
		Safe_Delete((*iter_UI));

	m_SelectUI_List.clear();



	list<CObj*>::iterator iter_eff = m_Effect_List.begin();
	list<CObj*>::iterator iter_eff_end = m_Effect_List.end();

	for( ; iter_eff != iter_eff_end; ++iter_eff)
		Safe_Delete(*iter_eff);

	m_Effect_List.clear();
}





