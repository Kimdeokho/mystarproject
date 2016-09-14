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
	vector<const vector<TEXINFO*>*>	m_HighDirtTex;
private:
	int						m_rbidx;
	bool					m_rbRender;
public:
	void ShowGrid(void);
	void SetRohmbusRender(bool _bRender);
	void Rohmbus_Picking(const CPoint&	_pt);
	void Rohmbus_Render(void);
	int	 GetRbIdx(void);
	int	 TileCheck(const int _index , const int _flr);
	void SetTerrain(const int idx);
	void SetTerrain(const int idx , const TILE&	temptile);
public:
	void InitTile(void);
	void TileRender(void);
	void MinimapRender(void);
	void Release(void);
public:
	CTileMgr(void);
	~CTileMgr(void);
};
