#include "StdAfx.h"
#include "Hill_R.h"

CHill_R::CHill_R(void)
{
}

CHill_R::~CHill_R(void)
{
}

void CHill_R::MakeTerrain_Group(const int istartidx)
{
	m_startidx = istartidx;

	int isquence = 0;
	for(int i = -3; i < 3; ++i)
	{
		for(int j = -3; j < 3; ++j)
		{
			isquence = (i+3)*6 + (j+3);
			SetTerrainInfo(m_startidx + (SQ_TILECNTX*i) + j , m_terrain_id , GROUP_R , isquence , 0 , true );
		}
	}
}
