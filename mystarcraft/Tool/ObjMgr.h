#pragma once

#include "Include.h"

class CObj;
class CObjMgr
{
	DECLARE_SINGLETON(CObjMgr)
private:
	list<CObj*>	m_ObjList[OBJ_END];
public:
	HRESULT AddObject(CObj* pObj , OBJID	eId);
	void	DeleteObj(const CPoint& _pt);
public:
	void Render(void);
	void Release(void);
public:
	CObjMgr(void);
	~CObjMgr(void);
};
