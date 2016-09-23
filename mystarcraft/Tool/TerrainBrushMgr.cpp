#include "StdAfx.h"
#include "TerrainBrushMgr.h"
#include "TileMgr.h"
#include "MainFrm.h"
#include "MyForm.h"
#include "MyProSheet.h"
#include "MyProPage.h"
#include "HighDirt_Brush.h"

IMPLEMENT_SINGLETON(CTerrainBrushMgr);

CTerrainBrushMgr::CTerrainBrushMgr(void)
{
	for(int i = 0; i < TERRAIN_END; ++i)
		m_Brush[i] = NULL;
}

CTerrainBrushMgr::~CTerrainBrushMgr(void)
{
	Release();
}

HRESULT CTerrainBrushMgr::Initialize(void)
{
	m_TerrainListBox = &((CMainFrame*)AfxGetMainWnd())->m_pMyFormView->m_pProSheet->m_page1.m_maptileListbox;

	m_Brush[TERRAIN_HIGHDIRT] = new CHighDirt_Brush;
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
	int sqidx = get_sqindex();


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
				icase = CTileMgr::GetInstance()->FloorCheck(iindex , m_curFloor);
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
		m_Brush[m_curTerrainID]->BrushPaint();
	}
	else if(icase == 2)
	{
		/*2단계 올릴때*/
	}


	return S_OK;
}
void CTerrainBrushMgr::High_DirtBrush(const int idx)/*브러쉬 칠하기로 대체*/
{
	TILE	temptile;
	TERRAIN_INFO	terrain_info;

	//LeftArea(idx - 132 , temptile , terrain_info);
	//LeftUpArea(idx - 258 , temptile , terrain_info) ;
	//RightUpArea(idx - 256 , temptile , terrain_info);
	//RightArea(idx - 126 , temptile , terrain_info);
	//RightDown(idx , temptile , terrain_info);
	//LeftDown(idx - 2 , temptile , terrain_info);
}
bool CTerrainBrushMgr::Overlap_Group(const int irow ,const int icol ,const int startidx ,const int group_id)
{
	const TERRAIN_INFO* pterrain_temp = NULL;
	int tempidx = 0;
	//for(int i = 0; i < irow; ++i)
	{
		//for(int j = 0; j < icol; ++j)
		{
			//tempidx = i*SQ_TILECNTX +j;
			pterrain_temp = CTileMgr::GetInstance()->GetTerrain_Info(startidx);

			if( group_id == pterrain_temp->byGroup_ID )
				return true;
		}
	}
	return false;
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
	for(int i = 0; i < TERRAIN_END; ++i)
	{
		Safe_Delete(m_Brush[i]);
	}
}

