#include "StdAfx.h"
#include "HD_Group_RD.h"

#include "MyBrush.h"
#include "TileMgr.h"
#include "TerrainBrushMgr.h"
CHD_Group_RD::CHD_Group_RD(void)
{
	m_terrain_id = TERRAIN_HIGHDIRT;
	m_startidx = 0;
	m_oriTerrainInfo = NULL;
}

CHD_Group_RD::~CHD_Group_RD(void)
{
}
void CHD_Group_RD::MakeTerrain_Group(int istartidx)
{
	if(istartidx < 0 || istartidx > SQ_TILECNTX*SQ_TILECNTY)
		return;

	m_startidx = istartidx;
	m_start_bottomidx = istartidx + SQ_TILECNTX;
	
	Group_RD_Algorithm();
	Group_RD_Algorithm2();
}
void CHD_Group_RD::Group_RD_Algorithm()
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
		if( (TERRAIN_DIRT == m_oriTerrainInfo->byTerrain_ID && TERRAIN_HIGHDIRT == m_terrain_id) ||
			(TERRAIN_WATER == m_oriTerrainInfo->byTerrain_ID && TERRAIN_WATER == m_terrain_id))
		{
			const TERRAIN_INFO*	DownSpace = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + SQ_TILECNTX);
			const TERRAIN_INFO*	DRSpace = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + SQ_TILECNTX + 2);

			if(GROUP_L == DownSpace->byGroup_ID)
			{
				if(GROUP_R == DRSpace->byGroup_ID)
				{
					Make_LD_Terrain(m_startidx + SQ_TILECNTX);
					SetTerrainInfo(m_startidx , m_terrain_id , GROUP_RD , 0 , 0 , true);
					SetTerrainInfo(m_startidx + 1, m_terrain_id , GROUP_RD , 1 , 0 , true);
					SetTerrainInfo(m_startidx + SQ_TILECNTX + 1, m_terrain_id , GROUP_RD , 3 , 0 , false);
					SetTerrainInfo(m_startidx + SQ_TILECNTX + 1, m_terrain_id , GROUP_RU , 3 , 1 , true);					
				}
				else
				{
					Make_FLAT_Terrain(m_startidx , 1, 2);
					Make_LD_Terrain(m_startidx + SQ_TILECNTX);
				}
			}
			else
			{
				Make_RD_Terrain(m_startidx , false);
			}
		}

/*		if(m_oriTerrainInfo->byTerrain_ID == m_terrain_id)
		{

		}
		else
		{
		}	*/	
	}
}
void CHD_Group_RD::Group_RD_Algorithm2()
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

void CHD_Group_RD::OverlapGroup_L(void)
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
void CHD_Group_RD::OverlapGroup_LU(void)
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
	else if(8 == m_oriTerrainInfo->byGroup_sequence)
	{
		OverlapSequence_LU_8();
	}
}

void CHD_Group_RD::OverlapGroup_RU(void)
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

void CHD_Group_RD::OverlapGroup_R(void)
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

void CHD_Group_RD::OverlapGroup_RD(void)
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

void CHD_Group_RD::OverlapGroup_LD(void)
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

void CHD_Group_RD::OverlapSequence_L_0(void)
{
	//Make_FLAT_Terrain(m_start_bottomidx - SQ_TILECNTX , 1, 2 );
	////Make_LD_Terrain(m_start_bottomidx , true);

	//SetTerrainInfo(m_start_bottomidx  , m_terrain_id , GROUP_LD , 0, 0 , true);
	//SetTerrainInfo(m_start_bottomidx + 1 , m_terrain_id , GROUP_LD , 1, 0 , true);

	//SetTerrainInfo(m_start_bottomidx + SQ_TILECNTX , m_terrain_id , GROUP_LD , 2, 0 , true);
	//SetTerrainInfo(m_start_bottomidx + SQ_TILECNTX + 1 , m_terrain_id , GROUP_LD , 3, 0 , true);

	//SetTerrainInfo(m_start_bottomidx + SQ_TILECNTX*2 , m_terrain_id , GROUP_LD , 4, 0 , false);
	//SetTerrainInfo(m_start_bottomidx + SQ_TILECNTX*2 + 1 , m_terrain_id , GROUP_LD , 5, 0 , false);
}

void CHD_Group_RD::OverlapSequence_L_1(void)
{
}

void CHD_Group_RD::OverlapSequence_L_2(void)
{
	//Make_RD_Terrain(m_startidx , true);
	const TERRAIN_INFO* DownSpace = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + SQ_TILECNTX);

	if(GROUP_L == DownSpace->byGroup_ID)
	{
		if(0 == DownSpace->byGroup_sequence)
		{
			Make_FLAT_Terrain(m_startidx ,1, 2);
			Make_LD_Terrain( m_startidx + SQ_TILECNTX);
		}
		else
			Make_RD_Terrain( m_startidx , true , false);
	}
	else
	{
		Make_RD_Terrain( m_startidx , true , false);
	}

	RightEdge_Algorithm();
}

void CHD_Group_RD::OverlapSequence_L_3(void)
{
}

void CHD_Group_RD::OverlapSequence_L_4(void)
{
}

void CHD_Group_RD::OverlapSequence_L_5(void)
{
}



void CHD_Group_RD::OverlapSequence_LU_0(void)
{
	TERRAIN_INFO*	RightSpace = CTileMgr::GetInstance()->GetTerrain_Info(m_start_bottomidx + 2 );
	TERRAIN_INFO*	LeftSpace = CTileMgr::GetInstance()->GetTerrain_Info(m_start_bottomidx - 1 );
	TERRAIN_INFO*	DRSpace = CTileMgr::GetInstance()->GetTerrain_Info(m_start_bottomidx + SQ_TILECNTX + 1);

	if(GROUP_LU == RightSpace->byGroup_ID)
	{
		if(GROUP_RU == LeftSpace->byGroup_ID)
		{
			if(GROUP_LU == DRSpace->byGroup_ID)
			{
				Make_FLAT_Terrain(m_startidx , 3, 2);
			}
			else if(GROUP_RD == DRSpace->byGroup_ID)
			{
				Make_FLAT_Terrain(m_startidx ,2, 2);
				SetTerrainInfo(m_startidx + SQ_TILECNTX*2, m_terrain_id , GROUP_RD , 0 , 0 , true);
				SetTerrainInfo(m_startidx + SQ_TILECNTX*2 + 1, m_terrain_id , GROUP_RD , 1 , 0 , true);
			}
		}
		else
		{
			Make_FLAT_Terrain(m_startidx , 1, 2);
			SetTerrainInfo(m_startidx + SQ_TILECNTX , m_terrain_id , GROUP_LD , 0 , 0 , true);
			SetTerrainInfo(m_startidx + SQ_TILECNTX + 1, m_terrain_id , GROUP_LD , 1 , 0 , true);
			SetTerrainInfo(m_startidx + SQ_TILECNTX*2 , m_terrain_id , GROUP_LD , 2 , 0 , false);
			SetTerrainInfo(m_startidx + SQ_TILECNTX*2, m_terrain_id , GROUP_LU , 2 , 1 , true);

			if(GROUP_RD == DRSpace->byGroup_ID)
			{
				SetTerrainInfo(m_startidx + SQ_TILECNTX*2 + 1, m_terrain_id , GROUP_RD , 1 , 0 , true);
			}
			else
			{
				SetTerrainInfo(m_startidx + SQ_TILECNTX*2 + 1, m_terrain_id , GROUP_LU , 3 , 0 , true);
			}
		}
	}
	else if(GROUP_RU == LeftSpace->byGroup_ID)
	{
		SetTerrainInfo(m_startidx , m_terrain_id , GROUP_RD , 0 , 0 , true);
		SetTerrainInfo(m_startidx + 1, m_terrain_id , GROUP_RD , 1 , 0 , true);
		SetTerrainInfo(m_startidx + SQ_TILECNTX, m_terrain_id , GROUP_RU , 2 , 0 , true);

		SetTerrainInfo(m_startidx + SQ_TILECNTX + 1, m_terrain_id , GROUP_RD , 3 , 0 , true);
		SetTerrainInfo(m_startidx + SQ_TILECNTX + 1, m_terrain_id , GROUP_RU , 3 , 1 , true);

		if(GROUP_RD == DRSpace->byGroup_ID)
		{
			SetTerrainInfo(m_startidx + SQ_TILECNTX*2, m_terrain_id , GROUP_RD , 0 , 0 , true);
			SetTerrainInfo(m_startidx + SQ_TILECNTX*2 + 1, m_terrain_id , GROUP_RD , 1 , 0 , true);
		}
		else
		{
			Make_FLAT_Terrain(m_startidx + SQ_TILECNTX*2 , 1, 2);
		}
	}
	else
	{
		SetTerrainInfo(m_startidx , m_terrain_id , GROUP_RD , 0 , 0 , true);
		SetTerrainInfo(m_startidx + 1, m_terrain_id , GROUP_RD , 1 , 0 , true);

		SetTerrainInfo(m_startidx + SQ_TILECNTX, m_terrain_id , GROUP_LD , 0 , 0 , true);

		SetTerrainInfo(m_startidx + SQ_TILECNTX + 1, m_terrain_id , GROUP_RD , 3 , 0 , false);
		SetTerrainInfo(m_startidx + SQ_TILECNTX + 1, m_terrain_id , GROUP_RU , 8 , 1 , true);

		SetTerrainInfo(m_startidx + SQ_TILECNTX*2, m_terrain_id , GROUP_LD , 2 , 0 , true);
		SetTerrainInfo(m_startidx + SQ_TILECNTX*2 , m_terrain_id , GROUP_LU , 2 , 1 , true);
		SetTerrainInfo(m_startidx + SQ_TILECNTX*2 + 1, m_terrain_id , GROUP_LU , 3 , 0 , true);
	}
}

void CHD_Group_RD::OverlapSequence_LU_1(void)
{
}

void CHD_Group_RD::OverlapSequence_LU_2(void)
{
	const TERRAIN_INFO* RightSpace = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + 1);

	if(GROUP_RD == RightSpace->byGroup_ID)
	{
		SetTerrainInfo(m_startidx , m_terrain_id , GROUP_RD , 0 , 0 , true);
	}
	else
	{
		Make_FLAT_Terrain(m_startidx , 1 , 2);
	}
}

void CHD_Group_RD::OverlapSequence_LU_3(void)
{
}
void CHD_Group_RD::OverlapSequence_LU_8(void)
{
	SetTerrainInfo(m_startidx , m_terrain_id , GROUP_RD , 0 , 0 , true );
}
void CHD_Group_RD::OverlapSequence_RU_0(void)
{
	TERRAIN_INFO*	DR_Space = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + 2);
	TERRAIN_INFO*	Down_Space = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + SQ_TILECNTX);

	if(GROUP_LU == DR_Space->byGroup_ID)
	{
		if(GROUP_LD == Down_Space->byGroup_ID)
		{
			Make_FLAT_Terrain(m_startidx , 1 ,2);
			SetTerrainInfo(m_startidx + SQ_TILECNTX , m_terrain_id , GROUP_LD , 0 , 0 , true);	
			SetTerrainInfo(m_startidx + SQ_TILECNTX + 1 , m_terrain_id , GROUP_LD , 1 , 0 , true);	
		}
		else
			Make_FLAT_Terrain(m_startidx , 2 ,2);
	}
	else
	{
		SetTerrainInfo(m_startidx , m_terrain_id , GROUP_RD , 0 , 0 , true);
		SetTerrainInfo(m_startidx + 1 , m_terrain_id , GROUP_RD , 1 , 0 , true);
		SetTerrainInfo(m_startidx + SQ_TILECNTX + 1 , m_terrain_id , GROUP_RD , 2 , 0 , false);	
	}
}

void CHD_Group_RD::OverlapSequence_RU_1(void)
{
}

void CHD_Group_RD::OverlapSequence_RU_2(void)
{
}

void CHD_Group_RD::OverlapSequence_RU_3(void)
{
}

void CHD_Group_RD::OverlapSequence_RD_0(void)
{
}

void CHD_Group_RD::OverlapSequence_RD_1(void)
{
}

void CHD_Group_RD::OverlapSequence_RD_2(void)
{
}

void CHD_Group_RD::OverlapSequence_RD_3(void)
{
}

void CHD_Group_RD::OverlapSequence_RD_4(void)
{
	const TERRAIN_INFO*	DownSpace = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + SQ_TILECNTX);
	const TERRAIN_INFO*	DRSpace = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + SQ_TILECNTX + 2);

	if(GROUP_L == DownSpace->byGroup_ID)
	{
		if(GROUP_RU == DRSpace->byGroup_ID)
		{
			SetTerrainInfo(m_startidx  , m_terrain_id , GROUP_RD , 0 , 0 , true);
			SetTerrainInfo(m_startidx  + 1, m_terrain_id , GROUP_RD , 1 , 0 , true);
			Make_LD_Terrain(m_startidx + SQ_TILECNTX);
			SetTerrainInfo(m_startidx + SQ_TILECNTX + 1, m_terrain_id , GROUP_RD , 3 , 0 , false);
			SetTerrainInfo(m_startidx + SQ_TILECNTX + 1, m_terrain_id , GROUP_RU , 3 , 1 , true);
			
		}
		else
		{
			Make_FLAT_Terrain(m_startidx , 1 , 2);
			Make_LD_Terrain(m_startidx + SQ_TILECNTX);
		}
	}
	else
	{
		Make_RD_Terrain(m_startidx , false);
	}
}

void CHD_Group_RD::OverlapSequence_RD_5(void)
{
}

void CHD_Group_RD::OverlapSequence_R_0(void)
{
	Make_RD_Terrain(m_startidx , false);
	SetTerrainInfo(m_startidx , m_terrain_id , GROUP_RD , 0 , 0 , true);
	SetTerrainInfo(m_startidx + 1, m_terrain_id , GROUP_RD , 1 , 0 , true);
	SetTerrainInfo(m_startidx + SQ_TILECNTX, m_terrain_id , GROUP_RD , 2 , 0 , true);
	SetTerrainInfo(m_startidx + SQ_TILECNTX + 1, m_terrain_id , GROUP_RD , 3 , 0 , true);
}

void CHD_Group_RD::OverlapSequence_R_1(void)
{
}

void CHD_Group_RD::OverlapSequence_R_2(void)
{
}

void CHD_Group_RD::OverlapSequence_R_3(void)
{
}

void CHD_Group_RD::OverlapSequence_R_4(void)
{
	TERRAIN_INFO*	DownSpace = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + SQ_TILECNTX);

	if(GROUP_L == DownSpace->byGroup_ID)
	{
		Make_FLAT_Terrain(m_startidx , 1 , 2);
		Make_LD_Terrain(m_startidx + SQ_TILECNTX);
	}
	else
		Make_RD_Terrain(m_startidx , false);
}

void CHD_Group_RD::OverlapSequence_R_5(void)
{
}

void CHD_Group_RD::OverlapSequence_LD_0(void)
{
}

void CHD_Group_RD::OverlapSequence_LD_1(void)
{
}

void CHD_Group_RD::OverlapSequence_LD_2(void)
{

	RightEdge_Algorithm();

	if(GROUP_LU == CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + SQ_TILECNTX)->byGroup_ID)
		return;

	TERRAIN_INFO*	DownSpace = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + SQ_TILECNTX + 1);
	TERRAIN_INFO*	RightSpace = CTileMgr::GetInstance()->GetTerrain_Info(m_startidx + 2);

	if(GROUP_L == DownSpace->byGroup_ID)
	{
		Make_FLAT_Terrain(m_startidx , 1, 2);
		Make_LD_Terrain(m_startidx + SQ_TILECNTX);
	}
	else if(GROUP_LU == DownSpace->byGroup_ID)
	{
		if(GROUP_LD == RightSpace->byGroup_ID)
		{
			Make_FLAT_Terrain(m_startidx , 1, 2);
			SetTerrainInfo(m_startidx + SQ_TILECNTX , m_terrain_id , GROUP_LD , 0 , 0 , true);
			SetTerrainInfo(m_startidx + SQ_TILECNTX + 1, m_terrain_id , GROUP_LD , 1 , 0 , true);
			SetTerrainInfo(m_startidx + SQ_TILECNTX*2 + 1, m_terrain_id , GROUP_LD , 3 , 0 , false);
		}
		else if(GROUP_RD == RightSpace->byGroup_ID)
		{
			Make_RD_Terrain(m_startidx);
			SetTerrainInfo(m_startidx + SQ_TILECNTX , m_terrain_id , GROUP_LD , 0 , 0 , true);
			SetTerrainInfo(m_startidx + SQ_TILECNTX + 1, m_terrain_id , GROUP_RU , 8 , 1 , true);

			SetTerrainInfo(m_startidx + SQ_TILECNTX*2 , m_terrain_id , GROUP_LD , 2 , 0 , false);
			SetTerrainInfo(m_startidx + SQ_TILECNTX*2 , m_terrain_id , GROUP_LU , 2 , 1 , true);
			SetTerrainInfo(m_startidx + SQ_TILECNTX*2 + 1, m_terrain_id , GROUP_LU , 3 , 0 , true);
		}
	}
	else
	{
		Make_RD_Terrain(m_startidx , true);
	}
}

void CHD_Group_RD::OverlapSequence_LD_3(void)
{
}

void CHD_Group_RD::OverlapSequence_LD_4(void)
{
}

void CHD_Group_RD::OverlapSequence_LD_5(void)
{
}