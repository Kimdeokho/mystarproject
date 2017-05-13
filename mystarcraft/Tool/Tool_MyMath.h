#pragma once

#include "Include.h"
class CTool_MyMath
{
public:
	static int		   Pos_to_index(const float& x , const float& y, const int& tilesize = 32);
	static D3DXVECTOR2 index_to_Pos(const int& idx ,const int& tilecnt , const int& tilesize);

	static float dgree_to_radian(const float& dgree);
	static float radian_to_dgree(const float& rad);
	static float scala_to_dgree(const float& scala);
	static int	 idx_distance(const int& idx1 , const int& idx2 , const int& tilecnt);
	static int	 pos_idstance(const D3DXVECTOR2& vstart , const D3DXVECTOR2& vdest);
public:
	CTool_MyMath(void);
	~CTool_MyMath(void);
};
