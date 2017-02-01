#include "StdAfx.h"
#include "TileManager.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "Device.h"
#include "TimeMgr.h"

IMPLEMENT_SINGLETON(CTileManager)

CTileManager::CTileManager(void)
{
	for(int i = 0; i < 7; ++i)
		for(int j = 0; j < 6; ++j)
			m_MapTexture[i][j] = NULL;
}

CTileManager::~CTileManager(void)
{
	Release();
}
void CTileManager::Initialize(void)
{
	D3DXMatrixIdentity(&m_matWorld);

	LPDIRECT3DDEVICE9 pdevice = CDevice::GetInstance()->GetDevice();

	for(int i = 0; i < 7; ++i)
		for(int j = 0; j < 6; ++j)
		{
			D3DXCreateTexture(pdevice , 800, 600, D3DX_DEFAULT, D3DUSAGE_RENDERTARGET
				,D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_MapTexture[i][j]);
		}


	m_vTileCenter = D3DXVECTOR3(16.f,16.f,0.f);
	m_TileColor = D3DCOLOR_ARGB(255,255,255,255);	

	ReadyTileTexture();
	m_pSprite = CDevice::GetInstance()->GetSprite();
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
	float fspeed = 500.f;
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

	D3DXMatrixIdentity(&m_matWorld);


	int iX = (int)CScrollMgr::m_fScrollX / BACKBUFFER_SIZEX;
	int iY = (int)CScrollMgr::m_fScrollY / BACKBUFFER_SIZEY;
	for(int i = 0; i < 2; ++i)
		for(int j = 0; j < 2; ++j)
		{
			m_matWorld._41 = BACKBUFFER_SIZEX*(j+iX) - CScrollMgr::m_fScrollX;
			m_matWorld._42 = BACKBUFFER_SIZEY*(i+iY) - CScrollMgr::m_fScrollY;
			m_pSprite->SetTransform(&m_matWorld);

			m_pSprite->Draw(m_MapTexture[i+iY][j+iX] , NULL , &D3DXVECTOR3(0,0,0), NULL
				, D3DCOLOR_ARGB(255,255,255,255));
		}
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
	
	
	//CopySurface(m_MapTexture[rowidx][colidx]);
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

	m_sqTile.reserve(mapsize);
	m_terrainInfo_List.reserve(mapsize);

	TILE* tempTile = NULL;
	TERRAIN_INFO* tempterrain = NULL;

	for(int i = 0; i < mapsize; ++i)
	{
		tempTile = new TILE();
		ReadFile(hFile, tempTile, sizeof(TILE), &dwbyte, NULL);

		m_sqTile.push_back(tempTile);
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

void CTileManager::Release(void)
{
	for(size_t i = 0; i < m_sqTile.size(); ++i)
	{
		list<TERRAIN_INFO*>::iterator iter = m_terrainInfo_List[i].begin();
		list<TERRAIN_INFO*>::iterator iter_end = m_terrainInfo_List[i].end();
		for( ; iter != iter_end; ++iter)
			Safe_Delete((*iter));

		m_terrainInfo_List[i].clear();
		Safe_Delete(m_sqTile[i]);
	}
	m_sqTile.clear();
	m_terrainInfo_List.clear();

	vector<TILE*>().swap(m_sqTile);
	vector<list<TERRAIN_INFO*>>().swap(m_terrainInfo_List);
}