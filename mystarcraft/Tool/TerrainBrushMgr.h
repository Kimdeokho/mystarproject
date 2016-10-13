#pragma once

#include "Include.h"
class CMyBrush;
class CTerrainBrushMgr
{
public:
	DECLARE_SINGLETON(CTerrainBrushMgr)
public:
	HRESULT	TerrainCheck(void);
	HRESULT	Initialize(void);
	HRESULT	InitBrush(const CString&	_str);
	void	Release(void);
public:
	void	High_DirtBrush(const int idx);
public:
	int		get_sqindex(void);
private:
	int			m_TerrainMask[8];
	int			m_curFloor;
	int			m_curTerrainID;
	CListBox*	m_TerrainListBox;
	CMyBrush*	m_Brush[FLOOR_END];
public:
	CTerrainBrushMgr(void);
	~CTerrainBrushMgr(void);
};
