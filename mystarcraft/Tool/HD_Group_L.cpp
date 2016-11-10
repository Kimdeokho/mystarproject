#include "StdAfx.h"
#include "HD_Group_L.h"

#include "TileMgr.h"
#include "TerrainBrushMgr.h"
#include "MyBrush.h"
CHD_Group_L::CHD_Group_L(void)
{
	m_terrain_id = TERRAIN_HIGHDIRT;
	m_startidx = 0;
	m_oriTerrainInfo = NULL;
}

CHD_Group_L::~CHD_Group_L(void)
{
}

void CHD_Group_L::MakeTerrain_Group(int istartidx)
{
	m_startidx = istartidx;
	m_start_bottomidx = istartidx + SQ_TILECNTX;

	Group_L_Algorithm();
}
void CHD_Group_L::Group_L_Algorithm()
{
	m_oriTerrainInfo = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx);

	if(GROUP_L == m_oriTerrainInfo->byGroup_ID)
	{
		OverlapGroup_L();
		Group_L_Algorithm2();
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
		Group_L_Algorithm2();
	}
	else if(GROUP_FLAT == m_oriTerrainInfo->byGroup_ID)
	{
		if(TERRAIN_HIGHDIRT != m_oriTerrainInfo->byTerrain_ID)
			Make_L_Terrain(m_startidx , false , false);

		Group_L_Algorithm2();
	}
}
void CHD_Group_L::Group_L_Algorithm2()
{
	m_oriTerrainInfo = CTileMgr::GetInstance()->GetTerrain_Info(m_start_bottomidx);

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
}
void CHD_Group_L::OverlapGroup_L(void)
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
void CHD_Group_L::OverlapGroup_LU(void)
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

void CHD_Group_L::OverlapGroup_RU(void)
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

void CHD_Group_L::OverlapGroup_R(void)
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

void CHD_Group_L::OverlapGroup_RD(void)
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

void CHD_Group_L::OverlapGroup_LD(void)
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

void CHD_Group_L::OverlapSequence_L_0(void)
{
}

void CHD_Group_L::OverlapSequence_L_1(void)
{
}

void CHD_Group_L::OverlapSequence_L_2(void)
{
}

void CHD_Group_L::OverlapSequence_L_3(void)
{
}

void CHD_Group_L::OverlapSequence_L_4(void)
{
	Make_L_Terrain(m_startidx , true , false);
}

void CHD_Group_L::OverlapSequence_L_5(void)
{
}



void CHD_Group_L::OverlapSequence_LU_0(void)
{
}

void CHD_Group_L::OverlapSequence_LU_1(void)
{
}

void CHD_Group_L::OverlapSequence_LU_2(void)
{
}

void CHD_Group_L::OverlapSequence_LU_3(void)
{
}
void CHD_Group_L::OverlapSequence_RU_0(void)
{
	const TERRAIN_INFO* DownSpace = CTileMgr::GetInstance()->GetTerrain_Info(m_start_bottomidx + SQ_TILECNTX);

	Make_LU_Terrain(m_start_bottomidx - SQ_TILECNTX);
	if(GROUP_LD == DownSpace->byGroup_ID)
	{
		SetTerrainInfo(m_start_bottomidx + SQ_TILECNTX , m_terrain_id , GROUP_LD , 0, 0 ,true);
		SetTerrainInfo(m_start_bottomidx + SQ_TILECNTX + 1 , m_terrain_id , GROUP_LD , 1, 0 ,true);
	}
	else
	{
		Make_FLAT_Terrain(m_start_bottomidx + SQ_TILECNTX , 1, 2);
	}
}

void CHD_Group_L::OverlapSequence_RU_1(void)
{
}

void CHD_Group_L::OverlapSequence_RU_2(void)
{
}

void CHD_Group_L::OverlapSequence_RU_3(void)
{
}

void CHD_Group_L::OverlapSequence_RD_0(void)
{
}

void CHD_Group_L::OverlapSequence_RD_1(void)
{
}

void CHD_Group_L::OverlapSequence_RD_2(void)
{
	const TERRAIN_INFO* ptemp1 = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + SQ_TILECNTX);
	const TERRAIN_INFO* ptemp2 = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + SQ_TILECNTX - 1);
	const TERRAIN_INFO* ptemp3 = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + SQ_TILECNTX*2 );

	const TERRAIN_INFO* Up_Space = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx - SQ_TILECNTX );



	if(GROUP_RU == ptemp1->byGroup_ID)
	{
		if(GROUP_RU == ptemp2->byGroup_ID &&
			GROUP_RU == ptemp3->byGroup_ID)
		{
			if(GROUP_LU == Up_Space->byGroup_ID)
			{
				SetTerrainInfo(m_startidx - SQ_TILECNTX + 1 , m_terrain_id , GROUP_LU , 3 , 0 , true);
				Make_FLAT_Terrain(m_startidx , 3 , 2);
			}
			else
			{
				Make_FLAT_Terrain(m_startidx - SQ_TILECNTX , 4 , 2);
			}
		}
		else if(GROUP_RU == ptemp3->byGroup_ID)
		{
			if(GROUP_LU == Up_Space->byGroup_ID)
			{
				SetTerrainInfo(m_startidx - SQ_TILECNTX, m_terrain_id , GROUP_LU , 2 , 1 ,true);
				SetTerrainInfo(m_startidx - SQ_TILECNTX + 1, m_terrain_id , GROUP_LU , 3 , 0 ,true);
			}
			else
			{
				SetTerrainInfo(m_startidx - SQ_TILECNTX, m_terrain_id , GROUP_FLAT , 0 , 0 ,true);
				SetTerrainInfo(m_startidx - SQ_TILECNTX + 1, m_terrain_id , GROUP_FLAT , 0 , 0 ,true);
			}
			SetTerrainInfo(m_startidx + SQ_TILECNTX*2, m_terrain_id , GROUP_FLAT , 0 , 0 ,true);
			SetTerrainInfo(m_startidx + SQ_TILECNTX*2 + 1, m_terrain_id , GROUP_FLAT , 0 , 0 ,true);

			SetTerrainInfo(m_startidx , m_terrain_id , GROUP_LD , 0 , 0 ,true);
			SetTerrainInfo(m_startidx + 1, m_terrain_id , GROUP_LD , 1 , 0 ,true);
			SetTerrainInfo(m_startidx + SQ_TILECNTX, m_terrain_id , GROUP_LU , 2 , 1 ,true);
			SetTerrainInfo(m_startidx + SQ_TILECNTX + 1, m_terrain_id , GROUP_LU , 3 , 0 ,true);
		}
		else if(GROUP_LD == ptemp3->byGroup_ID &&
			GROUP_RU == ptemp2->byGroup_ID)
		{
			if(GROUP_LU == Up_Space->byGroup_ID)
			{
				Make_LU_Terrain(m_startidx - SQ_TILECNTX*2 );
				Make_FLAT_Terrain(m_startidx , 2 , 2);

			}
			else
			{
				Make_FLAT_Terrain(m_startidx - SQ_TILECNTX , 3 , 2);
			}
			SetTerrainInfo(m_startidx + SQ_TILECNTX*2 , m_terrain_id , GROUP_LD , 0 , 0 , true);
			SetTerrainInfo(m_startidx + SQ_TILECNTX*2 + 1, m_terrain_id , GROUP_LD , 1 , 0 , true);
		}
		else if(GROUP_LD == ptemp3->byGroup_ID)
		{
			if(GROUP_LU == Up_Space->byGroup_ID)
			{
				SetTerrainInfo(m_startidx - SQ_TILECNTX + 1, m_terrain_id , GROUP_LU , 3 , 0 , true);
			}
			else
			{
				Make_FLAT_Terrain(m_startidx - SQ_TILECNTX , 1 , 2);
			}
			SetTerrainInfo(m_startidx , m_terrain_id , GROUP_LD , 0 , 0 , true);
			SetTerrainInfo(m_startidx + 1, m_terrain_id , GROUP_LD , 1 , 0 , true);

			SetTerrainInfo(m_startidx + SQ_TILECNTX , m_terrain_id , GROUP_LU , 2 , 1 , true);
			SetTerrainInfo(m_startidx + SQ_TILECNTX + 1, m_terrain_id , GROUP_LU , 3 , 0 , true);

			SetTerrainInfo(m_startidx + SQ_TILECNTX*2, m_terrain_id , GROUP_LD , 0 , 0 , true);
			SetTerrainInfo(m_startidx + SQ_TILECNTX*2 + 1, m_terrain_id , GROUP_LD , 1 , 0 , true);
		}
	}
	else if(GROUP_R == ptemp1->byGroup_ID)
	{
		if(GROUP_RU == ptemp2->byGroup_ID)
		{
			if(GROUP_LU == Up_Space->byGroup_ID)
			{
				SetTerrainInfo(m_startidx - SQ_TILECNTX + 1 , m_terrain_id , GROUP_LU , 3 , 0 , true);
				Make_FLAT_Terrain(m_startidx  , 1, 2);
			}
			else
			{
				Make_FLAT_Terrain(m_startidx - SQ_TILECNTX , 2, 2);
			}
			Make_RD_Terrain(m_startidx + SQ_TILECNTX );
		}
		else
		{
			Make_FLAT_Terrain(m_startidx - SQ_TILECNTX , 1 , 2);
			Make_LD_Terrain(m_startidx);

			SetTerrainInfo(m_startidx + SQ_TILECNTX, m_terrain_id , GROUP_LU , 2 , 1 , true);

			SetTerrainInfo(m_startidx + SQ_TILECNTX + 1 , m_terrain_id , GROUP_RD , 1, 0 , true);
			SetTerrainInfo(m_startidx + SQ_TILECNTX*2 , m_terrain_id , GROUP_RD , 2, 0 , true);
			SetTerrainInfo(m_startidx + SQ_TILECNTX*2 + 1, m_terrain_id , GROUP_RD , 3, 0 , true);
			SetTerrainInfo(m_startidx + SQ_TILECNTX*3 , m_terrain_id , GROUP_RD , 4, 0 , false);
			SetTerrainInfo(m_startidx + SQ_TILECNTX*3 + 1 , m_terrain_id , GROUP_RD , 5, 0 , false);
		}
	}
	else
	{
		if(GROUP_LU == Up_Space->byGroup_ID)
		{
			SetTerrainInfo(m_startidx - SQ_TILECNTX, m_terrain_id , GROUP_LU , 2 , 1 , true);
			SetTerrainInfo(m_startidx - SQ_TILECNTX + 1, m_terrain_id , GROUP_LU , 3 , 0 , true);
		}
		else
		{
			SetTerrainInfo(m_startidx - SQ_TILECNTX, m_terrain_id , GROUP_FLAT , 0 , 0 , true);
			SetTerrainInfo(m_startidx - SQ_TILECNTX + 1, m_terrain_id , GROUP_FLAT , 0 , 0 , true);
		}

	
		Make_LD_Terrain(m_startidx);
	}

	LeftEdge_Algorithm();
}

void CHD_Group_L::OverlapSequence_RD_3(void)
{
}

void CHD_Group_L::OverlapSequence_RD_4(void)
{
}

void CHD_Group_L::OverlapSequence_RD_5(void)
{
}

void CHD_Group_L::OverlapSequence_R_0(void)
{
	SetTerrainInfo(m_start_bottomidx - SQ_TILECNTX , m_terrain_id , GROUP_LU , 0 , 1 , true);
	SetTerrainInfo(m_start_bottomidx - SQ_TILECNTX + 1, m_terrain_id , GROUP_LU , 1 , 1 , true);
	SetTerrainInfo(m_start_bottomidx, m_terrain_id , GROUP_LU , 2 , 1 , true);

	SetTerrainInfo(m_start_bottomidx + 1, m_terrain_id , GROUP_RD , 1 , 0 , true);
	SetTerrainInfo(m_start_bottomidx + SQ_TILECNTX, m_terrain_id , GROUP_RD , 2 , 0 , true);
	SetTerrainInfo(m_start_bottomidx + SQ_TILECNTX + 1, m_terrain_id , GROUP_RD , 3 , 0 , true);
	SetTerrainInfo(m_start_bottomidx + SQ_TILECNTX*2, m_terrain_id , GROUP_RD , 4 , 0 , false);
	SetTerrainInfo(m_start_bottomidx + SQ_TILECNTX*2 + 1, m_terrain_id , GROUP_RD , 5 , 0 , false);
}

void CHD_Group_L::OverlapSequence_R_1(void)
{
}

void CHD_Group_L::OverlapSequence_R_2(void)
{

	LeftEdge_Algorithm();

	const TERRAIN_INFO* DownSpace = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + SQ_TILECNTX);
	const TERRAIN_INFO* DownSpace2 = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + SQ_TILECNTX*2);
	const TERRAIN_INFO* LeftSpace = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx - 1);

	if(GROUP_R == DownSpace->byGroup_ID && DownSpace->byGroup_sequence <= 1)
	{
		Make_RU_Terrain(m_startidx - SQ_TILECNTX);
		Make_RD_Terrain(m_startidx + SQ_TILECNTX);
	}
	else if(GROUP_RU == DownSpace->byGroup_ID)
	{
		Make_RU_Terrain(m_startidx - SQ_TILECNTX);

		if(GROUP_LD == DownSpace2->byGroup_ID)
		{
			if(GROUP_L == LeftSpace->byGroup_ID)
			{
				SetTerrainInfo(m_startidx , m_terrain_id , GROUP_LD , 0, 0 ,true);
				SetTerrainInfo(m_startidx + SQ_TILECNTX, m_terrain_id , GROUP_LD , 2, 0 ,false);
				SetTerrainInfo(m_startidx + SQ_TILECNTX, m_terrain_id , GROUP_LU , 2, 1 ,true);
				SetTerrainInfo(m_startidx + SQ_TILECNTX + 1, m_terrain_id , GROUP_LU , 3, 0 ,true);
			}
			else
				Make_FLAT_Terrain(m_startidx + SQ_TILECNTX , 1 , 2);

			SetTerrainInfo(m_startidx + SQ_TILECNTX*2 , m_terrain_id , GROUP_LD , 0, 0 ,true);
			SetTerrainInfo(m_startidx + SQ_TILECNTX*2 + 1 , m_terrain_id , GROUP_LD , 1, 0 ,true);

		}
		else
		{
			if(GROUP_L == LeftSpace->byGroup_ID)
			{
				SetTerrainInfo(m_startidx , m_terrain_id , GROUP_LD , 0, 0 ,true);
				SetTerrainInfo(m_startidx + SQ_TILECNTX, m_terrain_id , GROUP_LD , 2, 0 ,false);
				SetTerrainInfo(m_startidx + SQ_TILECNTX, m_terrain_id , GROUP_LU , 2, 1 ,true);
				SetTerrainInfo(m_startidx + SQ_TILECNTX + 1, m_terrain_id , GROUP_LU , 3, 0 ,true);
				Make_FLAT_Terrain(m_startidx + SQ_TILECNTX*2 , 1 , 2);
			}
			else
				Make_FLAT_Terrain(m_startidx + SQ_TILECNTX , 2 , 2);
		}
	}
	else
	{
		SetTerrainInfo(m_startidx - SQ_TILECNTX , m_terrain_id , GROUP_RU , 0 , 1 , true);
		SetTerrainInfo(m_startidx - SQ_TILECNTX + 1, m_terrain_id , GROUP_RU , 1 , 1 , true);
		SetTerrainInfo(m_startidx + 1, m_terrain_id , GROUP_RU , 3 , 1 , true);

		SetTerrainInfo(m_startidx , m_terrain_id , GROUP_LD , 0 , 0 , true);
		SetTerrainInfo(m_startidx + SQ_TILECNTX, m_terrain_id , GROUP_LD , 2 , 0 , true);
		SetTerrainInfo(m_startidx + SQ_TILECNTX + 1, m_terrain_id , GROUP_LD , 3 , 0 , true);
		SetTerrainInfo(m_startidx + SQ_TILECNTX*2, m_terrain_id , GROUP_LD , 4 , 0 , false);
		SetTerrainInfo(m_startidx + SQ_TILECNTX*2 + 1, m_terrain_id , GROUP_LD , 5 , 0 , false);
	}
}

void CHD_Group_L::OverlapSequence_R_3(void)
{
}

void CHD_Group_L::OverlapSequence_R_4(void)
{
}

void CHD_Group_L::OverlapSequence_R_5(void)
{
}

void CHD_Group_L::OverlapSequence_LD_0(void)
{
}

void CHD_Group_L::OverlapSequence_LD_1(void)
{
}

void CHD_Group_L::OverlapSequence_LD_2(void)
{
}

void CHD_Group_L::OverlapSequence_LD_3(void)
{
}

void CHD_Group_L::OverlapSequence_LD_4(void)
{
	Make_L_Terrain(m_startidx , true , false);
}

void CHD_Group_L::OverlapSequence_LD_5(void)
{
}
