#pragma once

#include "Include.h"

class CToolView;
class CTileMgr
{
public:
	DECLARE_SINGLETON(CTileMgr)
private:
	vector<TILE*>			m_sqTile;
	vector<D3DXVECTOR2>		m_rbTile;
	CToolView*				m_pToolView;
private:
	vector<const vector<TEXINFO*>*>	m_DirtTex;
	vector<vector<TEXINFO*>>	m_HighDirtTex;
public:
	void ShowGrid(void);
public:
	void InitTile(void);
	void TileRender(void);
	void MinimapRender(void);
	void Release(void);
public:
	CTileMgr(void);
	~CTileMgr(void);
};
