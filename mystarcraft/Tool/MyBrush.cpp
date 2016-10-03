#include "StdAfx.h"
#include "MyBrush.h"
#include "TileMgr.h"

CMyBrush::CMyBrush(void):m_curFloor(1)
{
}

CMyBrush::~CMyBrush(void)
{
}
bool CMyBrush::GetOverlap_GroupArea(const BYTE group_id ,const BYTE overlap_id)
{
	return m_bOverlap[group_id][overlap_id];
}
void CMyBrush::Overlap_GroupArea(const int irow ,const int icol ,const int startidx ,const int group_id)
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