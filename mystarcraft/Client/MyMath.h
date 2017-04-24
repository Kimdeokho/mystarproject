#pragma once

#include "Include.h"
class CMyMath
{
public:
	static int Pos_to_index(const float& x , const float& y);
	static int Pos_to_index64(const float& x , const float& y);
	static int Pos_to_index512(const float& x , const float& y);
	static void Bresenham_fog(const D3DXVECTOR2& vPos1 ,const D3DXVECTOR2& vPos2, const int fRadius,list<int>& light_IdxList , bool* fogsearch , UNIT_TYPE etype);
	static void Bresenham_Creep(const D3DXVECTOR2& vStart ,const D3DXVECTOR2& vDest, const int& fRadius ,const int& loopcnt,list<int>& light_IdxList);

	static float dgree_to_radian(const float& dgree);
	static float radian_to_dgree(const float& rad);
	static float scala_to_dgree(const float& scala);
public:
	CMyMath(void);
	~CMyMath(void);
};
