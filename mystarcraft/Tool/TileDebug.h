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
	void InitMineralPos();
	void InitGasPos();
	void InitStartBasePos();
public:
	void SetTile(const vector<TILE*>* ptile);
	void SwitchDebug(DEBUG_OPTION eop);
	void SetLR(HILL_DIR _edir);
	void SetDebugGroup(DEBUG_OPTION eop , bool _bdebug);
	void SetDebug_Allinit();
	void Set_TileOption(TILE_OPTION eOption);

public:
	void Preview_Hill(void);
	void Preview_Gas(void);
	void Preview_Mineral(void);
	void Preview_StartBase(void);

	void DebugRender(void);
	void DebugTile_PosSet(void);
	void DebugGroup(void);
	
	void MoveOption_Render(void);
public:
	const D3DXVECTOR2& Get_oddColPos(void);
	const D3DXVECTOR2& Get_evenColPos(void);
public:
	bool Whether_install_Mineral();
	bool Whether_install_Gas();
	bool Whether_install_StartBase();
private:
	const vector<TILE*>*		m_psqTile;
	int					m_DownFloorPos[14];
	int					m_UpFloorPos[6];
	int					m_HillPos[6][6];
	int					m_GasPos[2][4];
	int					m_StartbasePos[3][4];
	int					m_MineralPos[1][2];
	D3DXMATRIX			m_matWorld;
	bool				m_debug_set[DBG_END];
	HILL_DIR			m_eDir;				
	MYCOLOR				m_color;

	D3DXVECTOR2			m_vInstallPos;
public:
	CTileDebug(void);
	~CTileDebug(void);
};
