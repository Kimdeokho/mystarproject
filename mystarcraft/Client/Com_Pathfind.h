#pragma once
#include "component.h"

class CAstar;
class CCom_Targetsearch;
class CCom_Animation;

class CCom_Pathfind :
	public CComponent
{
private:
	D3DXVECTOR2&	m_vPos;
	MYRECT<float>&	m_rect;
	D3DXVECTOR2		m_vcurdir;
	CObj*			m_pTarget;
	float*			m_fspeed;
	
	int				m_objid;
	MYRECT<float>	m_obj_vertex;
	
private:
	CCom_Targetsearch*		m_com_target;

	CCom_Animation*			m_com_animation;
private:
	CAstar*		m_Astar;
	short		m_flowfieldpath[SQ_TILECNTX*SQ_TILECNTY];
	int			m_igoalidx;
	vector<D3DXVECTOR2>		m_terrainpath;
	int						m_curterrain_pathidx;

	int						m_realpathidx;

	vector<D3DXVECTOR2>		m_realpath;

	D3DXVECTOR2				m_goalpos;

	bool					m_multithread;
	int						m_stepsize;

	float					m_collisionmove_time;
	bool					m_collision_move;
	float					m_stoptime;

	int						m_arrivalrange;

	D3DXVECTOR2				m_vprepos;
	MYRECT<float>			m_prerect;

	int						m_target_oldidx;
	bool					m_pathfind_pause;
public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	void StartPathfinding(void);
	void SetPathfindPause(bool bpause);
	void ClearPath(void);
	void UnitMoving_update(void);
	void SetGoalPos(const D3DXVECTOR2& vgoalpos);
	void SetGoalidx(const int& idx);
	void SetFlowField(void);
public:
	CCom_Pathfind(D3DXVECTOR2& vpos , MYRECT<float>&	rc);
	~CCom_Pathfind(void);
};
