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

void CMyBrush::SetTerrain_ID(const int terrain_id ,const int icase)
{

	for(int i = 0; i < GROUP_END; ++i)
	{
		if(NULL == m_pGroup[i])
			continue;

		if(TERRAIN_DIRT == terrain_id)
		{
			if( -1 == icase)
				m_pGroup[i]->SetTerrain_ID(TERRAIN_HIGHDIRT , TERRAIN_DIRT);
			else if( 1 == icase)
				m_pGroup[i]->SetTerrain_ID(TERRAIN_WATER , TERRAIN_DIRT);
		}
		else if(TERRAIN_HIGHDIRT == terrain_id)
		{
			m_pGroup[i]->SetTerrain_ID(TERRAIN_HIGHDIRT , TERRAIN_HIGHDIRT);
		}
		else if(TERRAIN_WATER == terrain_id)
		{
			if( -1 == icase)
				m_pGroup[i]->SetTerrain_ID(TERRAIN_WATER , TERRAIN_WATER);
			else if( 1 == icase)
				m_pGroup[i]->SetTerrain_ID(TERRAIN_WATER , TERRAIN_DIRT);
		}
		//���̸� ���ٿ�, ��
	}
}
