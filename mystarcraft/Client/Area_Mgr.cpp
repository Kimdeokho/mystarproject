#include "StdAfx.h"
#include "Area_Mgr.h"

IMPLEMENT_SINGLETON(CArea_Mgr)
CArea_Mgr::CArea_Mgr(void)
{
}

CArea_Mgr::~CArea_Mgr(void)
{
}

void CArea_Mgr::SetObj_Area64(const int& curidx , const int& oldidx , CObj* pobj)
{
	if(!m_Area64[oldidx].empty())
	{
		list<CObj*>::iterator iter = m_Area64[oldidx].begin();
		list<CObj*>::iterator iter_end = m_Area64[oldidx].end();

		for( ; iter != iter_end;)
		{
			if( (*iter) == pobj )
			{
				iter = m_Area64[oldidx].erase(iter);
			}
			else
				++iter;
		}
	}

	m_Area64[curidx].push_back(pobj);
}
void CArea_Mgr::SetObj_Area512(const int& curidx , const int& oldidx , CObj* pobj)
{
	if(!m_Area512[oldidx].empty())
	{
		list<CObj*>::iterator iter = m_Area512[oldidx].begin();
		list<CObj*>::iterator iter_end = m_Area512[oldidx].end();

		for( ; iter != iter_end;)
		{
			if( (*iter) == pobj )
			{
				iter = m_Area512[oldidx].erase(iter);
			}
			else
				++iter;
		}
	}

	m_Area512[curidx].push_back(pobj);
}
