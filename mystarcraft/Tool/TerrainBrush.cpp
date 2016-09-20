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
void CTerrainBrush::LeftArea(const int startidx, TILE& temptile, TERRAIN_INFO& _terrain_info)
{
	TERRAIN_INFO*	getterrain_temp = NULL;
	int tempidx = 0;
	bool bdelete = false;


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

			
			_terrain_info.byTerrain_ID = m_curTerrainID;
			_terrain_info.byGroup_ID = GROUP_L;
			_terrain_info.byGroup_sequence = i*2+j;

			if(i*2+j >= 0 && i*2+j <= 2)
				_terrain_info.bysortLV = 1;
			else
				_terrain_info.bysortLV = 0;

			getterrain_temp = CTileMgr::GetInstance()->GetTerrain_Info(startidx + tempidx);
			if(m_curTerrainID == getterrain_temp->byTerrain_ID)
				if(GROUP_RD == getterrain_temp->byGroup_ID ||
					GROUP_LD == getterrain_temp->byGroup_ID ||
					GROUP_RU == getterrain_temp->byGroup_ID ||
					GROUP_LU == getterrain_temp->byGroup_ID ||
					GROUP_FLAT == getterrain_temp->byGroup_ID)
					continue;

			CTileMgr::GetInstance()->SetTerrain(startidx + tempidx , temptile, _terrain_info , bdelete);
		}
	}
}
void CTerrainBrush::LeftUpArea(const int startidx, TILE& temptile , TERRAIN_INFO& _terrain_info)
{
	TERRAIN_INFO*	getterrain_temp = NULL;
	int tempidx = 0;
	int isequenceidx = 0;

	temptile.byOption = 1;

	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			tempidx = i * SQ_TILECNTX + j;
			isequenceidx = i*2+j;

			_terrain_info.byTerrain_ID = m_curTerrainID;
			_terrain_info.byGroup_ID = GROUP_LU;
			_terrain_info.byGroup_sequence = isequenceidx;

			if(0 == isequenceidx)
				temptile.byFloor = m_curFloor - 1;
			else
				temptile.byFloor = m_curFloor;

			if(isequenceidx != 3)
				_terrain_info.bysortLV = 1;
			else
				_terrain_info.bysortLV = 0;



			getterrain_temp = CTileMgr::GetInstance()->GetTerrain_Info(startidx + tempidx);

			if( GROUP_RU == getterrain_temp->byGroup_ID)
			{
				if( 2 == getterrain_temp->byGroup_sequence ||
					3 == getterrain_temp->byGroup_sequence &&
					isequenceidx <= 1)
					continue;
			}
			else if(GROUP_RD == getterrain_temp->byGroup_ID)
			{
				if(getterrain_temp->byGroup_sequence <= 1 && isequenceidx >= 2)
				{
					_terrain_info.byGroup_ID = GROUP_FLAT;
					_terrain_info.byGroup_sequence = 0;
				}
			}
			else if(GROUP_LD == getterrain_temp->byGroup_ID)
			{
				if(isequenceidx <= 1 &&
					getterrain_temp->byGroup_sequence <= 1)
					continue;
			}
			else if( GROUP_FLAT == getterrain_temp->byGroup_ID &&
				m_curTerrainID == getterrain_temp->byTerrain_ID)
				continue;

			CTileMgr::GetInstance()->SetTerrain(startidx + tempidx , temptile, _terrain_info);
		}
	}
}
void CTerrainBrush::RightUpArea(const int startidx, TILE& temptile , TERRAIN_INFO& _terrain_info)
{
	int tempidx = 0;
	int isequenceidx = 0;

	temptile.byOption = 1;

	TERRAIN_INFO*	getterrain_temp = NULL;

	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			tempidx = i * SQ_TILECNTX + j;
			isequenceidx = i*2+j;

			_terrain_info.byTerrain_ID = m_curTerrainID;
			_terrain_info.byGroup_ID = GROUP_RU;
			_terrain_info.byGroup_sequence = isequenceidx;

			/*층 대입*/
			if(isequenceidx == 1)
				temptile.byFloor = m_curFloor - 1;
			else
				temptile.byFloor = m_curFloor;

			/*소팅레벨 대입*/
			if(isequenceidx != 2)	
				_terrain_info.bysortLV = 1;
			else
				_terrain_info.bysortLV = 0;

			getterrain_temp = CTileMgr::GetInstance()->GetTerrain_Info(startidx + tempidx);

			if( GROUP_LU == getterrain_temp->byGroup_ID)
			{
				if( 2 == getterrain_temp->byGroup_sequence ||
					3 == getterrain_temp->byGroup_sequence &&
					isequenceidx <= 1)
					continue;
			}
			else if(GROUP_LD == getterrain_temp->byGroup_ID)
			{
				if(getterrain_temp->byGroup_sequence <= 1 && isequenceidx >= 2)
				{
					_terrain_info.byGroup_ID = GROUP_FLAT;
					_terrain_info.byGroup_sequence = 0;
				}
			}
			else if(GROUP_RD == getterrain_temp->byGroup_ID)
			{
				if(isequenceidx <= 1 &&
					getterrain_temp->byGroup_sequence <= 1)
					continue;
			}
			else if( GROUP_FLAT == getterrain_temp->byGroup_ID &&
				m_curTerrainID == getterrain_temp->byTerrain_ID)
				continue;

			CTileMgr::GetInstance()->SetTerrain(startidx + tempidx , temptile, _terrain_info);
		}
	}
}
void CTerrainBrush::RightArea(const int startidx, TILE& temptile , TERRAIN_INFO& _terrain_info)
{
	TERRAIN_INFO*	getterrain_temp = NULL;
	int tempidx = 0;
	int isquenceidx = 0;
	bool bdelete = false;

	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			tempidx = i * SQ_TILECNTX + j;
			isquenceidx = i*2+j;

			if(i == 2)
				temptile.byFloor = m_curFloor - 1;
			else
				temptile.byFloor = m_curFloor;

			_terrain_info.byGroup_ID = GROUP_R;
			_terrain_info.byGroup_sequence = isquenceidx;
			_terrain_info.byTerrain_ID = m_curTerrainID;

			if( i == 0 || isquenceidx == 3)
				_terrain_info.bysortLV = 1;
			else
				_terrain_info.bysortLV = 0;

			getterrain_temp = CTileMgr::GetInstance()->GetTerrain_Info(startidx + tempidx);

			if(TERRAIN_HIGHDIRT == getterrain_temp->byTerrain_ID)
			if(GROUP_RD == getterrain_temp->byGroup_ID ||
				GROUP_LD == getterrain_temp->byGroup_ID ||
				GROUP_RU == getterrain_temp->byGroup_ID ||
				GROUP_LU == getterrain_temp->byGroup_ID ||
				GROUP_FLAT == getterrain_temp->byGroup_ID)
				continue;

			CTileMgr::GetInstance()->SetTerrain(startidx + tempidx , temptile, _terrain_info , bdelete);
		}
	}
}
void CTerrainBrush::RightDown(const int startidx, TILE& temptile , TERRAIN_INFO& _terrain_info)
{
	int tempidx = 0;
	TERRAIN_INFO*	get_terrain = NULL;
	int isquenceidx = 0;
	bool bdelete = true;

	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			tempidx = i*SQ_TILECNTX + j;
			isquenceidx = i*2+j;

			if(i >= 1)
				temptile.byFloor = m_curFloor - 1;
			else
				temptile.byFloor = m_curFloor;

			_terrain_info.byGroup_ID = GROUP_RD;
			_terrain_info.byGroup_sequence = isquenceidx;
			_terrain_info.byTerrain_ID = m_curTerrainID;
			_terrain_info.bysortLV = 0;

			get_terrain = CTileMgr::GetInstance()->GetTerrain_Info(startidx + tempidx);

			if(GROUP_LU == get_terrain->byGroup_ID)
			{
				if(	get_terrain->byGroup_sequence <=3 &&
					isquenceidx <= 1)
				{
					_terrain_info.byGroup_ID = GROUP_FLAT;
					_terrain_info.byGroup_sequence = 0;
				}
			}
			else if(GROUP_LD == get_terrain->byGroup_ID)
			{
				if(0 <= get_terrain->byGroup_sequence &&
					get_terrain->byGroup_sequence <= 3 &&
					2 <= isquenceidx && isquenceidx <= 5)
					break;
			}
			else if(GROUP_RU == get_terrain->byGroup_ID)
			{
				if(isquenceidx <= 1 && get_terrain->byGroup_sequence <= 1)
					bdelete = true;
				else if(3 == isquenceidx && get_terrain->byGroup_sequence == 3)
					bdelete = false;
				else if(isquenceidx == 2)
					continue;

			}
			else if(GROUP_RD == get_terrain->byGroup_ID)
			{
				if(isquenceidx >= 4)
					break;
			}
			else if(GROUP_FLAT == get_terrain->byGroup_ID &&
				m_curTerrainID == get_terrain->byTerrain_ID)
				continue;

			CTileMgr::GetInstance()->SetTerrain(startidx + tempidx , temptile, _terrain_info , bdelete);
		}
	}
}
void CTerrainBrush::LeftDown(const int startidx, TILE& temptile , TERRAIN_INFO& _terrain_info)
{
	int tempidx = 0;
	TERRAIN_INFO*	get_terrain = NULL;
	int isquenceidx = 0;
	bool bdelete = true;

	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			tempidx = i * SQ_TILECNTX + j;
			isquenceidx = i*2+j;

			if(i >= 1)
				temptile.byFloor = m_curFloor - 1;
			else
				temptile.byFloor = m_curFloor;

			_terrain_info.byGroup_ID = GROUP_LD;
			_terrain_info.byGroup_sequence = isquenceidx;
			_terrain_info.byTerrain_ID = m_curTerrainID;
			_terrain_info.bysortLV = 0;


			get_terrain = CTileMgr::GetInstance()->GetTerrain_Info(startidx + tempidx);

			if(GROUP_RU == get_terrain->byGroup_ID)
			{
				if(	get_terrain->byGroup_sequence <=3 &&
					isquenceidx <= 1)
				{
					_terrain_info.byGroup_ID = GROUP_FLAT;
					_terrain_info.byGroup_sequence = 0;
				}
			}
			else if(GROUP_RD == get_terrain->byGroup_ID)
			{
				if(0 <= get_terrain->byGroup_sequence &&
					get_terrain->byGroup_sequence <= 3 &&
					2 <= isquenceidx && isquenceidx <= 5)
					break;
			}
			else if(GROUP_LU == get_terrain->byGroup_ID)
			{
				if(isquenceidx <= 1 && get_terrain->byGroup_sequence <= 1)
					bdelete = true;
				else if(2 == isquenceidx && get_terrain->byGroup_sequence == 2)
					bdelete = false;
				else if(isquenceidx == 3)
					break;

			}
			else if(GROUP_LD == get_terrain->byGroup_ID)
			{
				if(isquenceidx >= 4)
					break;
			}
			else if(GROUP_FLAT == get_terrain->byGroup_ID &&
				m_curTerrainID == get_terrain->byTerrain_ID)
				continue;

			CTileMgr::GetInstance()->SetTerrain(startidx + tempidx , temptile , _terrain_info , bdelete);
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

