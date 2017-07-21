#include "StdAfx.h"
#include "MyMath.h"

#include "TileManager.h"
#include "TimeMgr.h"

CMyMath::CMyMath(void)
{
}

CMyMath::~CMyMath(void)
{
}

int CMyMath::Pos_to_index(const float& x , const float& y , const int& tilesize)
{
	return int(y/tilesize)*(4096/tilesize) + int(x/tilesize);
}
int CMyMath::Pos_to_index(const D3DXVECTOR2& vpos, const int& tilesize)
{
	return int(vpos.y/tilesize)*(4096/tilesize) + int(vpos.x/tilesize);
}
float  CMyMath::dgree_to_radian(const float& dgree)
{
	return dgree*PI/180;
}
float CMyMath::radian_to_dgree(const float& rad)
{
	return (180*rad)/PI;
}
float CMyMath::scala_to_dgree(const float& scala)
{
	return (180*acos(scala))/PI;
}

int CMyMath::idx_distance(const int& idx1 , const int& idx2 , const int& tilecnt , const int& tilesize)
{
	int iwidth = abs(idx1%tilecnt - idx2%tilecnt);//가로거리
	int iheght = abs(idx1/tilecnt - idx2/tilecnt);//세로거리

	return (iwidth + iheght)*tilesize;
}
int CMyMath::pos_distance(const D3DXVECTOR2& vstart , const D3DXVECTOR2& vdest)
{
	return int((vdest.x - vstart.x) * (vdest.x - vstart.x) + (vdest.y - vstart.y) * (vdest.y - vstart.y));
}

D3DXVECTOR2 CMyMath::index_to_Pos(const int& idx ,const int& tilecnt , const int& tilesize)
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
