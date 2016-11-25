#pragma once

#include "Include.h"
#include "Rewind.h"
class CToolView;
class CTileMgr
{
public:
	DECLARE_SINGLETON(CTileMgr)
private:
	vector<TILE*>			m_sqTile;
	vector<D3DXVECTOR2>		m_rbTile;
	CToolView*				m_pToolView;


	LPDIRECT3DTEXTURE9		m_textureMap;
	LPDIRECT3DTEXTURE9		m_newtextureMap;
private:
	vector<const vector<TEXINFO*>*>	m_DirtTex;
	vector<const vector<TEXINFO*>*>	m_HighDirtTex;
	vector<const vector<TEXINFO*>*>	m_WaterTex;
	vector<const vector<TEXINFO*>*>	m_HillTex;
private:
	int						m_rbidx;
	int						m_sqidx;
	bool					m_rbRender;

	int						m_oldidx;
public:
	void MinimapDraw(const int iindex);
	void ShowGrid(void);
	void SetRohmbusRender(bool _bRender);
	void SetBeforeTile(list<TERRAIN_INFO>&	terrain_list, const int idx);
	void Rohmbus_Picking(const CPoint&	_pt);
	void Rohmbus_Render(void);
	int	 GetRbIdx(void);
	int	 GetIdx(void);
	int	 FloorCheck(const int _index , const int _flr);
	void SetTerrain(const int idx , TERRAIN_INFO& pterrain_info , bool _bdelete = true);
	void TileOption_Update(void);	
	bool InstallHillCheck(const int idx, const int isequence, const int idir);
	void CopySurface(LPDIRECT3DTEXTURE9 ptexturemap);
	void Initminimap(HWND h);
	void SetMinimapupdate(void);
public:
	TERRAIN_INFO* GetTerrain_Info(const int _index);
public:
	void InitTile(void);
	void TileRender(void);
	void MinimapRender(void);
	void Release(void);
public:
	CTileMgr(void);
	~CTileMgr(void);
};
