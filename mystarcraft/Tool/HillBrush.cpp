#include "StdAfx.h"
#include "HillBrush.h"
#include "TileMgr.h"

#include "Hill_R.h"
#include "Hill_L.h"
CHillBrush::CHillBrush(void)
{
	for(int i = 0; i < GROUP_END; ++i)
		m_pGroup[i] = NULL;

	m_pGroup[GROUP_R] = new CHill_R;
	m_pGroup[GROUP_L] = new CHill_L;
}

CHillBrush::~CHillBrush(void)
{
	for(int i = 0; i < GROUP_END; ++i)
		Safe_Delete(m_pGroup[i]);
}

void CHillBrush::BrushPaint()
{
	//오른쪽 언덕일떄 왼쪽언덕일때 분기문 작성


	int idx = CTileMgr::GetInstance()->GetIdx();

	int isquence;
	for(int i = -3; i < 3; ++i)
	{
		for(int j = -3; j < 3; ++j)
		{
			isquence = (i+3)*6 + (j+3);
			if(!CTileMgr::GetInstance()->InstallHillCheck(idx + i*SQ_TILECNTX+j , isquence , m_eHillDir))
				return;
		}
	}

	if(HILL_R == m_eHillDir)
		m_pGroup[GROUP_R]->MakeTerrain_Group(idx);
	else if(HILL_L == m_eHillDir)
		m_pGroup[GROUP_L]->MakeTerrain_Group(idx);

	for(int i = 0; i < GROUP_END; ++i)
	{
		if( NULL == m_pGroup[i])
			continue;

		m_pGroup[i]->UpdateTerrain();
	}
	CTileMgr::GetInstance()->TileOption_Update();
}

void CHillBrush::SetHillDir(HILL_DIR edir)
{
	m_eHillDir = edir;
}
