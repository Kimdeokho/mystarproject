#pragma once
#include "terrain_group.h"

class CHill_R :
	public CTerrain_Group
{
public:
	virtual void	MakeTerrain_Group(const int istartidx);
public:
	CHill_R(void);
	~CHill_R(void);
};
