#pragma once

#include "Include.h"

const int MAXPATH_IDX = 150;
template<typename T> class CMyHeapSort;
class CObj;
class CArea_Mgr;
class CAstar //유닛마다 가지고 있어야할듯 싶다.
{
private: //유닛사이 이동 AStar
	CMyHeapSort<PATH_NODE*>*					m_openlist; //pop용도
	//boost::unordered_map<int , PATH_NODE*>	m_idxopenlist; //검색용
	//boost::unordered_map<int , PATH_NODE*>	m_closelist;   //검색용

	//bool										m_idxfind[128*128];
	//unsigned char 비트연산으로 해볼수도있겠다

	bool									m_is_escape;
	bool									m_terrain_end;
	D3DXVECTOR2								m_destdir;

	//마스크는 256으로 나누고, 
	//인덱스는 맵크기 0~4096이므로, 4096/n
	unsigned int							m_findidx[256*8];
	//unsigned int							m_findidx[128*4];
	unsigned int							m_mask[32];

	int										m_irow;
	int										m_icol;
	int										m_maskidx;
	int										m_widthcnt;
private:
	PATH_NODE*					m_unitpath_pool;
	int							m_pathpool_idx;
	int							m_maxnodecnt;
private:
	D3DXVECTOR2				m_vStart_pos;
	D3DXVECTOR2				m_vGoal_pos;
private:
	int						m_startindex;
	int						m_goalidx;

	int						m_tilecnt;
private:

	PATH_NODE*				m_dummynodeH;
	PATH_NODE*				m_dummynodeX;

	int						m_eight_idx[ASTAR_DIR_END];
	int						m_eight_weight[ASTAR_DIR_END];
	D3DXVECTOR2				m_eight_vpos[ASTAR_DIR_END];
	MYRECT<float>			m_eight_rect[ASTAR_DIR_END];
	float					m_fdummytime;

	int						m_pathidx;
	CObj*					m_pObj;

	CArea_Mgr*				m_areamgr_inst;

	int						m_unit_stepsize;
	float					m_unit_diagonalstep;
	bool					m_bweightG;

	CObj*					m_ptarget;
	MYRECT<float>			m_vertex;
	int						m_range;
public:
	void Initialize(CObj* pobj);
	void Init_eightidx(const int& idx);
	void Init_eightpos_rect(const D3DXVECTOR2& vpos , const MYRECT<float>& rc, const int& stepsize);
public:
	//void TerrainPath_calculation_Start(const D3DXVECTOR2& startpos , const D3DXVECTOR2& goalpos);
	//void TerrainPath_calculation_Update(const D3DXVECTOR2& goalpos);
	void UnitPath_calculation_Start(const D3DXVECTOR2& startpos , const D3DXVECTOR2& goalpos, const int& stepsize, const vector<D3DXVECTOR2>& terrain_path , const int& terrain_idx);
	void UnitPath_calculation_Update(vector<D3DXVECTOR2>& vecpath , CObj* ptarget);
	void Path_Render(void);
	bool Check_idx(const D3DXVECTOR2& vpos);
	bool Check_CloseList(const D3DXVECTOR2& vpos);
	bool Check_OpenList(const D3DXVECTOR2& idxpos, PATH_NODE* parentnode);
	bool Check_TileOption(const int& idx);
	bool Check_TileOption(const D3DXVECTOR2& vpos);
	
	void Make_UnitNode( PATH_NODE* parent_node ,const D3DXVECTOR2& vpos , const float& g_distance);
public:
	void Release_unit_openlist(void);
	void Release_unit_closelist(void);
	void Release(void);
public:
public:
	CAstar(void);
	~CAstar(void);
};
