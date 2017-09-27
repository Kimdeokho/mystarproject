#pragma once

#include "Include.h"
class CMyMath
{
public:
	static int		   Pos_to_index(const float& x , const float& y, const int& tilesize = 32);
	static int		   Pos_to_index(const D3DXVECTOR2& vpos, const int& tilesize);
	static D3DXVECTOR2 index_to_Pos(const int& idx ,const int& tilecnt , const int& tilesize);

	static float dgree_to_radian(const float& dgree);
	static D3DXVECTOR2 dgree_to_dir(const float& dgree);
	static float radian_to_dgree(const float& rad);
	static float scala_to_dgree(float& scala);
	static float idx_distance(const int& idx1 , const int& idx2 , const int& tilecnt, const int& tilesize);
	static float pos_distance(const D3DXVECTOR2& vstart , const D3DXVECTOR2& vdest);
	static bool	 linecross_check(const MYLINE& line1 , const MYLINE& line2);
public:
	CMyMath(void);
	~CMyMath(void);
};
