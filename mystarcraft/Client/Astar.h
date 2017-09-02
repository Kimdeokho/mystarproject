#pragma once

#include "Include.h"

const int MAXPATH_IDX = 250;
template<typename T> class CMyHeapSort;
class CObj;
class CArea_Mgr;
class CAstar //유닛마다 가지고 있어야할듯 싶다.
{
	//DECLARE_SINGLETON(CAstar);
private://지형이동 Astar
	//boost::pool<>*			m_PathPool;

	CMyHeapSort<PATH_NODE*>*			m_terrain_openlist;

	PATH_NODE*				m_terrain_idxopenlist[1];

	boost::unordered_map<int /*인덱스*/, PATH_NODE*> m_terrain_closelist;
private: //유닛사이 이동 AStar
	//multimap<int/*비용*/ , PATH_NODE*>		m_openlist;
	CMyHeapSort<PATH_NODE*>*					m_openlist;

	boost::unordered_map<int , PATH_NODE*>	m_idxopenlist;
	boost::unordered_map<int , PATH_NODE*>	m_closelist;
private:
	PATH_NODE*					m_unitpath_pool;
	int							m_pathpool_idx;
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
	vector<D3DXVECTOR2>		m_dummy_path;
	vector<D3DXVECTOR2>		m_unit_path;

	int						m_accumulate;
	bool					m_bdummy;
	PATH_NODE*				m_dummynode;
	int						m_eight_weight[ASTAR_DIR_END];
	D3DXVECTOR2				m_eight_vpos[ASTAR_DIR_END];
	MYRECT<float>			m_eight_rect[ASTAR_DIR_END];
	float					m_fAstarTime;
	float					m_funit_astartime;
	float					m_fdummytime;

	int						m_pathidx;
	CObj*					m_pObj;
	bool					m_brealpath;
	D3DXVECTOR2				m_ObjPos;

	CArea_Mgr*				m_areamgr_inst;

	int						m_unit_stepsize;
	float					m_unit_diagonalstep;
	int						m_objid;
	bool					m_bweightG;
public:
	void Initialize(CObj* pobj);
	void Init_eightidx(const int& idx);
	void Init_eightpos_rect(const D3DXVECTOR2& vpos , const MYRECT<float>& rc, const int& stepsize);
	void InitPos(const float& addx , const float& addy , const D3DXVECTOR2& vpos);
public:
	void TerrainPath_calculation_Start(const D3DXVECTOR2& startpos , const D3DXVECTOR2& goalpos);
	void TerrainPath_calculation_Update(const D3DXVECTOR2& goalpos);
	void UnitPath_calculation_Start(const D3DXVECTOR2& startpos , const D3DXVECTOR2& goalpos, const int& stepsize , const int& objid , bool weightG);
	void UnitPath_calculation_Update(const MYRECT<float>& rect , vector<D3DXVECTOR2>& vecpath);
	void Path_Render(void);
	bool Check_CloseList(const D3DXVECTOR2& vpos);
	bool Check_OpenList(const D3DXVECTOR2& idxpos, PATH_NODE* parentnode);
	bool Check_TerrainOpenList(const int& idx);
	bool Check_TerrainCloseList(const int& idx);
	bool Check_TileOption(const int& idx);
	bool Check_TileOption(const D3DXVECTOR2& vpos);
	bool Search_surrounding_units(const MYRECT<float>& rect , D3DXVECTOR2& vstartpos ,const MYRECT<float>& center_rc, const D3DXVECTOR2& vcenterpos);
	void Make_TerrainNode(const int& idx , PATH_NODE* parent_node ,const int& g);
	void Make_UnitNode(PATH_NODE* parent_node ,const D3DXVECTOR2& vpos , const float& g_distance);
	bool Shortest_distance_check(const int& idx , PATH_NODE* pnode , const int& g);
public:
	void Release_TerrainPath(void);
	void Release_UnitPath(void);
	void Release_unit_closelist(void);
	void Release_TerrainCloseList(void);
	void Release(void);
public:
	vector<D3DXVECTOR2>*		GetTerrain_Path(void);
	vector<D3DXVECTOR2>*		Getdummy_Path(void);
	vector<D3DXVECTOR2>*		GetUnit_Path(void);
	PATH_NODE*					GetNode(const int& idx);
public:
	CAstar(void);
	~CAstar(void);
};
