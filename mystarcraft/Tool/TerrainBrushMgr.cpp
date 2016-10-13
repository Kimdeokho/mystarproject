#include "StdAfx.h"
#include "TerrainBrushMgr.h"
#include "TileMgr.h"
#include "MainFrm.h"
#include "MyForm.h"
#include "MyProSheet.h"
#include "MyProPage.h"
#include "HighDirt_Brush.h"
#include "LowerGround_Brush.h"

IMPLEMENT_SINGLETON(CTerrainBrushMgr);

CTerrainBrushMgr::CTerrainBrushMgr(void)
{
}

CTerrainBrushMgr::~CTerrainBrushMgr(void)
{
	Release();
}

HRESULT CTerrainBrushMgr::Initialize(void)
{
	for(int i = 0; i < FLOOR_END; ++i)
		m_Brush[i] = NULL;

	m_TerrainListBox = &((CMainFrame*)AfxGetMainWnd())->m_pMyFormView->m_pProSheet->m_page1.m_maptileListbox;

	m_Brush[UP_FLOOR_1] = new CHighDirt_Brush;
	m_Brush[DOWN_FLOOR_1] = new CLowerGround_Brush;

	m_curTerrainID = TERRAIN_DIRT;
	return S_OK;
}
int CTerrainBrushMgr::get_sqindex(void)
{
	int rbidx = CTileMgr::GetInstance()->GetRbIdx();

	int sqidx = 0;
	if( (rbidx/RB_TILECNTX) % 2 == 0 )
		sqidx = rbidx*4 - (rbidx/RB_TILECNTX)*4;
	else
		sqidx = rbidx*4 - ((rbidx/RB_TILECNTX)*4 - 2);

	return sqidx;
}
HRESULT CTerrainBrushMgr::TerrainCheck(void)
{


	int boxidx = m_TerrainListBox->GetCurSel();

	//CString	str_terrainID;

	//if(boxidx >= 0)
	//	m_TerrainListBox->GetText(boxidx , str_terrainID);
	//else
	//	return E_FAIL;

	//if(InitBrush(str_terrainID))
	//	return E_FAIL;


	int icase = this->FloorCheck();

	if(icase == -2)
	{
		/*2단계 낮출때 */
	}
	if(icase == -1)
	{
		/*1단계 낮출때*/
		m_Brush[DOWN_FLOOR_1]->SetTerrain_ID(m_curTerrainID);
		m_Brush[DOWN_FLOOR_1]->BrushPaint();
	}
	if(icase == 1)
	{
		/*1단계 올릴때*/
		m_Brush[UP_FLOOR_1]->SetTerrain_ID(m_curTerrainID);
		m_Brush[UP_FLOOR_1]->BrushPaint();
	}
	if(icase == 2)
	{
		/*2단계 올릴때*/
	}


	return S_OK;
}
int CTerrainBrushMgr::FloorCheck(void)
{
	int sqidx = get_sqindex();
	int iindex = 0;
	int icase = 0;

	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			iindex = sqidx - 130 + (i*SQ_TILECNTX+j);

			if(iindex >= 0 && iindex < SQ_TILECNTX*SQ_TILECNTY)
			{
				icase = CTileMgr::GetInstance()->FloorCheck(iindex , m_curTerrainID);
				if(icase != 0)
					return icase;
			}
		}
	}

	return 0;
}
void CTerrainBrushMgr::High_DirtBrush(const int idx)/*브러쉬 칠하기로 대체*/
{
	TILE	temptile;
	TERRAIN_INFO	terrain_info;;
}
HRESULT	CTerrainBrushMgr::InitBrush(const CString&	_str)
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

void CTerrainBrushMgr::Release(void)
{
	for(int i = 0; i < FLOOR_END; ++i)
	{
		Safe_Delete(m_Brush[i]);
	}

	int a = 0;
}

void CTerrainBrushMgr::SetTerrain_ID(int id)
{
	m_curTerrainID = id;
}

