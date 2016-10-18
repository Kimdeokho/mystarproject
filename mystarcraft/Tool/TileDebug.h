#pragma once

#include "Include.h"

typedef struct tagcolor
{
	unsigned char a;
	unsigned char r;
	unsigned char g;
	unsigned char b;
	tagcolor()
	{
		a = 255;
		r = 255;
		g = 255;
		b = 255;
	}
}MYCOLOR;
class CTileDebug
{
	DECLARE_SINGLETON(CTileDebug)
public:
	void SetTile(const vector<TILE*>* ptile);
	void SetDebugGroup();
	void SetMoveOption();

	void DebugRender(void);
	void DebugTile_PosSet(void);
	void DebugGroup(void);
	void MoveOption_Render(void);
private:
	const vector<TILE*>*		m_psqTile;
	int					m_DownFloorPos[14];
	D3DXMATRIX			m_matWorld;
	//int					m_UpFloorPos[GROUP_END];
	bool				m_bDebugGroup;
	bool				m_bMoveOp;
	MYCOLOR				m_color;
public:
	CTileDebug(void);
	~CTileDebug(void);
};
