#include "StdAfx.h"
#include "Zerg_Building.h"

#include "MyMath.h"
#include "TileManager.h"
#include "MyMath.h"
#include "TimeMgr.h"
#include "MyMath.h"
#include "ScrollMgr.h"
CZerg_Building::CZerg_Building(void)
{
}

CZerg_Building::~CZerg_Building(void)
{
}

void CZerg_Building::Initialize(void)
{
}
void CZerg_Building::Creep_Initialize(bool bcomplete)
{
	m_creepoff_timer = 0.f;
	m_creeptimer = 0.f;
	m_loopcnt1 = 1;
	m_loopcnt2 = 1;

	m_bswitch = true;
	m_bdestroy = false;

	for(int i = 0; i < LINE_END; ++i)
	{
		m_top_line[i] = m_offset;
		m_bot_line[i] = m_offset;
	}

	for(int i = 0; i < UPDOWN_END; ++i)
	{
		m_left_line[i] = m_offset;
		m_right_line[i] = m_offset;
	}

	m_sqTile = CTileManager::GetInstance()->GetSqTile();


	int fradius = m_irange/2*32;
	int istartX = int( m_vPos.x - fradius )/SQ_TILESIZEX;
	int istartY = int( m_vPos.y - fradius )/SQ_TILESIZEY;

	int startIdx = CMyMath::Pos_to_index(m_vPos.x , m_vPos.y);	
	int destidx = 0;

	m_creepoff_list.push_back(startIdx);
	CTileManager::GetInstance()->SetCreepInstall(startIdx , true);


	if(istartY < 0)
		istartY = 0;
	if(istartY + m_irange - 1 >= SQ_TILECNTY)
		istartY = SQ_TILECNTY - m_irange;

	if(istartX < 0)
		istartX = 0;
	if(istartX + m_irange - 1 >= SQ_TILECNTX)
		istartX = SQ_TILECNTX - m_irange;

	m_creeptimer += GETTIME;

	while(m_bswitch)//초기화 단계에서 한번만 실행 
	{
		--m_top_line[LINE_LDIR];
		++m_top_line[LINE_RDIR];

		--m_bot_line[LINE_LDIR];
		++m_bot_line[LINE_RDIR];

		--m_left_line[LINE_UP];
		++m_left_line[LINE_DOWN];

		--m_right_line[LINE_UP];
		++m_right_line[LINE_DOWN];


		if( m_irange - 1 <= m_left_line[LINE_DOWN] )
		{
			m_left_line[LINE_DOWN] = m_offset;
			m_right_line[LINE_DOWN] = m_offset;
			++m_loopcnt2;
		}
		if( m_left_line[LINE_UP] < 1 )
		{
			m_left_line[LINE_UP] = m_offset;
			m_right_line[LINE_UP] = m_offset;
		}


		if(m_irange  <= m_top_line[LINE_RDIR])
		{
			m_top_line[LINE_RDIR] = m_offset;
			m_bot_line[LINE_RDIR] = m_offset;
			++m_loopcnt1;
		}
		if(m_top_line[LINE_LDIR] < 0)
		{
			m_top_line[LINE_LDIR] = m_offset;
			m_bot_line[LINE_LDIR] = m_offset;
		}

		if(m_loopcnt1 >= (m_irange >> 1) + 1)
		{
			m_bswitch = false;
			break;
		}


		for(int k = 0; k < LINE_END; ++k)
		{
			destidx = istartY*SQ_TILECNTX + (istartX + m_top_line[k]);

			if(istartX + m_top_line[k] >= SQ_TILECNTX || istartX + m_top_line[k] < 0)
				continue;
			if(destidx < 0 || destidx>= SQ_TILECNTY*SQ_TILECNTX || 
				startIdx < 0 || startIdx >= SQ_TILECNTY*SQ_TILECNTX)
				continue;

			CMyMath::Bresenham_Creep( m_sqTile[startIdx]->vPos , m_sqTile[destidx]->vPos ,fradius ,m_loopcnt1 , m_creep_list);
		}


		for(int k = 0; k < LINE_END; ++k)
		{
			destidx = (istartY + m_irange - 1)*SQ_TILECNTX + (istartX + m_bot_line[k]);

			if(istartX + m_bot_line[k] >= SQ_TILECNTX || istartX + m_bot_line[k] < 0)
				continue;
			if(destidx < 0 || destidx>= SQ_TILECNTY*SQ_TILECNTX || 
				startIdx < 0 || startIdx >= SQ_TILECNTY*SQ_TILECNTX)
				continue;

			CMyMath::Bresenham_Creep( m_sqTile[startIdx]->vPos , m_sqTile[destidx]->vPos ,fradius ,m_loopcnt1, m_creep_list);
		}


		//============================================================================================================================================

		for(int k = 0; k < UPDOWN_END; ++k)
		{
			destidx = (istartY + m_left_line[k])*SQ_TILECNTX + istartX;

			if(istartX < 0 || istartX >= SQ_TILECNTX)
				continue;
			if(destidx < 0 || destidx>= SQ_TILECNTY*SQ_TILECNTX || 
				startIdx < 0 || startIdx >= SQ_TILECNTY*SQ_TILECNTX)
				continue;

			CMyMath::Bresenham_Creep(m_sqTile[startIdx]->vPos , m_sqTile[destidx]->vPos ,fradius ,m_loopcnt2, m_creep_list);
		}


		for(int k = 0; k < UPDOWN_END; ++k)
		{
			destidx = (istartY + m_right_line[k])*SQ_TILECNTX + (istartX + m_irange - 1);

			if(istartX + m_irange - 1 < 0 || istartX + m_irange - 1 >= SQ_TILECNTX)
				continue;
			if(destidx < 0 || destidx >= SQ_TILECNTY*SQ_TILECNTX || 
				startIdx < 0 || startIdx >= SQ_TILECNTY*SQ_TILECNTX)
				continue;

			CMyMath::Bresenham_Creep(m_sqTile[startIdx]->vPos , m_sqTile[destidx]->vPos ,fradius ,m_loopcnt2, m_creep_list);
		}
	}

	m_bcreep_complete = bcomplete;

	int idx = 0;
	if(false == m_bcreep_complete)
	{
		for(int i = 0; i < 81; ++i) //초기화 단계에서 한번만 실행 
		{
			if(m_creep_list.empty())
				break;

			idx = m_creep_list.front();
			m_creep_list.pop_front();

			if(false == CTileManager::GetInstance()->GetCreepInstall(idx))
			{
				CTileManager::GetInstance()->SetCreepInstall(idx , true);
				CTileManager::GetInstance()->Creep_Autotile(idx);
				m_creepoff_list.push_back(idx);
			}
		}
	}
	else
	{
		while(!m_creep_list.empty())
		{
			idx = m_creep_list.front();
			m_creep_list.pop_front();

			if(false == CTileManager::GetInstance()->GetCreepInstall(idx))
			{
				CTileManager::GetInstance()->SetCreepInstall(idx , true);
				CTileManager::GetInstance()->Creep_Autotile(idx);
				m_creepoff_list.push_back(idx);
			}
		}
	}

}

void CZerg_Building::Update(void)
{

}

void CZerg_Building::Render(void)
{

}

void CZerg_Building::Release(void)
{

}

void CZerg_Building::Creep_expansion(void)
{
	if(true == m_bcreep_complete || true == m_bdestroy)
		return;

	m_creeptimer += GETTIME;

	if(m_creeptimer >= 0.1f && !m_creep_list.empty()) //업데이트단계
	{
		m_creeptimer = 0.f;
		int idx = 0;
		while(!m_creep_list.empty())
		{
			idx = m_creep_list.front();
			m_creep_list.pop_front();

			if(false == CTileManager::GetInstance()->GetCreepInstall(idx))
			{
				CTileManager::GetInstance()->SetCreepInstall(idx , true);
				CTileManager::GetInstance()->Creep_Autotile(idx);
				m_creepoff_list.push_back(idx);
				break;
			}
		}
	}

	if(m_creep_list.empty())
		m_bcreep_complete = true;
}
void CZerg_Building::Creep_decrase(void)
{
	if(m_creepoff_list.empty() || false == m_bdestroy)
		return;

	m_creepoff_timer += GETTIME;

	int idx = 0;
	if(m_creepoff_timer >= 0.1)
	{
		m_creepoff_timer = 0.f;

		idx = m_creepoff_list.back();
		m_creepoff_list.pop_back();

		CTileManager::GetInstance()->SetCreepInstall(idx , false);
		CTileManager::GetInstance()->Creep_decrease_autotile(idx);
	}
}
void CZerg_Building::SetDestroy(void)
{
	m_bdestroy = true;
}