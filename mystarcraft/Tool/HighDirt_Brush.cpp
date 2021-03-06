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
#include "HD_Group_Flat.h"

CHighDirt_Brush::CHighDirt_Brush(void)
{
	m_curFloor = 2;
	m_curTerrainID = TERRAIN_HIGHDIRT;

	for(int i = 0; i < GROUP_END; ++i)
		m_pGroup[i] = NULL;

	m_pGroup[GROUP_FLAT] = new CHD_Group_Flat;
	m_pGroup[GROUP_L] = new CHD_Group_L;
	m_pGroup[GROUP_LU] = new CHD_Group_LU;
	m_pGroup[GROUP_RU] = new CHD_Group_RU;
	m_pGroup[GROUP_R] = new CHD_Group_R;
	m_pGroup[GROUP_RD] = new CHD_Group_RD;
	m_pGroup[GROUP_LD] = new CHD_Group_LD;


}

CHighDirt_Brush::~CHighDirt_Brush(void)
{
	for(int i = 0; i < GROUP_END; ++i)
		Safe_Delete(m_pGroup[i]);
}
void CHighDirt_Brush::BrushPaint()
{
	int idx = CTerrainBrushMgr::GetInstance()->getrb_to_sqindex();


	m_pGroup[GROUP_L]->MakeTerrain_Group(idx - 132);
	m_pGroup[GROUP_LU]->MakeTerrain_Group(idx - 258);
	m_pGroup[GROUP_RU]->MakeTerrain_Group(idx - 256);
	m_pGroup[GROUP_R]->MakeTerrain_Group(idx - 126);
	m_pGroup[GROUP_RD]->MakeTerrain_Group(idx);
	m_pGroup[GROUP_LD]->MakeTerrain_Group(idx - 2);

	for(int i = 0; i < GROUP_END; ++i)
	{
		if( NULL == m_pGroup[i])
			continue;

		m_pGroup[i]->UpdateTerrain();
	}

	CTileMgr::GetInstance()->TileOption_Update();

}