#pragma once

#include "Include.h"
class CTerrainBrush
{
public:
	DECLARE_SINGLETON(CTerrainBrush)
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
private:
	int			m_TerrainMask[8];
	int			m_curFloor;
	int			m_curTerrainID;
	CListBox*	m_TerrainListBox;
public:
	CTerrainBrush(void);
	~CTerrainBrush(void);
};
