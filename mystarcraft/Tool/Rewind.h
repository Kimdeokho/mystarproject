#pragma once

#include "Include.h"


struct STACK_TERRAININFO
{
	BYTE			byTerrain_ID; //어느 지형
	BYTE			byGroup_ID; //지형의 어느그룹
	BYTE			byGroup_sequence; //그룹의 몇번째
	BYTE			bysortLV; //소팅레벨
	int				idx;
};

#define TERRAIN_STACKSIZE 100
#define TILE_INFO list<TERRAIN_INFO>
class CRewind
{
	DECLARE_SINGLETON(CRewind)
private:
	list<int>					m_curidxStack;
	list<list<int>>				m_TileIdxList;
	vector<list<TILE_INFO>>		m_TerrainRewind;
public:
	void	InitStackTile(list<TERRAIN_INFO*> temp ,const int idx);
	void	SetTerrainData(const list<TERRAIN_INFO*>&	templist , const int idx);
	void	Push_Begin(void);
	void	Push_end(void);
	void    StackRewind(void);
public:
	CRewind(void);
	~CRewind(void);
};
