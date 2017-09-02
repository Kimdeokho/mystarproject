#include "stdafx.h"
#include "UnitMgr.h"

#include "Obj.h"
#include "TileManager.h"
#include "Unit.h"

#include "FontMgr.h"
IMPLEMENT_SINGLETON(CUnitMgr)
CUnitMgr::CUnitMgr(void)
{

}

CUnitMgr::~CUnitMgr(void)
{

}

void CUnitMgr::Initialize(void)
{
	for(int i = 0; i < 10; ++i)
	{
		m_troops[i].reserve(12);
	}
}

void CUnitMgr::SetUnit(CObj* pobj)
{
	m_curunitList.push_back(pobj);
}

void CUnitMgr::discharge_unit(void)
{
	list<CObj*>::iterator iter = m_curunitList.begin();
	list<CObj*>::iterator iter_end = m_curunitList.end();

	for( ; iter != iter_end; ++iter)
		(*iter)->SetSelect(false);

	m_curunitList.clear();
}
void CUnitMgr::Intputkey_reaction(const int& firstkey , const int& secondkey)
{
	if(!m_curunitList.empty())
	{
		list<CObj*>::iterator iter = m_curunitList.begin();
		list<CObj*>::iterator iter_end = m_curunitList.end();

		for( ; iter != iter_end; ++iter)
		{
			(*iter)->Inputkey_reaction(firstkey , secondkey);
		}
	}
}
void CUnitMgr::Intputkey_reaction(const int& nkey)
{
	if(!m_curunitList.empty())
	{
		list<CObj*>::iterator iter = m_curunitList.begin();
		list<CObj*>::iterator iter_end = m_curunitList.end();

		for( ; iter != iter_end; ++iter)
		{
			(*iter)->Inputkey_reaction(nkey);
		}
	}
}
void CUnitMgr::path_relay(const int& goalidx)
{
	//flowfield 경로를 선택된 유닛들에게 전달한다

	//if(!m_curunitList.empty())
	//{
	//	int maxloop = SQ_TILECNTX*SQ_TILECNTY;

	//	CTileManager* tilemgr = CTileManager::GetInstance();
	//	//FLOW_NODE** flowfield = tilemgr->Get_flowfield_node();
	//	short* flowfield = tilemgr->Get_flowfield_node();
	//	CUnit* pobj = NULL;
	//	short* setflowfield;

	//	list<CObj*>::iterator iter = m_curunitList.begin();
	//	list<CObj*>::iterator iter_end = m_curunitList.end();

	//	for( ; iter != iter_end; ++iter)
	//	{
	//		pobj = ((CUnit*)(*iter));

	//		pobj->SetState(MOVE);
	//		if(MOVE_AIR == pobj->GetType())//공중이나 건물
	//			continue;
	//		
	//		setflowfield = pobj->getflowfield();

	//		memcpy(setflowfield , flowfield , sizeof(short)*maxloop);
			//memcpy(setflowfield , CTileManager::GetInstance()->Get_flowfield_node() , sizeof(short)*maxloop);

	//		pobj->make_flowfieldpath(goalidx);
	//	}
	//}
}

bool CUnitMgr::GetUnitlistempty(void)
{
	if(m_curunitList.empty())
		return true;
	else
	{
		list<CObj*>::iterator iter = m_curunitList.begin();
		list<CObj*>::iterator iter_end = m_curunitList.end();

		for( ; iter != iter_end; ++iter)
		{
			if(BUILDING == (*iter)->GetKategorie())
				return true;
		}
	}

	return false;
}

