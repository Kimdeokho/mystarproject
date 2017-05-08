#pragma once
#include "obj.h"

class CAstar;
class CUnit :
	public CObj
{
protected:
	LPDIRECT3DTEXTURE9	m_curtex;
	list<int>			m_Sightoff_List;
protected:
	bool				m_sightoffsw;
	bool				m_InitSight;
	bool				m_fogsearch[SQ_TILECNTY*SQ_TILECNTX];

	int					m_iSightrange;
	int					m_diridx;
	float				m_fogtime;
protected:
	CAstar*				m_Astar;
	vector<D3DXVECTOR2>*				m_terrain_path;
	const vector<D3DXVECTOR2>*				m_unit_path;
	D3DXVECTOR2					m_vGoalPos;
	D3DXVECTOR2					m_vwaypointPos;
	D3DXVECTOR2					m_voldwayPos;
	float				m_fAstarTick;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	//다른 클래스로 뺄 예정
	void FogInitialize(void);
	void FogUpdate(void); 
	void FogRelease(void);
public:
	//다른 클래스로 뺄예정
	void PathFinder_Initialize(void);
	void PathFinder_Update(void);
	void Pathfind_start(void);
public:
	void Dir_calculation(void);
public:
	CUnit(void);
	~CUnit(void);
};
