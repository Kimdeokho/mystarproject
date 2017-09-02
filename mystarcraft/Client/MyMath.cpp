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
float CMyMath::scala_to_dgree(float& scala)
{
	//acos 비율을 주면 라디안을 뱉는다
	if(scala <= -1)
		scala = -1;

	return (180.f*acos(scala))/PI;
}

float CMyMath::idx_distance(const int& idx1 , const int& idx2 , const int& tilecnt , const int& tilesize)
{
	int iwidth = abs(idx1%tilecnt - idx2%tilecnt);//가로거리
	int iheght = abs(idx1/tilecnt - idx2/tilecnt);//세로거리

	return float((iwidth + iheght)*tilesize);
}
float CMyMath::pos_distance(const D3DXVECTOR2& vstart , const D3DXVECTOR2& vdest)
{
	return ((vdest.x - vstart.x) * (vdest.x - vstart.x) + (vdest.y - vstart.y) * (vdest.y - vstart.y));
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
bool CMyMath::linecross_check(const MYLINE& line1 , const MYLINE& line2)
{
	float a = (line1.fy2 - line1.fy1)*(line2.fx1 - line1.fx1) - (line2.fy1 - line1.fy1)*(line1.fx2 - line1.fx1);
	float b = (line1.fy2 - line1.fy1)*(line2.fx2 - line1.fx1) - (line2.fy2 - line1.fy1)*(line1.fx2 - line1.fx1);

	if(a*b < 0)
	{
		a = (line2.fy2 - line2.fy1)*(line1.fx1 - line2.fx1) - (line1.fy1 - line2.fy1)*(line2.fx2 - line2.fx1);
		b = (line2.fy2 - line2.fy1)*(line1.fx2 - line2.fx1) - (line1.fy2 - line2.fy1)*(line2.fx2 - line2.fx1);
		if(a*b < 0)
			return true;
		else
			return false;
	}
	return false;

	//float den, ua, ub;

	//den = (line2.fy2 -  line2.fy1)*(line1.fx2 - line1.fx1) - (line2.fx2 - line2.fx1)*(line1.fy2 - line1.fy1);
	////ua , ub 둘다 0~1사이라면 선분교차이다
	//ua = (line2.fx2 - line2.fx1)*(line1.fy1 - line2.fy1) - (line2.fy2 - line2.fy1)*(line1.fx1 - line2.fx1) / den;
	//ub = (line1.fx2 - line1.fx1)*(line1.fy1 - line2.fy1) - (line1.fy2 - line1.fy1)*(line1.fx1 - line2.fx1) / den;

	//if(ua*ub <= 1)
	//	return true;
	//else
	//	return false;
}