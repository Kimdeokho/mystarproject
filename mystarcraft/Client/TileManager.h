#pragma once

#include "Include.h"

class CTileManager
{
public:
	DECLARE_SINGLETON(CTileManager)
private:
	TILE*						m_sqTile[SQ_TILECNTY*SQ_TILECNTX];
	vector<list<TERRAIN_INFO*>>	m_terrainInfo_List;
	FOG_INFO*					m_fogTile[SQ_TILECNTY*SQ_TILECNTX];
	CREEP_INFO*					m_creepTile[SQ_TILECNTY*SQ_TILECNTX];

	//실험용 (유닛마다 갖고있을 컨테이너)
	//list<int>					m_LightOff_List;
	list<int>					m_CreepOff_List;

private:
	LPD3DXSPRITE				m_pSprite;
	D3DCOLOR					m_TileColor;
	LPDIRECT3DTEXTURE9			m_MapTexture[7][6];
	//LPDIRECT3DTEXTURE9			m_fogTexture;
	//LPDIRECT3DTEXTURE9			m_fogMaskTexture;
private:
	D3DXMATRIX					m_matWorld;
	D3DXVECTOR3					m_vTileCenter;
	int							m_mapsize;
private:
	const vector<TEXINFO*>*	m_DirtTex[GROUP_END];
	const vector<TEXINFO*>*	m_HighDirtTex[GROUP_END];
	const vector<TEXINFO*>*	m_WaterTex[GROUP_END];
	const vector<TEXINFO*>*	m_HillTex[GROUP_END];




	int									m_icuridx;
	D3DXVECTOR2							m_vcurPos;






	LPDIRECT3DSURFACE9 pSurfacefog , pSurfacemask;
	D3DSURFACE_DESC fogDesc , pmaskDesc;
	D3DLOCKED_RECT plockedRect , pmaskrect;
	DWORD* fogimgData;
	DWORD* maskimgData;


	float m_fTimeTest;
public:
	void Initialize(void);
public:
	bool	GetFogLight(const int& idx);
	BYTE	GetTileOption(const int& idx);
	bool	GetCreepInstall(const int& idx);
	bool	GetFogSearch(const int& idx);
	TILE**	GetSqTile(void);
	CREEP_INFO**	GetCreepTile(void);
public:
	void SetFogSquence(int idx , unsigned short sequence);
	void SetFogLight(int idx, float fdistance , float fradius);
	void SetFogColor(const int& idx , D3DCOLOR color);
	void SetCreepInstall(const int& idx , bool binstall);

	void SetFogoverlap_cnt(const int& idx  , const int& cnt);
	void SetFogSearch(const int& idx,bool bsearch);
public:
	bool CheckFogFloor(int myidx , int destidx);
	bool CheckCreepFloor(int myidx , int destidx);
public:
	//void CreepAlgorithm(void);
public:	
	void SightOffRender(const int& idx);
	void SightOnRender(const int& idx ,const int& irange , list<int>& sightoff_list , bool* fogsearch , OBJ_TYPE etype);

	//void Creep_increase(const D3DXVECTOR2& vPos/*유닛의 위치*/ , int irange);
	void Creep_decrease_autotile(const int& idx);
	void Creep_Autotile(const int& idx);
public:
	void Bresenham_fog(const D3DXVECTOR2& vStart ,const D3DXVECTOR2& vDest, const int fRadius ,list<int>& light_IdxList , bool* fogsearch , OBJ_TYPE etype);
	void Bresenham_Creep(const D3DXVECTOR2& vStart ,const D3DXVECTOR2& vDest, const int& fRadius ,const int& loopcnt,list<int>& creep_IdxList);
public:
	void ReadyTileTexture(void);
	void ReadyMainMap(void);
public:
	void CopySurface(LPDIRECT3DTEXTURE9 psurface);
public:
	void RenderTile(void);
	void RenderFog(void);
	void RenderCreep(void);
public:
	void Release(void);
public:
	void LoadTileData(HANDLE hFile);
private:
	CTileManager(void);
public:
	~CTileManager(void);
};
