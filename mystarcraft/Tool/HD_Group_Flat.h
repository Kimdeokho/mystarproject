#pragma once
#include "terrain_group.h"

class CHD_Group_Flat :
	public CTerrain_Group
{
private:
	int		m_icol;
	int		m_irow;
public:
	virtual void MakeTerrain_Group(const int istartidx);
public:
	void	SetRowCol(const int irow , int icol);
public:
	CHD_Group_Flat(void);
	virtual ~CHD_Group_Flat(void);
};
