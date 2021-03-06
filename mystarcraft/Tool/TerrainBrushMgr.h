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
	int		FloorCheck(void);

	void	Release(void);
public:
	void	High_DirtBrush(const int idx);
public:
	int		getrb_to_sqindex(void);
private:
	int			m_TerrainMask[8];
	int			m_curFloor;
	int			m_curTerrainID;
	CListBox*	m_TerrainListBox;
	CMyBrush*	m_Brush[FLOOR_END];

	int			m_oldIdx;
public:
	CTerrainBrushMgr(void);
	~CTerrainBrushMgr(void);
};
