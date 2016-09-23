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
	bool	Overlap_Group(const int irow ,const int icol ,const int startidx ,const int group_id);
private:
	int			m_TerrainMask[8];
	int			m_curFloor;
	int			m_curTerrainID;
	CListBox*	m_TerrainListBox;
	CMyBrush*	m_Brush[TERRAIN_END];
public:
	CTerrainBrushMgr(void);
	~CTerrainBrushMgr(void);
};
