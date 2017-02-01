#include "StdAfx.h"
#include "ObjMgr.h"

#include "Obj.h"
#include "Mineral.h"
#include "GasResource.h"

IMPLEMENT_SINGLETON(CObjMgr)
CObjMgr::CObjMgr(void)
{
}

CObjMgr::~CObjMgr(void)
{
	Release();
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

void CObjMgr::Release()
{	
	for(int i = 0; i < OBJ_END; ++i)
	{
		list<CObj*>::iterator iter = m_ObjList[i].begin();
		list<CObj*>::iterator iter_end = m_ObjList[i].end();

		for( ; iter != iter_end; ++iter)
			Safe_Delete(*iter);

		m_ObjList[i].clear();
	}
}
