#include "StdAfx.h"
#include "TerrainBrushMgr.h"
#include "TileMgr.h"
#include "MainFrm.h"
#include "MyForm.h"
#include "MyProSheet.h"
#include "MyProPage.h"

IMPLEMENT_SINGLETON(CTerrainBrushMgr);

CTerrainBrushMgr::CTerrainBrushMgr(void)
{
}

CTerrainBrushMgr::~CTerrainBrushMgr(void)
{
}

HRESULT CTerrainBrushMgr::Initialize(void)
{
	m_TerrainListBox = &((CMainFrame*)AfxGetMainWnd())->m_pMyFormView->m_pProSheet->m_page1.m_maptileListbox;
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
void CTerrainBrushMgr::High_DirtBrush(const int idx)
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
void CTerrainBrushMgr::LeftArea(const int startidx, TILE& temptile, TERRAIN_INFO& _terrain_info)
{
	TERRAIN_INFO*	getterrain_temp = NULL;
	int tempidx = 0;
	bool bdelete = false;
	int isequnceidx = 0;


	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			tempidx = i*SQ_TILECNTX + j;
			isequnceidx = i*2+j;

			if(i >= 1)
				temptile.byFloor = m_curFloor - 1;
			else
				temptile.byFloor = m_curFloor;

			if(isequnceidx == 0)
				temptile.byOption = MOVE_OK;
			else
				temptile.byOption = MOVE_NONE;


			_terrain_info.byTerrain_ID = m_curTerrainID;
			_terrain_info.byGroup_ID = GROUP_L;
			_terrain_info.byGroup_sequence = isequnceidx;

			if(isequnceidx >= 0 && isequnceidx <= 2)
				_terrain_info.bysortLV = 1;
			else
				_terrain_info.bysortLV = 0;

			if(m_curTerrainID == TERRAIN_HIGHDIRT)
				HD_L_Algorithm(temptile , _terrain_info , startidx + tempidx , isequnceidx);
		}
	}
}
void CTerrainBrushMgr::LeftUpArea(const int startidx, TILE& temptile , TERRAIN_INFO& _terrain_info)
{
	int tempidx = 0;
	int isequenceidx = 0;

	temptile.byOption = MOVE_NONE;

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


			if(m_curTerrainID == TERRAIN_HIGHDIRT)
			{
				HD_LU_Algorithm(temptile , _terrain_info , startidx + tempidx , isequenceidx);
			}
		}
	}
}

void CTerrainBrushMgr::RightUpArea(const int startidx, TILE& temptile , TERRAIN_INFO& _terrain_info)
{
	int tempidx = 0;
	int isequenceidx = 0;

	temptile.byOption = MOVE_NONE;



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


			if(m_curTerrainID == TERRAIN_HIGHDIRT)
			{
				HD_RU_Algorithm(temptile , _terrain_info , startidx + tempidx , isequenceidx);
			}


		}
	}
}

void CTerrainBrushMgr::RightArea(const int startidx, TILE& temptile , TERRAIN_INFO& _terrain_info)
{

	int tempidx = 0;
	int isquenceidx = 0;


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

			if(isquenceidx != 1)
				temptile.byOption = MOVE_NONE;
			else
				temptile.byOption = MOVE_OK;

			_terrain_info.byGroup_ID = GROUP_R;
			_terrain_info.byGroup_sequence = isquenceidx;
			_terrain_info.byTerrain_ID = m_curTerrainID;

			if( i == 0 || isquenceidx == 3)
				_terrain_info.bysortLV = 1;
			else
				_terrain_info.bysortLV = 0;

			if(m_curTerrainID == TERRAIN_HIGHDIRT)
				HD_R_Algorithm(temptile , _terrain_info , startidx + tempidx , isquenceidx);
		}
	}
}
void CTerrainBrushMgr::RightDown(const int startidx, TILE& temptile , TERRAIN_INFO& _terrain_info)
{
	int tempidx = 0;
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

			if(m_curTerrainID == TERRAIN_HIGHDIRT)
				HD_RD_Algorithm(temptile , _terrain_info , startidx + tempidx , isquenceidx);
		}
	}
}
void CTerrainBrushMgr::LeftDown(const int startidx, TILE& temptile , TERRAIN_INFO& _terrain_info)
{
	int tempidx = 0;

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

			if(m_curTerrainID == TERRAIN_HIGHDIRT)
				HD_LD_Algorithm(temptile , _terrain_info , startidx + tempidx , isquenceidx);
		}
	}
}
void CTerrainBrushMgr::HD_RU_Algorithm(TILE& temptile , TERRAIN_INFO& _terrain_info ,const int oriidx , const int isequenceidx)
{
	bool bdelete = true;
	const TERRAIN_INFO*	getterrain_temp = NULL;
	getterrain_temp = CTileMgr::GetInstance()->GetTerrain_Info(oriidx);

	if( GROUP_LU == getterrain_temp->byGroup_ID)
	{
		if( 2 == getterrain_temp->byGroup_sequence ||
			3 == getterrain_temp->byGroup_sequence &&
			isequenceidx <= 1)
			return;
	}
	else if(GROUP_LD == getterrain_temp->byGroup_ID)
	{
		if(getterrain_temp->byGroup_sequence <= 1 && isequenceidx >= 2)
		{
			_terrain_info.byGroup_ID = GROUP_FLAT;
			_terrain_info.byGroup_sequence = 0;
			temptile.byFloor = m_curFloor;
			temptile.byOption = MOVE_OK;
		}
	}
	else if(GROUP_RD == getterrain_temp->byGroup_ID)
	{
		if(isequenceidx <= 1 &&
			getterrain_temp->byGroup_sequence <= 1)
			return;
	}
	else if(GROUP_L == getterrain_temp->byGroup_ID)
	{
		if( 2 <= getterrain_temp->byGroup_sequence &&
			getterrain_temp->byGroup_sequence <= 3 &&
			isequenceidx <= 1)
		{
			_terrain_info.byGroup_ID = GROUP_LU;
			_terrain_info.byGroup_sequence = isequenceidx;
			_terrain_info.bysortLV = 1;
			temptile.byFloor = m_curFloor - 1;
			temptile.byOption = MOVE_NONE;
			CTileMgr::GetInstance()->SetTerrain(oriidx - SQ_TILECNTX, temptile, _terrain_info , bdelete);

			_terrain_info.byGroup_sequence = isequenceidx + 2;
			if(isequenceidx + 2 != 3)
				_terrain_info.bysortLV = 1;
			else
				_terrain_info.bysortLV = 0;
		}
		else if(4 <= getterrain_temp->byGroup_sequence &&
			2 <= isequenceidx)
		{
			_terrain_info.byGroup_ID = GROUP_FLAT;
			_terrain_info.byGroup_sequence = 0;
			_terrain_info.bysortLV = 0;
			temptile.byOption = MOVE_OK;
			temptile.byFloor = m_curFloor;
		}
	}
	else if( GROUP_FLAT == getterrain_temp->byGroup_ID &&
		m_curTerrainID == getterrain_temp->byTerrain_ID)
		return;

	CTileMgr::GetInstance()->SetTerrain(oriidx , temptile, _terrain_info , bdelete);

}
void CTerrainBrushMgr::HD_LU_Algorithm(TILE& temptile , TERRAIN_INFO& _terrain_info ,const int oriidx , const int isequenceidx)
{
	bool bdelete = true;
	const TERRAIN_INFO*	getterrain_temp = NULL;
	getterrain_temp = CTileMgr::GetInstance()->GetTerrain_Info(oriidx);

	if( GROUP_RU == getterrain_temp->byGroup_ID)
	{
		if( 2 == getterrain_temp->byGroup_sequence ||
			3 == getterrain_temp->byGroup_sequence &&
			isequenceidx <= 1)
			return;
	}
	else if(GROUP_RD == getterrain_temp->byGroup_ID)
	{
		if(getterrain_temp->byGroup_sequence <= 1 && isequenceidx >= 2)
		{
			_terrain_info.byGroup_ID = GROUP_FLAT;
			_terrain_info.byGroup_sequence = 0;
			_terrain_info.bysortLV = 0;
			temptile.byFloor = m_curFloor;
			temptile.byOption = MOVE_OK;
		}
	}
	else if(GROUP_LD == getterrain_temp->byGroup_ID)
	{
		if(isequenceidx <= 1 &&
			getterrain_temp->byGroup_sequence <= 1)
			return;
	}
	else if(GROUP_R == getterrain_temp->byGroup_ID)
	{
		if( 2 <= getterrain_temp->byGroup_sequence &&
			getterrain_temp->byGroup_sequence <= 3 &&
			isequenceidx <= 1)
		{
			_terrain_info.byGroup_ID = GROUP_RU;
			_terrain_info.byGroup_sequence = isequenceidx;
			_terrain_info.bysortLV = 1;
			temptile.byFloor = m_curFloor - 1;
			temptile.byOption = MOVE_NONE;
			CTileMgr::GetInstance()->SetTerrain(oriidx - SQ_TILECNTX, temptile, _terrain_info , bdelete);

			_terrain_info.byGroup_sequence = isequenceidx + 2;
			if(isequenceidx + 2 != 2)
				_terrain_info.bysortLV = 1;
			else
				_terrain_info.bysortLV = 0;
		}
		else if(4 <= getterrain_temp->byGroup_sequence &&
			2 <= isequenceidx)
		{
			_terrain_info.byGroup_ID = GROUP_FLAT;
			_terrain_info.byGroup_sequence = 0;
			_terrain_info.bysortLV = 0;
			temptile.byOption = MOVE_OK;
			temptile.byFloor = m_curFloor;
		}
	}
	else if( GROUP_FLAT == getterrain_temp->byGroup_ID &&
		m_curTerrainID == getterrain_temp->byTerrain_ID)
		return;

	CTileMgr::GetInstance()->SetTerrain(oriidx , temptile, _terrain_info);
}
void CTerrainBrushMgr::HD_RD_Algorithm(TILE& temptile , TERRAIN_INFO& _terrain_info ,const int oriidx , const int isequenceidx)
{
	bool bdelete = true;
	TERRAIN_INFO*	get_terrain = NULL;
	get_terrain = CTileMgr::GetInstance()->GetTerrain_Info(oriidx);

	if(GROUP_LU == get_terrain->byGroup_ID)
	{
		if(	get_terrain->byGroup_sequence <=3 &&
			isequenceidx <= 1)
		{
			_terrain_info.byGroup_ID = GROUP_FLAT;
			_terrain_info.byGroup_sequence = 0;
			temptile.byFloor = m_curFloor;
		}
		else if(0 <= get_terrain->byTerrain_ID &&
			2 <= isequenceidx && isequenceidx <= 4) 
		{
			int sqidx = get_sqindex();
			if( Overlap_Group(3 , 2 , sqidx + 2 , GROUP_LU) )
			{
				//그룹 R이 그룹 LU와 겹칠경우
				_terrain_info.byGroup_ID = GROUP_FLAT;
				_terrain_info.byGroup_sequence = isequenceidx - 2;
				_terrain_info.bysortLV = 0;
				temptile.byFloor = m_curFloor;
				temptile.byOption = MOVE_OK;
				CTileMgr::GetInstance()->SetTerrain(oriidx - SQ_TILECNTX , temptile, _terrain_info , bdelete);

				_terrain_info.byGroup_ID = GROUP_LD;
				_terrain_info.byGroup_sequence = isequenceidx - 2;
				_terrain_info.bysortLV = 0;
				temptile.byOption = MOVE_NONE;
				bdelete = false;
			}
		}
	}
	else if(GROUP_LD == get_terrain->byGroup_ID)
	{
		if(0 <= get_terrain->byGroup_sequence &&
			get_terrain->byGroup_sequence <= 3 &&
			2 <= isequenceidx && isequenceidx <= 5)
			return;
	}
	else if(GROUP_RU == get_terrain->byGroup_ID)
	{
		if(isequenceidx <= 1 && get_terrain->byGroup_sequence <= 1)
			bdelete = true;
		else if(3 == isequenceidx && get_terrain->byGroup_sequence == 3)
			bdelete = false;
		else if(isequenceidx == 2)
			return;

	}
	else if(GROUP_RD == get_terrain->byGroup_ID)
	{
		if(isequenceidx >= 4)
			return;
	}
	else if(GROUP_L == get_terrain->byGroup_ID)
	{
		if(0 <= get_terrain->byGroup_sequence &&
			get_terrain->byGroup_sequence <= 1 &&
			2 <= isequenceidx &&
			isequenceidx <= 3)
		{
			_terrain_info.byGroup_ID = GROUP_FLAT;
			_terrain_info.byGroup_sequence = 0;
			_terrain_info.bysortLV = 0;
			temptile.byFloor = m_curFloor;
			temptile.byOption = MOVE_OK;
			CTileMgr::GetInstance()->SetTerrain(oriidx - SQ_TILECNTX , temptile, _terrain_info , bdelete);

			_terrain_info.byGroup_ID = GROUP_LD;
			_terrain_info.byGroup_sequence = isequenceidx - 2;
			temptile.byFloor = m_curFloor;
			temptile.byOption = MOVE_NONE;
		}
		else if( 2 <= get_terrain->byGroup_sequence && 
			get_terrain->byGroup_sequence <= 3 &&
			4 <= isequenceidx &&
			isequenceidx <= 5)
		{
			_terrain_info.byGroup_ID = GROUP_LD;
			_terrain_info.byGroup_sequence = isequenceidx;
			_terrain_info.bysortLV = 0;
			temptile.byOption = MOVE_NONE;
			temptile.byFloor = m_curFloor - 1;
			CTileMgr::GetInstance()->SetTerrain(oriidx + SQ_TILECNTX, temptile, _terrain_info , bdelete);

			_terrain_info.byGroup_sequence = isequenceidx - 2;
		}
	}
	else if(GROUP_FLAT == get_terrain->byGroup_ID &&
		m_curTerrainID == get_terrain->byTerrain_ID)
		return;

	CTileMgr::GetInstance()->SetTerrain(oriidx , temptile, _terrain_info , bdelete);
}

void CTerrainBrushMgr::HD_LD_Algorithm(TILE& temptile , TERRAIN_INFO& _terrain_info ,const int oriidx , const int isequenceidx)
{
	bool bdelete = true;
	const TERRAIN_INFO*	get_terrain = NULL;
	get_terrain = CTileMgr::GetInstance()->GetTerrain_Info(oriidx);

	if(GROUP_RU == get_terrain->byGroup_ID)
	{
		if(	get_terrain->byGroup_sequence <=3 &&
			isequenceidx <= 1)
		{
			_terrain_info.byGroup_ID = GROUP_FLAT;
			_terrain_info.byGroup_sequence = 0;
			temptile.byFloor = m_curFloor;
		}
	}
	else if(GROUP_RD == get_terrain->byGroup_ID)
	{
		if(0 <= get_terrain->byGroup_sequence &&
			get_terrain->byGroup_sequence <= 3 &&
			2 <= isequenceidx && isequenceidx <= 5)
			return;
	}
	else if(GROUP_LU == get_terrain->byGroup_ID)
	{
		if(isequenceidx <= 1 && get_terrain->byGroup_sequence <= 1)
			bdelete = true;
		else if(2 == isequenceidx && get_terrain->byGroup_sequence == 2)
			bdelete = false;
		else if(isequenceidx == 3)
			return;

	}
	else if(GROUP_LD == get_terrain->byGroup_ID)
	{
		if(isequenceidx >= 4)
			return;
	}
	else if(GROUP_R == get_terrain->byGroup_ID)
	{
		if(0 <= get_terrain->byGroup_sequence &&
			get_terrain->byGroup_sequence <= 1 &&
			2 <= isequenceidx &&
			isequenceidx <= 3)
		{
			_terrain_info.byGroup_ID = GROUP_FLAT;
			_terrain_info.byGroup_sequence = 0;
			_terrain_info.bysortLV = 0;
			temptile.byFloor = m_curFloor;
			temptile.byOption = MOVE_OK;
			CTileMgr::GetInstance()->SetTerrain(oriidx - SQ_TILECNTX , temptile, _terrain_info , bdelete);

			_terrain_info.byGroup_ID = GROUP_RD;
			_terrain_info.byGroup_sequence = isequenceidx - 2;
			temptile.byFloor = m_curFloor;
			temptile.byOption = MOVE_NONE;
		}
		else if( 2 <= get_terrain->byGroup_sequence && 
			get_terrain->byGroup_sequence <= 3 &&
			4 <= isequenceidx &&
			isequenceidx <= 5)
		{
			_terrain_info.byGroup_ID = GROUP_RD;
			_terrain_info.byGroup_sequence = isequenceidx;
			_terrain_info.bysortLV = 0;
			temptile.byOption = MOVE_NONE;
			temptile.byFloor = m_curFloor - 1;
			CTileMgr::GetInstance()->SetTerrain(oriidx + SQ_TILECNTX, temptile, _terrain_info , bdelete);

			_terrain_info.byGroup_sequence = isequenceidx - 2;
		}
	}
	else if(GROUP_FLAT == get_terrain->byGroup_ID &&
		m_curTerrainID == get_terrain->byTerrain_ID)
		return;

	CTileMgr::GetInstance()->SetTerrain(oriidx , temptile , _terrain_info , bdelete);
}

void CTerrainBrushMgr::HD_L_Algorithm(TILE& temptile , TERRAIN_INFO& _terrain_info ,const int oriidx , const int isequenceidx)
{
	bool bdelete = true;
	TERRAIN_INFO*	getterrain_temp = NULL;
	getterrain_temp = CTileMgr::GetInstance()->GetTerrain_Info(oriidx);

	if(GROUP_RU == getterrain_temp->byGroup_ID)
	{
		if(	getterrain_temp->byGroup_sequence <= 1 &&
			2 <= isequenceidx &&
			isequenceidx <= 3)
		{
			_terrain_info.byGroup_ID = GROUP_LU;
			_terrain_info.byGroup_sequence = isequenceidx - 2;
			_terrain_info.bysortLV = 1;
			CTileMgr::GetInstance()->SetTerrain(oriidx-SQ_TILECNTX , temptile, _terrain_info , bdelete);

			_terrain_info.byGroup_sequence = isequenceidx;
			if(isequenceidx != 3)
				_terrain_info.bysortLV = 1;
			else
				_terrain_info.bysortLV = 0;
		}
		else if(4 <= isequenceidx && isequenceidx <= 5)
		{
			_terrain_info.byGroup_ID = GROUP_FLAT;
			_terrain_info.byGroup_sequence = 0;
			_terrain_info.bysortLV = 0;

			temptile.byOption = MOVE_OK;
			temptile.byFloor = m_curFloor;
		}
		else if(0 <= isequenceidx && isequenceidx <=1)
			return;
		else
			return;
	}
	else if(GROUP_RD == getterrain_temp->byGroup_ID)
	{
		if(getterrain_temp->byGroup_sequence <= 3 &&
			isequenceidx <= 1)
		{
			_terrain_info.byGroup_ID = GROUP_FLAT;
			_terrain_info.byGroup_sequence = 0;
			_terrain_info.bysortLV = 0;
			temptile.byOption = MOVE_OK;
			temptile.byFloor = m_curFloor;
			CTileMgr::GetInstance()->SetTerrain(oriidx - SQ_TILECNTX, temptile, _terrain_info , bdelete);

			_terrain_info.byGroup_sequence = isequenceidx;
			_terrain_info.bysortLV = 0;
			_terrain_info.byGroup_ID = GROUP_LD;
			temptile.byOption = MOVE_NONE;
			temptile.byFloor = m_curFloor;
		}
		else if( 4 <= getterrain_temp->byGroup_sequence &&
			2 <= isequenceidx )
		{
			_terrain_info.byGroup_ID = GROUP_LD;
			_terrain_info.bysortLV = 0;
			_terrain_info.byGroup_sequence = isequenceidx;
			temptile.byOption = MOVE_NONE;
			temptile.byFloor = m_curFloor - 1;
		}
		else
			return;
	}
	if(m_curTerrainID == getterrain_temp->byTerrain_ID)
		if(//GROUP_RD == getterrain_temp->byGroup_ID ||
			GROUP_LD == getterrain_temp->byGroup_ID ||
			//GROUP_RU == getterrain_temp->byGroup_ID ||
			GROUP_LU == getterrain_temp->byGroup_ID ||
			GROUP_FLAT == getterrain_temp->byGroup_ID)
			return;

	CTileMgr::GetInstance()->SetTerrain(oriidx , temptile, _terrain_info , bdelete);
}

void CTerrainBrushMgr::HD_R_Algorithm(TILE& temptile , TERRAIN_INFO& _terrain_info ,const int oriidx , const int isequenceidx)
{
	bool bdelete = true;
	const TERRAIN_INFO*	getterrain_temp = NULL;
	getterrain_temp = CTileMgr::GetInstance()->GetTerrain_Info(oriidx);

	_terrain_info.byGroup_sequence = isequenceidx;

	if(GROUP_LU == getterrain_temp->byGroup_ID)
	{
		if(	getterrain_temp->byGroup_sequence <= 1 &&
			2 <= isequenceidx &&
			isequenceidx <= 3)
		{
			_terrain_info.byGroup_ID = GROUP_RU;
			_terrain_info.byGroup_sequence = isequenceidx - 2;
			_terrain_info.bysortLV = 1;
			CTileMgr::GetInstance()->SetTerrain(oriidx-SQ_TILECNTX , temptile, _terrain_info , bdelete);

			_terrain_info.byGroup_sequence = isequenceidx;
			if(isequenceidx != 2)
				_terrain_info.bysortLV = 1;
			else
				_terrain_info.bysortLV = 0;
		}
		else if(4 <= isequenceidx && isequenceidx <= 5)
		{
			_terrain_info.byGroup_ID = GROUP_FLAT;
			_terrain_info.byGroup_sequence = 0;
			_terrain_info.bysortLV = 0;

			temptile.byOption = MOVE_OK;
			temptile.byFloor = m_curFloor;
		}
		else if(0 <= isequenceidx && isequenceidx <=1)
			return;
		else
			return;
	}
	else if(GROUP_LD == getterrain_temp->byGroup_ID)
	{
		if(getterrain_temp->byGroup_sequence <= 3 &&
			isequenceidx <= 1)
		{
			_terrain_info.byGroup_ID = GROUP_FLAT;
			_terrain_info.byGroup_sequence = 0;
			_terrain_info.bysortLV = 0;
			temptile.byOption = MOVE_OK;
			temptile.byFloor = m_curFloor;
			CTileMgr::GetInstance()->SetTerrain(oriidx - SQ_TILECNTX, temptile, _terrain_info , bdelete);

			_terrain_info.byGroup_sequence = isequenceidx;
			_terrain_info.bysortLV = 0;
			_terrain_info.byGroup_ID = GROUP_RD;
			temptile.byOption = MOVE_NONE;
			temptile.byFloor = m_curFloor;
		}
		else if( 4 <= getterrain_temp->byGroup_sequence &&
				2 <= isequenceidx )
		{
			_terrain_info.byGroup_ID = GROUP_RD;
			_terrain_info.bysortLV = 0;
			_terrain_info.byGroup_sequence = isequenceidx;
			temptile.byOption = MOVE_NONE;
			temptile.byFloor = m_curFloor - 1;
		}
		else
			return;
	}
	if(m_curTerrainID == getterrain_temp->byTerrain_ID)
		if(	GROUP_RD == getterrain_temp->byGroup_ID ||
			//GROUP_LD == getterrain_temp->byGroup_ID ||
			GROUP_RU == getterrain_temp->byGroup_ID ||
			//GROUP_LU == getterrain_temp->byGroup_ID ||
			GROUP_FLAT == getterrain_temp->byGroup_ID)
			return;

	CTileMgr::GetInstance()->SetTerrain(oriidx , temptile, _terrain_info , bdelete);
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

