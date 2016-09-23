#include "StdAfx.h"
#include "HighDirt_Brush.h"
#include "TileMgr.h"
#include "TerrainBrushMgr.h"

CHighDirt_Brush::CHighDirt_Brush(void)
{
	m_curFloor = 2;
	m_curTerrainID = TERRAIN_HIGHDIRT;
	memset(m_bOverlap , 0 , sizeof(m_bOverlap) );
}

CHighDirt_Brush::~CHighDirt_Brush(void)
{
}
void CHighDirt_Brush::InitOverlap(const int groupid)
{
	memset(m_bOverlap[groupid] , 0 , sizeof(m_bOverlap[groupid]) );
}
void CHighDirt_Brush::BrushPaint()
{
	TILE	temptile;
	TERRAIN_INFO	terrain_info;
	int idx = CTerrainBrushMgr::GetInstance()->get_sqindex();

	Overlap_GroupArea(3, 2, idx - 126 , GROUP_R);
	Overlap_GroupArea(3, 2, idx - 132 , GROUP_L);
	Overlap_GroupArea(3, 2, idx , GROUP_RD);
	Overlap_GroupArea(3, 2, idx - 2 , GROUP_LD);

	LeftArea(idx - 132 , temptile , terrain_info);
	LeftUpArea(idx - 258 , temptile , terrain_info) ;
	RightUpArea(idx - 256 , temptile , terrain_info);
	RightArea(idx - 126 , temptile , terrain_info);
	RightDown(idx , temptile , terrain_info);
	LeftDown(idx - 2 , temptile , terrain_info);

	InitOverlap(GROUP_R);
	InitOverlap(GROUP_L);
	InitOverlap(GROUP_RD);
	InitOverlap(GROUP_LD);
}
void CHighDirt_Brush::Overlap_GroupArea(const int irow ,const int icol ,const int startidx ,const int group_id)
{
	const TERRAIN_INFO* pterrain_temp = NULL;
	int tempidx = 0;
	for(int i = 0; i < irow; ++i)
	{
		for(int j = 0; j < icol; ++j)
		{
			tempidx = i*SQ_TILECNTX +j;
			pterrain_temp = CTileMgr::GetInstance()->GetTerrain_Info(startidx + tempidx);

			m_bOverlap[group_id][pterrain_temp->byGroup_ID] = true;
		}
	}
}
void CHighDirt_Brush::LeftArea(const int startidx, TILE& temptile, TERRAIN_INFO& _terrain_info)
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


			_terrain_info.byTerrain_ID = TERRAIN_HIGHDIRT;
			_terrain_info.byGroup_ID = GROUP_L;
			_terrain_info.byGroup_sequence = isequnceidx;

			if(isequnceidx >= 0 && isequnceidx <= 2)
				_terrain_info.bysortLV = 1;
			else
				_terrain_info.bysortLV = 0;
			HD_L_Algorithm(temptile , _terrain_info , startidx + tempidx , isequnceidx);
		}
	}
}
void CHighDirt_Brush::HD_L_Algorithm(TILE& temptile , TERRAIN_INFO& _terrain_info ,const int oriidx , const int isequenceidx)
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
			_terrain_info.byGroup_sequence = isequenceidx + 2;
			temptile.byOption = MOVE_NONE;
			temptile.byFloor = m_curFloor - 1;

			CTileMgr::GetInstance()->SetTerrain(oriidx + SQ_TILECNTX, temptile, _terrain_info , bdelete);
			_terrain_info.byGroup_sequence = isequenceidx;
		}
		else
			return;
	}
	else if(GROUP_LU == getterrain_temp->byGroup_ID)
	{
		if( 4 <= isequenceidx &&
			0 <= getterrain_temp->byGroup_sequence &&
			getterrain_temp->byGroup_sequence <= 1)
		{
			_terrain_info.bysortLV = 0;
			bdelete = false;
		}
		else
			return;
	}
	else if(GROUP_L == getterrain_temp->byGroup_ID)
	{
		if( 4 <= isequenceidx)
			bdelete = false;
	}
	//else if(GROUP_LD == getterrain_temp->byGroup_ID)
	//{
	//	if(isequenceidx <= 1)
	//		bdelete = false;
	//	else
	//		return;
	//}
	if(TERRAIN_HIGHDIRT == getterrain_temp->byTerrain_ID)
		if(//GROUP_RD == getterrain_temp->byGroup_ID ||
			GROUP_LD == getterrain_temp->byGroup_ID ||
			//GROUP_RU == getterrain_temp->byGroup_ID ||
			//GROUP_LU == getterrain_temp->byGroup_ID ||
			GROUP_FLAT == getterrain_temp->byGroup_ID)
			return;

	CTileMgr::GetInstance()->SetTerrain(oriidx , temptile, _terrain_info , bdelete);
}

void CHighDirt_Brush::LeftUpArea(const int startidx, TILE& temptile , TERRAIN_INFO& _terrain_info)
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

			_terrain_info.byTerrain_ID = TERRAIN_HIGHDIRT;
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

			HD_LU_Algorithm(temptile , _terrain_info , startidx + tempidx , isequenceidx);
		}
	}
}
void CHighDirt_Brush::HD_LU_Algorithm(TILE& temptile , TERRAIN_INFO& _terrain_info ,const int oriidx , const int isequenceidx)
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
		else if( 2 <= getterrain_temp->byGroup_sequence &&
			0 <= isequenceidx)
		{
			if(m_bOverlap[GROUP_L][GROUP_RD] == true)
			{
				if(isequenceidx < 2)
				{
					_terrain_info.byGroup_ID = GROUP_RU;
					_terrain_info.byGroup_sequence = isequenceidx + 2;
					_terrain_info.bysortLV = 1;
					temptile.byOption = MOVE_NONE;
					temptile.byFloor = m_curFloor;
				}
				else
				{
					_terrain_info.byGroup_ID = GROUP_FLAT;
					_terrain_info.byGroup_sequence = 0;
					_terrain_info.bysortLV = 0;
					temptile.byOption = MOVE_OK;
					temptile.byFloor = m_curFloor;
				}
			}
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
		TERRAIN_HIGHDIRT == getterrain_temp->byTerrain_ID)
		return;

	CTileMgr::GetInstance()->SetTerrain(oriidx , temptile, _terrain_info);
}
void CHighDirt_Brush::RightUpArea(const int startidx, TILE& temptile , TERRAIN_INFO& _terrain_info)
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

			_terrain_info.byTerrain_ID = TERRAIN_HIGHDIRT;
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


			HD_RU_Algorithm(temptile , _terrain_info , startidx + tempidx , isequenceidx);


		}
	}
}
void CHighDirt_Brush::HD_RU_Algorithm(TILE& temptile , TERRAIN_INFO& _terrain_info ,const int oriidx , const int isequenceidx)
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
		if(getterrain_temp->byGroup_sequence <= 1 && 2 <= isequenceidx)
		{
			_terrain_info.byGroup_ID = GROUP_FLAT;
			_terrain_info.byGroup_sequence = 0;
			temptile.byFloor = m_curFloor;
			temptile.byOption = MOVE_OK;
		}
		else if( 2 <= getterrain_temp->byGroup_sequence &&
			0 <= isequenceidx)
		{
			if(m_bOverlap[GROUP_R][GROUP_LD] == true)
			{
				if(isequenceidx < 2)
				{
					_terrain_info.byGroup_ID = GROUP_LU;
					_terrain_info.byGroup_sequence = isequenceidx + 2;
					_terrain_info.bysortLV = 1;
					temptile.byOption = MOVE_NONE;
					temptile.byFloor = m_curFloor;
				}
				else
				{
					_terrain_info.byGroup_ID = GROUP_FLAT;
					_terrain_info.byGroup_sequence = 0;
					_terrain_info.bysortLV = 0;
					temptile.byOption = MOVE_OK;
					temptile.byFloor = m_curFloor;
				}
			}
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
		TERRAIN_HIGHDIRT == getterrain_temp->byTerrain_ID)
		return;

	CTileMgr::GetInstance()->SetTerrain(oriidx , temptile, _terrain_info , bdelete);

}
void CHighDirt_Brush::RightArea(const int startidx, TILE& temptile , TERRAIN_INFO& _terrain_info)
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
			_terrain_info.byTerrain_ID = TERRAIN_HIGHDIRT;

			if( i == 0 || isquenceidx == 3)
				_terrain_info.bysortLV = 1;
			else
				_terrain_info.bysortLV = 0;

			HD_R_Algorithm(temptile , _terrain_info , startidx + tempidx , isquenceidx);
		}
	}
}
void CHighDirt_Brush::HD_R_Algorithm(TILE& temptile , TERRAIN_INFO& _terrain_info ,const int oriidx , const int isequenceidx)
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
			_terrain_info.byGroup_sequence = isequenceidx + 2;
			temptile.byOption = MOVE_NONE;
			temptile.byFloor = m_curFloor - 1;
			CTileMgr::GetInstance()->SetTerrain(oriidx + SQ_TILECNTX, temptile, _terrain_info , bdelete);
			_terrain_info.byGroup_sequence = isequenceidx;
		}
		else
			return;
	}
	else if(GROUP_R == getterrain_temp->byGroup_ID)
	{
		if( 4 <= isequenceidx)
			bdelete = false;
	}
	else if(GROUP_RU == getterrain_temp->byGroup_ID)
	{
		if( 4 <= isequenceidx &&
			0 <= getterrain_temp->byGroup_sequence &&
			getterrain_temp->byGroup_sequence <= 1)
		{
			_terrain_info.bysortLV = 0;
			bdelete = false;
		}
		else
			return;
	}
	if(TERRAIN_HIGHDIRT == getterrain_temp->byTerrain_ID)
		if(	GROUP_RD == getterrain_temp->byGroup_ID ||
			//GROUP_LD == getterrain_temp->byGroup_ID ||
			//GROUP_RU == getterrain_temp->byGroup_ID ||
			//GROUP_LU == getterrain_temp->byGroup_ID ||
			GROUP_FLAT == getterrain_temp->byGroup_ID)
			return;

	CTileMgr::GetInstance()->SetTerrain(oriidx , temptile, _terrain_info , bdelete);
}
void CHighDirt_Brush::RightDown(const int startidx, TILE& temptile , TERRAIN_INFO& _terrain_info)
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
			_terrain_info.byTerrain_ID = TERRAIN_HIGHDIRT;
			_terrain_info.bysortLV = 0;

			HD_RD_Algorithm(temptile , _terrain_info , startidx + tempidx , isquenceidx);
		}
	}
	//InitOverlap(GROUP_R);
}
void CHighDirt_Brush::LeftDown(const int startidx, TILE& temptile , TERRAIN_INFO& _terrain_info)
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
			_terrain_info.byTerrain_ID = TERRAIN_HIGHDIRT;
			_terrain_info.bysortLV = 0;

			HD_LD_Algorithm(temptile , _terrain_info , startidx + tempidx , isquenceidx);
		}
	}
	//InitOverlap(GROUP_L);
}
void CHighDirt_Brush::HD_RD_Algorithm(TILE& temptile , TERRAIN_INFO& _terrain_info ,const int oriidx , const int isequenceidx)
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
		else if(0 <= get_terrain->byGroup_sequence &&
			2 <= isequenceidx && isequenceidx <= 3) 
		{
			//그룹 R이 그룹 LU와 겹칠경우
			if(true == m_bOverlap[GROUP_R][GROUP_LU])
			{
				_terrain_info.byGroup_ID = GROUP_FLAT;
				_terrain_info.byGroup_sequence = 0;
				_terrain_info.bysortLV = 0;
				temptile.byFloor = m_curFloor;
				temptile.byOption = MOVE_OK;
				CTileMgr::GetInstance()->SetTerrain(oriidx - SQ_TILECNTX , temptile, _terrain_info , bdelete);


				_terrain_info.byGroup_ID = GROUP_LD;
				_terrain_info.byGroup_sequence = isequenceidx - 2;
				_terrain_info.bysortLV = 0;
				temptile.byOption = MOVE_NONE;
				//bdelete = false;
			}
			else if(true == m_bOverlap[GROUP_LD][GROUP_RU])
			{
				bdelete = false;
				_terrain_info.byGroup_ID = GROUP_RU;
				_terrain_info.byGroup_sequence = isequenceidx;
				_terrain_info.bysortLV = 1;

				temptile.byFloor = m_curFloor;
				temptile.byOption = MOVE_NONE;
				CTileMgr::GetInstance()->SetTerrain(oriidx , temptile, _terrain_info , bdelete);

				_terrain_info.byGroup_ID = GROUP_RD;
				_terrain_info.bysortLV = 0;
				temptile.byFloor = m_curFloor - 1;
			}
		}
		else if(0 <= get_terrain->byGroup_sequence && 
			4 <= isequenceidx) 
		{
			if(true == m_bOverlap[GROUP_R][GROUP_LU])
			{
				if(5 != isequenceidx)
				{
					_terrain_info.byGroup_ID = GROUP_LD;
					_terrain_info.byGroup_sequence = isequenceidx - 2;
					_terrain_info.bysortLV = 0;
					temptile.byOption = MOVE_NONE;
					bdelete = false;
				}
				else
					return;
			}
			else if(true == m_bOverlap[GROUP_RD][GROUP_LU])
			{
				_terrain_info.byGroup_ID = GROUP_FLAT;
				_terrain_info.byGroup_sequence = 0;
				_terrain_info.bysortLV = 0;
				temptile.byOption = MOVE_OK;
				temptile.byFloor = 2;
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
	else if(GROUP_R == get_terrain->byGroup_ID)
	{
		if(4 <= isequenceidx)
			bdelete = false;
	}
	else if(GROUP_FLAT == get_terrain->byGroup_ID &&
		m_curTerrainID == get_terrain->byTerrain_ID)
		return;

	CTileMgr::GetInstance()->SetTerrain(oriidx , temptile, _terrain_info , bdelete);
}

void CHighDirt_Brush::HD_LD_Algorithm(TILE& temptile , TERRAIN_INFO& _terrain_info ,const int oriidx , const int isequenceidx)
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
		else if(0 <= get_terrain->byGroup_sequence &&
			2 <= isequenceidx && isequenceidx <= 3) 
		{
			if(true == m_bOverlap[GROUP_L][GROUP_RU])
			{
				_terrain_info.byGroup_ID = GROUP_FLAT;
				_terrain_info.byGroup_sequence = 0;
				_terrain_info.bysortLV = 0;
				temptile.byFloor = m_curFloor;
				temptile.byOption = MOVE_OK;
				CTileMgr::GetInstance()->SetTerrain(oriidx - SQ_TILECNTX , temptile, _terrain_info , bdelete);


				_terrain_info.byGroup_ID = GROUP_RD;
				_terrain_info.byGroup_sequence = isequenceidx - 2;
				_terrain_info.bysortLV = 0;
				temptile.byOption = MOVE_NONE;
				//bdelete = false;
			}
			else if(true == m_bOverlap[GROUP_RD][GROUP_LU])
			{
				bdelete = false;
				_terrain_info.byGroup_ID = GROUP_LU;
				_terrain_info.byGroup_sequence = isequenceidx;
				_terrain_info.bysortLV = 1;
				
				temptile.byFloor = m_curFloor;
				temptile.byOption = MOVE_NONE;
				CTileMgr::GetInstance()->SetTerrain(oriidx , temptile, _terrain_info , bdelete);

				_terrain_info.byGroup_ID = GROUP_LD;
				_terrain_info.bysortLV = 0;
				temptile.byFloor = m_curFloor - 1;
			}
		}
		else if(0 <= get_terrain->byGroup_sequence && 
			4 <= isequenceidx) 
		{
			if(true == m_bOverlap[GROUP_L][GROUP_RU])
			{
				if(4 != isequenceidx)
				{
					_terrain_info.byGroup_ID = GROUP_RD;
					_terrain_info.byGroup_sequence = isequenceidx - 2;
					_terrain_info.bysortLV = 0;
					temptile.byOption = MOVE_NONE;
					bdelete = false;
				}
				else
					return;
			}
			else if(true == m_bOverlap[GROUP_RD][GROUP_LU])
			{
				_terrain_info.byGroup_ID = GROUP_FLAT;
				_terrain_info.byGroup_sequence = 0;
				_terrain_info.bysortLV = 0;
				temptile.byOption = MOVE_OK;
				temptile.byFloor = 2;
			}
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
	else if(GROUP_L == get_terrain->byGroup_ID)
	{
		if(4 <= isequenceidx)
			bdelete = false;
	}
	else if(GROUP_FLAT == get_terrain->byGroup_ID &&
		m_curTerrainID == get_terrain->byTerrain_ID)
		return;

	CTileMgr::GetInstance()->SetTerrain(oriidx , temptile , _terrain_info , bdelete);
}

