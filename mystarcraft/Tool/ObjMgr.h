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
	void	SaveObj(HANDLE h);
	void	LoadObj(HANDLE h);
public:
	void Render(void);
	void MinimapRender(const float fratioX , const float fratioY);
	void Release(void);
public:
	CObjMgr(void);
	~CObjMgr(void);
};
