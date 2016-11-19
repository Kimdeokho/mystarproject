#include "StdAfx.h"
#include "Hill_L.h"

CHill_L::CHill_L(void)
{
}

CHill_L::~CHill_L(void)
{
}

void CHill_L::MakeTerrain_Group(const int istartidx)
{
	m_startidx = istartidx;

	int isquence = 0;
	for(int i = -3; i < 3; ++i)
	{
		for(int j = -3; j < 3; ++j)
		{
			isquence = (i+3)*6 + (j+3);
			SetTerrainInfo(m_startidx + (SQ_TILECNTX*i) + j , m_terrain_id , GROUP_L , isquence , 0 , true );
		}
	}
}
