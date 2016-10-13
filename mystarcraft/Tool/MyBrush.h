#pragma once

#include "Include.h"
class CTerrain_Group;
class CMyBrush
{
protected:
	int		m_curFloor;
	int		m_curTerrainID;
	//TILE_TEMP			m_temptile; //����Ʈ�� �����ϰ��ִ� ����ü�� ����������� Ŭ������ ������ ��� ����
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
