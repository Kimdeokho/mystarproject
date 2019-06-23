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
	map<int , CObj*>	m_troops[10]; //<objcnt값 , obj값>

	D3DXVECTOR2		m_vUnitcenterpt;
	D3DXVECTOR2		m_vGoalPos;
	int				m_vGoalIdx;
	int				m_magicbox_unitcnt;

	int				m_unitcnt;
public:
	void Troop_call(const int idx);
	void Assigned_troop(const int idx);
	void SetUnit(CObj* pobj);
	bool is_unit(const CObj* const pobj);
public:
	int				GetSelectunit_size(void);
	bool			GetUnitlistempty(void);
	D3DXVECTOR2		GetUnitGoalPos(void);
	D3DXVECTOR2		GetUnitCentterPt(void);
	int				GetGoalidx(void);
	list<CObj*>*	Getcur_unitlist(void);
	bool			Unit_Unification(void);
	void			Calculate_UnitCenterPt(list<CObj*>& unitlist, const D3DXVECTOR2& vgoalpos);
	void			Calculate_UnitCenterPt(const D3DXVECTOR2& vgoalpos);

public:
	void		Update_UI_Infomation(void);
public:
	void Initialize(void);
	void discharge_unit(void);
	void Intputkey_reaction(const int& nkey);
	void Intputkey_reaction(const int& firstkey , const int& secondkey);
	bool Input_cmd(const int& nkey, bool* waitkey);
	bool Input_cmd(const int& firstkey , const int& secondkey);
	void order_reaction(ORDER order);
	void clear_destroy_unitlist(CObj* pobj);
	
public:
	CUnitMgr(void);
	~CUnitMgr(void);
};