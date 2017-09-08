#include "StdAfx.h"
#include "ObjMgr.h"

#include "Obj.h"
#include "Mineral.h"
#include "GasResource.h"

#include "Zerg_Building.h"
#include "ObjPoolMgr.h"
#include "Area_Mgr.h"

IMPLEMENT_SINGLETON(CObjMgr)
CObjMgr::CObjMgr(void)
{
}

CObjMgr::~CObjMgr(void)
{
	Release();
}
void CObjMgr::Update(void)
{

	float fy = 0.f;
	SORT_ID sortid = SORT_END;

	for(int i = 0; i < OBJ_END; ++i)
	{
		list<CObj*>::iterator iter = m_ObjList[i].begin();
		list<CObj*>::iterator iter_end = m_ObjList[i].end();

		for( ; iter != iter_end; ++iter)
		{
			(*iter)->Update();

			fy = (*iter)->GetY();
			sortid = (*iter)->GetsortID();

			if(true == (*iter)->Be_in_camera() )
				m_rendersort[ sortid ].insert( make_pair( fy , (*iter) ) );
		}
	}


	for(int i = 0; i < ZB_END; ++i)
	{
		list<CObj*>::iterator iter = m_ZBuilding_List[i].begin();
		list<CObj*>::iterator iter_end = m_ZBuilding_List[i].end();

		for( ; iter != iter_end;)
		{
			(*iter)->Update();

			if(true == (*iter)->GetDestroy() )
			{
				Safe_Delete(*iter);
				iter = m_ZBuilding_List[i].erase(iter);
				continue;
			}
			else
			{
				fy = (*iter)->GetY();
				sortid = (*iter)->GetsortID();

				if(true == (*iter)->Be_in_camera() )
					m_rendersort[ sortid ].insert( make_pair( fy , (*iter) ) );
				++iter;
			}
		}
	}

	for(int i = 0; i < ZU_END; ++i)
	{
		list<CObj*>::iterator iter = m_ZUnit_List[i].begin();
		list<CObj*>::iterator iter_end = m_ZUnit_List[i].end();

		for( ; iter != iter_end; )
		{
			(*iter)->Update();

			if(true == (*iter)->GetDestroy() )
			{
				Safe_Delete(*iter);
				iter = m_ZUnit_List[i].erase(iter);
				continue;
			}
			else
			{
				fy = (*iter)->GetY();
				sortid = (*iter)->GetsortID();

				if(true == (*iter)->Be_in_camera() )
					m_rendersort[ sortid ].insert( make_pair( fy , (*iter) ) );

				++iter;
			}
		}
	}





	for(int i = 0; i < TB_END; ++i)
	{
		list<CObj*>::iterator iter = m_TBuilding_List[i].begin();
		list<CObj*>::iterator iter_end = m_TBuilding_List[i].end();

		for( ; iter != iter_end;)
		{
			(*iter)->Update();

			if(true == (*iter)->GetDestroy() )
			{
				Safe_Delete(*iter);
				iter = m_TBuilding_List[i].erase(iter);
				continue;
			}
			else
			{
				fy = (*iter)->GetY();
				sortid = (*iter)->GetsortID();

				if(true == (*iter)->Be_in_camera() )
					m_rendersort[ sortid ].insert( make_pair( fy , (*iter) ) );
				++iter;
			}
		}
	}

	for(int i = 0; i < TU_END; ++i)
	{
		list<CObj*>::iterator iter = m_TUnit_List[i].begin();
		list<CObj*>::iterator iter_end = m_TUnit_List[i].end();

		for( ; iter != iter_end; )
		{
			(*iter)->Update();

			if(true == (*iter)->GetDestroy() )
			{
				Safe_Delete(*iter);
				iter = m_TUnit_List[i].erase(iter);
				continue;
			}
			else
			{
				fy = (*iter)->GetY();
				sortid = (*iter)->GetsortID();

				if(true == (*iter)->Be_in_camera() )
					m_rendersort[ sortid ].insert( make_pair( fy , (*iter) ) );

				++iter;
			}
		}
	}
}

void CObjMgr::Render(void)
{
	for(int i = 0; i < SORT_END; ++i)
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

void CObjMgr::AddObject(CObj* pObj , OBJID eid)
{
	m_ObjList[eid].push_back(pObj);
}
void CObjMgr::AddObject(CObj* pObj , ZERG_BUILDING_ID eid)
{
	m_ZBuilding_List[eid].push_back(pObj);
}
void CObjMgr::AddObject(CObj* pObj , ZERG_UNIT_ID eid)
{
	m_ZUnit_List[eid].push_back(pObj);
}

void CObjMgr::AddObject(CObj* pObj , TERRAN_BUILDING_ID eid)
{
	m_TBuilding_List[eid].push_back(pObj);
}

void CObjMgr::AddObject(CObj* pObj , TERRAN_UNIT_ID eid)
{
	m_TUnit_List[eid].push_back(pObj);
}

void CObjMgr::DestroyObj(ZERG_BUILDING_ID eid)
{
	CObj* pObj = m_ZBuilding_List[eid].back();
	((CZerg_Building*)pObj)->SetDestroy();
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



	for(int i = 0; i < SORT_END; ++i)
	{
		if(true == m_rendersort[i].empty())
			continue;

		m_rendersort[i].clear();
	}
}