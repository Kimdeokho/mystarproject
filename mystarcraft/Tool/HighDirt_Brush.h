#pragma once

#include "MyBrush.h"
class CHighDirt_Brush : public CMyBrush
{
private:
	bool	m_bOverlap[GROUP_END][GROUP_END];
public:
	virtual void BrushPaint();
	void	LeftArea(const int startidx);
	void	LeftUpArea(const int startidx);
	void	LeftDown(const int startidx);
	void	RightUpArea(const int startidx);
	void	RightArea(const int startidx);
	void	RightDown(const int startidx);
	void	HD_L_Algorithm(const int oriidx , const int isequenceidx);
	void	HD_LU_Algorithm(const int oriidx , const int isequenceidx);
	void	HD_RU_Algorithm(const int oriidx , const int isequenceidx);
	void	HD_R_Algorithm(const int oriidx , const int isequenceidx);
	void	HD_RD_Algorithm(const int oriidx , const int isequenceidx);
	void	HD_LD_Algorithm(const int oriidx , const int isequenceidx);
public:
	void	InitOverlap(const int groupid);
	void	Overlap_GroupArea(const int irow ,const int icol ,const int startidx ,const int group_id);
	void	SetTerrainInfo(BYTE	byterrain_id ,BYTE bygroup_id ,BYTE bysequence , BYTE bysortlv);
	void	SetTileInfo(BYTE option , BYTE byfloor);
public:
	CHighDirt_Brush(void);
	virtual ~CHighDirt_Brush(void);
};
