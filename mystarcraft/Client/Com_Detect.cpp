#include "StdAfx.h"
#include "Com_Detect.h"

#include "Obj.h"

#include "Area_Mgr.h"
#include "TimeMgr.h"

#include "MyMath.h"
CCom_Detect::CCom_Detect(void)
{
}

CCom_Detect::~CCom_Detect(void)
{
}

void CCom_Detect::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;

	m_range = 8*32;

	m_detecttime = 0.2f;
	m_area64 = CArea_Mgr::GetInstance()->GetArea64();
}

void CCom_Detect::Update(void)
{
	m_detecttime += GETTIME;
	if(m_detecttime > 0.2f)
	{
		m_detecttime = 0.f;

		TEAM_NUMBER eteam = m_pobj->GetTeamNumber();

		list<CObj*>::iterator iter = m_oldlist.begin();
		list<CObj*>::iterator iter_end = m_oldlist.end();

		for( ; iter != iter_end; ++iter) 
		{
			if((*iter)->GetUnitinfo().is_hide)
				(*iter)->Get_ref_Unitinfo().detect[eteam] -= 1;
		}

		m_oldlist.clear();

		D3DXVECTOR2 vpos = m_pobj->GetPos();
		D3DXVECTOR2 vstartpos = vpos;
		D3DXVECTOR2 vendpos = vpos;

		if(vstartpos.x - (m_range + 64) < 0)
			vstartpos.x = 0;
		else
			vstartpos.x = vstartpos.x - (m_range + 64);

		if(vstartpos.y - (m_range + 64) < 0)
			vstartpos.y = 0;
		else
			vstartpos.y = vstartpos.y - (m_range + 64);

		if(vendpos.x + (m_range + 64) > 4096)
			vendpos.x = 4096;
		else
			vendpos.x = vendpos.x + (m_range + 64);

		if(vendpos.y + (m_range + 64) > 4096)
			vendpos.y = 4096;
		else
			vendpos.y = vendpos.y + (m_range + 64);

		int idx = CMyMath::Pos_to_index(vstartpos , 64);
		vstartpos = CMyMath::index_to_Pos(idx , 64 , 64);
		vpos = vstartpos;

		int colcnt = int((vendpos.x - vstartpos.x) / 64) + 1;
		int rowcnt = int((vendpos.y - vstartpos.y) / 64) + 1;
			

		for(int i = 0; i < colcnt; ++i)
		{
			for(int j = 0; j < rowcnt; ++j)
			{
				idx = CMyMath::Pos_to_index(vpos , 64);

				if(!m_area64[idx].empty())
				{
					iter = m_area64[idx].begin();
					iter_end = m_area64[idx].end();

					for( ; iter != iter_end; ++iter)
					{
						if(false == (*iter)->GetUnitinfo().is_hide)
							continue;

						if(CMyMath::pos_distance(m_pobj->GetPos() , (*iter)->GetPos()) < m_range*m_range)
						{
							//sethide(true);
							(*iter)->Get_ref_Unitinfo().detect[eteam] += 1;

							m_oldlist.push_back((*iter));
						}
					}
				}


				if(vpos.x + 64 > 4096)
					break;

				vpos.x += 64;
			}

			if(vpos.y + 64 > 4096)
				break;

			vpos.x = vstartpos.x;
			vpos.y += 64;
		}
	}
}

void CCom_Detect::Render(void)
{

}

void CCom_Detect::Release(void)
{

}
