#pragma once

#include "Include.h"
class CTileDebug
{
	DECLARE_SINGLETON(CTileDebug)
public:
	void SetTile(const vector<TILE*>* ptile);
	void DebugRender(void);
	void DebugTile_PosSet(void);
private:
	const vector<TILE*>*		m_psqTile;
	int					m_DownFloorPos[10];
	//int					m_UpFloorPos[GROUP_END];
public:
	CTileDebug(void);
	~CTileDebug(void);
};
