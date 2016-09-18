#pragma once

template <typename T>
inline void Safe_Delete(T& Temp)
{
	if(Temp)
	{
		delete Temp;
		Temp = NULL;
	}
}

struct DelObject
{
	template <typename T>
	void operator()(T& Temp)
	{
		if(Temp)
		{
			delete Temp;
			Temp = NULL;
		}
	}
};

struct rendersort_compare
{
	bool operator()(const TERRAIN_INFO* lhs , const TERRAIN_INFO* rhs)
	{
		if(lhs->bysortLV < rhs->bysortLV)
			return true;
		else
			return false;
	}
};
inline bool	sort_compare(const TERRAIN_INFO* lhs , const TERRAIN_INFO* rhs)
{
	return lhs->bysortLV < rhs->bysortLV;
};