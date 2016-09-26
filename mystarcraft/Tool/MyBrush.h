#pragma once

#include "Include.h"
class CMyBrush
{
protected:
	int		m_curFloor;
	int		m_curTerrainID;
	TILE_TEMP			m_temptile; //����Ʈ�� �����ϰ��ִ� ����ü�� ����������� Ŭ������ ������ ��� ����
	TERRAIN_INFO	m_terraininfo;

	//���δٸ� ������Ʈ ��������� ����
public:
	virtual void BrushPaint(){};
public:
	CMyBrush(void);
	virtual ~CMyBrush(void);
};
