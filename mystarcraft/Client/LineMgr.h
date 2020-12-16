#pragma once

#include "Include.h"

typedef struct tagline
{
	D3DXVECTOR2 vpoint[2];
}LINE_INFO;
class CLineMgr
{
	DECLARE_SINGLETON(CLineMgr)
private:
	list<LINE_INFO>	m_lineList;
	LPD3DXLINE		m_pLine;
	D3DXVECTOR2		m_RectLine[5];
	D3DXVECTOR2		m_Rally_Line[2];
	bool			m_bSwitch;
public:
	void Initialize(void);
	void SetRectPoint(const D3DXVECTOR2& vstart , const D3DXVECTOR2& vend);
	void SetRenderSwitch(bool bswitch);
	void SetRallyPoint(const D3DXVECTOR2& vstart , const D3DXVECTOR2& vend);

	void RectLineRender(void);
	void RallyLineRender(void);
	void PathLineRender(const D3DXVECTOR2& vstart , const D3DXVECTOR2& vend , const float fwidth);
	void Select_unit(void);
	void collisionbox_render(const MYRECT<float>& rc);
	void minicambox_render(const MYRECT<float>& rc);
	void RenderGrid(int tilesize , int tilecnt);
public:
	CLineMgr(void);
	~CLineMgr(void);
};
