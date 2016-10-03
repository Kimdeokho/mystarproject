#include "StdAfx.h"
#include "Terrain_Group.h"
#include "TileMgr.h"
CTerrain_Group::CTerrain_Group(void)
{
	m_bTerrainDelete = true;
	m_MyBrush = NULL;

	for(int i = 0; i < 8; ++i)
	{
		//m_TerrainInfo[i] = NULL;
	}
	m_breturn = false;
}

CTerrain_Group::~CTerrain_Group(void)
{
}

void CTerrain_Group::SetTerrainInfo(BYTE byterrain_id ,BYTE bygroup_id ,BYTE bysequence , BYTE bysortlv)
{
	m_sendTerrainInfo.byTerrain_ID = byterrain_id;
	m_sendTerrainInfo.byGroup_ID = bygroup_id;
	m_sendTerrainInfo.byGroup_sequence = bysequence;
	m_sendTerrainInfo.bysortLV = bysortlv;
}
void	CTerrain_Group::SetBrush(CMyBrush*	pbrush)
{
	m_MyBrush = pbrush;
}
void	CTerrain_Group::A_Search(void)
{
	m_breturn = true;

	m_TerrainGroup[SEARCH_LU] = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx - SQ_TILECNTX - 1)->byGroup_ID;
	m_TerrainGroup[SEARCH_UP] = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx - SQ_TILECNTX)->byGroup_ID;
	m_TerrainGroup[SEARCH_RU] = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx - SQ_TILECNTX + 1)->byGroup_ID;
	m_TerrainGroup[SEARCH_R] = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx + 1)->byGroup_ID;
	m_TerrainGroup[SEARCH_RD] = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx + SQ_TILECNTX + 1)->byGroup_ID;
	m_TerrainGroup[SEARCH_DOWN] = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx + SQ_TILECNTX)->byGroup_ID;
	m_TerrainGroup[SEARCH_LD] = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx + SQ_TILECNTX - 1)->byGroup_ID;
	m_TerrainGroup[SEARCH_L] = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx - 1)->byGroup_ID;
}