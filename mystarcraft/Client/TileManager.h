#pragma once

#include "Include.h"

template<typename T>class CMyHeapSort;
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

	const vector<TEXINFO*>*	m_fogtexvec;
	vector<int>				m_minifog_onidx;
	vector<int>				m_minifog_offidx;
private:
	LPD3DXSPRITE				m_pSprite;
	D3DCOLOR					m_TileColor;
	LPDIRECT3DTEXTURE9			m_MapTexture[7][6];
	LPDIRECT3DTEXTURE9			m_MinimapTexture;
	LPDIRECT3DTEXTURE9			m_MinifogTexture;


	//LPDIRECT3DTEXTURE9			m_fogTexture;
	//LPDIRECT3DTEXTURE9			m_fogMaskTexture;
private:
	D3DXMATRIX					m_matWorld;
	D3DXMATRIX					m_fogmat;
	D3DXVECTOR3					m_vTileCenter;
	int							m_mapsize;
private:
	const vector<TEXINFO*>*	m_DirtTex[GROUP_END];
	const vector<TEXINFO*>*	m_HighDirtTex[GROUP_END];
	const vector<TEXINFO*>*	m_WaterTex[GROUP_END];
	const vector<TEXINFO*>*	m_HillTex[GROUP_END];




	int									m_icuridx;
	D3DXVECTOR2							m_vcurPos;
	//LPDIRECT3DSURFACE9 pSurfacefog , pSurfacemask;
	//D3DSURFACE_DESC fogDesc , pmaskDesc;
	//D3DLOCKED_RECT plockedRect , pmaskrect;
	//DWORD* fogimgData;
	//DWORD* maskimgData;

/*다음으로 갈 인덱스, 비용만 필요 검사했는지 확인용 bool*/
	CMyHeapSort<FLOW_NODE*>*	m_heapsort;
	FLOW_NODE*	m_flownode[SQ_TILECNTX*SQ_TILECNTY];
	short		m_flowdestidx[SQ_TILECNTX*SQ_TILECNTY];
	int			m_eight_idx[ASTAR_DIR_END];
	float		m_fTimeTest;
	int			m_flowfield_goalidx;
	D3DXVECTOR2	m_flowfield_goalpos;
public:
	void Initialize(void);
	void Init_eightidx(const int& idx);
public:
	void Flowfield_Pathfinding(void);
public:
	void MinifogUpdate(void);
public:
	bool	GetFogLight(const int& idx);
	BYTE	GetTileOption(const int& idx);
	bool	GetCreepInstall(const int& idx);
	bool	GetFogSearch(const int& idx);
	TILE**	GetSqTile(void);
	CREEP_INFO**	GetCreepTile(void);
	void	GetFlowfield_Path(const int& idx , vector<int>& path);
	short*   Get_flowfield_node(void);
	LPDIRECT3DTEXTURE9 GetMiniampTexture(void);
	LPDIRECT3DTEXTURE9 GetMiniFogmapTexture(void);
	//int				GetFlowFiled_Goalidx(void);
	//D3DXVECTOR2		GetFlowFiled_GoalPos(void);
public:
	void SetFogSquence(int idx , unsigned short sequence);
	void SetFogLight(int idx, float fdistance , float fradius);
	void SetFogColor(const int& idx , D3DCOLOR color);
	void SetCreepInstall(const int& idx , bool binstall);

	void SetFogoverlap_cnt(const int& idx);
	void SetFogSearch(const int& idx,bool bsearch);
public:
	bool CheckFogFloor(int myidx , int destidx);
	bool CheckCreepFloor(int myidx , int destidx);
public:
	//void CreepAlgorithm(void);
public:	
	void SightOffRender(const int& idx);
	void SightOnRender(const int& idx ,const int& irange , list<int>& sightoff_list , bool* fogsearch , MOVE_TYPE etype);

	//void Creep_increase(const D3DXVECTOR2& vPos/*유닛의 위치*/ , int irange);
	void Creep_decrease_autotile(const int& idx);
	void Creep_Autotile(const int& idx);
public:
	void Bresenham_fog(const D3DXVECTOR2& vStart ,const D3DXVECTOR2& vDest, const int fRadius ,list<int>& light_IdxList , bool* fogsearch , MOVE_TYPE etype);
	void Bresenham_Creep(const D3DXVECTOR2& vStart ,const D3DXVECTOR2& vDest, const int& fRadius ,const int& loopcnt,list<int>& creep_IdxList);
public:
	void ReadyTileTexture(void);
	void ReadyMainMap(void);
	void ReadyMiniMap(void);
public:
	void CopySurface(LPDIRECT3DTEXTURE9 psurface);
public:
	void RenderTile(void);
	void RenderFog(void);
	void RenderCreep(void);
	void Render_Flowfield(void);
public:
	void Release(void);
public:
	void LoadTileData(HANDLE hFile);
private:
	CTileManager(void);
public:
	~CTileManager(void);
};
