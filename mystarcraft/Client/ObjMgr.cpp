#include "StdAfx.h"
#include "ObjMgr.h"

#include "Obj.h"
#include "Mineral.h"
#include "GasResource.h"

#include "Zerg_Building.h"
#include "ObjPoolMgr.h"
#include "TileManager.h"
#include "Area_Mgr.h"
#include "Debug_Mgr.h"
#include "Session_Mgr.h"
#include "Ingame_UIMgr.h"
#include "FontMgr.h"
#include "ScrollMgr.h"

#include "UI_Resource.h"

#include "UI.h"
#include "UnitMgr.h"
#include "Effect.h"

#include "MyMath.h"
#include "Nuclear_part.h"

#include "Larva.h"
#include "Drone.h"
#include "Overload.h"
#include "SCv.h"
#include "Hatchery.h"
#include "Comandcenter.h"


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

	if(!m_bullet_list.empty())
	{
		iter = m_bullet_list.begin();
		iter_end = m_bullet_list.end();

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
	TEAM_NUMBER eteam = CSession_Mgr::GetInstance()->GetTeamNumber();

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


				if(true == (*iter)->Be_in_camera())
				{ 
					if(CDebug_Mgr::m_dbglist[CDebug_Mgr::DBG_FOG])
					{
						if(SORT_AIR == sortid)
							m_air_rendersort.push_back( (*iter) );
						else if(SORT_AIR_EFF == sortid)
							m_aireff_renderlist.push_back( (*iter) );
						else if(SORT_GROUND_EFF == sortid)
							m_groundeff_renderlist.push_back( (*iter) );
						else
							m_rendersort[ sortid ].insert( make_pair( fy , (*iter)) );
					}
					else
					{
						if(FOG_ALPHA == CTileManager::GetInstance()->GetFogLight((*iter)->Getcuridx(32), eteam))
						{
							if( (*iter)->GetUnitinfo().detect[eteam] > 0)
							{
								if(SORT_AIR == sortid)
									m_air_rendersort.push_back( (*iter) );
								else if(SORT_AIR_EFF == sortid)
									m_aireff_renderlist.push_back( (*iter) );
								else if(SORT_GROUND_EFF == sortid)
									m_groundeff_renderlist.push_back( (*iter) );
								else
									m_rendersort[ sortid ].insert( make_pair( fy , (*iter)) );
							}
						}
						else
						{
							if(CATEGORY_BUILDING == (*iter)->GetCategory() ||
								CATEGORY_RESOURCE == (*iter)->GetCategory())
							{
								m_rendersort[ sortid ].insert( make_pair( fy , (*iter)) );
							}
						}
					}
				}
				else
				{

					if( FOG_ALPHA != CTileManager::GetInstance()->GetFogLight((*iter)->Getcuridx(32), eteam) ||
						CATEGORY_BUILDING == (*iter)->GetCategory())
					{
						m_rendersort[ sortid ].insert( make_pair( fy , (*iter)) );
					}
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

				int idx32 = CMyMath::Pos_to_index((*iter)->GetPos(), 32);
				if(true == (*iter)->Be_in_camera())
				{
					if(CDebug_Mgr::m_dbglist[CDebug_Mgr::DBG_FOG])
						m_rendersort[ sortid ].insert( make_pair( fy , (*iter)) );
					else
					{
						if(FOG_ALPHA == CTileManager::GetInstance()->GetFogLight(idx32, eteam))
							m_rendersort[ sortid ].insert( make_pair( fy , (*iter)) );
					}
				}

				++iter;
			}
		}
	}

	if(!m_bullet_list.empty())
	{
		list<CObj*>::iterator iter = m_bullet_list.begin();
		list<CObj*>::iterator iter_end = m_bullet_list.end();

		for( ; iter != iter_end; )
		{
			if(true == (*iter)->GetDestroy() )
			{
				obj_leave( (*iter)->GetObjNumber() );

				Safe_Delete(*iter);
				iter = m_bullet_list.erase(iter);
			}
			else
			{
				fy = (*iter)->GetY();
				sortid = (*iter)->GetsortID();

				if(true == (*iter)->Be_in_camera())
				{ 
					if(CDebug_Mgr::m_dbglist[CDebug_Mgr::DBG_FOG])
					{
						if(SORT_AIR == sortid)
							m_air_rendersort.push_back( (*iter) );
						else if(SORT_AIR_EFF == sortid)
							m_aireff_renderlist.push_back( (*iter) );
						else if(SORT_GROUND_EFF == sortid)
							m_groundeff_renderlist.push_back( (*iter) );
						else
							m_rendersort[ sortid ].insert( make_pair( fy , (*iter)) );
					}
					else
					{
						if(FOG_ALPHA == CTileManager::GetInstance()->GetFogLight((*iter)->Getcuridx(32), eteam))
						{
							if( (*iter)->GetUnitinfo().detect[eteam] > 0)
							{
								if(SORT_AIR == sortid)
									m_air_rendersort.push_back( (*iter) );
								else if(SORT_AIR_EFF == sortid)
									m_aireff_renderlist.push_back( (*iter) );
								else if(SORT_GROUND_EFF == sortid)
									m_groundeff_renderlist.push_back( (*iter) );
								else
									m_rendersort[ sortid ].insert( make_pair( fy , (*iter)) );
							}
						}
						else
						{
							if(CATEGORY_BUILDING == (*iter)->GetCategory() ||
								CATEGORY_RESOURCE == (*iter)->GetCategory())
							{
								m_rendersort[ sortid ].insert( make_pair( fy , (*iter)) );
							}
						}
					}
				}

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
				int idx32 = CMyMath::Pos_to_index((*iter)->GetPos(), 32);

				if(true == (*iter)->Be_in_camera())
				{
					if(CDebug_Mgr::m_dbglist[CDebug_Mgr::DBG_FOG])
					{
						if(SORT_GROUND_EFF == sortid)
							m_groundeff_renderlist.push_back( (*iter) );
						else if(SORT_AIR_EFF == sortid)
							m_aireff_renderlist.push_back( (*iter) );
						else if(SORT_AIR == sortid)
							m_air_rendersort.push_back( (*iter) );
						else
							m_rendersort[ sortid ].insert( make_pair( fy , (*iter)) );
					}
					else
					{
						if(FOG_ALPHA == CTileManager::GetInstance()->GetFogLight(idx32, eteam))
						{
							if(SORT_GROUND_EFF == sortid)
								m_groundeff_renderlist.push_back( (*iter) );
							else if(SORT_AIR_EFF == sortid)
								m_aireff_renderlist.push_back( (*iter) );
							else if(SORT_AIR == sortid)
								m_air_rendersort.push_back( (*iter) );
							else
								m_rendersort[ sortid ].insert( make_pair( fy , (*iter)) );
						}
					}
				}

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

	if(!m_groundeff_renderlist.empty())
	{
		list<CObj*>::iterator iter = m_groundeff_renderlist.begin();
		list<CObj*>::iterator iter_end = m_groundeff_renderlist.end();

		for( ; iter != iter_end; ++iter)
		{
			(*iter)->Render();
		}
		m_groundeff_renderlist.clear();
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

	if(!m_aireff_renderlist.empty())
	{
		list<CObj*>::iterator iter = m_aireff_renderlist.begin();
		list<CObj*>::iterator iter_end = m_aireff_renderlist.end();

		for( ; iter != iter_end; ++iter)
		{
			(*iter)->Render();
		}
		m_aireff_renderlist.clear();
	}
}
void CObjMgr::LoadObj(HANDLE hFile)
{
	DWORD dwbyte;
	D3DXVECTOR2 vPos;
	WCHAR		objname[MIN_STR] = L"";
	CObj*		pObj = NULL;

	vector<D3DXVECTOR2>	base_list;
	while(TRUE)
	{
		ReadFile(hFile , &vPos , sizeof(D3DXVECTOR2) , &dwbyte , NULL);
		ReadFile(hFile , objname , sizeof(WCHAR)*MIN_STR , &dwbyte , NULL);
		if(0 == dwbyte)
			break;

		if(!wcscmp(objname ,L"Startbase"))
		{
			base_list.push_back(vPos);
		}

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
	}

}
bool CObjMgr::obj_place(CObj* pobj)
{
	for(int i = 1 ; i < MAX_OBJCNT; ++i)
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
		if(OBJ_BULLET == eid)
		{
			pObj->SetObjName(eid);
			m_bullet_list.push_back(pObj);
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
}
void CObjMgr::AddEffect(CObj* peff)
{
	m_Effect_List.push_back(peff);
}
void CObjMgr::AddCorpse(CObj* pObj)
{
	pObj->Update();
	m_CorpseList.push_back(pObj);
}

CObj* CObjMgr::GetCoreBuilding(CObj* pself)
{
	float fminval = FLT_MAX;
	CObj* pbuilding = NULL;


	if(!m_ObjList[OBJ_COMMAND].empty())
	{
		list<CObj*>::iterator iter = m_ObjList[OBJ_COMMAND].begin();
		list<CObj*>::iterator iter_end = m_ObjList[OBJ_COMMAND].end();

		for( ; iter != iter_end; ++iter)
		{
			if(pself->GetTeamNumber() == (*iter)->GetTeamNumber())
			{
				if( BUILD == (*iter)->GetUnitinfo().state)
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

	if(!m_ObjList[OBJ_HATCERY].empty())
	{
		list<CObj*>::iterator iter = m_ObjList[OBJ_HATCERY].begin();
		list<CObj*>::iterator iter_end = m_ObjList[OBJ_HATCERY].end();

		for( ; iter != iter_end; ++iter)
		{
			if(pself->GetTeamNumber() == (*iter)->GetTeamNumber())
			{
				if( BUILD == (*iter)->GetUnitinfo().state)
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

void CObjMgr::Nuclear_launch(void)
{
	if(!m_ObjList[OBJ_NC_PART].empty())
	{
		list<CObj*>::iterator iter = m_ObjList[OBJ_NC_PART].begin();
		list<CObj*>::iterator iter_end = m_ObjList[OBJ_NC_PART].end();

		for( ; iter != iter_end; ++iter)
		{
			if( ((CNuclear_part*)(*iter))->GetNuclear_cnt() >= 1)
			{
				((CNuclear_part*)(*iter))->SetNuclear_cnt(-1);
				break;
			}
		}
	}
}

void CObjMgr::Place_Terran(D3DXVECTOR2 vpos , TEAM_NUMBER eteam)
{
	CObj* pobj = new CComandcenter(0.f);
	vpos.x -= 48;
	vpos.y -= 32;
	pobj->SetPos(vpos);
	pobj->SetTeamNumber(eteam);
	pobj->Initialize();
	AddObject(pobj , OBJ_COMMAND);

	MYRECT<float> build_vtx = pobj->GetVertex();
	MYRECT<float> unit_vtx;
	D3DXVECTOR2 collocate_pos;
	CObj*	pscv = NULL;

	vpos = pobj->GetPos();
	for(int i = 0; i < 4; ++i)
	{		
		pscv = new CSCV;
		collocate_pos.x = vpos.x - (46 - 11.5f) +	23.f*i;
		collocate_pos.y = vpos.y + build_vtx.bottom + 11.5f;

		pscv->SetPos(collocate_pos);		
		pscv->SetTeamNumber(eteam);
		pscv->Initialize();
		AddObject(pscv , OBJ_SCV);
		CIngame_UIMgr::GetInstance()->GetResource_UI()->SetPopvalue(1 , eteam);
	}
}
void CObjMgr::Place_Zerg(D3DXVECTOR2 vpos , TEAM_NUMBER eteam)
{
	CObj* pobj = new CHatchery(0.f);
	AddObject(pobj , OBJ_HATCERY);

	vpos.x -= 48;
	vpos.y -= 32;
	pobj->SetPos(vpos);
	pobj->SetTeamNumber(eteam);
	pobj->Initialize();
	


	MYRECT<float> build_vtx = pobj->GetVertex();
	MYRECT<float> unit_vtx;
	D3DXVECTOR2 collocate_pos;
	vpos = pobj->GetPos();

	CObj*	pdrone = NULL;

	for(int i = 0; i < 4; ++i)
	{		
		pdrone = new CDrone;
		AddObject(pdrone , OBJ_DRONE);

		collocate_pos.x = vpos.x - (46 - 11.5f) + 23.f*i;
		collocate_pos.y = vpos.y + build_vtx.bottom + 11.5f;

		pdrone->SetPos(collocate_pos);		
		pdrone->SetTeamNumber(eteam);
		pdrone->Initialize();
		CIngame_UIMgr::GetInstance()->GetResource_UI()->SetPopvalue(1 , eteam);
	}

	CObj*	poverload = new COverload;
	AddObject(poverload , OBJ_OVERLOAD);

	vpos.y -= 30;
	poverload->SetPos(vpos);
	poverload->SetTeamNumber(eteam);
	poverload->Initialize();
	
}
CObj* CObjMgr::GetObj(const USHORT objnum)
{
	return m_obj_alive[objnum];
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


	iter = m_bullet_list.begin();
	iter_end = m_bullet_list.end();

	for( ; iter != iter_end; ++iter)
		Safe_Delete(*iter);
	m_bullet_list.clear();


	for(int i = 0; i < SORT_END; ++i)
	{
		if(true == m_rendersort[i].empty())
			continue;

		m_rendersort[i].clear();
	}

	list<CObj*>::iterator iter_eff = m_Effect_List.begin();
	list<CObj*>::iterator iter_eff_end = m_Effect_List.end();

	for( ; iter_eff != iter_eff_end; ++iter_eff)
		Safe_Delete(*iter_eff);

	m_Effect_List.clear();



}
