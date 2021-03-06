#include "StdAfx.h"
#include "HD_Group_Flat.h"

CHD_Group_Flat::CHD_Group_Flat(void)
{
	m_icol = 0;
	m_irow = 0;
	m_terrain_id = TERRAIN_HIGHDIRT;
}

CHD_Group_Flat::~CHD_Group_Flat(void)
{
}

void CHD_Group_Flat::MakeTerrain_Group(const int istartidx)
{
	if(istartidx < 0 || istartidx > SQ_TILECNTX*SQ_TILECNTY)
		return;

	m_startidx = istartidx;

	Make_FLAT_Terrain(m_startidx ,m_irow ,m_icol);
}
void CHD_Group_Flat::MakeTerrain_Group(const int istartidx , const int irow , const int icol)
{
	m_startidx = istartidx;
	m_irow = irow;
	m_icol = icol;

	Make_FLAT_Terrain(m_startidx ,m_irow ,m_icol);
}
void CHD_Group_Flat::SetRowCol(const int irow , int icol)
{
	m_irow = irow;
	m_icol = icol;
}
