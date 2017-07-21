#include "StdAfx.h"
#include "Com_fog.h"

#include "TimeMgr.h"
#include "Obj.h"
#include "MyMath.h"
#include "TileManager.h"

CCom_fog::CCom_fog(const int& curidx , const int& irange):m_rcuridx32(curidx)
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
}
void CCom_fog::Update(void)
{
	/*m_iSightrange�� �ȼ����� ����, 32���*/
	if(true == m_sightoffsw)
		m_fogtime += GETTIME;

	if(m_fogtime >3.0f)
	{
		if(!m_Sightoff_List.empty() )
		{
			//vector�� �ٲ���ҵ�
			list<int>::iterator iter = m_Sightoff_List.begin();
			list<int>::iterator iter_end = m_Sightoff_List.end();

			for( ; iter != iter_end; ++iter)
			{
				m_fogsearch[(*iter)] = false;
				CTileManager::GetInstance()->SightOffRender(*iter);
			}

			m_Sightoff_List.clear();
		}
		m_fogtime = 0.f;
		m_sightoffsw = false;
		CTileManager::GetInstance()->SightOnRender(m_rcuridx32 , m_iSightrange , m_Sightoff_List , m_fogsearch , m_pobj->GetType());
	}

	if(m_rcuridx32 != m_oldidx32)
	{
		m_sightoffsw = true;		
		CTileManager::GetInstance()->SightOnRender(m_rcuridx32 , m_iSightrange , m_Sightoff_List , m_fogsearch , m_pobj->GetType());
		m_oldidx32 = m_rcuridx32;
	}
}

void CCom_fog::Release(void)
{
	if(!m_Sightoff_List.empty())
	{
		m_Sightoff_List.clear();
	}
}
