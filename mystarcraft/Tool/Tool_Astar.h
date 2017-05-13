#pragma once

#include "Include.h"

class CTool_MyHeapSort;
class CTool_Astar //유닛마다 가지고 있어야할듯 싶다.
{
	//DECLARE_SINGLETON(CAstar);
private:
private://지형이동 Astar
	boost::pool<>*			m_PathPool;

	CTool_MyHeapSort*		m_terrain_openlist;
	PATH_NODE*				m_terrain_idxopenlist[SQ_TILECNTY*SQ_TILECNTX];

	boost::unordered_map<int /*인덱스*/, PATH_NODE*> m_terrain_closelist;

	int						m_pathidx[SQ_TILECNTY*SQ_TILECNTX];
private:
	D3DXVECTOR2				m_vNode_pos;
	D3DXVECTOR2				m_vStart_pos;
	D3DXVECTOR2				m_vGoal_pos;
private:
	int						m_startindex;
	int						m_goalidx;
	int						m_eight_idx[ASTAR_DIR_END];
	int						m_tilecnt;

	int						m_maxloopcnt;

	bool					m_bPathUpdatestart;
	bool					m_brelease_start;
	int						m_releasecnt;
private:
	PATH_NODE*				m_terrain_pathnode;
	vector<D3DXVECTOR2>		m_terrain_path;
	vector<D3DXVECTOR2>		m_unit_path;

	int						m_accumulate;
	bool					m_bdummy;
	PATH_NODE*				m_dummynode;
	int						m_eight_weight[ASTAR_DIR_END];
	float					m_fAstarTime;
	float					m_fdummytime;
public:
	void Initialize(void);
	void Init_eightidx(const int& idx);
	void InitPos(const float& addx , const float& addy , const D3DXVECTOR2& vpos);
public:
	void Path_calculation_Start(const D3DXVECTOR2& startpos , const D3DXVECTOR2& goalpos , bool bdummypath);
	void Path_calculation_Update(const D3DXVECTOR2& goalpos);
	void Path_Render(void);
	bool Check_TerrainOpenList(const int& idx);
	bool Check_TerrainCloseList(const int& idx);
	bool Check_TileOption(const int& idx);
	void Make_TerrainNode(const int& idx , PATH_NODE* parent_node ,const int& g);
	bool Shortest_distance_check(const int& idx , PATH_NODE* pnode , const int& g);
public:
	void Waypoint_calculation(void);
public:
	void Release_TerrainPath(void);
	void Release(void);
public:
	vector<D3DXVECTOR2>*	GetTerrain_Path(void);
public:
	CTool_Astar(void);
	~CTool_Astar(void);
};
