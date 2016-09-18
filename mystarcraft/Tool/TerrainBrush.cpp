#include "StdAfx.h"
#include "TerrainBrush.h"
#include "TileMgr.h"
#include "MainFrm.h"
#include "MyForm.h"
#include "MyProSheet.h"
#include "MyProPage.h"

IMPLEMENT_SINGLETON(CTerrainBrush);

CTerrainBrush::CTerrainBrush(void)
{
}

CTerrainBrush::~CTerrainBrush(void)
{
}

HRESULT CTerrainBrush::Initialize(void)
{
	m_TerrainListBox = &((CMainFrame*)AfxGetMainWnd())->m_pMyFormView->m_pProSheet->m_page1.m_maptileListbox;
	return S_OK;
}
HRESULT CTerrainBrush::TerrainCheck(void)
{
	int rbidx = CTileMgr::GetInstance()->GetRbIdx();

	int sqidx = 0;
	if( (rbidx/RB_TILECNTX) % 2 == 0 )
		sqidx = rbidx*4 - (rbidx/RB_TILECNTX)*4;
	else
	{
		sqidx = rbidx*4 - ((rbidx/RB_TILECNTX)*4 - 2);
	}


	int boxidx = m_TerrainListBox->GetCurSel();

	CString	str_terrainID;
	if(boxidx >= 0)
		m_TerrainListBox->GetText(boxidx , str_terrainID);
	else
		return E_FAIL;

	if(InitBrush(str_terrainID))
		return E_FAIL;


	int iindex = 0;
	int icase = 0;
	/* 먼저 2행4열의 범위의 층수를 검사한다.*/
	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			iindex = sqidx - 130 + (i*SQ_TILECNTX+j);

			if(iindex >= 0 && iindex < SQ_TILECNTX*SQ_TILECNTY)
			{
				icase = CTileMgr::GetInstance()->TileCheck(iindex , m_curFloor);
				if(icase != 0)
					break;
			}
		}
		if(icase != 0)
			break;
	}

	if(icase == -2)
	{
		/*2단계 낮출때 */
	}
	else if(icase == -1)
	{
		/*1단계 낮출때*/
	}
	else if(icase == 1)
	{
		/*1단계 올릴때*/

		if( !str_terrainID.Compare(L"High_dirt") )
		{
			High_DirtBrush(sqidx);
		}
	}
	else if(icase == 2)
	{
		/*2단계 올릴때*/
	}


	return S_OK;
}
void CTerrainBrush::High_DirtBrush(const int idx)
{
	TILE	temptile;
	TERRAIN_INFO	terrain_info;

	LeftArea(idx - 132 , temptile , terrain_info);
	LeftUpArea(idx - 258 , temptile , terrain_info) ;
	RightUpArea(idx - 256 , temptile , terrain_info);
	RightArea(idx - 126 , temptile , terrain_info);
	RightDown(idx , temptile , terrain_info);
	LeftDown(idx - 2 , temptile , terrain_info);
}
void CTerrainBrush::LeftArea(const int startidx, TILE& temptile, TERRAIN_INFO& pterrain_info)
{
	int tempidx = 0;


	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			tempidx = i*SQ_TILECNTX + j;
			if(i == 2)
				temptile.byFloor = m_curFloor - 1;
			else
				temptile.byFloor = m_curFloor;

			if(i*2+j == 0)
				temptile.byOption = 0;
			else
				temptile.byOption = 1;

			
			pterrain_info.byTerrain_ID = m_curTerrainID;
			pterrain_info.byGroup_ID = GROUP_L;
			pterrain_info.byGroup_sequence = i*2+j;

			if(i*2+j >= 0 && i*2+j <= 2)
				pterrain_info.bysortLV = 1;
			else
				pterrain_info.bysortLV = 0;

			CTileMgr::GetInstance()->SetTerrain(startidx + tempidx , temptile, pterrain_info);
		}
	}
}
void CTerrainBrush::LeftUpArea(const int startidx, TILE& temptile , TERRAIN_INFO& pterrain_info)
{
	int tempidx = 0;

	temptile.byFloor = m_curFloor;
	temptile.byOption = 1;

	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			tempidx = i * SQ_TILECNTX + j;

			pterrain_info.byTerrain_ID = m_curTerrainID;
			pterrain_info.byGroup_ID = GROUP_LU;
			pterrain_info.byGroup_sequence = i*2+j;

			if(i*2+j != 3)
				pterrain_info.bysortLV = 1;
			else
				pterrain_info.bysortLV = 0;

			CTileMgr::GetInstance()->SetTerrain(startidx + tempidx , temptile, pterrain_info);
		}
	}
}
void CTerrainBrush::RightUpArea(const int startidx, TILE& temptile , TERRAIN_INFO& pterrain_info)
{
	int tempidx = 0;

	temptile.byFloor = m_curFloor;
	temptile.byOption = 1;

	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			tempidx = i * SQ_TILECNTX + j;

			pterrain_info.byTerrain_ID = m_curTerrainID;
			pterrain_info.byGroup_ID = GROUP_RU;
			pterrain_info.byGroup_sequence = i*2+j;

			if(i*2+j != 2)	
				pterrain_info.bysortLV = 1;
			else
				pterrain_info.bysortLV = 0;

			CTileMgr::GetInstance()->SetTerrain(startidx + tempidx , temptile, pterrain_info);
		}
	}
}
void CTerrainBrush::RightArea(const int startidx, TILE& temptile , TERRAIN_INFO& pterrain_info)
{
	int tempidx = 0;

	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			tempidx = i * SQ_TILECNTX + j;
			if(i == 2)
				temptile.byFloor = m_curFloor - 1;
			else
				temptile.byFloor = m_curFloor;

			pterrain_info.byGroup_ID = GROUP_R;
			pterrain_info.byGroup_sequence = i*2+j;
			pterrain_info.byTerrain_ID = m_curTerrainID;

			if( i == 0 || i*2+j == 3)
				pterrain_info.bysortLV = 1;
			else
				pterrain_info.bysortLV = 0;

			CTileMgr::GetInstance()->SetTerrain(startidx + tempidx , temptile, pterrain_info);
		}
	}
}
void CTerrainBrush::RightDown(const int startidx, TILE& temptile , TERRAIN_INFO& pterrain_info)
{
	int tempidx = 0;

	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			tempidx = i*SQ_TILECNTX + j;
			if(i == 2)
				temptile.byFloor = m_curFloor - 1;
			else
				temptile.byFloor = m_curFloor;

			pterrain_info.byGroup_ID = GROUP_RD;
			pterrain_info.byGroup_sequence = i*2+j;
			pterrain_info.byTerrain_ID = m_curTerrainID;
			pterrain_info.bysortLV = 0;

			CTileMgr::GetInstance()->SetTerrain(startidx + tempidx , temptile, pterrain_info);
		}
	}
}
void CTerrainBrush::LeftDown(const int startidx, TILE& temptile , TERRAIN_INFO& pterrain_info)
{
	int tempidx = 0;

	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			tempidx = i * SQ_TILECNTX + j;
			if(i == 2)
				temptile.byFloor = m_curFloor - 1;
			else
				temptile.byFloor = m_curFloor;

			pterrain_info.byGroup_ID = GROUP_LD;
			pterrain_info.byGroup_sequence = i*2+j;
			pterrain_info.byTerrain_ID = m_curTerrainID;
			pterrain_info.bysortLV = 0;

			CTileMgr::GetInstance()->SetTerrain(startidx + tempidx , temptile , pterrain_info);
		}
	}
}
HRESULT	CTerrainBrush::InitBrush(const CString&	_str)
{
	if(!_str.Compare(L"Dirt"))
	{
		m_curFloor = 1;
		m_curTerrainID = TERRAIN_DIRT;
	}
	else if(!_str.Compare(L"High_dirt"))
	{
		m_curFloor = 2;
		m_curTerrainID = TERRAIN_HIGHDIRT;
	}

	return S_OK;
}

