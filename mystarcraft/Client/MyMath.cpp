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

int CMyMath::Pos_to_index(const float& x , const float& y)
{
	return int(y/SQ_TILESIZEY)*SQ_TILECNTX + int(x/SQ_TILESIZEX);
}

void CMyMath::Bresenham_fog(const D3DXVECTOR2& vStart ,const D3DXVECTOR2& vDest, const int fRadius ,list<int>& light_IdxList)
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
	int   iendidx = 0;

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




			if(X + Y <= fRadius*fRadius )
			{
				idx = CMyMath::Pos_to_index(vStart.x + i * SQ_TILESIZEX * isignX, vStart.y + icnt * isignY);

				if(true == CTileManager::GetInstance()->CheckFogFloor(myidx , idx))
				{
					iendidx = idx;

					if(false == CTileManager::GetInstance()->GetFogLight(idx))
						light_IdxList.push_back(idx);


					if(fRadius*fRadius - 160*160 <= X + Y)
					{
						CTileManager::GetInstance()->SetFogSquence(idx , 1);
						CTileManager::GetInstance()->SetFogLight(idx , X + Y , float(fRadius*fRadius));
					}
					else
					{
						CTileManager::GetInstance()->SetFogSquence(idx , 0);
						CTileManager::GetInstance()->SetFogLight(idx , 0 , 1);
					}

				}
				else
					break;
			}
			else
				break;
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


			if(X + Y <= fRadius*fRadius )
			{
				idx = CMyMath::Pos_to_index(vStart.x + icnt * isignX , vStart.y + i*SQ_TILESIZEY*isignY);

				if(true == CTileManager::GetInstance()->CheckFogFloor(myidx , idx))
				{
					iendidx = idx;

					if(false == CTileManager::GetInstance()->GetFogLight(idx))
						light_IdxList.push_back(idx);

					if(fRadius*fRadius - 160*160 <= X + Y)
					{
						CTileManager::GetInstance()->SetFogSquence(idx , 1);
						CTileManager::GetInstance()->SetFogLight(idx , X + Y , float(fRadius*fRadius));
					}
					else
					{
						CTileManager::GetInstance()->SetFogSquence(idx , 0);
						CTileManager::GetInstance()->SetFogLight(idx , 0 , 1);
					}
				}
				else
					break;
			}
			else
				break;
		}
	}
}
void CMyMath::Bresenham_Creep(const D3DXVECTOR2& vStart ,const D3DXVECTOR2& vDest, const int& fRadius ,const int& loopcnt,list<int>& creep_IdxList)
{
	int iWidth = int(vDest.x - vStart.x);
	int iHeight = int(vDest.y - vStart.y);

	int e = 0;
	int icnt = 0;
	int idx = 0;
	int iAdd = 32;

	int isignX = 1;
	int isignY = 1;

	int myidx = CMyMath::Pos_to_index(vStart.x , vStart.y);

	if(iHeight < 0)
	{
		iHeight = -iHeight;
		isignY = -1;
	}

	if(iWidth < 0)
	{
		iWidth = -iWidth;
		isignX = -1;
	}

	float X = 0;
	float Y = 0;
	int   iendidx = 0;

	if(iWidth >= iHeight)
	{
		e = iWidth;
		for(int i = 0; i < loopcnt; ++i)
		{
			e += iHeight*2;

			if(e >= iWidth*2)
			{
				icnt += iAdd;
				e -= iWidth*2;
			}
		}


		X = (vStart.x + loopcnt * SQ_TILESIZEX * isignX - vStart.x)*(vStart.x + loopcnt * SQ_TILESIZEX * isignX - vStart.x);
		Y = (vStart.y + icnt * isignY - vStart.y)*(vStart.y + icnt * isignY - vStart.y);




		if(X + Y <= fRadius*fRadius + 32*32)
		{
			idx = CMyMath::Pos_to_index(vStart.x + loopcnt * SQ_TILESIZEX * isignX, vStart.y + icnt * isignY);

			if(true == CTileManager::GetInstance()->CheckCreepFloor(myidx , idx) &&
				MOVE_OK == CTileManager::GetInstance()->GetTileOption(idx))
			{
				creep_IdxList.push_back(idx);
				//CTileManager::GetInstance()->SetCreepInstall(idx , true);
			}
		}		
	}
	else
	{
		e = iHeight;
		for(int i = 0; i < loopcnt; ++i)
		{
			e += iWidth*2;

			if( e >= iHeight*2)
			{
				icnt += iAdd;
				e -= iHeight*2;
			}
		}
		X = (vStart.x + icnt * isignX - vStart.x)*(vStart.x + icnt * isignX - vStart.x);
		Y = (vStart.y + loopcnt*SQ_TILESIZEY*isignY - vStart.y)*(vStart.y + loopcnt*SQ_TILESIZEY*isignY - vStart.y);


		if(X + Y <= fRadius*fRadius + 32*32)
		{
			idx = CMyMath::Pos_to_index(vStart.x + icnt * isignX , vStart.y + loopcnt*SQ_TILESIZEY*isignY);

			if(true == CTileManager::GetInstance()->CheckCreepFloor(myidx , idx) &&
				MOVE_OK == CTileManager::GetInstance()->GetTileOption(idx))
			{
				creep_IdxList.push_back(idx);
				//CTileManager::GetInstance()->SetCreepInstall(idx , true);
			}
		};		
	}
}