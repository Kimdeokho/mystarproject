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
	bool  Check_Area(const MYRECT<float>& rc, D3DXVECTOR2& vpos ,const MYRECT<float>& center_rc, const D3DXVECTOR2& vcenterpos , const int& stepsize , const int& objid , ASTAR_DIR edirflag);
	void Collision_Area64(const int& idx ,D3DXVECTOR2& vpos , const int& objid);
	bool Collision_Area64Check(const int& idx , const MYRECT<float>& myrc , const int& objid);
	
	void Calculator_eightidx(const int& idx , const int& tilecnt);
	void SelectCheck(const int& idx , const D3DXVECTOR2& vpos);
	void DragCheck(const int& idx , const MYRECT<float>& rc);
	bool Choice_unit(const int& idx , const D3DXVECTOR2& vpos);
	void Choice_unit(const int& idx , const MYRECT<float>& rc);
	bool DummyCheck(const int& idx, const MYRECT<float>& rc);

	bool empty_area64(void);
	bool LineCross(const int& idx , const int& lineidx , const int& objid ,bool movecheck);
	bool diamond_check(const int& idx , const int& objid);

	bool overlap_prevention(const MYRECT<float>& rc ,const MYRECT<float>& orirect , const D3DXVECTOR2& vpos , const D3DXVECTOR2& voripos ,  const D3DXVECTOR2& vmydir, const int& objid);
public:
	void Areasize_debugrender(const int& areasize , const int& areacnt);
	bool waypoint_check(const MYRECT<float>& rc ,const D3DXVECTOR2& vpos ,const int& objid);
	void waypoint_modify(MYRECT<float>& rc , D3DXVECTOR2& vpos , const int& objid);
public:
	bool Collision_check(const int& idx, CObj* pself, CObj*& ptarget);
	bool Collision_unit(const int& idx , CObj* pself, CObj*& ptarget);
public:
	CObj* AutoSearch_target(CObj* pobj , const int& idx, const int& isearch_range , const int& areasize ,TARGET_SEARCH_TYPE esearchtype);
	void Target_extract256(CObj* pobj ,CObj*& ptarget , const int& idx , const int& irange , float& minvalue ,TARGET_SEARCH_TYPE esearchtype);
	void Target_extract512(CObj* pobj ,CObj*& ptarget , const int& idx , const int& irange , float& minvalue ,TARGET_SEARCH_TYPE esearchtype);
public:
	void CArea_Mgr::ReleaseObj_Area64(const int& curidx , CObj* pobj);
	void CArea_Mgr::ReleaseObj_Area256(const int& curidx , CObj* pobj);
	void CArea_Mgr::ReleaseObj_Area512(const int& curidx , CObj* pobj);
public:
	CArea_Mgr(void);
	~CArea_Mgr(void);
};
