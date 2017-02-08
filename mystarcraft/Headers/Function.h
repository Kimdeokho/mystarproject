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
class RenderObject
{
public:
	template<typename T>
	void operator()(T& Temp)
	{
		if(NULL != Temp)
			Temp->Render();
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

template<typename T>
bool MyIntersectrect(MYRECT<T>* outrect , const MYRECT<T>* A ,const MYRECT<T>* B)
{
	T minX , maxX;
	T minY , maxY;
	minX = MyMin(A->right , B->right);
	maxX = MyMax(A->left , B->left);
	minY = MyMin(A->bottom , B->bottom );
	maxY = MyMax(A->top , B->top);

	if(maxX > minX &&
		maxY < minY )
	{
		outrect->left = minX;
		outrect->right = maxX;
		outrect->top = maxY;
		outrect->bottom = minY;
		return true;
	}
	return false;
}

template<typename T>
T MyMin(T a , T b)
{
	return a > b ? b : a;
}
template<typename T>
T MyMax(T a , T b)
{
	return a > b ? a : b;
}