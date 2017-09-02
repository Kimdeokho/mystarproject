#pragma once
#include "obj.h"

class CAstar;
class CUnit :
	public CObj
{
protected:
	LPDIRECT3DTEXTURE9	m_curtex;
protected:
protected:
	const vector<TEXINFO*>* 	m_unittexture[DIR_CNT];/*[]는 방향(각도)을 뜻한다*/
protected:
	int					m_diridx;
protected:
	//transform 관련
	D3DXVECTOR2			m_vcurdir; //현재 가르키고 있는 방향
	float				m_fspeed;
protected:
	//collision
	CObj*				m_collision_target;
	D3DXVECTOR2 vtargetpos;
	D3DXVECTOR2 m_collision_vnormal;
	float		m_search_time;
protected:
	//길찾기 관련
	CAstar*		m_Astar;
	short		m_flowfieldpath[SQ_TILECNTX*SQ_TILECNTY];
	float		m_movetime;
	int			m_igoalidx;
	vector<D3DXVECTOR2>		m_terrainpath;
	int						m_curterrain_pathidx;
	
	D3DXVECTOR2				m_vgoal_clickpos;
	int						m_realpathidx;

	vector<D3DXVECTOR2>		m_realpath;

	D3DXVECTOR2				m_curpos;
	D3DXVECTOR2				m_goalpos;

	bool					m_multithread;
	int						m_stepsize;
	
	float					m_collisionmove_time;
	bool					m_collision_move;

	int						m_arrivalrange;

	D3DXVECTOR2				m_vprepos;
	D3DXVECTOR2				m_voripos;
	
	MYRECT<float>			m_orirect;
	MYRECT<float>			m_prerect;

	int						m_target_oldidx;
	bool					m_pathfind_pause;
protected:
	CObj*					m_attack_target;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void Set_texture_statename(const TCHAR* statekey);
public:
	void SetState(STATE estate);
public:

public:
	//충돌관련
	void Collision_update(void);
public:
	//다른 클래스로 뺄 예정
	//void FogInitialize(void);
	void FogUpdate(void); 
	void FogRelease(void);
public:
	void Animation_update(void);
public:

public:
	void Search_Update(void);
public:
	void Dir_calculation(D3DXVECTOR2& vnomal);
public:
	//길찾기 관련
	//다른 클래스로 뺄예정
	void	Pathfind_start(void);
	short*	getflowfield(void);
	void	PathFinder_Update(void);
	void	make_flowfieldpath(void);
	void	UnitMoving_update(void);
public:
public:
	CUnit(void);
	virtual ~CUnit(void);
};
