#pragma once

#include "Include.h"

class CObj;
class CArea_Mgr
{
	DECLARE_SINGLETON(CArea_Mgr)
private:
	list<CObj*>	m_Area64[64*64];
	list<CObj*>	m_Area256[16*16];
	list<CObj*>	m_Area512[8*8]; //공격할 대상 탐지
private:
	MYRECT<float>	m_unitrc;
private:
	int				m_idxdir[ASTAR_DIR_END];
	int				m_idx;
	//int			m_up_idx;	
	//int			m_leftup_idx;
	//int			m_rightup_idx;

	//int			m_left_idx;
	//int			m_right_idx;

	//int			m_down_idx;
	//int			m_leftdown_idx;
	//int			m_rightdown_idx;
private:
	int			m_checkidx;
	int			m_objId;
public:
	void SetObj_Area64(const int& curidx , const int& oldidx , CObj* pobj);
	void SetObj_Area256(const int& curidx , const int& oldidx , CObj* pobj);
	void SetObj_Area512(const int& curidx , const int& oldidx , CObj* pobj);
	bool Check_Area(ASTAR_DIR edir , const int& idx , const MYRECT<float>& rc, D3DXVECTOR2& vpos);
	void Collision_Area64(const int& idx , D3DXVECTOR2& vpos , const ASTAR_DIR& edir);
	bool Collision_Area64Check(const int& idx , D3DXVECTOR2& vpos , const MYRECT<float>& myrc);
	
	void SetObjId(const int& id);
	void Calculator_eightidx(const int& idx , const int& tilecnt);
	void SelectCheck(const int& idx , const D3DXVECTOR2& vpos);
	void DragCheck(const int& idx , const MYRECT<float>& rc);
	bool Choice_unit(const int& idx , const D3DXVECTOR2& vpos);
	void Choice_unit(const int& idx , const MYRECT<float>& rc);
	bool DummyCheck(const int& idx, const MYRECT<float>& rc);

	bool empty_area64(void);
public:
	bool Collision_check(const int& idx, CObj* pself, CObj*& ptarget);
	bool Collision_unit(const int& idx , CObj* pself, CObj*& ptarget);
public:
	void AutoSearch_target(CObj* pobj , CObj*& ptarget, const int& idx, const int& isearch_range , const int& areasize);
	void Target_extract256(CObj* pobj , CObj*& ptarget, const int& idx , const int& irange , int& minvalue);
	void Target_extract512(CObj* pobj , CObj*& ptarget, const int& idx , const int& irange , int& minvalue);
public:
	void CArea_Mgr::ReleaseObj_Area64(const int& curidx , CObj* pobj);
	void CArea_Mgr::ReleaseObj_Area256(const int& curidx , CObj* pobj);
	void CArea_Mgr::ReleaseObj_Area512(const int& curidx , CObj* pobj);
public:
	CArea_Mgr(void);
	~CArea_Mgr(void);
};
