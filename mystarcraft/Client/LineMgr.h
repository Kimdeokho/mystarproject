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
	bool			m_bSwitch;
public:
	void Initialize(void);
	void SetRectPoint(const D3DXVECTOR2& vstart , const D3DXVECTOR2& vend);
	void SetRenderSwitch(bool bswitch);
	void LineRender(void);
	void Select_unit(void);
	void collisionbox_render(const MYRECT<float>& rc);
	void RenderGrid(const int& tilesize , const int& tilecnt);
public:
	CLineMgr(void);
	~CLineMgr(void);
};
