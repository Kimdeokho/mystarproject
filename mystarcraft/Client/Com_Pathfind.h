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
	D3DXVECTOR2				m_vgap;

	bool					m_pathfind_pause;
	bool					m_multithread;
	bool					m_collision_move;
	bool					m_bmagicbox;
	
	MYRECT<float>			m_prerect;	

	int						m_igoalidx;
	int						m_curterrain_pathidx;
	int						m_realpathidx;
	int						m_mainstep;
	int						m_substep;
	int						m_arrivalrange;
	int						m_target_oldidx;
	int						m_refindcnt;
	float					m_collisionmove_time;
	float					m_stoptime;
	float					m_fchase_time;
	float					m_refind_time; //어택땅할때 최종목표로 다시 길찾기하는 텀

public:
	virtual void Initialize(CObj* pobj = NULL);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	void gap_initialize(bool bmagicbox);
	void StartPathfinding(void);
	void SetPathfindPause(bool bpause);
	void SetTargetObjID(const int& objid);
	void ClearPath(void);
	void UnitMoving_update(void);
	void SetGoalPos(const D3DXVECTOR2& vgoalpos , bool bmagicbox);

	void SetFlowField(void);
	void Setrally_path(const vector<D3DXVECTOR2>& vrally_path);
	void Target_chase(void);
public:
	CCom_Pathfind(D3DXVECTOR2& vpos , MYRECT<float>&	rc, const int& mainstep , const int& substep);
	~CCom_Pathfind(void);
};
