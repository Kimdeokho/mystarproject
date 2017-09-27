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

	vector<D3DXVECTOR2> m_areapos64;
	vector<D3DXVECTOR2> m_areapos256;
	vector<D3DXVECTOR2> m_areapos512;
private:
	vector<CObj*>		m_vecmoveobj;
private:
	MYRECT<float>	m_unitrc;
private:
	int				m_idxdir[ASTAR_DIR_END];
	int				m_idx;
	MYLINE			m_line[4];
private:
	int			m_checkidx;
	int			m_stepsize;
public:
public:
	
	void SetObj_Area64(const int& curidx , const int& oldidx , CObj* pobj);
	void SetObj_Area256(const int& curidx , const int& oldidx , CObj* pobj);
	void SetObj_Area512(const int& curidx , const int& oldidx , CObj* pobj);
	bool  Check_Area(const MYRECT<float>& rc, D3DXVECTOR2& vpos ,const MYRECT<float>& center_rc, const D3DXVECTOR2& vcenterpos , const int& stepsize , const CObj* pself ,const CObj* ptarget, ASTAR_DIR edirflag);
	bool Collision_Area64(const int& idx ,D3DXVECTOR2& vpos , const CObj* pself , const CObj* ptarget);
	bool Collision_Area64Check(const int& idx , const MYRECT<float>& myrc , const CObj* pself , const CObj* ptarget);
	bool Collision_Attack(const MYRECT<float>& myrc ,CObj* pself );
	
	void Calculator_eightidx(const int& idx , const int& tilecnt);
	void SelectCheck(const int& idx , const D3DXVECTOR2& vpos);
	void DragCheck( const MYRECT<float>& rc);
	bool Choice_unit(const int& idx , const D3DXVECTOR2& vpos);
	void Choice_unit(const int& idx , const MYRECT<float>& rc);
	bool DummyCheck(const int& idx, const MYRECT<float>& rc);

	bool empty_area64(void);
	bool LineCross(const int& idx , const int& lineidx , const CObj* pself ,const CObj* ptarget, bool movecheck);
	bool diamond_check(const int& idx , const CObj* pself ,const CObj* ptarget);

	bool overlap_prevention(const MYRECT<float>& rc ,const MYRECT<float>& orirect , const D3DXVECTOR2& vpos , const D3DXVECTOR2& voripos , const CObj* pself);
public:
	void Areasize_debugrender(const int& areasize , const int& areacnt);
	//bool waypoint_check(const MYRECT<float>& rc ,const D3DXVECTOR2& vpos ,const int& objid);
	void waypoint_modify(MYRECT<float>& rc , D3DXVECTOR2& vpos , const int& objid);
public:
	CObj* Collision_check(const int& idx, CObj* pself/*, CObj*& ptarget*/);
public:
	CObj* AutoSearch_target(CObj* pobj , const int& isearch_range , const int& iattack_range ,TARGET_SEARCH_TYPE esearchtype);
	CObj* Target_extract(CObj* pobj ,const int& idx, const int& irange ,const int& iattackrange ,TARGET_SEARCH_TYPE esearchtype);
public:
	void ReleaseObj_Area64(const int& curidx , CObj* pobj);
	void ReleaseObj_Area256(const int& curidx , CObj* pobj);
	void ReleaseObj_Area512(const int& curidx , CObj* pobj);
public:
	CArea_Mgr(void);
	~CArea_Mgr(void);
};
