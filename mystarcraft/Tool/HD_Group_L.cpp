#include "StdAfx.h"
#include "HD_Group_L.h"

#include "TileMgr.h"
#include "TerrainBrushMgr.h"
#include "MyBrush.h"
CHD_Group_L::CHD_Group_L(void)
{
	m_terrain_id = TERRAIN_HIGHDIRT;
	m_startidx = 0;
	m_oriidx = 0;
	m_cursequence = 0;
	ZeroMemory(&m_sendTerrainInfo , sizeof(TERRAIN_INFO));
	m_oriTerrainInfo = NULL;
}

CHD_Group_L::~CHD_Group_L(void)
{
}

void CHD_Group_L::MakeTerrain_Group(int istartidx)
{
	m_startidx = istartidx;

	m_breturn = false;

	int tempidx = 0;
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			m_cursequence = i*2+j;
			tempidx = i * SQ_TILECNTX + j;
			m_oriidx = m_startidx + tempidx;
			m_bTerrainDelete = true;

				SetTerrainInfo(TERRAIN_HIGHDIRT , GROUP_L , m_cursequence , 0);

			if(m_cursequence <= 2)
				SetTerrainInfo(TERRAIN_HIGHDIRT , GROUP_L , m_cursequence , 1);

			Group_L_Algorithm();
		}
	}
}

void CHD_Group_L::Group_L_Algorithm()
{
	m_oriTerrainInfo = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx);

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
		{

		}
		else
		{
			CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , true);
		}
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
	if(0 == m_cursequence)
	{
	}
	else if(1 == m_cursequence)
	{
	}
	else if(2 == m_cursequence)
	{
	}
	else if(3 == m_cursequence)
	{
	}
	else if(4 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_L , 4 , 0);
		m_bTerrainDelete = false;
	}
	else if(5 == m_cursequence)
	{

	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_L::OverlapSequence_L_1(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{

	}
	else if(3 == m_cursequence)
	{

	}
	else if(4 == m_cursequence)
	{

	}
	else if(5 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_L , 5 , 0);
		m_bTerrainDelete = false;
	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_L::OverlapSequence_L_2(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{

	}
	else if(3 == m_cursequence)
	{

	}
	else if(4 == m_cursequence)
	{

	}
	else if(5 == m_cursequence)
	{

	}
}

void CHD_Group_L::OverlapSequence_L_3(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{

	}
	else if(3 == m_cursequence)
	{

	}
	else if(4 == m_cursequence)
	{

	}
	else if(5 == m_cursequence)
	{

	}
}

void CHD_Group_L::OverlapSequence_L_4(void)
{
	if(0 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_L , 0 , 1);
	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{

	}
	else if(3 == m_cursequence)
	{

	}
	else if(4 == m_cursequence)
	{

	}
	else if(5 == m_cursequence)
	{

	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_L::OverlapSequence_L_5(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_L , 1 , 1);
	}
	else if(2 == m_cursequence)
	{

	}
	else if(3 == m_cursequence)
	{

	}
	else if(4 == m_cursequence)
	{

	}
	else if(5 == m_cursequence)
	{

	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}



void CHD_Group_L::OverlapSequence_LU_0(void)
{
	/*설치하려는 그룹 L이 기존의 LU와 겹칠때*/
	if(0 == m_cursequence)
	{
		return;
	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{

	}
	else if(3 == m_cursequence)
	{

	}
	else if(4 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_L , m_cursequence , 0);
		m_bTerrainDelete = false;
	}
	else if(5 == m_cursequence)
	{

	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_L::OverlapSequence_LU_1(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{
		return;
	}
	else if(2 == m_cursequence)
	{

	}
	else if(3 == m_cursequence)
	{

	}
	else if(4 == m_cursequence)
	{
	}
	else if(5 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_L , m_cursequence , 0);
		m_bTerrainDelete = false;
	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_L::OverlapSequence_LU_2(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{
		return;
	}
	else if(3 == m_cursequence)
	{

	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_L::OverlapSequence_LU_3(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{

	}
	else if(3 == m_cursequence)
	{
		return;
	}
	else if(4 == m_cursequence)
	{

	}
	else if(5 == m_cursequence)
	{

	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}
void CHD_Group_L::OverlapSequence_RU_0(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{
		if(m_breturn)
			return;

		TERRAIN_INFO*	ptemp1 = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx - 2);
		TERRAIN_INFO*	ptemp2 = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx - 2 - SQ_TILECNTX);

		SetTerrainInfo(m_terrain_id , GROUP_LU , 2 , 1);

		if(GROUP_RU == ptemp1->byGroup_ID && GROUP_FLAT == ptemp2->byGroup_ID)
		{
			SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx - 2 , m_sendTerrainInfo , m_bTerrainDelete);
		}
		else if(GROUP_LD != ptemp2->byGroup_ID)
		{
			SetTerrainInfo(m_terrain_id , GROUP_LU , 0 , 1);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx - SQ_TILECNTX , m_sendTerrainInfo , m_bTerrainDelete);
			SetTerrainInfo(m_terrain_id , GROUP_LU , 2 , 1);
		}
	}
	else if(3 == m_cursequence)
	{

	}
	else if(4 == m_cursequence)
	{

	}
	else if(5 == m_cursequence)
	{

	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_L::OverlapSequence_RU_1(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{

	}
	else if(3 == m_cursequence)
	{
		if(m_breturn)
			return;

		TERRAIN_INFO*	ptemp1 = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx - 2);
		TERRAIN_INFO*	ptemp2 = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx - 2 - SQ_TILECNTX);

		SetTerrainInfo(m_terrain_id , GROUP_LU , 3 , 0);

		if(GROUP_RU == ptemp1->byGroup_ID && GROUP_FLAT == ptemp2->byGroup_ID)
		{
			SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx - 2 , m_sendTerrainInfo , m_bTerrainDelete);
		}
		else if(GROUP_LD != ptemp2->byGroup_ID)
		{
			SetTerrainInfo(m_terrain_id , GROUP_LU , 1 , 1);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx - SQ_TILECNTX , m_sendTerrainInfo , m_bTerrainDelete);
			SetTerrainInfo(m_terrain_id , GROUP_LU , 3 , 0);
		}
	}
	else if(4 == m_cursequence)
	{

	}
	else if(5 == m_cursequence)
	{

	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_L::OverlapSequence_RU_2(void)
{
	if(0 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_RU , 2 , 0);
	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{

	}
	else if(3 == m_cursequence)
	{

	}
	else if(4 == m_cursequence)
	{
		if(m_breturn)
			return;
		SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);

	}
	else if(5 == m_cursequence)
	{
	}

	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_L::OverlapSequence_RU_3(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{
		if(m_breturn)
			return;
		SetTerrainInfo(m_terrain_id , GROUP_RU , 3 , 1);
	}
	else if(2 == m_cursequence)
	{

	}
	else if(3 == m_cursequence)
	{

	}
	else if(4 == m_cursequence)
	{

	}
	else if(5 == m_cursequence)
	{
		if(m_breturn)
			return;

		TERRAIN_INFO* ptemp1 = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx - 1);
		if(GROUP_LD == ptemp1->byGroup_ID)
		{
			SetTerrainInfo(m_terrain_id , GROUP_LD , 1 , 0);
		}
		else
		{
			SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);
		}
	}

	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_L::OverlapSequence_RD_0(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{
		TERRAIN_INFO* ptemp1 = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx + SQ_TILECNTX);
		SetTerrainInfo(m_terrain_id , GROUP_RD , 0 , 0);

		if(GROUP_RU == ptemp1->byGroup_ID)
		{
			SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);
		}
		if(GROUP_LD == ptemp1->byGroup_ID)
		{
			SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);
		}
	}
	else if(3 == m_cursequence)
	{

	}
	else if(4 == m_cursequence)
	{

	}
	else if(5 == m_cursequence)
	{

	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_L::OverlapSequence_RD_1(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{

	}
	else if(3 == m_cursequence)
	{
		TERRAIN_INFO* ptemp1 = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx + SQ_TILECNTX);
		SetTerrainInfo(m_terrain_id , GROUP_RD , 1 , 0);

		if(GROUP_RU == ptemp1->byGroup_ID)
		{
			if(8 == ptemp1->byGroup_sequence)
			{
				SetTerrainInfo(m_terrain_id , GROUP_LD , 1 , 0);
				CTileMgr::GetInstance()->SetTerrain(m_oriidx + SQ_TILECNTX , m_sendTerrainInfo , m_bTerrainDelete);
			}
			SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);
		}
	}
	else if(4 == m_cursequence)
	{

	}
	else if(5 == m_cursequence)
	{

	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_L::OverlapSequence_RD_2(void)
{
	if(0 == m_cursequence)
	{

		TERRAIN_INFO*	ptemp1 = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx - 2);
		TERRAIN_INFO*	ptemp2 = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx - 2 + SQ_TILECNTX);
		TERRAIN_INFO*	ptemp3 = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx - SQ_TILECNTX*2);

		if(GROUP_RD == ptemp1->byGroup_ID && GROUP_RU == ptemp2->byGroup_ID)
		{
			SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx - 2, m_sendTerrainInfo , m_bTerrainDelete);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx - SQ_TILECNTX , m_sendTerrainInfo , m_bTerrainDelete);
		}
		else if(GROUP_LU == ptemp3->byGroup_ID)
		{
			SetTerrainInfo(m_terrain_id , GROUP_LU , 2 , 1);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx - SQ_TILECNTX , m_sendTerrainInfo , m_bTerrainDelete);
			SetTerrainInfo(m_terrain_id , GROUP_LD , 0 , 0);
		}
		else if(GROUP_RU == ptemp3->byGroup_ID || GROUP_FLAT == ptemp3->byGroup_ID)
		{
			SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx - SQ_TILECNTX , m_sendTerrainInfo , m_bTerrainDelete);
			SetTerrainInfo(m_terrain_id , GROUP_LD , 0 , 0);
		}
	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{

	}
	else if(3 == m_cursequence)
	{

	}
	else if(4 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_RD , 2 , 0);
	}
	else if(5 == m_cursequence)
	{

	}

	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_L::OverlapSequence_RD_3(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{
		TERRAIN_INFO*	ptemp1 = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx - 2);
		TERRAIN_INFO*	ptemp2 = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx - 2 + SQ_TILECNTX);
		TERRAIN_INFO*	ptemp3 = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx - SQ_TILECNTX*2);

		if(GROUP_RD == ptemp1->byGroup_ID && GROUP_RU == ptemp2->byGroup_ID)
		{
			SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx - 2, m_sendTerrainInfo , m_bTerrainDelete);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx - SQ_TILECNTX , m_sendTerrainInfo , m_bTerrainDelete);
		}
		else if(GROUP_LU == ptemp3->byGroup_ID)
		{
			SetTerrainInfo(m_terrain_id , GROUP_LU , 3 , 0);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx - SQ_TILECNTX , m_sendTerrainInfo , m_bTerrainDelete);
			SetTerrainInfo(m_terrain_id , GROUP_LD , 1 , 0);
		}
		else if(GROUP_RU == ptemp3->byGroup_ID || GROUP_FLAT == ptemp3->byGroup_ID)
		{
			SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx - SQ_TILECNTX , m_sendTerrainInfo , m_bTerrainDelete);
			SetTerrainInfo(m_terrain_id , GROUP_LD , 1 , 0);
		}
	}
	else if(2 == m_cursequence)
	{

	}
	else if(3 == m_cursequence)
	{

	}
	else if(4 == m_cursequence)
	{

	}
	else if(5 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_RD , 3 , 0);
	}

	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_L::OverlapSequence_RD_4(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_LD , 4 , 0);
		CTileMgr::GetInstance()->SetTerrain(m_oriidx + SQ_TILECNTX , m_sendTerrainInfo , m_bTerrainDelete);
		SetTerrainInfo(m_terrain_id , GROUP_LD , 2 , 0);
	}
	else if(3 == m_cursequence)
	{

	}
	else if(4 == m_cursequence)
	{

	}
	else if(5 == m_cursequence)
	{

	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_L::OverlapSequence_RD_5(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{

	}
	else if(3 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_LD , 5 , 0);
		CTileMgr::GetInstance()->SetTerrain(m_oriidx + SQ_TILECNTX , m_sendTerrainInfo , m_bTerrainDelete);
		SetTerrainInfo(m_terrain_id , GROUP_LD , 3 , 0);
	}
	else if(4 == m_cursequence)
	{
	}
	else if(5 == m_cursequence)
	{

	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_L::OverlapSequence_R_0(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{
		TERRAIN_INFO* ptemp1 = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx - 2 );
		TERRAIN_INFO* ptemp2 = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx - 2 - SQ_TILECNTX);

		if(GROUP_RU == ptemp1->byGroup_ID && GROUP_RD == ptemp2->byGroup_ID)
		{
			SetTerrainInfo(m_terrain_id , GROUP_RU , 2, 0);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx -  SQ_TILECNTX, m_sendTerrainInfo , m_bTerrainDelete);
			SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx -  2, m_sendTerrainInfo , m_bTerrainDelete);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx -  2 - SQ_TILECNTX, m_sendTerrainInfo , m_bTerrainDelete);
			SetTerrainInfo(m_terrain_id , GROUP_RD , 0, 0);
		}
		else
		{
			SetTerrainInfo(m_terrain_id , GROUP_LU , 0 , 1);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx -  SQ_TILECNTX, m_sendTerrainInfo , m_bTerrainDelete);
			SetTerrainInfo(m_terrain_id , GROUP_LU , 2 , 1);
		}	
	}
	else if(3 == m_cursequence)
	{

	}
	else if(4 == m_cursequence)
	{

	}
	else if(5 == m_cursequence)
	{

	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_L::OverlapSequence_R_1(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{

	}
	else if(3 == m_cursequence)
	{
		TERRAIN_INFO* ptemp1 = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx - 2 );
		TERRAIN_INFO* ptemp2 = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx - 2 - SQ_TILECNTX);

		if(GROUP_RU == ptemp1->byGroup_ID && GROUP_RD == ptemp2->byGroup_ID)
		{
			SetTerrainInfo(m_terrain_id , GROUP_RU , 3, 1);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx -  SQ_TILECNTX, m_sendTerrainInfo , m_bTerrainDelete);
			SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx -  2, m_sendTerrainInfo , m_bTerrainDelete);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx -  2 - SQ_TILECNTX, m_sendTerrainInfo , m_bTerrainDelete);
			SetTerrainInfo(m_terrain_id , GROUP_RD , 1, 0);
		}
		else
		{
			SetTerrainInfo(m_terrain_id , GROUP_LU , 1 , 1);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx -  SQ_TILECNTX, m_sendTerrainInfo , m_bTerrainDelete);
			SetTerrainInfo(m_terrain_id , GROUP_RD , 1 , 0);
		}
	}
	else if(4 == m_cursequence)
	{

	}
	else if(5 == m_cursequence)
	{

	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_L::OverlapSequence_R_2(void)
{
	if(0 == m_cursequence)
	{
		A_Search();
		TERRAIN_INFO* ptemp1 = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx - 2 );
		TERRAIN_INFO* ptemp2 = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx - 2 + SQ_TILECNTX);

		SetTerrainInfo(m_terrain_id , GROUP_RU , 0 , 1);
		CTileMgr::GetInstance()->SetTerrain(m_oriidx - SQ_TILECNTX, m_sendTerrainInfo , m_bTerrainDelete);

		SetTerrainInfo(m_terrain_id , GROUP_LD , 0 , 0);

		if(m_TerrainGroup[SEARCH_LU] == GROUP_RU &&
			m_TerrainGroup[SEARCH_UP] == GROUP_R &&
			m_TerrainGroup[SEARCH_RU] == GROUP_R &&
			m_TerrainGroup[SEARCH_R] == GROUP_R &&
			m_TerrainGroup[SEARCH_RD] == GROUP_RU &&
			m_TerrainGroup[SEARCH_DOWN] == GROUP_RU &&
			m_TerrainGroup[SEARCH_LD] == GROUP_RU &&
			m_TerrainGroup[SEARCH_L] == GROUP_RD)
		{
			SetTerrainInfo(m_terrain_id , GROUP_RU , 0 , 1);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx - SQ_TILECNTX , m_sendTerrainInfo , m_bTerrainDelete);
			SetTerrainInfo(m_terrain_id , GROUP_RU , 1 , 1);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx - SQ_TILECNTX + 1 , m_sendTerrainInfo , m_bTerrainDelete);
			SetTerrainInfo(m_terrain_id , GROUP_RU , 2 , 0);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
			SetTerrainInfo(m_terrain_id , GROUP_RU , 3 , 1);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx + 1 , m_sendTerrainInfo , m_bTerrainDelete);
			SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx + SQ_TILECNTX , m_sendTerrainInfo , m_bTerrainDelete);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx + SQ_TILECNTX + 1, m_sendTerrainInfo , m_bTerrainDelete);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx + SQ_TILECNTX*2 , m_sendTerrainInfo , m_bTerrainDelete);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx + SQ_TILECNTX*2 + 1, m_sendTerrainInfo , m_bTerrainDelete);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx  - 1, m_sendTerrainInfo , m_bTerrainDelete);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx  - 2, m_sendTerrainInfo , m_bTerrainDelete);

			if(GROUP_RU == ptemp2->byGroup_ID)
			{
				SetTerrainInfo(m_terrain_id,  GROUP_FLAT , 0 , 0);
				CTileMgr::GetInstance()->SetTerrain(m_oriidx  - 1 + SQ_TILECNTX, m_sendTerrainInfo , m_bTerrainDelete);
				CTileMgr::GetInstance()->SetTerrain(m_oriidx  - 2 + SQ_TILECNTX, m_sendTerrainInfo , m_bTerrainDelete);
			}
			else
			{
				SetTerrainInfo(m_terrain_id , GROUP_LD , 1 , 0);
				CTileMgr::GetInstance()->SetTerrain(m_oriidx  - 1 + SQ_TILECNTX, m_sendTerrainInfo , m_bTerrainDelete);
			}

			return;
		}

	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{

	}
	else if(3 == m_cursequence)
	{

	}
	else if(4 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_RD , 4 , 0);
		CTileMgr::GetInstance()->SetTerrain(m_oriidx + SQ_TILECNTX, m_sendTerrainInfo , m_bTerrainDelete);
		SetTerrainInfo(m_terrain_id , GROUP_RD , 2 , 0);
	}
	else if(5 == m_cursequence)
	{

	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_L::OverlapSequence_R_3(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{
		TERRAIN_INFO* ptemp1 = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx - 2);
		TERRAIN_INFO* ptemp2 = CTileMgr::GetInstance()->GetTerrain_Info(m_oriidx - 2 + SQ_TILECNTX);

		SetTerrainInfo(m_terrain_id , GROUP_RU , 1 , 1);
		CTileMgr::GetInstance()->SetTerrain(m_oriidx - SQ_TILECNTX, m_sendTerrainInfo , m_bTerrainDelete);

		SetTerrainInfo(m_terrain_id , GROUP_RU , 3 , 1);
	}
	else if(2 == m_cursequence)
	{

	}
	else if(3 == m_cursequence)
	{

	}
	else if(4 == m_cursequence)
	{

	}
	else if(5 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_RD , 5 , 0);
		CTileMgr::GetInstance()->SetTerrain(m_oriidx + SQ_TILECNTX, m_sendTerrainInfo , m_bTerrainDelete);
		SetTerrainInfo(m_terrain_id , GROUP_RD , 3 , 0);
	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_L::OverlapSequence_R_4(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_LD , 4 , 0);
		CTileMgr::GetInstance()->SetTerrain(m_oriidx + SQ_TILECNTX, m_sendTerrainInfo , m_bTerrainDelete);
		SetTerrainInfo(m_terrain_id , GROUP_LD , 2 , 0);
	}
	else if(3 == m_cursequence)
	{

	}
	else if(4 == m_cursequence)
	{

	}
	else if(5 == m_cursequence)
	{

	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_L::OverlapSequence_R_5(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{

	}
	else if(3 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_LD , 5 , 0);
		CTileMgr::GetInstance()->SetTerrain(m_oriidx + SQ_TILECNTX, m_sendTerrainInfo , m_bTerrainDelete);
		SetTerrainInfo(m_terrain_id , GROUP_LD , 3 , 0);
	}
	else if(4 == m_cursequence)
	{

	}
	else if(5 == m_cursequence)
	{

	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_L::OverlapSequence_LD_0(void)
{
	if(0 == m_cursequence)
	{
		return;
	}
	else if(1 == m_cursequence)
	{
		
	}
	else if(2 == m_cursequence)
	{

	}
	else if(3 == m_cursequence)
	{

	}
	else if(4 == m_cursequence)
	{
		return;
	}
	else if(5 == m_cursequence)
	{

	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_L::OverlapSequence_LD_1(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{
		return;
	}
	else if(2 == m_cursequence)
	{

	}
	else if(3 == m_cursequence)
	{

	}
	else if(4 == m_cursequence)
	{

	}
	else if(5 == m_cursequence)
	{
		return;
	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_L::OverlapSequence_LD_2(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{
		return;
	}
	else if(3 == m_cursequence)
	{

	}
	else if(4 == m_cursequence)
	{

	}
	else if(5 == m_cursequence)
	{

	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_L::OverlapSequence_LD_3(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{

	}
	else if(3 == m_cursequence)
	{
		return;
	}
	else if(4 == m_cursequence)
	{

	}
	else if(5 == m_cursequence)
	{

	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_L::OverlapSequence_LD_4(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{

	}
	else if(3 == m_cursequence)
	{

	}
	else if(4 == m_cursequence)
	{
		//return;
	}
	else if(5 == m_cursequence)
	{

	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_L::OverlapSequence_LD_5(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{

	}
	else if(3 == m_cursequence)
	{

	}
	else if(4 == m_cursequence)
	{

	}
	else if(5 == m_cursequence)
	{
		//return;
	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

