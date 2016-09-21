#pragma once

#include "Include.h"
class CTerrainBrushMgr
{
public:
	DECLARE_SINGLETON(CTerrainBrushMgr)
public:
	HRESULT	TerrainCheck(void);
	HRESULT	Initialize(void);
	HRESULT	InitBrush(const CString&	_str);
public:
	void	High_DirtBrush(const int idx);
	void	LeftArea(const int index, TILE& temptile , TERRAIN_INFO& pterrain_info);
	void	LeftUpArea(const int index, TILE& temptile , TERRAIN_INFO& pterrain_info);
	void	RightUpArea(const int index, TILE& temptile , TERRAIN_INFO& pterrain_info);
	void	RightArea(const int index,  TILE& temptile , TERRAIN_INFO& pterrain_info);
	void	RightDown(const int index, TILE& temptile , TERRAIN_INFO& pterrain_info);
	void	LeftDown(const int index, TILE& temptile , TERRAIN_INFO& pterrain_info);

	void	HD_RU_Algorithm(TILE& temptile , TERRAIN_INFO& _terrain_info ,const int oriidx , const int isequenceidx);
	void	HD_LU_Algorithm(TILE& temptile , TERRAIN_INFO& _terrain_info ,const int oriidx , const int isequenceidx);
	void	HD_RD_Algorithm(TILE& temptile , TERRAIN_INFO& _terrain_info ,const int oriidx , const int isequenceidx);
	void	HD_LD_Algorithm(TILE& temptile , TERRAIN_INFO& _terrain_info ,const int oriidx , const int isequenceidx);
	void	HD_L_Algorithm(TILE& temptile , TERRAIN_INFO& _terrain_info ,const int oriidx , const int isequenceidx);
	void	HD_R_Algorithm(TILE& temptile , TERRAIN_INFO& _terrain_info ,const int oriidx , const int isequenceidx);
public:
	int		get_sqindex(void);
	bool	Overlap_Group(const int irow ,const int icol ,const int startidx ,const int group_id);
private:
	int			m_TerrainMask[8];
	int			m_curFloor;
	int			m_curTerrainID;
	CListBox*	m_TerrainListBox;
public:
	CTerrainBrushMgr(void);
	~CTerrainBrushMgr(void);
};
