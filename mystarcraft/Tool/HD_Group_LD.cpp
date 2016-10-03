#include "StdAfx.h"
#include "HD_Group_LD.h"

#include "MyBrush.h"
#include "TileMgr.h"
#include "TerrainBrushMgr.h"
CHD_Group_LD::CHD_Group_LD(void)
{
	m_terrain_id = TERRAIN_HIGHDIRT;
	m_startidx = 0;
	m_oriidx = 0;
	m_cursequence = 0;
	ZeroMemory(&m_sendTerrainInfo , sizeof(TERRAIN_INFO));
	m_oriTerrainInfo = NULL;
}

CHD_Group_LD::~CHD_Group_LD(void)
{
}
void CHD_Group_LD::MakeTerrain_Group(int istartidx)
{
	m_startidx = istartidx;

	int tempidx = 0;
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			m_cursequence = i*2+j;
			tempidx = i * SQ_TILECNTX + j;
			m_oriidx = m_startidx + tempidx;
			m_bTerrainDelete = true;

			SetTerrainInfo(TERRAIN_HIGHDIRT , GROUP_LD , m_cursequence , 0);

			Group_LD_Algorithm();
		}
	}
}
void CHD_Group_LD::Group_LD_Algorithm()
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
void CHD_Group_LD::OverlapGroup_L(void)
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
void CHD_Group_LD::OverlapGroup_LU(void)
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

void CHD_Group_LD::OverlapGroup_RU(void)
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

void CHD_Group_LD::OverlapGroup_R(void)
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

void CHD_Group_LD::OverlapGroup_RD(void)
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

void CHD_Group_LD::OverlapGroup_LD(void)
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

void CHD_Group_LD::OverlapSequence_L_0(void)
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
		SetTerrainInfo(m_terrain_id , GROUP_LD , m_cursequence,  0);
		m_bTerrainDelete = false;
	}
	else if(5 == m_cursequence)
	{

	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_LD::OverlapSequence_L_1(void)
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
		SetTerrainInfo(m_terrain_id , GROUP_LD , m_cursequence,  0);
		m_bTerrainDelete = false;
	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_LD::OverlapSequence_L_2(void)
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
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_LD::OverlapSequence_L_3(void)
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
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_LD::OverlapSequence_L_4(void)
{
	if(0 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_LD , m_cursequence , 0);
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

void CHD_Group_LD::OverlapSequence_L_5(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_LD , m_cursequence , 0);
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



void CHD_Group_LD::OverlapSequence_LU_0(void)
{
	/*설치하려는 그룹 L이 기존의 LU와 겹칠때*/
	if(0 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_LD , m_cursequence , 0);
		if(m_MyBrush->GetOverlap_GroupArea(GROUP_L , GROUP_RU))
		{
			SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);
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
		m_bTerrainDelete = false;
	}
	else if(5 == m_cursequence)
	{

	}

	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_LD::OverlapSequence_LU_1(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_LD , m_cursequence , 0);
		if(m_MyBrush->GetOverlap_GroupArea(GROUP_L , GROUP_RU))
		{
			SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);
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
		m_bTerrainDelete = false;
	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_LD::OverlapSequence_LU_2(void)
{
	if(0 == m_cursequence)
	{
	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{
		//SetTerrainInfo(m_terrain_id , GROUP_LU , m_cursequence , 1);
		//CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
		SetTerrainInfo(m_terrain_id , GROUP_LD , m_cursequence , 0);
		m_bTerrainDelete = false;

		if(m_MyBrush->GetOverlap_GroupArea(GROUP_L , GROUP_RU))
		{
			SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);
			m_bTerrainDelete = true;
		}
	}
	else if(3 == m_cursequence)
	{

	}
	else if(4 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_LU , 2, 1);
	}
	else if(5 == m_cursequence)
	{

	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_LD::OverlapSequence_LU_3(void)
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
		SetTerrainInfo(m_terrain_id , GROUP_LU , m_cursequence , 0);
		if(m_MyBrush->GetOverlap_GroupArea(GROUP_L , GROUP_RU))
		{
			SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);
		}
	}
	else if(4 == m_cursequence)
	{

	}
	else if(5 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_LU , 3, 0);
	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}
void CHD_Group_LD::OverlapSequence_RU_0(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_LD , m_cursequence , 0);
		m_bTerrainDelete = false;
		CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
		SetTerrainInfo(m_terrain_id , GROUP_LU , 8 , 1);

		if(m_MyBrush->GetOverlap_GroupArea(GROUP_R , GROUP_LU) && 
			m_MyBrush->GetOverlap_GroupArea(GROUP_L , GROUP_RU))
		{
			m_bTerrainDelete = true;
			SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0 );
			CTileMgr::GetInstance()->SetTerrain(m_oriidx - SQ_TILECNTX, m_sendTerrainInfo , m_bTerrainDelete);
		}
		else if(m_MyBrush->GetOverlap_GroupArea(GROUP_L , GROUP_RU))
		{
			m_bTerrainDelete = true;
			SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx - SQ_TILECNTX, m_sendTerrainInfo , m_bTerrainDelete);
			SetTerrainInfo(m_terrain_id , GROUP_RD , 0 , 0);
		}
		else if(m_MyBrush->GetOverlap_GroupArea(GROUP_L , GROUP_LU) &&
			m_MyBrush->GetOverlap_GroupArea(GROUP_RD , GROUP_LU))
		{
			m_bTerrainDelete = true;
			SetTerrainInfo(m_terrain_id , GROUP_LU , 2, 1);
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

void CHD_Group_LD::OverlapSequence_RU_1(void)
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
		SetTerrainInfo(m_terrain_id , GROUP_RD , 1 , 0);

		if(m_MyBrush->GetOverlap_GroupArea(GROUP_R , GROUP_LU) && 
			m_MyBrush->GetOverlap_GroupArea(GROUP_L , GROUP_RU))
		{
			SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx - SQ_TILECNTX, m_sendTerrainInfo , m_bTerrainDelete);
		}
		else if(m_MyBrush->GetOverlap_GroupArea(GROUP_L , GROUP_RU))
		{
			SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx - SQ_TILECNTX, m_sendTerrainInfo , m_bTerrainDelete);
			SetTerrainInfo(m_terrain_id , GROUP_RD , 1 , 0);
		}
		else if(m_MyBrush->GetOverlap_GroupArea(GROUP_L , GROUP_LU) &&
			m_MyBrush->GetOverlap_GroupArea(GROUP_RD , GROUP_LU))
		{
			SetTerrainInfo(m_terrain_id , GROUP_LU , 3, 0);
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

void CHD_Group_LD::OverlapSequence_RU_2(void)
{
	if(0 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);
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
		if(m_MyBrush->GetOverlap_GroupArea(GROUP_L , GROUP_RU) && 
			m_MyBrush->GetOverlap_GroupArea(GROUP_R , GROUP_LU))
		{
			SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
		}
		else if(m_MyBrush->GetOverlap_GroupArea(GROUP_L , GROUP_LU) &&
			m_MyBrush->GetOverlap_GroupArea(GROUP_RD , GROUP_LU))
		{
			SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);
			CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
		}
		return;
	}
	else if(5 == m_cursequence)
	{

	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_LD::OverlapSequence_RU_3(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);
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
		m_bTerrainDelete = false;

		if(m_MyBrush->GetOverlap_GroupArea(GROUP_L , GROUP_RU) && 
			m_MyBrush->GetOverlap_GroupArea(GROUP_R , GROUP_LU))
		{
			m_bTerrainDelete = true;
			SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);
		}
		else if(m_MyBrush->GetOverlap_GroupArea(GROUP_L , GROUP_LU) &&
			m_MyBrush->GetOverlap_GroupArea(GROUP_RD , GROUP_LU))
		{
			m_bTerrainDelete = true;
			SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);
		}
	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_LD::OverlapSequence_RD_0(void)
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

void CHD_Group_LD::OverlapSequence_RD_1(void)
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
}

void CHD_Group_LD::OverlapSequence_RD_2(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{
		//SetTerrainInfo(m_terrain_id , GROUP_LD , 2 , 0);
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

void CHD_Group_LD::OverlapSequence_RD_3(void)
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
		return;
	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_LD::OverlapSequence_RD_4(void)
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
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_LD::OverlapSequence_RD_5(void)
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
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_LD::OverlapSequence_R_0(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);
		CTileMgr::GetInstance()->SetTerrain(m_oriidx -SQ_TILECNTX , m_sendTerrainInfo , m_bTerrainDelete);
		SetTerrainInfo(m_terrain_id , GROUP_RD , 0 , 0);
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

void CHD_Group_LD::OverlapSequence_R_1(void)
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
		SetTerrainInfo(m_terrain_id , GROUP_FLAT , 0 , 0);
		CTileMgr::GetInstance()->SetTerrain(m_oriidx -SQ_TILECNTX , m_sendTerrainInfo , m_bTerrainDelete);
		SetTerrainInfo(m_terrain_id , GROUP_RD , 1 , 0);
	}
	else if(4 == m_cursequence)
	{

	}
	else if(5 == m_cursequence)
	{

	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_LD::OverlapSequence_R_2(void)
{
	if(0 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_LD , m_cursequence , 0);
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
		CTileMgr::GetInstance()->SetTerrain(m_oriidx + SQ_TILECNTX , m_sendTerrainInfo , m_bTerrainDelete);
		SetTerrainInfo(m_terrain_id , GROUP_RD , 2 , 0);
	}
	else if(5 == m_cursequence)
	{

	}

	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_LD::OverlapSequence_R_3(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_LD , m_cursequence , 0);
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
		CTileMgr::GetInstance()->SetTerrain(m_oriidx + SQ_TILECNTX , m_sendTerrainInfo , m_bTerrainDelete);
		SetTerrainInfo(m_terrain_id , GROUP_RD , 3 , 0);
	}

	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_LD::OverlapSequence_R_4(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{

	}
	else if(2 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_LD , m_cursequence , 0);
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

void CHD_Group_LD::OverlapSequence_R_5(void)
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
		SetTerrainInfo(m_terrain_id , GROUP_LD , m_cursequence , 0);
	}
	else if(4 == m_cursequence)
	{

	}
	else if(5 == m_cursequence)
	{

	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_LD::OverlapSequence_LD_0(void)
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
		SetTerrainInfo(m_terrain_id , GROUP_LD , 0 , 0);
	}
	else if(5 == m_cursequence)
	{
		
	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_LD::OverlapSequence_LD_1(void)
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
		SetTerrainInfo(m_terrain_id , GROUP_LD , 1 , 0);
	}
	CTileMgr::GetInstance()->SetTerrain(m_oriidx , m_sendTerrainInfo , m_bTerrainDelete);
}

void CHD_Group_LD::OverlapSequence_LD_2(void)
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

void CHD_Group_LD::OverlapSequence_LD_3(void)
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

void CHD_Group_LD::OverlapSequence_LD_4(void)
{
	if(0 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_LD , 0 , 0);
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

void CHD_Group_LD::OverlapSequence_LD_5(void)
{
	if(0 == m_cursequence)
	{

	}
	else if(1 == m_cursequence)
	{
		SetTerrainInfo(m_terrain_id , GROUP_LD , 1 , 0);
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

