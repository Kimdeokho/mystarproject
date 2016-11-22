#include "StdAfx.h"
#include "LG_Group_REdge.h"

#include "TileMgr.h"
#include "TerrainBrushMgr.h"
#include "MyBrush.h"

CLG_Group_REdge::CLG_Group_REdge(void)
{
}

CLG_Group_REdge::~CLG_Group_REdge(void)
{
}

void CLG_Group_REdge::MakeTerrain_Group(const int istartidx)
{
	if(istartidx < 0 || istartidx > SQ_TILECNTX*SQ_TILECNTY)
		return;

	m_startidx = istartidx;
	Group_REdge_Algorithm();
}
void CLG_Group_REdge::Group_REdge_Algorithm()
{
	Hill_Algorithm(m_startidx);

	m_oriTerrainInfo = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx);

	if(GROUP_L == m_oriTerrainInfo->byGroup_ID)
	{
		OverlapGroup_L();
	}
	else if(GROUP_LU == m_oriTerrainInfo->byGroup_ID)
	{
		OverlapGroup_LU();
	}
	else if(GROUP_RU == m_oriTerrainInfo->byGroup_ID)
	{
		OverlapGroup_RU();
	}
	else if(GROUP_R == m_oriTerrainInfo->byGroup_ID)
	{
		OverlapGroup_R();
	}
	else if(GROUP_RD == m_oriTerrainInfo->byGroup_ID)
	{
		OverlapGroup_RD();
	}
	else if(GROUP_LD == m_oriTerrainInfo->byGroup_ID)
	{
		OverlapGroup_LD();
	}
	else if(GROUP_FLAT == m_oriTerrainInfo->byGroup_ID)
	{
		const TERRAIN_INFO* DownSpace = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + SQ_TILECNTX);

		if(TERRAIN_HIGHDIRT == m_oriTerrainInfo->byTerrain_ID)
			Make_REdge(m_startidx);
		else if(TERRAIN_DIRT == m_oriTerrainInfo->byTerrain_ID &&
			TERRAIN_WATER == m_terrain_id)
			Make_REdge(m_startidx);

		if(GROUP_RD == DownSpace->byGroup_ID)
		{
			SetTerrainInfo(m_startidx + SQ_TILECNTX + 1, m_terrain_id , GROUP_RD , 1 , 0 , true);
		}
	}
}
void CLG_Group_REdge::OverlapGroup_L(void)
{
	if( 0 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_L_0();
	}
	else if(1 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_L_1();
	}
	else if(2 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_L_2();
	}
	else if(3 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_L_3();
	}
	else if(4 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_L_4();
	}
	else if(5 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_L_5();
	}
}
void CLG_Group_REdge::OverlapGroup_LU(void)
{
	if( 0 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_LU_0();
	}
	else if(1 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_LU_1();
	}
	else if(2 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_LU_2();
	}
	else if(3 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_LU_3();
	}
}

void CLG_Group_REdge::OverlapGroup_RU(void)
{
	if( 0 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_RU_0();
	}
	else if(1 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_RU_1();
	}
	else if(2 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_RU_2();
	}
	else if(3 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_RU_3();
	}
}

void CLG_Group_REdge::OverlapGroup_R(void)
{
	if( 0 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_R_0();
	}
	else if(1 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_R_1();
	}
	else if(2 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_R_2();
	}
	else if(3 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_R_3();
	}
	else if(4 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_R_4();
	}
	else if(5 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_R_5();
	}
}

void CLG_Group_REdge::OverlapGroup_RD(void)
{
	if( 0 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_RD_0();
	}
	else if(1 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_RD_1();
	}
	else if(2 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_RD_2();
	}
	else if(3 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_RD_3();
	}
	else if(4 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_RD_4();
	}
	else if(5 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_RD_5();
	}
}

void CLG_Group_REdge::OverlapGroup_LD(void)
{
	if( 0 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_LD_0();
	}
	else if(1 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_LD_1();
	}
	else if(2 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_LD_2();
	}
	else if(3 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_LD_3();
	}
	else if(4 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_LD_4();
	}
	else if(5 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_LD_5();
	}
}

void CLG_Group_REdge::OverlapSequence_L_0(void)
{
}

void CLG_Group_REdge::OverlapSequence_L_1(void)
{
}

void CLG_Group_REdge::OverlapSequence_L_2(void)
{
}

void CLG_Group_REdge::OverlapSequence_L_3(void)
{
}

void CLG_Group_REdge::OverlapSequence_L_4(void)
{
}

void CLG_Group_REdge::OverlapSequence_L_5(void)
{
}



void CLG_Group_REdge::OverlapSequence_LU_0(void)
{
}

void CLG_Group_REdge::OverlapSequence_LU_1(void)
{
}

void CLG_Group_REdge::OverlapSequence_LU_2(void)
{
}

void CLG_Group_REdge::OverlapSequence_LU_3(void)
{
}
void CLG_Group_REdge::OverlapSequence_RU_0(void)
{
}

void CLG_Group_REdge::OverlapSequence_RU_1(void)
{
}

void CLG_Group_REdge::OverlapSequence_RU_2(void)
{
	const TERRAIN_INFO* DownSpace = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + SQ_TILECNTX);

	if(GROUP_RD == DownSpace->byGroup_ID)
	{
		SetTerrainInfo(m_startidx , m_terrain_id , GROUP_LD , 0 , 0 , true);
		SetTerrainInfo(m_startidx + SQ_TILECNTX , m_terrain_id , GROUP_LD , 2 , 0 , true);
		SetTerrainInfo(m_startidx + SQ_TILECNTX , m_terrain_id , GROUP_LU , 2 , 1 , true);
	}
	else
	{
		SetTerrainInfo(m_startidx , m_terrain_id , GROUP_LD , 0 , 0 , true);
		SetTerrainInfo(m_startidx + SQ_TILECNTX , m_terrain_id , GROUP_LD , 2 , 0 , true);
		SetTerrainInfo(m_startidx + SQ_TILECNTX , m_terrain_id , GROUP_LU , 2 , 1 , true);
		SetTerrainInfo(m_startidx + SQ_TILECNTX + 1, m_terrain_id , GROUP_LU , 3 , 0 , true);
	}
}

void CLG_Group_REdge::OverlapSequence_RU_3(void)
{
}

void CLG_Group_REdge::OverlapSequence_RD_0(void)
{
}

void CLG_Group_REdge::OverlapSequence_RD_1(void)
{
}

void CLG_Group_REdge::OverlapSequence_RD_2(void)
{
}

void CLG_Group_REdge::OverlapSequence_RD_3(void)
{
}

void CLG_Group_REdge::OverlapSequence_RD_4(void)
{
}

void CLG_Group_REdge::OverlapSequence_RD_5(void)
{
}

void CLG_Group_REdge::OverlapSequence_R_0(void)
{
}

void CLG_Group_REdge::OverlapSequence_R_1(void)
{
}

void CLG_Group_REdge::OverlapSequence_R_2(void)
{
}

void CLG_Group_REdge::OverlapSequence_R_3(void)
{
}

void CLG_Group_REdge::OverlapSequence_R_4(void)
{
}

void CLG_Group_REdge::OverlapSequence_R_5(void)
{
}

void CLG_Group_REdge::OverlapSequence_LD_0(void)
{
}

void CLG_Group_REdge::OverlapSequence_LD_1(void)
{
}

void CLG_Group_REdge::OverlapSequence_LD_2(void)
{
}

void CLG_Group_REdge::OverlapSequence_LD_3(void)
{
}

void CLG_Group_REdge::OverlapSequence_LD_4(void)
{
}

void CLG_Group_REdge::OverlapSequence_LD_5(void)
{
}
