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

#include "MyMath.h"


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

	if(!m_CorpseList.empty())
	{
		iter = m_CorpseList.begin();
		iter_end = m_CorpseList.end();

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
				obj_leave( (*iter)->GetObjNumber() );

				Safe_Delete(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
			{
				fy = (*iter)->GetY();
				sortid = (*iter)->GetsortID();
				if(true == (*iter)->Be_in_camera()  )
				{
					if(SORT_AIR == sortid)
						m_air_rendersort.push_back( (*iter) );
					else
						m_rendersort[ sortid ].insert( make_pair( fy , (*iter)) );
				}

				++iter;
			}
		}
	}
	if(!m_CorpseList.empty())
	{
		list<CObj*>::iterator iter = m_CorpseList.begin();
		list<CObj*>::iterator iter_end = m_CorpseList.end();

		for( ; iter != iter_end; )
		{
			if(true == (*iter)->GetDestroy() )
			{
				Safe_Delete(*iter);
				iter = m_CorpseList.erase(iter);
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


	if(!m_air_rendersort.empty())
	{
		list<CObj*>::iterator iter = m_air_rendersort.begin();
		list<CObj*>::iterator iter_end = m_air_rendersort.end();

		for( ; iter != iter_end; ++iter)
		{
			(*iter)->Render();
		}
		m_air_rendersort.clear();
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
			pObj->SetPos(vPos);
			pObj->Initialize();
			AddObject(pObj , OBJ_MINERAL);
			
		}
		else if(!wcscmp(objname ,L"Gas"))
		{
			pObj = new CGasResource;
			pObj->SetPos(vPos);
			pObj->Initialize();
			AddObject(pObj , OBJ_GAS);
		}
		//else if(!wcscmp(objname ,L"Startbase"))
		//{
		//	pObj = new CStartBase;
		//	AddObject(pObj , OBJ_STARTBASE);
		//}

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
	if(OBJ_FRAGMENT == eid)
	{
		pObj->SetObjName(eid);
		m_ObjList[eid].push_back(pObj);
	}
	else
	{
		if( true == obj_place(pObj) )
		{
			pObj->SetObjName(eid);
			m_ObjList[eid].push_back(pObj);
		}
	}
}
void CObjMgr::AddSelect_UI(CUI* pui)
{
	m_SelectUI_List.push_back(pui);
}
void CObjMgr::AddEffect(CObj* peff)
{
	m_Effect_List.push_back(peff);
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


	iter = m_CorpseList.begin();
	iter_end = m_CorpseList.end();

	for( ; iter != iter_end; ++iter)
		Safe_Delete(*iter);
	m_CorpseList.clear();


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

void CObjMgr::AddCorpse(CObj* pObj)
{
	m_CorpseList.push_back(pObj);
}

CObj* CObjMgr::GetCoreBuilding(CObj* pself)
{
	float fminval = 10000000;
	CObj* pbuilding = NULL;

	if(!m_ObjList[OBJ_COMMAND].empty())
	{
		list<CObj*>::iterator iter = m_ObjList[OBJ_COMMAND].begin();
		list<CObj*>::iterator iter_end = m_ObjList[OBJ_COMMAND].end();

		for( ; iter != iter_end; ++iter)
		{
			if(pself->GetTeamNumber() == (*iter)->GetTeamNumber())
			{
				if( BUILD == (*iter)->GetUnitinfo().estate)
					continue;

				float idistance = CMyMath::pos_distance(pself->GetPos() , (*iter)->GetPos() );

				if( fminval > idistance )
				{
					fminval= idistance;
					pbuilding = (*iter);
				}
			}
		}
	}

	return pbuilding;
}





