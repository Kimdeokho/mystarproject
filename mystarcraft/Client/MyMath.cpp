#include "StdAfx.h"
#include "MyMath.h"

#include "TileManager.h"

CMyMath::CMyMath(void)
{
}

CMyMath::~CMyMath(void)
{
}

int CMyMath::Pos_to_index(const float& x , const float& y)
{
	return int(y/SQ_TILESIZEY)*SQ_TILECNTX + int(x/SQ_TILESIZEX);
}

void CMyMath::Bresenham(const D3DXVECTOR2& vStart ,const D3DXVECTOR2& vDest, const int fRadius)
{
	int iWidth = int(vDest.x - vStart.x);
	int iHeight = int(vDest.y - vStart.y);

	int e = 0;
	int icnt = 0;
	int idx = 0;
	int iAdd = 32;

	int loopcnt = 0;
	int isignX = 1;
	int isignY = 1;

	int myidx = CMyMath::Pos_to_index(vStart.x , vStart.y);
	if(iHeight < 0)
	{
		iHeight = -iHeight;
		isignY *= -1;
	}

	if(iWidth < 0)
	{
		iWidth = -iWidth;
		isignX *= -1;
	}

	float X = 0;
	float Y = 0;
	if(iWidth >= iHeight)
	{
		loopcnt = iWidth/SQ_TILESIZEX;

		e = iWidth;
		for(int i = 1; i < loopcnt + 1; ++i)
		{
			e += iHeight*2;

			if(e >= iWidth*2)
			{
				icnt += iAdd;
				e -= iWidth*2;
			}

			X = (vStart.x + i * SQ_TILESIZEX * isignX - vStart.x)*(vStart.x + i * SQ_TILESIZEX * isignX - vStart.x);
			Y = (vStart.y + icnt * isignY - vStart.y)*(vStart.y + icnt * isignY - vStart.y);


			if(X + Y <= fRadius*fRadius + 32*32)
			{
				idx = CMyMath::Pos_to_index(vStart.x + i * SQ_TILESIZEX * isignX, vStart.y + icnt * isignY);
				if(true ==CTileManager::GetInstance()->CheckFogFloor(myidx , idx))
					CTileManager::GetInstance()->SetFogSquence(idx , 0);
				else
					break;
			}
		}
	}
	else
	{
		loopcnt = iHeight/SQ_TILESIZEY;

		e = iHeight;
		for(int i = 1; i < loopcnt + 1; ++i)
		{
			e += iWidth*2;

			if( e >= iHeight*2)
			{
				icnt += iAdd;
				e -= iHeight*2;
			}

			X = (vStart.x + icnt * isignX - vStart.x)*(vStart.x + icnt * isignX - vStart.x);
			Y = (vStart.y + i*SQ_TILESIZEY*isignY - vStart.y)*(vStart.y + i*SQ_TILESIZEY*isignY - vStart.y);

			if(X + Y <= fRadius*fRadius + 32*32)
			{
				idx = CMyMath::Pos_to_index(vStart.x + icnt * isignX , vStart.y + i*SQ_TILESIZEY*isignY);
				if(true ==CTileManager::GetInstance()->CheckFogFloor(myidx , idx))
					CTileManager::GetInstance()->SetFogSquence(idx , 0);
				else
					break;
			}
		}
	}
}
