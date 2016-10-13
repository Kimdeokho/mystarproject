#pragma once

#include "Include.h"

typedef struct tagTerrainOption
{
	int		idx;
	TERRAIN_INFO	terrain_info;
	bool	bdelete;

}TERRAIN_UPDATE;

class CMyBrush;
class CTerrain_Group
{
public:
	virtual void	MakeTerrain_Group(const int istartidx){};
	virtual void	MakeTerrain_Group(const int istartidx , const int irow , const int icol){};
public:
	void	SetTerrainInfo(BYTE	byterrain_id ,BYTE bygroup_id ,BYTE bysequence , BYTE bysortlv);
	void	SetTerrainInfo(int idx , BYTE byterrain_id ,BYTE bygroup_id ,BYTE bysequence , BYTE bysortlv , bool bdelete);
	void	SetTerrain_ID(const int terrain_id);
	void	UpdateTerrain(void);
	void	Make_LU_Terrain(const int idx);
	void	Make_RU_Terrain(const int idx);
	void	Make_L_Terrain(const int idx , bool bdelete = true);
	void	Make_R_Terrain(const int idx , bool bdelete = true);
	void	Make_LD_Terrain(const int idx , bool bdelete = true);
	void	Make_RD_Terrain(const int idx , bool bdelete = true);
	void	Make_FLAT_Terrain(const int idx , int irow , int icol , int isequence = 0);
	void	Make_REdge(const int idx);
	void	Make_LEdge(const int idx);

	void	LeftEdge_Algorithm(void);
	void	RightEdge_Algorithm(void);
protected:
	bool	m_bTerrainDelete;
	int		m_terrain_id;
	int		m_startidx;
	int		m_start_bottomidx;
	//int		m_cursequence;
	const TERRAIN_INFO*	m_oriTerrainInfo;
	BYTE	m_TerrainGroup[8];

	list<TERRAIN_UPDATE>	m_TerrainUpdateList;
public:
	CTerrain_Group(void);
	virtual ~CTerrain_Group(void);
};
