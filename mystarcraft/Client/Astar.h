#pragma once

#include "Include.h"

template<typename T> class CMyHeapSort;
class CObj;
class CArea_Mgr;
class CAstar //유닛마다 가지고 있어야할듯 싶다.
{
	//DECLARE_SINGLETON(CAstar);
private://지형이동 Astar
	boost::pool<>*			m_PathPool;

	CMyHeapSort<PATH_NODE*>*			m_terrain_openlist;

	PATH_NODE*				m_terrain_idxopenlist[SQ_TILECNTY*SQ_TILECNTX];

	boost::unordered_map<int /*인덱스*/, PATH_NODE*> m_terrain_closelist;
private: //유닛사이 이동 AStar
	multimap<int/*비용*/ , PATH_NODE*>		m_openlist;
	//CMyHeapSort<PATH_NODE*>*							m_openlist;
	//boost::unordered_set<int>				m_idxopenlist;//열린목록이 있나 없나 체크할때 필요
	boost::unordered_map<int , PATH_NODE*>	m_idxopenlist;
	boost::unordered_map<int , PATH_NODE*>	m_closelist;
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
	float					m_fAstarTime;
	float					m_funit_astartime;
	float					m_fdummytime;

	int						m_pathidx;
	CObj*					m_pObj;
	bool					m_brealpath;
	D3DXVECTOR2				m_ObjPos;

	CArea_Mgr*				m_areamgr_inst;
public:
	void Initialize(CObj* pobj);
	void Init_eightidx(const int& idx);
	void InitPos(const float& addx , const float& addy , const D3DXVECTOR2& vpos);
public:
	void TerrainPath_calculation_Start(const D3DXVECTOR2& startpos , const D3DXVECTOR2& goalpos);
	void TerrainPath_calculation_Update(const D3DXVECTOR2& goalpos);
	void UnitPath_calculation_Start(const D3DXVECTOR2& startpos , const D3DXVECTOR2& goalpos);
	void UnitPath_calculation_Update(const D3DXVECTOR2& startpos , const D3DXVECTOR2& goalpos , const MYRECT<float>& rect , const int& objid , vector<D3DXVECTOR2>& vecpath, bool dummymode);
	void Path_Render(void);
	bool Check_CloseList(const int& idx);
	bool Check_OpenList(const int& idx , const int& _g , PATH_NODE* parentnode);
	bool Check_TerrainOpenList(const int& idx);
	bool Check_TerrainCloseList(const int& idx);
	bool Check_TileOption(const int& idx);
	bool Check_TileOption(const D3DXVECTOR2& vpos);
	bool Search_surrounding_units(ASTAR_DIR edir , const int& idx , const MYRECT<float>& rect  ,D3DXVECTOR2& vstartpos);
	void Make_TerrainNode(const int& idx , PATH_NODE* parent_node ,const int& g);
	void Make_UnitNode(const int& idx , PATH_NODE* parent_node ,const int& g);
	void MoveUpdate(void);
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
