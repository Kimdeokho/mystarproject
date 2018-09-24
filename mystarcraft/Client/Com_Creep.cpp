#include "StdAfx.h"
#include "Com_Creep.h"

#include "Obj.h"
#include "TileManager.h"
#include "TimeMgr.h"
#include "MyMath.h"
#include "ObjMgr.h"
#include "Creep_collapse.h"
CCom_Creep::CCom_Creep(bool complete )
{
	m_bcreep_complete = complete;
}

CCom_Creep::~CCom_Creep(void)
{
}

void CCom_Creep::Initialize(CObj* pobj /*= NULL*/)
{
	m_pobj = pobj;
	m_obj_pos = m_pobj->GetPos();

	int range = 15;
	int offset = range / 2;
	int loopcnt = 1;
	int startIdx = CMyMath::Pos_to_index(m_obj_pos.x , m_obj_pos.y);
	int fradius = (offset*32 + 16)*(offset*32 + 16);

	D3DXVECTOR2 vstartpos;
	D3DXVECTOR2 vup_destpos , vdown_destpos, vleft_destpos, vright_destpos;

	vstartpos = m_obj_pos;
	vup_destpos = m_obj_pos;
	vdown_destpos = m_obj_pos;
	vleft_destpos = m_obj_pos;
	vright_destpos = m_obj_pos;

	int idx = 0;
	int startidx = CMyMath::Pos_to_index(m_obj_pos , 32);

	map<int, int> finddix;
	map<int, int>::iterator iter;

	while(offset >= 0)
	{
		--offset;
		for(int i  = 0; i < 4; ++i)
			m_creep_list[i].push_back(list<int>());

		D3DXVECTOR2 temp_pos;

		temp_pos = vup_destpos;
		temp_pos.y += 32;
		for(int i = 0; i < loopcnt; ++i)
		{
			if(temp_pos.y <= 0)
				break;
			temp_pos.y -= 32;
			idx = CMyMath::Pos_to_index(temp_pos , 32);			

			if(CTileManager::GetInstance()->Bresenham_Tilecheck(vstartpos , temp_pos))
				continue;	
			if(CMyMath::pos_distance(vstartpos, temp_pos) > fradius)
				continue;

			if(finddix.find(idx) == finddix.end())
			{
				m_creep_list[CREEP_UP].back().push_back(idx);			
				finddix.insert(make_pair(idx , idx));
			}
		}

		temp_pos = vright_destpos;
		temp_pos.x -= 32;
		for(int i = 0; i < loopcnt; ++i)
		{
			if(temp_pos.x >= 4096)
				break;

			temp_pos.x += 32;
			idx = CMyMath::Pos_to_index(temp_pos , 32);						

			if(CTileManager::GetInstance()->Bresenham_Tilecheck(vstartpos , temp_pos))
				continue;
			if(CMyMath::pos_distance(vstartpos, temp_pos) > fradius)
				continue;

			if(finddix.find(idx) == finddix.end())
			{
				m_creep_list[CREEP_RIGHT].back().push_back(idx);			
				finddix.insert(make_pair(idx , idx));
			}
		}

		temp_pos = vleft_destpos;
		temp_pos.x += 32;
		for(int i = 0; i < loopcnt; ++i)
		{
			if(temp_pos.x <= 0)
				break;

			temp_pos.x -= 32;
			idx = CMyMath::Pos_to_index(temp_pos , 32);						

			if(CTileManager::GetInstance()->Bresenham_Tilecheck(vstartpos , temp_pos))
				continue;	
			if(CMyMath::pos_distance(vstartpos, temp_pos) > fradius)
				continue;

			if(finddix.find(idx) == finddix.end())
			{
				m_creep_list[CREEP_LEFT].back().push_back(idx);			
				finddix.insert(make_pair(idx , idx));
			}
		}

		temp_pos = vdown_destpos;
		temp_pos.y -= 32;
		for(int i = 0; i < loopcnt; ++i)
		{
			if(temp_pos.y >= 4096)
				break;

			temp_pos.y += 32;
			idx = CMyMath::Pos_to_index(temp_pos , 32);						

			if(CTileManager::GetInstance()->Bresenham_Tilecheck(vstartpos , temp_pos))
				continue;	
			if(CMyMath::pos_distance(vstartpos, temp_pos) > fradius)
				continue;

			if(finddix.find(idx) == finddix.end())
			{
				m_creep_list[CREEP_DOWN].back().push_back(idx);			
				finddix.insert(make_pair(idx , idx));
			}
		}


		vup_destpos.x -= 32;
		vup_destpos.y += 32;

		vdown_destpos.x += 32;
		vdown_destpos.y -= 32;

		vleft_destpos.x += 32;
		vleft_destpos.y += 32;

		vright_destpos.x -= 32;
		vright_destpos.y -= 32;

		loopcnt += 2;
	}

	finddix.clear();

	if(true == m_bcreep_complete)
	{
		bool bescape = false;
		while(!bescape)
		{
			for(int i = 0; i < 4; ++i)
			{
				if(!m_creep_list[i].empty())
				{
					while(!m_creep_list[i].front().empty())
					{
						USHORT umiddle = m_creep_list[i].front().size()/2;
						list<int>::iterator iter = m_creep_list[i].front().begin();

						for(USHORT k = 0; k < umiddle; ++k)
							++iter;

						idx = (*iter);
						m_creep_list[i].front().erase(iter);

						if(false == CTileManager::GetInstance()->GetCreepInstall(idx))
						{
							//CTileManager::GetInstance()->SetCreepInstall(idx , true);
							CTileManager::GetInstance()->Creep_Autotile(idx);
							m_creepoff_list.push_back(idx);
							break;
						}
					}
					if(m_creep_list[i].front().empty())
						m_creep_list[i].pop_front();
				}
			}

			bescape = true;
			for(int j = 0; j < CREEP_LINEEND; ++j)
			{
				if(!m_creep_list[j].empty())
				{
					bescape = false;
					break;
				}
			}
		}
	}
	else
	{
		for(int k = 0; k < 6; ++k)
		{
			for(int i = 0; i < 4; ++i)
			{
				if(!m_creep_list[i].empty())
				{
					while(!m_creep_list[i].front().empty())
					{
						USHORT umiddle = m_creep_list[i].front().size()/2;
						list<int>::iterator iter = m_creep_list[i].front().begin();

						for(USHORT k = 0; k < umiddle; ++k)
							++iter;

						idx = (*iter);
						m_creep_list[i].front().erase(iter);

						if(false == CTileManager::GetInstance()->GetCreepInstall(idx))
						{
							//CTileManager::GetInstance()->SetCreepInstall(idx , true);
							CTileManager::GetInstance()->Creep_Autotile(idx);
							m_creepoff_list.push_back(idx);
							break;
						}
					}
					if(m_creep_list[i].front().empty())
						m_creep_list[i].pop_front();
				}
			}
		}
	}

	m_creeptimer = 0.f;
	m_eline = 0;
}

void CCom_Creep::Update(void)
{
	Creep_expansion();
}

void CCom_Creep::Render(void)
{

}
void CCom_Creep::Creep_expansion(void)
{
	if(true == m_bcreep_complete)
		return;

	m_creeptimer += GETTIME;

	if(m_creeptimer >= 0.1f) //업데이트단계
	{
		m_creeptimer = 0.f;
		int idx = 0;
		++m_eline;

		m_eline = m_eline % 4;

		if(!m_creep_list[m_eline].empty())
		{
			while(!m_creep_list[m_eline].front().empty())
			{
				USHORT umiddle = m_creep_list[m_eline].front().size()/2;
				list<int>::iterator iter = m_creep_list[m_eline].front().begin();

				for(USHORT i = 0; i < umiddle; ++i)
					++iter;

				idx = (*iter);
				m_creep_list[m_eline].front().erase(iter);

				if(false == CTileManager::GetInstance()->GetCreepInstall(idx))
				{
					//CTileManager::GetInstance()->SetCreepInstall(idx , true);
					CTileManager::GetInstance()->Creep_Autotile(idx);
					m_creepoff_list.push_back(idx);
					break;
				}
			}
			if(m_creep_list[m_eline].front().empty())
				m_creep_list[m_eline].pop_front();
		}
	}

	for(int i = 0; i < CREEP_LINEEND; ++i)
	{
		if(!m_creep_list[i].empty())
		{
			m_bcreep_complete = false;
			return;
		}
	}
	m_bcreep_complete = true;
}
void CCom_Creep::Release(void)
{
	CObj* pobj = NULL;
	pobj = new CCreep_collapse(m_creepoff_list);
	pobj->Initialize();
	CObjMgr::GetInstance()->AddCorpse(pobj);
}

