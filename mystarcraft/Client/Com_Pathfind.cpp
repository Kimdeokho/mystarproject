#include "StdAfx.h"
#include "Com_Pathfind.h"

#include "Obj.h"
#include "Astar.h"
#include "TileManager.h"
#include "MyMath.h"

Com_Pathfind::Com_Pathfind(void)
{
}

Com_Pathfind::~Com_Pathfind(void)
{
	Release();
}
void Com_Pathfind::Initialize(CObj* pobj)
{
	m_pobj = pobj;

	m_Astar = new CAstar;
	m_Astar->Initialize(m_pobj);

	m_terrainpath.reserve(256);
}

void Com_Pathfind::Update(void)
{

}
void Com_Pathfind::StartPathfinding(void)
{
	if(!m_terrainpath.empty())
		m_terrainpath.clear();

	int startidx = CMyMath::Pos_to_index(m_pobj->GetPos().x , m_pobj->GetPos().y , 32);

	CTileManager::GetInstance()->GetFlowfield_Path(startidx , m_terrainpath);

}
void Com_Pathfind::Release(void)
{
	Safe_Delete(m_Astar);
}

