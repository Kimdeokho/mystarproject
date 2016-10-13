#pragma once

#include "Include.h"
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
	int					m_DownFloorPos[10];
	D3DXMATRIX			m_matWorld;
	//int					m_UpFloorPos[GROUP_END];
	bool				m_bDebugGroup;
	bool				m_bMoveOp;
public:
	CTileDebug(void);
	~CTileDebug(void);
};
