#pragma once

#include "Include.h"

class CMyBrush;
class CTerrain_Group
{
public:
	virtual void	MakeTerrain_Group(const int istartidx){};
public:
	void	SetTerrainInfo(BYTE	byterrain_id ,BYTE bygroup_id ,BYTE bysequence , BYTE bysortlv);
	void	SetBrush(CMyBrush*	pbrush);
	void	A_Search(void);
protected:
	bool	m_bTerrainDelete;
	int		m_terrain_id;
	int		m_startidx;
	int		m_oriidx;
	int		m_cursequence;
	TERRAIN_INFO	m_sendTerrainInfo;
	const TERRAIN_INFO*	m_oriTerrainInfo;
	CMyBrush*  m_MyBrush;
	BYTE	m_TerrainGroup[8];
	bool	m_breturn;
public:
	CTerrain_Group(void);
	virtual ~CTerrain_Group(void);
};
