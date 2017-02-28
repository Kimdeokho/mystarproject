#include "StdAfx.h"
#include "TileManager.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "Device.h"
#include "TimeMgr.h"
#include "MouseMgr.h"
#include "MyMath.h"
#include "FontMgr.h"

IMPLEMENT_SINGLETON(CTileManager)

CTileManager::CTileManager(void)
{
	for(int i = 0; i < 7; ++i)
		for(int j = 0; j < 6; ++j)
			m_MapTexture[i][j] = NULL;

	m_fogTexture = NULL;
	m_fogMaskTexture = NULL;
}

CTileManager::~CTileManager(void)
{
	Release();
}
void CTileManager::Initialize(void)
{
	m_pSprite = CDevice::GetInstance()->GetSprite();
	D3DXMatrixIdentity(&m_matWorld);

	LPDIRECT3DDEVICE9 pdevice = CDevice::GetInstance()->GetDevice();

	for(int i = 0; i < 7; ++i)
	{
		for(int j = 0; j < 6; ++j)
		{
			D3DXCreateTexture(pdevice , 800, 600, D3DX_DEFAULT, D3DUSAGE_RENDERTARGET
				,D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_MapTexture[i][j]);
		}
	}

	//m_fogTile.reserve(SQ_TILECNTY*SQ_TILECNTX);
	FOG_INFO*	pfoginfo_temp = NULL;
	for(int i = 0; i < SQ_TILECNTY*SQ_TILECNTX; ++i)
	{
			pfoginfo_temp = new FOG_INFO();
			m_fogTile[i] = pfoginfo_temp;
			//m_fogTile.push_back(pfoginfo_temp);
	}

	m_fogTexture = CTextureMgr::GetInstance()->GetSingleTexture(L"Fog" , L"blackfog4096")->pTexture;
	m_fogMaskTexture = CTextureMgr::GetInstance()->GetSingleTexture(L"Fog" , L"fogmask640")->pTexture;

	m_vTileCenter = D3DXVECTOR3(16.f,16.f,0.f);
	m_TileColor = D3DCOLOR_ARGB(255,255,255,255);	

	ReadyTileTexture();
}

void CTileManager::ReadyTileTexture(void)
{
	m_DirtTex[GROUP_FLAT] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"Dirt" , L"FLAT");


	m_HighDirtTex[GROUP_FLAT] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"High_dirt" , L"FLAT");
	m_HighDirtTex[GROUP_L] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"High_dirt" , L"L");
	m_HighDirtTex[GROUP_LU] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"High_dirt" , L"LU");
	m_HighDirtTex[GROUP_RU] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"High_dirt" , L"RU");
	m_HighDirtTex[GROUP_R] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"High_dirt" , L"R");
	m_HighDirtTex[GROUP_RD] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"High_dirt" , L"RD");
	m_HighDirtTex[GROUP_LD] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"High_dirt" , L"LD");

	m_WaterTex[GROUP_FLAT] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"Water" , L"FLAT");
	m_WaterTex[GROUP_L] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"Water" , L"L");
	m_WaterTex[GROUP_LU] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"Water" , L"LU");
	m_WaterTex[GROUP_RU] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"Water" , L"RU");
	m_WaterTex[GROUP_R] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"Water" , L"R");
	m_WaterTex[GROUP_RD] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"Water" , L"RD");
	m_WaterTex[GROUP_LD] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"Water" , L"LD");

	m_HillTex[GROUP_L] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"HillL" , L"L");
	m_HillTex[GROUP_R] = CTextureMgr::GetInstance()->GetTileTexture_vecset(L"HillR" , L"R");
}

void CTileManager::RenderTile(void)
{
	float fspeed = 1500.f;
	if(GetAsyncKeyState(VK_LEFT))
	{
		CScrollMgr::m_fScrollX -= GETTIME*fspeed;
	}
	if(GetAsyncKeyState(VK_RIGHT))
	{
		CScrollMgr::m_fScrollX += GETTIME*fspeed;
	}
	if(GetAsyncKeyState(VK_UP))
	{
		CScrollMgr::m_fScrollY -= GETTIME*fspeed;
	}
	if(GetAsyncKeyState(VK_DOWN))
	{
		CScrollMgr::m_fScrollY += GETTIME*fspeed;
	}

	if(0 > CScrollMgr::m_fScrollX)
		CScrollMgr::m_fScrollX = 0;
	else if(3296 <= CScrollMgr::m_fScrollX)
		CScrollMgr::m_fScrollX = 3296;

	if(0 > CScrollMgr::m_fScrollY)
		CScrollMgr::m_fScrollY = 0;
	else if(3496 <= CScrollMgr::m_fScrollY)
		CScrollMgr::m_fScrollY = 3496;

	m_matWorld._41 = 0;
	m_matWorld._42 = 0;
	m_matWorld._11 = 1;
	m_matWorld._22 = 1;


	int iX = (int)CScrollMgr::m_fScrollX / BACKBUFFER_SIZEX;
	int iY = (int)CScrollMgr::m_fScrollY / BACKBUFFER_SIZEY;
	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			m_matWorld._41 = BACKBUFFER_SIZEX*(j+iX) - CScrollMgr::m_fScrollX;
			m_matWorld._42 = BACKBUFFER_SIZEY*(i+iY) - CScrollMgr::m_fScrollY;
			m_pSprite->SetTransform(&m_matWorld);
			m_pSprite->Draw(m_MapTexture[i+iY][j+iX] , NULL , &D3DXVECTOR3(0,0,0), NULL
				, D3DCOLOR_ARGB(255,255,255,255));
		}
	}

	




	FogRender();


	//const vector<TEXINFO*>*	ptemp = CTextureMgr::GetInstance()->GetGeneralTexture(L"Fog");
	TEXINFO**	ptemp = CTextureMgr::GetInstance()->GetGeneralTexture(L"Fog");
	int ifogsquence = 1;

	int istartX = (int)CScrollMgr::m_fScrollX/SQ_TILESIZEX;
	int istartY = (int)CScrollMgr::m_fScrollY/SQ_TILESIZEY;
	int idx = 0;
	unsigned short fog_option = 0;

	for(int i = 0; i < 20; ++i)
	{
		for(int j = 0; j < 26; ++j)
		{
			idx = (istartY+i) * SQ_TILECNTX + (istartX + j);
			if(idx < 0 || idx>= SQ_TILECNTY*SQ_TILECNTX)
				continue;

			m_matWorld._41 = float((istartX + j)*SQ_TILESIZEX) - CScrollMgr::m_fScrollX;
			m_matWorld._42 = float((istartY + i)*SQ_TILESIZEY) - CScrollMgr::m_fScrollY;

			fog_option = m_fogTile[idx]->fog_sequence;

			if(0 == fog_option)
				ifogsquence = 0; //ARGB(255,0,0,0)
			else if(1 == fog_option)
				ifogsquence = 1; //°ËÀº»ö

			m_pSprite->SetTransform(&m_matWorld);
			m_pSprite->Draw(ptemp[ifogsquence]->pTexture , NULL , &D3DXVECTOR3(0,0,0), NULL
				,m_fogTile[idx]->fog_color);
		}
	}


	//D3DXVECTOR2	vPoint[2];

	//int scrollX = (int)CScrollMgr::m_fScrollX;
	//int scrollY = (int)CScrollMgr::m_fScrollY;

	//CDevice::GetInstance()->Render_End();
	//CDevice::GetInstance()->Render_Begin();

	//for(int j = 0; j < 40; ++j)
	//{
	//	int index = j + scrollX/SQ_TILESIZEX;
	//	vPoint[0] = D3DXVECTOR2( float(index*SQ_TILESIZEX) - scrollX,0 - (float)scrollY);
	//	vPoint[1] = D3DXVECTOR2( float(index*SQ_TILESIZEX) - scrollX, float(SQ_TILECNTY*SQ_TILESIZEY) - scrollY);

	//	CDevice::GetInstance()->GetLine()->SetWidth(1.0f);
	//	CDevice::GetInstance()->GetLine()->Draw(vPoint , 2 , D3DCOLOR_ARGB(255,0,255,0));
	//}
	//for(int j = 0; j < 32; ++j)
	//{
	//	int index = j + scrollY/SQ_TILESIZEY;
	//	vPoint[0] = D3DXVECTOR2(0 - (float)scrollX ,float(index*SQ_TILESIZEX) - scrollY );
	//	vPoint[1] = D3DXVECTOR2(float (SQ_TILECNTX*SQ_TILESIZEX) - scrollX, float(index*SQ_TILESIZEX) - scrollY);

	//	CDevice::GetInstance()->GetLine()->SetWidth(1.0f);
	//	CDevice::GetInstance()->GetLine()->Draw(vPoint , 2 , D3DCOLOR_ARGB(255,0,255,0));
	//}

	//CDevice::GetInstance()->Render_End();
	//CDevice::GetInstance()->Render_Begin();
}
void CTileManager::FogRender(void)
{
	static int oldIdx = -1;

	m_vcurPos.x = CMouseMgr::GetMousePt().x /*À¯´ÖÀÇÁÂÇ¥*/ + CScrollMgr::m_fScrollX;
	m_vcurPos.y = CMouseMgr::GetMousePt().y + CScrollMgr::m_fScrollY;



	m_icuridx = CMyMath::Pos_to_index(m_vcurPos.x , m_vcurPos.y); 

	if(m_icuridx != oldIdx)
		oldIdx = m_icuridx;
	else
		return;


	SightOnRender(m_vcurPos , 15);


}
void CTileManager::SightOffRender(D3DXVECTOR2 vPos)
{
}
void CTileManager::SightOnRender(const D3DXVECTOR2& vPos , int irange, int imyfloor)
{
	//range´Â °¡±ÞÀû È¦¼ö

	int fradius = irange/2*32;
	int istartX = int( vPos.x - fradius )/SQ_TILESIZEX;
	int istartY = int( vPos.y - fradius )/SQ_TILESIZEY;

	int startIdx = CMyMath::Pos_to_index(vPos.x , vPos.y);	
	int destidx = 0;

	CTileManager::GetInstance()->SetFogSquence(startIdx , 0);

	if(istartY < 0)
		istartY = 0;
	if(istartY + irange - 1 >= SQ_TILECNTY)
		istartY = SQ_TILECNTY - irange;

	if(istartX < 0)
		istartX = 0;
	if(istartX + irange - 1 >= SQ_TILECNTX)
		istartX = SQ_TILECNTX - irange;
	for(int j = 0; j < irange; ++j) //À­ÁÙ 
	{
		destidx = istartY*SQ_TILECNTX + (istartX + j);

		if(istartX + j >= SQ_TILECNTX || istartX + j < 0)
			continue;
		if(destidx < 0 || destidx>= SQ_TILECNTY*SQ_TILECNTX || 
			startIdx < 0 || startIdx >= SQ_TILECNTY*SQ_TILECNTX)
			continue;

		CMyMath::Bresenham(m_sqTile[startIdx]->vPos , m_sqTile[destidx]->vPos ,fradius);
	}

	for(int j = 0; j < irange; ++j) //¾Æ·§ÁÙ 
	{
		destidx = (istartY + irange - 1)*SQ_TILECNTX + (istartX + j);

		if(istartX + j >= SQ_TILECNTX || istartX + j< 0)
			continue;
		if(destidx < 0 || destidx>= SQ_TILECNTY*SQ_TILECNTX || 
			startIdx < 0 || startIdx >= SQ_TILECNTY*SQ_TILECNTX)
			continue;

		CMyMath::Bresenham(m_sqTile[startIdx]->vPos , m_sqTile[destidx]->vPos ,fradius);
	}

	for(int i = 1; i < irange - 1; ++i) //¿ÞÂÊ ¿­
	{
		destidx = (istartY + i)*SQ_TILECNTX + istartX;

		if(istartX < 0 || istartX >= SQ_TILECNTX)
			continue;
		if(destidx < 0 || destidx>= SQ_TILECNTY*SQ_TILECNTX || 
			startIdx < 0 || startIdx >= SQ_TILECNTY*SQ_TILECNTX)
			continue;

		CMyMath::Bresenham(m_sqTile[startIdx]->vPos , m_sqTile[destidx]->vPos ,fradius);
	}

	for(int i = 1; i < irange - 1; ++i) //¿À¸¥ÂÊ ¿­
	{
		destidx = (istartY + i)*SQ_TILECNTX + (istartX + irange - 1);

		if(istartX + irange - 1 < 0 || istartX + irange - 1 >= SQ_TILECNTX)
			continue;
		if(destidx < 0 || destidx >= SQ_TILECNTY*SQ_TILECNTX || 
			startIdx < 0 || startIdx >= SQ_TILECNTY*SQ_TILECNTX)
			continue;

		CMyMath::Bresenham(m_sqTile[startIdx]->vPos , m_sqTile[destidx]->vPos ,fradius);
	}
}
void CTileManager::SetFogSquence(int idx , unsigned short sequence)
{
	if(idx < 0 || idx >= SQ_TILECNTX*SQ_TILECNTY)
		return;

	m_fogTile[idx]->fog_sequence = sequence;
	m_fogTile[idx]->fog_color = D3DCOLOR_ARGB(0,50,50,50);
}
bool CTileManager::CheckFogFloor(int myidx , int destidx)
{
	if(destidx < 0 || destidx >= SQ_TILECNTX*SQ_TILECNTY)
		return false;
	if(myidx < 0 || myidx >= SQ_TILECNTX*SQ_TILECNTY)
		return false;


	if( 1 == m_sqTile[myidx]->byFloor)
	{
		if(2 == m_sqTile[destidx]->byFloor)
			return false;
	}
	return true;
}
void CTileManager::CopySurface(LPDIRECT3DTEXTURE9 ptexturemap)
{
	LPDIRECT3DSURFACE9 pBackBuffer , pSurface;
	LPDIRECT3DDEVICE9 pdevice = CDevice::GetInstance()->GetDevice();

	pdevice->GetRenderTarget(0 , &pBackBuffer);

	ptexturemap->GetSurfaceLevel(0, &pSurface);

	pdevice->StretchRect(pBackBuffer , NULL, pSurface ,NULL , D3DTEXF_NONE);

	pBackBuffer->Release();
}

void CTileManager::LoadTileData(HANDLE hFile)
{
	DWORD dwbyte;
	int mapsize = 0;
	int sortsize = -1;

	ReadFile(hFile , &mapsize, sizeof(int), &dwbyte, NULL);
	m_mapsize = mapsize;

	m_terrainInfo_List.reserve(mapsize);

	TILE* tempTile = NULL;
	TERRAIN_INFO* tempterrain = NULL;

	for(int i = 0; i < mapsize; ++i)
	{
		tempTile = new TILE();
		ReadFile(hFile, tempTile, sizeof(TILE), &dwbyte, NULL);

		m_sqTile[i] = tempTile;
	}

	for(int i = 0; i < mapsize; ++i)
	{
		ReadFile(hFile, &sortsize , sizeof(int), &dwbyte, NULL);

		m_terrainInfo_List.push_back(list<TERRAIN_INFO*>());
		for(int j = 0; j < sortsize; ++j)
		{
			tempterrain = new TERRAIN_INFO;
			ReadFile(hFile, tempterrain, sizeof(TERRAIN_INFO), &dwbyte, NULL);
			m_terrainInfo_List[i].push_back(tempterrain);
		}
	}

	ReadyMainMap();
}
void CTileManager::ReadyMainMap(void)
{

	list<TERRAIN_INFO*>::iterator iter;
	list<TERRAIN_INFO*>::iterator iter_end;

	LPDIRECT3DTEXTURE9 pTexture;
	BYTE byTerrain_ID = 0;
	BYTE byGroup_ID = 0;
	BYTE byGroup_sequence = 0;

	int idx = 0;
	int istartidx = 0;
	int rowidx = 0;
	int colidx = 0;



	for(int Y = 0; Y < 7; ++Y)
	{
		for(int X = 0; X < 6; ++X)
		{
			istartidx = ((Y*BACKBUFFER_SIZEY)/SQ_TILESIZEY)*SQ_TILECNTX + (X*BACKBUFFER_SIZEX)/SQ_TILESIZEX;

			for(int i = 0; i < 20; ++i)
			{		

				for(int j = 0; j < 26; ++j)
				{
					CDevice::GetInstance()->Render_Begin();
					idx = istartidx + i*SQ_TILECNTX + j;

					if(idx < 0 || idx >= SQ_TILECNTX*SQ_TILECNTY)
						continue;

					m_matWorld._41 = m_sqTile[idx]->vPos.x - BACKBUFFER_SIZEX*X;
					m_matWorld._42 = m_sqTile[idx]->vPos.y - BACKBUFFER_SIZEY*Y;


					m_pSprite->SetTransform(&m_matWorld);

					iter = m_terrainInfo_List[idx].begin();
					iter_end = m_terrainInfo_List[idx].end();
					for( ; iter != iter_end; ++iter)
					{
						byTerrain_ID = (*iter)->byTerrain_ID;
						byGroup_ID = (*iter)->byGroup_ID;
						byGroup_sequence = (*iter)->byGroup_sequence;

						if(TERRAIN_DIRT == byTerrain_ID)
							pTexture = (*m_DirtTex[byGroup_ID])[byGroup_sequence]->pTexture;
						else if(TERRAIN_HIGHDIRT == byTerrain_ID)
							pTexture = (*m_HighDirtTex[byGroup_ID])[byGroup_sequence]->pTexture;
						else if(TERRAIN_WATER == byTerrain_ID)
							pTexture = (*m_WaterTex[byGroup_ID])[byGroup_sequence]->pTexture;
						else if(TERRAIN_HILL_R == byTerrain_ID)
							pTexture = (*m_HillTex[byGroup_ID])[byGroup_sequence]->pTexture;
						else if(TERRAIN_HILL_L == byTerrain_ID)
							pTexture = (*m_HillTex[byGroup_ID])[byGroup_sequence]->pTexture;

						m_pSprite->Draw(pTexture , NULL , &m_vTileCenter , NULL ,
							m_TileColor);
					}
					CDevice::GetInstance()->Render_End();
					CopySurface(m_MapTexture[Y][X]);

				}
			}
		}
	}
}
void CTileManager::Release(void)
{
	for(int i = 0; i < SQ_TILECNTY*SQ_TILECNTX; ++i)
	{
		list<TERRAIN_INFO*>::iterator iter = m_terrainInfo_List[i].begin();
		list<TERRAIN_INFO*>::iterator iter_end = m_terrainInfo_List[i].end();
		for( ; iter != iter_end; ++iter)
			Safe_Delete((*iter));

		m_terrainInfo_List[i].clear();
		Safe_Delete(m_sqTile[i]);
	}
	//m_sqTile.clear();
	m_terrainInfo_List.clear();

	for(int i = 0; i < SQ_TILECNTY*SQ_TILECNTX; ++i)
	{
		Safe_Delete(m_fogTile[i]);
	}
	//m_fogTile.clear();

	//vector<TILE*>().swap(m_sqTile);
	vector<list<TERRAIN_INFO*>>().swap(m_terrainInfo_List);
}
