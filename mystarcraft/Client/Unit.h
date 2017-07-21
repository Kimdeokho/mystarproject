#pragma once
#include "obj.h"

class CAstar;
class CUnit :
	public CObj
{
protected:
	LPDIRECT3DTEXTURE9	m_curtex;
protected:
	const vector<TEXINFO*>* 	m_unittexture[DIR_CNT];/*[]는 방향(각도)을 뜻한다*/
	//list<int>			m_Sightoff_List;
	//bool				m_sightoffsw;
	//bool				m_InitSight;
	//bool				m_fogsearch[SQ_TILECNTY*SQ_TILECNTX];

	//int					m_iSightrange;	
	//float				m_fogtime;
protected:
	int					m_diridx;
protected:
	CAstar*				m_Astar;
	//vector<D3DXVECTOR2>*				m_terrain_path;
	//vector<D3DXVECTOR2>*				m_dummy_path;
	//const vector<D3DXVECTOR2>*				m_unit_path;
	//D3DXVECTOR2					m_vGoalPos;
	//D3DXVECTOR2					m_vwaypointPos;
	//D3DXVECTOR2					m_voldwayPos;
	//float				m_fAstarTick;
protected:
	//collision
	D3DXVECTOR2 vtargetpos;
	D3DXVECTOR2 vnormal;
	float		m_search_time;
protected:
	//길찾기 관련
	short		m_flowfieldpath[SQ_TILECNTX*SQ_TILECNTY];
	float		m_movetime;
	int			m_igoalidx;
	vector<D3DXVECTOR2>		m_terrainpath;
	int						m_curterrain_pathidx;
	
	D3DXVECTOR2				m_vgoal_clickpos;
	int						m_realpathidx;

	vector<D3DXVECTOR2>		m_dummypath;
	vector<D3DXVECTOR2>		m_goalpath;
	vector<D3DXVECTOR2>		m_realpath;

	D3DXVECTOR2				m_curpos;
	D3DXVECTOR2				m_destpos;

	bool					m_multithread;
protected:
	CObj*				m_collision_target;
protected:
	CObj*				m_attack_target;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);

	virtual void Setstate(const TCHAR* statekey);
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
	//다른 클래스로 뺄예정
	void PathFinder_Update(void);
	void Pathfind_start(void);
public:
	void Search_Update(void);
public:
	void Dir_calculation(void);
public:
	//길찾기 관련
	short*	getflowfield(void);
	void	testmove(void);
	void	make_flowfieldpath(const int& goalidx);
	void	UnitMove_update(void);
public:
	CUnit(void);
	~CUnit(void);
};
