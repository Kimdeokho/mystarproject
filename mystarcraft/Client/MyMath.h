#pragma once

#include "Include.h"
class CMyMath
{
public:
	static int Pos_to_index(const float& x , const float& y);
	static void Bresenham_fog(const D3DXVECTOR2& vPos1 ,const D3DXVECTOR2& vPos2, const int fRadius,list<int>& light_IdxList);
	static void Bresenham_Creep(const D3DXVECTOR2& vStart ,const D3DXVECTOR2& vDest, const int& fRadius ,const int& loopcnt,list<int>& light_IdxList);
public:
	CMyMath(void);
	~CMyMath(void);
};
