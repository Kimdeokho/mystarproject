#pragma once
#include "component.h"

class CCom_AirPathfind :
	public CComponent
{
private:
	CObj*			m_pTarget;
	D3DXVECTOR2&	m_vpos;
	D3DXVECTOR2		m_vgoalpos;
	D3DXVECTOR2		m_vcurdest_pos;
	D3DXVECTOR2		m_vcurdir;

	float			m_fchase_time;
	const float*	m_fspeed;

	int				m_target_objid;
	int				m_target_oldidx;
	bool			m_is_moveupdate;
	bool			m_is_arrive;
public:
	void Air_MovingUpdate(void);
	void TargetChase(void);
	void SetTargetObjID(const int& objid);
	void SetDestPos(const D3DXVECTOR2& vgoalpos);
	void SetGoalPos(const D3DXVECTOR2& vgoalpos);
	void SetAir_moveupdate(bool is_dupdate);
	bool Getarrive(void);
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CCom_AirPathfind(D3DXVECTOR2& vpos);
	~CCom_AirPathfind(void);
};
