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
bool MyPtInrect(const D3DXVECTOR2& vpos ,const MYRECT<T>* rect)
{
	if(vpos.x > rect->left &&
		vpos.x < rect->right &&
		vpos.y < rect->bottom &&
		vpos.y > rect->top)
		return true;

	return false;
}

template<typename T>
bool MyIntersectrect(MYRECT<T>* outrect , const MYRECT<T>* A ,const MYRECT<T>* B)
{
	static T maxX , minX;
	static T maxY , minY;

	maxX = A->left < B->left ? B->left : A->left;
	minX = A->right < B->right ? A->right : B->right;
	minY = A->bottom < B->bottom ? A->bottom : B->bottom;
	maxY = A->top < B->top ? B->top : A->top;

	if(maxX < minX && minY > maxY)
	{
		outrect->left = maxX;
		outrect->right = minX;
		outrect->top = maxY;
		outrect->bottom = minY;
		return true;
	}
	return false;
}
template<typename T>
bool MyIntersectrect( const MYRECT<T>* A ,const MYRECT<T>* B)
{
	static T maxX , minX;
	static T maxY , minY;

	maxX = A->left < B->left ? B->left : A->left;
	minX = A->right < B->right ? A->right : B->right;
	minY = A->bottom < B->bottom ? A->bottom : B->bottom;
	maxY = A->top < B->top ? B->top : A->top;

	if(maxX < minX && minY > maxY)
	{
		return true;
	}
	return false;
}
template<typename T>
T MyMin(const T& a ,const T& b)
{
	return a > b ? b : a;
}
template<typename T>
T MyMax(const T& a ,const T& b)
{
	return a > b ? a : b;
}