#include "stdafx.h"
#include "UnitMgr.h"

#include "Obj.h"
#include "TileManager.h"
#include "Unit.h"

#include "FontMgr.h"
#include "MyMath.h"
#include "MouseMgr.h"
#include "ScrollMgr.h"

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

	m_vUnitcenterpt		= D3DXVECTOR2(0,0);
	m_vGoalPos			= D3DXVECTOR2(0,0);
	m_vparallel_travel  = D3DXVECTOR2(0,0);
	m_magicbox_unitcnt	= 0;
	m_vGoalIdx			= 0;
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
void CUnitMgr::clear_destroy_unitlist(CObj* pobj)
{
	if(!m_curunitList.empty())
	{
		list<CObj*>::iterator iter = m_curunitList.begin();
		list<CObj*>::iterator iter_end = m_curunitList.end();

		for( ; iter != iter_end; )
		{
			if(pobj == (*iter))
			{
				iter = m_curunitList.erase(iter);
				break;
			}
			else
				++iter;
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
void CUnitMgr::Calculate_UnitCenterPt(void)
{
	list<CObj*>::iterator iter = m_curunitList.begin();
	list<CObj*>::iterator iter_end = m_curunitList.end();

	map<int , CObj*> map_unitidxsort;


	int idx = 0;
	
	for( ; iter != iter_end; ++iter)
	{
		idx = (*iter)->Getcuridx(32);
		map_unitidxsort.insert(map<int , CObj*>::value_type(idx , (*iter)) );		
	}


	map<int , CObj*>::iterator map_iter = map_unitidxsort.begin();
	CObj* pobj = map_iter->second;
	m_vUnitcenterpt = D3DXVECTOR2(0,0);
	m_magicbox_unitcnt = 0;

	iter	 = m_curunitList.begin();
	iter_end = m_curunitList.end();

	for( ; iter != iter_end; ++iter) 
	{
		if(CMyMath::pos_distance(pobj->GetPos() , (*iter)->GetPos()) < 300*300)
		{
			++m_magicbox_unitcnt;
			m_vUnitcenterpt += (*iter)->GetPos();
			(*iter)->SetMagicBox(true);
		}
		else
			(*iter)->SetMagicBox(false);

	}

	m_vUnitcenterpt /= float(m_magicbox_unitcnt);
	m_vGoalPos = CMouseMgr::GetAddScrollvMousePt();
	m_vGoalIdx = CMyMath::Pos_to_index(m_vGoalPos , 32);

	m_vparallel_travel = m_vGoalPos - m_vUnitcenterpt;

	printf("unit_goal %f , %f\n" , m_vGoalPos.x , m_vGoalPos.y);
	printf("unit_index %d\n" , m_vGoalIdx);
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
			if(BUILDING == (*iter)->GetCategory())
				return true;
		}
	}

	return false;
}
D3DXVECTOR2 CUnitMgr::GetParallel_travel(void)
{
	return m_vparallel_travel;
}
D3DXVECTOR2 CUnitMgr::GetUnitGoalPos(void)
{
	return m_vGoalPos;
}
D3DXVECTOR2 CUnitMgr::GetUnitCentterPt(void)
{
	return m_vUnitcenterpt;
}
int CUnitMgr::GetGoalidx(void)
{
	return m_vGoalIdx;
}

