#pragma once

#include "Include.h"

class CMyHeapSort;
class CAstar //유닛마다 가지고 있어야할듯 싶다.
{
	//DECLARE_SINGLETON(CAstar);
private:
private://지형이동 Astar
	boost::pool<>*			m_PathPool;

	CMyHeapSort*			m_terrain_openlist;
	//multimap<int/*비용*/ , PATH_NODE*>	m_terrain_openlist;
	bool					m_terrain_idxopenlist[SQ_TILECNTY*SQ_TILECNTX];

	//map<int , PATH_NODE*>	m_terrain_closelist;
	boost::unordered_map<int /*인덱스*/, PATH_NODE*> m_terrain_closelist;
private: //유닛사이 이동 AStar
	//multimap<int/*비용*/ , PATH_NODE*>		m_openlist;
	CMyHeapSort*							m_openlist;
	boost::unordered_set<int>				m_idxopenlist;//열린목록이 있나 없나 체크할때 필요

	boost::unordered_map<int , PATH_NODE*>	m_closelist;
private:
	D3DXVECTOR2				m_vNode_pos;
	D3DXVECTOR2				m_vStart_pos;
	D3DXVECTOR2				m_vGoal_pos;
private:
	int						m_startindex;
	int						m_goalidx;
	int						m_eight_idx[ASTAR_DIR_END];
	int						m_startidx;
	int						m_tilecnt;

	int						m_curloopcnt;
	int						m_default_loop;
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
public:
	void Initialize(void);
	void Init_eightidx(const int& idx);
	void InitPos(const float& addx , const float& addy , const D3DXVECTOR2& vpos);
public:
	void Path_calculation_Start(const D3DXVECTOR2& startpos , const D3DXVECTOR2& goalpos , bool bdummypath);
	void Path_calculation_Update(const D3DXVECTOR2& goalpos);
	void Path_calculation(const D3DXVECTOR2& startpos , const D3DXVECTOR2& goalpos , const MYRECT<float>& rect , const int& objid);
	void Path_Render(void);
	bool Check_CloseList(const int& idx);
	bool Check_OpenList(const int& idx);
	bool Check_TerrainOpenList(const int& idx);
	bool Check_TerrainCloseList(const int& idx);
	bool Check_TileOption(const int& idx);
	bool Check_TileOption(const D3DXVECTOR2& vpos);
	bool Search_surrounding_units(ASTAR_DIR edir , const int& idx , const MYRECT<float>& rect  ,D3DXVECTOR2& vstartpos);
	void Make_TerrainNode(const int& idx , PATH_NODE* parent_node ,const int& g);
	void Make_UnitNode(const int& idx , PATH_NODE* parent_node ,const int& g);
	bool Shortest_distance_check(const int& idx);
public:
	void Release_TerrainPath(void);
	void Release_UnitPath(void);
	void Release_TerrainCloseList(void);
	void Release(void);
public:
	vector<D3DXVECTOR2>*	GetTerrain_Path(void);
	const vector<D3DXVECTOR2>*	GetUnit_Path(void);
public:
	CAstar(void);
	~CAstar(void);
};
