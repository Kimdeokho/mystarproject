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
	return 180*rad/PI;
}
float CMyMath::scala_to_dgree(float& scala)
{
	//acos ������ �ָ� ������ ��´�
	if(scala <= -1)
		scala = -1;

	return (180.f*acos(scala))/PI;
}

float CMyMath::idx_distance(const int& idx1 , const int& idx2 , const int& tilecnt , const int& tilesize)
{
	int iwidth = abs(idx1%tilecnt - idx2%tilecnt);//���ΰŸ�
	int iheght = abs(idx1/tilecnt - idx2/tilecnt);//���ΰŸ�

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
	////ua , ub �Ѵ� 0~1���̶�� ���б����̴�
	//ua = (line2.fx2 - line2.fx1)*(line1.fy1 - line2.fy1) - (line2.fy2 - line2.fy1)*(line1.fx1 - line2.fx1) / den;
	//ub = (line1.fx2 - line1.fx1)*(line1.fy1 - line2.fy1) - (line1.fy2 - line1.fy1)*(line1.fx1 - line2.fx1) / den;

	//if(ua*ub <= 1)
	//	return true;
	//else
	//	return false;
}

D3DXVECTOR2 CMyMath::dgree_to_dir(const float& dgree)
{
	//0���� ��ǥ 1,0�̴� 
	//90���� ��ǥ 0,1�̴�
	return D3DXVECTOR2( cosf(dgree_to_radian(dgree)) , sinf(dgree_to_radian(dgree)));
}
float CMyMath::dir_to_dgree(D3DXVECTOR2 vdir)
{
	D3DXVec2Normalize(&vdir , &vdir);
	float fdot = D3DXVec2Dot(&OFFSET_DIRVEC , &vdir);

	float frad = acosf(fdot);
	float fangle = radian_to_dgree(frad);

	if(vdir.x < 0)
	{
		fangle = 360 - fangle;
		//360 - dgree
	}

	return fangle;
}

void CMyMath::Calculator_eight_idx(int* idxarr , const D3DXVECTOR2& vpos, const int& tilecnt)
{
	idxarr[CENTER]		= CMyMath::Pos_to_index(vpos , 64);

	if( 0 == idxarr[CENTER]%64 || 0 == idxarr[CENTER]/64 )
		idxarr[LEFT_UP]	= -1;
	else
		idxarr[LEFT_UP]	= idxarr[CENTER] - 1 - 64;

	if( 0 == idxarr[CENTER]/64 )
		idxarr[UP]	= -1;
	else
		idxarr[UP]	= idxarr[CENTER] - 64;

	if( 0 == idxarr[CENTER]/64 )
		idxarr[UP]	= -1;
	else
		idxarr[UP]	= idxarr[CENTER] - 64;

	if( 63 == idxarr[CENTER]%64 || 0 == idxarr[CENTER]/64 )
		idxarr[RIGHT_UP]	= -1;
	else
		idxarr[RIGHT_UP]	= idxarr[CENTER] + 1 - 64;

	if( 63 == idxarr[CENTER]%64 )
		idxarr[RIGHT]	= -1;
	else
		idxarr[RIGHT]	= idxarr[CENTER] + 1;

	if( 63 == idxarr[CENTER]%64 || 63 == idxarr[CENTER]/64)
		idxarr[RIGHT_DOWN]	= -1;
	else
		idxarr[RIGHT_DOWN]	= idxarr[CENTER] + 1 + 64;

	if( 63 == idxarr[CENTER]/64)
		idxarr[DOWN]	= -1;
	else
		idxarr[DOWN]	= idxarr[CENTER] + 64;

	if( 0 == idxarr[CENTER]%64 || 63 == idxarr[CENTER]/64)
		idxarr[LEFT_DOWN]	= -1;
	else
		idxarr[LEFT_DOWN]	= idxarr[CENTER] - 1 + 64;

	if( 0 == idxarr[CENTER]%64)
		idxarr[LEFT]	= -1;
	else
		idxarr[LEFT]	= idxarr[CENTER] - 1;
}
