#include "StdAfx.h"
#include "LowerGround_Brush.h"

#include "TerrainBrushMgr.h"
#include "TileMgr.h"
#include "LG_Group_FLAT.h"
#include "LG_Group_LD.h"
#include "LG_Group_LEdge.h"
#include "LG_Group_LU.h"
#include "LG_Group_RD.h"
#include "LG_Group_REdge.h"
#include "LG_Group_RU.h"

CLowerGround_Brush::CLowerGround_Brush(void)
{

	for(int i = 0; i < GROUP_END; ++i)
		m_pGroup[i] = NULL;

	m_pGroup[GROUP_FLAT] = new CLG_Group_FLAT;
	m_pGroup[GROUP_LEDGE] = new CLG_Group_LEdge;
	m_pGroup[GROUP_LU] = new CLG_Group_LU;
	m_pGroup[GROUP_RU] = new CLG_Group_RU;
	m_pGroup[GROUP_REDGE] = new CLG_Group_REdge;
	m_pGroup[GROUP_RD] = new CLG_Group_RD;
	m_pGroup[GROUP_LD] = new CLG_Group_LD;

}

CLowerGround_Brush::~CLowerGround_Brush(void)
{
	for(int i = 0; i < GROUP_END; ++i)
		Safe_Delete(m_pGroup[i]);
}

void CLowerGround_Brush::BrushPaint()
{
	int idx = CTerrainBrushMgr::GetInstance()->get_sqindex();

	m_pGroup[GROUP_LEDGE]->MakeTerrain_Group(idx - SQ_TILECNTX - 6);

	m_pGroup[GROUP_FLAT]->MakeTerrain_Group(idx - 2 , 1 , 2);
	m_pGroup[GROUP_FLAT]->MakeTerrain_Group(idx , 1 , 2);
	m_pGroup[GROUP_FLAT]->MakeTerrain_Group(idx - 3 + SQ_TILECNTX , 1 , 6);
	m_pGroup[GROUP_FLAT]->MakeTerrain_Group(idx - 1 + SQ_TILECNTX*2 , 1 , 2 );

	m_pGroup[GROUP_RD]->MakeTerrain_Group(idx - SQ_TILECNTX*3 - 2);
	m_pGroup[GROUP_RD]->MakeTerrain_Group(idx - SQ_TILECNTX*2 - 4);

	m_pGroup[GROUP_LD]->MakeTerrain_Group(idx - SQ_TILECNTX*3);
	m_pGroup[GROUP_LD]->MakeTerrain_Group(idx - SQ_TILECNTX*2 + 2);

	m_pGroup[GROUP_RU]->MakeTerrain_Group(idx + SQ_TILECNTX - 2);
	m_pGroup[GROUP_RU]->MakeTerrain_Group(idx - 4);

	m_pGroup[GROUP_LU]->MakeTerrain_Group(idx + SQ_TILECNTX );
	m_pGroup[GROUP_LU]->MakeTerrain_Group(idx + 2);

	m_pGroup[GROUP_REDGE]->MakeTerrain_Group(idx - SQ_TILECNTX + 4);



	for(int i = 0; i < GROUP_END; ++i)
	{
		if( NULL == m_pGroup[i])
			continue;

		m_pGroup[i]->UpdateTerrain();
	}

	CTileMgr::GetInstance()->TileOption_Update();
}
