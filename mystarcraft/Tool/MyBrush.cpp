#include "StdAfx.h"
#include "MyBrush.h"
#include "TileMgr.h"

#include "Terrain_Group.h"
CMyBrush::CMyBrush(void):m_curFloor(1)
{
}

CMyBrush::~CMyBrush(void)
{
}

void CMyBrush::SetTerrain_ID(const int terrain_id)
{

	for(int i = 0; i < GROUP_END; ++i)
	{
		if(NULL == m_pGroup[i])
			continue;

		if(TERRAIN_DIRT == terrain_id)
		{
			m_pGroup[i]->SetTerrain_ID(TERRAIN_HIGHDIRT , TERRAIN_DIRT);
		}
		else if(TERRAIN_HIGHDIRT == terrain_id)
		{
			m_pGroup[i]->SetTerrain_ID(TERRAIN_HIGHDIRT , TERRAIN_HIGHDIRT);
		}
		//물이면 물다운, 업
	}
}
