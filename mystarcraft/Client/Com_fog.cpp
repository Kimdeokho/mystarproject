#include "StdAfx.h"
#include "Com_fog.h"

#include "TimeMgr.h"
#include "Obj.h"
#include "MyMath.h"
#include "TileManager.h"

CCom_fog::CCom_fog(const int& curidx , const int* irange):m_rcuridx32(curidx)
 , m_iSightrange(irange)
{
}

CCom_fog::~CCom_fog(void)
{
	Release();
}

void CCom_fog::Initialize(CObj* pobj)
{
	m_pobj = pobj;

	m_fogtime = 0.f;
	m_sightoffsw = false;
	m_InitSight = false;
	memset(m_fogsearch , 0 , sizeof(m_fogsearch));

	m_oldidx32 = -1;

	m_Sightoff_List.reserve( (BACKBUFFER_SIZEX/SQ_TILESIZEX) * (BACKBUFFER_SIZEY/SQ_TILESIZEY)*2);
}
void CCom_fog::Update(void)
{
	/*m_iSightrange는 픽셀단위 범위, 32배수*/

	if(true == m_sightoffsw)
		m_fogtime += GETTIME;

	if(m_fogtime >3.0f)
	{
		if(!m_Sightoff_List.empty() )
		{
			DWORD	fogsize = m_Sightoff_List.size();

			for(DWORD i = 0; i < fogsize; ++i)
			{
				m_fogsearch[ m_Sightoff_List[i] ] = false;
				CTileManager::GetInstance()->SightOffRender(m_Sightoff_List[i]);
			}
			m_Sightoff_List.clear();

			//vector로 바꿔야할듯
			//list<int>::iterator iter = m_Sightoff_List.begin();
			//list<int>::iterator iter_end = m_Sightoff_List.end();

			//for( ; iter != iter_end; ++iter)
			//{
			//	m_fogsearch[(*iter)] = false;
			//	CTileManager::GetInstance()->SightOffRender(*iter);
			//}

			//m_Sightoff_List.clear();
			//printf("안개 끄기\n");
		}
		m_fogtime = 0.f;
		m_sightoffsw = false;
		CTileManager::GetInstance()->SightOnRender(m_rcuridx32 , *m_iSightrange , m_Sightoff_List , m_fogsearch , m_pobj->GetUnitinfo().eMoveType);
		//printf("안개 지우기\n");
	}

	if(m_rcuridx32 != m_oldidx32)
	{
		m_sightoffsw = true;		
		CTileManager::GetInstance()->SightOnRender(m_rcuridx32 , *m_iSightrange , m_Sightoff_List , m_fogsearch , m_pobj->GetUnitinfo().eMoveType);
		m_oldidx32 = m_rcuridx32;
	}
}

void CCom_fog::Release(void)
{
	m_pobj = NULL;
	if(!m_Sightoff_List.empty())
	{
		//list<int>::iterator iter = m_Sightoff_List.begin();
		//list<int>::iterator iter_end = m_Sightoff_List.end();

		//for( ; iter != iter_end; ++iter)
		//{
		//	m_fogsearch[(*iter)] = false;
		//	CTileManager::GetInstance()->SightOffRender(*iter);
		//}

		//m_Sightoff_List.clear();

		DWORD	fogsize = m_Sightoff_List.size();

		for(DWORD i = 0; i < fogsize; ++i)
		{
			m_fogsearch[ m_Sightoff_List[i] ] = false;
			CTileManager::GetInstance()->SightOffRender(m_Sightoff_List[i]);
		}
		m_Sightoff_List.clear();
	}

	vector<int> temp;
	temp.swap(m_Sightoff_List);
}

