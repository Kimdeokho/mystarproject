#include "StdAfx.h"
#include "LG_Group_FLAT.h"

#include "TileMgr.h"
#include "TerrainBrushMgr.h"
#include "MyBrush.h"

CLG_Group_FLAT::CLG_Group_FLAT(void)
{
}

CLG_Group_FLAT::~CLG_Group_FLAT(void)
{
}

void CLG_Group_FLAT::MakeTerrain_Group(const int istartidx)
{
	if(istartidx < 0 || istartidx > SQ_TILECNTX*SQ_TILECNTY)
		return;

	Group_FLAT_Algorithm();
}

void CLG_Group_FLAT::MakeTerrain_Group(const int istartidx , const int irow , const int icol)
{
	if(istartidx < 0 || istartidx > SQ_TILECNTX*SQ_TILECNTY)
		return;

	m_startidx = istartidx;
	m_irow = irow;
	m_icol = icol;

	Group_FLAT_Algorithm();
}

void CLG_Group_FLAT::Group_FLAT_Algorithm()
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
		Make_FLAT_Terrain(m_startidx , m_irow , m_icol , false);
	}
}
void CLG_Group_FLAT::OverlapGroup_L(void)
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
void CLG_Group_FLAT::OverlapGroup_LU(void)
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

void CLG_Group_FLAT::OverlapGroup_RU(void)
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

void CLG_Group_FLAT::OverlapGroup_R(void)
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

void CLG_Group_FLAT::OverlapGroup_RD(void)
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

void CLG_Group_FLAT::OverlapGroup_LD(void)
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

void CLG_Group_FLAT::OverlapSequence_L_0(void)
{
	Make_FLAT_Terrain(m_startidx , 2 , 2 , true);
	Make_FLAT_Terrain(m_startidx + SQ_TILECNTX*2, 1 , 2 , false);
}

void CLG_Group_FLAT::OverlapSequence_L_1(void)
{
	Make_FLAT_Terrain(m_startidx , 1, 3, false);
}

void CLG_Group_FLAT::OverlapSequence_L_2(void)
{
	Make_FLAT_Terrain(m_startidx , 1, 2, true);
	Make_FLAT_Terrain(m_startidx + SQ_TILECNTX, 1, 2, false);
}

void CLG_Group_FLAT::OverlapSequence_L_3(void)
{
}

void CLG_Group_FLAT::OverlapSequence_L_4(void)
{
	Make_FLAT_Terrain(m_startidx , 1, 2 , false);
}

void CLG_Group_FLAT::OverlapSequence_L_5(void)
{
	Make_FLAT_Terrain(m_startidx , 1, 3 , false);
}



void CLG_Group_FLAT::OverlapSequence_LU_0(void)
{
	Make_FLAT_Terrain(m_startidx , 1 , 2);
}

void CLG_Group_FLAT::OverlapSequence_LU_1(void)
{
	Make_FLAT_Terrain(m_startidx , 1 , 3 , false);
}

void CLG_Group_FLAT::OverlapSequence_LU_2(void)
{
	Make_FLAT_Terrain(m_startidx - SQ_TILECNTX, 2 , 2);
}

void CLG_Group_FLAT::OverlapSequence_LU_3(void)
{
}
void CLG_Group_FLAT::OverlapSequence_RU_0(void)
{
	Make_FLAT_Terrain(m_startidx , 1 , 2);
}

void CLG_Group_FLAT::OverlapSequence_RU_1(void)
{
}

void CLG_Group_FLAT::OverlapSequence_RU_2(void)
{
	Make_FLAT_Terrain(m_startidx - SQ_TILECNTX, 2 , 2);
}

void CLG_Group_FLAT::OverlapSequence_RU_3(void)
{
	Make_FLAT_Terrain(m_startidx , 1 , 2 , false);
}

void CLG_Group_FLAT::OverlapSequence_RD_0(void)
{
	Make_FLAT_Terrain(m_startidx, 1, 2);
}

void CLG_Group_FLAT::OverlapSequence_RD_1(void)
{
}

void CLG_Group_FLAT::OverlapSequence_RD_2(void)
{
	Make_FLAT_Terrain(m_startidx , 1 , 2);
}

void CLG_Group_FLAT::OverlapSequence_RD_3(void)
{
	//Make_FLAT_Terrain(m_startidx , 1, 2 , false);
}

void CLG_Group_FLAT::OverlapSequence_RD_4(void)
{
	Make_FLAT_Terrain(m_startidx , 1 , 2);
}

void CLG_Group_FLAT::OverlapSequence_RD_5(void)
{
}

void CLG_Group_FLAT::OverlapSequence_R_0(void)
{
	Make_FLAT_Terrain(m_startidx , 2 , 2 , true);
	Make_FLAT_Terrain(m_startidx + SQ_TILECNTX*2, 1 , 2 , false);
}

void CLG_Group_FLAT::OverlapSequence_R_1(void)
{
}

void CLG_Group_FLAT::OverlapSequence_R_2(void)
{
	Make_FLAT_Terrain(m_startidx , 1, 2, true);
	Make_FLAT_Terrain(m_startidx + SQ_TILECNTX, 1, 2, false);
}

void CLG_Group_FLAT::OverlapSequence_R_3(void)
{
}

void CLG_Group_FLAT::OverlapSequence_R_4(void)
{
	Make_FLAT_Terrain(m_startidx , 1, 2 , false);
}

void CLG_Group_FLAT::OverlapSequence_R_5(void)
{
	Make_FLAT_Terrain(m_startidx , 1, 3 , false);
}

void CLG_Group_FLAT::OverlapSequence_LD_0(void)
{
	Make_FLAT_Terrain(m_startidx, 1, 2);
}

void CLG_Group_FLAT::OverlapSequence_LD_1(void)
{
	Make_FLAT_Terrain(m_startidx, 1, 3);
}

void CLG_Group_FLAT::OverlapSequence_LD_2(void)
{
	Make_FLAT_Terrain(m_startidx , 1 , 2);
}

void CLG_Group_FLAT::OverlapSequence_LD_3(void)
{
}

void CLG_Group_FLAT::OverlapSequence_LD_4(void)
{
	Make_FLAT_Terrain(m_startidx , 1 , 2);
}

void CLG_Group_FLAT::OverlapSequence_LD_5(void)
{
	Make_FLAT_Terrain(m_startidx , 1, 3);
}
