#include "StdAfx.h"
#include "HighDirt_Brush.h"
#include "TileMgr.h"
#include "TerrainBrushMgr.h"
#include "HD_Group_L.h"
#include "HD_Group_LU.h"
#include "HD_Group_RU.h"
#include "HD_Group_R.h"
#include "HD_Group_RD.h"
#include "HD_Group_LD.h"

CHighDirt_Brush::CHighDirt_Brush(void)
{
	m_curFloor = 2;
	m_curTerrainID = TERRAIN_HIGHDIRT;
	memset(m_bOverlap , 0 , sizeof(m_bOverlap) );
	memset(&m_terraininfo , 0 , sizeof(TERRAIN_INFO));
	memset(&m_temptile , 0 , sizeof(TILE));

	for(int i = 0; i < GROUP_END; ++i)
		m_pGroup[i] = NULL;

	m_pGroup[GROUP_L] = new CHD_Group_L;
	m_pGroup[GROUP_LU] = new CHD_Group_LU;
	m_pGroup[GROUP_RU] = new CHD_Group_RU;
	m_pGroup[GROUP_R] = new CHD_Group_R;
	m_pGroup[GROUP_RD] = new CHD_Group_RD;
	m_pGroup[GROUP_LD] = new CHD_Group_LD;



	for(int i = 0; i < GROUP_END; ++i)
	{
		if(NULL == m_pGroup[i])
			continue;
		m_pGroup[i]->SetBrush(this);
	}
}

CHighDirt_Brush::~CHighDirt_Brush(void)
{
	for(int i = 0; i < GROUP_END; ++i)
		Safe_Delete(m_pGroup[i]);
}
void CHighDirt_Brush::InitOverlap(const int groupid)
{
	memset(m_bOverlap[groupid] , 0 , sizeof(m_bOverlap[groupid]) );
}
void CHighDirt_Brush::BrushPaint()
{
	int idx = CTerrainBrushMgr::GetInstance()->get_sqindex();

	Overlap_GroupArea(3, 2, idx - 126 , GROUP_R);
	Overlap_GroupArea(3, 2, idx - 132 , GROUP_L);
	Overlap_GroupArea(3, 2, idx , GROUP_RD);
	Overlap_GroupArea(3, 2, idx - 2 , GROUP_LD);
	Overlap_GroupArea(2, 2, idx - 256 , GROUP_RU);
	Overlap_GroupArea(2, 2, idx - 258 , GROUP_LU);

	m_pGroup[GROUP_L]->MakeTerrain_Group(idx - 132);
	m_pGroup[GROUP_LU]->MakeTerrain_Group(idx - 258);
	m_pGroup[GROUP_RU]->MakeTerrain_Group(idx - 256);
	m_pGroup[GROUP_R]->MakeTerrain_Group(idx - 126);
	m_pGroup[GROUP_RD]->MakeTerrain_Group(idx);
	m_pGroup[GROUP_LD]->MakeTerrain_Group(idx - 2);

	//LeftArea(idx - 132);
	//LeftUpArea(idx - 258) ;
	//RightUpArea(idx - 256);
	//RightArea(idx - 126);
	//RightDown(idx);
	//LeftDown(idx - 2);



	CTileMgr::GetInstance()->TileOption_Update();

	for(int i = 0; i < GROUP_END; ++i)
		InitOverlap(i);
}
void CHighDirt_Brush::SetTerrainInfo( BYTE terrainid ,BYTE bygroup_id ,BYTE bysequence , BYTE bysortlv)
{
	m_terraininfo.byGroup_ID = bygroup_id;
	m_terraininfo.byGroup_sequence = bysequence;
	m_terraininfo.bysortLV = bysortlv;
}

void CHighDirt_Brush::SetTileInfo(BYTE option , BYTE byfloor)
{
	m_temptile.byOption = option;
	m_temptile.byFloor = byfloor;
}
