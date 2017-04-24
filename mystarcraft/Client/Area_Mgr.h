#pragma once

#include "Include.h"

class CObj;
class CArea_Mgr
{
	DECLARE_SINGLETON(CArea_Mgr)
private:
	list<CObj*>	m_Area64[64*64];
	list<CObj*>	m_Area512[8*8];
public:
	void SetObj_Area64(const int& curidx , const int& oldidx , CObj* pobj);
	void SetObj_Area512(const int& curidx , const int& oldidx , CObj* pobj);
public:
	CArea_Mgr(void);
	~CArea_Mgr(void);
};
