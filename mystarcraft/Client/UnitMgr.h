#pragma once

#include "Include.h"

class CObj;
class CUnitMgr
{
	DECLARE_SINGLETON(CUnitMgr)
private:
	//���� ���õ� ���ָ��
	list<CObj*>		m_curunitList;

	//0~9�� �δ븦 ������ �����̳�
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