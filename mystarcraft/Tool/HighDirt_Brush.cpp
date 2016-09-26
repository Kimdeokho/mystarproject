#include "StdAfx.h"
#include "HighDirt_Brush.h"
#include "TileMgr.h"
#include "TerrainBrushMgr.h"

CHighDirt_Brush::CHighDirt_Brush(void)
{
	m_curFloor = 2;
	m_curTerrainID = TERRAIN_HIGHDIRT;
	memset(m_bOverlap , 0 , sizeof(m_bOverlap) );
	memset(&m_terraininfo , 0 , sizeof(TERRAIN_INFO));
	memset(&m_temptile , 0 , sizeof(TILE));
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
	int idx = CTerrainBrushMgr::GetInstance()->get_sqindex();

	Overlap_GroupArea(3, 2, idx - 126 , GROUP_R);
	Overlap_GroupArea(3, 2, idx - 132 , GROUP_L);
	Overlap_GroupArea(3, 2, idx , GROUP_RD);
	Overlap_GroupArea(3, 2, idx - 2 , GROUP_LD);
	Overlap_GroupArea(3, 2, idx - 256 , GROUP_RU);
	Overlap_GroupArea(3, 2, idx - 258 , GROUP_LU);

	LeftArea(idx - 132);
	LeftUpArea(idx - 258) ;
	RightUpArea(idx - 256);
	RightArea(idx - 126);
	RightDown(idx);
	LeftDown(idx - 2);

	for(int i = 0; i < GROUP_END; ++i)
	{
		InitOverlap(i);
	}
	//InitOverlap(GROUP_R);
	//InitOverlap(GROUP_L);
	//InitOverlap(GROUP_RD);
	//InitOverlap(GROUP_LD);
	//InitOverlap(GROUP_RU);
	//InitOverlap(GROUP_LU);
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
void CHighDirt_Brush::LeftArea(const int startidx)
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
				m_temptile.byFloor = m_curFloor - 1;
			else
				m_temptile.byFloor = m_curFloor;

			if(isequnceidx == 0)
				m_temptile.byOption = MOVE_OK;
			else
				m_temptile.byOption = MOVE_NONE;


			m_terraininfo.byTerrain_ID = TERRAIN_HIGHDIRT;
			m_terraininfo.byGroup_ID = GROUP_L;
			m_terraininfo.byGroup_sequence = isequnceidx;

			if(isequnceidx >= 0 && isequnceidx <= 2)
				m_terraininfo.bysortLV = 1;
			else
				m_terraininfo.bysortLV = 0;
			HD_L_Algorithm(startidx + tempidx , isequnceidx);
		}
	}
}
void CHighDirt_Brush::HD_L_Algorithm(const int oriidx , const int isequenceidx)
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
			SetTerrainInfo(m_curTerrainID ,GROUP_LU , isequenceidx - 2 , 1);
			CTileMgr::GetInstance()->SetTerrain(oriidx-SQ_TILECNTX , m_temptile, m_terraininfo , bdelete);


			if(isequenceidx != 3)
				SetTerrainInfo(m_curTerrainID ,GROUP_LU , isequenceidx, 1);
			else
				SetTerrainInfo(m_curTerrainID ,GROUP_LU , isequenceidx, 0);
		}
		else if(4 <= isequenceidx && isequenceidx <= 5)
		{
			SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0, 0);
			SetTileInfo(MOVE_OK ,m_curFloor);
		}
		else if(0 <= isequenceidx && isequenceidx <=1)
			return;
		else
			return;
	}
	else if(GROUP_RD == getterrain_temp->byGroup_ID)
	{
		if(m_bOverlap[GROUP_LU][GROUP_LD] == true)
		{
			if(isequenceidx <= 1)
			{
				SetTerrainInfo(m_curTerrainID , GROUP_FLAT , 0 , 0);
				SetTileInfo(MOVE_OK , m_curFloor);
				CTileMgr::GetInstance()->SetTerrain(oriidx - SQ_TILECNTX, m_temptile, m_terraininfo , bdelete);
			}
			SetTerrainInfo(m_curTerrainID , GROUP_LD , isequenceidx , 0);
			if(isequenceidx != 1)
				SetTileInfo(MOVE_NONE , m_curFloor -1);
			else
				SetTileInfo(MOVE_NONE , m_curFloor);
		}
		else
		{
			if(getterrain_temp->byGroup_sequence <= 3 &&
				isequenceidx <= 1)
			{
				SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0, 0);
				SetTileInfo(MOVE_OK ,m_curFloor);

				CTileMgr::GetInstance()->SetTerrain(oriidx - SQ_TILECNTX, m_temptile, m_terraininfo , bdelete);

				SetTerrainInfo(m_curTerrainID ,GROUP_LD , isequenceidx, 0);
				SetTileInfo(MOVE_NONE ,m_curFloor);
			}
			else if( 4 <= getterrain_temp->byGroup_sequence &&
				2 <= isequenceidx )
			{
				SetTerrainInfo(m_curTerrainID ,GROUP_LD , isequenceidx + 2, 0);
				SetTileInfo(MOVE_NONE ,m_curFloor - 1);

				CTileMgr::GetInstance()->SetTerrain(oriidx + SQ_TILECNTX, m_temptile, m_terraininfo , bdelete);
				SetTerrainInfo(m_curTerrainID ,GROUP_LD , isequenceidx, 0);
			}
			else if(getterrain_temp->byGroup_sequence <= 3 &&
				2<= isequenceidx)
			{
				SetTerrainInfo(m_curTerrainID ,GROUP_RD , isequenceidx - 2, 0);
				SetTileInfo(MOVE_NONE ,m_curFloor - 1);
			}

			BYTE tempgroup_id1 = CTileMgr::GetInstance()->GetTerrain_Info(oriidx - 2)->byGroup_ID;
			BYTE tempgroup_id2 = CTileMgr::GetInstance()->GetTerrain_Info(oriidx + SQ_TILECNTX - 2)->byGroup_ID;
			BYTE tempgroup_id3 = CTileMgr::GetInstance()->GetTerrain_Info(oriidx + SQ_TILECNTX)->byGroup_ID;
			if(tempgroup_id1 == GROUP_RD && tempgroup_id2 == GROUP_RU)
			{
				if(isequenceidx <= 1)
				{
					SetTerrainInfo(m_curTerrainID , GROUP_FLAT , 0 , 0);
					SetTileInfo(MOVE_OK , m_curFloor);

					for(int i = 0; i < 2; ++i)
					{
						CTileMgr::GetInstance()->SetTerrain(oriidx + SQ_TILECNTX*i, m_temptile, m_terraininfo , bdelete);
						CTileMgr::GetInstance()->SetTerrain(oriidx + SQ_TILECNTX*i - 2, m_temptile, m_terraininfo , bdelete);
					}
				}
			}
			else if(tempgroup_id1 == GROUP_LD && tempgroup_id2 == GROUP_LU)
			{
				if(isequenceidx <= 1)
				{
					SetTerrainInfo(m_curTerrainID , GROUP_LD , isequenceidx , 0);
					SetTileInfo(MOVE_NONE , m_curFloor - 1);

					CTileMgr::GetInstance()->SetTerrain(oriidx, m_temptile, m_terraininfo , bdelete);

					SetTerrainInfo(m_curTerrainID , GROUP_LU , isequenceidx + 2 , 1);
					CTileMgr::GetInstance()->SetTerrain(oriidx + SQ_TILECNTX, m_temptile, m_terraininfo , false);
					return;
				}
			}
			else if(tempgroup_id3 == GROUP_RU)
			{
				SetTerrainInfo(m_curTerrainID , GROUP_FLAT , 0 , 0);
				SetTileInfo(MOVE_OK , m_curFloor);
			}

		}
	}
	else if(GROUP_LU == getterrain_temp->byGroup_ID)
	{
		if( 4 <= isequenceidx &&
			0 <= getterrain_temp->byGroup_sequence &&
			getterrain_temp->byGroup_sequence <= 1)
		{
			m_terraininfo.bysortLV = 0;
			bdelete = false;
		}
		else
			return;
	}
	else if(GROUP_R == getterrain_temp->byGroup_ID)
	{
		TERRAIN_INFO* temp2 = CTileMgr::GetInstance()->GetTerrain_Info(oriidx + SQ_TILECNTX*2);
		TERRAIN_INFO* temp3 = CTileMgr::GetInstance()->GetTerrain_Info(oriidx - SQ_TILECNTX);
		if( 2<= getterrain_temp->byGroup_sequence && 
			getterrain_temp->byGroup_sequence <= 3 &&
			2<= temp2->byGroup_sequence && 
			temp2->byGroup_sequence <= 3)
		{
			if(isequenceidx <= 1)
			{
				SetTileInfo(MOVE_NONE , m_curFloor);
				SetTerrainInfo(m_curTerrainID , GROUP_FLAT , 0 , 0);
				for(int i = 0; i < 2; ++i)
					CTileMgr::GetInstance()->SetTerrain(oriidx + SQ_TILECNTX*i - 2 , m_temptile, m_terraininfo , bdelete);				

				SetTileInfo(MOVE_NONE , m_curFloor - 1);
				for(int i = 0; i < 3; ++i)
				{
					SetTerrainInfo(m_curTerrainID , GROUP_RD , isequenceidx + i*2 , 0);
					CTileMgr::GetInstance()->SetTerrain(oriidx + SQ_TILECNTX*(i+1) , m_temptile, m_terraininfo , bdelete);				
				}

				if(isequenceidx == 2)
					SetTerrainInfo(m_curTerrainID , GROUP_RU , isequenceidx + 2 , 0);
				else
					SetTerrainInfo(m_curTerrainID , GROUP_RU , isequenceidx + 2 , 1);

			}
		}
	}
	else if(GROUP_L == getterrain_temp->byGroup_ID)
	{
		if( 4 <= isequenceidx)
			bdelete = false;
	}
	else if(GROUP_LD == getterrain_temp->byGroup_ID)
	{
		if( 4 <= getterrain_temp->byGroup_sequence &&
			isequenceidx <= 1)
			bdelete = false;
		else
			return;
	}
	else if(GROUP_FLAT == getterrain_temp->byGroup_ID)
	{
		if(getterrain_temp->byTerrain_ID == m_curTerrainID)
		{
			if(m_bOverlap[GROUP_LU][GROUP_R] == true)
			{
				if( 2 <= isequenceidx &&	isequenceidx <= 3)
				{
					for(int i = 0; i < 2; ++i)
					{
						SetTerrainInfo(m_curTerrainID , GROUP_FLAT , 0 , 0);
						SetTileInfo(MOVE_OK , m_curFloor);
						CTileMgr::GetInstance()->SetTerrain(oriidx - SQ_TILECNTX*(i+1) , m_temptile, m_terraininfo , bdelete);
					}
				}
			}
			else
			{
				//if(isequenceidx <= 1) 
				//{
				//	//SetTerrainInfo(m_curTerrainID , GROUP_FLAT , 0 , 0);
				//	//SetTileInfo(MOVE_OK , m_curFloor);
				//	//CTileMgr::GetInstance()->SetTerrain(oriidx + SQ_TILECNTX , m_temptile, m_terraininfo , bdelete);
				//}
				//else
				return;
			}
		}
	}
	//if(TERRAIN_HIGHDIRT == getterrain_temp->byTerrain_ID)
	//	if(//GROUP_RD == getterrain_temp->byGroup_ID ||
	//		//GROUP_LD == getterrain_temp->byGroup_ID ||
	//		//GROUP_RU == getterrain_temp->byGroup_ID ||
	//		//GROUP_LU == getterrain_temp->byGroup_ID ||
	//		GROUP_FLAT == getterrain_temp->byGroup_ID)
	//		return;

	CTileMgr::GetInstance()->SetTerrain(oriidx , m_temptile, m_terraininfo , bdelete);
}

void CHighDirt_Brush::LeftUpArea(const int startidx)
{
	int tempidx = 0;
	int isequenceidx = 0;

	m_temptile.byOption = MOVE_NONE;

	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			tempidx = i * SQ_TILECNTX + j;
			isequenceidx = i*2+j;

			m_terraininfo.byTerrain_ID = TERRAIN_HIGHDIRT;
			m_terraininfo.byGroup_ID = GROUP_LU;
			m_terraininfo.byGroup_sequence = isequenceidx;

			if(0 == isequenceidx)
				m_temptile.byFloor = m_curFloor - 1;
			else
				m_temptile.byFloor = m_curFloor;

			if(isequenceidx != 3)
				m_terraininfo.bysortLV = 1;
			else
				m_terraininfo.bysortLV = 0;

			HD_LU_Algorithm(startidx + tempidx , isequenceidx);
		}
	}
}
void CHighDirt_Brush::HD_LU_Algorithm(const int oriidx , const int isequenceidx)
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
			SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0, 0);
			SetTileInfo(MOVE_OK ,m_curFloor);
		}
		else if( 2 <= getterrain_temp->byGroup_sequence &&
			0 <= isequenceidx)
		{
			if(m_bOverlap[GROUP_L][GROUP_RD] == true)
			{
				if(isequenceidx <= 1)
				{
					if(m_bOverlap[GROUP_R][GROUP_RD] == true)
					{
						SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0, 0);
						SetTileInfo(MOVE_OK ,m_curFloor);
						CTileMgr::GetInstance()->SetTerrain(oriidx - SQ_TILECNTX, m_temptile, m_terraininfo);
					}
					else
					{
						SetTerrainInfo(m_curTerrainID ,GROUP_RU , isequenceidx + 2, 1);
						SetTileInfo(MOVE_NONE ,m_curFloor - 1);
					}
				}
				else
				{
					SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0 , 0);
					if(isequenceidx != 3)
						SetTileInfo(MOVE_OK ,m_curFloor - 1);
					else
						SetTileInfo(MOVE_OK ,m_curFloor);
				}

				if(m_bOverlap[GROUP_R][GROUP_LD] == true)
				{
					SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0, 0);
					SetTileInfo(MOVE_NONE ,m_curFloor);
					CTileMgr::GetInstance()->SetTerrain(oriidx - SQ_TILECNTX, m_temptile, m_terraininfo);
				}
			}
			else if(m_bOverlap[GROUP_RU][GROUP_LD] == true)
			{
				if(isequenceidx <= 1)
				{
					SetTerrainInfo(m_curTerrainID , GROUP_FLAT , 0 , 0);
					SetTileInfo(MOVE_NONE , m_curFloor);
					CTileMgr::GetInstance()->SetTerrain(oriidx - SQ_TILECNTX, m_temptile, m_terraininfo);
					SetTerrainInfo(m_curTerrainID , GROUP_LD , isequenceidx , 0);
				}
				else
				{
					bdelete = false;
					SetTileInfo(MOVE_NONE , m_curFloor - 1);
					if(isequenceidx == 2)
					{
						SetTerrainInfo(m_curTerrainID , GROUP_LD , isequenceidx , 0);						
						CTileMgr::GetInstance()->SetTerrain(oriidx , m_temptile, m_terraininfo);
						SetTerrainInfo(m_curTerrainID , GROUP_LU , isequenceidx , 1);
					}
					else
						SetTerrainInfo(m_curTerrainID , GROUP_LU , isequenceidx , 0);


				}
			}
		}
	}
	else if(GROUP_LD == getterrain_temp->byGroup_ID)
	{
		if(m_bOverlap[GROUP_L][GROUP_RD])
		{
			SetTerrainInfo(m_curTerrainID , GROUP_FLAT , 0 , 0);
			SetTileInfo(MOVE_OK , m_curFloor);
		}
		else
		{
			if(isequenceidx <= 1 &&
				getterrain_temp->byGroup_sequence <= 1)
				return;
		}
	}
	else if(GROUP_R == getterrain_temp->byGroup_ID)
	{
		if( 2 <= getterrain_temp->byGroup_sequence &&
			getterrain_temp->byGroup_sequence <= 3 &&
			isequenceidx <= 1)
		{
			SetTerrainInfo(m_curTerrainID ,GROUP_RU , isequenceidx, 1);
			SetTileInfo(MOVE_NONE ,m_curFloor - 1);
			CTileMgr::GetInstance()->SetTerrain(oriidx - SQ_TILECNTX, m_temptile, m_terraininfo , bdelete);

			SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0, 0);
			SetTileInfo(MOVE_OK ,m_curFloor);
			CTileMgr::GetInstance()->SetTerrain(oriidx + SQ_TILECNTX, m_temptile, m_terraininfo , bdelete);

			SetTerrainInfo(m_curTerrainID ,GROUP_RU , isequenceidx, 1);
			SetTileInfo(MOVE_NONE ,m_curFloor);
			m_terraininfo.byGroup_sequence = isequenceidx + 2;
			if(isequenceidx + 2 != 2)
			{
				m_terraininfo.bysortLV = 1;
				SetTileInfo(MOVE_NONE ,m_curFloor - 1);
			}
			else
			{
				m_terraininfo.bysortLV = 0;
				SetTileInfo(MOVE_NONE ,m_curFloor);
			}
		}
		else if(4 <= getterrain_temp->byGroup_sequence &&
			2 <= isequenceidx)
		{
			SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0, 0);
			if(isequenceidx != 3)
				SetTileInfo(MOVE_OK ,m_curFloor - 1);
			else
				SetTileInfo(MOVE_OK ,m_curFloor);
		}
	}
	else if( GROUP_FLAT == getterrain_temp->byGroup_ID &&
		TERRAIN_HIGHDIRT == getterrain_temp->byTerrain_ID)
		return;

	CTileMgr::GetInstance()->SetTerrain(oriidx , m_temptile, m_terraininfo);
}
void CHighDirt_Brush::RightUpArea(const int startidx)
{
	int tempidx = 0;
	int isequenceidx = 0;

	m_temptile.byOption = MOVE_NONE;



	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			tempidx = i * SQ_TILECNTX + j;
			isequenceidx = i*2+j;

			m_terraininfo.byTerrain_ID = TERRAIN_HIGHDIRT;
			m_terraininfo.byGroup_ID = GROUP_RU;
			m_terraininfo.byGroup_sequence = isequenceidx;

			/*층 대입*/
			if(isequenceidx == 1)
				m_temptile.byFloor = m_curFloor - 1;
			else
				m_temptile.byFloor = m_curFloor;

			/*소팅레벨 대입*/
			if(isequenceidx != 2)	
				m_terraininfo.bysortLV = 1;
			else
				m_terraininfo.bysortLV = 0;


			HD_RU_Algorithm(startidx + tempidx , isequenceidx);


		}
	}
}
void CHighDirt_Brush::HD_RU_Algorithm(const int oriidx , const int isequenceidx)
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
			if(isequenceidx == 3)
			{
				SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0, 1);
				SetTileInfo(MOVE_NONE ,m_curFloor - 1);
			}
			else
			{
				SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0, 0);
				SetTileInfo(MOVE_NONE ,m_curFloor);
			}			
		}
		else if( 2 <= getterrain_temp->byGroup_sequence &&
			0 <= isequenceidx)
		{
			if(m_bOverlap[GROUP_R][GROUP_LD] == true)
			{
				if(isequenceidx <= 1)
				{
					if(m_bOverlap[GROUP_L][GROUP_LD] == true)
					{
						SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0, 0);
						SetTileInfo(MOVE_OK ,m_curFloor);
						CTileMgr::GetInstance()->SetTerrain(oriidx - SQ_TILECNTX, m_temptile, m_terraininfo);
					}
					else
					{
						SetTerrainInfo(m_curTerrainID ,GROUP_LU , isequenceidx + 2, 1);
						SetTileInfo(MOVE_NONE ,m_curFloor - 1);
					}
				}
				else
				{
					SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0, 1);
					SetTileInfo(MOVE_OK ,m_curFloor);
				}

				if(m_bOverlap[GROUP_L][GROUP_RD] == true)
				{
					SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0, 0);
					SetTileInfo(MOVE_NONE ,m_curFloor);
					CTileMgr::GetInstance()->SetTerrain(oriidx - SQ_TILECNTX, m_temptile, m_terraininfo);
				}
			}
			else if(m_bOverlap[GROUP_LU][GROUP_RD] == true)
			{
				if(isequenceidx <= 1)
				{
					SetTerrainInfo(m_curTerrainID , GROUP_FLAT , 0 , 0);
					SetTileInfo(MOVE_NONE , m_curFloor);
					CTileMgr::GetInstance()->SetTerrain(oriidx - SQ_TILECNTX, m_temptile, m_terraininfo);
					SetTerrainInfo(m_curTerrainID , GROUP_RD , isequenceidx , 0);
				}
				else
				{
					bdelete = false;
					SetTileInfo(MOVE_NONE , m_curFloor - 1);
					if(isequenceidx == 3)
					{
						SetTerrainInfo(m_curTerrainID , GROUP_RD , isequenceidx , 0);						
						CTileMgr::GetInstance()->SetTerrain(oriidx , m_temptile, m_terraininfo);
						SetTerrainInfo(m_curTerrainID , GROUP_RU , isequenceidx , 1);
					}
					else
						SetTerrainInfo(m_curTerrainID , GROUP_RU , isequenceidx , 0);


				}
			}
		}
	}
	else if(GROUP_RD == getterrain_temp->byGroup_ID)
	{
		if(m_bOverlap[GROUP_R][GROUP_LD])
		{
			SetTerrainInfo(m_curTerrainID , GROUP_FLAT , 0 , 0);
			SetTileInfo(MOVE_OK , m_curFloor);
		}
		else
		{
			if(isequenceidx <= 1 &&
				getterrain_temp->byGroup_sequence <= 1)
				return;
		}
	}
	else if(GROUP_L == getterrain_temp->byGroup_ID)
	{
		if( 2 <= getterrain_temp->byGroup_sequence &&
			getterrain_temp->byGroup_sequence <= 3 &&
			isequenceidx <= 1)
		{
			SetTerrainInfo(m_curTerrainID ,GROUP_LU , isequenceidx, 1);
			SetTileInfo(MOVE_NONE ,m_curFloor - 1);
			CTileMgr::GetInstance()->SetTerrain(oriidx - SQ_TILECNTX, m_temptile, m_terraininfo , bdelete);

			SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0, 0);
			SetTileInfo(MOVE_OK ,m_curFloor);
			CTileMgr::GetInstance()->SetTerrain(oriidx + SQ_TILECNTX, m_temptile, m_terraininfo , bdelete);

			SetTerrainInfo(m_curTerrainID ,GROUP_LU , 0, 0);

			m_terraininfo.byGroup_sequence = isequenceidx + 2;

			if(isequenceidx + 2 != 3)
			{
				m_terraininfo.bysortLV = 1;
				SetTileInfo(MOVE_NONE ,m_curFloor - 1);
			}
			else
			{
				m_terraininfo.bysortLV = 0;
				SetTileInfo(MOVE_NONE ,m_curFloor);
			}
		}
		else if(4 <= getterrain_temp->byGroup_sequence &&
			2 <= isequenceidx)
		{
			SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0, 0);
			SetTileInfo(MOVE_OK ,m_curFloor);
		}
	}
	else if(GROUP_RU == getterrain_temp->byGroup_ID)
	{
	}
	else if( GROUP_FLAT == getterrain_temp->byGroup_ID &&
		TERRAIN_HIGHDIRT == getterrain_temp->byTerrain_ID)
		return;

	CTileMgr::GetInstance()->SetTerrain(oriidx , m_temptile, m_terraininfo , bdelete);

}
void CHighDirt_Brush::RightArea(const int startidx)
{

	int tempidx = 0;
	int isquenceidx = 0;


	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			tempidx = i * SQ_TILECNTX + j;
			isquenceidx = i*2+j;

			m_temptile.byFloor = m_curFloor - 1;

			if(isquenceidx != 1)
				m_temptile.byOption = MOVE_NONE;
			else
				m_temptile.byOption = MOVE_OK;

			m_terraininfo.byGroup_ID = GROUP_R;
			m_terraininfo.byGroup_sequence = isquenceidx;
			m_terraininfo.byTerrain_ID = TERRAIN_HIGHDIRT;

			if( i == 0 || isquenceidx == 3)
				m_terraininfo.bysortLV = 1;
			else
				m_terraininfo.bysortLV = 0;

			HD_R_Algorithm(startidx + tempidx , isquenceidx);
		}
	}
}
void CHighDirt_Brush::HD_R_Algorithm(const int oriidx , const int isequenceidx)
{
	bool bdelete = true;
	const TERRAIN_INFO*	getterrain_temp = NULL;
	getterrain_temp = CTileMgr::GetInstance()->GetTerrain_Info(oriidx);


	if(GROUP_LU == getterrain_temp->byGroup_ID)
	{
		if(	getterrain_temp->byGroup_sequence <= 1 &&
			2 <= isequenceidx &&
			isequenceidx <= 3)
		{
				SetTerrainInfo(m_curTerrainID ,GROUP_RU , isequenceidx - 2, 1);
				CTileMgr::GetInstance()->SetTerrain(oriidx-SQ_TILECNTX , m_temptile, m_terraininfo , bdelete);

				m_terraininfo.byGroup_sequence = isequenceidx;
				if(isequenceidx != 2)
					m_terraininfo.bysortLV = 1;
				else
					m_terraininfo.bysortLV = 0;
		}
		else if(4 <= isequenceidx && isequenceidx <= 5)
		{
			SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0, 0);
			SetTileInfo(MOVE_OK ,m_curFloor);
		}
		else if(0 <= isequenceidx && isequenceidx <=1)
			return;
		else
			return;
	}
	else if(GROUP_LD == getterrain_temp->byGroup_ID)
	{

			if(m_bOverlap[GROUP_RU][GROUP_RD] == true)
			{
				if(isequenceidx <= 1)
				{
					SetTerrainInfo(m_curTerrainID , GROUP_FLAT , 0 , 0);
					SetTileInfo(MOVE_OK , m_curFloor);
					CTileMgr::GetInstance()->SetTerrain(oriidx - SQ_TILECNTX, m_temptile, m_terraininfo , bdelete);
				}
				SetTerrainInfo(m_curTerrainID , GROUP_RD , isequenceidx , 0);
				if(isequenceidx == 0)
					SetTileInfo(MOVE_NONE , m_curFloor -1);
				else
					SetTileInfo(MOVE_NONE , m_curFloor);
			}
			else
			{
				if(getterrain_temp->byGroup_sequence <= 3 &&
					isequenceidx <= 1)
				{
					SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0, 0);
					SetTileInfo(MOVE_OK ,m_curFloor);

					CTileMgr::GetInstance()->SetTerrain(oriidx - SQ_TILECNTX, m_temptile, m_terraininfo , bdelete);
					;
					SetTerrainInfo(m_curTerrainID ,GROUP_RD , isequenceidx, 0);

					if(isequenceidx == 1)
						SetTileInfo(MOVE_NONE ,m_curFloor);
					else
						SetTileInfo(MOVE_NONE ,m_curFloor - 1);
				}
				else if( 4 <= getterrain_temp->byGroup_sequence &&
					2 <= isequenceidx )
				{
					SetTerrainInfo(m_curTerrainID ,GROUP_RD , isequenceidx + 2, 0);
					SetTileInfo(MOVE_NONE ,m_curFloor - 1);

					CTileMgr::GetInstance()->SetTerrain(oriidx + SQ_TILECNTX, m_temptile, m_terraininfo , bdelete);
					m_terraininfo.byGroup_sequence = isequenceidx;
				}
				else if(getterrain_temp->byGroup_sequence <= 3 &&
					2<= isequenceidx)
				{
					SetTerrainInfo(m_curTerrainID ,GROUP_LD , isequenceidx - 2, 0);
					SetTileInfo(MOVE_NONE ,m_curFloor - 1);
				}

				BYTE tempgroup_id1 = CTileMgr::GetInstance()->GetTerrain_Info(oriidx + 2)->byGroup_ID;
				BYTE tempgroup_id2 = CTileMgr::GetInstance()->GetTerrain_Info(oriidx + SQ_TILECNTX + 2)->byGroup_ID;
				BYTE tempgroup_id3 = CTileMgr::GetInstance()->GetTerrain_Info(oriidx + SQ_TILECNTX)->byGroup_ID;

				if(tempgroup_id1 == GROUP_LD && tempgroup_id2 == GROUP_LU)
				{
					if(isequenceidx <= 1)
					{
						SetTerrainInfo(m_curTerrainID , GROUP_FLAT , 0 , 0);
						SetTileInfo(MOVE_OK , m_curFloor);

						for(int i = 0; i < 2; ++i)
						{
							CTileMgr::GetInstance()->SetTerrain(oriidx + SQ_TILECNTX*i, m_temptile, m_terraininfo , bdelete);
							CTileMgr::GetInstance()->SetTerrain(oriidx + SQ_TILECNTX*i + 2, m_temptile, m_terraininfo , bdelete);
						}
					}
				}
				else if(tempgroup_id1 == GROUP_RD && tempgroup_id2 == GROUP_RU)
				{
					if(isequenceidx <= 1)
					{
						SetTerrainInfo(m_curTerrainID , GROUP_RD , isequenceidx , 0);
						SetTileInfo(MOVE_NONE , m_curFloor - 1);

						CTileMgr::GetInstance()->SetTerrain(oriidx, m_temptile, m_terraininfo , bdelete);

						SetTerrainInfo(m_curTerrainID , GROUP_RU , isequenceidx + 2 , 1);
						CTileMgr::GetInstance()->SetTerrain(oriidx + SQ_TILECNTX, m_temptile, m_terraininfo , false);
						return;
					}
				}
				else if(tempgroup_id3 == GROUP_LU)
				{
					SetTerrainInfo(m_curTerrainID , GROUP_FLAT , 0 , 0);
					SetTileInfo(MOVE_OK , m_curFloor);
				}
			}
	}
	else if(GROUP_R == getterrain_temp->byGroup_ID)
	{
		if( 4 <= isequenceidx)
			bdelete = false;
	}
	else if(GROUP_L == getterrain_temp->byGroup_ID)
	{
		TERRAIN_INFO* temp2 = CTileMgr::GetInstance()->GetTerrain_Info(oriidx + SQ_TILECNTX*2);
		TERRAIN_INFO* temp3 = CTileMgr::GetInstance()->GetTerrain_Info(oriidx - SQ_TILECNTX);
		if( 2<= getterrain_temp->byGroup_sequence && 
			getterrain_temp->byGroup_sequence <= 3 &&
			2<= temp2->byGroup_sequence && 
			temp2->byGroup_sequence <= 3)
		{
			if(isequenceidx <= 1)
			{
				SetTileInfo(MOVE_NONE , m_curFloor);
				SetTerrainInfo(m_curTerrainID , GROUP_FLAT , 0 , 0);
				for(int i = 0; i < 2; ++i)
					CTileMgr::GetInstance()->SetTerrain(oriidx + SQ_TILECNTX*i + 2 , m_temptile, m_terraininfo , bdelete);				

				SetTileInfo(MOVE_NONE , m_curFloor - 1);
				for(int i = 0; i < 3; ++i)
				{
					SetTerrainInfo(m_curTerrainID , GROUP_LD , isequenceidx + i*2 , 0);
					CTileMgr::GetInstance()->SetTerrain(oriidx + SQ_TILECNTX*(i+1) , m_temptile, m_terraininfo , bdelete);				
				}

				if(isequenceidx == 2)
					SetTerrainInfo(m_curTerrainID , GROUP_LU , isequenceidx + 2 , 0);
				else
					SetTerrainInfo(m_curTerrainID , GROUP_LU , isequenceidx + 2 , 1);

			}
		}
	}
	else if(GROUP_RU == getterrain_temp->byGroup_ID)
	{
		if( 4 <= isequenceidx &&
			0 <= getterrain_temp->byGroup_sequence &&
			getterrain_temp->byGroup_sequence <= 1)
		{
			m_terraininfo.bysortLV = 0;
			bdelete = false;
		}
		else
			return;
	}
	else if(GROUP_RD == getterrain_temp->byGroup_ID)
	{
		if( 4 <= getterrain_temp->byGroup_sequence &&
			isequenceidx <= 1)
			bdelete = false;
		else
			return;
	}
	else if(GROUP_FLAT == getterrain_temp->byGroup_ID)
	{
		if(getterrain_temp->byTerrain_ID == m_curTerrainID)
		{

			if(m_bOverlap[GROUP_RU][GROUP_L] == true)
			{
				if( 2 <= isequenceidx &&	isequenceidx <= 3)
				{
					for(int i = 0; i < 2; ++i)
					{
						SetTerrainInfo(m_curTerrainID , GROUP_FLAT , 0 , 0);
						SetTileInfo(MOVE_OK , m_curFloor);
						CTileMgr::GetInstance()->SetTerrain(oriidx - SQ_TILECNTX*(i+1) , m_temptile, m_terraininfo , bdelete);
					}
				}
			}
			else
				return;
		}
	}
	//if(TERRAIN_HIGHDIRT == getterrain_temp->byTerrain_ID)
	//	if(	//GROUP_RD == getterrain_temp->byGroup_ID ||
	//		//GROUP_LD == getterrain_temp->byGroup_ID ||
	//		//GROUP_RU == getterrain_temp->byGroup_ID ||
	//		//GROUP_LU == getterrain_temp->byGroup_ID ||
	//		GROUP_FLAT == getterrain_temp->byGroup_ID)
	//		return;

	CTileMgr::GetInstance()->SetTerrain(oriidx , m_temptile, m_terraininfo , bdelete);
}
void CHighDirt_Brush::RightDown(const int startidx)
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

			if(i == 0)
				m_temptile.byFloor = m_curFloor - 1;
			else
				m_temptile.byFloor = m_curFloor;

			m_terraininfo.byGroup_ID = GROUP_RD;
			m_terraininfo.byGroup_sequence = isquenceidx;
			m_terraininfo.byTerrain_ID = TERRAIN_HIGHDIRT;
			m_terraininfo.bysortLV = 0;

			HD_RD_Algorithm(startidx + tempidx , isquenceidx);
		}
	}
	//InitOverlap(GROUP_R);
}
void CHighDirt_Brush::LeftDown(const int startidx)
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
				m_temptile.byFloor = m_curFloor - 1;
			else
				m_temptile.byFloor = m_curFloor;

			m_terraininfo.byGroup_ID = GROUP_LD;
			m_terraininfo.byGroup_sequence = isquenceidx;
			m_terraininfo.byTerrain_ID = TERRAIN_HIGHDIRT;
			m_terraininfo.bysortLV = 0;

			HD_LD_Algorithm(startidx + tempidx , isquenceidx);
		}
	}
	//InitOverlap(GROUP_L);
}
void CHighDirt_Brush::HD_RD_Algorithm(const int oriidx , const int isequenceidx)
{
	bool bdelete = true;
	TERRAIN_INFO*	get_terrain = NULL;
	get_terrain = CTileMgr::GetInstance()->GetTerrain_Info(oriidx);

	if(GROUP_LU == get_terrain->byGroup_ID)
	{
		if(	get_terrain->byGroup_sequence <=3 &&
			isequenceidx <= 1)
		{
			SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0, 0);
			SetTileInfo(MOVE_OK ,m_curFloor);
		}
		else if(0 <= get_terrain->byGroup_sequence &&
			2 <= isequenceidx && isequenceidx <= 3) 
		{
			//그룹 R이 그룹 LU와 겹칠경우
			if(true == m_bOverlap[GROUP_R][GROUP_LU])
			{
				SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0, 0);
				SetTileInfo(MOVE_OK ,m_curFloor);
				CTileMgr::GetInstance()->SetTerrain(oriidx - SQ_TILECNTX , m_temptile, m_terraininfo , bdelete);

				SetTerrainInfo(m_curTerrainID ,GROUP_LD , isequenceidx - 2, 0);
				SetTileInfo(MOVE_NONE ,m_curFloor);

				if(true == m_bOverlap[GROUP_L][GROUP_RU])
				{
					if( 2 <= isequenceidx)
					{
						SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0, 0);
						SetTileInfo(MOVE_OK ,m_curFloor);
						CTileMgr::GetInstance()->SetTerrain(oriidx + SQ_TILECNTX , m_temptile, m_terraininfo , bdelete);
					}
				}
				else if(true == m_bOverlap[GROUP_L][GROUP_LU])
				{
					SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0, 0);
					SetTileInfo(MOVE_OK ,m_curFloor);
					for(int i = 0; i < 2; ++i)
						CTileMgr::GetInstance()->SetTerrain(oriidx + SQ_TILECNTX*i , m_temptile, m_terraininfo , bdelete);
				}
			}
			else if(true == m_bOverlap[GROUP_LD][GROUP_RU])
			{
					bdelete = false;
					SetTerrainInfo(m_curTerrainID ,GROUP_RU , isequenceidx, 1);

					if(isequenceidx == 2)
						SetTileInfo(MOVE_NONE ,m_curFloor - 1);
					else
						SetTileInfo(MOVE_NONE ,m_curFloor);

					CTileMgr::GetInstance()->SetTerrain(oriidx , m_temptile, m_terraininfo , bdelete);

					SetTerrainInfo(m_curTerrainID ,GROUP_RD , isequenceidx, 0);
					SetTileInfo(MOVE_OK ,m_curFloor - 1);
			}
		}
		else if(0 <= get_terrain->byGroup_sequence && 
			4 <= isequenceidx) 
		{
			if(true == m_bOverlap[GROUP_R][GROUP_LU])
			{
				if(5 != isequenceidx)
				{
					SetTerrainInfo(m_curTerrainID ,GROUP_LD , isequenceidx - 2, 0);
					SetTileInfo(MOVE_NONE ,m_curFloor - 1);
					bdelete = false;
				}
				else
					return;
			}
			else if(true == m_bOverlap[GROUP_LD][GROUP_RU])
			{
				SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0, 0);
				SetTileInfo(MOVE_OK ,m_curFloor);
			}
			else
				return;
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
		if(m_bOverlap[GROUP_R][GROUP_LU] == true)
		{
			if(isequenceidx <= 4)
			{
				SetTerrainInfo(m_curTerrainID , GROUP_FLAT , 0 ,0);
				SetTileInfo(MOVE_OK , m_curFloor);
			}
			else
				return;
		}
		else
		{
			if(isequenceidx <= 1 && get_terrain->byGroup_sequence <= 1)
				bdelete = true;
			else if(3 == isequenceidx && get_terrain->byGroup_sequence == 3)
				bdelete = false;
			else if(isequenceidx == 2)
				return;
			else
				bdelete = false;
		}	
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
			SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0, 0);
			SetTileInfo(MOVE_OK ,m_curFloor);

			CTileMgr::GetInstance()->SetTerrain(oriidx - SQ_TILECNTX , m_temptile, m_terraininfo , bdelete);

			SetTerrainInfo(m_curTerrainID ,GROUP_LD , isequenceidx - 2, 0);
			if(isequenceidx - 2 == 1)
				SetTileInfo(MOVE_NONE ,m_curFloor);
			else
				SetTileInfo(MOVE_NONE ,m_curFloor - 1);
		}
		else if( 2 <= get_terrain->byGroup_sequence && 
			get_terrain->byGroup_sequence <= 3 &&
			4 <= isequenceidx &&
			isequenceidx <= 5)
		{
			SetTerrainInfo(m_curTerrainID ,GROUP_LD , isequenceidx, 0);
			SetTileInfo(MOVE_NONE ,m_curFloor - 1);

			CTileMgr::GetInstance()->SetTerrain(oriidx + SQ_TILECNTX, m_temptile, m_terraininfo , bdelete);

			m_terraininfo.byGroup_sequence = isequenceidx - 2;
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

	CTileMgr::GetInstance()->SetTerrain(oriidx , m_temptile, m_terraininfo , bdelete);
}

void CHighDirt_Brush::HD_LD_Algorithm(const int oriidx , const int isequenceidx)
{
	bool bdelete = true;
	const TERRAIN_INFO*	get_terrain = NULL;
	get_terrain = CTileMgr::GetInstance()->GetTerrain_Info(oriidx);

	if(GROUP_RU == get_terrain->byGroup_ID)
	{
		if(	get_terrain->byGroup_sequence <=3 &&
			isequenceidx <= 1)
		{
			SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0, 0);
			SetTileInfo(MOVE_OK ,m_curFloor);
		}
		else if(2 <= isequenceidx && isequenceidx <= 3) 
		{
			if(true == m_bOverlap[GROUP_L][GROUP_RU])
			{
				SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0, 0);
				SetTileInfo(MOVE_OK ,m_curFloor);
				CTileMgr::GetInstance()->SetTerrain(oriidx - SQ_TILECNTX , m_temptile, m_terraininfo , bdelete);

				SetTerrainInfo(m_curTerrainID ,GROUP_RD , isequenceidx - 2, 0);
				SetTileInfo(MOVE_NONE ,m_curFloor);

				//bdelete = false;

				if(true == m_bOverlap[GROUP_R][GROUP_LU])
				{
					if( 2 <= isequenceidx)
					{
						SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0, 0);
						SetTileInfo(MOVE_OK ,m_curFloor);
						CTileMgr::GetInstance()->SetTerrain(oriidx + SQ_TILECNTX , m_temptile, m_terraininfo , bdelete);
					}
				}
				else if(true == m_bOverlap[GROUP_R][GROUP_RU])
				{
					SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0, 0);
					SetTileInfo(MOVE_OK ,m_curFloor);
					for(int i = 0; i < 2; ++i)
						CTileMgr::GetInstance()->SetTerrain(oriidx + SQ_TILECNTX*i , m_temptile, m_terraininfo , bdelete);
				}
			}
			else if(true == m_bOverlap[GROUP_RD][GROUP_LU])
			{
				bdelete = false;

				SetTerrainInfo(m_curTerrainID ,GROUP_LU , isequenceidx, 1);

				if(isequenceidx == 2)
					SetTileInfo(MOVE_NONE ,m_curFloor - 1);
				else
					SetTileInfo(MOVE_NONE ,m_curFloor);

				CTileMgr::GetInstance()->SetTerrain(oriidx , m_temptile, m_terraininfo , bdelete);

				SetTerrainInfo(m_curTerrainID ,GROUP_LD , isequenceidx, 0);
				SetTileInfo(MOVE_OK ,m_curFloor - 1);
			}
		}
		else if(0 <= get_terrain->byGroup_sequence && 
			4 <= isequenceidx) 
		{
			if(true == m_bOverlap[GROUP_L][GROUP_RU])
			{
				if(4 != isequenceidx)
				{
					SetTerrainInfo(m_curTerrainID ,GROUP_RD , isequenceidx - 2, 0);
					SetTileInfo(MOVE_NONE ,m_curFloor - 1);
					bdelete = false;
				}
				else
					return;
			}
			else if(true == m_bOverlap[GROUP_RD][GROUP_LU])
			{
				SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0, 0);
				SetTileInfo(MOVE_OK ,m_curFloor);
			}
			else
				return;
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
		if(m_bOverlap[GROUP_L][GROUP_RU] == true)
		{
			if(isequenceidx <= 4)
			{
				SetTerrainInfo(m_curTerrainID , GROUP_FLAT , 0 ,0);
				SetTileInfo(MOVE_OK , m_curFloor);
			}
			else
				return;
		}
		else
		{
			if(isequenceidx <= 1 && get_terrain->byGroup_sequence <= 1)
				bdelete = true;
			else if(2 == isequenceidx && get_terrain->byGroup_sequence == 2)
				bdelete = false;
			else if(isequenceidx == 3)
				return;
			else
				bdelete = false;
		}
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
			SetTerrainInfo(m_curTerrainID ,GROUP_FLAT , 0, 0);
			SetTileInfo(MOVE_OK ,m_curFloor);

			CTileMgr::GetInstance()->SetTerrain(oriidx - SQ_TILECNTX , m_temptile, m_terraininfo , bdelete);

			SetTerrainInfo(m_curTerrainID ,GROUP_RD , isequenceidx - 2, 0);
			if(isequenceidx - 2 == 0)
				SetTileInfo(MOVE_NONE ,m_curFloor);
			else
				SetTileInfo(MOVE_NONE ,m_curFloor - 1);
		}
		else if( 2 <= get_terrain->byGroup_sequence && 
			get_terrain->byGroup_sequence <= 3 &&
			4 <= isequenceidx &&
			isequenceidx <= 5)
		{
			SetTerrainInfo(m_curTerrainID ,GROUP_RD , isequenceidx, 0);
			SetTileInfo(MOVE_NONE ,m_curFloor - 1);

			CTileMgr::GetInstance()->SetTerrain(oriidx + SQ_TILECNTX, m_temptile, m_terraininfo , bdelete);

			m_terraininfo.byGroup_sequence = isequenceidx - 2;
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

	CTileMgr::GetInstance()->SetTerrain(oriidx , m_temptile , m_terraininfo , bdelete);
}

void CHighDirt_Brush::SetTerrainInfo( BYTE terrainid ,BYTE bygroup_id ,BYTE bysequence , BYTE bysortlv)
{
	m_terraininfo.byGroup_ID = bygroup_id;
	m_terraininfo.byGroup_sequence = bysequence;
	m_terraininfo.bysortLV = bysortlv;
}

void CHighDirt_Brush::SetTileInfo(BYTE option , BYTE byfloor)
{
	m_temptile.byOption = option;
	m_temptile.byFloor = byfloor;
}

