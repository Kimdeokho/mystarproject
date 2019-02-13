#pragma once

#include "Include.h"

class CDebug_Mgr
{
	DECLARE_SINGLETON(CDebug_Mgr)
public:
	enum DBG
	{
		DBG_COLLBOX,
		DBG_FOG,
		DBG_TILEPOP,
		DBG_PATHFIND,
		DBG_FLOWPATH,
		DBG_END,
	};
	static	bool	m_dbglist[DBG_END];
private:
	FOG_INFO		m_oldfog[SQ_TILECNTX*SQ_TILECNTY];
public:
	void	Update(void);
	void	DBG_Fog_on(void);
	void	DBG_Fog_off(void);
public:
	CDebug_Mgr(void);
	~CDebug_Mgr(void);
};
