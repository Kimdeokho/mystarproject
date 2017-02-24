#pragma once

#include "Include.h"

class CTileManager
{
public:
	DECLARE_SINGLETON(CTileManager)
private:
	vector<TILE*>				m_sqTile;
	vector<list<TERRAIN_INFO*>>	m_terrainInfo_List;
	vector<unsigned short>		m_fogTile;
private:
	LPD3DXSPRITE				m_pSprite;
	D3DCOLOR					m_TileColor;
	LPDIRECT3DTEXTURE9			m_MapTexture[7][6];
	LPDIRECT3DTEXTURE9			m_fogTexture;
	LPDIRECT3DTEXTURE9			m_fogMaskTexture;
private:
	D3DXMATRIX					m_matWorld;
	D3DXVECTOR3					m_vTileCenter;
	int							m_mapsize;
private:
	const vector<TEXINFO*>*	m_DirtTex[GROUP_END];
	const vector<TEXINFO*>*	m_HighDirtTex[GROUP_END];
	const vector<TEXINFO*>*	m_WaterTex[GROUP_END];
	const vector<TEXINFO*>*	m_HillTex[GROUP_END];





	list<D3DXVECTOR2>					m_SightOffList;
	int									m_icuridx;
	D3DXVECTOR2							m_vcurPos;






	LPDIRECT3DSURFACE9 pSurfacefog , pSurfacemask;
	D3DSURFACE_DESC fogDesc , pmaskDesc;
	D3DLOCKED_RECT plockedRect , pmaskrect;
	DWORD* fogimgData;
	DWORD* maskimgData;

public:
	void Initialize(void);
public:
	void FogRender(void);
	void SightOffRender(D3DXVECTOR2 vPos);
	void SightOnRender(D3DXVECTOR2 vPos);
public:
	void ReadyTileTexture(void);
	void ReadyMainMap(void);
public:
	void CopySurface(LPDIRECT3DTEXTURE9 psurface);
public:
	void RenderTile(void);
public:
	void Release(void);
public:
	void LoadTileData(HANDLE hFile);
private:
	CTileManager(void);
public:
	~CTileManager(void);
};
