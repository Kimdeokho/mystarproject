#pragma once

#include "MyBrush.h"
class CHighDirt_Brush : public CMyBrush
{
private:
	bool	m_bOverlap[GROUP_END][GROUP_END];
public:
	virtual void BrushPaint();
	void	LeftArea(const int startidx, TILE& temptile, TERRAIN_INFO& _terrain_info);
	void	LeftUpArea(const int startidx, TILE& temptile , TERRAIN_INFO& _terrain_info);
	void	LeftDown(const int startidx, TILE& temptile , TERRAIN_INFO& _terrain_info);
	void	RightUpArea(const int startidx, TILE& temptile , TERRAIN_INFO& _terrain_info);
	void	RightArea(const int startidx, TILE& temptile , TERRAIN_INFO& _terrain_info);
	void	RightDown(const int startidx, TILE& temptile , TERRAIN_INFO& _terrain_info);
	void	HD_L_Algorithm(TILE& temptile , TERRAIN_INFO& _terrain_info ,const int oriidx , const int isequenceidx);
	void	HD_LU_Algorithm(TILE& temptile , TERRAIN_INFO& _terrain_info ,const int oriidx , const int isequenceidx);
	void	HD_RU_Algorithm(TILE& temptile , TERRAIN_INFO& _terrain_info ,const int oriidx , const int isequenceidx);
	void	HD_R_Algorithm(TILE& temptile , TERRAIN_INFO& _terrain_info ,const int oriidx , const int isequenceidx);
	void	HD_RD_Algorithm(TILE& temptile , TERRAIN_INFO& _terrain_info ,const int oriidx , const int isequenceidx);
	void	HD_LD_Algorithm(TILE& temptile , TERRAIN_INFO& _terrain_info ,const int oriidx , const int isequenceidx);
public:
	void	InitOverlap(const int groupid);
	void	Overlap_GroupArea(const int irow ,const int icol ,const int startidx ,const int group_id);
public:
	CHighDirt_Brush(void);
	virtual ~CHighDirt_Brush(void);
};
