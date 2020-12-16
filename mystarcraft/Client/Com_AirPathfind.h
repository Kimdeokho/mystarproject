#pragma once
#include "component.h"

class CCom_AirPathfind :
	public CComponent
{
private:
	CObj*			m_pTarget;
	D3DXVECTOR2&	m_vpos;
	D3DXVECTOR2		m_vgoalpos; //처음 명령내린 도착지점
	D3DXVECTOR2		m_vcurdest_pos; //가야할 지점이 수시로 바뀐다.
	D3DXVECTOR2		m_vcurdir;
	D3DXVECTOR2		m_vgap;

	float			m_fchase_time;
	const float*	m_fspeed;

	int				m_target_objid;
	int				m_target_oldidx;
	bool			m_is_moveupdate;
	bool			m_is_arrive;

	int				m_target_objcnt;
public:
	void Air_MovingUpdate(void);
	void TargetChase(void);
	void SetTargetObjID(const int objid);
	void SetDestPos(const D3DXVECTOR2& vgoalpos);
	void SetGoalPos(const D3DXVECTOR2& vgoalpos , bool is_magicbox = false);
	void SetAir_moveupdate(bool is_dupdate);
	bool Getarrive(void);
	void gap_initialize(bool is_magicbox);
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CCom_AirPathfind(D3DXVECTOR2& vpos);
	~CCom_AirPathfind(void);
};
