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
	void SetLR(HILL_DIR _edir);
	void SetDebugGroup(bool _bdebug);

	void DebugRender(void);
	void DebugHillRender(void);
	void DebugTile_PosSet(void);
	void DebugGroup(void);
	void MoveOption_Render(void);
	void DebugSwitch(bool bdebug);
private:
	const vector<TILE*>*		m_psqTile;
	int					m_DownFloorPos[14];
	int					m_UpFloorPos[6];
	int					m_HillPos[6][6];
	D3DXMATRIX			m_matWorld;
	//int					m_UpFloorPos[GROUP_END];
	bool				m_bDebugGroup;
	bool				m_bMoveOp;
	bool				m_hillDebug;
	HILL_DIR			m_eDir;				
	MYCOLOR				m_color;
public:
	CTileDebug(void);
	~CTileDebug(void);
};
