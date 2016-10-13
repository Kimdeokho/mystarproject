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
	int idx = CTerrainBrushMgr::GetInstance()->get_sqindex();

	//Overlap_GroupArea(3, 2, idx - 126 , GROUP_R);
	//Overlap_GroupArea(3, 2, idx - 132 , GROUP_L);
	//Overlap_GroupArea(3, 2, idx , GROUP_RD);
	//Overlap_GroupArea(3, 2, idx - 2 , GROUP_LD);
	//Overlap_GroupArea(2, 2, idx - 256 , GROUP_RU);
	//Overlap_GroupArea(2, 2, idx - 258 , GROUP_LU);

	m_pGroup[GROUP_L]->MakeTerrain_Group(idx - 132);
	m_pGroup[GROUP_LU]->MakeTerrain_Group(idx - 258);
	m_pGroup[GROUP_RU]->MakeTerrain_Group(idx - 256);
	m_pGroup[GROUP_R]->MakeTerrain_Group(idx - 126);
	m_pGroup[GROUP_RD]->MakeTerrain_Group(idx);
	m_pGroup[GROUP_LD]->MakeTerrain_Group(idx - 2);

	/*1층 내릴때*/
	//m_pGroup[GROUP_RD]->MakeTerrain_Group(idx - SQ_TILECNTX*3 - 2);
	//m_pGroup[GROUP_RD]->MakeTerrain_Group(idx - SQ_TILECNTX*2 - 4);

	//m_pGroup[GROUP_LD]->MakeTerrain_Group(idx - SQ_TILECNTX*3);
	//m_pGroup[GROUP_LD]->MakeTerrain_Group(idx - SQ_TILECNTX*2 + 2);

	//m_pGroup[GROUP_RU]->MakeTerrain_Group(idx + SQ_TILECNTX - 2);
	//m_pGroup[GROUP_RU]->MakeTerrain_Group(idx - 4);

	//m_pGroup[GROUP_LU]->MakeTerrain_Group(idx + SQ_TILECNTX );
	//m_pGroup[GROUP_LU]->MakeTerrain_Group(idx + 2);

	//((CHD_Group_Flat*)m_pGroup[GROUP_FLAT])->SetRowCol(1,4);
	//m_pGroup[GROUP_FLAT]->MakeTerrain_Group(idx - 2);
	/*1층 내릴때*/

	/*임시용*/
	//m_pGroup[GROUP_LU]->MakeTerrain_Group(idx - SQ_TILECNTX*4 - 2);
	//m_pGroup[GROUP_LU]->MakeTerrain_Group(idx -  SQ_TILECNTX*3 - 4);
	//m_pGroup[GROUP_LU]->MakeTerrain_Group(idx -  SQ_TILECNTX*2 - 6);
	//m_pGroup[GROUP_L]->MakeTerrain_Group(idx - 136);

	//m_pGroup[GROUP_LD]->MakeTerrain_Group(idx + SQ_TILECNTX*2 - 2);
	//m_pGroup[GROUP_LD]->MakeTerrain_Group(idx + SQ_TILECNTX - 4);
	//m_pGroup[GROUP_LD]->MakeTerrain_Group(idx  - 6);

	//((CHD_Group_Flat*)m_pGroup[GROUP_FLAT])->SetRowCol(4,2);
	//m_pGroup[GROUP_FLAT]->MakeTerrain_Group(idx - SQ_TILECNTX*2 - 2);
	//((CHD_Group_Flat*)m_pGroup[GROUP_FLAT])->SetRowCol(2,2);
	//m_pGroup[GROUP_FLAT]->MakeTerrain_Group(idx - SQ_TILECNTX*1 - 4);
	/**/

	//LeftArea(idx - 132);
	//LeftUpArea(idx - 258) ;
	//RightUpArea(idx - 256);
	//RightArea(idx - 126);
	//RightDown(idx);
	//LeftDown(idx - 2);


	for(int i = 0; i < GROUP_END; ++i)
	{
		if( NULL == m_pGroup[i])
			continue;

		m_pGroup[i]->UpdateTerrain();
	}

	CTileMgr::GetInstance()->TileOption_Update();

}