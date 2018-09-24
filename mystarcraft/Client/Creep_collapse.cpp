#include "StdAfx.h"
#include "Creep_collapse.h"

#include "TimeMgr.h"
#include "TileManager.h"
CCreep_collapse::CCreep_collapse(const list<int>& creeplist)
{
	m_creepoff_list.assign(creeplist.begin() , creeplist.end());
}

CCreep_collapse::~CCreep_collapse(void)
{
}

void CCreep_collapse::Initialize(void)
{
	m_creepoff_timer = 0.f;
}

void CCreep_collapse::Update(void)
{
	if(m_creepoff_list.empty())
		return;

	//저그 빌딩 시체에서 업데이트하자

	if(m_creepoff_list.empty())
		return;

	m_creepoff_timer += GETTIME;

	int idx = 0;
	if(m_creepoff_timer >= 0.3)
	{
		m_creepoff_timer = 0.f;

		idx = m_creepoff_list.back();
		m_creepoff_list.pop_back();

		CTileManager::GetInstance()->SetCreepInstall(idx , false);
		CTileManager::GetInstance()->Creep_decrease_autotile(idx);
	}
}

void CCreep_collapse::Render(void)
{

}

void CCreep_collapse::Release(void)
{

}
