#pragma once

#include "Include.h"
class CTerrain_Group;
class CMyBrush
{
protected:
	int		m_curFloor;
	int		m_curTerrainID;
	TILE_TEMP			m_temptile; //리스트를 포함하고있는 구조체를 멤버변수로한 클래스를 삭제할 경우 에러
	TERRAIN_INFO	m_terraininfo;
	bool	m_bOverlap[GROUP_END][GROUP_END];
	CTerrain_Group*	m_pGroup[GROUP_END];
public:
	virtual void BrushPaint(){};
public:
	void Overlap_GroupArea(const int irow ,const int icol ,const int startidx ,const int group_id);
	bool GetOverlap_GroupArea(const BYTE group_id ,const BYTE overlap_id);
public:
	CMyBrush(void);
	virtual ~CMyBrush(void);
};
