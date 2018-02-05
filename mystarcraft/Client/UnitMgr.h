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

	D3DXVECTOR2		m_vUnitcenterpt;
	D3DXVECTOR2		m_vGoalPos;
	int				m_vGoalIdx;
	int				m_magicbox_unitcnt;
public:
	void SetUnit(CObj* pobj);
public:
	int				GetSelectunit_size(void);
	bool			GetUnitlistempty(void);
	D3DXVECTOR2		GetUnitGoalPos(void);
	D3DXVECTOR2		GetUnitCentterPt(void);
	int				GetGoalidx(void);
	list<CObj*>*	Getcur_unitlist(void);
	void			Calculate_UnitCenterPt(const D3DXVECTOR2& vgoalpos);

public:
	void		Update_UI_Infomation(void);
public:
	void Initialize(void);
	void discharge_unit(void);
	void Intputkey_reaction(const int& nkey);
	void Intputkey_reaction(const int& firstkey , const int& secondkey);
	void order_reaction(ORDER eorder);
	void clear_destroy_unitlist(CObj* pobj);
public:
	CUnitMgr(void);
	~CUnitMgr(void);
};