#include "StdAfx.h"
#include "ObjMgr.h"
#include "Obj.h"

#include "Mineral.h"
#include "GasResource.h"
#include "StartBase.h"

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
			(*iter)->Render();
	}
}
void CObjMgr::MinimapRender(const float fratioX , const float fratioY)
{
	for(int i = 0; i < OBJ_END; ++i)
	{
		list<CObj*>::iterator iter = m_ObjList[i].begin();
		list<CObj*>::iterator iter_end = m_ObjList[i].end();

		for( ; iter != iter_end; ++iter)
			(*iter)->MinmapRender(fratioX , fratioY);
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
void CObjMgr::Save_StartBaseCnt(HANDLE h)
{
	list<CObj*>::iterator iter = m_ObjList[OBJ_STARTBASE].begin();
	list<CObj*>::iterator iter_end = m_ObjList[OBJ_STARTBASE].end();

	int basecnt = 0;
	for( ; iter != iter_end; ++iter)
		++basecnt;


	DWORD dwbyte;
	WriteFile(h , &basecnt , sizeof(int) , &dwbyte , NULL);
}
void CObjMgr::SaveObj(HANDLE h)
{
	for(int i = 0; i < OBJ_END; ++i)
	{
		list<CObj*>::iterator iter = m_ObjList[i].begin();
		list<CObj*>::iterator iter_end = m_ObjList[i].end();

		for( ; iter != iter_end; ++iter)
			(*iter)->SaveInfo(h);
	}
}
void CObjMgr::LoadObj(HANDLE h)
{
	Release();

	DWORD dwbyte;
	D3DXVECTOR2 vPos;
	WCHAR		objname[MIN_STR] = L"";
	CObj*		pObj = NULL;

	while(TRUE)
	{
		ReadFile(h , &vPos , sizeof(D3DXVECTOR2) , &dwbyte , NULL);
		ReadFile(h , objname , sizeof(WCHAR)*MIN_STR , &dwbyte , NULL);
		if(0 == dwbyte)
			break;

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
		else if(!wcscmp(objname ,L"Startbase"))
		{
			pObj = new CStartBase;
			AddObject(pObj , OBJ_STARTBASE);
		}
		pObj->SetPos(vPos.x , vPos.y);
		pObj->Initialize();
	}
}