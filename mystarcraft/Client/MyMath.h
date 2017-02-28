#pragma once

#include "Include.h"
class CMyMath
{
public:
	static int Pos_to_index(const float& x , const float& y);
	static void Bresenham(const D3DXVECTOR2& vPos1 ,const D3DXVECTOR2& vPos2, const int fRadius);
public:
	CMyMath(void);
	~CMyMath(void);
};
