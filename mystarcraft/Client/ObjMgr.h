#pragma once

#include "Include.h"

class CObj;
class CObjMgr
{
public:
	DECLARE_SINGLETON(CObjMgr)
private:
	list<CObj*>		m_ObjList[OBJ_END];
public:
	void Release();
public:
	void AddObject(CObj* pObj , OBJID eid);
public:
	void LoadObj(HANDLE hFile);
private:
	CObjMgr(void);
public:
	~CObjMgr(void);
};
