#pragma once

#include "MyBrush.h"
class CHighDirt_Brush : public CMyBrush
{
private:
public:
	virtual void BrushPaint();
public:
	void	InitOverlap(const int groupid);
	void	SetTerrainInfo(BYTE	byterrain_id ,BYTE bygroup_id ,BYTE bysequence , BYTE bysortlv);
	void	SetTileInfo(BYTE option , BYTE byfloor);
public:
	CHighDirt_Brush(void);
	virtual ~CHighDirt_Brush(void);
};
