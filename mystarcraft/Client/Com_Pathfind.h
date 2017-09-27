#pragma once
#include "component.h"

class CAstar;

class CCom_Pathfind :
	public CComponent
{
private:
	D3DXVECTOR2&	m_vPos;
	MYRECT<float>&	m_rect;
	D3DXVECTOR2		m_vcurdir;
	CObj*			m_pTarget;
	const float*			m_fspeed;
	
	int				m_target_objid;
	MYRECT<float>	m_obj_vertex;
	
private:
	CComponent*			m_com_target;
	CComponent*			m_com_animation;
	CComponent*			m_com_collision;
private:
	CAstar*		m_Astar;
	short		m_flowfieldpath[SQ_TILECNTX*SQ_TILECNTY];
	
	vector<D3DXVECTOR2>		m_terrainpath;
	vector<D3DXVECTOR2>		m_realpath;

	D3DXVECTOR2				m_goalpos;
	D3DXVECTOR2				m_vprepos;
	D3DXVECTOR2				m_vtargetpos;

	bool					m_pathfind_pause;
	bool					m_multithread;
	bool					m_collision_move;
	bool					m_bmagicbox;
	
	MYRECT<float>			m_prerect;	

	int						m_igoalidx;
	int						m_curterrain_pathidx;
	int						m_realpathidx;
	int						m_stepsize;
	int						m_arrivalrange;
	int						m_target_oldidx;
	int						m_refindcnt;
	float					m_collisionmove_time;
	float					m_stoptime;
	float					m_stoptime_offset;
	float					m_fchase_time;

public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	void StartPathfinding(bool bmagicbox);
	void SetPathfindPause(bool bpause);
	void SetTargetObjID(const int& objid);
	void ClearPath(void);
	void UnitMoving_update(void);
	void SetGoalPos(const D3DXVECTOR2& vgoalpos);
	void SetGoalidx(const int& idx);
	void SetFlowField(void);
	void Target_chase(void);
public:
	CCom_Pathfind(D3DXVECTOR2& vpos , MYRECT<float>&	rc);
	~CCom_Pathfind(void);
};
