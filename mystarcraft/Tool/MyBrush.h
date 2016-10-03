#pragma once

#include "Include.h"
class CTerrain_Group;
class CMyBrush
{
protected:
	int		m_curFloor;
	int		m_curTerrainID;
	TILE_TEMP			m_temptile; //����Ʈ�� �����ϰ��ִ� ����ü�� ����������� Ŭ������ ������ ��� ����
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
