#include "StdAfx.h"
#include "Rewind.h"
#include "TileMgr.h"

IMPLEMENT_SINGLETON(CRewind)
CRewind::CRewind(void)
{
	m_TerrainRewind.reserve(SQ_TILECNTX*SQ_TILECNTY);
	m_TerrainRewind.resize(SQ_TILECNTX*SQ_TILECNTY);
}

CRewind::~CRewind(void)
{
	m_curidxStack.clear();
	m_TileIdxList.clear();
	m_TerrainRewind.clear();
}
void CRewind::InitStackTile(list<TERRAIN_INFO*> temp ,const int idx)
{
	list<TERRAIN_INFO>	stacklist;

	for(list<TERRAIN_INFO*>::iterator iter = temp.begin(); iter != temp.end(); ++iter)
	{
		TERRAIN_INFO terrain;
		terrain.byGroup_ID = (*iter)->byGroup_ID;
		terrain.byGroup_sequence = (*iter)->byGroup_sequence;
		terrain.bysortLV = (*iter)->bysortLV;
		terrain.byTerrain_ID = (*iter)->byTerrain_ID;

		stacklist.push_back(terrain);
	}

	m_TerrainRewind[idx].push_back(stacklist);
}
void CRewind::SetTerrainData(const list<TERRAIN_INFO*>&	templist , const int idx)
{
	TERRAIN_INFO*	ptemp = NULL;
	TERRAIN_INFO	terrain_info;
	TILE_INFO		tile_list;

	if( 2 == templist.size())
	{
		ptemp = templist.front();
		terrain_info.byGroup_ID = ptemp->byGroup_ID;
		terrain_info.byGroup_sequence = ptemp->byGroup_sequence;
		terrain_info.bysortLV = ptemp->bysortLV;
		terrain_info.byTerrain_ID = ptemp->byTerrain_ID;

		tile_list.push_back(terrain_info);

		ptemp = templist.back();
		terrain_info.byGroup_ID = ptemp->byGroup_ID;
		terrain_info.byGroup_sequence = ptemp->byGroup_sequence;
		terrain_info.bysortLV = ptemp->bysortLV;
		terrain_info.byTerrain_ID = ptemp->byTerrain_ID;

		tile_list.push_back(terrain_info);

	}
	else
	{
		ptemp = templist.back();
		terrain_info.byGroup_ID = ptemp->byGroup_ID;
		terrain_info.byGroup_sequence = ptemp->byGroup_sequence;
		terrain_info.bysortLV = ptemp->bysortLV;
		terrain_info.byTerrain_ID = ptemp->byTerrain_ID;

		tile_list.push_back(terrain_info);
	}

	if(m_TerrainRewind[idx].size() <= TERRAIN_STACKSIZE)
	{
		m_TerrainRewind[idx].push_back(tile_list);
	}
	else
	{
		m_TerrainRewind[idx].pop_front();
		m_TerrainRewind[idx].push_back(tile_list);
	}


	m_curidxStack.push_back(idx); 
}

void CRewind::Push_Begin(void)
{
	m_curidxStack.clear();
}

void CRewind::Push_end(void)
{
	if(m_TileIdxList.size() <= TERRAIN_STACKSIZE)
		m_TileIdxList.push_back(m_curidxStack);
	else
	{
		m_TileIdxList.pop_front();
		m_TileIdxList.push_back(m_curidxStack);
	}
}
void CRewind::StackRewind(void)
{
	if(m_TileIdxList.empty())
		return;

	list<int> templist = m_TileIdxList.back();

	if(!templist.empty())
	{
		list<int>::iterator iter = templist.begin();
		list<int>::iterator iter_end = templist.end();

		int idx = 0;
		for(; iter != iter_end; ++iter)
		{
			idx = (*iter);
			if(m_TerrainRewind[idx].size() > 1)
			{
				m_TerrainRewind[idx].pop_back();
				CTileMgr::GetInstance()->SetBeforeTile(m_TerrainRewind[idx].back() , idx);
			}
		}
	}

	m_TileIdxList.pop_back();

	CTileMgr::GetInstance()->TileOption_Update();
}