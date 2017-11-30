#pragma once

#include "Include.h"

class CObj;
class CArea_Mgr
{
	DECLARE_SINGLETON(CArea_Mgr)
private:
	list<CObj*>	m_Area64[64*64];
	//list<CObj*>	m_Area256[16*16];
	//list<CObj*>	m_Area512[8*8]; //공격할 대상 탐지

	vector<D3DXVECTOR2> m_areapos64;
	vector<D3DXVECTOR2> m_areapos256;
	vector<D3DXVECTOR2> m_areapos512;
	CObj*				m_pchoice_target;
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
	
	void SetObj_Area64(const int& curidx , const int& oldidx , CObj* pobj );
	bool Check_Area(const MYRECT<float>& rc, D3DXVECTOR2& vpos ,const MYRECT<float>& center_rc, const D3DXVECTOR2& vcenterpos , const int& stepsize , CObj* pself ,const CObj* ptarget);
	bool PathFind_Area(const int& idx , D3DXVECTOR2& vpos , CObj* pself , const CObj* ptarget);
	bool CollisionCheck_Area( const int& idx , CObj* pself , const CObj* ptarget);
	
	void Calculator_eightidx(const int& idx , const int& tilecnt);
	void SelectCheck(const int& idx , const D3DXVECTOR2& vpos);
	void DragCheck( const MYRECT<float>& rc);
	bool Choice_unit(const int& idx , const D3DXVECTOR2& vpos);
	void Choice_unit(const int& idx , const MYRECT<float>& rc);
	
	bool DummyCheck(const int& idx, const MYRECT<float>& rc);

	bool MeleeAttack_Search( CObj* pself , CObj*& ptarget, const MYRECT<float>& myrc ,TARGET_SEARCH_TYPE esearchtype);

	//bool empty_area64(void);
	bool LineCross(const int& idx , const int& lineidx , CObj* pself , bool movecheck);
	bool diamond_check( const int& idx , CObj* pself ,const CObj* ptarget);

	bool overlap_prevention(const MYRECT<float>& rc ,const MYRECT<float>& orirect , const D3DXVECTOR2& vpos , const D3DXVECTOR2& voripos , CObj* pself);
	void Search_Mineral( const int& idx , const int& isearch_range ,CObj* pself , CObj*& pmineral);
	CObj* Search_Partbuilding(const int& areaidx , const int& searchidx , OBJID eid);
	bool Mineral_extractor(const int& isearch_range ,CObj* pself , CObj*& pmineral);
	void gas_extractor(CObj* pself , const D3DXVECTOR2& vpt);
public:
	void Areasize_debugrender(const int& areasize , const int& areacnt);

	CObj* GetChoiceTarget(void);

	void  Setsplash_damage(CObj* pmyobj ,const WEAPON_INFO& weaponinfo,  const D3DXVECTOR2& vsplash_pos, const float& range1 , const float& range2, const float& range3 , bool random_splash);
public:
	CObj* Collision_check(CObj* pself , const int& idx);
	bool Collocate_check(CObj* pself , const int& idx ,MYRECT<float>& myrc );
	bool Building_Collocate_check(CObj* pself , const int& idx ,MYRECT<float>& myrc );
public:
	CObj*	AutoSearch_target(CObj* pobj , const int& isearch_range ,TARGET_SEARCH_TYPE esearchtype);
	CObj*	Target_extract(CObj* pobj ,const int& idx, const int& irange ,TARGET_SEARCH_TYPE esearchtype);
	CObj*	MedicTarget_extract(CObj* pself ,const int& idx, const int& isearch_range ,TARGET_SEARCH_TYPE esearchtype);
	void	TargetChoice(const D3DXVECTOR2& vpos);
public:
	void ReleaseObj_Area64(const int& curidx , CObj* pobj);
public:
	CArea_Mgr(void);
	~CArea_Mgr(void);
};
