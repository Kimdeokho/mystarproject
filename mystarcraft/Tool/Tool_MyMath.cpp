#include "StdAfx.h"
#include "Tool_MyMath.h"


CTool_MyMath::CTool_MyMath(void)
{
}

CTool_MyMath::~CTool_MyMath(void)
{
}

int CTool_MyMath::Pos_to_index(const float& x , const float& y , const int& tilesize)
{
	return int(y/tilesize)*(4096/tilesize) + int(x/tilesize);
}
float  CTool_MyMath::dgree_to_radian(const float& dgree)
{
	return dgree*PI/180;
}
float CTool_MyMath::radian_to_dgree(const float& rad)
{
	return (180*rad)/PI;
}
float CTool_MyMath::scala_to_dgree(const float& scala)
{
	return (180*acos(scala))/PI;
}

int CTool_MyMath::idx_distance(const int& idx1 , const int& idx2 , const int& tilecnt)
{
	int iwidth = abs(idx1%tilecnt - idx2%tilecnt);//가로거리
	int iheght = abs(idx1/tilecnt - idx2/tilecnt);//세로거리

	return (iwidth + iheght)*32;
}
int CTool_MyMath::pos_idstance(const D3DXVECTOR2& vstart , const D3DXVECTOR2& vdest)
{
	return int((vdest.x - vstart.x) * (vdest.x - vstart.x) + (vdest.y - vstart.y) * (vdest.y - vstart.y));
}

D3DXVECTOR2 CTool_MyMath::index_to_Pos(const int& idx ,const int& tilecnt , const int& tilesize)
{
	/*X
	0 16
	1 48
	128 0
	129 48*/

	D3DXVECTOR2 vpos;
	vpos.x = float(tilesize/2 + (idx % tilecnt)*tilesize);
	vpos.y = float(tilesize/2 + (idx / tilecnt)*tilesize);

	return vpos;
}
