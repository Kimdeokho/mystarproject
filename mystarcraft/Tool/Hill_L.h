#pragma once
#include "terrain_group.h"

class CHill_L :
	public CTerrain_Group
{
public:
	virtual void	MakeTerrain_Group(const int istartidx);
public:
	CHill_L(void);
	~CHill_L(void);
};
