#include "StdAfx.h"
#include "ObjMgr.h"
#include "Obj.h"

IMPLEMENT_SINGLETON(CObjMgr)
CObjMgr::CObjMgr(void)
{
}

CObjMgr::~CObjMgr(void)
{
	Release();
}

HRESULT CObjMgr::AddObject(CObj* pObj , OBJID	eId)
{
	if(eId < OBJ_END)
	{
		m_ObjList[eId].push_back(pObj);
		return S_OK;
	}
	else
		return E_FAIL;
}
void CObjMgr::DeleteObj(const CPoint& _pt)
{
	for(int i = 0; i < OBJ_END; ++i)
	{
		list<CObj*>::iterator iter = m_ObjList[i].begin();
		list<CObj*>::iterator iter_end = m_ObjList[i].end();

		for( ; iter != iter_end; )
		{
			if((*iter)->PtCollCheck(_pt) )
			{
				(*iter)->Restore_TIleOption();
				Safe_Delete( (*iter) );
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}
}
void CObjMgr::Render(void)
{	
	for(int i = 0; i < OBJ_END; ++i)
	{
		list<CObj*>::iterator iter = m_ObjList[i].begin();
		list<CObj*>::iterator iter_end = m_ObjList[i].end();

		for( ; iter != iter_end; ++iter)
		{
			(*iter)->Render();
		}
	}
}

void CObjMgr::Release(void)
{
	for(int i = 0; i < OBJ_END; ++i)
	{
		list<CObj*>::iterator iter = m_ObjList[i].begin();
		list<CObj*>::iterator iter_end = m_ObjList[i].end();

		for( ; iter != iter_end; ++iter)
		{
			Safe_Delete( (*iter) );
		}
		m_ObjList[i].clear();
	}
}
