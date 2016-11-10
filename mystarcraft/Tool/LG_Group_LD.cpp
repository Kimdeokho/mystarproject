#include "StdAfx.h"
#include "LG_Group_LD.h"

#include "TileMgr.h"
#include "TerrainBrushMgr.h"
#include "MyBrush.h"
CLG_Group_LD::CLG_Group_LD(void)
{
}

CLG_Group_LD::~CLG_Group_LD(void)
{
}

void CLG_Group_LD::MakeTerrain_Group(const int istartidx ,const int iflag)
{
	m_Flag = iflag;
	m_startidx = istartidx;
	Group_LD_Algorithm();
}
void CLG_Group_LD::Group_LD_Algorithm()
{
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
		if(TERRAIN_HIGHDIRT == m_oriTerrainInfo->byTerrain_ID)
			Make_LD_Terrain(m_startidx, true , true);
		else
			Make_FLAT_Terrain(m_startidx , 3, 2);
	}
}
void CLG_Group_LD::OverlapGroup_L(void)
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
void CLG_Group_LD::OverlapGroup_LU(void)
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

void CLG_Group_LD::OverlapGroup_RU(void)
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

void CLG_Group_LD::OverlapGroup_R(void)
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

void CLG_Group_LD::OverlapGroup_RD(void)
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

void CLG_Group_LD::OverlapGroup_LD(void)
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

void CLG_Group_LD::OverlapSequence_L_0(void)
{
	Make_FLAT_Terrain(m_startidx + SQ_TILECNTX*2 , 1 , 2 );
	Make_L_Terrain(m_startidx);
}

void CLG_Group_LD::OverlapSequence_L_1(void)
{
}

void CLG_Group_LD::OverlapSequence_L_2(void)
{
}

void CLG_Group_LD::OverlapSequence_L_3(void)
{
}

void CLG_Group_LD::OverlapSequence_L_4(void)
{
	if(1 == m_Flag)
		Make_FLAT_Terrain(m_startidx + SQ_TILECNTX , 3, 2, true);
	else
		Make_FLAT_Terrain(m_startidx + SQ_TILECNTX , 2, 2, true);
}

void CLG_Group_LD::OverlapSequence_L_5(void)
{
}



void CLG_Group_LD::OverlapSequence_LU_0(void)
{
	const TERRAIN_INFO*	UpSpace = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx - SQ_TILECNTX);
	const TERRAIN_INFO*	DownSpace = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + SQ_TILECNTX);

	bool bdelete1 = true;
	bool bdelete2 = false;

	if(1 == m_Flag)
	{
		bdelete1 = true;
		bdelete2 = true;
	}
	else
	{
		bdelete1 = true;
		bdelete2 = false;
	}

	if(GROUP_LD == UpSpace->byGroup_ID)
	{
		SetTerrainInfo(m_startidx + SQ_TILECNTX, m_flat_id , GROUP_FLAT , 0 , 0, false);

		Make_L_Terrain(m_startidx , bdelete1 , bdelete2);
	}
	else if(GROUP_L == UpSpace->byGroup_ID)
	{
		Make_L_Terrain(m_startidx , bdelete1 , bdelete2);
	}
	else
	{
		Make_FLAT_Terrain(m_startidx + SQ_TILECNTX*2 , 1, 2 , true);
		Make_L_Terrain(m_startidx , bdelete1 , bdelete2);
	}
}

void CLG_Group_LD::OverlapSequence_LU_1(void)
{
}

void CLG_Group_LD::OverlapSequence_LU_2(void)
{
}

void CLG_Group_LD::OverlapSequence_LU_3(void)
{
}
void CLG_Group_LD::OverlapSequence_RU_0(void)
{
}

void CLG_Group_LD::OverlapSequence_RU_1(void)
{
}

void CLG_Group_LD::OverlapSequence_RU_2(void)
{
	SetTerrainInfo(m_startidx , m_terrain_id , GROUP_LD , 0 , 0 , true);
	SetTerrainInfo(m_startidx + SQ_TILECNTX, m_terrain_id , GROUP_LD , 2 , 0 , true);
	SetTerrainInfo(m_startidx + 1 + SQ_TILECNTX, m_terrain_id , GROUP_LD , 3 , 0 , true);
	if(1 == m_Flag)
	{
		SetTerrainInfo(m_startidx + SQ_TILECNTX*2 , m_terrain_id , GROUP_LD , 4 , 0 , true);
		SetTerrainInfo(m_startidx + 1 + SQ_TILECNTX*2 , m_terrain_id , GROUP_LD , 5 , 0 , true);
	}
	else
	{
		SetTerrainInfo(m_startidx + SQ_TILECNTX*2 , m_terrain_id , GROUP_LD , 4 , 0 , false);
		SetTerrainInfo(m_startidx + 1 + SQ_TILECNTX*2 , m_terrain_id , GROUP_LD , 5 , 0 , false);
	}
}

void CLG_Group_LD::OverlapSequence_RU_3(void)
{
}

void CLG_Group_LD::OverlapSequence_RD_0(void)
{
}

void CLG_Group_LD::OverlapSequence_RD_1(void)
{
}

void CLG_Group_LD::OverlapSequence_RD_2(void)
{
	SetTerrainInfo(m_startidx + SQ_TILECNTX , m_terrain_id , GROUP_RD , 4 , 0 , true);
	SetTerrainInfo(m_startidx + SQ_TILECNTX + 1, m_terrain_id , GROUP_RD , 5 , 0 , true);
	Make_FLAT_Terrain(m_startidx + SQ_TILECNTX*2 , 1 , 2);
}

void CLG_Group_LD::OverlapSequence_RD_3(void)
{
}

void CLG_Group_LD::OverlapSequence_RD_4(void)
{
}

void CLG_Group_LD::OverlapSequence_RD_5(void)
{
}

void CLG_Group_LD::OverlapSequence_R_0(void)
{
}

void CLG_Group_LD::OverlapSequence_R_1(void)
{
}

void CLG_Group_LD::OverlapSequence_R_2(void)
{
	Make_FLAT_Terrain(m_startidx + SQ_TILECNTX , 2, 2);
	Make_R_Terrain(m_startidx - SQ_TILECNTX);
}

void CLG_Group_LD::OverlapSequence_R_3(void)
{
}

void CLG_Group_LD::OverlapSequence_R_4(void)
{
}

void CLG_Group_LD::OverlapSequence_R_5(void)
{
}

void CLG_Group_LD::OverlapSequence_LD_0(void)
{
	const TERRAIN_INFO* DownSpace = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + SQ_TILECNTX);
	const TERRAIN_INFO* RightSpace = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + 1);

	Make_FLAT_Terrain(m_startidx , 3 , 2);

	if(GROUP_RU == RightSpace->byGroup_ID)
	{
		SetTerrainInfo(m_startidx , m_terrain_id , GROUP_LD , 0 , 0 , true);
		SetTerrainInfo(m_startidx + SQ_TILECNTX, m_terrain_id , GROUP_LD , 2 , 0 , true);
		SetTerrainInfo(m_startidx + SQ_TILECNTX + 1, m_terrain_id , GROUP_LD , 3 , 0 , true);
		SetTerrainInfo(m_startidx + SQ_TILECNTX*2, m_terrain_id , GROUP_LD , 4 , 0 , true);
		SetTerrainInfo(m_startidx + SQ_TILECNTX*2 + 1, m_terrain_id , GROUP_LD , 5 , 0 , true);

		SetTerrainInfo(m_startidx + 1, m_terrain_id , GROUP_RU , 3 , 1 , true);
	}
	else
	{
		Make_LD_Terrain(m_startidx , true ,false);
	}
}

void CLG_Group_LD::OverlapSequence_LD_1(void)
{
}

void CLG_Group_LD::OverlapSequence_LD_2(void)
{
}

void CLG_Group_LD::OverlapSequence_LD_3(void)
{
}

void CLG_Group_LD::OverlapSequence_LD_4(void)
{
	Make_FLAT_Terrain(m_startidx + SQ_TILECNTX , 2, 2, true);
}

void CLG_Group_LD::OverlapSequence_LD_5(void)
{
}
