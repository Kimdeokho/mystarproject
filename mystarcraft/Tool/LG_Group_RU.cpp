#include "StdAfx.h"
#include "LG_Group_RU.h"

#include "TileMgr.h"
#include "TerrainBrushMgr.h"
#include "MyBrush.h"
CLG_Group_RU::CLG_Group_RU(void)
{
}

CLG_Group_RU::~CLG_Group_RU(void)
{
}

void CLG_Group_RU::MakeTerrain_Group(const int istartidx ,const int iflag)
{
	if(istartidx < 0 || istartidx > SQ_TILECNTX*SQ_TILECNTY)
		return;

	m_Flag = iflag;
	m_startidx = istartidx;
	Group_RU_Algorithm();
}
void CLG_Group_RU::Group_RU_Algorithm()
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
		{
			Make_RU_Terrain(m_startidx);
		}
		else if(TERRAIN_DIRT == m_oriTerrainInfo->byTerrain_ID &&
			TERRAIN_WATER == m_terrain_id)
		{
			Make_RU_Terrain(m_startidx);
		}
		if(GROUP_LD == DownSpace->byGroup_ID)
		{
			SetTerrainInfo(m_startidx + SQ_TILECNTX, m_terrain_id , GROUP_LD , 0 , 0 , true);
		}
	}
}
void CLG_Group_RU::OverlapGroup_L(void)
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
void CLG_Group_RU::OverlapGroup_LU(void)
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

void CLG_Group_RU::OverlapGroup_RU(void)
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

void CLG_Group_RU::OverlapGroup_R(void)
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

void CLG_Group_RU::OverlapGroup_RD(void)
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

void CLG_Group_RU::OverlapGroup_LD(void)
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

void CLG_Group_RU::OverlapSequence_L_0(void)
{
}

void CLG_Group_RU::OverlapSequence_L_1(void)
{
}

void CLG_Group_RU::OverlapSequence_L_2(void)
{
	if(1 == m_Flag)
	{
	}
	else
	{
		Make_FLAT_Terrain(m_startidx , 2 , 2 , false);
	}
}

void CLG_Group_RU::OverlapSequence_L_3(void)
{
}

void CLG_Group_RU::OverlapSequence_L_4(void)
{
}

void CLG_Group_RU::OverlapSequence_L_5(void)
{
}



void CLG_Group_RU::OverlapSequence_LU_0(void)
{
}

void CLG_Group_RU::OverlapSequence_LU_1(void)
{
}

void CLG_Group_RU::OverlapSequence_LU_2(void)
{
	const TERRAIN_INFO*	RightSpace = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + 1);
	const TERRAIN_INFO*	DownSpace = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + SQ_TILECNTX);
	const TERRAIN_INFO*	UpSpace = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx - SQ_TILECNTX);

	if(GROUP_RD == RightSpace->byGroup_ID)
	{
		if(GROUP_RU == DownSpace->byGroup_ID)
		{
			Make_FLAT_Terrain(m_startidx , 1, 2);
			Make_RU_Terrain(m_startidx);
		}
		else if(GROUP_LD == DownSpace->byGroup_ID)
		{
			Make_RU_Terrain(m_startidx);
			SetTerrainInfo(m_startidx + SQ_TILECNTX , m_terrain_id , GROUP_LD , 0 , 0 , true);
		}
		else
		{
			if(1 == m_Flag)
			{
				Make_FLAT_Terrain(m_startidx  , 1 ,2);				
			}
			Make_FLAT_Terrain(m_startidx + SQ_TILECNTX + 1 , 1 ,2);
			Make_R_Terrain(m_startidx , true , false);
		}
	}
	else
	{
		if(GROUP_LD == DownSpace->byGroup_ID)
		{
			//Make_FLAT_Terrain(m_startidx + 1, 1, 1 , false);
			Make_RU_Terrain(m_startidx);
			SetTerrainInfo(m_startidx + SQ_TILECNTX , m_terrain_id , GROUP_LD , 0 , 0 , true);
		}
		else if(GROUP_FLAT == DownSpace->byGroup_ID)
		{
			Make_FLAT_Terrain(m_startidx + 1, 1, 1 , false);
			Make_RU_Terrain(m_startidx);
		}
		else
		{
			//Make_FLAT_Terrain(m_startidx + 1, 1, 1 , false);
			Make_RU_Terrain(m_startidx);
		}
	}
}

void CLG_Group_RU::OverlapSequence_LU_3(void)
{
}
void CLG_Group_RU::OverlapSequence_RU_0(void)
{
	const TERRAIN_INFO*	DownSpace = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + SQ_TILECNTX);

	if(GROUP_LD == DownSpace->byGroup_ID)
	{
		Make_RU_Terrain(m_startidx);
		SetTerrainInfo(m_startidx + SQ_TILECNTX, m_terrain_id , GROUP_LD , 0 , 0 , true);
	}
	else
	{
		if( 1 == m_Flag)
			Make_FLAT_Terrain(m_startidx , 1 , 2 , false);

		Make_RU_Terrain(m_startidx);
	}
}

void CLG_Group_RU::OverlapSequence_RU_1(void)
{
}

void CLG_Group_RU::OverlapSequence_RU_2(void)
{
}

void CLG_Group_RU::OverlapSequence_RU_3(void)
{
}

void CLG_Group_RU::OverlapSequence_RD_0(void)
{
	const TERRAIN_INFO*	DownSpace = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + SQ_TILECNTX);

	if(GROUP_RU == DownSpace->byGroup_ID)
	{
		Make_FLAT_Terrain(m_startidx + SQ_TILECNTX , 1 , 1 , false);
		Make_RU_Terrain(m_startidx);
	}
	else if(GROUP_LD == DownSpace->byGroup_ID)
	{
		Make_RU_Terrain(m_startidx);
		SetTerrainInfo(m_startidx + SQ_TILECNTX, m_terrain_id , GROUP_LD , 0 , 0, false);
	}
	else
	{
		//SetTerrainInfo(m_startidx + SQ_TILECNTX + 1, TERRAIN_DIRT , GROUP_FLAT , 0 , 0, false);
		Make_FLAT_Terrain(m_startidx + SQ_TILECNTX + 1  , 1 ,2 , false);
		Make_R_Terrain(m_startidx , true , false);
	}
}

void CLG_Group_RU::OverlapSequence_RD_1(void)
{
}

void CLG_Group_RU::OverlapSequence_RD_2(void)
{
}

void CLG_Group_RU::OverlapSequence_RD_3(void)
{
}

void CLG_Group_RU::OverlapSequence_RD_4(void)
{
	if(1 == m_Flag)
	{
		Make_FLAT_Terrain(m_startidx , 1 , 2);
	}
	else
	{

	}
}

void CLG_Group_RU::OverlapSequence_RD_5(void)
{
	
}

void CLG_Group_RU::OverlapSequence_R_0(void)
{
	if( 1 == m_Flag)
		Make_FLAT_Terrain(m_startidx , 1 , 2 , false);

	Make_R_Terrain(m_startidx , true , false);
}

void CLG_Group_RU::OverlapSequence_R_1(void)
{
}

void CLG_Group_RU::OverlapSequence_R_2(void)
{
}

void CLG_Group_RU::OverlapSequence_R_3(void)
{
}

void CLG_Group_RU::OverlapSequence_R_4(void)
{
}

void CLG_Group_RU::OverlapSequence_R_5(void)
{
}

void CLG_Group_RU::OverlapSequence_LD_0(void)
{
}

void CLG_Group_RU::OverlapSequence_LD_1(void)
{
}

void CLG_Group_RU::OverlapSequence_LD_2(void)
{
	if(1 == m_Flag)
	{
		Make_FLAT_Terrain(m_startidx , 2, 2 , false);
	}
	else
	{
		Make_FLAT_Terrain(m_startidx + SQ_TILECNTX , 1, 2 , false);
	}
}

void CLG_Group_RU::OverlapSequence_LD_3(void)
{
}

void CLG_Group_RU::OverlapSequence_LD_4(void)
{
	Make_FLAT_Terrain(m_startidx ,1 , 2 );
}

void CLG_Group_RU::OverlapSequence_LD_5(void)
{
}
