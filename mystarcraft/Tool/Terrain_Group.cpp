#include "StdAfx.h"
#include "Terrain_Group.h"
#include "TileMgr.h"
CTerrain_Group::CTerrain_Group(void)
{
	m_start_bottomidx = 0;
	m_bTerrainDelete = true;
	m_flat_id = TERRAIN_HIGHDIRT;

	m_Flag = 1;
}

CTerrain_Group::~CTerrain_Group(void)
{
}

void CTerrain_Group::SetTerrainInfo(BYTE byterrain_id ,BYTE bygroup_id ,BYTE bysequence , BYTE bysortlv)
{
}
void CTerrain_Group::SetTerrainInfo(int idx , BYTE byterrain_id ,BYTE bygroup_id ,BYTE bysequence , BYTE bysortlv , bool bdelete)
{
	TERRAIN_UPDATE	temp;

	temp.idx = idx;
	temp.terrain_info.byTerrain_ID = byterrain_id;
	temp.terrain_info.byGroup_ID = bygroup_id;
	temp.terrain_info.byGroup_sequence = bysequence;
	temp.terrain_info.bysortLV = bysortlv;
	temp.bdelete = bdelete;

	m_TerrainUpdateList.push_back(temp);
}
void CTerrain_Group::UpdateTerrain(void)
{
	list<TERRAIN_UPDATE>::iterator iter = m_TerrainUpdateList.begin();
	list<TERRAIN_UPDATE>::iterator iter_end = m_TerrainUpdateList.end();

	for( ; iter != iter_end; ++iter)
	{
		CTileMgr::GetInstance()->SetTerrain( (*iter).idx , (*iter).terrain_info , (*iter).bdelete );
	}

	m_TerrainUpdateList.clear();
}
void CTerrain_Group::Make_LU_Terrain(const int idx)
{
	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			if(i*2+j != 3)
				SetTerrainInfo(idx + (i*SQ_TILECNTX+j) ,m_terrain_id, GROUP_LU  , i*2+j , 1 , true );
			else
				SetTerrainInfo(idx + (i*SQ_TILECNTX+j) ,m_terrain_id , GROUP_LU , i*2+j , 0 , true );
		}
	}
}
void CTerrain_Group::Make_RU_Terrain(const int idx)
{
	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			if(i*2+j != 2)
				SetTerrainInfo(idx + (i*SQ_TILECNTX+j) ,m_terrain_id, GROUP_RU  , i*2+j , 1 , true );
			else
				SetTerrainInfo(idx + (i*SQ_TILECNTX+j) ,m_terrain_id , GROUP_RU , i*2+j , 0 , true );
		}
	}
}

void CTerrain_Group::Make_L_Terrain(const int idx , bool bdelete , bool bdelete2)
{
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			if(i*2+j <= 3)
				SetTerrainInfo(idx + (i*SQ_TILECNTX+j) ,m_terrain_id, GROUP_L  , i*2+j , 1 , bdelete );
			else
				SetTerrainInfo(idx + (i*SQ_TILECNTX+j) ,m_terrain_id, GROUP_L  , i*2+j , 0 , bdelete2 );
		}
	}
}
void CTerrain_Group::Make_R_Terrain(const int idx , bool bdelete , bool bdelete2)
{
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			if(i*2+j <= 3)
				SetTerrainInfo(idx + (i*SQ_TILECNTX+j) ,m_terrain_id, GROUP_R  , i*2+j , 1 , bdelete );
			else
				SetTerrainInfo(idx + (i*SQ_TILECNTX+j) ,m_terrain_id, GROUP_R  , i*2+j , 0 , bdelete2 );
		}
	}
}
void CTerrain_Group::Make_RD_Terrain(const int idx , bool bdelete , bool bdelete2)
{
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			if(i*2+j <= 3)
				SetTerrainInfo(idx + (i*SQ_TILECNTX+j) ,m_terrain_id, GROUP_RD  , i*2+j , 0 , bdelete );
			else
				SetTerrainInfo(idx + (i*SQ_TILECNTX+j) ,m_terrain_id, GROUP_RD  , i*2+j , 0 , bdelete2 );
		}
	}
}
void CTerrain_Group::Make_LD_Terrain(const int idx , bool bdelete , bool bdelete2)
{
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			if(i*2+j <= 3)
				SetTerrainInfo(idx + (i*SQ_TILECNTX+j) ,m_terrain_id, GROUP_LD  , i*2+j , 0 , bdelete );
			else
				SetTerrainInfo(idx + (i*SQ_TILECNTX+j) ,m_terrain_id, GROUP_LD  , i*2+j , 0 , bdelete2 );
		}
	}
}
void CTerrain_Group::Make_FLAT_Terrain(const int idx , int irow , int icol  ,bool bdelete/*true*/)
{
	for(int i = 0; i < irow; ++i)
	{
		for(int j = 0; j < icol; ++j)
		{
			//if(TERRAIN_HIGHDIRT == m_flat_id)
				//SetTerrainInfo(idx + (i*SQ_TILECNTX+j), m_flat_id , GROUP_FLAT , rand()%2 , 1 ,bdelete);
			//else
				SetTerrainInfo(idx + (i*SQ_TILECNTX+j), m_flat_id , GROUP_FLAT , rand()%2 , 0 ,bdelete);
		}
	}
}
void CTerrain_Group::Make_REdge(const int idx)
{
	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			if(i*2+j <= 1)
				SetTerrainInfo(idx + (i*SQ_TILECNTX+j) , m_terrain_id , GROUP_LD , i*2+j , 0, true);
			else if(i*2+j == 2)
			{
				SetTerrainInfo(idx + (i*SQ_TILECNTX+j) , m_terrain_id , GROUP_LD , i*2+j , 0, true);
				SetTerrainInfo(idx + (i*SQ_TILECNTX+j) , m_terrain_id , GROUP_LU , i*2+j , 1, true);
			}
			else
				SetTerrainInfo(idx + (i*SQ_TILECNTX+j) , m_terrain_id , GROUP_LU , i*2+j , 0, true);
		}
	}
}
void CTerrain_Group::Make_LEdge(const int idx)
{
	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			if(i*2+j <= 1)
				SetTerrainInfo(idx + (i*SQ_TILECNTX+j) , m_terrain_id , GROUP_RD , i*2+j , 0, true);
			else if(i*2+j == 2)
				SetTerrainInfo(idx + (i*SQ_TILECNTX+j) , m_terrain_id , GROUP_RU , i*2+j , 0, true);
			else
			{
				SetTerrainInfo(idx + (i*SQ_TILECNTX+j) , m_terrain_id , GROUP_RD , i*2+j , 0, true);
				SetTerrainInfo(idx + (i*SQ_TILECNTX+j) , m_terrain_id , GROUP_RU , i*2+j , 1, true);
			}
		}
	}
}
void CTerrain_Group::LeftEdge_Algorithm(void)
{
	const TERRAIN_INFO* Edge1 = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx - 2);
	const TERRAIN_INFO* Edge2 = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx - 1);
	const TERRAIN_INFO* Edge3 = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx - 2 + SQ_TILECNTX);
	const TERRAIN_INFO* Edge4 = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx - 1 + SQ_TILECNTX);

	if(GROUP_RD == Edge2->byGroup_ID && GROUP_RU == Edge4->byGroup_ID)
	{
		if(GROUP_RU == Edge3->byGroup_ID)
		{
			if(GROUP_RD == Edge1->byGroup_ID)
			{
				Make_FLAT_Terrain(m_startidx - 2 , 2 , 2);
			}
			else if(GROUP_LU == Edge1->byGroup_ID)
			{
				SetTerrainInfo(m_startidx - 2, m_terrain_id , GROUP_LU , 2 , 1 , true);
				SetTerrainInfo(m_startidx - 1, m_terrain_id , GROUP_LU , 3 , 0 , true);

				Make_FLAT_Terrain(m_startidx - 2  + SQ_TILECNTX , 1 , 2);
			}
		}
		else if(GROUP_LD == Edge3->byGroup_ID)
		{
			if(GROUP_RD == Edge1->byGroup_ID)
			{
				Make_FLAT_Terrain(m_startidx - 2  , 1 , 2);

				SetTerrainInfo(m_startidx - 2 + SQ_TILECNTX , m_terrain_id , GROUP_LD , 0 , 0 , true);
				SetTerrainInfo(m_startidx - 1 + SQ_TILECNTX , m_terrain_id , GROUP_LD , 1 , 0 , true);
			}
			else if(GROUP_LU == Edge1->byGroup_ID)
			{
				SetTerrainInfo(m_startidx - 2  , m_terrain_id , GROUP_LU , 2 ,1 , true);
				SetTerrainInfo(m_startidx - 1  , m_terrain_id , GROUP_LU , 3 ,0 , true);

				SetTerrainInfo(m_startidx - 2 + SQ_TILECNTX , m_terrain_id , GROUP_LD , 0 , 0 , true);
				SetTerrainInfo(m_startidx - 1 + SQ_TILECNTX , m_terrain_id , GROUP_LD , 1 , 0 , true);
			}
		}
	}
}

void CTerrain_Group::RightEdge_Algorithm(void)
{
	const TERRAIN_INFO* Edge1 = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + 2);
	const TERRAIN_INFO* Edge2 = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + 3);
	const TERRAIN_INFO* Edge3 = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + 2 + SQ_TILECNTX);
	const TERRAIN_INFO* Edge4 = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + 3 + SQ_TILECNTX);

	if(GROUP_LD == Edge1->byGroup_ID && GROUP_LU == Edge3->byGroup_ID)
	{
		if(GROUP_LU == Edge4->byGroup_ID)
		{
			if(GROUP_LD == Edge2->byGroup_ID)
			{
				Make_FLAT_Terrain(m_startidx + 2 , 2, 2);
			}
			else if(GROUP_RU == Edge2->byGroup_ID)
			{
				SetTerrainInfo(m_startidx + 2 , m_terrain_id , GROUP_RU , 2, 0 ,true);
				SetTerrainInfo(m_startidx + 3 , m_terrain_id , GROUP_RU , 3, 1 ,true);
				Make_FLAT_Terrain(m_startidx + 2 + SQ_TILECNTX , 1, 2);
			}
		}
		else if(GROUP_RD == Edge4->byGroup_ID)
		{
			if(GROUP_LD == Edge2->byGroup_ID)
			{
				Make_FLAT_Terrain(m_startidx + 2 , 1, 2);
				SetTerrainInfo(m_startidx + 2 + SQ_TILECNTX , m_terrain_id , GROUP_RD , 0, 0 ,true);
				SetTerrainInfo(m_startidx + 3 + SQ_TILECNTX, m_terrain_id , GROUP_RD , 1, 0 ,true);
			}
			else if(GROUP_RU == Edge2->byGroup_ID)
			{
				SetTerrainInfo(m_startidx + 2 , m_terrain_id , GROUP_RU , 2, 0 ,true);
				SetTerrainInfo(m_startidx + 3 , m_terrain_id , GROUP_RU , 3, 1 ,true);
				SetTerrainInfo(m_startidx + 2 + SQ_TILECNTX , m_terrain_id , GROUP_RD , 0, 0 ,true);
				SetTerrainInfo(m_startidx + 3 + SQ_TILECNTX , m_terrain_id , GROUP_RD , 1, 0 ,true);
			}
		}
	}
}

void CTerrain_Group::SetTerrain_ID(const int terrain_id , const int flat_id)
{
	m_terrain_id = terrain_id;
	m_flat_id = flat_id;
}
