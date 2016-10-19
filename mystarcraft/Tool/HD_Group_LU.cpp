#include "StdAfx.h"
#include "HD_Group_LU.h"

#include "TileMgr.h"
#include "TerrainBrushMgr.h"
#include "MyBrush.h"
CHD_Group_LU::CHD_Group_LU(void)
{
	m_terrain_id = TERRAIN_HIGHDIRT;
	m_startidx = 0;
	m_oriTerrainInfo = NULL;
}

CHD_Group_LU::~CHD_Group_LU(void)
{
}
void CHD_Group_LU::MakeTerrain_Group(int istartidx)
{
	m_startidx = istartidx;

	Group_LU_Algorithm();
}
void CHD_Group_LU::Group_LU_Algorithm()
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
		if(m_terrain_id == m_oriTerrainInfo->byTerrain_ID)
		{
			Make_FLAT_Terrain(m_startidx + SQ_TILECNTX , 1, 2);
		}
		else
		{
			Make_LU_Terrain(m_startidx);
		}
	}
}
void CHD_Group_LU::OverlapGroup_L(void)
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
void CHD_Group_LU::OverlapGroup_LU(void)
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

void CHD_Group_LU::OverlapGroup_RU(void)
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

void CHD_Group_LU::OverlapGroup_R(void)
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

void CHD_Group_LU::OverlapGroup_RD(void)
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

void CHD_Group_LU::OverlapGroup_LD(void)
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

void CHD_Group_LU::OverlapSequence_L_0(void)
{
	Make_LU_Terrain(m_startidx);
}

void CHD_Group_LU::OverlapSequence_L_1(void)
{
}

void CHD_Group_LU::OverlapSequence_L_2(void)
{
}

void CHD_Group_LU::OverlapSequence_L_3(void)
{
}

void CHD_Group_LU::OverlapSequence_L_4(void)
{
	Make_LU_Terrain(m_startidx);
}

void CHD_Group_LU::OverlapSequence_L_5(void)
{
}
void CHD_Group_LU::OverlapSequence_LU_0(void)
{
	Make_LU_Terrain(m_startidx);
}

void CHD_Group_LU::OverlapSequence_LU_1(void)
{
}

void CHD_Group_LU::OverlapSequence_LU_2(void)
{
}

void CHD_Group_LU::OverlapSequence_LU_3(void)
{
}
void CHD_Group_LU::OverlapSequence_RU_0(void)
{
}

void CHD_Group_LU::OverlapSequence_RU_1(void)
{
}

void CHD_Group_LU::OverlapSequence_RU_2(void)
{
	SetTerrainInfo(m_startidx + SQ_TILECNTX, m_terrain_id , GROUP_FLAT , 0 , 0 , true);
	SetTerrainInfo(m_startidx + SQ_TILECNTX + 1 , m_terrain_id , GROUP_FLAT , 0 , 0 , true);
}
void CHD_Group_LU::OverlapSequence_RU_3(void)
{
}

void CHD_Group_LU::OverlapSequence_RD_0(void)
{
}

void CHD_Group_LU::OverlapSequence_RD_1(void)
{
}

void CHD_Group_LU::OverlapSequence_RD_2(void)
{
	const TERRAIN_INFO* ptemp1 = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx - 1);
	const TERRAIN_INFO* ptemp2 = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + 2);

	const TERRAIN_INFO* lu_top1 = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx - SQ_TILECNTX);
	const TERRAIN_INFO* lu_top2 = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx - SQ_TILECNTX + 1);

	const TERRAIN_INFO* Edge1 = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx - 2);
	const TERRAIN_INFO* Edge2 = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx - 1);
	const TERRAIN_INFO* Edge3 = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx - 2 + SQ_TILECNTX);
	const TERRAIN_INFO* Edge4 = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx - 1 + SQ_TILECNTX);

	if(GROUP_RD == ptemp1->byGroup_ID &&
		GROUP_LD == ptemp2->byGroup_ID)
	{
		SetTerrainInfo(m_startidx , m_terrain_id , GROUP_FLAT , 0 , 0 , true);
		SetTerrainInfo(m_startidx + 1 , m_terrain_id , GROUP_FLAT , 0 , 0 , true);
		SetTerrainInfo(m_startidx + SQ_TILECNTX , m_terrain_id , GROUP_FLAT , 0 , 0 , true);
		SetTerrainInfo(m_startidx + SQ_TILECNTX + 1, m_terrain_id , GROUP_FLAT , 0 , 0 , true);

		if(GROUP_RD == lu_top1->byGroup_ID && GROUP_RD == lu_top2->byGroup_ID)
		{
			Make_FLAT_Terrain(m_startidx - SQ_TILECNTX , 1, 2);
		}
		else if(GROUP_LU == lu_top1->byGroup_ID || GROUP_LU == lu_top2->byGroup_ID)
		{
			SetTerrainInfo(m_startidx - SQ_TILECNTX , m_terrain_id , GROUP_LU , 2 , 1 , true);
			SetTerrainInfo(m_startidx + 1 - SQ_TILECNTX, m_terrain_id , GROUP_LU , 3 , 0 , true);
		}
	}
	else
	{
		if(GROUP_RD == ptemp1->byGroup_ID)
		{
			SetTerrainInfo(m_startidx , m_terrain_id , GROUP_RU , 2 , 0 , true);
			SetTerrainInfo(m_startidx + 1, m_terrain_id , GROUP_RU , 3 , 1 , true);
			SetTerrainInfo(m_startidx + SQ_TILECNTX , m_terrain_id , GROUP_FLAT , 0 , 0 , true);
			SetTerrainInfo(m_startidx + SQ_TILECNTX + 1 , m_terrain_id , GROUP_FLAT , 0 , 0 , true);
		}
		else if(GROUP_LD == ptemp2->byGroup_ID)
		{
			Make_FLAT_Terrain(m_startidx - SQ_TILECNTX , 1, 2);
			SetTerrainInfo(m_startidx , m_terrain_id , GROUP_LD , 0 , 0 , true);
			SetTerrainInfo(m_startidx + 1, m_terrain_id , GROUP_LD , 1 , 0 , true);
			SetTerrainInfo(m_startidx + SQ_TILECNTX, m_terrain_id , GROUP_LD , 2 , 0 , false);
			SetTerrainInfo(m_startidx + SQ_TILECNTX, m_terrain_id , GROUP_LU , 2 , 1 , true);
			SetTerrainInfo(m_startidx + SQ_TILECNTX + 1, m_terrain_id , GROUP_LU , 3 , 0 , true);

			if(GROUP_RD == lu_top1->byGroup_ID && GROUP_RD == lu_top2->byGroup_ID)
			{
				Make_FLAT_Terrain(m_startidx - SQ_TILECNTX , 1, 2);
			}
			else if(GROUP_LU == lu_top1->byGroup_ID || GROUP_LU == lu_top2->byGroup_ID)
			{
				SetTerrainInfo(m_startidx - SQ_TILECNTX , m_terrain_id , GROUP_RU , 2 , 0 , true);
				SetTerrainInfo(m_startidx + 1 - SQ_TILECNTX, m_terrain_id , GROUP_RU , 3 , 1 , true);
			}
		}
		else
		{
			SetTerrainInfo(m_startidx , m_terrain_id , GROUP_LD , 0 , 0 , true);
			//SetTerrainInfo(m_startidx + 1, m_terrain_id , GROUP_RD , 3 , 0 , false);
			SetTerrainInfo(m_startidx + 1, m_terrain_id , GROUP_RU , 8 , 1 , true);
			SetTerrainInfo(m_startidx + SQ_TILECNTX, m_terrain_id , GROUP_LU , 2 , 1 , true);
			SetTerrainInfo(m_startidx + SQ_TILECNTX + 1, m_terrain_id , GROUP_LU , 3 , 0 , true);
		}
	}

	if(GROUP_RD == Edge2->byGroup_ID && GROUP_RU == Edge4->byGroup_ID)
	{
		if(GROUP_RU == Edge3->byGroup_ID)
		{
			if(GROUP_LU == Edge1->byGroup_ID)
			{
				SetTerrainInfo(m_startidx - 2 , m_terrain_id , GROUP_LU , 2, 1 , true);
				SetTerrainInfo(m_startidx - 1 , m_terrain_id , GROUP_LU , 3, 0 , true);
				SetTerrainInfo(m_startidx - 2 + SQ_TILECNTX, m_terrain_id , GROUP_FLAT , 0, 0 , true);
				SetTerrainInfo(m_startidx - 1 + SQ_TILECNTX, m_terrain_id , GROUP_FLAT , 0, 0 , true);
			}
			else if(GROUP_RD == Edge1->byGroup_ID)
			{
				Make_FLAT_Terrain(m_startidx - 2 , 2 ,2);
			}
		}
		else if(GROUP_LD == Edge3->byGroup_ID)
		{
			if(GROUP_LU == Edge1->byGroup_ID)
			{
				SetTerrainInfo(m_startidx - 2 , m_terrain_id , GROUP_LU , 2, 1 , true);
				SetTerrainInfo(m_startidx - 1 , m_terrain_id , GROUP_LU , 3, 0 , true);
				SetTerrainInfo(m_startidx - 2 + SQ_TILECNTX, m_terrain_id , GROUP_LD , 0, 0 , true);
				SetTerrainInfo(m_startidx - 1 + SQ_TILECNTX, m_terrain_id , GROUP_LD , 1, 0 , true);
			}
			else if(GROUP_RD == Edge1->byGroup_ID)
			{
				Make_FLAT_Terrain(m_startidx - 2 , 2 ,2);
				SetTerrainInfo(m_startidx - 2 + SQ_TILECNTX, m_terrain_id , GROUP_LD , 0, 0 , true);
				SetTerrainInfo(m_startidx - 1 + SQ_TILECNTX, m_terrain_id , GROUP_LD , 1, 0 , true);
			}
		}
	}
}

void CHD_Group_LU::OverlapSequence_RD_3(void)
{
}

void CHD_Group_LU::OverlapSequence_RD_4(void)
{
}

void CHD_Group_LU::OverlapSequence_RD_5(void)
{
}

void CHD_Group_LU::OverlapSequence_R_0(void)
{
}

void CHD_Group_LU::OverlapSequence_R_1(void)
{
}

void CHD_Group_LU::OverlapSequence_R_2(void)
{
	LeftEdge_Algorithm();
	Make_RU_Terrain(m_startidx - SQ_TILECNTX);
	SetTerrainInfo(m_startidx + SQ_TILECNTX , m_terrain_id , GROUP_FLAT , 0 , 0 , true);
	SetTerrainInfo(m_startidx + SQ_TILECNTX + 1, m_terrain_id , GROUP_FLAT , 0 , 0 , true);
}

void CHD_Group_LU::OverlapSequence_R_3(void)
{
}

void CHD_Group_LU::OverlapSequence_R_4(void)
{
}

void CHD_Group_LU::OverlapSequence_R_5(void)
{
}

void CHD_Group_LU::OverlapSequence_LD_0(void)
{
	const TERRAIN_INFO* ptemp1 = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx - 1);
	const TERRAIN_INFO* RightSpace = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + 1);

	if(GROUP_RD == ptemp1->byGroup_ID)
	{
		if(GROUP_RU == RightSpace->byGroup_ID)
		{
			SetTerrainInfo(m_startidx , m_terrain_id , GROUP_RU , 2, 0 , true);
			SetTerrainInfo(m_startidx + 1, m_terrain_id , GROUP_RU , 3, 1 , true);

			Make_FLAT_Terrain(m_startidx + SQ_TILECNTX , 1, 2);
		}
		else
		{
			Make_FLAT_Terrain(m_startidx , 2, 2);
		}
	}
	else
	{
		SetTerrainInfo(m_startidx + SQ_TILECNTX , m_terrain_id , GROUP_LU , 2, 1 , true);
		SetTerrainInfo(m_startidx + SQ_TILECNTX + 1 , m_terrain_id , GROUP_LU , 3, 0 , true);
	}
}

void CHD_Group_LU::OverlapSequence_LD_1(void)
{
}

void CHD_Group_LU::OverlapSequence_LD_2(void)
{
}

void CHD_Group_LU::OverlapSequence_LD_3(void)
{
}

void CHD_Group_LU::OverlapSequence_LD_4(void)
{
	Make_LU_Terrain(m_startidx);
}

void CHD_Group_LU::OverlapSequence_LD_5(void)
{
}

