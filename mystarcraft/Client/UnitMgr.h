#pragma once

#include "Include.h"

class CObj;
class CUnitMgr
{
	DECLARE_SINGLETON(CUnitMgr)
private:
	//현재 선택된 유닛목록
	list<CObj*>		m_curunitList;

	//0~9번 부대를 관리할 컨테이너
	vector<CObj*>	m_troops[10];
public:
	void SetUnit(CObj* pobj);
public:
	void Initialize(void);
	void discharge_unit(void);
	void path_relay(const int& goalidx);
public:
	CUnitMgr(void);
	~CUnitMgr(void);
};