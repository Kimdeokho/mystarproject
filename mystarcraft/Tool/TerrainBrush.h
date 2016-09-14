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
	void	LeftArea(const int index);
private:
	int			m_TerrainMask[8];
	int			m_curFloor;
	int			m_curTerrainID;
	CListBox*	m_TerrainListBox;
public:
	CTerrainBrush(void);
	~CTerrainBrush(void);
};
