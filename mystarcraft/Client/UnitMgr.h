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

	D3DXVECTOR2		m_vUnitcenterpt;
	D3DXVECTOR2		m_vGoalPos;
	D3DXVECTOR2		m_vparallel_travel;//�����̵�
	int				m_vGoalIdx;
	int				m_magicbox_unitcnt;
public:
	void SetUnit(CObj* pobj);
public:
	bool		GetUnitlistempty(void);
	D3DXVECTOR2	GetParallel_travel(void);
	D3DXVECTOR2	GetUnitGoalPos(void);
	D3DXVECTOR2 GetUnitCentterPt(void);
	int			GetGoalidx(void);
	void		Calculate_UnitCenterPt(void);
public:
	void Initialize(void);
	void discharge_unit(void);
	void Intputkey_reaction(const int& nkey);
	void Intputkey_reaction(const int& firstkey , const int& secondkey);
	void clear_destroy_unitlist(CObj* pobj);
public:
	CUnitMgr(void);
	~CUnitMgr(void);
};