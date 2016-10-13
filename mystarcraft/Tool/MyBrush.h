#pragma once

#include "Include.h"
class CTerrain_Group;
class CMyBrush
{
protected:
	int		m_curFloor;
	int		m_curTerrainID;
	//TILE_TEMP			m_temptile; //리스트를 포함하고있는 구조체를 멤버변수로한 클래스를 삭제할 경우 에러
	//TERRAIN_INFO	m_terraininfo;
	CTerrain_Group*	m_pGroup[GROUP_END];
public:
	virtual void BrushPaint(){};
public:
	void SetTerrain_ID(const int terrain_id);
public:
	CMyBrush(void);
	virtual ~CMyBrush(void);
};
