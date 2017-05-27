#pragma once
#include "component.h"

class CAstar;
class Com_Pathfind :
	public CComponent
{
private:
	CAstar*				m_Astar;
	vector<int>			m_terrainpath;
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Release(void);
public:
	void StartPathfinding(void);
public:
	Com_Pathfind(void);
	~Com_Pathfind(void);
};
