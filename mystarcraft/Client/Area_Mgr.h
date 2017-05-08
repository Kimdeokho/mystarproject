#pragma once

#include "Include.h"

class CObj;
class CArea_Mgr
{
	DECLARE_SINGLETON(CArea_Mgr)
private:
	list<CObj*>	m_Area64[64*64];
	list<CObj*>	m_Area512[8*8]; //공격할 대상 탐지
private:
	MYRECT<float>	m_unitrc;
private:
	//int		m_idxdir[ASTAR_DIR_END];
	int			m_idx;
	int			m_up_idx;	
	int			m_leftup_idx;
	int			m_rightup_idx;

	int			m_left_idx;
	int			m_right_idx;

	int			m_down_idx;
	int			m_leftdown_idx;
	int			m_rightdown_idx;
private:
	int			m_checkidx;
	int			m_objId;
public:
	void SetObj_Area64(const int& curidx , const int& oldidx , CObj* pobj);
	void SetObj_Area512(const int& curidx , const int& oldidx , CObj* pobj);
	bool Check_Area(ASTAR_DIR edir , const int& idx , const MYRECT<float>& rc, D3DXVECTOR2& vpos);
	void Collision_Area(const int& idx , D3DXVECTOR2& vpos);
	void SetObjId(const int& id);
public:
	CArea_Mgr(void);
	~CArea_Mgr(void);
};
