#pragma once

#include "Include.h"
#include "Rewind.h"
class CToolView;
class CTileMgr
{
public:
	DECLARE_SINGLETON(CTileMgr)
private:
	vector<TILE*>				m_sqTile;
	vector<list<TERRAIN_INFO*>>	m_terrainInfo_List;
	vector<D3DXVECTOR2>			m_rbTile;
	CToolView*					m_pToolView;


	LPDIRECT3DTEXTURE9		m_textureMap;
	LPDIRECT3DTEXTURE9		m_newtextureMap;
private:
	vector<const vector<TEXINFO*>*>	m_DirtTex;
	vector<const vector<TEXINFO*>*>	m_HighDirtTex;
	vector<const vector<TEXINFO*>*>	m_WaterTex;
	vector<const vector<TEXINFO*>*>	m_HillTex;
private:
	bool					m_rbRender;
	int						m_rbidx;
	int						m_sqidx;
	int						m_oldidx;
	D3DXVECTOR2				m_minimaRatio;
	CPoint					m_minimappt;
public:
	void InitTile(void);
	void Initminimap(void);
public:	
	void TileRender(void);
	void MinimapRender(void);
	void Rohmbus_Render(void);
	void ShowGrid(void);
	void MinimapDraw(const int iindex);
	void MinimapSquare();
public:
	void SetRohmbusRender(bool _bRender);
	void SetBeforeTile(list<TERRAIN_INFO>&	terrain_list, const int idx);
	void SetTerrain(const int idx , TERRAIN_INFO& pterrain_info , bool _bdelete = true);
	void SetMinimapupdate(void);
	void SetTileOption(const int idx , TILE_OPTION eoption);
	void SetMinimapPoint(const CPoint& pt);
public:
	int	 FloorCheck(const int _index , const int _flr);
	bool InstallHillCheck(const int idx, const int isequence, const int idir);
	bool InstallResourceCheck(const int idx);
public:
	int	 GetRbIdx(void);
	int	 GetsqIdx(void);
	TERRAIN_INFO* GetTerrain_Info(const int _index);
	int			  GetTileOption(const int _index);
public:	
	void Rohmbus_Picking(const CPoint&	_pt);		
	void TileOption_Update(void);		
	void CopySurface(LPDIRECT3DTEXTURE9 ptexturemap);		
public:
	void SaveTile(HANDLE h);
	void LoadTile(HANDLE h);
public:
	void Release(void);
public:
	CTileMgr(void);
	~CTileMgr(void);
};
